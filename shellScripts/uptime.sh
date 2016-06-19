#!/bin/bash
#文件名：uptime.sh
#用途：系统运行时间监视器

IP_LIST="192.168.0.1 192.168.0.2 192.168.0.3"
USER="test"

for IP in $IP_LIST;
do
    uptime=$(ssh $USER@$IP uptime | awk '{ print $3 }')
    echo $IP uptime: $uptime
done
