from HcServices.Http import Http
import asyncio
from Database.Db import Db
from Cache.GlobalVariables import GlobalVariables
import logging
from Contracts.ITransport import ITransport
from Helper.System import (
    System,
    ping_google,
    check_and_kill_all_repeat_progress,
)
from Contracts.IHandler import IHandler
import Constant.constant as Const
import time
import json
import random
import re
import uuid
import subprocess
import os

current_weather = {}


class RdHc:
    __httpServices: Http
    __signalServices: ITransport
    __mqttServices: ITransport
    __globalVariables: GlobalVariables
    __logger: logging.Logger
    __mqttHandler: IHandler
    __signalrHandler: IHandler

    def __init__(
        self,
        log: logging.Logger,
        http: Http,
        signalr: ITransport,
        mqtt: ITransport,
        mqtt_handler: IHandler,
        signalr_handler: IHandler,
    ):
        self.__logger = log
        self.__httpServices = http
        self.__signalServices = signalr
        self.__mqttServices = mqtt
        self.__globalVariables = GlobalVariables()
        self.__mqttHandler = mqtt_handler
        self.__signalrHandler = signalr_handler

    def __hc_handler_mqtt_control_data(self):
        if not self.__mqttServices.receive_command_data_queue.empty():
            item = self.__mqttServices.receive_command_data_queue.get()
            topic = item["topic"]
            msg = item["msg"]
            if topic == "HC.CONTROL":
                self.__mqttHandler.handler_mqtt_command(msg)
                self.__mqttServices.receive_command_data_queue.task_done()

    def __hc_handler_mqtt_response_data(self):
        if not self.__mqttServices.receive_response_data_queue.empty():
            item = self.__mqttServices.receive_response_data_queue.get()
            topic = item["topic"]
            msg = item["msg"]
            if topic == "HC.CONTROL.RESPONSE":
                self.__mqttHandler.handler_mqtt_response(msg)
                # self.__signalServices.receive_response_data_queue.task_done()

    def __hc_handler_signalr_command_data(self):
        if not self.__signalServices.receive_command_data_queue.empty():
            item = self.__signalServices.receive_command_data_queue.get()
            self.__signalrHandler.handler_signalr_command(item)

    def __hc_handler_signalr_response_data(self):
        if not self.__signalServices.receive_response_data_queue.empty():
            item = self.__signalServices.receive_response_data_queue.get()
            self.__signalrHandler.handler_signalr_response(item)
            self.__signalServices.receive_response_data_queue.task_done()

    # report time interval is 1800(FPT requirements)
    async def __hc_report_online_status_to_cloud(self):
        await asyncio.sleep(15)
        while True:
            mac = ":".join(re.findall("..", "%012x" % uuid.getnode()))
            cmd = {"macAddress": mac}
            send_data = [
                Const.SIGNALR_ONLINE_ENTITY,
                json.dumps(cmd),
            ]
            try:
                self.__signalServices.send_response_data_queue.put(send_data)
                print(
                    f"HC >> Cloud : PING                                                                              \n")
            except:
                pass
            await asyncio.sleep(Const.HC_REPORT_ONLINE_STATUS_INTERVAL)

    # load refresh token and dormitoryId from db in runtime
    def __hc_load_user_data(self):
        db = Db()
        user_data = db.Services.UserdataServices.FindUserDataById(id=1)
        dt = user_data.first()
        if dt is not None:
            self.__globalVariables.DormitoryId = dt["DormitoryId"]
            self.__globalVariables.RefreshToken = dt["RefreshToken"]
            self.__globalVariables.AllowChangeCloudAccountFlag = dt[
                "AllowChangeAccount"
            ]
        self.__globalVariables.SignalrConnectSuccessFlag = False
        print(
            f"DormitoryId \t\t {self.__globalVariables.DormitoryId}")
        print(
            f"RefreshToken \t\t {self.__globalVariables.RefreshToken}")

    # load current wifi SSID
    def __hc_load_current_wifi_name(self):
        s = System(self.__logger)
        s.update_current_wifi_name()

    def __hc_get_mac_address(self):
        mac = ":".join(re.findall("..", "%012x" % uuid.getnode()))
        self.__globalVariables.GatewayMac = mac
        print(
            f"Mac Address    \t\t {self.__globalVariables.GatewayMac}")

    def __hc_get_version(self):
        file = open("/etc/version.txt", "r")
        current_ver = file.read().strip()
        file.close()
        self.__globalVariables.CurrentVersion = current_ver
        print(
            f"Version    \t\t {self.__globalVariables.CurrentVersion}")

    def __hc_get_id(self):
        output = os.popen(
            f"echo -n {self.__globalVariables.GatewayMac} | md5sum | cut -d ' ' -f 1"
        ).readlines()
        str_id = output[0].replace("\n", "")
        hc_id = (
            str_id[0:8]
            + "-"
            + str_id[8:12]
            + "-"
            + str_id[12:16]
            + "-"
            + str_id[16:20]
            + "-"
            + str_id[20:32]
        )
        self.__globalVariables.HcId = hc_id
        print(f"HC ID      \t\t {self.__globalVariables.HcId}")
        self.__logger.info(f"Get HC ID: {self.__globalVariables.HcId}")

    # checking when wifi is changed
    async def __hc_check_wifi_change(self):
        s = System(self.__logger)
        await asyncio.sleep(2)
        while True:
            s.check_wifi_change()
            await asyncio.sleep(Const.HC_CHECK_WIFI_CHANGE_INTERVAL)

    def __get_weather(self):
        import os
        import time
        import requests
        import json

        db = Db()
        try:
            os.system("ntpd -q -p 1.openwrt.pool.ntp.org")
            time.sleep(2)
            os.system("hwclock -w")
            user_data = db.Services.UserdataServices.FindUserDataById(id=1)
            dt = user_data.first()
            if dt is not None:
                lat = dt["Latitude"]
                lon = dt["Longitude"]

            api_key_list = [
                "9e5c3f896fa33c7bb5f5b457256320d0",
                "05f40f1972e5ae4ff4fa2bbe2bd9adae",
                "ebd13e00acf60358e311499f1701ffc2",
                "818928201aaae0727494b4a67a9e8d8f",
                "4f3116907b2008008649f522a64b66e3",
                "90b27251096f146fb43a236a62285e73",
                "d6d3dd3b2c32bef84d3b651ec4a9312b",
                "1c876a574ac1c30b3af318704f055c8f",
                "a657910a4ec138aaee8cfe62db3c35a9",
                "2be04eff65745f3f5819daa17ffaf90b",
            ]
            api_key = api_key_list[random.randint(0, 9)]
            url = (
                "https://api.openweathermap.org/data/2.5/onecall?lat=%s&lon=%s&appid=%s&units=metric"
                % (lat, lon, api_key)
            )
            response = requests.get(url)
            data = json.loads(response.text)
            weather = data["current"]["weather"][0]
            cmd = {"cmd": "outWeather"}
            weather.update(cmd)
            temp = {"temp": data["current"]["temp"]}
            weather.update(temp)
            humi = {"humi": data["current"]["humidity"]}
            weather.update(humi)
            self.__mqttServices.send(
                Const.MQTT_CONTROL_TOPIC, json.dumps(weather))
            self.__globalVariables.CheckWeatherFlag = True
            self.__logger.info(f"Weather Info: {weather}")
        except:
            self.__logger.error(f"Can Not Get Weather Info")
            pass

    async def __hc_update_weather_status(self):
        while self.__globalVariables.PingGoogleSuccessFlag:
            try:
                print("Update Weather Info")
                self.__get_weather()
                await asyncio.sleep(Const.HC_UPDATE_WEATHER_INTERVAL)
            except:
                pass

    def __check_the_first_connect_internet(self):
        ping_waiting_time = 10
        send_weather_info_success_flag = False
        while not send_weather_info_success_flag:
            self.__globalVariables.PingGoogleSuccessFlag = ping_google()
            send_weather_info_success_flag = (
                self.__globalVariables.PingGoogleSuccessFlag
            )
            if self.__globalVariables.PingGoogleSuccessFlag:
                print("Internet status \t success")
                self.__logger.info("Internet Successfully !")
                if not self.__globalVariables.CheckWeatherFlag:
                    self.__get_weather()
                    self.__globalVariables.CheckWeatherFlag = True
                    return True
                else:
                    self.__globalVariables.CheckWeatherFlag = False
                    time.sleep(ping_waiting_time)

    def __hc_upload_version_info(self):
        s = System(self.__logger)
        send_version_success_flag = False
        while not send_version_success_flag:
            try:
                s.update_firmware_version_info_to_cloud(self.__httpServices)
                send_version_success_flag = True
            except Exception as e:
                print(f"Error While Updating Firmware Version: {e}")
                self.__logger.error(f"Error Updating Firmware Version: {e}")
                pass

    def __hc_check_auto_update(self):
        s = System(self.__logger)
        check_update_success_flag = False
        while not check_update_success_flag:
            try:
                resp = s.check_auto_update_firmware(self.__httpServices)
                check_update_success_flag = True
                if resp.get("success") == True:
                    cmd = {
                        "CMD": "UPDATE_FIRMWARE",
                        "DATA": [
                            {
                                "NAME": resp.get("name"),
                                "URL": resp.get("url"),
                                "CHECK_SUM": resp.get("checksum"),
                            }
                        ],
                    }
                    data = [
                        self.__globalVariables.DormitoryId,
                        Const.SIGNALR_APP_COMMAND_ENTITY,
                        json.dumps(cmd),
                    ]
                    self.__signalServices.receive_command_data_queue.put(data)
                else:
                    return
            except Exception as e:
                print(f"Error While Updating Firmware Version: {e}")
                self.__logger.error(f"Error Updating Firmware Version: {e}")
                return

    async def __process_send_signalr_data(self):
        while True:
            if (
                self.__globalVariables.SignalrConnectSuccessFlag
                and not self.__signalServices.send_response_data_queue.empty()
            ):
                try:
                    item = self.__signalServices.send_response_data_queue.get()
                    self.__signalServices.send(
                        self.__globalVariables.DormitoryId, item)
                    self.__signalServices.send_response_data_queue.task_done()
                except OverflowError as error:
                    print(f"Send message error: {error}")
            await asyncio.sleep(0.1)

    async def __process_command(self):
        while True:
            self.__hc_handler_signalr_command_data()
            self.__hc_handler_mqtt_control_data()
            time.sleep(0.05)

    async def __process_response(self):
        while True:
            self.__hc_handler_mqtt_response_data()
            self.__hc_handler_signalr_response_data()
            time.sleep(0.05)

    async def program_1(self):
        check_and_kill_all_repeat_progress()
        self.__hc_load_user_data()
        self.__hc_get_mac_address()
        self.__hc_get_id()
        self.__hc_get_version()
        self.__hc_load_current_wifi_name()
        self.__check_the_first_connect_internet()
        self.__hc_check_auto_update()
        self.__hc_upload_version_info()
        task0 = asyncio.create_task(self.__hc_update_weather_status())
        tasks = [task0]
        await asyncio.gather(*tasks)

    async def program_2(self):
        task0 = asyncio.create_task(self.__hc_check_wifi_change())
        task1 = asyncio.create_task(self.__process_send_signalr_data())
        tasks = [task0, task1]
        await asyncio.gather(*tasks)

    async def program_4(self):
        task0 = asyncio.create_task(self.__process_command())
        tasks = [task0]
        await asyncio.gather(*tasks)

    async def program_5(self):
        task0 = asyncio.create_task(self.__process_response())
        tasks = [task0]
        await asyncio.gather(*tasks)

    async def program_6(self):
        task0 = asyncio.create_task(self.__hc_report_online_status_to_cloud())
        tasks = [task0]
        await asyncio.gather(*tasks)
