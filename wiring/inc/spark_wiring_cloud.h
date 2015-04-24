/**
 ******************************************************************************
 * @file    spark_wiring_cloud.h
 * @author  Satish Nair, Zachary Crockett, Matthew McGowan
 ******************************************************************************
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

#pragma once

#include "spark_wiring_string.h"
#include "events.h"
#include "system_cloud.h"
#include "spark_protocol_functions.h"
#include "spark_wiring_system.h"
#include <functional>


typedef int (*user_function_t)(String paramString);
typedef std::function<int(String)> user_std_function_t;

class SparkClass {
    
        
public:
    static void variable(const char *varKey, const void *userVar, Spark_Data_TypeDef userVarType) 
    {
        spark_variable(varKey, userVar, userVarType, NULL);
    }
    
    static bool function(const char *funcKey, user_function_t func) 
    {
        return register_function(call_raw_user_function, &func, funcKey);
    }
    
    static bool function(const char *funcKey, user_std_function_t func)
    {
        auto fn = new user_std_function_t(func);        
        return fn ? register_function(call_std_user_function, fn, funcKey) : false;
    }

    bool publish(const char *eventName, Spark_Event_TypeDef eventType=PUBLIC)
    {
        return spark_send_event(eventName, NULL, 60, eventType, NULL);
    }

    bool publish(const char *eventName, const char *eventData, Spark_Event_TypeDef eventType=PUBLIC)
    {
        return spark_send_event(eventName, eventData, 60, eventType, NULL);
    }

    bool publish(const char *eventName, const char *eventData, int ttl, Spark_Event_TypeDef eventType=PUBLIC)
    {
        return spark_send_event(eventName, eventData, ttl, eventType, NULL);
    }

    bool publish(const String& eventName, Spark_Event_TypeDef eventType=PUBLIC)
    {
        return publish(eventName.c_str(), eventType);
    }

    bool publish(const String& eventName, const String& eventData, Spark_Event_TypeDef eventType=PUBLIC)
    {
        return publish(eventName.c_str(), eventData.c_str(), eventType);
    }

    bool publish(const String& eventName, const String& eventData, int ttl, Spark_Event_TypeDef eventType=PUBLIC)
    {
        return publish(eventName.c_str(), eventData.c_str(), ttl, eventType);
    }
    
    bool subscribe(const char *eventName, EventHandler handler, Spark_Subscription_Scope_TypeDef scope=ALL_DEVICES)
    {
        return spark_subscribe(eventName, handler, NULL, scope, NULL, NULL);
    }

    bool subscribe(const char *eventName, EventHandler handler, const char *deviceID)
    {
        return spark_subscribe(eventName, handler, NULL, MY_DEVICES, deviceID, NULL);
    }

#if 0    
    bool subscribe(String eventName, EventHandler handler)
    {
        return subscribe(eventName.c_str(), handler);
    }

    bool subscribe(String eventName, EventHandler handler, Spark_Subscription_Scope_TypeDef scope)
    {
        return subscribe(eventName.c_str(), handler, scope);
    }

    bool subscribe(String eventName, EventHandler handler, String deviceID)
    {
        return subscribe(eventName.c_str(), handler, deviceID.c_str());
    }
#endif    
    
    void unsubscribe() 
    {
        spark_protocol_remove_event_handlers(sp(), NULL);
    }

    void syncTime(void)
    {
        spark_protocol_send_time_request(sp());
    }
    
    static void sleep(long seconds) __attribute__ ((deprecated("Please use System.sleep() instead.")))
    { SystemClass::sleep(seconds); }    
    static void sleep(uint16_t wakeUpPin, uint16_t edgeTriggerMode, long seconds=0) __attribute__ ((deprecated("Please use System.sleep() instead.")))
    { SystemClass::sleep(wakeUpPin, edgeTriggerMode, seconds); }
    
    static bool connected(void) { return spark_connected(); }
    static void connect(void) { spark_connect(); }
    static void disconnect(void) { spark_disconnect(); }
    static void process(void) { spark_process(); }
    static String deviceID(void) { return SystemClass::deviceID(); }
    
private:

    static bool register_function(cloud_function_t fn, void* data, const char* funcKey);
    static int call_raw_user_function(void* data, const char* param, void* reserved);
    static int call_std_user_function(void* data, const char* param, void* reserved);
    
    SparkProtocol* sp() { return spark_protocol_instance(); }
};


extern SparkClass Spark;
