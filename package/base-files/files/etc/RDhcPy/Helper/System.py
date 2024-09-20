from tabnanny import check
from Helper.Terminal import execute, execute_with_result
from Database.Db import Db
from Cache.GlobalVariables import GlobalVariables
import datetime
from HcServices.Http import Http
from sqlalchemy import and_, or_
from HcServices.Http import Http
import Constant.constant as const
import logging

import re
import uuid
import time
import os
import sys
import signal


def time_split(time: datetime.datetime):
    m = str(time.month)
    if int(m) < 10:
        m = "0" + m

    d = str(time.day)
    if int(d) < 10:
        d = "0" + d

    update_day = int(str(time.year) + m + d)
    update_time = 60 * time.hour + time.minute
    return update_day, update_time


def ping_google():
    rel = execute_with_result("ping -c3 www.google.com|grep packet")[1]
    try:
        rel2 = rel.split(", ")
        rel3 = rel2[2].split(" ")
        r = rel3[0] == "0%"
    except:
        r = False
        print("Can not ping to Google")
    return r


def eliminate_current_progress():
    print(f"This program is killed now")
    logging.debug("This Program is killed now")
    # s = execute_with_result(f"ps | grep python3")
    # dt = s[1].split(" ")
    # current_progress_port = ""
    # for i in range(len(dt)):
    #     if dt[i] != "":
    #         current_progress_port = dt[i]
    #         break
    pid = os.getpid()
    os.system(
        '/bin/echo "0" > /sys/class/leds/linkit-smart-7688:orange:service/brightness'
    )
    os.kill(pid, signal.SIGTERM)
    # print(f"Kill -9 {current_progress_port}")
    # logging.debug(f"Kill -9 {current_progress_port}")
    # execute(f"kill -9 {current_progress_port}")
    sys.exit()


def check_and_kill_all_repeat_progress():
    s = execute_with_result(f"ps|grep python3")
    current_self_repeat_process_list_info = s[1].split("\n")
    current_self_repeat_process_list_port = []
    for i in range(len(current_self_repeat_process_list_info)):
        p = current_self_repeat_process_list_info[i].split(" ")
        if p[len(p) - 1] != "RDhcPy/main.py":
            continue
        current_self_repeat_process_list_port.append(p[1])

    if len(current_self_repeat_process_list_port) > 1:
        kill_all_cmd = "kill -9"
        for i in range(len(current_self_repeat_process_list_port)):
            kill_all_cmd = kill_all_cmd + " " + \
                current_self_repeat_process_list_port[i]
        execute(kill_all_cmd)


