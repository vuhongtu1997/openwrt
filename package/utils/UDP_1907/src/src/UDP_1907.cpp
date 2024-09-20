
// //============================================================================
// // Name        : UDP18_06.cpp
// // Author      : rd
// // Version     :
// // Copyright   : Your copyright notice
// // Description : Hello World in C++, Ansi-style
// //============================================================================

// #include "UDP.h"

// list<string> wifi_data;
// list<string> rsp_wifi_data;
// list<string> rsp_ip;

// string msg_rsp = "";
// string ip = "";
// string mac = "";

// bool flag_encryption = false;

// const char *PASSWORD, *ENCRYPTION;
// const char *SSID;

// int sock = 0;
// int ret = 0;
// int count = 0;
// fd_set readfd;
// unsigned int addr_len;
// struct sockaddr_in server_addr;
// struct sockaddr_in client_addr;

// char *StringToChar(string s)
// {
// 	char *sendChar = new char[s.length() + 1];
// 	strcpy(sendChar, s.c_str());
// 	return sendChar;
// }

// void ExecuteCMD(char const *command)
// {
// 	FILE *file;
// 	msg_rsp = "";
// 	cout << command << endl;
// 	stringstream msg;
// 	char msg_line[100] = {0};
// 	file = popen(command, "r");
// 	if (file == NULL)
// 	{
// 		exit(1);
// 	}
// 	fgets(msg_line, 100, file);
// 	msg_rsp += msg_line;
// 	while (1)
// 	{
// 		fgets(msg_line, 100, file);
// 		if (feof(file))
// 		{
// 			break;
// 		}
// 		msg_rsp += msg_line;
// 	}
// 	pclose(file);
// }

// void GetMacAddress(char *uc_Mac)
// {
// 	struct ifreq s;
// 	unsigned char *mac = NULL;
// 	int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);

// 	strcpy(s.ifr_name, "eth0");
// 	if (0 == ioctl(fd, SIOCGIFHWADDR, &s))
// 	{
// 		mac = (unsigned char *)s.ifr_addr.sa_data;
// 	}
// 	sprintf((char *)uc_Mac, (const char *)"%.2x:%.2x:%.2x:%.2x:%.2x:%.2x",
// 			mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
// 	puts(uc_Mac);
// }

// string GetIP()
// {
// 	string string_ip_1;
// 	string string_ip_2;
// 	string string_ip_3;
// 	ExecuteCMD("ip -4 addr show ${link_name} | sed -Ene \'s/^.*inet ([0-9.]+)\\/.*$/\\1/p\'");
// 	int count = 0;
// 	const char *char_msg_rsp = msg_rsp.c_str();
// 	puts(char_msg_rsp);
// 	for (unsigned int i = 0; i < strlen(char_msg_rsp); i++)
// 	{
// 		if (char_msg_rsp[i] == '\n')
// 		{
// 			count++;
// 		}
// 	}
// 	printf("----%d----\n", count);
// 	if (count == 3)
// 	{
// 		int ip_index = 0;
// 		int count_line_index = 0;
// 		for (unsigned int i = 0; i < strlen(char_msg_rsp); i++)
// 		{
// 			if (char_msg_rsp[i] == '\n')
// 			{
// 				count_line_index++;
// 			}
// 			if (count_line_index == 0 && char_msg_rsp[i] != '\n')
// 			{
// 				string_ip_1 = string_ip_1 + char_msg_rsp[i];
// 			}
// 			if (count_line_index == 1 && char_msg_rsp[i] != '\n')
// 			{
// 				string_ip_2 = string_ip_2 + char_msg_rsp[i];
// 			}
// 			if (count_line_index == 2 && char_msg_rsp[i] != '\n')
// 			{
// 				string_ip_3 = string_ip_3 + char_msg_rsp[i];
// 				ip_index++;
// 			}
// 		}
// 		if (string_ip_1.compare("127.0.0.1") != 0 && string_ip_1.compare("10.10.10.1") != 0 && string_ip_1.compare("") != 0)
// 		{
// 			return string_ip_1;
// 		}
// 		if (string_ip_2.compare("127.0.0.1") != 0 && string_ip_2.compare("10.10.10.1") != 0 && string_ip_2.compare("") != 0)
// 		{
// 			return string_ip_2;
// 		}
// 		if (string_ip_3.compare("127.0.0.1") != 0 && string_ip_3.compare("10.10.10.1") != 0 && string_ip_3.compare("") != 0)
// 		{
// 			return string_ip_3;
// 		}
// 	}
// 	if (count == 2)
// 	{
// 		int count_line_index = 0;
// 		for (unsigned int i = 0; i < strlen(char_msg_rsp); i++)
// 		{
// 			if (char_msg_rsp[i] == '\n')
// 			{
// 				count_line_index++;
// 			}
// 			if (count_line_index == 0 && char_msg_rsp[i] != '\n')
// 			{
// 				string_ip_1 = string_ip_1 + char_msg_rsp[i];
// 			}
// 			if (count_line_index == 1 && char_msg_rsp[i] != '\n')
// 			{
// 				string_ip_2 = string_ip_2 + char_msg_rsp[i];
// 			}
// 		}
// 		if (string_ip_1.compare("127.0.0.1") != 0 && string_ip_1.compare("") != 0)
// 		{
// 			return string_ip_1;
// 		}
// 		if (string_ip_2.compare("127.0.0.1") != 0 && string_ip_2.compare("") != 0)
// 		{
// 			return string_ip_2;
// 		}
// 	}
// 	return "";
// }

// bool CheckIntoHome(const char *msg)
// {
// 	FILE *file;
// 	msg_rsp = "";
// 	bool check = false;
// 	char Dormitory[100] = {0};
// 	file = fopen("/etc/Dormitory.txt", "a+");
// 	if (!file)
// 	{
// 		cout << "Can not open this file" << endl;
// 	}
// 	else
// 	{
// 		cout << "File is open" << endl;
// 	}
// 	fgets(Dormitory, 100, file);
// 	cout << Dormitory << endl;
// 	cout << strlen(Dormitory) << endl;
// 	if (strcmp(Dormitory, "") == 0 || Dormitory[0] == EOF)
// 	{
// 		fputs(msg, file);
// 		check = true;
// 	}
// 	if (strcmp(Dormitory, msg) == 0)
// 	{
// 		check = true;
// 	}
// 	fclose(file);
// 	return check;
// }

