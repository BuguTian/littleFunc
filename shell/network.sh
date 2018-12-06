#!/bin/sh

cat /proc/net/dev | grep 'em' | awk '{ if($2!=0) print $1"/"$2}'
