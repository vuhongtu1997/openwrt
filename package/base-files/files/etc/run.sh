#!/bin/sh

Count=0
ModeTest=0

Q=$(ps | grep 'SYSTEM' | grep -v "grep" | wc -l)
if [ $Q == 1 ]
then
        echo "SYSTEM is running"
	Count=$((Count+1))
elif [ $Q == 0 ]
then
	if [ -e /root/rd.Sqlite ]
	then
		echo "SYSTEM is not running"
		SYSTEM &
	else
		CREATE_DB 
		SYSTEM &
	fi
fi

Q=$(ps | grep 'RD_SMART' | grep -v "grep" | wc -l)
if [ $Q == 1 ]
then
    echo "RD_SMART is running"
	Count=$((Count+1))
elif [ $Q == 0 ]
then
	if [ -e /root/rd.Sqlite ]
	then
	        echo "RD_SMART is not running"
	        RD_SMART &
	else
	        CREATE_DB
	        RD_SMART &
	fi
fi

Q=$(ps | grep 'python3 RDhcPy/main.py' | grep -v "grep" | wc -l)
if [ $Q == 1 ]
then
        echo "python3 RDhcPy/main.py is running"
elif [ $Q == 0 ]
then
	if [ -e /usr/lib/python3.7/site-packages/signalrcore ]
	then
		echo "python3 RDhcPy/main.py is not running"
        	python3 RDhcPy/main.py &
        else
        	echo "check again lib signalrcore"
        fi
fi

if P=$(pgrep UDP_1907)
then
        echo "UDP_1907 is running"
	Count=$((Count+1))
else
	if [ -e /etc/check_download ]
	then
		download_package &
	else
		echo "UDP_1907 is not running"
		UDP_1907 &
	fi
fi

if [ $ModeTest == 0 ]
then
	if [ $Count == 3 ]
	then
		/bin/echo "1" > /sys/class/leds/linkit-smart-7688:orange:ble1/brightness
	else
		/bin/echo "0" > /sys/class/leds/linkit-smart-7688:orange:ble1/brightness
	fi
fi


if [ -e /etc/checkled ] && [ ! -e /etc/run_config.sh ]
then
	ModeTest=1
	/bin/echo "1" > /sys/class/leds/linkit-smart-7688:orange:internet/brightness
	/bin/echo "1" > /sys/class/leds/linkit-smart-7688:orange:service/brightness
	if [ -e /sys/bus/i2c/devices/i2c-0/0-0068/rtc/rtc0/date ]
	then
		/bin/echo "1" > /sys/class/leds/linkit-smart-7688:orange:ble2/brightness
	else
		/bin/echo "0" > /sys/class/leds/linkit-smart-7688:orange:ble2/brightness
	fi
	/bin/echo "1" > /sys/class/leds/linkit-smart-7688:orange:ble1/brightness
fi