// void GetVersionHC(char *Version)
// {
// 	FILE *file;
// 	msg_rsp = "";
// 	char TemVer[20] = {0};
// 	file = fopen("/etc/version.txt", "r");
// 	if (!file)
// 	{
// 		cout << "Can not open this file" << endl;
// 	}
// 	else
// 	{
// 		cout << "File is open" << endl;
// 	}
// 	fgets(TemVer, 20, file);
// 	fclose(file);
// 	for (int i = 0; i < 20; i++)
// 	{
// 		if (TemVer[i] == '\n')
// 			break;
// 		Version[i] = TemVer[i];
// 	}
// }

// void ConnectToWifi()
// {
// 	char ssid[100];
// 	ExecuteCMD("uci del network.wan.ifname");
// 	sprintf(ssid, "uci set wireless.wifinet1.ssid=\"%s\"", SSID);
// 	ExecuteCMD("uci del wireless.wifinet1");
// 	ExecuteCMD("uci set wireless.wifinet1=wifi-iface");
// 	ExecuteCMD(ssid);
// 	ExecuteCMD("uci set wireless.wifinet1.mode='sta'");
// 	ExecuteCMD("uci set wireless.wifinet1.network='wan'");
// 	ExecuteCMD("uci set wireless.wifinet1.device='radio0'");
// 	char pass[100];
// 	sprintf(pass, "uci set wireless.wifinet1.key='%s'", PASSWORD);
// 	ExecuteCMD(pass);
// 	char encryption[100];
// 	if (strcmp(ENCRYPTION, "none") == 0)
// 	{
// 		sprintf(encryption, "uci set wireless.wifinet1.encryption='%s'", ENCRYPTION);
// 		ExecuteCMD(encryption);
// 		// ExecuteCMD("uci set wireless.default_radio0.mode='sta'");
// 		ExecuteCMD("uci commit wireless");
// 		ExecuteCMD("wifi");
// 		sleep(30);
// 		ip = GetIP();
// 		cout << ip << endl;
// 		if (strcmp(ip.c_str(), "10.10.10.1") == 0)
// 		{
// 			ExecuteCMD("uci del wireless.wifinet1");
// 			ExecuteCMD("uci set wireless.wifinet1=wifi-iface");
// 			// ExecuteCMD("uci set wireless.default_radio0.mode='ap'");
// 			ExecuteCMD("uci commit wireless");
// 			ExecuteCMD("uci commit network");
// 			ExecuteCMD("wifi");
// 			ExecuteCMD("/etc/init.d/network restart");
// 		}
// 	}
// 	if (strcmp(ENCRYPTION, "none") != 0)
// 	{
// 		ExecuteCMD("uci set wireless.wifinet1.encryption='psk2'");
// 		// ExecuteCMD("uci set wireless.default_radio0.mode='sta'");
// 		ExecuteCMD("uci commit wireless");
// 		ExecuteCMD("uci commit network");
// 		ExecuteCMD("wifi");
// 		ExecuteCMD("/etc/init.d/network restart");
// 		sleep(30);
// 		ip = GetIP();
// 		cout << ip << endl;
// 		if (strcmp(ip.c_str(), "10.10.10.1") == 0)
// 		{
// 			ExecuteCMD("uci del wireless.wifinet1");
// 			// ExecuteCMD("uci set wireless.default_radio0.mode='ap'");
// 			ExecuteCMD("uci commit network");
// 			ExecuteCMD("uci commit wireless");
// 			ExecuteCMD("wifi");
// 			ExecuteCMD("/etc/init.d/network restart");
// 		}
// 	}
// }

// void ScanWifi()
// {
// 	ExecuteCMD("iwinfo wlan0 scan");
// 	ProcessData(msg_rsp);
// }

// void ProcessData(string msg_rsp)
// {
// 	string BSS = "Cell ";
// 	size_t pos = 0;
// 	string wifi;
// 	while ((pos = msg_rsp.find(BSS)) != string::npos)
// 	{
// 		wifi = msg_rsp.substr(0, pos);
// 		msg_rsp.erase(0, pos + BSS.length());
// 		wifi_data.push_front(wifi);
// 	}
// 	for (list<string>::iterator it = wifi_data.begin(); it != wifi_data.end(); it++)
// 	{
// 		const char *it_char = (*it).c_str();
// 		puts(it_char);
// 		int line_index = 0;
// 		if (strlen(it_char) >= 10)
// 		{
// 			char SSID[200] = {0};
// 			char QUALITY[50] = {0};
// 			char ENCRYPTION[50] = {0};
// 			unsigned int Authentication = 0;
// 			unsigned int Authentication1 = 0;
// 			const char *sub;
// 			const char *sub1;
// 			sub1 = strstr(it_char, "/70");
// 			sub = strstr(it_char, "Quality");
// 			if (sub)
// 			{
// 				Authentication = sub - it_char;
// 			}
// 			if (sub1)
// 			{
// 				Authentication1 = sub1 - it_char;
// 			}
// 			int QUALITY_index = 0;
// 			int SSID_index = 0;
// 			int ENCRYPTION_index = 0;
// 			for (unsigned int i = 0; i < strlen(it_char); i++)
// 			{
// 				if (it_char[i] == '\n')
// 				{
// 					line_index++;
// 				}
// 				else if (line_index == 1 && it_char[i + 1] != '\n')
// 				{
// 					if (SSID_index > 17)
// 					{
// 						SSID[SSID_index - 18] = it_char[i];
// 						SSID_index++;
// 					}
// 					else
// 					{
// 						SSID_index++;
// 					}
// 				}
// 				else if (line_index == 3 && i >= Authentication && i < Authentication1)
// 				{
// 					if (QUALITY_index > 8)
// 					{
// 						QUALITY[QUALITY_index - 9] = it_char[i];
// 						QUALITY_index++;
// 					}
// 					else
// 					{
// 						QUALITY_index++;
// 					}
// 				}
// 				else if (line_index == 4)
// 				{
// 					if (ENCRYPTION_index > 21)
// 					{
// 						ENCRYPTION[ENCRYPTION_index - 22] = it_char[i];
// 						ENCRYPTION_index++;
// 					}
// 					else
// 					{
// 						ENCRYPTION_index++;
// 					}
// 				}
// 			}
// 			int int_QUALITY = 0;
// 			int_QUALITY = atoi(QUALITY);
// 			int_QUALITY = int_QUALITY * 100 / 70;
// 			const char *char_QUALITY;
// 			string tmp = to_string(int_QUALITY);
// 			char_QUALITY = tmp.c_str();
// 			StringBuffer SendToApp;
// 			Writer<StringBuffer> json(SendToApp);
// 			json.StartObject();
// 			json.Key("CMD");
// 			json.String("HC_RESPONE");
// 			json.Key("SSID");
// 			json.String(SSID);
// 			json.Key("QUALITY");
// 			json.String(char_QUALITY);
// 			json.Key("ENCRYPTION");
// 			json.String(ENCRYPTION);
// 			json.EndObject();
// 			string s = SendToApp.GetString();
// 			rsp_wifi_data.push_front(s);
// 			printf("----%d----\n", int_QUALITY);
// 			puts(SSID);
// 			puts(ENCRYPTION);
// 		}
// 	}
// }

