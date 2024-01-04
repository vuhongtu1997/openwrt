/*
 * UDP.h
 *
 *  Created on: Mar 3, 2022
 *      Author: rd
 */

#ifndef UDP_H_
#define UDP_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <list>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <json-c/json.h>
#include <cstddef>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "MQTTs.h"

#define SCAN_HC 				"SCAN_HC"
#define HC_SCAN_WIFI 			"HC_SCAN_WIFI"
#define HC_CONNECT_TO_CLOUD 	"HC_CONNECT_TO_CLOUD"
#define PORT 					8181

#define SIZE 200

using namespace rapidjson;
using namespace std;

extern list<string> wifi_data;
extern list<string> rsp_wifi_data;
extern list<string> rsp_ip;

extern string msg_rsp;
extern string ip;
extern string mac;

extern bool flag_encryption;

extern const char *PASSWORD;
extern const char *ENCRYPTION;
extern const char *SSID;

extern int sock;
extern int ret;
extern int count;
extern fd_set readfd;
extern unsigned int addr_len;
extern struct sockaddr_in server_addr;
extern struct sockaddr_in client_addr;

char* StringToChar(string s);
void ExecuteCMD(char const *command);
void GetMacAddress(char *uc_Mac);
string GetIP();
bool CheckIntoHome(const char *msg);
void GetVersionHC(char *Version);
void ConnectToWifi();
void ScanWifi();
void ProcessData(string msg_rsp);
char* Base64Decoder(char encoded[], int len_str);
int UDPSetup();
bool CheckFile (const std::string& name);

#endif /* UDP_H_ */
