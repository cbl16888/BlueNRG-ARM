/******************** (C) COPYRIGHT 2015 STMicroelectronics ********************
 * File Name          : sensor.c
 * Author             : AMS - VMA RF Application team
 * Version            : V1.0.0
 * Date               : 23-November-2015
 * Description        : Sensor init and sensor state machines
 ********************************************************************************
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
 * AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
 * INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
 * CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
 * INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "BlueNRG1_it.h"
#include "BlueNRG1_conf.h"
#include "ble_const.h" 
#include "bluenrg1_stack.h"
#include "gp_timer.h"
#include "SDK_EVAL_Config.h"
#include "gatt_db.h"
#include "app_state.h"

//#define SENSOR_EMULATION 1

#ifndef SENSOR_EMULATION
#include "LPS25HB.h"
#include "lsm6ds3.h"
#include "lsm6ds3_reg.h"
#include "lsm6ds3_hal.h"
#include "gatt_db.h"
#include "bluenrg1_events.h"
#endif
#include "OTA_btl.h"   
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/


#ifndef DEBUG
#define DEBUG 1
#endif

#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

#define UPDATE_CONN_PARAM 1
#define  ADV_INTERVAL_MIN_MS  100
#define  ADV_INTERVAL_MAX_MS  120

#define BLE_SENSOR_VERSION_STRING "1.1.0" 

#define TX_OCTETS_TO_TIME(OCTET)    ((OCTET + 14) * 8)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint8_t set_connectable = 1;
uint16_t connection_handle = 0;
uint8_t connInfo[20];
uint8_t buffer[18];
uint16_t level;
BOOL sensor_board = FALSE; // It is True if sensor boad has been detected

int connected = FALSE;
#define UPDATE_TIMER 2 //TBR
int l2cap_request_sent = FALSE;
int l2cap_request_accepted = FALSE;
static uint8_t l2cap_req_timer_expired = FALSE; 


#ifndef SENSOR_EMULATION
PRESSURE_DrvTypeDef* xLPS25HBDrv = &LPS25HBDrv;  
IMU_6AXES_DrvTypeDef *Imu6AxesDrv = NULL;
LSM6DS3_DrvExtTypeDef *Imu6AxesDrvExt = NULL;
static AxesRaw_t acceleration_data; 
static AxesRaw_t gyroscope_data;  //Add variable to support the gyroscope
struct DataSet_t FIFO_data[250];
struct DataSet_t * write_ptr = &FIFO_data[0];
struct DataSet_t * send_ptr = &FIFO_data[0];
#endif 

uint8_t start_request= FALSE;
volatile uint8_t request_free_fall_notify = FALSE; 
extern uint8_t request_fifo_full_notify;
extern uint8_t request_fifo_full_read;
extern uint16_t accCharHandle, startCharHandle;
extern int i;
extern volatile int app_flags;
uint16_t available_buffers;

  
/* Private function prototypes -----------------------------------------------*/
void Data_Read(void);
int32_t platform_write(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len);
int32_t platform_read(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len);
IMU_6AXES_StatusTypeDef    LSM6DS3_X_GetSensitivity( float *pfData );
IMU_6AXES_StatusTypeDef    LSM6DS3_G_GetSensitivity( float *pfData );
extern void MicroSecondDelay(uint32_t delay);
/* Private functions ---------------------------------------------------------*/

#ifndef SENSOR_EMULATION
/*******************************************************************************
 * Function Name  : Init_Accelerometer.
 * Description    : Init LIS331DLH accelerometer.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void Init_Accelerometer(void)
{
  /* LSM6DS3 library setting */
  IMU_6AXES_InitTypeDef InitStructure;
  uint8_t tmp1 = 0x00;
  
  Imu6AxesDrv = &LSM6DS3Drv;
  Imu6AxesDrvExt = &LSM6DS3Drv_ext_internal;
  InitStructure.G_FullScale      = 125.0f;
  InitStructure.G_OutputDataRate = 13.0f;
  InitStructure.G_X_Axis         = 1;
  InitStructure.G_Y_Axis         = 1;
  InitStructure.G_Z_Axis         = 1;
  InitStructure.X_FullScale      = 2.0f;
  InitStructure.X_OutputDataRate = 13.0f;
  InitStructure.X_X_Axis         = 1;
  InitStructure.X_Y_Axis         = 1;
  InitStructure.X_Z_Axis         = 1;  
  
  /* LSM6DS3 initiliazation */
  Imu6AxesDrv->Init(&InitStructure);
    
  /* Disable all mems IRQs in order to enable free fall detection */ //TBR
  LSM6DS3_IO_Write(&tmp1, LSM6DS3_XG_MEMS_ADDRESS, LSM6DS3_XG_INT1_CTRL, 1);
  
  /* Clear first previous data */
  Imu6AxesDrv->Get_X_Axes((int32_t *)&acceleration_data);
  Imu6AxesDrv->Get_G_Axes((int32_t *)&gyroscope_data);
  
  /* Enable Free fall detection */
  Imu6AxesDrvExt->Enable_Free_Fall_Detection(); 
}
#endif
/*******************************************************************************
 * Function Name  : Sensor_DeviceInit.
 * Description    : Init the device sensors.
 * Input          : None.
 * Return         : Status.
 *******************************************************************************/
