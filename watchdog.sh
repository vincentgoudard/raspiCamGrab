#!/bin/sh

COMMAND=./bin/raspiCamGrab
LOGFILE=restart.txt

writelog() {
  now=`date`
  echo "$now $*" >> $LOGFILE
}

writelog "Starting"
while true ; do
  $COMMAND
  writelog "Exited with status $?"
  writelog "Restarting"
done
