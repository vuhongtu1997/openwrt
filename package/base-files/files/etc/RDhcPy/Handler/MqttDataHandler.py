import http
from Contracts.IHandler import IHandler
from Contracts.ITransport import ITransport
from Cache.GlobalVariables import GlobalVariables
import Constant.constant as const
from Database.Db import Db
from Model.userData import userData
from HcServices.Http import Http

import logging
import json
import sqlite3
import requests
import subprocess
import os
import time


def sql_update(cmd):
    con = sqlite3.connect("rd.Sqlite")
    cur = con.cursor()
    cur.execute(cmd)
    con.commit()
    con.close()


tokenTime = 0

#   ==============================================================================================


class MqttDataHandler(IHandler):
    __logger: logging.Logger
    __signalr: ITransport
    __globalVariables: GlobalVariables
    __mqtt: ITransport

    def __init__(self, log: logging.Logger, signalr: ITransport, mqtt: ITransport):
        self.__logger = log
        self.__globalVariables = GlobalVariables()
        self.__signalr = signalr
        self.__mqtt = mqtt

    def handler_signalr_command(self, item):
        pass

    def handler_signalr_response(self, item):
        pass

    def handler_mqtt_command(self, msg):
        self.__handler_topic_hc_control(msg)
        return

    def handler_mqtt_response(self, msg):
        self.__handler_topic_hc_control_response(msg)
        return

    def __handler_topic_hc_control(self, data):
        print(f"SignalR >> MQTT : HC.CONTROL :\n{data}\n")
        try:
            json_data = json.loads(data)
            cmd = json_data.get("CMD", "")
            dt = json_data.get("DATA", "")
            if (cmd == "UPDATE_FIRMWARE"):
                self.__handler_cmd_update_firmware(json_data)
            switcher = {
                "HC_CONNECT_TO_CLOUD": self.__handler_cmd_hc_connect_to_cloud,
                "RESET_HC": self.__handler_cmd_reset_hc,
                "AUTO_UPDATE": self.__handler_cmd_auto_update,
                "HC_BACKUP_DATA": self.__handler_cmd_backup_data,
                "HC_RESTORE_DATA": self.__handler_cmd_restore_data,
            }
            func = switcher.get(cmd)
            func(dt)
        except:
            pass

    def __handler_topic_hc_control_response(self, data):
        if self.__globalVariables.AllowChangeCloudAccountFlag:
            return
        print(
            f"RD_SMART >> MQTT : HC.CONTROL.RESPONSE :\n{data}\n")
        if self.__globalVariables.SignalrConnectSuccessFlag:
            try:
                json_data = json.loads(data)
                cmd = json_data.get("CMD", "")
                dt = json_data.get("DATA", "")
                self.__hc_check_cmd_and_send_response_to_cloud(cmd, data)
                switcher = {
                    "DEVICE": self.__handler_cmd_device,
                    # "DEVICE_UPDATE_STATUS": self.__handler_cmd_device_status,
                    "DEVICE_UPDATE": self.__handler_cmd_device,
                    "BACKUP": self.__handler_cmd_backup_response,
                    "DEVICE_LOG": self.__handler_cmd_device_log,
                    "RULE_LOG": self.__handler_cmd_rule_log,
                    "SCENE_LOG": self.__handler_cmd_scene_log,
                    "GROUP_LOG": self.__handler_cmd_group_log,
                }
                func = switcher.get(cmd)
                func(dt)
            except:
                pass

    def __handler_cmd_device(self, data):
        if self.__globalVariables.AllowChangeCloudAccountFlag:
            return
        signal_data = []
        try:
            for d in data:
                for i in d["PROPERTIES"]:
                    data_send_to_cloud = {
                        "deviceId": d["DEVICE_ID"],
                        "deviceAttributeId": i["ID"],
                        "value": i["VALUE"],
                    }
                    signal_data.append(data_send_to_cloud)
        except:
            self.__logger.error("\nData Attached With Device CMD Invalid")
            print("Data Attached With Device CMD Invalid")
            return

        if signal_data:
            size_arr = const.SIZE_OF_FRAME_DATA
            element = [
                signal_data[i: i + size_arr]
                for i in range(0, len(signal_data), size_arr)
            ]

            for i in range(len(element)):
                send_data = [
                    const.SIGNALR_CLOUD_RESPONSE_ENTITY,
                    json.dumps(element[i]),
                ]
                self.__signalr.send_response_data_queue.put(send_data)
        else:
            return

    def __handler_cmd_hc_connect_to_cloud(self, data):
        db = Db()
        refresh_token = data.get("REFRESH_TOKEN", "")
        dormitory_id = data.get("DORMITORY_ID", "")
        if self.__globalVariables.DormitoryId == "":
            self.__globalVariables.DormitoryId = dormitory_id
        longitude = data.get("LONGITUDE")
        latitude = data.get("LATITUDE")
        msg = {
            "CMD": "HC_CONNECT_TO_CLOUD",
            "DATA": {
                "SUCCESS": True,
            }
        }
        self.__mqtt.send(const.MQTT_RESPONSE_TOPIC, json.dumps(msg))
        if refresh_token is not None or refresh_token != "" or refresh_token != self.__globalVariables.RefreshToken:
            self.__globalVariables.SignalrConnectSuccessFlag = False
            self.__globalVariables.RefreshToken = refresh_token
        else:
            return
        self.__globalVariables.AllowChangeCloudAccountFlag = False
        if self.__globalVariables.SignalrConnectSuccessFlag:
            return
        self.__globalVariables.SignalrConnectSuccessFlag = False
        user_data = userData(
            refreshToken=self.__globalVariables.RefreshToken,
            dormitoryId=self.__globalVariables.DormitoryId,
            allowChangeAccount=False,
        )
        rel = db.Services.UserdataServices.FindUserDataById(id=1)
        dt = rel.first()
        if dt is not None:
            db.Services.UserdataServices.UpdateUserDataById(
                id=1, newUserData=user_data)
        if dt is None:
            db.Services.UserdataServices.AddNewUserData(newUserData=user_data)
            cmd = (
                "UPDATE UserData SET Longitude = "
                + str(longitude)
                + ", Latitude = "
                + str(latitude)
                + " WHERE Id = 1 "
            )
            sql_update(cmd)
            return

    def __handler_cmd_reset_hc(self, data):
        print("Allow To Change Account. Now New Account Can Log In")
        self.__logger.info(
            "Allow To Change Account. Now New Account Can Log In")

        db = Db()
        self.__globalVariables.AllowChangeCloudAccountFlag = True

        rel = db.Services.UserdataServices.FindUserDataById(id=1)
        dt = rel.first()
        if dt is None:
            return
        user_data = userData(
            refreshToken=self.__globalVariables.RefreshToken,
            dormitoryId=self.__globalVariables.DormitoryId,
            allowChangeAccount=self.__globalVariables.AllowChangeCloudAccountFlag,
        )

        db.Services.UserdataServices.UpdateUserDataById(
            id=1, newUserData=user_data)

    def __hc_check_cmd_and_send_response_to_cloud(self, cmd: str, data: str):
        room_response_cmd = [
            "CREATE_ROOM",
            "ADD_DEVICE_TO_ROOM",
            "REMOVE_DEVICE_FROM_ROOM",
        ]
        scene_response_cmd = ["CREATE_SCENE", "EDIT_SCENE", "SCENE"]
        if room_response_cmd.count(cmd) > 0:
            if data:
                send_data = [
                    const.SIGNALR_APP_ROOM_RESPONSE_ENTITY,
                    data,
                ]
                self.__signalr.send_response_data_queue.put(send_data)
            else:
                return
            return

        if scene_response_cmd.count(cmd) > 0:
            if data:
                send_data = [
                    const.SIGNALR_APP_SCENE_RESPONSE_ENTITY,
                    data,
                ]
                self.__signalr.send_response_data_queue.put(send_data)
            else:
                return
            return

        if (room_response_cmd + scene_response_cmd).count(cmd) == 0:
            signalr_data = []
            msg = json.loads(data)
            data_conv = msg["DATA"]
            if isinstance(data_conv, dict):
                send_data = [
                    const.SIGNALR_APP_DEVICE_RESPONSE_ENTITY,
                    data,
                ]
                self.__signalr.send_response_data_queue.put(send_data)
                return
            else:
                try:
                    for d in data_conv:
                        signalr_data.append(d)
                except:
                    self.__logger.error(
                        "\nData Attached With Device CMD Invalid")
                    print("Data Attached With Device CMD Invalid")
                    return

                if signalr_data:
                    size_arr = const.SIZE_OF_FRAME_DEVICE_DATA
                    element = [
                        signalr_data[i: i + size_arr]
                        for i in range(0, len(signalr_data), size_arr)
                    ]
                    for i in range(len(element)):
                        cmd = {"CMD": "DEVICE", "DATA": element[i]}
                        send_data = [
                            const.SIGNALR_APP_DEVICE_RESPONSE_ENTITY,
                            json.dumps(cmd),
                        ]
                        self.__signalr.send_response_data_queue.put(send_data)
                else:
                    return

    def __handler_cmd_device_status(self, data):
        if self.__globalVariables.AllowChangeCloudAccountFlag:
            return
        signal_data = []
        try:
            for d in data:
                for i in d["PROPERTIES"]:
                    data_send_to_cloud = {
                        "deviceId": d["DEVICE_ID"],
                        "deviceAttributeId": i["ID"],
                        "value": i["VALUE"],
                    }
                    signal_data.append(data_send_to_cloud)
        except:
            self.__logger.error("\nData Attached With Device CMD Invalid")
            print("Data Attached With Device CMD Invalid")
            return

        if signal_data:
            size_arr = const.SIZE_OF_FRAME_DATA
            element = [
                signal_data[i: i + size_arr]
                for i in range(0, len(signal_data), size_arr)
            ]

            for i in range(len(element)):
                send_data = [
                    const.SIGNALR_CLOUD_RESPONSE_ENTITY,
                    json.dumps(element[i]),
                ]
                self.__signalr.send_response_data_queue.put(send_data)
        if data:
            send_data = [
                const.SIGNALR_CLOUD_RESPONSE_ENTITY,
                data,
            ]
            self.__signalr.send_response_data_queue.put(send_data)

        else:
            return
        return

    def __handler_cmd_update_firmware(self, cmd):
        if "TO" in cmd:
            dest = cmd.get('TO')
            mac = dest.get('MAC')
            if mac != self.__globalVariables.GatewayMac:
                return
        data = cmd.get("DATA", "")
        print("Start update firmware")
        self.__logger.info("Start update firmware")
        os.system("rm /root/*.xz")
        try:
            os.system("opkg update")
            os.system("pip3 install packaging")
            os.system("opkg upgrade tar")
            file = open("/etc/version.txt", "r")
            current_ver = file.read().strip()
            print(f"Current version: {current_ver}")
            file.close()
            from packaging import version

            lastest_ver = data[-1]
            lastest_ver_name = lastest_ver.get("NAME")

            os.system("mkdir /etc/RECOVERY")
            os.system("rm /root/*.ipk")

            if version.parse(lastest_ver_name) > version.parse(current_ver):
                link = lastest_ver.get("URL")
                file_name = link[link.rfind("/") + 1:]
                link_dl = "wget " + const.SERVER_HOST + link
                os.system(link_dl)

                process = subprocess.Popen(
                    ["sha256sum", f"{file_name}"],
                    stdout=subprocess.PIPE,
                    universal_newlines=True,
                )
                output = process.stdout.readline()
                src = output.strip()
                check_sum = lastest_ver.get("CHECK_SUM") + "  " + file_name

                if src == check_sum:
                    print(file_name)
                    try:
                        os.system(f"tar -xf {file_name}")
                    except OSError:
                        print("error")
                    # move old file to dir /etc/RECOVERY
                    os.system("mv /root/RDhcPy/ /etc/RECOVERY")
                    os.system("mv /root/*.ipk /etc/RECOVERY")
                    os.system("mv /root/version.txt /etc/RECOVERY")
                    os.system(f"rm /root/{file_name}")

                    # move new file to dir root
                    os.system(f"mv /root/{lastest_ver_name}/* /root/")
                    os.system(f"rm -r /root/{lastest_ver_name}/")

                    # handle condition version required

                    file = open("/root/version.txt", "r")
                    str_ver = file.read().strip()
                    list_vers = str_ver.split("-")
                    print(list_vers)
                    file.close()

                    # required list version
                    req_list_vers = []
                    for ver in list_vers:
                        if version.parse(ver) > version.parse(current_ver):
                            req_list_vers.append(ver)
                    print(req_list_vers)

                    for req_ver in req_list_vers:
                        for d in data:
                            if req_ver == d.get("NAME"):
                                link_sub = d.get("URL")
                                file_sub_name = link_sub[link_sub.rfind(
                                    "/") + 1:]
                                link_sub_dl = "wget " + const.SERVER_HOST + link_sub
                                os.system(link_sub_dl)

                                process = subprocess.Popen(
                                    ["sha256sum", f"{file_sub_name}"],
                                    stdout=subprocess.PIPE,
                                    universal_newlines=True,
                                )
                                output = process.stdout.readline()
                                src = output.strip()
                                print(src)
                                check_sum = d.get(
                                    "CHECK_SUM") + "  " + file_sub_name
                                print(check_sum)
                                if src == check_sum:
                                    print("Start install sub-version")
                                    os.system(f"tar -xf /root/{file_sub_name}")

                                    # move old file to dir /etc/RECOVERY
                                    os.system("rm -r /etc/RECOVERY/*")
                                    os.system("mv /root/RDhcPy/ /etc/RECOVERY")
                                    os.system("mv /root/*.ipk /etc/RECOVERY")
                                    os.system(
                                        "mv /root/version.txt /etc/RECOVERY")
                                    os.system(f"rm /root/{file_sub_name}")

                                    # move new file to dir root
                                    os.system(f"mv /root/{req_ver}/* /root/")
                                    os.system(f"rm -r /root/{req_ver}/")

                                    # install new file
                                    os.system("opkg install /root/*.ipk")

                                    # delete /etc/RECOVERY
                                    os.system("rm -r /etc/RECOVERY/*")

                                    file = open("/etc/version.txt", "w")
                                    file.write(req_ver)
                                    file.close()
                                    os.system("chmod +x /root/config.sh")
                                    os.system("/root/config.sh")
                                    os.system("rm /root/config.sh")
                                    os.system("rm /root/*.ipk")
                                    os.system("rm /root/version.txt")
                    time.sleep(4)
                    os.system("reboot -f")
        except:
            print("Can Not Update Firmware")
            self.__logger.error("Can not update firmware")

    def __handler_cmd_auto_update(self, data):
        file = open("/etc/version.txt", "r")
        current_ver = file.read().strip()
        file.close()
        payload = {"id": self.__globalVariables.HcId}
        print(payload)
        firmware_url = const.SERVER_HOST + const.CLOUD_CHECK_AUTO_UPDATE_STATUS_URL
        headers = {
            "X-DormitoryId": self.__globalVariables.DormitoryId,
            "Cookie": f"Token={self.__get_token()}",
            "Content-Type": "application/json",
        }
        try:
            response = requests.request(
                "POST", firmware_url, headers=headers, data=json.dumps(payload)
            )
            resp = json.loads(response.text)
            status = resp["autoUpdate"]
            if status == True:
                get_auto_update_firmware_url = (
                    const.SERVER_HOST + const.CLOUD_CHECK_AUTO_UPDATE_URL
                )
                headers = {
                    "X-DormitoryId": self.__globalVariables.DormitoryId,
                    "Cookie": f"Token={self.__get_token()}",
                    "Content-Type": "application/json",
                }
                payload = {}

                return_data = requests.request(
                    "POST",
                    get_auto_update_firmware_url,
                    headers=headers,
                    data=payload,
                )
                self.__logger.info(
                    f"Get Auto Update Firmware: {return_data.status_code}"
                )
                print(
                    f"Auto update firmware \t {return_data.status_code}")
                return_info = return_data.json()
                cmd = []
                if return_info["version"] == current_ver:
                    return
                element = {
                    "NAME": return_info["name"],
                    "CHECK_SUM": return_info["checksum"],
                    "URL": return_info["url"],
                }
                cmd.append(element)
                self.__handler_cmd_update_firmware(cmd)
            else:
                print("HC Auto Update Status Response: Disable")
                self.__logger.info("HC Auto Update Status Response: Disable")
                return
        except Exception as e:
            print(f"Exception: {e}")
            pass

    def __get_token(self):
        refresh_token = self.__globalVariables.RefreshToken
        h = Http()
        token_url = const.SERVER_HOST + const.TOKEN_URL
        cookie = f"RefreshToken={refresh_token}"
        headers = h.create_new_http_header(
            cookie=cookie, domitory_id=self.__globalVariables.DormitoryId
        )
        token = ""
        try:
            res = h.post(token_url, headers=headers, data={}).json()
            token = res["token"]
        except Exception as e:
            print(f"Exception Get Token: {e}")
            self.__logger.error(f"Exception Get Token: {e}")
        return token

    def __handler_cmd_backup_data(self, data):
        token = self.__get_token()
        backup_data_url = const.SERVER_HOST + const.HC_BACKUP_FILE_URL
        hc_id = data.get("HC_ID")
        if hc_id != self.__globalVariables.HcId:
            return
        headers = {
            "X-DormitoryId": self.__globalVariables.DormitoryId,
            "Cookie": f"Token={token}",
        }
        payload = {}
        files = [
            (
                "file",
                (
                    "rd.Sqlite",
                    open("/root/rd.Sqlite", "rb"),
                    "application/octet-stream",
                ),
            )
        ]
        print("Start Backup Data")
        resp_code = False
        try:
            res = requests.request(
                "POST", backup_data_url, headers=headers, data=payload, files=files
            )
            self.__logger.info(
                f"Uploaded Database To Server : {res.status_code}")
            print(f"Uploaded Database To Server: {res.json()}")
            info = res.json()
            url = info["url"]
            size = info["size"]
            if res.status_code == 200:
                if self.__handler_create_backup_data(token, url, size):
                    cmd = {
                        "CMD": "HC_BACKUP_DATA",
                        "DATA": {
                            "HC_ID": f"{self.__globalVariables.HcId}",
                            "STATUS": 1,
                        },
                    }
                    send_data = [
                        const.SIGNALR_CLOUD_RESPONSE_ENTITY,
                        json.dumps(cmd),
                    ]
                    self.__signalr.send_response_data_queue.put(send_data)
                    resp_code = True
        except Exception as e:
            print(f"Exception Backup Data: {e}")
            self.__logger.error(f"Exception Backup Data: {e}")
            pass
        return resp_code

    def __handler_create_backup_data(self, token, url, size):
        link = const.SERVER_HOST + const.HC_BACKUP_CREATE_BACKUP_URL
        payload = json.dumps(
            {
                "name": f"HC-{self.__globalVariables.GatewayMac}",
                "version": self.__globalVariables.CurrentVersion,
                "size": size,
                "url": url,
                "homeControllerId": self.__globalVariables.HcId,
            }
        )
        headers = {
            "X-DormitoryId": self.__globalVariables.DormitoryId,
            "Cookie": f"Token={token}",
            "Content-Type": "application/json",
        }
        response_code = False
        try:
            response = requests.request(
                "POST", link, headers=headers, data=payload)
            if response.status_code == 200:
                response_code = True
        except Exception as e:
            print(f"Exception Create Backup Data: {e}")
            self.__logger.error(f"Exception Create Backup Data: {e}")
        return response_code

    def __handler_cmd_restore_data(self, data):
        hc_id = data.get("HC_ID")
        if hc_id != self.__globalVariables.HcId:
            return
        try:
            url = data.get("BACKUP_URL")
            download_link = const.SERVER_HOST + url
            payload = {}
            headers = {
                "X-DormitoryId": self.__globalVariables.DormitoryId,
                "Cookie": f"Token={self.__get_token()}",
            }

            response = requests.request(
                "POST", download_link, headers=headers, data=payload
            )
            open("rd.Sqlite", "wb").write(response.content)
            msg = {"CMD": "BACKUP", "DATA": {"DIRECTORY": "/root/rd.Sqlite"}}
            self.__mqtt.send("RD_CONTROL", json.dumps(msg))
        except Exception as e:
            print(f"Download Database Failure With Error: {e}")
            self.__logger.error(f"Download database failure: {e}")

    def __handler_cmd_backup_response(self, data):
        if data.get("SUCCESS"):
            status = 1
            print("Restore Success")
            self.__logger.info("Restore Success")
        else:
            status = 0
            print("Restore Failure")
            self.__logger.info("Restore Failure")
        cmd = {
            "CMD": "HC_RESTORE_DATA",
            "DATA": {
                "HC_ID": self.__globalVariables.HcId,
                "DORMITORY_ID": self.__globalVariables.DormitoryId,
                "BACKUP_ID": 1,
                "STATUS": status,
            },
        }
        send_data = [
            const.SIGNALR_CLOUD_RESPONSE_ENTITY,
            json.dumps(cmd),
        ]
        self.__signalr.send_response_data_queue.put(send_data)
        pass

    def __handler_cmd_device_log(self, data):
        send_data = [
            const.HC_UPDATE_DEVICE_LOG,
            json.dumps(data),
        ]
        self.__signalr.send_response_data_queue.put(send_data)

    def __handler_cmd_rule_log(self, data):
        send_data = [
            const.HC_UPDATE_RULE_LOG,
            json.dumps(data),
        ]
        self.__signalr.send_response_data_queue.put(send_data)

    def __handler_cmd_scene_log(self, data):
        send_data = [
            const.HC_UPDATE_SCENE_LOG,
            json.dumps(data),
        ]
        self.__signalr.send_response_data_queue.put(send_data)

    def __handler_cmd_group_log(self, data):
        send_data = [
            const.HC_UPDATE_GROUP_LOG,
            json.dumps(data),
        ]
        self.__signalr.send_response_data_queue.put(send_data)
