#!/bin/bash

#
# Sun Oct 16 19:56:32 CEST 2016
# Autor: Leonid Burmistrov
#
# Script description: 
#
# This script check if a given task is running 
# on screen with given name 
#
# return 1 if running
# return 0 if NOT running 
#

nobj=$(screen -ls | grep $1 | wc -l)

#echo "nobj = $nobj"

if [ "$nobj" -eq 0 ]
then
    echo " --> screen with name: $1 is NOT running "
    exit 0
else
    echo " --> screen with name: $1 is running "
    exit 1
fi
