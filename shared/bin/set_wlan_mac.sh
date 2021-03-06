#! /system/bin/sh
# Set wlan0 mac address.
#
# Get the unique board serial number from /proc/cmdline,
# hash it and use the first 6 bytes as the mac address.
# If the serial number is not available, use some random number
# instead. After first boot the MAC address is written to userdata
# so that subsequent boots don't change the mac address.

DEV="wlan0"

SERIALNO=`cat /proc/cmdline | grep -o "serialno.*" | cut -d" " -f1`

# If for some reason the serial number isn't on cmdline, just use random
if [ -z "$SERIALNO" ]; then
    SERIALNO="$RANDOM$RANDOM$RANDOM$RANDOM$RANDOM"
fi

# Do some funky stuff to generate a MAC address from the serial number
# We sha256 it because some devices use non-base 16 characters in their
# serial number (*ahem* SHIFT6mq).
SERIALNO=`echo $SERIALNO | sha256sum | awk '{printf("02%010s\n", $1)}' | sed 's/\(..\)/\1:/g' | sed '$s/:$//' | cut -d: -f1,2,3,4,5,6`

# If the mac address has already been generated then use that instead
if [ -f /data/wlan_mac.bin ]; then
    WLAN_MAC=`cat /data/wlan_mac.bin`
else
    WLAN_MAC="$SERIALNO"
    echo $WLAN_MAC > /data/wlan_mac.bin
fi

echo "Setting $DEV mac addr to $WLAN_MAC"

while ! ip link show $DEV > /dev/null; do
        echo "Device not up yet, waiting for it"
        sleep 1
done

/system/bin/ip link set dev $DEV down
/system/bin/ip link set dev $DEV address "${WLAN_MAC}"
/system/bin/ip link set dev $DEV up
