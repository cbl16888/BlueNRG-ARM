
/******************** (C) COPYRIGHT 2018 STMicroelectronics ********************
* File Name          : SensorDemo_main.c
* Author             : RF Application Team
* Version            : 1.1.0
* Date               : 15-January-2016
* Description        : Code demostrating a proprietary Bluetooth Low Energy profile: the sensor profile.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/**
 * @file SensorDemo_main.c
 * @brief This application contains an example which shows how implementing a proprietary
 * Bluetooth Low Energy profile: the sensor profile. It also provides a reference example about how using the 
 * BLE Over-The-Air (OTA) firmware upgrade capability with the BLE Sensor Demo.
 * 

* \section ATOLLIC_project ATOLLIC project
  To use the project with ATOLLIC TrueSTUDIO for ARM, please follow the instructions below:
  -# Open the ATOLLIC TrueSTUDIO for ARM and select File->Import... Project menu. 
  -# Select Existing Projects into Workspace. 
  -# Select the ATOLLIC project
  -# Select desired configuration to build from Project->Manage Configurations
  -# Select Project->Rebuild Project. This will recompile and link the entire application
  -# To download the binary image, please connect STLink to JTAG connector in your board (if available).
  -# Select Project->Download to download the related binary image.
  -# Alternatively, open the BlueNRG1 Flasher utility and download the built binary image.

* \section KEIL_project KEIL project
  To use the project with KEIL uVision 5 for ARM, please follow the instructions below:
  -# Open the KEIL uVision 5 for ARM and select Project->Open Project menu. 
  -# Open the KEIL project
     <tt> ...\\Project\\BLE_Examples\\BLE_SensorDemo\\MDK-ARM\\BlueNRG-1\\BLE_SensorDemo.uvprojx </tt> or
     <tt> ...\\Project\\BLE_Examples\\BLE_SensorDemo\\MDK-ARM\\BlueNRG-2\\BLE_SensorDemo.uvprojx </tt>
  -# Select desired configuration to build
  -# Select Project->Rebuild all target files. This will recompile and link the entire application
  -# To download the binary image, please connect STLink to JTAG connector in your board (if available).
  -# Select Project->Download to download the related binary image.
  -# Alternatively, open the BlueNRG1 Flasher utility and download the built binary image.

* \section IAR_project IAR project
  To use the project with IAR Embedded Workbench for ARM, please follow the instructions below:
  -# Open the Embedded Workbench for ARM and select File->Open->Workspace menu. 
  -# Open the IAR project
     <tt> ...\\Project\\BLE_Examples\\BLE_SensorDemo\\EWARM\\BlueNRG-1\\BLE_SensorDemo.eww </tt> or
     <tt> ...\\Project\\BLE_Examples\\BLE_SensorDemo\\EWARM\\BlueNRG-2\\BLE_SensorDemo.eww </tt>
  -# Select desired configuration to build
  -# Select Project->Rebuild All. This will recompile and link the entire application
  -# To download the binary image, please connect STLink to JTAG connector in your board (if available).
  -# Select Project->Download and Debug to download the related binary image.
  -# Alternatively, open the BlueNRG1 Flasher utility and download the built binary image.

* \subsection Project_configurations Project configurations
- \c HigherApp_OTA - Release configuration for Higher Application with OTA Service
- \c LowerApp_OTA - Release configuration for Lower Application with OTA Service
- \c Release - Release configuration
- \c Use_OTA_ServiceManager - Release configuration for Application using OTA Service Manager


* \section Board_supported Boards supported
- \c STEVAL-IDB007V1
- \c STEVAL-IDB007V2
- \c STEVAL-IDB008V1
- \c STEVAL-IDB008V2
- \c STEVAL-IDB009V1


* \section Power_settings Power configuration settings
@table

==========================================================================================================
|                                         STEVAL-IDB00XV1                                                |
----------------------------------------------------------------------------------------------------------
| Jumper name |            |  Description                                                                |
| JP1         |   JP2      |                                                                             |
----------------------------------------------------------------------------------------------------------
| ON 1-2      | ON 2-3     | USB supply power                                                            |
| ON 2-3      | ON 1-2     | The supply voltage must be provided through battery pins.                   |
| ON 1-2      |            | USB supply power to STM32L1, JP2 pin 2 external power to BlueNRG1           |


@endtable 

* \section Jumper_settings Jumper settings
@table

========================================================================================================================================================================================
|                                                                             STEVAL-IDB00XV1                                                                                          |
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
| Jumper name |                                                                Description                                                                                             |
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------          
| JP1         | 1-2: to provide power from USB (JP2:2-3). 2-3: to provide power from battery holder (JP2:1-2)                                                                          |          
| JP2         | 1-2: to provide power from battery holder (JP1:2-3). 2-3: to provide power from USB (JP1:1-2). Pin2 to VDD  to provide external power supply to BlueNRG-1 (JP1: 1-2)   |
| JP3         | pin 1 and 2 UART RX and TX of MCU. pin 3 GND.                                                                                                                          |          
| JP4         | Fitted: to provide VBLUE to BlueNRG1. It can be used also for current measurement.                                                                                     |
| JP5         | Fitted : TEST pin to VBLUE. Not fitted:  TEST pin to GND                                                                                                               |


@endtable 
                        
* \section Pin_settings Pin settings
@table
|            |                                                 Release                                                 |||||                                              HigherApp_OTA                                              |||||                                                        Use_OTA_ServiceManager                                                         |||||                                              LowerApp_OTA                                               |||||
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
|  PIN name  |   STEVAL-IDB007V1  |   STEVAL-IDB007V2  |   STEVAL-IDB008V1  |   STEVAL-IDB008V2  |   STEVAL-IDB009V1  |   STEVAL-IDB007V1  |   STEVAL-IDB007V2  |   STEVAL-IDB008V1  |   STEVAL-IDB008V2  |   STEVAL-IDB009V1  |      STEVAL-IDB007V1     |      STEVAL-IDB007V2     |      STEVAL-IDB008V1     |      STEVAL-IDB008V2     |      STEVAL-IDB009V1     |   STEVAL-IDB007V1  |   STEVAL-IDB007V2  |   STEVAL-IDB008V1  |   STEVAL-IDB008V2  |   STEVAL-IDB009V1  |
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
|    ADC1    |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |
|    ADC2    |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |
|     IO0    |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |
|     IO1    |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |
|    IO11    |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |
|    IO12    |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |
|    IO13    |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |
|    IO14    |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |
|    IO15    |        N.A.        |        N.A.        |        N.A.        |        N.A.        |      Not Used      |        N.A.        |        N.A.        |        N.A.        |        N.A.        |      Not Used      |           N.A.           |           N.A.           |           N.A.           |           N.A.           |         Not Used         |        N.A.        |        N.A.        |        N.A.        |        N.A.        |      Not Used      |
|    IO16    |        N.A.        |        N.A.        |        N.A.        |        N.A.        |      Not Used      |        N.A.        |        N.A.        |        N.A.        |        N.A.        |      Not Used      |           N.A.           |           N.A.           |           N.A.           |           N.A.           |         Not Used         |        N.A.        |        N.A.        |        N.A.        |        N.A.        |      Not Used      |
|    IO17    |        N.A.        |        N.A.        |        N.A.        |        N.A.        |      Not Used      |        N.A.        |        N.A.        |        N.A.        |        N.A.        |      Not Used      |           N.A.           |           N.A.           |           N.A.           |           N.A.           |         Not Used         |        N.A.        |        N.A.        |        N.A.        |        N.A.        |      Not Used      |
|    IO18    |        N.A.        |        N.A.        |        N.A.        |        N.A.        |      Not Used      |        N.A.        |        N.A.        |        N.A.        |        N.A.        |      Not Used      |           N.A.           |           N.A.           |           N.A.           |           N.A.           |         Not Used         |        N.A.        |        N.A.        |        N.A.        |        N.A.        |      Not Used      |
|    IO19    |        N.A.        |        N.A.        |        N.A.        |        N.A.        |      Not Used      |        N.A.        |        N.A.        |        N.A.        |        N.A.        |      Not Used      |           N.A.           |           N.A.           |           N.A.           |           N.A.           |         Not Used         |        N.A.        |        N.A.        |        N.A.        |        N.A.        |      Not Used      |
|     IO2    |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |
|    IO20    |        N.A.        |        N.A.        |        N.A.        |        N.A.        |      Not Used      |        N.A.        |        N.A.        |        N.A.        |        N.A.        |      Not Used      |           N.A.           |           N.A.           |           N.A.           |           N.A.           |         Not Used         |        N.A.        |        N.A.        |        N.A.        |        N.A.        |      Not Used      |
|    IO21    |        N.A.        |        N.A.        |        N.A.        |        N.A.        |      Not Used      |        N.A.        |        N.A.        |        N.A.        |        N.A.        |      Not Used      |           N.A.           |           N.A.           |           N.A.           |           N.A.           |         Not Used         |        N.A.        |        N.A.        |        N.A.        |        N.A.        |      Not Used      |
|    IO22    |        N.A.        |        N.A.        |        N.A.        |        N.A.        |      Not Used      |        N.A.        |        N.A.        |        N.A.        |        N.A.        |      Not Used      |           N.A.           |           N.A.           |           N.A.           |           N.A.           |         Not Used         |        N.A.        |        N.A.        |        N.A.        |        N.A.        |      Not Used      |
|    IO23    |        N.A.        |        N.A.        |        N.A.        |        N.A.        |      Not Used      |        N.A.        |        N.A.        |        N.A.        |        N.A.        |      Not Used      |           N.A.           |           N.A.           |           N.A.           |           N.A.           |         Not Used         |        N.A.        |        N.A.        |        N.A.        |        N.A.        |      Not Used      |
|    IO24    |        N.A.        |        N.A.        |        N.A.        |        N.A.        |      Not Used      |        N.A.        |        N.A.        |        N.A.        |        N.A.        |      Not Used      |           N.A.           |           N.A.           |           N.A.           |           N.A.           |         Not Used         |        N.A.        |        N.A.        |        N.A.        |        N.A.        |      Not Used      |
|    IO25    |        N.A.        |        N.A.        |        N.A.        |        N.A.        |      Not Used      |        N.A.        |        N.A.        |        N.A.        |        N.A.        |      Not Used      |           N.A.           |           N.A.           |           N.A.           |           N.A.           |         Not Used         |        N.A.        |        N.A.        |        N.A.        |        N.A.        |      Not Used      |
|     IO3    |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |
|     IO4    |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |
|     IO5    |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |
|     IO6    |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |
|     IO7    |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |
|     IO8    |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |
|    TEST1   |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |         Not Used         |         Not Used         |         Not Used         |         Not Used         |         Not Used         |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |

@endtable 

* \section Serial_IO Serial I/O
@table
| Parameter name  | Value            | Unit      |
----------------------------------------------------
| Baudrate        | 115200 [default] | bit/sec   |
| Data bits       | 8                | bit       |
| Parity          | None             | bit       |
| Stop bits       | 1                | bit       |
@endtable

* \section LEDs_description LEDs description
@table
|            |                                                 Release                                                 |||||                                                                                                          HigherApp_OTA                                                                                                          |||||                                                                                                     Use_OTA_ServiceManager                                                                                                      |||||                                                                                                          LowerApp_OTA                                                                                                           |||||
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
|  LED name  |   STEVAL-IDB007V1  |   STEVAL-IDB007V2  |   STEVAL-IDB008V1  |   STEVAL-IDB008V2  |   STEVAL-IDB009V1  |               STEVAL-IDB007V1              |               STEVAL-IDB007V2              |               STEVAL-IDB008V1              |               STEVAL-IDB008V2              |               STEVAL-IDB009V1              |               STEVAL-IDB007V1              |               STEVAL-IDB007V2              |               STEVAL-IDB008V1              |               STEVAL-IDB008V2              |               STEVAL-IDB009V1              |               STEVAL-IDB007V1              |               STEVAL-IDB007V2              |               STEVAL-IDB008V1              |               STEVAL-IDB008V2              |               STEVAL-IDB009V1              |
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
|     DL1    |    Activity led    |    Activity led    |    Activity led    |    Activity led    |    Activity led    |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |
|     DL2    |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |
|     DL3    |      Error led     |      Error led     |      Error led     |      Error led     |      Error led     |   ON when OTA firmware upgrade is ongoing  |   ON when OTA firmware upgrade is ongoing  |   ON when OTA firmware upgrade is ongoing  |   ON when OTA firmware upgrade is ongoing  |   ON when OTA firmware upgrade is ongoing  |   ON when OTA firmware upgrade is ongoing  |   ON when OTA firmware upgrade is ongoing  |   ON when OTA firmware upgrade is ongoing  |   ON when OTA firmware upgrade is ongoing  |   ON when OTA firmware upgrade is ongoing  |   ON when OTA firmware upgrade is ongoing  |   ON when OTA firmware upgrade is ongoing  |   ON when OTA firmware upgrade is ongoing  |   ON when OTA firmware upgrade is ongoing  |   ON when OTA firmware upgrade is ongoing  |
|     DL4    |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |                  Not Used                  |

@endtable


* \section Buttons_description Buttons description
@table
|                |                                                 Release                                                 |||||                                              HigherApp_OTA                                              |||||                                                                       Use_OTA_ServiceManager                                                                        |||||                                              LowerApp_OTA                                               |||||
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
|   BUTTON name  |   STEVAL-IDB007V1  |   STEVAL-IDB007V2  |   STEVAL-IDB008V1  |   STEVAL-IDB008V2  |   STEVAL-IDB009V1  |   STEVAL-IDB007V1  |   STEVAL-IDB007V2  |   STEVAL-IDB008V1  |   STEVAL-IDB008V2  |   STEVAL-IDB009V1  |         STEVAL-IDB007V1        |         STEVAL-IDB007V2        |         STEVAL-IDB008V1        |         STEVAL-IDB008V2        |         STEVAL-IDB009V1        |   STEVAL-IDB007V1  |   STEVAL-IDB007V2  |   STEVAL-IDB008V1  |   STEVAL-IDB008V2  |   STEVAL-IDB009V1  |
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
|      PUSH1     |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |   Jump to OTA Service manager  |   Jump to OTA Service manager  |   Jump to OTA Service manager  |   Jump to OTA Service manager  |   Jump to OTA Service manager  |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |
|      PUSH2     |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |            Not Used            |            Not Used            |            Not Used            |            Not Used            |            Not Used            |      Not Used      |      Not Used      |      Not Used      |      Not Used      |      Not Used      |
|      RESET     |   Reset BlueNRG1   |   Reset BlueNRG1   |   Reset BlueNRG2   |   Reset BlueNRG2   |   Reset BlueNRG2   |   Reset BlueNRG1   |   Reset BlueNRG1   |   Reset BlueNRG2   |   Reset BlueNRG2   |   Reset BlueNRG2   |         Reset BlueNRG1         |         Reset BlueNRG1         |         Reset BlueNRG2         |         Reset BlueNRG2         |         Reset BlueNRG2         |   Reset BlueNRG1   |   Reset BlueNRG1   |   Reset BlueNRG2   |   Reset BlueNRG2   |   Reset BlueNRG2   |

@endtable

* \section Usage Usage
This profile exposes two services: 
 - acceleration service 
 - environmental service.

Acceleration service exposes these characteristics: 
 - free-fall characteristic (read & notify properties ). 
    The application will send a notification on this characteristic if a free-fall condition has been
detected by the MEMS sensor (the condition is detected if the acceleration on the 3
axes is near zero for a certain amount of time).
 - acceleration characteristic  measured by the accelerometer (read & notify properties). The value is made up of six bytes. Each couple of
bytes contains the acceleration on one of the 3 axes. The values are given in mg.

Environmental service exposes these characteristics: 
-  temperature, pressure characteristics (read property). For each
characteristic, a characteristic format descriptor is present to describe the type of data
contained inside the characteristic. 

NOTEs:
     - Setting the preprocessor option SENSOR_EMULATION=1, the sensor demo is built for using emulated values for acceleration and environmental sensors values. 
     - OTA service support for lower or higher application is enabled, respectively,  through ST_OTA_LOWER_APPLICATION=1 or ST_OTA_HIGHER_APPLICATION=1 (preprocessor, linker) options and files: OTA_btl.[ch] (refer to LowerApp_OTA and HigherApp_OTA IAR workspaces).
     - OTA service manager support is enabled, respectively,  through ST_USE_OTA_SERVICE_MANAGER_APPLICATION (preprocessor, linker) options and files: OTA_btl.[ch] (refer to Use_OTA_ServiceManager IAR workspace).
     - OTA FW upgrade feature is supported only on BlueNRG-2, BLE stack v2.x.

**/
   
