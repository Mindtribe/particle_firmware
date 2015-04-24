/**
  Copyright (c) 2013-2015 Spark Labs, Inc.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, see <http://www.gnu.org/licenses/>.
  ******************************************************************************
 */


#ifndef CLOUD_H
#define	CLOUD_H

typedef struct SparkProtocol SparkProtocol;

#ifdef __cplusplus
extern "C" {
#endif
    
/**
 * Functions for managing the cloud connection, performing cloud operations
 * and system upgrades.
 */

int Internet_Test(void);

int Spark_Connect(void);
int Spark_Disconnect(void);

void Spark_Protocol_Init(void);
int Spark_Handshake(void);
bool Spark_Communication_Loop(void);
void Multicast_Presence_Announcement(void);
void Spark_Signal(bool on);
void Spark_SetTime(unsigned long dateTime);
void Spark_Process_Events();

extern volatile uint8_t LED_Spark_Signal;
void LED_Signaling_Override(void);


typedef enum
{
	BOOLEAN = 1, INT = 2, STRING = 4, DOUBLE = 9
} Spark_Data_TypeDef;

typedef enum
{
	PUBLIC = 0, PRIVATE = 1
} Spark_Event_TypeDef;

typedef struct String String;

typedef void (*EventHandler)(const char* name, const char* data);

typedef enum
{
  MY_DEVICES,
  ALL_DEVICES
} Spark_Subscription_Scope_TypeDef;

typedef int (*cloud_function_t)(void* data, const char* param, void* reserved);

typedef struct {
    uint16_t size;
    const char *funcKey;
    cloud_function_t fn;
    void* data; 
} cloud_function_descriptor;

bool spark_variable(const char *varKey, const void *userVar, Spark_Data_TypeDef userVarType, void* reserved);
bool spark_function(const cloud_function_descriptor* desc, void* reserved);
bool spark_send_event(const char* name, const char* data, int ttl, Spark_Event_TypeDef eventType, void* reserved);
bool spark_subscribe(const char *eventName, EventHandler handler, void* handler_data, 
        Spark_Subscription_Scope_TypeDef scope, const char* deviceID, void* reserved);


void spark_process(void);
void spark_connect(void);
void spark_disconnect(void);
bool spark_connected(void);
SparkProtocol* spark_protocol_instance(void);

char* bytes2hexbuf(const uint8_t* buf, unsigned len, char* output);
String bytes2hex(const uint8_t* buf, unsigned len);
String spark_deviceID(void);

#define SPARK_BUF_LEN			        600

//#define SPARK_SERVER_IP			        "54.235.79.249"
#define SPARK_SERVER_PORT		        5683

#define SPARK_LOOP_DELAY_MILLIS		        1000    //1sec
#define SPARK_RECEIVE_DELAY_MILLIS              10      //10ms

#define TIMING_FLASH_UPDATE_TIMEOUT             30000   //30sec

#define USER_VAR_MAX_COUNT		        10
#define USER_VAR_KEY_LENGTH		        12

#define USER_FUNC_MAX_COUNT		        4
#define USER_FUNC_KEY_LENGTH		        12
#define USER_FUNC_ARG_LENGTH		        64

#define USER_EVENT_NAME_LENGTH		        64
#define USER_EVENT_DATA_LENGTH		        64

#ifdef __cplusplus
}
#endif

#endif	/* CLOUD_H */

