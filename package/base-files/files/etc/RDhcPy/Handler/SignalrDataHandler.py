from Contracts.ITransport import ITransport
from Contracts.IHandler import IHandler
import logging
from Cache.GlobalVariables import GlobalVariables
import Constant.constant as const
import json


class SignalrDataHandler(IHandler):
    __logger: logging.Logger
    __mqtt: ITransport
    __globalVariables: GlobalVariables

    def __init__(self, log: logging.Logger, mqtt: ITransport):
        self.__logger = log
        self.__mqtt = mqtt
        self.__globalVariables = GlobalVariables()

    def handler_mqtt_command(self, item):
        pass

    def handler_mqtt_response(self, item):
        pass

    def handler_signalr_command(self, item):
        if self.__globalVariables.AllowChangeCloudAccountFlag:
            return

        dorId = item[0]
        entity = item[1]

        if dorId != self.__globalVariables.DormitoryId:
            return

        try:
            switcher = {
                const.SIGNALR_APP_COMMAND_ENTITY: self.__handler_log_signalr_command,
                const.SIGNALR_AUTO_UPDATE_FIRMWARE_ENTITY: self.__handler_signalr_auto_update_command,
                const.SIGNALR_APP_DEVICE_RESPONSE_ENTITY: self.__handler_log_signalr_response,
                const.SIGNALR_APP_ROOM_RESPONSE_ENTITY: self.__handler_log_signalr_response,
                const.SIGNALR_APP_SCENE_RESPONSE_ENTITY: self.__handler_log_signalr_response,
                const.SIGNALR_CLOUD_RESPONSE_ENTITY: self.__handler_log_signalr_response,
                const.SIGNALR_COMMAND_CONFIRM_ENTIRY: self.__handler_command_confirm,
            }
            func = switcher.get(entity)
            func(item)
        except:
            pass
        return

    def handler_signalr_response(self, item):
        pass

    def __handler_log_signalr_response(self, item):
        entity = item[1]
        data = item[2]
        print(f"Cloud >> HC : {entity} :\n{data}\n")
        pass

    def __handler_log_signalr_command(self, item):
        entity = item[1]
        data = item[2]
        print(f"Cloud >> HC : {entity} :\n{data}\n")
        self.__logger.info(f"Cloud >> HC : {entity} : {data}\n")
        try:
            self.__mqtt.send(const.MQTT_CONTROL_TOPIC, data)
        except:
            pass

    def __handler_signalr_auto_update_command(self, item):
        entity = item[1]
        data = item[2]
        self.__logger.info(
            f"SignalR Data - {entity} Entity: {data}\n")
        print(f"Cloud >> HC : {entity} :\n{data}\n")
        msg = {"CMD": "AUTO_UPDATE"}
        try:
            self.__mqtt.send(const.MQTT_CONTROL_TOPIC, json.dumps(msg))
        except:
            pass

    def __handler_command_confirm(self, item):
        msg = {
            "CMD": "HC_CONNECT_TO_CLOUD",
            "DATA": {
                "SUCCESS": True,
            }
        }
        try:
            self.__mqtt.send(const.MQTT_RESPONSE_TOPIC, json.dumps(msg))
        except:
            pass
