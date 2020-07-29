#!/bin/sh

#time1=$(date "+%Y-%m-%d %H:%M:%S.%N")
#echo $time1
#rm -rf ${HOME}/.cache/fontconfig
time2=$(date "+%Y-%m-%d %H:%M:%S.%N")
echo $time2

/usr/share/deepin-manual/dman $@