/** @addtogroup BlueNRG1_demonstrations_applications
 * BlueNRG-1 SensorDemo \see SensorDemo_main.c for documentation.
 *
 *@{
 */

/** @} */
/** \cond DOXYGEN_SHOULD_SKIP_THIS
 */
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "BlueNRG1_it.h"
#include "BlueNRG1_conf.h"
#include "ble_const.h" 
#include "bluenrg1_stack.h"
#include "SDK_EVAL_Config.h"
#include "sleep.h"
#include "sensor.h"
#include "SensorDemo_config.h"
#include "OTA_btl.h"  
#include "gatt_db.h"
#include "clock.h"
#include "SDK_EVAL_Button.h"
#include "lsm6ds3_reg.h"
#include "lsm6ds3.h"
#include "lsm6ds3_hal.h"
#include "app_state.h"



/* Private typedef -----------------------------------------------------------*/
typedef union {
	uint8_t byte[4];
	uint32_t val;
} timestamp_t;

/* Private define ------------------------------------------------------------*/
//#define SENSOR_EMULATION 1
#ifndef DEBUG
#define DEBUG 1
#endif

#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

#define BLE_SENSOR_VERSION_STRING "1.0.0" 

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t request_fifo_full_notify=FALSE;
uint8_t request_fifo_full_read=FALSE;
extern int connected;
extern int l2cap_request_accepted;
extern uint8_t start_request;
/*ST LSM6DS3 driver variables init*/
lsm6ds3_ctx_t dev_ctx;
timestamp_t timestamp;
lsm6ds3_int1_route_t int_1_reg;
uint16_t pattern_len, pattern_numbers, number_of_patterns;
static uint8_t whoamI, rst;
volatile int app_flags = SET_CONNECTABLE;

