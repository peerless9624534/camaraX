echo 0 > /sys/class/gpio/gpio123/value
echo 1 > /sys/class/gpio/gpio122/value
sleep "1"
echo 0 > /sys/class/gpio/gpio122/value