// char *Base64Decoder(char encoded[], int len_str)
// {
// 	char *decoded_string;
// 	decoded_string = (char *)malloc(sizeof(char) * SIZE);
// 	int i, j, k = 0;
// 	int num = 0;
// 	int count_bits = 0;
// 	for (i = 0; i < len_str; i += 4)
// 	{
// 		num = 0, count_bits = 0;
// 		for (j = 0; j < 4; j++)
// 		{
// 			if (encoded[i + j] != '=')
// 			{
// 				num = num << 6;
// 				count_bits += 6;
// 			}
// 			if (encoded[i + j] >= 'A' && encoded[i + j] <= 'Z')
// 				num = num | (encoded[i + j] - 'A');
// 			else if (encoded[i + j] >= 'a' && encoded[i + j] <= 'z')
// 				num = num | (encoded[i + j] - 'a' + 26);
// 			else if (encoded[i + j] >= '0' && encoded[i + j] <= '9')
// 				num = num | (encoded[i + j] - '0' + 52);
// 			else if (encoded[i + j] == '+')
// 				num = num | 62;
// 			else if (encoded[i + j] == '/')
// 				num = num | 63;
// 			else
// 			{
// 				num = num >> 2;
// 				count_bits -= 2;
// 			}
// 		}

// 		while (count_bits != 0)
// 		{
// 			count_bits -= 8;
// 			decoded_string[k++] = (num >> count_bits) & 255;
// 		}
// 	}
// 	decoded_string[k] = '\0';
// 	return decoded_string;
// }

// int UDPSetup()
// {
// 	sock = socket(AF_INET, SOCK_DGRAM, 0);
// 	if (sock < 0)
// 	{
// 		perror("socket error\n");
// 		return -1;
// 	}

// 	addr_len = sizeof(struct sockaddr_in);
// 	int broadcastEnable = 1;
// 	setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));

// 	memset((void *)&server_addr, 0, addr_len);
// 	server_addr.sin_family = AF_INET;
// 	server_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
// 	server_addr.sin_port = htons(PORT);

// 	ret = bind(sock, (struct sockaddr *)&server_addr, addr_len);
// 	if (ret < 0)
// 	{
// 		perror("bind error\n");
// 		return -1;
// 	}
// 	return 0;
// }

// void KillProcess(char *msg)
// {
// 	char GetPID[64];
// 	sprintf(GetPID, "pgrep -f '%s'", msg);
// 	ExecuteCMD(GetPID);
// 	char KillProcess[64];
// 	sprintf(KillProcess, "kill -9 %s", msg_rsp.c_str());
// 	ExecuteCMD(KillProcess);
// }

// string SetupMqqts(char *msg)
// {
// 	try
// 	{
// 		Document document;
// 		document.Parse(msg);
// 		const Value &DATA = document["DATA"];
// 		string RefreshToken = DATA["REFRESH_TOKEN"].GetString();
// 		string DormitoryId = DATA["DORMITORY_ID"].GetString();
// 		string token = GetToken(RefreshToken);
// 		string certification = GetCertification(token, DormitoryId);
// 		// download file
// 		string cmd = "wget --header=\"Authorization: Bearer " + token + "\" --header=\"X-DormitoryId: " + DormitoryId + "\" " + BASE + certification;
// 		ExecuteCMD(cmd.c_str());
// 		if (CheckFile("/etc/mosquitto/ca.crt"))
// 		{
// 			ExecuteCMD("rm /etc/mosquitto/ca.crt");
// 			ExecuteCMD("rm /etc/mosquitto/server.crt");
// 			ExecuteCMD("rm /etc/mosquitto/server.key");
// 		}
// 		ExecuteCMD("tar -xf *.tar.gz -C /etc/mosquitto");
// 		ExecuteCMD("rm /root/*.tar.gz");
// 		ExecuteCMD("rm /etc/mosquitto/mosquitto.conf");
// 		ExecuteCMD("echo -e \"listener 8883\ncafile /etc/mosquitto/ca.crt\ncertfile /etc/mosquitto/server.crt\nkeyfile /etc/mosquitto/server.key\" >> /etc/mosquitto/mosquitto.conf");
// 		ExecuteCMD("rm /etc/PassMqtt.txt");
// 		ExecuteCMD("/etc/init.d/mosquitto stop");
// 		ExecuteCMD("/etc/init.d/mosquitto start");
// 		KillProcess("SYSTEM");
// 		KillProcess("RD_SMART");
// 		KillProcess("python3 RDhcPy/main.py");
// 		ExecuteCMD("/bin/echo \"0\" > /sys/class/leds/linkit-smart-7688:orange:service/brightness");
// 		ExecuteCMD("/root/run.sh");
// 		StringBuffer SendToApp;
// 		Writer<StringBuffer> json(SendToApp);
// 		json.StartObject();
// 		json.Key("CMD");
// 		json.String("HC_CONNECT_TO_CLOUD");
// 		json.Key("DATA");
// 		json.StartObject();
// 		json.Key("TLS");
// 		json.Bool(true);
// 		json.Key("MQTT_PORT");
// 		json.Int(8883);
// 		json.Key("STATUS");
// 		json.String("READY");
// 		json.EndObject();
// 		json.EndObject();
// 		string s = SendToApp.GetString();
// 		return s;
// 	}
// 	catch (exception &e)
// 	{
// 		return "";
// 	}
// }