/* Private function prototypes -----------------------------------------------*/
int32_t platform_write(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len);
int32_t platform_read(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len);
void LSM6DS3_FIFO_init(void);
void GPIO_12_interrupt_init(void);
/* Private functions ---------------------------------------------------------*/


int main(void) 
{
  uint8_t ret;

  /* System Init */
  SystemInit();
  
  /* Systick Init */
  Clock_Init();

  /* Identify BlueNRG1 platform */
  SdkEvalIdentification();

  /* Configure I/O communication channel */
  SdkEvalComUartInit(UART_BAUDRATE);

  /* BlueNRG-1 stack init */
  ret = BlueNRG_Stack_Initialization(&BlueNRG_Stack_Init_params);
  if (ret != BLE_STATUS_SUCCESS) {
    PRINTF("Error in BlueNRG_Stack_Initialization() 0x%02x\r\n", ret);
    while(1);
  }
  
  /* Application demo Led Init */
  SdkEvalLedInit(LED1); //Activity led
  SdkEvalLedInit(LED3); //Error led
  SdkEvalLedOn(LED1);
  SdkEvalLedOff(LED3);

  PRINTF("BlueNRG-1 BLE Sensor Demo Application (version: %s)\r\n", BLE_SENSOR_VERSION_STRING);
  
#if ST_USE_OTA_SERVICE_MANAGER_APPLICATION
  /* Initialize the button: to be done before Sensor_DeviceInit for avoiding to 
     overwrite pressure/temperature sensor IO configuration when using BUTTON_2 (IO5) */
  SdkEvalPushButtonInit(USER_BUTTON);
#endif /* ST_USE_OTA_SERVICE_MANAGER_APPLICATION */

  /* Sensor Device Init */
  ret = Sensor_DeviceInit();
  if (ret != BLE_STATUS_SUCCESS) {
    SdkEvalLedOn(LED3);
    while(1);
  }

  /* Enable Push Button 1 as IRQ */
  SdkEvalPushButtonInit(BUTTON_1);
  SdkEvalPushButtonIrq(BUTTON_1,IRQ_ON_FALLING_EDGE);

  /*Enable GPIO_12 as interrupt */
  GPIO_12_interrupt_init();

  /*LSM6DS3 FIFO init*/
  LSM6DS3_FIFO_init();

  uint8_t fifo_status;

  while(1)
  {

    /* BLE Stack Tick */
    BTLE_StackTick();

    /* Application Tick */
    APP_Tick();
		
		if(APP_FLAG(CONNECTED) && APP_FLAG(L2CAP_PARAM_UPD_SENT) && !APP_FLAG(FIFO_NOTIFY) && APP_FLAG(NOTIFICATIONS_ENABLED)){
			BlueNRG_Sleep(SLEEPMODE_NOTIMER, WAKEUP_IO12, WAKEUP_IOx_HIGH<<WAKEUP_IO12_SHIFT_MASK);
		}

    //PRINTF("%u %u %u %u %u \n", request_fifo_full_read,request_fifo_full_notify,connected,l2cap_request_accepted,start_request);

    /* Set the FIFO full flag when getting out of sleep mode */
    lsm6ds3_fifo_wtm_flag_get(&dev_ctx, &fifo_status);
    if(fifo_status==1)
    {
      APP_FLAG_SET(EMPTY_FIFO);
      //PRINTF("FIFO WATERMARK REACHED \n");
    }


    //PRINTF("BLUE NRG WOKE UP SOURCE %i \n", BlueNRG_WakeupSource());

#if ST_OTA_FIRMWARE_UPGRADE_SUPPORT
    /* Check if the OTA firmware upgrade session has been completed */
    if (OTA_Tick() == 1)
    {
      /* Jump to the new application */
      OTA_Jump_To_New_Application();
    }
#endif /* ST_OTA_FIRMWARE_UPGRADE_SUPPORT */

#if ST_USE_OTA_SERVICE_MANAGER_APPLICATION
    if (SdkEvalPushButtonGetState(USER_BUTTON) == RESET)
    {
      OTA_Jump_To_Service_Manager_Application();
    }
#endif /* ST_USE_OTA_SERVICE_MANAGER_APPLICATION */
  }/* while (1) */
}

