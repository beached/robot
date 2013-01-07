#!/bin/bash

#Rarely change
SERVERADDRESS="--http-address 0.0.0.0"
SERVERPORT="--http-port 80"
#WTCONFIG="--config ./wt_config.xml"
WTCONFIG=""
DOCROOT="--docroot ./"

#Change often
#SIZE="960 720"
SIZE="320 240"
#SIZE="352 288"
#SIZE="176 144"
#SIZE="160 120"
FINDFACES="false"
ARDUINOPORT="/dev/ttyUSB0"
ROOMBAPORT="/dev/ttyUSB1"

./roombaweb ${ROOMBAPORT} ${ARDUINOPORT} ${SIZE} ${FINDFACES} ${DOCROOT} ${SERVERADDRESS} ${SERVERPORT} ${WTCONFIG}
