#Make file for Beagle Bone
CXX=g++-4.7
OPT=-march=armv7-a -mtune=cortex-a8 -mfpu=vfpv3-d16 -ftree-vectorize -ffast-math -mfloat-abi=hard 
include ./Makefile.common
