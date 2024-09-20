cd /etc/mosquitto
rm -rf passwd.txt
CONFIG_MQTT_PASSWD
mosquitto_passwd -U passwd.txt
rm -rf mosquitto.conf
echo -e "#Default listener
port 1883
allow_anonymous false
password_file /etc/mosquitto/passwd.txt" >> mosquitto.conf
touch /etc/rc.button/count.txt
echo "0" >> /etc/rc.button/count.txt
/etc/init.d/mosquitto enable
/etc/init.d/mosquitto restart
rm /etc/rc.button/wps
touch /etc/rc.button/wps
echo -e "#!/bin/sh

[ \"\${ACTION}\" = \"released\" -o \"\${ACTION}\" = \"timeout\" ] || exit 25

. /lib/functions.sh

logger \"\$BUTTON pressed for \$SEEN seconds\"

# factory reset
if [ \"\$SEEN\" -gt 20 ]
then
#	[ -f /tmp/.factory_reset ] && return
#	touch /tmp/.factory_reset
	echo \"FACTORY RESET\" > /dev/console
	/bin/echo \"1\" > /sys/class/leds/linkit-smart-7688:orange:internet/brightness
	/bin/echo \"1\" > /sys/class/leds/linkit-smart-7688:orange:service/brightness
	/bin/echo \"1\" > /sys/class/leds/linkit-smart-7688:orange:ble1/brightness
	/bin/echo \"1\" > /sys/class/leds/linkit-smart-7688:orange:ble2/brightness
	RESET_BLE
	
elif [ \"\$SEEN\" -gt 10 ]
then
	echo \"REMOVE DORMITORYID\" > /dev/console
	/bin/echo \"1\" > /sys/class/leds/linkit-smart-7688:orange:internet/brightness
	sleep 1
	/bin/echo \"0\" > /sys/class/leds/linkit-smart-7688:orange:internet/brightness
	sleep 1
	/bin/echo \"1\" > /sys/class/leds/linkit-smart-7688:orange:internet/brightness
	sleep 1
	/bin/echo \"0\" > /sys/class/leds/linkit-smart-7688:orange:internet/brightness
	sleep 1
	/bin/echo \"1\" > /sys/class/leds/linkit-smart-7688:orange:internet/brightness
	sleep 1
	/bin/echo \"0\" > /sys/class/leds/linkit-smart-7688:orange:internet/brightness
	sleep 1
	rm /etc/Dormitory.txt
	
# wifi reset (back to ap mode)
elif [ \"\$SEEN\" -gt 5 ]
then
	echo \"RESET WIFI\" > /dev/console
	/bin/echo \"1\" > /sys/class/leds/linkit-smart-7688:orange:internet/brightness
	/bin/echo \"1\" > /sys/class/leds/linkit-smart-7688:orange:service/brightness
	sleep 1
	/bin/echo \"0\" > /sys/class/leds/linkit-smart-7688:orange:internet/brightness
	/bin/echo \"0\" > /sys/class/leds/linkit-smart-7688:orange:service/brightness
	sleep 1
	/bin/echo \"1\" > /sys/class/leds/linkit-smart-7688:orange:internet/brightness
	/bin/echo \"1\" > /sys/class/leds/linkit-smart-7688:orange:service/brightness
	sleep 1
	/bin/echo \"0\" > /sys/class/leds/linkit-smart-7688:orange:internet/brightness
	/bin/echo \"0\" > /sys/class/leds/linkit-smart-7688:orange:service/brightness
	sleep 1
	/bin/echo \"1\" > /sys/class/leds/linkit-smart-7688:orange:internet/brightness
	/bin/echo \"1\" > /sys/class/leds/linkit-smart-7688:orange:service/brightness
	sleep 1
	/bin/echo \"0\" > /sys/class/leds/linkit-smart-7688:orange:internet/brightness
	/bin/echo \"0\" > /sys/class/leds/linkit-smart-7688:orange:service/brightness
	sleep 1
	echo \"DISABLE WIFI STATION MODE\" > /dev/console
	uci set network.wan.ifname='eth0'
	uci commit network
	/etc/init.d/network restart
	uci del wireless.wifinet1
	uci commit wireless
	wifi
	
# print wifi information (show mac under ap mode / ip under sta mode
elif [ \"\$SEEN\" -lt 1 ]
then
	echo \"REBOOT\" > /dev/console
	if [ -e /etc/checkled ]
	then
		rm /etc/checkled
	fi
	/bin/echo \"0\" > /sys/class/leds/linkit-smart-7688:orange:internet/brightness
	/bin/echo \"0\" > /sys/class/leds/linkit-smart-7688:orange:service/brightness
	/bin/echo \"0\" > /sys/class/leds/linkit-smart-7688:orange:ble1/brightness
	/bin/echo \"0\" > /sys/class/leds/linkit-smart-7688:orange:ble2/brightness
	sleep 1
	/bin/echo \"1\" > /sys/class/leds/linkit-smart-7688:orange:internet/brightness
	/bin/echo \"1\" > /sys/class/leds/linkit-smart-7688:orange:service/brightness
	/bin/echo \"1\" > /sys/class/leds/linkit-smart-7688:orange:ble1/brightness
	/bin/echo \"1\" > /sys/class/leds/linkit-smart-7688:orange:ble2/brightness
	sleep 1
	/bin/echo \"0\" > /sys/class/leds/linkit-smart-7688:orange:internet/brightness
	/bin/echo \"0\" > /sys/class/leds/linkit-smart-7688:orange:service/brightness
	/bin/echo \"0\" > /sys/class/leds/linkit-smart-7688:orange:ble1/brightness
	/bin/echo \"0\" > /sys/class/leds/linkit-smart-7688:orange:ble2/brightness
	sleep 1

	echo \"REBOOT\" > /dev/console
	reboot -f
fi

return 0">> /etc/rc.button/wps
chmod +x /etc/rc.button/wps
/etc/init.d/check_internet enable
touch /etc/checkled
touch /etc/bb.txt
awk '{print toupper($0)}' < /sys/class/net/eth0/address > /etc/bb.txt
read MAC </etc/bb.txt
Mac1=${MAC:12:2}
Mac2=${MAC:15:2}
temp="$Mac1$Mac2"
rm /etc/bb.txt
touch /etc/bb.txt
read MAC < /sys/class/net/eth0/address
echo "$MAC"
echo -n "$MAC" | md5sum | cut -d ' ' -f 1 >> /etc/bb.txt
read ID < /etc/bb.txt
SUBID="-"
ID1=${ID:0:8}
ID2=${ID:8:4}
ID3=${ID:12:4}
ID4=${ID:16:4}
ID5=${ID:20:8}
UUID="$ID1$SUBID$ID2$SUBID$ID3$SUBID$ID4$SUBID$ID5"
echo "$UUID" >> /etc/HomeControllerId.txt
mv /etc/RDhcPy /root
mv /etc/run.sh /root
mv /etc/signalrcore/* /usr/lib/python3.7/site-packages/
mv /etc/rd.Sqlite /root
touch /etc/crontabs/root
echo -e "*/1 * * * * /root/run.sh">> /etc/crontabs/root
echo -e "0 8 * * * LOG">> /etc/crontabs/root
echo -e "0 3 */1 * * sleep 120 && /sbin/reboot -f">> /etc/crontabs/root
/etc/init.d/cron enable
/etc/init.d/cron start
sleep 3
uci set wireless.default_radio0.ssid="RD_HC_$temp"
uci set wireless.default_radio0.key='ABC123456'
uci set wireless.default_radio0.encryption='psk2'
uci set wireless.default_radio0.disabled='0'
uci set system.@system[0].hostname="RD_HC_$temp"
uci set wireless.radio0.disabled='0'
uci set network.wan=interface
uci set network.wan.proto='dhcp'
uci set system.@system[0].zonename='Asia/Ho Chi Minh'
uci set system.@system[0].timezone='<+7>-7'
uci set network.wan.ifname='eth0'
uci set network.lan.macaddr="$MAC"
uci set network.lan.force_link='1'
uci set network.lan.ipaddr='10.10.10.1'
uci del network.lan.ifname
uci add dhcp domain
uci set dhcp.@domain[0].name="RD_HC_$temp"
uci set dhcp.@domain[0].ip='10.10.10.1'
uci commit
/etc/init.d/wireless restart
/etc/init.d/network restart
/etc/init.d/system restart
