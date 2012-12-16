#!/bin/bash

./roombaweb /dev/ttyUSB0 320 240 --docroot ./ --http-address 0.0.0.0 --http-port 80 --config ./wt_config.xml
#./Roomba/roombaweb /dev/ttyUSB0 176 144 --docroot ./ --http-address 0.0.0.0 --http-port 80 
#./roombaweb /dev/ttyUSB0 352 288 --docroot ./ --http-address 0.0.0.0 --http-port 80 
