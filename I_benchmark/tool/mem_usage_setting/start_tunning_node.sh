#!/bin/bash

export LANG=C
export PATH="/usr/bin:/bin:/usr/sbin:/sbin:$PATH"

#PROGRAM_DIR="/home/wulm/mem_usage_setting"
PROGRAM_DIR=`pwd`

version="1.01"
PERCENTAGE=$1
# tyep1 1KB/time  type2 1MB/time  type default 1GB/time
TYPE=$2
USLEEP=$3

MEM_TOTAL=`cat /proc/meminfo | grep 'MemTotal' | awk '{print $2}'` #kB
MEM_FREE=`cat /proc/meminfo | grep 'MemFree' | awk '{print $2}'` #kB
MEM_USED=`echo "$MEM_TOTAL - $MEM_FREE" | bc` #kB

EXPECTED_MEM_USE=`echo "$MEM_TOTAL * $1 / 100" | bc` #kB


cd $PROGRAM_DIR
echo "memory allocation version: $version"
echo "TUNNING STARTED: MEM_USAGE=$1% TYPE=$2 USLEEP=$3"
./mem_take $EXPECTED_MEM_USE $TYPE $USLEEP
