#!/bin/bash

#
# Sat Oct 15 22:27:02 CEST 2016
# Autor: Leonid Burmistrov
#
# Script description: 
#
# This script perform data synchronization
# between USB-WC laptop and data analisys 
# laptop 
#

SYNCSCREENNAME="filesync"

./check_if_screen_isRunning.bash $SYNCSCREENNAME

checkStatus=$(echo $?)
#echo "checkStatus = $checkStatus"

if [ "$checkStatus" -eq 1 ]
then
    echo "----------------------------------------------------" 
    echo " --> File synchronisation is already running        " 
    #echo " checkStatus = $checkStatus                         " 
    echo "----------------------------------------------------" 
    exit 0
fi

echo " --> Start file synchronization"
screen -S $SYNCSCREENNAME -L -d -m ./makeFilesync.bash