/***************************************************************************************/

#ifdef USE_FULL_ASSERT
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    
    /* Infinite loop */
    while (1)
    {}
}
#endif

/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/
/** \endcond
 */

void LSM6DS3_FIFO_init(void){
  dev_ctx.write_reg = platform_write;
  dev_ctx.read_reg = platform_read;
  /*
    * Check device ID
    */
   lsm6ds3_device_id_get(&dev_ctx, &whoamI);
   if (whoamI != LSM6DS3_ID)
     while(1)
     {
       PRINTF("ERROR \n");
     }

   /*
    * Restore default configuration
    */
   lsm6ds3_reset_set(&dev_ctx, PROPERTY_ENABLE);
   do {
 	  lsm6ds3_reset_get(&dev_ctx, &rst);
   } while (rst);

   /*
    * Set full scale
    */
   lsm6ds3_xl_full_scale_set(&dev_ctx, LSM6DS3_2g);
   lsm6ds3_gy_full_scale_set(&dev_ctx, LSM6DS3_250dps);

   /*
    * Enable Block Data Update
    */
   lsm6ds3_block_data_update_set(&dev_ctx, PROPERTY_ENABLE);

   /*
    * Enable pedometer
    */
   lsm6ds3_pedo_sens_set(&dev_ctx, PROPERTY_ENABLE);

   /*
    * Set High Resolution Timestamp (25 us tick) !!Don't forget to reset it every once in a while!!
    */
   lsm6ds3_timestamp_res_set(&dev_ctx, LSM6DS3_LSB_6ms4);

   /*
    * Enable timestamp in HW
    */
   lsm6ds3_timestamp_set(&dev_ctx, PROPERTY_ENABLE);

   /*
    * Reset Timestamp
    */
   lsm6ds3_timestamp_rst_set(&dev_ctx);

   /*
    * Set FIFO watermark to a multiple of a pattern
    * in this example we set watermark to 32 pattern
    * which means 32 sequence of:
    * (XL) = 6 bytes (3 word)
    */
   pattern_len = 3;
   pattern_numbers = 30;
   number_of_patterns = 3;
   lsm6ds3_fifo_watermark_set(&dev_ctx, pattern_numbers * pattern_len * number_of_patterns);

   /*
    * Set FIFO mode to Stream to FIFO
    */
   lsm6ds3_fifo_mode_set(&dev_ctx, LSM6DS3_STREAM_MODE);

   /*
    * Set FIFO sensor decimators
    */
   lsm6ds3_fifo_xl_batch_set(&dev_ctx, LSM6DS3_FIFO_XL_NO_DEC);
   lsm6ds3_fifo_gy_batch_set(&dev_ctx, LSM6DS3_FIFO_XL_NO_DEC);
   lsm6ds3_fifo_dataset_4_batch_set(&dev_ctx, LSM6DS3_FIFO_DS4_NO_DEC);

   /*
    * Enable PEDO/TIMESTAMP as 4th data set
    */

   lsm6ds3_fifo_pedo_batch_set(&dev_ctx, PROPERTY_ENABLE);

   /*
    * Set PEDO FIFO storing mode
    */
   lsm6ds3_fifo_write_trigger_set(&dev_ctx, LSM6DS3_TRG_XL_GY_DRDY);

   /*
    * Set ODR FIFO
    */
   lsm6ds3_fifo_data_rate_set(&dev_ctx, LSM6DS3_FIFO_52Hz);

   /*
    * Set Output Data Rate for acc/gyro to 52 Hz
    */
   lsm6ds3_xl_data_rate_set(&dev_ctx, LSM6DS3_XL_ODR_52Hz);
   lsm6ds3_gy_data_rate_set(&dev_ctx, LSM6DS3_XL_ODR_52Hz);

   /*
    * Enable drdy 75 us pulse: uncomment if interrupt must be pulsed
    */
   lsm6ds3_int_notification_set(&dev_ctx, LSM6DS3_INT_PULSED);

   /*
    * Enable interrupt generation on DRDY INT1 pin
    */
   lsm6ds3_pin_int1_route_get(&dev_ctx, &int_1_reg);
   int_1_reg.int1_fth = PROPERTY_ENABLE;
   int_1_reg.drdy_on_int1 = PROPERTY_DISABLE;
   lsm6ds3_pin_int1_route_set(&dev_ctx, &int_1_reg);
}

