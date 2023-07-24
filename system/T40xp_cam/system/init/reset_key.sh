grepStr="led_blink.sh"
sleepTime=1
PressCount=0
PressThreshold=8

IFS=$'\n'

FACTORY_RESET=/tmp/factory_reset.sh
LED_BLINK=/tmp/led_blink.sh
rm -rf ${FACTORY_RESET}
rm -rf ${LED_BLINK}
touch  ${FACTORY_RESET}
touch  ${LED_BLINK}

#create factory_reset.sh
echo "#!/bin/sh" >> ${FACTORY_RESET}
echo 'grepStr="led_blink.sh"' >> ${FACTORY_RESET}
echo "rm -rf /system/*" >> ${FACTORY_RESET}
echo "sleep 1s" >> ${FACTORY_RESET}
echo 'programs=$(ps -l | grep "app_init.sh" | grep -v "grep" | awk '"'"'{print $3}'"'"')' >> ${FACTORY_RESET}
echo 'kill -9 "$programs"' >> ${FACTORY_RESET}
echo 'programs=$(ps -l | grep "udhcpc" | grep -v "grep" | awk '"'"'{print $3}'"'"')' >> ${FACTORY_RESET}
echo 'kill -9 "$programs"' >> ${FACTORY_RESET}
echo 'sleep 3s'  >> ${FACTORY_RESET}
echo 'echo "kill restart.sh ..."'  >> ${FACTORY_RESET}
echo 'programs=$(ps -l | grep "restart.sh" | grep -v "grep" | awk '"'"'{print $3}'"'"')' >> ${FACTORY_RESET}
echo 'kill -9 "$programs"' >> ${FACTORY_RESET}
echo 'echo "kill media_sample ..."' >> ${FACTORY_RESET}
echo 'programs=$(ps -l | grep "media_sample" | grep -v "grep" | awk '"'"'{print $3}'"'"')' >> ${FACTORY_RESET}
echo 'kill -9 "$programs"' >> ${FACTORY_RESET}
echo 'sleep 2s'  >> ${FACTORY_RESET}
echo 'echo "kill onvif ..."' >> ${FACTORY_RESET}
echo 'programs=$(ps -l | grep "onvif" | grep -v "grep" | awk '"'"'{print $3}'"'"')' >> ${FACTORY_RESET}
echo 'kill -9 "$programs"' >> ${FACTORY_RESET}
echo "cp -rf /restore_system/* /system/" >> ${FACTORY_RESET}
echo 'echo "kill led_blink.sh ..."' >> ${FACTORY_RESET}
echo 'programs=$(ps -l | grep "led_blink.sh" | grep -v "grep" | awk '"'"'{print $3}'"'"')' >> ${FACTORY_RESET}
echo 'kill -9 "$programs"' >> ${FACTORY_RESET}
echo 'echo "Kill led_blink.sh succeed"' >> ${FACTORY_RESET}
echo 'echo "System will restarted"' >> ${FACTORY_RESET}
echo 'sync'  >> ${FACTORY_RESET}
echo 'sleep 1s' >> ${FACTORY_RESET}
echo 'sync' >> ${FACTORY_RESET}
echo 'sleep 1s' >> ${FACTORY_RESET}
echo 'sync' >> ${FACTORY_RESET}
echo 'sleep 3s' >> ${FACTORY_RESET}
echo "reboot -f" >> ${FACTORY_RESET}

chmod 777 /tmp/factory_reset.sh

#create led_blink.sh
echo "sleepTime=0.2" >> ${LED_BLINK}
echo "while :; do" >> ${LED_BLINK}
echo "echo 1 > /sys/class/gpio/gpio59/value" >> ${LED_BLINK}
echo "echo 0 > /sys/class/gpio/gpio60/value" >> ${LED_BLINK}
echo "sleep 0.25s" >> ${LED_BLINK}
echo "echo 0 > /sys/class/gpio/gpio59/value" >> ${LED_BLINK}
echo "echo 1 > /sys/class/gpio/gpio60/value" >> ${LED_BLINK}
echo "sleep 0.25s" >> ${LED_BLINK}
echo 'echo "reset factory setup ... ..."' >> ${LED_BLINK}
echo 'done' >> ${LED_BLINK}

chmod 777 /tmp/led_blink.sh


while :; do
 for line in $(cat /sys/class/gpio/gpio79/value)
  do
   if [ $line -eq 0 ]; then
     PressCount=0
   else
	 echo "ResetKey is ON, PressCount is: $PressCount"
	 PressCount=$(($PressCount+1))
	 if [ "$PressCount" -ge $PressThreshold ]; then
        echo "$PressCount >= $PressThreshold"
		PressCount=0
		/tmp/led_blink.sh &
		sleep 1s
		/tmp/factory_reset.sh &
		exit 0
	 fi
   fi
  done
  sleep "$sleepTime"s
done