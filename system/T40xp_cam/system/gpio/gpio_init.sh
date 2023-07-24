#!/bin/sh

#speak
#echo 7 > /sys/class/gpio/export
#echo out > /sys/class/gpio/gpio7/direction
#echo 0 > /sys/class/gpio/gpio7/active_low
#set gpio value
#echo 0 > /sys/class/gpio/gpio7/value

#LED
#gpio59 green, gpio60 red
echo 59 > /sys/class/gpio/export
echo 60 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio59/direction
echo out > /sys/class/gpio/gpio60/direction
echo 0 > /sys/class/gpio/gpio59/active_low
echo 0 > /sys/class/gpio/gpio60/active_low
#set gpio value
echo 1 > /sys/class/gpio/gpio59/value
echo 0 > /sys/class/gpio/gpio60/value

#ir light
#echo 52 > /sys/class/gpio/export
#echo in > /sys/class/gpio/gpio52/direction
#echo 0 > /sys/class/gpio/gpio52/active_low
#set gpio value
#echo 0 > /sys/class/gpio/gpio52/value

#IRCut LED_Power                                  
echo 116 > /sys/class/gpio/export           
echo out > /sys/class/gpio/gpio116/direction
echo 0 > /sys/class/gpio/gpio116/active_low 
#set gpio value                            
echo 0 > /sys/class/gpio/gpio116/value   

#ircut
echo 122 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio122/direction
echo 0 > /sys/class/gpio/gpio122/active_low

echo 123 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio123/direction
echo 0 > /sys/class/gpio/gpio123/active_low

#reset button
echo 79 > /sys/class/gpio/export
echo in > /sys/class/gpio/gpio79/direction
echo 1 > /sys/class/gpio/gpio79/active_low
#get gpio value
cat /sys/class/gpio/gpio79/value

#SD POWER
#echo 118 > /sys/class/gpio/export
#echo out > /sys/class/gpio/gpio118/direction
#echo 0 > /sys/class/gpio/gpio118/active_low
#set gpio value
#echo 0 > /sys/class/gpio/gpio118/value

                              
  


                          
 

#IRCUT
#/system/gpio/ircut/gpio_init



#SD0_CD_N
#echo 70 > /sys/class/gpio/export
#echo out > /sys/class/gpio/gpio70/direction
#echo 0 > /sys/class/gpio/gpio70/active_low
#set gpio value
#echo 1 > /sys/class/gpio/gpio70/value


#WIFI Power
#echo 8 > /sys/class/gpio/export
#echo out > /sys/class/gpio/gpio8/direction
#echo 0 > /sys/class/gpio/gpio8/active_low
#set gpio value, 0 effective
#echo 0 > /sys/class/gpio/gpio8/value