// bool CheckFile(const std::string &name)
// {
// 	if (FILE *file = fopen(name.c_str(), "r"))
// 	{
// 		fclose(file);
// 		return true;
// 	}
// 	else
// 	{
// 		return false;
// 	}
// }

// string ScanHC(char *msg)
// {
// 	Document document;
// 	document.Parse(msg);
// 	if (document.HasMember("DORMITORY_ID"))
// 	{
// 		string DormitoryId = document["DORMITORY_ID"].GetString();
// 		bool CheckDormitoryId = CheckIntoHome(DormitoryId.c_str());
// 		if (CheckDormitoryId)
// 		{
// 			ip = GetIP();
// 			cout << ip << endl;
// 			char mac[20] = {0};
// 			char VersionHC[20] = {0};
// 			char char_hostname[256] = {0};
// 			GetMacAddress(mac);
// 			GetVersionHC(VersionHC);
// 			gethostname(char_hostname, sizeof(char_hostname));
// 			cout << string(char_hostname) << endl;
// 			StringBuffer SendToApp;
// 			Writer<StringBuffer> json(SendToApp);
// 			json.StartObject();
// 			json.Key("CMD");
// 			json.String("HC_RESPONE");
// 			json.Key("IP");
// 			json.String(const_cast<char *>(ip.c_str()));
// 			json.Key("HOSTNAME");
// 			json.String(char_hostname);
// 			json.Key("MAC");
// 			json.String(mac);
// 			json.Key("VERSION");
// 			json.String(VersionHC);
// 			if (CheckFile("/etc/PassMqtt.txt"))
// 			{
// 				json.Key("TLS");
// 				json.Bool(true);
// 				json.Key("MQTT_PORT");
// 				json.Int(8883);
// 			}
// 			else
// 			{
// 				json.Key("TLS");
// 				json.Bool(false);
// 				json.Key("MQTT_PORT");
// 				json.Int(1883);
// 			}
// 			json.EndObject();
// 			string s = SendToApp.GetString();
// 			return s;
// 		}
// 	}
// 	else
// 	{
// 		ip = GetIP();
// 		cout << ip << endl;
// 		char mac[20] = {0};
// 		char VersionHC[20] = {0};
// 		char char_hostname[256] = {0};
// 		GetMacAddress(mac);
// 		GetVersionHC(VersionHC);
// 		gethostname(char_hostname, sizeof(char_hostname));
// 		cout << string(char_hostname) << endl;
// 		StringBuffer SendToApp;
// 		Writer<StringBuffer> json(SendToApp);
// 		json.StartObject();
// 		json.Key("CMD");
// 		json.String("HC_RESPONE");
// 		json.Key("IP");
// 		json.String(const_cast<char *>(ip.c_str()));
// 		json.Key("HOSTNAME");
// 		json.String(char_hostname);
// 		json.Key("MAC");
// 		json.String(mac);
// 		json.Key("VERSION");
// 		json.String(VersionHC);
// 		if (CheckFile("/etc/PassMqtt.txt"))
// 		{
// 			json.Key("TLS");
// 			json.Bool(true);
// 			json.Key("MQTT_PORT");
// 			json.Int(8883);
// 		}
// 		else
// 		{
// 			json.Key("TLS");
// 			json.Bool(false);
// 			json.Key("MQTT_PORT");
// 			json.Int(1883);
// 		}
// 		json.EndObject();
// 		string s = SendToApp.GetString();
// 		return s;
// 	}
// 	return "";
// }

// string GetHCInfo(string rqi)
// {
// 	ip = GetIP();
// 	char mac[20] = {0};
// 	char versionHC[20] = {0};
// 	char char_hostname[256] = {0};
// 	GetMacAddress(mac);
// 	GetVersionHC(versionHC);
// 	gethostname(char_hostname, sizeof(char_hostname));
// 	StringBuffer SendToApp;
// 	Writer<StringBuffer> json(SendToApp);
// 	json.StartObject();
// 	json.Key("cmd");
// 	json.String("getHcInfoRsp");
// 	json.Key("rqi");
// 	json.String(const_cast<char *>(rqi.c_str()));
// 	json.Key("data");
// 	json.StartObject();
// 	json.Key("ip");
// 	json.String(const_cast<char *>(ip.c_str()));
// 	json.Key("name");
// 	json.String(char_hostname);
// 	json.Key("mac");
// 	json.String(mac);
// 	json.Key("ver");
// 	json.String(versionHC);
// 	json.Key("isInHome");
// 	json.Bool(true);
// 	json.EndObject();
// 	json.EndObject();
// 	string s = SendToApp.GetString();
// 	return s;
// }

