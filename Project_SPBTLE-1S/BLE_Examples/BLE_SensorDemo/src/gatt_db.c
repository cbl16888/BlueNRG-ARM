/******************** (C) COPYRIGHT 2015 STMicroelectronics ********************
* File Name          : gatt_db.c
* Author             : BlueNRG-1 main file for Chat demo
* Version            : V1.0.0
* Date               : 16-September-2015
* Description        : Functions to build GATT DB and handle GATT events.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "BlueNRG1_it.h"
#include "BlueNRG1_conf.h"
#include "ble_const.h" 
#include "bluenrg1_stack.h"
#include "clock.h"
#include "gp_timer.h"
#include "gatt_db.h"
#include "osal.h"
#include "SDK_EVAL_Config.h"
#include "app_state.h"
#include "LPS25HB.h"
#include "lsm6ds3.h"


#ifndef DEBUG
#define DEBUG 1
#endif

//#define SENSOR_EMULATION 1

#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif


#define COPY_UUID_128(uuid_struct, uuid_15, uuid_14, uuid_13, uuid_12, uuid_11, uuid_10, uuid_9, uuid_8, uuid_7, uuid_6, uuid_5, uuid_4, uuid_3, uuid_2, uuid_1, uuid_0) \
do {\
    uuid_struct[0] = uuid_0; uuid_struct[1] = uuid_1; uuid_struct[2] = uuid_2; uuid_struct[3] = uuid_3; \
        uuid_struct[4] = uuid_4; uuid_struct[5] = uuid_5; uuid_struct[6] = uuid_6; uuid_struct[7] = uuid_7; \
            uuid_struct[8] = uuid_8; uuid_struct[9] = uuid_9; uuid_struct[10] = uuid_10; uuid_struct[11] = uuid_11; \
                uuid_struct[12] = uuid_12; uuid_struct[13] = uuid_13; uuid_struct[14] = uuid_14; uuid_struct[15] = uuid_15; \
}while(0)


#define COPY_ACC_SERVICE_UUID(uuid_struct)  COPY_UUID_128(uuid_struct,0x02,0x36,0x6e,0x80, 0xcf,0x3a, 0x11,0xe1, 0x9a,0xb4, 0x00,0x02,0xa5,0xd5,0xc5,0x1b)
#define COPY_ACC_UUID(uuid_struct)          COPY_UUID_128(uuid_struct,0x34,0x0a,0x1b,0x80, 0xcf,0x4b, 0x11,0xe1, 0xac,0x36, 0x00,0x02,0xa5,0xd5,0xc5,0x1b)
#define COPY_GYR_UUID(uuid_struct)          COPY_UUID_128(uuid_struct,0x02,0xf7,0x09,0x0f, 0xcf,0x8a, 0x11,0xe1, 0xdc,0xa9, 0x00,0x02,0xa5,0xd5,0xc5,0x1b)
#define COPY_TIM_UUID(uuid_struct)          COPY_UUID_128(uuid_struct,0xe2,0x3e,0x78,0xa0, 0xcf,0x4a, 0x11,0xe1, 0x8f,0xfc, 0x00,0x02,0xa5,0xd5,0xc5,0x1b)
#define COPY_PED_UUID(uuid_struct)          COPY_UUID_128(uuid_struct,0x78,0xc0,0x8b,0x3e, 0xcf,0x0f, 0x11,0xe1, 0xb2,0x10, 0x00,0x02,0xa5,0xd5,0xc5,0x1b)
#define COPY_START_UUID(uuid_struct)        COPY_UUID_128(uuid_struct,0x2c,0x41,0xcc,0x24, 0xcf,0x13, 0x11,0xe1, 0x4f,0xdf, 0x00,0x02,0xa5,0xd5,0xc5,0x1b)


uint16_t accServHandle, freeFallCharHandle, accCharHandle, gyrCharHandle, timCharHandle, pedCharHandle, startCharHandle;
uint16_t envSensServHandle, tempCharHandle, pressCharHandle, humidityCharHandle;

/* UUIDS */
Service_UUID_t service_uuid;
Char_UUID_t char_uuid;
Char_Desc_Uuid_t char_desc_uuid;
int i=0;
extern uint8_t request_fifo_full_notify;
extern uint8_t request_fifo_full_read;
extern uint16_t connection_handle;
extern BOOL sensor_board;
extern uint16_t level;
#ifndef SENSOR_EMULATION
extern IMU_6AXES_DrvTypeDef *Imu6AxesDrv;
extern LSM6DS3_DrvExtTypeDef *Imu6AxesDrvExt;
extern struct DataSet_t FIFO_data[30];
#endif