void GPIO_12_interrupt_init(void){
	  GPIO_InitType GPIO_InitStructure;

	  /* Enables the GPIO Clock */
	  SysCtrl_PeripheralClockCmd(CLOCK_PERIPH_GPIO, ENABLE);

	  /* Configures Button pin as input */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Input;
	  GPIO_InitStructure.GPIO_Pull = DISABLE;
	  GPIO_InitStructure.GPIO_HighPwr = DISABLE;
	  GPIO_Init(&GPIO_InitStructure);

	  GPIO_EXTIConfigType GPIO_EXTIStructure;
	  NVIC_InitType NVIC_InitStructure;

	  /* Set the GPIO interrupt priority and enable it */
	  NVIC_InitStructure.NVIC_IRQChannel = GPIO_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = LOW_PRIORITY;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC_InitStructure);

	  /* Configures EXTI line */
	  GPIO_EXTIStructure.GPIO_Pin = GPIO_Pin_12;
	  GPIO_EXTIStructure.GPIO_IrqSense = GPIO_IrqSense_Edge;
	  GPIO_EXTIStructure.GPIO_Event = GPIO_Event_High;
	  GPIO_EXTIConfig(&GPIO_EXTIStructure);

	  /* Clear pending interrupt */
	  GPIO_ClearITPendingBit(GPIO_Pin_12);

	  /* Enable the interrupt */
	  GPIO_EXTICmd(GPIO_Pin_12, ENABLE);
}

