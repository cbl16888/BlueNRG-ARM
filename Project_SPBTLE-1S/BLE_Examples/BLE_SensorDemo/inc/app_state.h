/******************** (C) COPYRIGHT 2016 STMicroelectronics ********************
* File Name          : app_state.h
* Author             : AMS - VMA, RF Application Team
* Version            : V1.0.0
* Date               : 08-February-2016
* Description        : Header file wich contains variable used for application.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_STATE__H
#define __APP_STATE_H

#ifdef __cplusplus
 extern "C" {
#endif


/* Exported variables ------------------------------------------------------- */  
     
/** 
  * @brief  Variable which contains some flags useful for application
  */ 
extern volatile int app_flags;

/**
 * @name Flags for application
 * @{
 */
#define CONNECTED               0x01
#define SET_CONNECTABLE         0x02
#define NOTIFICATIONS_ENABLED   0x04
#define CONN_PARAM_UPD_SENT     0x08
#define L2CAP_PARAM_UPD_SENT    0x10
#define TX_BUFFER_FULL          0x20
#define LSM6DS3_DATA_READY		  0x40
#define FIFO_NOTIFY		          0x80
/* Added flags for handling TX, RX characteristics discovery */
#define START_READ_TX_CHAR_HANDLE 0x100 
#define END_READ_TX_CHAR_HANDLE   0x200
#define START_READ_RX_CHAR_HANDLE 0x400 
#define END_READ_RX_CHAR_HANDLE   0x800
#define HIGH_POWER								0x1000
#define SET_HIGH_POWER						0x2000
#define LOW_POWER									0x4000
#define SET_LOW_POWER					  	0x8000
/**
 * @}
 */



/* Exported macros -----------------------------------------------------------*/
#define APP_FLAG(flag) (app_flags & flag)

#define APP_FLAG_SET(flag) (app_flags |= flag)
#define APP_FLAG_CLEAR(flag) (app_flags &= ~flag)


#ifdef __cplusplus
}
#endif

#endif /*__APP_STATE__H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
