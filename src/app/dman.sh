#!/bin/sh

#time1=$(date "+%Y-%m-%d %H:%M:%S.%N")
#echo $time1
#rm -rf ${HOME}/.cache/fontconfig
# time2=$(date "+%Y-%m-%d %H:%M:%S.%N")
# echo $time2

if [ ! x"${USER}" = x"root" ];then
    /usr/share/deepin-manual/dman $@
else
    /usr/share/deepin-manual/dman $@ --no-sandbox
fi