uint8_t Sensor_DeviceInit()
{
  uint8_t bdaddr[] = {0x12, 0x34, 0x00, 0xE1, 0x80, 0x02};
  uint8_t ret;
  uint16_t service_handle, dev_name_char_handle, appearance_char_handle;
  uint8_t device_name[] = {'B', 'l', 'u', 'e', 'N', 'R', 'G'};

  /* Set the device public address */
  ret = aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET, CONFIG_DATA_PUBADDR_LEN,
                                  bdaddr);  
  if(ret != BLE_STATUS_SUCCESS) {
    PRINTF("aci_hal_write_config_data() failed: 0x%02x\r\n", ret);
    return ret;
  }
  
  /* Set the TX power 8 dBm */
  aci_hal_set_tx_power_level(1, 7);
  
  /* GATT Init */
  ret = aci_gatt_init();
  if (ret != BLE_STATUS_SUCCESS) {
    PRINTF("aci_gatt_init() failed: 0x%02x\r\n", ret);
    return ret;
  }
  
  /* GAP Init */
  ret = aci_gap_init(GAP_PERIPHERAL_ROLE, 0, 0x08, &service_handle, &dev_name_char_handle, &appearance_char_handle);
  if (ret != BLE_STATUS_SUCCESS) {
    PRINTF("aci_gap_init() failed: 0x%02x\r\n", ret);
    return ret;
  }
 
  /* Update device name */
  ret = aci_gatt_update_char_value_ext(0, service_handle, dev_name_char_handle, 0,sizeof(device_name), 0, sizeof(device_name), device_name);
  if(ret != BLE_STATUS_SUCCESS) {
    PRINTF("aci_gatt_update_char_value_ext() failed: 0x%02x\r\n", ret);
    return ret;
  }
  
//  ret = aci_gap_set_authentication_requirement(BONDING,
//                                               MITM_PROTECTION_REQUIRED,
//                                               SC_IS_NOT_SUPPORTED,
//                                               KEYPRESS_IS_NOT_SUPPORTED,
//                                               7, 
//                                               16,
//                                               USE_FIXED_PIN_FOR_PAIRING,
//                                               123456,
//                                               0x00);
//  if(ret != BLE_STATUS_SUCCESS) {
//    PRINTF("aci_gap_set_authentication_requirement()failed: 0x%02x\r\n", ret);
//    return ret;
//  } 
  
  PRINTF("BLE Stack Initialized with SUCCESS\n");

#ifndef SENSOR_EMULATION /* User Real sensors */
  Init_Accelerometer();
#endif   


  /* Add ACC service and Characteristics */
  ret = Add_Acc_Service();
  if(ret == BLE_STATUS_SUCCESS) {
    PRINTF("Acceleration service added successfully.\n");
  } else {
    PRINTF("Error while adding Acceleration service: 0x%02x\r\n", ret);
    return ret;
  }

#if ST_OTA_FIRMWARE_UPGRADE_SUPPORT     
  ret = OTA_Add_Btl_Service();
  if(ret == BLE_STATUS_SUCCESS)
    PRINTF("OTA service added successfully.\n");
  else
    PRINTF("Error while adding OTA service.\n");
#endif /* ST_OTA_FIRMWARE_UPGRADE_SUPPORT */ 

  return BLE_STATUS_SUCCESS;
}

