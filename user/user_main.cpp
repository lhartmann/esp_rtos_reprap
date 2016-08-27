/*
 * ESPRSSIF MIT License
 *
 * Copyright (c) 2015 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS ESP8266 only, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "esp_common.h"
#include <driver/uart.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/timers.h>
#include <esphttpd/esp8266.h>
#include <esphttpd/httpd.h>
#include <esphttpd/httpdespfs.h>
#include <esphttpd/espfs.h>
#include <esphttpd/webpages-espfs.h>
#include <esphttpd/cgiwebsocket.h>
#include <json/cJSON.h>
#include "cgi-test.h"
#include <math.h>
#include <stdlib.h>

int __errno;

//On reception of a message, echo it back verbatim
void myEchoWebsocketRecv(Websock *ws, char *data, int len, int flags) {
	os_printf("EchoWs: echo, len=%d\n", len);
	cgiWebsocketSend(ws, data, len, flags);
}

//Echo websocket connected. Install reception handler.
void myEchoWebsocketConnect(Websock *ws) {
	os_printf("EchoWs: connect\n");
	ws->recvCb=myEchoWebsocketRecv;
}

//On reception of a message, echo it back verbatim
double getAFuckingDouble(cJSON *object) {
	if (!object) return NAN;
	switch (object->type & 0xff) {
		case cJSON_False:  return 0;
		case cJSON_True:   return 1;
		case cJSON_NULL:   return 0;
		case cJSON_Number: return object->valuedouble; 
		case cJSON_String: return atoi(object->valuestring);
		case cJSON_Array:  return NAN;
		case cJSON_Object: return NAN;
		}
}
void myBhaskaraSolver_onMessage(Websock *ws, char *data, int len, int flags) {
	double a,b,c,d, x1, x2;
	char *str;
	cJSON *root;
	
	os_printf("BHWS: Got data!\n");
	for (str=data; str-data < len; ++str) os_putc(*str);
	
	root = cJSON_Parse(data);
	if (!root) {
		os_printf("BhaskaraWs: Bad JSON data:\n");
		
		root = cJSON_CreateNull();
		str = cJSON_Print(root);
	} else {
		a = getAFuckingDouble(cJSON_GetObjectItem(root,"a"));
		b = getAFuckingDouble(cJSON_GetObjectItem(root,"b"));
		c = getAFuckingDouble(cJSON_GetObjectItem(root,"c"));
		
		cJSON_Delete(root);

		root = cJSON_CreateObject();
		
		d = b*b - 4*a*c;
		if (d>=0) { // Real responses
			x1 = (-b - sqrt(d)) / (2*a);
			x2 = (-b + sqrt(d)) / (2*a);
			
			cJSON_AddItemToObjectCS(root,"isComplex", cJSON_CreateFalse());
			cJSON_AddItemToObjectCS(root,"x1",        cJSON_CreateNumber(x1));
			cJSON_AddItemToObjectCS(root,"x2",        cJSON_CreateNumber(x2));
		} else { // Complex
			x1 =     (-b) / (2*a); // Real
			x2 = sqrt(-d) / (2*a); // imaginary
			
			cJSON_AddItemToObjectCS(root,"isComplex", cJSON_CreateTrue());
			cJSON_AddItemToObjectCS(root,"real",      cJSON_CreateNumber(x1));
			cJSON_AddItemToObjectCS(root,"imag",      cJSON_CreateNumber(1));//x2));
		}

		os_printf("BhaskaraWs: a=%d, b=%d, c=%d ==> x1=%d, x2=%d.\n",
			(int)a, (int)b, (int)c, (int)x1, (int)x2
		);
		
		str = cJSON_Print(root);
	}
	
	cgiWebsocketSend(ws, str, strlen(str), flags);
	
	free(str);
	cJSON_Delete(root);
}

// Echo websocket connected. Install reception handler.
void myBhaskaraSolver_onOpen(Websock *ws) {
        os_printf("BhaskaraWs: connect\n");
        ws->recvCb=myBhaskaraSolver_onMessage;
}

HttpdBuiltInUrl builtInUrls[]={
	{"/cgiTestbed",             cgiTestbed,   NULL},
	{"/websocket/echo.cgi",     cgiWebsocket, (void*)myEchoWebsocketConnect},
	{"/websocket/bhaskara.cgi", cgiWebsocket, (void*)myBhaskaraSolver_onOpen},
	{"/",                       cgiRedirect,  "/index.html"},
	{"*", cgiEspFsHook, NULL},
	{NULL, NULL, NULL}
};

// RTOS Task
void myTask(void *pdParameters) {
	while(1) {
		// Delay 1 second
		vTaskDelay(configTICK_RATE_HZ);
		
		os_printf("myTask: Still alive...\n");
	}
}

/******************************************************************************
 * FunctionName : user_init
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
extern "C" void user_init(void) {
	
	UART_SetPrintPort(UART0);
	UART_SetBaudrate(UART0, 115200);
	os_printf("SDK version:%s\n", system_get_sdk_version());

#ifdef ESPFS_POS
	espFsInit((void*)(0x40200000 + ESPFS_POS));
#else
	espFsInit((void*)(webpages_espfs_start));
#endif
	httpdInit(builtInUrls, 80);
	
	if (pdPASS == xTaskCreate(
		&myTask,
		(signed char *)"myTask",
		1024, // unsigned portSHORT usStackDepth,
		0, // void *pvParameters,
		6, // unsigned portBASE_TYPE uxPriority,
		0 // xTaskHandle *pvCreatedTask
	)) {
		os_printf("Creating myTask succeeded.\n");
	} else {
		os_printf("Creating myTask failed.\n");
	}
}