// void ProcessUDPMsg()
// {
// 	FD_ZERO(&readfd);
// 	FD_SET(sock, &readfd);
// 	ret = select(sock + 1, &readfd, NULL, NULL, 0);
// 	if (ret > 0)
// 	{
// 		if (FD_ISSET(sock, &readfd))
// 		{
// 			char buffer[1024] = {0};
// 			count = recvfrom(sock, buffer, 1024, 0, (struct sockaddr *)&client_addr, &addr_len);
// 			char *Msg = buffer;
// 			cout << Msg << endl;
// 			Document document;
// 			document.Parse(buffer);
// 			if (!document.IsObject())
// 			{
// 				Msg = Base64Decoder(buffer, strlen(buffer));
// 			}
// 			Document document1;
// 			document1.Parse(Msg);
// 			if (document1.IsObject())
// 			{
// 				if (document1.HasMember("CMD"))
// 				{
// 					string CMD = document1["CMD"].GetString();
// 					if (CMD.compare(SCAN_HC) == 0)
// 					{
// 						string MsgSendToApp = ScanHC(Msg);
// 						count = sendto(sock, MsgSendToApp.c_str(), MsgSendToApp.length(), 0, (struct sockaddr *)&client_addr, addr_len);
// 					}
// 					else if (CMD.compare(HC_SCAN_WIFI) == 0)
// 					{
// 						ScanWifi();
// 						for (list<string>::iterator it = rsp_wifi_data.begin(); it != rsp_wifi_data.end(); it++)
// 						{
// 							count = sendto(sock, StringToChar(*it), strlen(StringToChar(*it)), MSG_CONFIRM, (struct sockaddr *)&client_addr, addr_len);
// 						}
// 						rsp_wifi_data.clear();
// 					}
// 					else if (CMD.compare(HC_CONNECT_TO_CLOUD) == 0)
// 					{
// 						string Str_SetupMqtts = SetupMqqts(Msg);
// 						if (Str_SetupMqtts != "")
// 						{
// 							count = sendto(sock, Str_SetupMqtts.c_str(), Str_SetupMqtts.length(), 0, (struct sockaddr *)&client_addr, addr_len);
// 						}
// 					}
// 				}
// 				if (document1.HasMember("SSID") && document1.HasMember("PASSWORD") && document1.HasMember("ENCRYPTION"))
// 				{
// 					string ssid = document1["SSID"].GetString();
// 					string passwd = document1["PASSWORD"].GetString();
// 					string encryption = document1["ENCRYPTION"].GetString();
// 					SSID = ssid.c_str();
// 					PASSWORD = passwd.c_str();
// 					ENCRYPTION = encryption.c_str();
// 					ConnectToWifi();
// 				}
// 				if (document1.HasMember("cmd"))
// 				{
// 					string cmd = document1["cmd"].GetString();
// 					string rqi = document1["rqi"].GetString();
// 					string hcInfo = GetHCInfo(rqi);
// 					count = sendto(sock, hcInfo.c_str(), hcInfo.length(), 0, (struct sockaddr *)&client_addr, addr_len);
// 				}
// 			}
// 		}
// 	}
// }

// int main()
// {
// 	UDPSetup();
// 	while (1)
// 	{
// 		ProcessUDPMsg();
// 	}
// 	return 0;
// }

//============================================================================
// Name        : UDP18_06.cpp
// Author      : rd
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "UDP.h"

list<string> wifi_data;
list<string> rsp_wifi_data;
list<string> rsp_ip;

string msg_rsp = "";
string ip = "";
string mac = "";

bool flag_encryption = false;

const char *PASSWORD, *ENCRYPTION;
const char *SSID;

int sock = 0;
int ret = 0;
int count = 0;
fd_set readfd;
unsigned int addr_len;
struct sockaddr_in server_addr;
struct sockaddr_in client_addr;

char *StringToChar(string s)
{
	char *sendChar = new char[s.length() + 1];
	strcpy(sendChar, s.c_str());
	return sendChar;
}

void ExecuteCMD(char const *command)
{
	FILE *file;
	msg_rsp = "";
	cout << command << endl;
	stringstream msg;
	char msg_line[100] = {0};
	file = popen(command, "r");
	if (file == NULL)
	{
		exit(1);
	}
	fgets(msg_line, 100, file);
	msg_rsp += msg_line;
	while (1)
	{
		fgets(msg_line, 100, file);
		if (feof(file))
		{
			break;
		}
		msg_rsp += msg_line;
	}
	pclose(file);
}

void GetMacAddress(char *uc_Mac)
{
	struct ifreq s;
	unsigned char *mac = NULL;
	int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);

	strcpy(s.ifr_name, "eth0");
	if (0 == ioctl(fd, SIOCGIFHWADDR, &s))
	{
		mac = (unsigned char *)s.ifr_addr.sa_data;
	}
	sprintf((char *)uc_Mac, (const char *)"%.2x:%.2x:%.2x:%.2x:%.2x:%.2x",
			mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	puts(uc_Mac);
}

string GetIP()
{
	string string_ip_1;
	string string_ip_2;
	string string_ip_3;
	ExecuteCMD("ip -4 addr show ${link_name} | sed -Ene \'s/^.*inet ([0-9.]+)\\/.*$/\\1/p\'");
	int count = 0;
	const char *char_msg_rsp = msg_rsp.c_str();
	puts(char_msg_rsp);
	for (unsigned int i = 0; i < strlen(char_msg_rsp); i++)
	{
		if (char_msg_rsp[i] == '\n')
		{
			count++;
		}
	}
	printf("----%d----\n", count);
	if (count == 3)
	{
		int ip_index = 0;
		int count_line_index = 0;
		for (unsigned int i = 0; i < strlen(char_msg_rsp); i++)
		{
			if (char_msg_rsp[i] == '\n')
			{
				count_line_index++;
			}
			if (count_line_index == 0 && char_msg_rsp[i] != '\n')
			{
				string_ip_1 = string_ip_1 + char_msg_rsp[i];
			}
			if (count_line_index == 1 && char_msg_rsp[i] != '\n')
			{
				string_ip_2 = string_ip_2 + char_msg_rsp[i];
			}
			if (count_line_index == 2 && char_msg_rsp[i] != '\n')
			{
				string_ip_3 = string_ip_3 + char_msg_rsp[i];
				ip_index++;
			}
		}
		if (string_ip_1.compare("127.0.0.1") != 0 && string_ip_1.compare("10.10.10.1") != 0 && string_ip_1.compare("") != 0)
		{
			return string_ip_1;
		}
		if (string_ip_2.compare("127.0.0.1") != 0 && string_ip_2.compare("10.10.10.1") != 0 && string_ip_2.compare("") != 0)
		{
			return string_ip_2;
		}
		if (string_ip_3.compare("127.0.0.1") != 0 && string_ip_3.compare("10.10.10.1") != 0 && string_ip_3.compare("") != 0)
		{
			return string_ip_3;
		}
	}
	if (count == 2)
	{
		int count_line_index = 0;
		for (unsigned int i = 0; i < strlen(char_msg_rsp); i++)
		{
			if (char_msg_rsp[i] == '\n')
			{
				count_line_index++;
			}
			if (count_line_index == 0 && char_msg_rsp[i] != '\n')
			{
				string_ip_1 = string_ip_1 + char_msg_rsp[i];
			}
			if (count_line_index == 1 && char_msg_rsp[i] != '\n')
			{
				string_ip_2 = string_ip_2 + char_msg_rsp[i];
			}
		}
		if (string_ip_1.compare("127.0.0.1") != 0 && string_ip_1.compare("") != 0)
		{
			return string_ip_1;
		}
		if (string_ip_2.compare("127.0.0.1") != 0 && string_ip_2.compare("") != 0)
		{
			return string_ip_2;
		}
	}
	return "";
}