/*******************************************************************************
 * Function Name  : Set_DeviceConnectable.
 * Description    : Puts the device in connectable mode.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void Set_DeviceConnectable(void)
{  
  uint8_t ret;
  uint8_t local_name[] = {AD_TYPE_COMPLETE_LOCAL_NAME,'B','l','u','e','N','R','G'}; 

#if ST_OTA_FIRMWARE_UPGRADE_SUPPORT
  hci_le_set_scan_response_data(18,BTLServiceUUID4Scan); 
#else
  hci_le_set_scan_response_data(0,NULL);
#endif /* ST_OTA_FIRMWARE_UPGRADE_SUPPORT */ 
  PRINTF("Set General Discoverable Mode.\n");
  
  ret = aci_gap_set_discoverable(ADV_IND,
                                (ADV_INTERVAL_MIN_MS*1000)/625,(ADV_INTERVAL_MAX_MS*1000)/625,
                                 STATIC_RANDOM_ADDR, NO_WHITE_LIST_USE,
                                 sizeof(local_name), local_name, 0, NULL, 0x06, 0x06); 
  if(ret != BLE_STATUS_SUCCESS)
  {
    PRINTF("aci_gap_set_discoverable() failed: 0x%02x\r\n",ret);
    SdkEvalLedOn(LED3);  
  }
  else
    PRINTF("aci_gap_set_discoverable() --> SUCCESS\r\n");
}

/*******************************************************************************
 * Function Name  : APP_Tick.
 * Description    : Sensor Demo state machine.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
 void APP_Tick(void)
{
  /* Make the device discoverable */
	if(APP_FLAG(SET_CONNECTABLE))
  {
    Set_DeviceConnectable();
    APP_FLAG_CLEAR(SET_CONNECTABLE);
  }

#if UPDATE_CONN_PARAM      
  /* Connection parameter update request */
  if(APP_FLAG(CONNECTED) && !APP_FLAG(L2CAP_PARAM_UPD_SENT) && l2cap_req_timer_expired){
    aci_l2cap_connection_parameter_update_req(connection_handle, 6, 10, 20, 1000);
    aci_gatt_exchange_config(connection_handle);
    APP_FLAG_SET(L2CAP_PARAM_UPD_SENT);
  }
#endif

  if(APP_FLAG(CONNECTED) && !APP_FLAG(TX_BUFFER_FULL) && APP_FLAG(FIFO_NOTIFY)) {
    FIFO_Notify();
  }
}

/* ***************** BlueNRG-1 Stack Callbacks ********************************/

/*******************************************************************************
 * Function Name  : hci_le_connection_complete_event.
 * Description    : This event indicates that a new connection has been created.
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/
void hci_le_connection_complete_event(uint8_t Status,
                                      uint16_t Connection_Handle,
                                      uint8_t Role,
                                      uint8_t Peer_Address_Type,
                                      uint8_t Peer_Address[6],
                                      uint16_t Conn_Interval,
                                      uint16_t Conn_Latency,
                                      uint16_t Supervision_Timeout,
                                      uint8_t Master_Clock_Accuracy)
{
  connection_handle = Connection_Handle;
  APP_FLAG_SET(CONNECTED);
	
	/*Connection update */
	#if UPDATE_CONN_PARAM    
		l2cap_request_sent = FALSE;
		HAL_VTimerStart_ms(UPDATE_TIMER, CLOCK_SECOND*2);
		{
			l2cap_req_timer_expired = FALSE;
		}
	#endif
	
  PRINTF("Device connected \n");
    
}/* end hci_le_connection_complete_event() */

void hci_le_data_length_change_event(uint16_t Connection_Handle,
                                     uint16_t MaxTxOctets,
                                     uint16_t MaxTxTime,
                                     uint16_t MaxRxOctets,
                                     uint16_t MaxRxTime)
{
   PRINTF("Data length extension enabled MaxTxOctets %u MaxTxTime %u MaxRxOctets %u MaxRxTime %u \n", MaxTxOctets, MaxTxTime, MaxRxOctets, MaxRxTime);
}

void aci_l2cap_connection_update_resp_event(uint16_t Connection_Handle,
                                            uint16_t Result){
	if(Result==0)
	{
		PRINTF("Connection update accepted \n");
		//SdkEvalLedOn(LED2);
		l2cap_request_accepted=TRUE;
	}else{
		PRINTF("Connection update refused \n");
		//SdkEvalLedOff(LED2);
		l2cap_request_accepted=FALSE;
	}
}

