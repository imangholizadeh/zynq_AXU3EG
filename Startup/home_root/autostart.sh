#!/bin/sh

echo  " iman gholizadeh !!!\n"
#/usr/bin/weston --log=${XDG_RUNTIME_DIR}/weston.log $OPTARGS &
sleep 0.5
/media/sd-mmcblk1p1/Standby --platform wayland-egl &
ip addr add 172.30.202.211/24 dev eth0 2>> net.log &
ip addr add 192.168.16.211/24 dev eth0 2>> net.log &
ip addr add 192.168.4.211/24 dev eth0 2>> net.log &
#ip addr add 192.168.0.11/24 dev eth0 2>> net.log &
#/home/root/MFD --platform wayland-egl &
#weston-terminal