class System:
    __db = Db()
    __globalVariables = GlobalVariables()
    __logger = logging.Logger

    def __init__(self, logger: logging.Logger):
        self.__logger = logger

    def update_current_wifi_name(self):
        s = execute_with_result("iwinfo")
        dt = s[1].split("\n")
        if str(dt[0]).find("RD_HC") == -1:
            wifi_name_started_point = str(dt[0]).find('"') + 1
            wifi_name_ended_point = str(dt[0]).find(
                '"', wifi_name_started_point) - 1
            self.__globalVariables.CurrentWifiName = str(dt[0])[
                wifi_name_started_point: wifi_name_ended_point + 1
            ]
            print(
                f"Wifi          \t\t {self.__globalVariables.CurrentWifiName}")
            self.__logger.info(
                f"Wifi: {self.__globalVariables.CurrentWifiName}"
            )

    def check_wifi_change(self):
        s = execute_with_result("iwinfo")
        dt = s[1].split("\n")
        wifi_name = ""
        if str(dt[0]).find("RD_HC") == -1:
            wifi_name_started_point = str(dt[0]).find('"') + 1
            wifi_name_ended_point = str(dt[0]).find(
                '"', wifi_name_started_point) - 1
            wifi_name = str(dt[0])[
                wifi_name_started_point: wifi_name_ended_point + 1]
        if wifi_name != "" and wifi_name != self.__globalVariables.CurrentWifiName:
            self.__logger.info(
                f"Current Wifi Change From {self.__globalVariables.CurrentWifiName} To {wifi_name}"
            )
            print(
                f"Current Wifi Change From {self.__globalVariables.CurrentWifiName} To {wifi_name}"
            )
            print(f"Check Wifi Change ???")
            eliminate_current_progress()
            # self.__globalVariables.SignalrConnectSuccessFlag = False

    def __get_token(self, h: Http):
        refresh_token = self.__globalVariables.RefreshToken
        if refresh_token == "":
            return ""
        token_url = const.SERVER_HOST + const.TOKEN_URL
        cookie = f"RefreshToken={refresh_token}"
        headers = h.create_new_http_header(
            cookie=cookie, domitory_id=self.__globalVariables.DormitoryId
        )
        token = ""
        try:
            res = h.post(token_url, headers=headers, data={}).json()
            token = res["token"]
        except:
            pass
        return token

    def update_firmware_version_info_to_cloud(self, h: Http):
        import re
        import uuid

        token = self.__get_token(h)
        cookie = f"Token={token}"
        update_firmware_url = const.SERVER_HOST + const.SIGNALR_UPDATE_FIRMWARE_URL
        headers = h.create_new_http_header(
            cookie=cookie, domitory_id=self.__globalVariables.DormitoryId
        )
        file = open("/etc/version.txt", "r")
        current_ver = file.read().strip()
        file.close()
        mac = ":".join(re.findall("..", "%012x" % uuid.getnode()))
        firmware_report_body_data = {
            "macAddress": mac,
            "version": current_ver,
            "dormitoryId": self.__globalVariables.DormitoryId,
        }
        try:
            res = h.post(
                update_firmware_url,
                headers=headers,
                data=firmware_report_body_data,
            )
            self.__logger.info(f"Update firmware \t {res.json()}")
            print(f"Update firmware \t {res.json()}")
        except:
            self.__logger.error(f"Can Not Update Firmware Info")
            pass

    def send_http_request_to_gw_online_status_url(self, h: Http):
        token = self.__get_token(h)
        cookie = f"Token={token}"
        heartbeat_url = const.SERVER_HOST + const.SIGNALR_GW_HEARTBEAT_URL
        headers = h.create_new_http_header(
            cookie=cookie, domitory_id=self.__globalVariables.DormitoryId
        )
        mac = ":".join(re.findall("..", "%012x" % uuid.getnode()))
        payload = {"macAddress": mac}
        try:
            res = h.post(url=heartbeat_url, headers=headers, data=payload)
            data = res.json()
            self.__logger.info(f"Ping Cloud Status: {data}")
            print(f"Ping Cloud Status: {data}")
            return data
        except:
            print(f"Can Not Ping To Cloud")
            self.__logger.debug(f"Can Not Ping To Cloud")
            return False

    def check_auto_update_firmware(self, h: Http):
        token = self.__get_token(h)
        cookie = f"Token={token}"
        update_firmware_url = (
            const.SERVER_HOST + const.CLOUD_CHECK_AUTO_UPDATE_STATUS_URL
        )
        headers = h.create_new_http_header(
            cookie=cookie, domitory_id=self.__globalVariables.DormitoryId
        )
        # payload = {"id": self.__globalVariables.HcId}
        payload = {"macAddress": self.__globalVariables.GatewayMac}
        headers["Content-Type"] = "application/json"
        success = False
        name = ""
        latest_ver = ""
        checksum = ""
        cmd = {
            "success": success,
            "name": name,
            "url": latest_ver,
            "checksum": checksum,
        }
        try:
            res = h.post(
                update_firmware_url,
                headers=headers,
                data=payload,
            )
            if res.status_code == 200:
                status = res.json()["autoUpdate"]
                if status == True:
                    url = const.SERVER_HOST + const.CLOUD_CHECK_AUTO_UPDATE_URL
                    payload = {}
                    headers = {
                        "X-DormitoryId": self.__globalVariables.DormitoryId,
                        "Cookie": cookie,
                    }
                    return_data = h.post(url, headers=headers, data=payload)
                    self.__logger.info(
                        f"Auto update status \t {res.status_code}")
                    print(f"Auto update status \t {res.status_code}")
                    return_info = return_data.json()
                    if return_data.status_code == 200:
                        success = True
                    name = return_info["version"]
                    latest_ver = return_info["url"]
                    checksum = return_info["checksum"]
                    if name != self.__globalVariables.CurrentVersion:
                        cmd = {
                            "success": success,
                            "name": name,
                            "url": latest_ver,
                            "checksum": checksum,
                        }
                        return cmd
                    else:
                        return cmd
                else:
                    print("Auto update config \t Disable")
                    self.__logger.info("Auto update config \t Disable")
                    return cmd
        except:
            self.__logger.error(f"Check Auto Update Failed")
            return cmd