void aci_att_exchange_mtu_resp_event(uint16_t Connection_Handle, uint16_t Server_RX_MTU){
	PRINTF("The agreed ATT MTU size is %u \n", Server_RX_MTU);
}
/*******************************************************************************
 * Function Name  : hci_disconnection_complete_event.
 * Description    : This event occurs when a connection is terminated.
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/
void hci_disconnection_complete_event(uint8_t Status,
                                      uint16_t Connection_Handle,
                                      uint8_t Reason)
{
  /* Make the device connectable again. */
	APP_FLAG_CLEAR(CONNECTED);
  APP_FLAG_SET(SET_CONNECTABLE);
  APP_FLAG_CLEAR(NOTIFICATIONS_ENABLED);
  APP_FLAG_CLEAR(TX_BUFFER_FULL);
	APP_FLAG_CLEAR(L2CAP_PARAM_UPD_SENT);
	/* Restart pointers*/
	write_ptr = send_ptr = &FIFO_data[0];
	/*Stop the current timer*/
	HAL_VTimer_Stop(0);
	
  APP_FLAG_CLEAR(START_READ_TX_CHAR_HANDLE);
  APP_FLAG_CLEAR(END_READ_TX_CHAR_HANDLE);
  APP_FLAG_CLEAR(START_READ_RX_CHAR_HANDLE); 
  APP_FLAG_CLEAR(END_READ_RX_CHAR_HANDLE);
  PRINTF("Device disconnected \n");
#if ST_OTA_FIRMWARE_UPGRADE_SUPPORT
  OTA_terminate_connection();
#endif 
}/* end hci_disconnection_complete_event() */

/**
  * @brief Each time BLE FW stack raises the error code @ref ble_status_insufficient_resources (0x64),
the @ref aci_gatt_tx_pool_available_event event is generated as soon as the available buffer size
is greater than maximum ATT MTU (on stack versions below v2.1 this event is generated when at least 2 packets
with MTU of 23 bytes are available).
  * @param Connection_Handle Connection handle related to the request
  * @param Available_Buffers Not used.
  * @retval None
*/
void aci_gatt_tx_pool_available_event(uint16_t Connection_Handle,
                                      uint16_t Available_Buffers){
	APP_FLAG_CLEAR(TX_BUFFER_FULL);
}
/*******************************************************************************
 * Function Name  : aci_gatt_read_permit_req_event.
 * Description    : This event is given when a read request is received
 *                  by the server from the client.
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/
void aci_gatt_read_permit_req_event(uint16_t Connection_Handle,
                                    uint16_t Attribute_Handle,
                                    uint16_t Offset)
{
  Read_Request_CB(Attribute_Handle);    
}

/*******************************************************************************
 * Function Name  : HAL_VTimerTimeoutCallback.
 * Description    : This function will be called on the expiry of 
 *                  a one-shot virtual timer.
 * Input          : See file bluenrg1_stack.h
 * Output         : See file bluenrg1_stack.h
 * Return         : See file bluenrg1_stack.h
 *******************************************************************************/
void HAL_VTimerTimeoutCallback(uint8_t timerNum)
{
#if UPDATE_CONN_PARAM
  if (timerNum == UPDATE_TIMER) {
    l2cap_req_timer_expired = TRUE;
  }
#endif
	if (timerNum == 0) {
		/* Wait ~800 us to get 20ms */
		MicroSecondDelay(750);
		/* Start a new timer */
		HAL_VTimerStart_ms(0, 19); //Timer expires after 19ms (50Hz)
		/* Retrieve data */
		Data_Read();
	}
}

/*******************************************************************************
 * Function Name  : aci_gatt_attribute_modified_event.
 * Description    : This event occurs when an attribute is modified.
 * Input          : See file bluenrg1_events.h
 * Output         : See file bluenrg1_events.h
 * Return         : See file bluenrg1_events.h
 *******************************************************************************/