IMU_6AXES_StatusTypeDef GetAccAxesRaw(AxesRaw_t * acceleration_data, AxesRaw_t * gyroscope_data)
{
  IMU_6AXES_StatusTypeDef status = IMU_6AXES_OK;
  
#ifdef SENSOR_EMULATION
  acceleration_data->AXIS_X = ((uint64_t)rand()) % X_OFFSET;
  acceleration_data->AXIS_Y = ((uint64_t)rand()) % Y_OFFSET; 
  acceleration_data->AXIS_Z = ((uint64_t)rand()) % Z_OFFSET;
  gyroscope_data->AXIS_X = ((uint64_t)rand()) % X_OFFSET;
  gyroscope_data->AXIS_Y = ((uint64_t)rand()) % Y_OFFSET;
  gyroscope_data->AXIS_Z = ((uint64_t)rand()) % Z_OFFSET;
#else
  status = Imu6AxesDrv->Get_X_Axes((int32_t *)acceleration_data);
  status = Imu6AxesDrv->Get_G_Axes((int32_t *)gyroscope_data);
#endif 
  return status;
}

void GetFreeFallStatus(void)
{
#ifndef SENSOR_EMULATION
  uint8_t free_fall_status;
  
  /* Set the IRQ flag */
  Imu6AxesDrvExt->Get_Status_Free_Fall_Detection(&free_fall_status);
  if (free_fall_status == 1)
  {
    request_free_fall_notify = TRUE;
  }  
#endif
}
  
