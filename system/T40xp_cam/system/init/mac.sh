#!/bin/sh
rebootThreshold=3
manual=0

DATE=`date '+%Y%m%d-%H%M%S'` 
LogDir=/system/log

loginfo(){
	LogNameDATE=`date '+%Y%m%d'`

	if [ ! -d $LogDir ]; then
		mkdir -p $LogDir
	fi

	echo " " >> $LogDir/log${LogNameDATE}.log
	echo "----------------------------------" >> $LogDir/log${LogNameDATE}.log
	echo "SET MAC DATE:" $(date +"%Y-%m-%d %H:%M:%S")" "$1 >> $LogDir/log${LogNameDATE}.log
	echo "----------------------------------" >> $LogDir/log${LogNameDATE}.log
}


check_mac_address()
{
	for i in `seq 1 6`
	do 
		mac_section=`echo $*|awk -F ':' -v t="${i}" '{print $t}'`
		#echo $mac_section	
		if [ ${#mac_section} != 2 ]; then
			echo "mac section:[$mac_section] is error!"
			return 1
		fi

		#macedit="`echo $mac_section|sed 's/[0-9]//g'|sed 's/[a-f]//g'|sed 's/[A-F]//g'`"	
		if [ -n "`echo $mac_section|sed 's/[0-9]//g'|sed 's/[a-f]//g'|sed 's/[A-F]//g'`" ]; then
			echo "mac_section:[$mac_section] is error!"
			return 1
		fi
	done

	return 0
}

mount -t ubifs /dev/ubi1_0 /syskey
sleep 3
macstr=`cat /syskey/device_info.ini|grep "mac_addr"`
umount /syskey

#judge auto or manual
if [  $# -lt 1  ];then
	manual=0
	echo "too few params"
	#macstr=`cat /syskey/device_info.ini|grep "mac_addr"`
	if [ -z "$macstr" ];  then
		echo "hwaddr not found in /dev/mtdbloc3"
		exit 1
	fi

	if [ ${macstr:0:8} != "mac_addr" ]; then
	echo "hwaddr string is invalid!"
	exit 1
	fi	

	if [ ${#macstr} -lt 26 ]; then
		echo "hwaddr is invalid!" 
		exit 1
	fi
    echo "macstr:$macstr"	
	macaddr=`echo $macstr|awk -F '=' '{print $2}'`
	echo "macaddr:$macaddr"
	macaddr=`eval echo $macaddr`
	echo "macaddr:$macaddr"
	echo "mac from mtdbloc3 is" $macaddr

        if [ "$macaddr" = "FF:FF:FF:FF:FF:FF" ]; then
                echo "hwaddr is random"
                exit 1
        fi
else
	manual=1
	
	while  getopts hv:t: OPTION
	do
	case   $OPTION   in
	h) echo "this is option h"
	shift 1
	;;
	v) echo "this is option m.And the var is " $OPTARG
	macstr=$OPTARG
	
	shift 2
	;;
	esac
	done
	echo $1
	
	if [ ${#macstr} -lt 17 ]; then
	echo "hwaddr is invalid!" 
	exit 1
	fi
	
    str2="-"
	result=$(echo $macstr | grep "${str2}")
    if [[ "$result" != "" ]]
    then
		echo "console mac include '-'"
		macstr=${macstr//-/:}
    else
		echo "console mac don't include '-'"
	fi
	macaddr=$macstr
	echo "mac from console is" $macaddr
fi

check_mac_address ${macaddr}
if [ $? != 0 ]; then
	echo -e " [ ERROR ] '${macaddr}' is a invalid mac"
	exit 1
else
	echo -e " [ OK ] '${macaddr}' is a valid mac"
fi

ifconfig eth0 down
sleep 2
ifconfig eth0 hw ether ${macaddr}

get_macaddr=$(ifconfig -a|grep eth0|awk '{print $5}')
echo "mac from eth0 is" $get_macaddr
if [ $macaddr = $get_macaddr ]; then
	echo "set macaddr is successfully"
else
	if [ $manual -eq 1 ]; then
		echo "set mac failed ,try again"
	else
		echo "set mac failed,Auto-assign mac!"
		loginfo "set mac failed,Auto-assign mac!"
	fi
fi
sleep 2

ifconfig eth0 up
sleep 2