void aci_gatt_attribute_modified_event(uint16_t Connection_Handle,
                                       uint16_t Attr_Handle,
                                       uint16_t Offset,
                                       uint16_t Attr_Data_Length,
                                       uint8_t Attr_Data[])
{
		lsm6ds3_ctx_t dev_ctx;
	  dev_ctx.write_reg = platform_write;
	  dev_ctx.read_reg = platform_read;
	if(Attr_Handle == accCharHandle + 2){ //For notifications
		if(Attr_Data[0]==0x01){
				APP_FLAG_SET(NOTIFICATIONS_ENABLED);
				/*Start Timer*/
				HAL_VTimerStart_ms(0, 19); //Timer expires after 19ms (50Hz)
				/*Empty LSM6DS3 FIFO */
				lsm6ds3_fifo_mode_set(&dev_ctx, LSM6DS3_BYPASS_MODE);
				Clock_Wait(1);
				lsm6ds3_fifo_mode_set(&dev_ctx, LSM6DS3_STREAM_MODE);
				/*Reset timestamp */
				lsm6ds3_timestamp_rst_set(&dev_ctx);
				/* Retrieve data */
				Data_Read();
		}
		
	}
	if(Attr_Handle == startCharHandle + 1){ //For sync request
		if(Attr_Data[0]==0x01){
			/*Stop Timer and restart it*/
			HAL_VTimer_Stop(0);
			HAL_VTimerStart_ms(0, 19); //Timer expires after 20ms (50Hz)
			/*Clear FIFO level and stop notifications of old data*/
			level=0;
			i=0;
			APP_FLAG_CLEAR(FIFO_NOTIFY);
			/*Empty LSM6DS3 FIFO */
			lsm6ds3_fifo_mode_set(&dev_ctx, LSM6DS3_BYPASS_MODE);
			Clock_Wait(1);
			lsm6ds3_fifo_mode_set(&dev_ctx, LSM6DS3_STREAM_MODE);
			/*Reset timestamp */
			lsm6ds3_timestamp_rst_set(&dev_ctx);
			/* Retrieve data */
			Data_Read();
		}
	}

#if ST_OTA_FIRMWARE_UPGRADE_SUPPORT
  OTA_Write_Request_CB(Connection_Handle, Attr_Handle, Attr_Data_Length, Attr_Data);
#endif /* ST_OTA_FIRMWARE_UPGRADE_SUPPORT */ 
}


void aci_hal_end_of_radio_activity_event(uint8_t Last_State,
                                         uint8_t Next_State,
                                         uint32_t Next_State_SysTime)
{
#if ST_OTA_FIRMWARE_UPGRADE_SUPPORT
  if (Next_State == 0x02) /* 0x02: Connection event slave */
  {
    OTA_Radio_Activity(Next_State_SysTime);  
  }
#endif 
}
/*******************************************************************************
 * Function Name  : Data_Read.
 * Description    : This function will be called once the FIFO watermark level is
 *                 reached. This function reads all the data stored in the FIFO
 * Input          : None
 * Output         : Updated data structure
 * Return         : None
 *******************************************************************************/
void Data_Read(void){
	uint16_t unsent =0;
	/* Retrieve the most recent samples */
	LSM6DS3_IO_Read(&buffer[0], LSM6DS3_XG_MEMS_ADDRESS, LSM6DS3_XG_OUT_X_L_XL, 6);
	LSM6DS3_IO_Read(&buffer[6], LSM6DS3_XG_MEMS_ADDRESS, LSM6DS3_XG_OUT_X_L_G, 6);
	LSM6DS3_IO_Read(&buffer[12], LSM6DS3_XG_MEMS_ADDRESS, LSM6DS3_XG_TIMESTAMP0_REG, 3);
	LSM6DS3_IO_Read(&buffer[15], LSM6DS3_XG_MEMS_ADDRESS, LSM6DS3_XG_STEP_COUNTER_L, 2);

	/* ACM data */
	write_ptr->AXIS_X=(int16_t)(((int16_t)buffer[0] | (int16_t)buffer[1]<<8));
	write_ptr->AXIS_Y=(int16_t)(((int16_t)buffer[2] | (int16_t)buffer[3]<<8));
	write_ptr->AXIS_Z=(int16_t)(((int16_t)buffer[4] | (int16_t)buffer[5]<<8));
	
	/* Gyro data */
	write_ptr->AXIS_GX=(int16_t)(((int16_t)buffer[6] | (int16_t)buffer[7]<<8));
	write_ptr->AXIS_GY=(int16_t)(((int16_t)buffer[8] | (int16_t)buffer[9]<<8));
	write_ptr->AXIS_GZ=(int16_t)(((int16_t)buffer[10] | (int16_t)buffer[11]<<8));
	
	/* Timestamp and pedometer */
	write_ptr->TIMESTAMP=(((uint32_t)buffer[14])<<16) | (((uint32_t)buffer[13])<<8) | ((uint32_t)buffer[12]);
	write_ptr->PEDOMETER=(uint16_t)buffer[15] | (uint16_t)buffer[16]<<8;
	
	write_ptr++;
	
	if(write_ptr==&FIFO_data[250])
		write_ptr=&FIFO_data[0];
	
	/* Set flag to notify */
	if((write_ptr - send_ptr) < 250 && (write_ptr - send_ptr) > 0){
		unsent = (write_ptr - send_ptr);
	}else{
		unsent = (write_ptr - send_ptr) + 250;
	}

	if(unsent > 4){
		APP_FLAG_SET(FIFO_NOTIFY);
	}
}
















