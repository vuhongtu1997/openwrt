from re import M
from Controller.RdHc import RdHc
import asyncio
from Database.Db import Db
import logging
import threading
from logging.handlers import TimedRotatingFileHandler
from HcServices.Http import Http
from HcServices.Signalr import Signalr
from HcServices.Mqtt import Mqtt
from Handler.MqttDataHandler import MqttDataHandler
from Handler.SignalrDataHandler import SignalrDataHandler
import os

d = os.path.dirname(__file__)

loghandler = TimedRotatingFileHandler(
    filename=d + "/Logging/runtime.log", when="MIDNIGHT", backupCount=4
)
logfomatter = logging.Formatter(
    fmt=("%(asctime)s\t" "%(levelname)s\t" "%(message)s"))
logger = logging.getLogger("mylog")
loghandler.setFormatter(logfomatter)
logger.addHandler(loghandler)
logger.propagate = False
logger.setLevel(logging.DEBUG)

http = Http()
mqtt = Mqtt(logger)
signalr = Signalr(logger, mqtt)

signalrHandler = SignalrDataHandler(logger, mqtt)
mqttHandler = MqttDataHandler(logger, signalr, mqtt)

db = Db()
hc = RdHc(logger, http, signalr, mqtt, mqttHandler, signalrHandler)


def thread_1():
    asyncio.run(hc.program_1())


def thread_2():
    asyncio.run(hc.program_2())


def thread_3():
    asyncio.run(signalr.connect())


def thread_4():
    asyncio.run(hc.program_4())


def thread_5():
    asyncio.run(hc.program_5())


def thread_6():
    asyncio.run(hc.program_6())


def main():
    print("Service start! <---")
    logger.info("Service start!")
    db.init()
    mqtt.connect()

    threads = list()

    threads.append(threading.Thread(target=thread_1, args=()))
    threads.append(threading.Thread(target=thread_2, args=()))
    threads.append(threading.Thread(target=thread_3, args=()))
    threads.append(threading.Thread(target=thread_4, args=()))
    threads.append(threading.Thread(target=thread_5, args=()))
    threads.append(threading.Thread(target=thread_6, args=()))

    [thread.start() for thread in threads]
    [thread.join() for thread in threads]


if __name__ == "__main__":
    main()
