#!/bin/sh

MemTotal=$(cat /proc/meminfo | grep 'MemTotal' | awk '{print $2}')
MemFree=$(cat /proc/meminfo | grep 'MemFree' | awk '{print $2}')

Disp_SYS_Rate=`expr "scale=3; a=100*$MemFree/$MemTotal;  if(length(a)==scale(a)) print 0;print a" |bc`
echo $Disp_SYS_Rate%
