#!/bin/bash

/root/Roomba/roombaweb /dev/ttyUSB0 320 240 --docroot /root/Roomba/ --http-address 0.0.0.0 --http-port 80 --config /root/Roomba/wt_config.xml
#/root/Roomba/roombaweb /dev/ttyUSB0 176 144 --docroot /root/Roomba/ --http-address 0.0.0.0 --http-port 80 
#/root/Roomba/roombaweb /dev/ttyUSB0 352 288 --docroot /root/Roomba/ --http-address 0.0.0.0 --http-port 80 