bool CheckIntoHome(const char *msg)
{
	FILE *file;
	msg_rsp = "";
	bool check = false;
	char Dormitory[100] = {0};
	file = fopen("/etc/Dormitory.txt", "a+");
	if (!file)
	{
		cout << "Can not open this file" << endl;
	}
	else
	{
		cout << "File is open" << endl;
	}
	fgets(Dormitory, 100, file);
	cout << Dormitory << endl;
	cout << strlen(Dormitory) << endl;
	if (strcmp(Dormitory, "") == 0 || Dormitory[0] == EOF)
	{
		fputs(msg, file);
		check = true;
	}
	if (strcmp(Dormitory, msg) == 0)
	{
		check = true;
	}
	fclose(file);
	return check;
}

bool CheckScanDevice()
{
	FILE *file;
	bool check = false;
	char temp[100] = {0};
	file = fopen("/etc/scanDevice", "a+");
	if (!file)
	{
		check = false;
	}
	else
	{
		cout << "File is open" << endl;
	}
	fgets(temp, 100, file);
	cout << temp[0] << endl;
	if (temp[0]=='1')
	{
		check = true;
	}
	fclose(file);
	return check;
}

void GetVersionHC(char *Version)
{
	FILE *file;
	msg_rsp = "";
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

void ConnectToWifi()
{
	char ssid[100];
	ExecuteCMD("uci del network.wan.ifname");
	sprintf(ssid, "uci set wireless.wifinet1.ssid=\"%s\"", SSID);
	ExecuteCMD("uci del wireless.wifinet1");
	ExecuteCMD("uci set wireless.wifinet1=wifi-iface");
	ExecuteCMD(ssid);
	ExecuteCMD("uci set wireless.wifinet1.mode='sta'");
	ExecuteCMD("uci set wireless.wifinet1.network='wan'");
	ExecuteCMD("uci set wireless.wifinet1.device='radio0'");
	char pass[100];
	sprintf(pass, "uci set wireless.wifinet1.key='%s'", PASSWORD);
	ExecuteCMD(pass);
	char encryption[100];
	if (strcmp(ENCRYPTION, "none") == 0)
	{
		sprintf(encryption, "uci set wireless.wifinet1.encryption='%s'", ENCRYPTION);
		ExecuteCMD(encryption);
		// ExecuteCMD("uci set wireless.default_radio0.mode='sta'");
		ExecuteCMD("uci commit wireless");
		ExecuteCMD("wifi");
		sleep(30);
		ip = GetIP();
		cout << ip << endl;
		if (strcmp(ip.c_str(), "10.10.10.1") == 0)
		{
			ExecuteCMD("uci del wireless.wifinet1");
			ExecuteCMD("uci set wireless.wifinet1=wifi-iface");
			// ExecuteCMD("uci set wireless.default_radio0.mode='ap'");
			ExecuteCMD("uci commit wireless");
			ExecuteCMD("uci commit network");
			ExecuteCMD("wifi");
			ExecuteCMD("/etc/init.d/network restart");
		}
	}
	if (strcmp(ENCRYPTION, "none") != 0)
	{
		ExecuteCMD("uci set wireless.wifinet1.encryption='psk2'");
		// ExecuteCMD("uci set wireless.default_radio0.mode='sta'");
		ExecuteCMD("uci commit wireless");
		ExecuteCMD("uci commit network");
		ExecuteCMD("wifi");
		ExecuteCMD("/etc/init.d/network restart");
		sleep(30);
		ip = GetIP();
		cout << ip << endl;
		if (strcmp(ip.c_str(), "10.10.10.1") == 0)
		{
			ExecuteCMD("uci del wireless.wifinet1");
			// ExecuteCMD("uci set wireless.default_radio0.mode='ap'");
			ExecuteCMD("uci commit network");
			ExecuteCMD("uci commit wireless");
			ExecuteCMD("wifi");
			ExecuteCMD("/etc/init.d/network restart");
		}
	}
}

void ScanWifi()
{
	ExecuteCMD("iwinfo wlan0 scan");
	ProcessData(msg_rsp);
}

void ProcessData(string msg_rsp)
{
	string BSS = "Cell ";
	size_t pos = 0;
	string wifi;
	while ((pos = msg_rsp.find(BSS)) != string::npos)
	{
		wifi = msg_rsp.substr(0, pos);
		msg_rsp.erase(0, pos + BSS.length());
		wifi_data.push_front(wifi);
	}
	for (list<string>::iterator it = wifi_data.begin(); it != wifi_data.end(); it++)
	{
		const char *it_char = (*it).c_str();
		puts(it_char);
		int line_index = 0;
		if (strlen(it_char) >= 10)
		{
			char SSID[200] = {0};
			char QUALITY[50] = {0};
			char ENCRYPTION[50] = {0};
			unsigned int Authentication = 0;
			unsigned int Authentication1 = 0;
			const char *sub;
			const char *sub1;
			sub1 = strstr(it_char, "/70");
			sub = strstr(it_char, "Quality");
			if (sub)
			{
				Authentication = sub - it_char;
			}
			if (sub1)
			{
				Authentication1 = sub1 - it_char;
			}
			int QUALITY_index = 0;
			int SSID_index = 0;
			int ENCRYPTION_index = 0;
			for (unsigned int i = 0; i < strlen(it_char); i++)
			{
				if (it_char[i] == '\n')
				{
					line_index++;
				}
				else if (line_index == 1 && it_char[i + 1] != '\n')
				{
					if (SSID_index > 17)
					{
						SSID[SSID_index - 18] = it_char[i];
						SSID_index++;
					}
					else
					{
						SSID_index++;
					}
				}
				else if (line_index == 3 && i >= Authentication && i < Authentication1)
				{
					if (QUALITY_index > 8)
					{
						QUALITY[QUALITY_index - 9] = it_char[i];
						QUALITY_index++;
					}
					else
					{
						QUALITY_index++;
					}
				}
				else if (line_index == 4)
				{
					if (ENCRYPTION_index > 21)
					{
						ENCRYPTION[ENCRYPTION_index - 22] = it_char[i];
						ENCRYPTION_index++;
					}
					else
					{
						ENCRYPTION_index++;
					}
				}
			}
			int int_QUALITY = 0;
			int_QUALITY = atoi(QUALITY);
			int_QUALITY = int_QUALITY * 100 / 70;
			const char *char_QUALITY;
			string tmp = to_string(int_QUALITY);
			char_QUALITY = tmp.c_str();
			StringBuffer SendToApp;
			Writer<StringBuffer> json(SendToApp);
			json.StartObject();
			json.Key("CMD");
			json.String("HC_RESPONE");
			json.Key("SSID");
			json.String(SSID);
			json.Key("QUALITY");
			json.String(char_QUALITY);
			json.Key("ENCRYPTION");
			json.String(ENCRYPTION);
			json.EndObject();
			string s = SendToApp.GetString();
			rsp_wifi_data.push_front(s);
			printf("----%d----\n", int_QUALITY);
			puts(SSID);
			puts(ENCRYPTION);
		}
	}
}

char *Base64Decoder(char encoded[], int len_str)
{
	char *decoded_string;
	decoded_string = (char *)malloc(sizeof(char) * SIZE);
	int i, j, k = 0;
	int num = 0;
	int count_bits = 0;
	for (i = 0; i < len_str; i += 4)
	{
		num = 0, count_bits = 0;
		for (j = 0; j < 4; j++)
		{
			if (encoded[i + j] != '=')
			{
				num = num << 6;
				count_bits += 6;
			}
			if (encoded[i + j] >= 'A' && encoded[i + j] <= 'Z')
				num = num | (encoded[i + j] - 'A');
			else if (encoded[i + j] >= 'a' && encoded[i + j] <= 'z')
				num = num | (encoded[i + j] - 'a' + 26);
			else if (encoded[i + j] >= '0' && encoded[i + j] <= '9')
				num = num | (encoded[i + j] - '0' + 52);
			else if (encoded[i + j] == '+')
				num = num | 62;
			else if (encoded[i + j] == '/')
				num = num | 63;
			else
			{
				num = num >> 2;
				count_bits -= 2;
			}
		}

		while (count_bits != 0)
		{
			count_bits -= 8;
			decoded_string[k++] = (num >> count_bits) & 255;
		}
	}
	decoded_string[k] = '\0';
	return decoded_string;
}

int UDPSetup()
{
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0)
	{
		perror("socket error\n");
		return -1;
	}

	addr_len = sizeof(struct sockaddr_in);

	memset((void *)&server_addr, 0, addr_len);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT);

	ret = bind(sock, (struct sockaddr *)&server_addr, addr_len);
	if (ret < 0)
	{
		perror("bind error\n");
		return -1;
	}
	return 0;
}

