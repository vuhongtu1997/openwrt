HttpStatusCodeOk = 200

# Cloud generic information

# SERVER_HOST = "https://iot-dev.truesight.asia"
# SERVER_SIGNALR_HOST = "https://iot-dev.truesight.asia"

# SERVER_HOST = "https://rallismartv2-staging.rangdong.com.vn"
# SERVER_SIGNALR_HOST = "https://rallismartv2-staging.rangdong.com.vn"

SERVER_HOST = "https://rallismartv2.rangdong.com.vn"
SERVER_SIGNALR_HOST = "https://rallismart-ebe.rangdong.com.vn"


# SignalR connection option
SIGNALR_SERVER_URL = "/rpc/iot-ebe/signalr/sync"
SIGNSLR_HEARDBEAT_URL = "/rpc/iot-ebe/sync/time"
SIGNALR_GW_HEARTBEAT_URL = "/rpc/iot-ebe/sync/hc/ping"
SIGNALR_APP_COMMAND_ENTITY = "Command"
SIGNALR_COMMAND_CONFIRM_ENTIRY = "Command-confirm"
SIGNALR_APP_DEVICE_RESPONSE_ENTITY = "DeviceResponse"
SIGNALR_APP_ROOM_RESPONSE_ENTITY = "RoomResponse"
SIGNALR_APP_SCENE_RESPONSE_ENTITY = "SceneResponse"
SIGNALR_CLOUD_RESPONSE_ENTITY = "HC-DeviceAttributeValue"
SIGNALR_UPDATE_FIRMWARE_URL = "/rpc/iot-ebe/sync/hc/update-version"
SIGNALR_ONLINE_ENTITY = "HC-Ping"
SIGNALR_AUTO_UPDATE_FIRMWARE_ENTITY = "HC-PublishFirmware"
HC_BACKUP_FILE_URL = "/rpc/iot-ebe/home-controller/upload-file"
HC_BACKUP_CREATE_BACKUP_URL = "/rpc/iot-ebe/home-controller/create-backup"
HC_UPDATE_DEVICE_LOG = "HC-DeviceActivityLog"
HC_UPDATE_RULE_LOG = "HC-RuleActivationEventLog"
HC_UPDATE_SCENE_LOG = "HC-SceneActivityLog"
HC_UPDATE_GROUP_LOG = "HC-GroupActivityLog"
# pull,push data url
CLOUD_PUSH_DATA_URL = "/rpc/iot-ebe/sync/hc/merge-device-attribute-value"
CLOUD_CHECK_FIRMWARE_URL = "/rpc/iot-ebe/firmware/list"
CLOUD_CHECK_AUTO_UPDATE_URL = "/rpc/iot-ebe/firmware/get-latest"
CLOUD_CHECK_AUTO_UPDATE_STATUS_URL = "/rpc/iot-ebe/home-controller/get"

# Server connection option
TOKEN_URL = "/rpc/iot-ebe/account/renew-token"

# Mqtt connection option
MQTT_PORT = 1883
MQTT_QOS = 2
MQTT_KEEPALIVE = 60
MQTT_CONTROL_TOPIC = "HC.CONTROL"
MQTT_RESPONSE_TOPIC = "HC.CONTROL.RESPONSE"
MQTT_USER = "RD"

# Sqlite connection option
DB_NAME = "rd.Sqlite"

# Check ext
HC_UPDATE_WEATHER_INTERVAL = 3619
HC_CHECK_SERVICE_INTERVAL = 59
HC_CHECK_WIFI_CHANGE_INTERVAL = 67
HC_REPORT_ONLINE_STATUS_INTERVAL = 311

SIZE_OF_FRAME_DATA = 40
SIZE_OF_FRAME_DEVICE_DATA = 15
