#!/bin/bash

#Rarely change
SERVERADDRESS="--http-address 0.0.0.0"
SERVERPORT="--http-port 80"
WTCONFIG="--config ./wt_config.xml"
DOCROOT="--docroot ./"

#Change often
#SIZE="960 720"
#SIZE="320 240"
#SIZE="176 144"
SIZE="160 120"
FINDFACES="false"
SERIALPORT="/dev/ttyUSB0"

./roombaweb ${SERIALPORT} ${SIZE} ${FINDFACES} ${DOCROOT} ${SERVERADDRESS} ${SERVERPORT} ${WTCONFIG}