void KillProcess(char *msg)
{
	char GetPID[64];
	sprintf(GetPID, "pgrep -f '%s'", msg);
	ExecuteCMD(GetPID);
	char KillProcess[64];
	sprintf(KillProcess, "kill -9 %s", msg_rsp.c_str());
	ExecuteCMD(KillProcess);
}

string SetupMqqts(char *msg)
{
	try
	{
		Document document;
		document.Parse(msg);
		const Value &DATA = document["DATA"];
		string RefreshToken = DATA["REFRESH_TOKEN"].GetString();
		string DormitoryId = DATA["DORMITORY_ID"].GetString();
		string token = GetToken(RefreshToken);
		string certification = GetCertification(token, DormitoryId);
		// download file
		string cmd = "wget --header=\"Authorization: Bearer " + token + "\" --header=\"X-DormitoryId: " + DormitoryId + "\" " + BASE + certification;
		ExecuteCMD(cmd.c_str());
		if (CheckFile("/etc/mosquitto/ca.crt"))
		{
			ExecuteCMD("rm /etc/mosquitto/ca.crt");
			ExecuteCMD("rm /etc/mosquitto/server.crt");
			ExecuteCMD("rm /etc/mosquitto/server.key");
		}
		ExecuteCMD("tar -xf *.tar.gz -C /etc/mosquitto");
		ExecuteCMD("rm /root/*.tar.gz");
		ExecuteCMD("rm /etc/mosquitto/mosquitto.conf");
		ExecuteCMD("echo -e \"listener 8883\ncafile /etc/mosquitto/ca.crt\ncertfile /etc/mosquitto/server.crt\nkeyfile /etc/mosquitto/server.key\" >> /etc/mosquitto/mosquitto.conf");
		ExecuteCMD("rm /etc/PassMqtt.txt");
		ExecuteCMD("/etc/init.d/mosquitto stop");
		ExecuteCMD("/etc/init.d/mosquitto start");
		KillProcess("SYSTEM");
		KillProcess("RD_SMART");
		KillProcess("python3 RDhcPy/main.py");
		ExecuteCMD("/bin/echo \"0\" > /sys/class/leds/linkit-smart-7688:orange:service/brightness");
		ExecuteCMD("/root/run.sh");
		StringBuffer SendToApp;
		Writer<StringBuffer> json(SendToApp);
		json.StartObject();
		json.Key("CMD");
		json.String("HC_CONNECT_TO_CLOUD");
		json.Key("DATA");
		json.StartObject();
		json.Key("TLS");
		json.Bool(true);
		json.Key("MQTT_PORT");
		json.Int(8883);
		json.Key("STATUS");
		json.String("READY");
		json.EndObject();
		json.EndObject();
		string s = SendToApp.GetString();
		return s;
	}
	catch (exception &e)
	{
		return "";
	}
}

bool CheckFile(const std::string &name)
{
	if (FILE *file = fopen(name.c_str(), "r"))
	{
		fclose(file);
		return true;
	}
	else
	{
		return false;
	}
}

