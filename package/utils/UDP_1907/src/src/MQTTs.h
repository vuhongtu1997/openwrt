/*
 * SetupMQTTs.h
 *
 *  Created on: Mar 3, 2022
 *      Author: rd
 */

#ifndef MQTTS_H_
#define MQTTS_H_

#include <iostream>
#include <string>
#include <curl/curl.h>
#include <stdexcept>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#define BASE "https://iot-dev.truesight.asia"

using namespace rapidjson;
using namespace std;

string HTTPRequest(string method, string url, string header, string body);
string GetToken(string RefreshToken);
string GetCertification(string bearer_token, string dormitoryId);
string LoadCertification(string url, string bearer_token, string dormitoryId);

#endif /* MQTTS_H_ */
