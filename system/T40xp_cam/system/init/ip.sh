#            author: humy  2023.3.10
#
#*********************************************************************************
# 1. ip.sh 默认从　/system/network/interfaces 中读取IP配置
# 2. ip.sh set dhcp  将当前IP方式设置为DHCP方式
# 3. ip.sh set ip mask gateway DNS, 表示设置当前的IP　掩码　网关　DNS，并保存到　interfaces文件中
# 4. 设置IP和保存配置同步进行
#
#
#*********************************************************************************

#!/bin/sh

configFile=/system/network/interfaces
dhcp=0
ipaddr=10.10.100.157
netmask=255.255.255.0
gateway=10.10.100.250
dns=10.10.100.250

checkIPValid(){
	a1=0
	a2=0
	a3=0
	a4=0	
	validIP=1
	
	if [ -z $1 ]; then
		echo "ipstr is null"
		return -1
	fi

	a1=`echo $1 | awk -F. '{print $1}'`
	a2=`echo $1 | awk -F. '{print $2}'`
	a3=`echo $1 | awk -F. '{print $3}'`
	a4=`echo $1 | awk -F. '{print $4}'`

	#echo "a1=$a1 a2=$a2 a3=$a3 a4=$a4"
	
	validIP=0
	echo "$a1"|[ -n "`sed -n '/^[0-9][0-9]*$/p'`" ] && validIP=1
	if [ $validIP -eq 0 ] || [ "$a1" -lt 0 ] || [ "$a1" -gt 255 ]; then
		echo "ipaddr[$1] sub field 0[$a1] is invalid!"
		return -1
	fi

	validIP=0
    echo "$a2"|[ -n "`sed -n '/^[0-9][0-9]*$/p'`" ] && validIP=1
    if [ $validIP -eq 0 ]; then
		echo "ipaddr[$1] sub field 1 is invalid!"
		return -1
    fi

    validIP=0        
    echo "$a3"|[ -n "`sed -n '/^[0-9][0-9]*$/p'`" ] && validIP=1 
    if [ $validIP -eq 0 ]; then
		echo "ipaddr[$1] sub field 2 is invalid!"
		return -1              
    fi

    validIP=0        
    echo "$a4"|[ -n "`sed -n '/^[0-9][0-9]*$/p'`" ] && validIP=1 
    if [ $validIP -eq 0 ]; then
		echo "ipaddr[$1] sub field 3 is invalid!"
		return -1            
    fi

	return 0
}

echo "$configFile"
dhcpstr=`cat $configFile|grep "iface"|grep "eth0"|grep "inet"|grep -v "#"|grep "dhcp"`

echo "dhcp str is [${dhcpstr}]"

if [ -z "$dhcpstr" ]; then
	dhcp=0
else
	dhcp=1
fi

echo "dhcp value:$dhcp"

#get ipaddr
ipaddr=`cat $configFile|grep "address "|grep -v "#"|awk '{print $2}'`
if [ -z "$ipaddr" ]; then
	echo "ip:[$ipaddr] is invalid!"
	exit 1
fi
ipaddr=`eval echo $ipaddr`

#get netmask
netmask=`cat $configFile|grep "netmask "|grep -v "#"|awk '{print $2}'`
if [ -z "$netmask" ]; then
	echo "netmask:[$netmask] is invalid!"
	exit 1
fi
netmask=`eval echo $netmask`

#get gateway
gateway=`cat $configFile|grep "gateway "|grep -v "#"|awk '{print $2}'` 
if [ -z "$gateway" ]; then
        echo "gateway:[$gateway] is invalid!"
        exit 1
fi
gateway=`eval echo $gateway`

#get dns
dns=`cat $configFile|grep "dns-nameservers "|grep -v "#"|awk '{print $2}'`
if [ -z "$dns" ]; then
        echo "dns:[$dns] is invalid!"
        exit 1
fi
dns=`eval echo $dns`

if [ "$1" = "set" ]; then
	echo "set ipaddr ... "
	
	if [ ! -z "$2" ]; then
		ipaddr=$2
		if [ "$ipaddr" = "dhcp" ]; then
			dhcp=1
			ipaddr=10.10.100.157
		else
			dhcp=0
			ipaddr=`eval echo $ipaddr`
		fi
	fi

	if [ ! -z "$3" ]; then
		netmask=$3
		netmask=`eval echo $netmask`
	fi

	if [ ! -z "$4" ]; then
		gateway=$4
		gateway=`eval echo $gateway`
	fi

	if [ ! -z "$5" ]; then
		dns=$4
		dns=`eval echo $dns`
	fi
fi

echo "ipaddr:$ipaddr"
echo "netmask:$netmask"
echo "gateway:$gateway"
echo "dns:$dns"

echo "now begin config ipaddr..."

if [ $dhcp == 1 ]; then
	echo "dpcp mode to get ip"
	udhcpc -i eth0 -R

	dhcpgateway=`route -e | grep  "default" | awk '{print $2}'`

	ifconfig eth0 down
	sleep 1
	ifconfig eth0 up

	route delete default                                                          
	route add default gw $dhcpgateway
else
	echo "static ip addr"

	checkIPValid $ipaddr
	if [ $? -ne 0 ]; then
		echo "ipaddr[$ipaddr] is invalid!"
		exit 1
	fi

	checkIPValid $netmask
	if [ $? -ne 0 ]; then
		echo "sub netmask[$netmask] is invalid!"
		exit 1
	fi

	checkIPValid $gateway
	if [ $? -ne 0 ]; then
		echo "gateway[$gateway] is invalid!"
		exit 1
	fi

	checkIPValid $dns
	if [ $? -ne 0 ]; then
		echo "gateway[$dns] is invalid!"
		exit 1
	fi

	#write network/interfaces
	rm -rf $configFile
	touch $configFile
	echo "auto lo" >> $configFile
	echo "iface lo inet loopback" >> $configFile
	echo " " >>$configFile
	echo "auto eth0" >> $configFile

	if [ $dhcp == 1 ]; then
		echo "#iface eth0 inet static" >> $configFile	
		echo "iface eth0 inet dhcp" >> $configFile	
	else
		echo "iface eth0 inet static" >> $configFile	
		echo "#iface eth0 inet dhcp" >> $configFile
	fi

	echo "address $ipaddr" >> $configFile
	echo "netmask $netmask" >> $configFile
	echo "gateway $gateway" >> $configFile
	echo "dns-nameservers $dns" >> $configFile

	sync
	sync

	#config ip mask gw dns
	ifconfig eth0 $ipaddr netmask $netmask
	route delete default
	route add default gw $gateway
	echo nameserver $dns > /tmp/resolv.conf

	ifconfig eth0 down
	sleep 1
	ifconfig eth0 up
	
	route delete default                                                          
	route add default gw $gateway	

	sync
	sync

	echo "config ipaddr is finished!"

	ifconfig -a	
		
fi
