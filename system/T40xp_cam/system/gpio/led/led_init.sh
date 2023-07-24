echo 59 > /sys/class/gpio/export
echo 60 > /sys/class/gpio/export

echo out > /sys/class/gpio/gpio59/direction
echo out > /sys/class/gpio/gpio60/direction

echo 0 > /sys/class/gpio/gpio59/active_low
echo 0 > /sys/class/gpio/gpio60/active_low

echo 0 > /sys/class/gpio/gpio59/value
echo 0 > /sys/class/gpio/gpio60/value
