from signalrcore.hub_connection_builder import HubConnectionBuilder
import asyncio
import requests
from Cache.GlobalVariables import GlobalVariables
import Constant.constant as const
import logging
import threading
from Contracts.ITransport import ITransport
from Helper.System import eliminate_current_progress
import time
import os


def get_token():
    cache = GlobalVariables()
    try:
        renew_token_url = const.SERVER_SIGNALR_HOST + const.TOKEN_URL
        headers = {
            "Content-type": "application/json",
            "Accept": "text/plain",
            "X-DormitoryId": cache.DormitoryId,
            "User-Agent": "RANG DONG",
            "Cookie": "RefreshToken={refresh_token}".format(
                refresh_token=cache.RefreshToken
            ),
        }
        response = requests.request(
            "POST", renew_token_url, headers=headers, data={}
        ).json()
        token = response["token"]
        headers["Cookie"] = "Token={token}".format(token=token)
        return token
    except Exception as e:
        print(f"Exception While Getting SignalR Token: {e}")
        logging.Logger.error(f"Exception While Getting SignalR Token: {e}")
        return None


class Signalr(ITransport):
    __hub: HubConnectionBuilder
    __globalVariables: GlobalVariables
    __logger: logging.Logger
    __lock: threading.Lock
    __mqtt: ITransport

    def __init__(self, log: logging.Logger, mqtt: ITransport):
        super().__init__()
        self.__logger = log
        self.__globalVariables = GlobalVariables()
        self.__lock = threading.Lock()
        self.__hub = None
        self.__mqtt = mqtt

    def __build_connection(self):
        handler = logging.StreamHandler()
        handler.setLevel(logging.DEBUG)
        self.__hub = (
            HubConnectionBuilder()
            .with_url(
                const.SERVER_SIGNALR_HOST + const.SIGNALR_SERVER_URL,
                options={
                    "access_token_factory": get_token,
                    "headers": {},
                    # "keep_alive_interval": 10,
                },
            )
            .build()
            # .configure_logging(logging.DEBUG, socket_trace=True, handler=handler)
            # .with_automatic_reconnect(
            #     {
            #         "type": "interval",
            #         "keep_alive_interval": 10,
            #         "intervals": [1, 3, 5, 6, 7, 87, 3],
            #     }
            # )
        )
        self.__hub.on_open(lambda: self.handle_connect())
        self.__hub.on_close(lambda: self.reconnect())
        return self

    def handle_connect(self):
        os.system(
            '/bin/echo "1" > /sys/class/leds/linkit-smart-7688:orange:service/brightness'
        )
        print("SignalR connection \t Success")
        self.__logger.info("SignalR connection \t Success")

    def __on_receive_event(self):
        self.__hub.on("Receive", self.__receive_event_callback)

    def __receive_event_callback(self, data):
        with self.__lock:
            self.receive_command_data_queue.put(data)
            # if data[1] == const.SIGNALR_APP_COMMAND_ENTITY:

    async def disconnect(self):
        try:
            self.__hub.stop()
        except:
            eliminate_current_progress()

    def send(self, destination, data_send):
        entity = data_send[0]
        message = data_send[1]
        try:
            self.__hub.send("Send", [destination, entity, message])
        except:
            try:
                time.sleep(2)
                self.__hub.send("Send", [destination, entity, message])
            except:
                print("Can Not Send Data To Cloud")
                self.__logger.error("Can Not Send Data To Cloud")
                self.__globalVariables.SignalrConnectSuccessFlag = False

    async def connect(self):
        await asyncio.sleep(10)
        connect_success = False
        while self.__globalVariables.RefreshToken == "":
            await asyncio.sleep(1)
        while True:
            if not self.__globalVariables.SignalrConnectSuccessFlag:
                try:
                    if self.__hub is not None:
                        self.__hub.stop()
                        self.__hub = None
                        print("Clear SignalR Connect")
                        self.__logger.info("Clear SignalR Connect")
                    self.__build_connection()
                    connect_success = self.__hub.start()
                    if connect_success:
                        self.__globalVariables.SignalrConnectSuccessFlag = True
                        self.__on_receive_event()
                        await asyncio.sleep(5)
                        data = [
                            self.__globalVariables.DormitoryId,
                            const.SIGNALR_APP_COMMAND_ENTITY,
                            '{"CMD":"DEVICE_UPDATE"}',
                        ]
                        self.receive_command_data_queue.put(data)
                        time.sleep(1)
                        msg = {
                            "CMD": "HC_CONNECT_TO_CLOUD_CONFIRM",
                            "SUCCESS": True}

                        data_test = [
                            self.__globalVariables.DormitoryId,
                            const.SIGNALR_COMMAND_CONFIRM_ENTIRY,
                            msg,
                        ]
                        self.receive_command_data_queue.put(data_test)
                except Exception as err:
                    self.__logger.error(
                        f"Exception While Connecting Signalr to Server: {err}"
                    )
                    print(
                        f"Exception While Connecting Signalr to Server: {err}"
                    )
                    eliminate_current_progress()
            await asyncio.sleep(10)

    def reconnect(self):
        os.system(
            '/bin/echo "0" > /sys/class/leds/linkit-smart-7688:orange:service/brightness'
        )
        print("Signalr Disconnected")
        self.__logger.debug("Signalr Disconnected")
        self.__globalVariables.SignalrConnectSuccessFlag = False
        time.sleep(2)

    def receive(self):
        pass