string ScanHC(char *msg)
{
	Document document;
	document.Parse(msg);
	if (document.HasMember("DORMITORY_ID"))
	{
		string DormitoryId = document["DORMITORY_ID"].GetString();
		bool CheckDormitoryId = CheckIntoHome(DormitoryId.c_str());
		if (CheckDormitoryId)
		{
			ip = GetIP();
			cout << ip << endl;
			char mac[20] = {0};
			char VersionHC[20] = {0};
			char char_hostname[256] = {0};
			GetMacAddress(mac);
			GetVersionHC(VersionHC);
			gethostname(char_hostname, sizeof(char_hostname));
			cout << string(char_hostname) << endl;
			StringBuffer SendToApp;
			Writer<StringBuffer> json(SendToApp);
			json.StartObject();
			json.Key("CMD");
			json.String("HC_RESPONE");
			json.Key("IP");
			json.String(const_cast<char *>(ip.c_str()));
			json.Key("HOSTNAME");
			json.String(char_hostname);
			json.Key("MAC");
			json.String(mac);
			json.Key("VERSION");
			json.String(VersionHC);
			if (CheckFile("/etc/PassMqtt.txt"))
			{
				json.Key("TLS");
				json.Bool(true);
				json.Key("MQTT_PORT");
				json.Int(8883);
			}
			else
			{
				json.Key("TLS");
				json.Bool(false);
				json.Key("MQTT_PORT");
				json.Int(1883);
			}
			json.EndObject();
			string s = SendToApp.GetString();
			return s;
		}
	}
	else
	{
		ip = GetIP();
		cout << ip << endl;
		char mac[20] = {0};
		char VersionHC[20] = {0};
		char char_hostname[256] = {0};
		GetMacAddress(mac);
		GetVersionHC(VersionHC);
		gethostname(char_hostname, sizeof(char_hostname));
		cout << string(char_hostname) << endl;
		StringBuffer SendToApp;
		Writer<StringBuffer> json(SendToApp);
		json.StartObject();
		json.Key("CMD");
		json.String("HC_RESPONE");
		json.Key("IP");
		json.String(const_cast<char *>(ip.c_str()));
		json.Key("HOSTNAME");
		json.String(char_hostname);
		json.Key("MAC");
		json.String(mac);
		json.Key("VERSION");
		json.String(VersionHC);
		if (CheckFile("/etc/PassMqtt.txt"))
		{
			json.Key("TLS");
			json.Bool(true);
			json.Key("MQTT_PORT");
			json.Int(8883);
		}
		else
		{
			json.Key("TLS");
			json.Bool(false);
			json.Key("MQTT_PORT");
			json.Int(1883);
		}
		json.EndObject();
		string s = SendToApp.GetString();
		return s;
	}
	return "";
}

string GetHCInfo(string rqi)
{
	ip = GetIP();
	char mac[20] = {0};
	char versionHC[20] = {0};
	char char_hostname[256] = {0};
	GetMacAddress(mac);
	GetVersionHC(versionHC);
	gethostname(char_hostname, sizeof(char_hostname));
	StringBuffer SendToApp;
	Writer<StringBuffer> json(SendToApp);
	json.StartObject();
	json.Key("cmd");
	json.String("getHcInfoRsp");
	json.Key("rqi");
	json.String(const_cast<char *>(rqi.c_str()));
	json.Key("data");
	json.StartObject();
	json.Key("ip");
	json.String(const_cast<char *>(ip.c_str()));
	json.Key("name");
	json.String(char_hostname);
	json.Key("mac");
	json.String(mac);
	json.Key("ver");
	json.String(versionHC);
	json.Key("isInHome");
	json.Bool(true);
	json.EndObject();
	json.EndObject();
	string s = SendToApp.GetString();
	return s;
}

void ProcessUDPMsg()
{
	FD_ZERO(&readfd);
	FD_SET(sock, &readfd);
	ret = select(sock + 1, &readfd, NULL, NULL, 0);
	if (ret > 0)
	{
		if (FD_ISSET(sock, &readfd))
		{
			char buffer[1024] = {0};
			count = recvfrom(sock, buffer, 1024, 0, (struct sockaddr *)&client_addr, &addr_len);
			char *Msg = buffer;
			cout << Msg << endl;
			Document document;
			document.Parse(buffer);
			if (!document.IsObject())
			{
				Msg = Base64Decoder(buffer, strlen(buffer));
			}
			Document document1;
			document1.Parse(Msg);
			if (document1.IsObject())
			{
				if (document1.HasMember("CMD"))
				{
					string CMD = document1["CMD"].GetString();
					if (CMD.compare(SCAN_HC) == 0)
					{
						string MsgSendToApp = ScanHC(Msg);
						if (MsgSendToApp != "")
						{
							count = sendto(sock, MsgSendToApp.c_str(), MsgSendToApp.length(), 0, (struct sockaddr *)&client_addr, addr_len);
						}
					}
					else if (CMD.compare(HC_SCAN_WIFI) == 0)
					{
						ScanWifi();
						for (list<string>::iterator it = rsp_wifi_data.begin(); it != rsp_wifi_data.end(); it++)
						{
							count = sendto(sock, StringToChar(*it), strlen(StringToChar(*it)), MSG_CONFIRM, (struct sockaddr *)&client_addr, addr_len);
						}
						rsp_wifi_data.clear();
					}
					else if (CMD.compare(HC_CONNECT_TO_CLOUD) == 0)
					{
						string Str_SetupMqtts = SetupMqqts(Msg);
						if (Str_SetupMqtts != "")
						{
							count = sendto(sock, Str_SetupMqtts.c_str(), Str_SetupMqtts.length(), 0, (struct sockaddr *)&client_addr, addr_len);
						}
					}
				}
				if (document1.HasMember("SSID") && document1.HasMember("PASSWORD") && document1.HasMember("ENCRYPTION"))
				{
					string ssid = document1["SSID"].GetString();
					string passwd = document1["PASSWORD"].GetString();
					string encryption = document1["ENCRYPTION"].GetString();
					SSID = ssid.c_str();
					PASSWORD = passwd.c_str();
					ENCRYPTION = encryption.c_str();
					ConnectToWifi();
				}
				if (document1.HasMember("cmd") && CheckScanDevice())
				{
					string cmd = document1["cmd"].GetString();
					string rqi = document1["rqi"].GetString();
					string hcInfo = GetHCInfo(rqi);
					count = sendto(sock, hcInfo.c_str(), hcInfo.length(), 0, (struct sockaddr *)&client_addr, addr_len);
				}
			}
		}
	}
}

int main()
{
	UDPSetup();
	while (1)
	{
		ProcessUDPMsg();
	}
	return 0;
}
