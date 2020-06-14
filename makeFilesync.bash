#!/bin/bash

#
# Sat Feb 18 10:58:21 CET 2017
# Autor: Leonid Burmistrov
#
# Script description: 
#
# This script perform data synchronization
# between USB-WC laptop and data analisys laptop.
#

fileSyncLog="fileSynchronization.log"
fileSyncTeeLog="fileSynchronization.tee.log"

#TOP
echo "rsync --log-file=$fileSyncLog -a -v /home/leonid/usbwc-laptop/Program\ Files\ \(x86\)/WaveCatcher_64ch/Run_Data/ ../data/ | tee $fileSyncTeeLog"
rsync --log-file=$fileSyncLog -a -v /home/leonid/usbwc-laptop/Program\ Files\ \(x86\)/WaveCatcher_64ch/Run_Data/ ../data/ | tee $fileSyncTeeLog
