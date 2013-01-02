#Make file for Beagle Bone
CXX=g++-4.7
OPT=-march=armv7-a -mtune=cortex-a8 -mfpu=vfpv3-d16 -ftree-vectorize -ffast-math -mfloat-abi=hard 
#CFLAGS=-I./include -I/usr/include -std=c++0x -Wall -pedantic -g3 $(OPT)
CFLAGS=-g -O2 -I./include -I/usr/include -std=c++0x -Wall -pedantic $(OPT)
DEFS=-lboost_system  
DEFSCV=$(DEFS) -lopencv_core -lopencv_highgui -lopencv_objdetect -lboost_thread -lboost_signals -lopencv_imgproc
DEFSTH=$(DEFSCV) -lpthread 
DEFSWT=$(DEFSTH) -lwt -lwthttp  
all: roombacontroller roombaweb mjpegserver
OBJDIR=./obj/
SRCDIR=./src/
INCDIR=./include/

include ./Makefile.common
