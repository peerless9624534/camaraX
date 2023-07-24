#!/bin/sh

grepStr="/system/bin/media_sample"
sleepTime=2
failedCount=0
restartThreshold=8
loopcount1=0

name="restart"
DATE=`date '+%Y%m%d-%H%M%S'` 
LogDir=/system/log

loginfo(){
	LogNameDATE=`date '+%Y%m%d'`

	if [ ! -d $LogDir ]; then
		mkdir -p $LogDir
	fi

	echo " " >> $LogDir/log${LogNameDATE}.log
	echo "----------------------------------" >> $LogDir/log${LogNameDATE}.log
	echo "RESTART DATE:" $(date +"%Y-%m-%d %H:%M:%S")" "$1 >> $LogDir/log${LogNameDATE}.log
	echo "----------------------------------" >> $LogDir/log${LogNameDATE}.log
}

while :; do
	programs=$(ps -l | grep "$grepStr" | grep -v "grep")
  
	if [ -n "$programs" ]; then
		failedCount=0		
		#echo "is running now"
	else
		loopcount1=0
		while [[ $loopcount1 -le 6 ]]; do
 			echo 1 > /sys/class/gpio/gpio59/value
			echo 0 > /sys/class/gpio/gpio60/value
			sleep 0.5s
			echo 0 > /sys/class/gpio/gpio59/value
			echo 1 > /sys/class/gpio/gpio60/value
			sleep 0.5s
			loopcount1=$(($loopcount1+1))
		done
		
		/system/bin/media_sample &

		sleep 2s

		echo "restart media_sample is succeed, restart Count is: $failedCount"

		loginfo "restart media_sample is succeed, restart Count is: $failedCount"

		failedCount=$(($failedCount+1))
		
		if [ "$failedCount" -ge $restartThreshold ]; then
			reboot=1
			index=6
			while [[ $index -ne 0 ]]; do
				index=$(($index-1))
				read -s -n 1 -t 1 -p "reboot after ${index} seconds, press any key cancel..." input
				echo				
				if [ ! -z ${input} ]; then
					reboot=0	
					exit 0
				fi
			done
			
			if [ $reboot -eq 1 ]; then
				echo "restart media_sample count is >= 3,  now reboot system!"
				loginfo "restart media_sample count is >= 3,  now reboot system!"
				sync
				failedCount=0
				reboot
			fi
		fi
	fi
	
	sleep "$sleepTime"s
done

