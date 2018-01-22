#!/bin/sh
#
# Program:
#       himeno test scripts
# History:
#       yangtao 2017/11/28 1.0v
#

export LANG=c
TAZYU_COUNT=$1
CFLAGS=$2
CPU_BIND=$3
CPU_NUM=`cat /proc/cpuinfo | grep processor | wc -l`
sed -i "s/DLARGE/$CFLAGS/g" Makefile
make clean
make

../tool/log_start.sh getperfinfo
PIDS=""
for((i=0;i<$TAZYU_COUNT;i++))
do
	if [ "$CPU_BIND" = "yes" ];then
		CPU=`expr $i % $CPU_NUM`
		taskset -c $CPU ./bmt >> himeno-${i}.log &
	else
		./bmt >> himeno-${i}.log &
	fi	
		PIDS="$PIDS $!"
done	
wait $PIDS
../tool/log_stop.sh
sleep 10 
sed -i "s/$CFLAGS/DLARGE/g" Makefile
