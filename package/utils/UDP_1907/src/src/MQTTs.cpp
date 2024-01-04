/*
 * SetupMQTTs.cpp
 *
 *  Created on: Mar 3, 2022
 *      Author: rd
 */

#include "MQTTs.h"

string execute_command(const char *command)
{
	char buffer[128];
	string result = "";
	FILE *pipe = popen(command, "r");
	if (!pipe)
		throw std::runtime_error("popen() failed!");
	try
	{
		while (fgets(buffer, sizeof buffer, pipe) != NULL)
		{
			result += buffer;
		}
	}
	catch (...)
	{
		pclose(pipe);
		throw;
	}
	pclose(pipe);
	return result;
}

size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp)
{
	((string *)userp)->append((char *)contents, size * nmemb);
	return size * nmemb;
}

string HTTPRequest(string method, string url, string header, string body)
{
	curl_global_init(CURL_GLOBAL_ALL);

	CURL *easyhandle = curl_easy_init();
	string readBuffer;
	curl_easy_setopt(easyhandle, CURLOPT_CUSTOMREQUEST, method.c_str());
	curl_easy_setopt(easyhandle, CURLOPT_URL, url.c_str());
	struct curl_slist *HTTPHeaders = NULL;
	HTTPHeaders = curl_slist_append(HTTPHeaders, header.c_str());
	HTTPHeaders = curl_slist_append(HTTPHeaders, "Content-Type: application/json");
	curl_easy_setopt(easyhandle, CURLOPT_HTTPHEADER, HTTPHeaders);
	curl_easy_setopt(easyhandle, CURLOPT_POSTFIELDS, body.c_str());
	curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, &readBuffer);

	curl_easy_perform(easyhandle);
	curl_easy_cleanup(easyhandle);
	curl_slist_free_all(HTTPHeaders);
	// std::cout << readBuffer << std::endl;
	return readBuffer;
}

string GetToken(string RefreshToken)
{
	string method = "POST";
	string get_token_url = "https://iot-dev.truesight.asia/rpc/iot-ebe/account/renew-token";
	string get_token_header = "Cookie: RefreshToken="+RefreshToken+"";
	string get_token_body = "";
	string http_rsp = HTTPRequest(method, get_token_url, get_token_header, get_token_body);

	const char *json = http_rsp.c_str();
	Document resp_json;
	resp_json.Parse(json);
	if (resp_json.HasMember("token"))
	{
		string access_token = resp_json["token"].GetString();
		return access_token;
	}

	return http_rsp;
}

string GetCertification(string bearer_token, string dormitoryId)
{
	string method = "POST";
	string get_cert_url = "https://iot-dev.truesight.asia/rpc/iot-ebe/certification/get";
	string get_cert_header = "Authorization: Bearer " + bearer_token;
	string get_cert_body = "{\"dormitoryId\": \"" + dormitoryId + "\"}";
	string http_rsp = HTTPRequest(method, get_cert_url, get_cert_header, get_cert_body);

	const char *json = http_rsp.c_str();
	Document resp_json;
	resp_json.Parse(json);
	if (resp_json.HasMember("url"))
	{
		string load_url = resp_json["url"].GetString();
		return load_url;
	}

	return http_rsp;
}

string LoadCertification(string url, string bearer_token, string dormitoryId)
{
	string method = "POST";
	string load_cert_url = BASE + url;
	cout << load_cert_url << endl;
	string load_cert_header = "Authorization: Bearer " + bearer_token;
	string load_cert_body = "{\"dormitoryId\": \"" + dormitoryId + "\"}";
	string token = HTTPRequest(method, load_cert_url, load_cert_header, load_cert_body);

	return token;
}
