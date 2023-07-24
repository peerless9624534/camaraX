#!/bin/sh

START_CMD="carrier-server"

SINFO_KO_PATH=/lib/modules
SENSOR_DRV_PATH=/lib/modules
ISP_DRV_PATH=/lib/modules

check_return()
{
	if [ $? -ne 0 ] ;then
		echo err: $1
		echo exit
		exit
	fi
}

lsmod | grep "soc-nna" > /dev/null
if [ $? -ne 0 ]; then
	insmod /lib/modules/soc-nna.ko
	check_return "insmod soc-nna"
fi

lsmod | grep "mpsys" > /dev/null
if [ $? -ne 0 ]; then
	insmod /lib/modules/mpsys.ko
	check_return "insmod mpsys"
fi

lsmod | grep "avpu" > /dev/null
if [ $? -ne 0 ]; then
	insmod ${SINFO_KO_PATH/%\//}/avpu.ko
	check_return "insmod avpu"
fi

lsmod | grep "sinfo" > /dev/null
if [ $? -ne 0 ] ;then
	insmod ${SINFO_KO_PATH/%\//}/sinfo.ko
	check_return "insmod sinfo"
fi

echo 1 >/proc/jz/sinfo/info
check_return "start sinfo"

SENSOR_INFO=`cat /proc/jz/sinfo/info`
check_return "get sensor type"
echo ${SENSOR_INFO}

PARAM_PATH=/tmp/start_param
find /tmp/start_param > /dev/zero 2>&1
if [ $? -eq 0 ]; then
	PARAM_PATH=/tmp/start_param
else
	PARAM_PATH=start_param
fi
echo ${PARAM_PATH}

SENSOR=${SENSOR_INFO#*:}
ISP_PARAM="isp_clk=125000000"
SENSOR_PARAM=
CARRIER_SERVER_PARAM="--nrvbs 2"
START=0
while read str
do
	#echo $str
	#ignore blank line
	if [ "$str" = "" ];then
		continue
	fi
	name=${str%:*}
	value=${str#*:}
	if [ ${START} = 0 ];then
		if [ "$value" = "$SENSOR" ];then
			START=1
		fi
	else
		case ${name} in
			"isp_param")
				ISP_PARAM=${value}
				;;
			"sensor_param")
				SENSOR_PARAM=${value}
				;;
			"carrier_server_param")
				CARRIER_SERVER_PARAM=${value}
				;;
			*)
				break;
				;;
		esac
	fi
done<${PARAM_PATH}
echo --------------------
echo ${ISP_PARAM}
echo ${SENSOR_PARAM}
echo ${CARRIER_SERVER_PARAM}

lsmod | grep "tx_isp" > /dev/null
if [ $? -ne 0 ] ;then
	insmod ${ISP_DRV_PATH/%\//}/tx-isp-t40.ko  ${ISP_PARAM}
	check_return "insmod isp drv"
fi

lsmod | grep "audio" > /dev/null
if [ $? -ne 0 ] ;then
        insmod ${ISP_DRV_PATH/%\//}/audio.ko mono_channel=1
        check_return "insmod audio"
fi

lsmod | grep ${SENSOR} > /dev/null
if [ $? -ne 0 ] ;then
	insmod ${SENSOR_DRV_PATH/%\//}/sensor_${SENSOR}_t40.ko ${SENSOR_PARAM}
	check_return "insmod sensor drv"
fi

chmod 777 /system/config/luxconfig.ini
chmod 777 /system/bin/onvif
chmod 777 /system/bin/media_sample
sync
/system/bin/onvif &
#sleep 1
#/system/bin/media_sample &
#sleep 2
#/system/init/restart.sh &

