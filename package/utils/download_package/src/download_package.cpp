//============================================================================
// Name        : download_package.cpp
// Author      : rd
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
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

#define DOWN_LOAD_PACKAGE	"DOWN_LOAD_PACKAGE"
#define CHECK_TIME 			"CHECK_TIME"
#define PORT 8181

using namespace std;

list<string> wifi_data;
list<string> rsp_wifi_data;

string msg_rsp;
string ip;
string mac;

bool flag_encryption = FALSE;

const char *PASSWORD,  *ENCRYPTION;
const char * SSID;

void getMacAddress(char *uc_Mac){
	struct ifreq s;
	unsigned char *mac = NULL;
	int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);

	strcpy(s.ifr_name, "eth0");
	if (0 == ioctl(fd, SIOCGIFHWADDR, &s)) {
		mac = (unsigned char*) s.ifr_addr.sa_data;
	}
	sprintf((char*) uc_Mac, (const char*) "%.2x:%.2x:%.2x:%.2x:%.2x:%.2x",
			mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	puts(uc_Mac);
}

void openfile(char const *command){
	FILE *file;
	msg_rsp = "";
	stringstream msg;
	char msg_line[255]={0};
	file = popen(command, "r");
	if(file==NULL){
		puts("lỗi");
		exit(1);
	}
	fgets(msg_line, 255, file);
	msg_rsp += msg_line;
	while(1){
		fgets(msg_line, 255, file);
		if(feof(file)){
			break;
		}
		msg_rsp += msg_line;
	}
	pclose(file);
	cout << msg_rsp << endl;
}

int ping(){
	openfile("ping -c3 www.google.com");
	size_t found = msg_rsp.find("0% packet loss");
	if(found != string::npos){
		openfile("hwclock -w");
		openfile("rm /etc/check_download");
		return 1;
	}
	else{
		return 0;
	}
}

void GetVersionHC(char *Version)
{
	FILE *file;
	msg_rsp = "";
	bool check = false;
	char TemVer[20] = {0};
	file = fopen("/etc/version.txt", "r");
	if (!file)
	{
		cout << "Can not open this file" << endl;
	}
	else
	{
		cout << "File is open" << endl;
	}
	fgets(TemVer, 20, file);
	fclose(file);
	for (int i = 0; i < 20; i++)
	{
		if (TemVer[i] == '\n')
			break;
		Version[i] = TemVer[i];
	}
}

int main(){
	struct json_object *jobj;
	int sock, ret, count;
	fd_set readfd;
	unsigned int addr_len;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0){
		perror("socket error\n");
		return -1;
	}
	addr_len = sizeof(struct sockaddr_in);
	memset((void*) &server_addr, 0, addr_len);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT);
	ret = bind(sock, (struct sockaddr*) &server_addr, addr_len);
	if (ret < 0){
		perror("bind error\n");
		return -1;
	}
	while(1){
		FD_ZERO(&readfd);
		FD_SET(sock, &readfd);
		ret = select(sock + 1, &readfd, NULL, NULL, 0);
		if (ret > 0){
			if (FD_ISSET(sock, &readfd)){
				char buffer[1024] = {0};
				count = recvfrom(sock, buffer, 1024, 0, (struct sockaddr*) &client_addr, &addr_len);
				puts(buffer);
				jobj = json_tokener_parse(buffer);
				enum json_type type;
				const char *cmd;
				json_object_object_foreach(jobj, key, val){
					type = json_object_get_type(val);
					switch (type){
						case json_type_string:
							if(strcmp(key,"CMD") == 0){
								cmd = json_object_get_string(json_object_object_get(jobj,"CMD"));
								if( strcmp(cmd,DOWN_LOAD_PACKAGE) == 0){
									char mac[20] = { 0 };
									char VersionHC[20] = {0};
									GetVersionHC(VersionHC);
									getMacAddress(mac);
									struct json_object * object;
									object = json_object_new_object();
									json_object_object_add(object, "CMD", json_object_new_string("HC_RESPONE"));
									json_object_object_add(object, "MAC", json_object_new_string(mac));
									json_object_object_add(object, "VERSION", json_object_new_string(VersionHC));
									const char * rsp;
									rsp = json_object_to_json_string(object);
									count = sendto(sock, rsp, strlen(rsp), 0, (struct sockaddr*) &client_addr, addr_len);
									openfile("ntpd -q -p 1.openwrt.pool.ntp.org");
									sleep(2);
									int stt = ping();
									if(stt == 1){
										struct json_object * object_done;
										object_done = json_object_new_object();
										json_object_object_add(object_done, "CMD", json_object_new_string("HC_RESPONE"));
										json_object_object_add(object_done, "STT", json_object_new_string("DOWNLOAD_SUCCESS"));
										const char * rsp_done;
										rsp_done = json_object_to_json_string(object_done);
										cout << rsp_done << endl;
										count = sendto(sock, rsp_done, strlen(rsp_done), 0, (struct sockaddr*) &client_addr, addr_len);
									}
									else{
										struct json_object * object_fault;
										object_fault = json_object_new_object();
										json_object_object_add(object_fault, "CMD", json_object_new_string("HC_RESPONE"));
										json_object_object_add(object_fault, "STT", json_object_new_string("DOWNLOAD_FAULT"));
										const char * rsp_fault;
										rsp_fault = json_object_to_json_string(object_fault);
										cout << rsp_fault << endl;
										count = sendto(sock, rsp_fault, strlen(rsp_fault), 0, (struct sockaddr*) &client_addr, addr_len);
									}
								}
								else if( strcmp(cmd,CHECK_TIME) == 0){
									openfile("hwclock -r");
									cout << msg_rsp << endl;
									struct json_object * object;
									object = json_object_new_object();
									json_object_object_add(object, "CMD", json_object_new_string("HC_RESPONE"));
									json_object_object_add(object, "TIME", json_object_new_string(msg_rsp.c_str()));
									const char * rsp;
									rsp = json_object_to_json_string(object);
									count = sendto(sock, rsp, strlen(rsp), 0, (struct sockaddr*) &client_addr, addr_len);
								}
							}
						break;
					}
				}
			}
		}
	}
	return 0;
}