/*******************************************************************************
* Function Name  : Add_Chat_Service
* Description    : Add the 'Accelerometer' service.
* Input          : None
* Return         : Status.
*******************************************************************************/
tBleStatus Add_Acc_Service(void)
{
  tBleStatus ret;
  uint8_t uuid[16];
  /* Lors de l'ajout d'un attribut, il faut modifier trois #define dans SensorDemo_config.h: ATT_VALUE_ARRAY_SIZE, MAX_NUMBER_ATTRIBUTES_RECORDS_SERVICE_1 et NUM_APP_GATT_ATTRIBUTES*/
  COPY_ACC_SERVICE_UUID(uuid); //copy le UUID correspondant au service d'ACC dans la variable uuid
    
  Osal_MemCpy(&service_uuid.Service_UUID_128, uuid, 16); //copy de la valeur du UUID de ACC vers la variable service_uuid de type Service_UUID_t_s qui puisse être utilisé par les fonctions suivantes
  ret = aci_gatt_add_service(UUID_TYPE_128,  &service_uuid, PRIMARY_SERVICE, 6, &accServHandle); //creation du service avec le UUID, le type de UUID, le handle du service et le nombre de char contenu dans le service
  if (ret != BLE_STATUS_SUCCESS) goto fail;    

  COPY_ACC_UUID(uuid);  
  Osal_MemCpy(&char_uuid.Char_UUID_128, uuid, 16);
  ret =  aci_gatt_add_char(accServHandle, UUID_TYPE_128, &char_uuid, 18, CHAR_PROP_NOTIFY, ATTR_PERMISSION_NONE, 0,
                           16, 0, &accCharHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;

  COPY_START_UUID(uuid);
  Osal_MemCpy(&char_uuid.Char_UUID_128, uuid, 16);
  ret =  aci_gatt_add_char(accServHandle, UUID_TYPE_128, &char_uuid, 1, CHAR_PROP_WRITE_WITHOUT_RESP, ATTR_PERMISSION_NONE, GATT_NOTIFY_ATTRIBUTE_WRITE ,
                           16, 0, &startCharHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;

  PRINTF("Service ACC added. Handle 0x%04X, Acc Charac handle: 0x%04X, Start Charac handle: 0x%04X", accServHandle,  accCharHandle, startCharHandle);
  return BLE_STATUS_SUCCESS; 

 fail:
  PRINTF("Error while adding ACC service.\n");
  return BLE_STATUS_ERROR ;
    
}

/*******************************************************************************
* Function Name  : Free_Fall_Notify
* Description    : Send a notification for a Free Fall detection.
* Input          : None
* Return         : Status.
*******************************************************************************/
tBleStatus Free_Fall_Notify(void)
{  
  uint8_t val;
  tBleStatus ret;

  val = 0x01;	
  ret = aci_gatt_update_char_value_ext(connection_handle, accServHandle, freeFallCharHandle, 1,1, 0, 1, &val);
  if (ret != BLE_STATUS_SUCCESS){
    PRINTF("Error while updating Free Fall characteristic: 0x%02X\n",ret) ;
    return BLE_STATUS_ERROR ;
  }
  return BLE_STATUS_SUCCESS;
	
}

/*******************************************************************************
* Function Name  : FIFO_Notify TO DO
* Description    : Send a notification for a Free Fall detection.
* Input          : None
* Return         : Status.
*******************************************************************************/
tBleStatus FIFO_Notify(void)
{
  uint8_t buff[18]; //make room for 6*8 bits of data
  tBleStatus ret;
  //PRINTF("Test %u \n", i);

	HOST_TO_LE_16(buff,-FIFO_data[i].AXIS_Y);
  HOST_TO_LE_16(buff+2,FIFO_data[i].AXIS_X);
  HOST_TO_LE_16(buff+4,-FIFO_data[i].AXIS_Z);
	HOST_TO_LE_16(buff+6,FIFO_data[i].AXIS_GY);
  HOST_TO_LE_16(buff+8,FIFO_data[i].AXIS_GX);
  HOST_TO_LE_16(buff+10,FIFO_data[i].AXIS_GZ);
	HOST_TO_LE_32(buff+12,FIFO_data[i].TIMESTAMP);
	HOST_TO_LE_16(buff+16, FIFO_data[i].PEDOMETER);
	

  ret = aci_gatt_update_char_value_ext(connection_handle, accServHandle, accCharHandle, 1, 18, 0, 18, buff);
  if (ret == BLE_STATUS_SUCCESS){
	  i++;
	  if(i==(level-1)){ 
		  i=0;
		  APP_FLAG_CLEAR(FIFO_NOTIFY);
	  }
  }else if(ret == BLE_STATUS_INSUFFICIENT_RESOURCES){
	  APP_FLAG_SET(TX_BUFFER_FULL);
	  return BLE_STATUS_SUCCESS;
  }

  return BLE_STATUS_SUCCESS;

}

/*******************************************************************************
* Function Name  : Acc_Update
* Description    : Update acceleration characteristic value
* Input          : AxesRaw_t structure containing acceleration value in mg.
* Return         : Status.
*******************************************************************************/
tBleStatus Acc_Update(AxesRaw_t *data_acc)
{  
  uint8_t buff[6]; //make room for 6*8 bits of data
  tBleStatus ret;

  HOST_TO_LE_16(buff,-data_acc->AXIS_Y);
  HOST_TO_LE_16(buff+2,data_acc->AXIS_X);
  HOST_TO_LE_16(buff+4,-data_acc->AXIS_Z);
  
  ret = aci_gatt_update_char_value_ext(connection_handle, accServHandle, accCharHandle, 1,6,0, 6, buff);
  if (ret != BLE_STATUS_SUCCESS){
    PRINTF("Error while updating Acceleration characteristic: 0x%02X\n",ret) ;
    return BLE_STATUS_ERROR ;
  }


  return BLE_STATUS_SUCCESS;

}

/*******************************************************************************
* Function Name  : Gyro_Update
* Description    : Update acceleration characteristic value
* Input          : AxesRaw_t structure containing acceleration value in mg.
* Return         : Status.
*******************************************************************************/
tBleStatus Gyr_Update(AxesRaw_t *data_gyro)
{
  uint8_t buff[6]; //make room for 6*8 bits of data
  tBleStatus ret;
  //PRINTF("Gyroscopes values are %ld, %ld, %ld\n", data_gyro->AXIS_X, data_gyro->AXIS_Y, data_gyro->AXIS_Z);
  HOST_TO_LE_16(buff,data_gyro->AXIS_Y);
  HOST_TO_LE_16(buff+2,data_gyro->AXIS_X);
  HOST_TO_LE_16(buff+4,data_gyro->AXIS_Z);

  ret = aci_gatt_update_char_value_ext(connection_handle, accServHandle, gyrCharHandle, 1,6,0, 6, buff);
  if (ret != BLE_STATUS_SUCCESS){
    PRINTF("Error while updating Gyroscope characteristic: 0x%02X\n",ret) ;
    return BLE_STATUS_ERROR ;
  }

  return BLE_STATUS_SUCCESS;
    
}

#ifdef SENSOR_EMULATION //Humidity sensor is not available on BlueNRG-1 Development board*/
/*******************************************************************************
* Function Name  : Humidity_Update
* Description    : Update humidity characteristic value
* Input          : RH (Relative Humidity) in thenths of %.
* Return         : Status.
*******************************************************************************/
tBleStatus Humidity_Update(uint16_t humidity)
{  
  tBleStatus ret;

  ret = aci_gatt_update_char_value_ext(connection_handle, envSensServHandle, humidityCharHandle, 1, 2, 0, 2, (uint8_t*)&humidity);
  if (ret != BLE_STATUS_SUCCESS){
          PRINTF("Error while updating Humidity characteristic: 0x%04X\n",ret) ;
          return BLE_STATUS_ERROR ;
  }
  return BLE_STATUS_SUCCESS;

}
#endif 

/*******************************************************************************
* Function Name  : Read_Request_CB.
* Description    : Update the sensor values.
* Input          : Handle of the characteristic to update.
* Return         : None.
*******************************************************************************/
void Read_Request_CB(uint16_t handle)
{
  tBleStatus ret;
  AxesRaw_t acc_data;
  AxesRaw_t gyr_data;
 
  if(handle == accCharHandle + 1)
  {
    if (GetAccAxesRaw(&acc_data, &gyr_data) == IMU_6AXES_OK)
    {
      Acc_Update(&acc_data);
    }
  }
  if(handle == gyrCharHandle + 1)
  {
    if (GetAccAxesRaw(&acc_data, &gyr_data) == IMU_6AXES_OK)
    {
      Gyr_Update(&gyr_data);
    }
  }
  if(connection_handle !=0)
  {
    ret = aci_gatt_allow_read(connection_handle);
    if (ret != BLE_STATUS_SUCCESS)
    {
      SdkEvalLedOn(LED3); 
    }
  }
}

