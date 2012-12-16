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

$(OBJDIR)roombasensors.o: $(SRCDIR)roombasensors.cpp $(INCDIR)roombasensors.h
	$(CXX) $(CFLAGS) -c -o $@ $<

$(OBJDIR)camera.o: $(SRCDIR)camera.cpp $(INCDIR)camera.h $(INCDIR)opencvmat.h $(INCDIR)nullptr.h
	$(CXX) $(CFLAGS) -c -o $@ $<

$(OBJDIR)serialport.o: $(SRCDIR)serialport.cpp $(INCDIR)serialport.h 
	$(CXX) $(CFLAGS) -c -o $@ $<

$(OBJDIR)roombacontrol.o: $(SRCDIR)roombacontrol.cpp $(INCDIR)roombacontrol.h 
	$(CXX) $(CFLAGS) -c -o $@ $<

$(OBJDIR)opencvimage.o: $(SRCDIR)opencvimage.cpp $(INCDIR)opencvimage.h 
	$(CXX) $(CFLAGS) -c -o $@ $<

$(OBJDIR)util.o: $(SRCDIR)util.cpp $(INCDIR)util.h $(INCDIR)nullptr.h
	$(CXX) $(CFLAGS) -c -o $@ $<

$(OBJDIR)wtutil.o: $(SRCDIR)wtutil.cpp $(INCDIR)wtutil.h
	$(CXX) $(CFLAGS) -c -o $@ $<

$(OBJDIR)roombawebimagepush.o: $(SRCDIR)roombawebimagepush.cpp $(INCDIR)roombawebimagepush.h
	$(CXX) $(CFLAGS) -c -o $@ $<

$(OBJDIR)roombawebserver.o: $(SRCDIR)roombawebserver.cpp $(INCDIR)roombawebserver.h
	$(CXX) $(CFLAGS) -c -o $@ $<

$(OBJDIR)roombawebapplication.o: $(SRCDIR)roombawebapplication.cpp $(INCDIR)roombawebapplication.h
	$(CXX) $(CFLAGS) -c -o $@ $<

$(OBJDIR)mjpegserver.o: $(SRCDIR)mjpegserver.cpp $(INCDIR)mjpegserver.h $(INCDIR)opencvmat.h $(INCDIR)nullptr.h
	$(CXX) $(CFLAGS) -c -o $@ $< 

$(OBJDIR)main.o: $(SRCDIR)main.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

$(OBJDIR)webmain.o: $(SRCDIR)webmain.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

$(OBJDIR)cameratest.o: $(SRCDIR)cameratest.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

$(OBJDIR)mjpegservermain.o: $(SRCDIR)mjpegservermain.cpp $(INCDIR)opencvmat.h
	$(CXX) $(CFLAGS) -c -o $@ $<

cameratest: $(OBJDIR)opencvimage.o $(OBJDIR)util.o $(OBJDIR)camera.o $(OBJDIR)cameratest.o
	$(CXX) $(OBJDIR)opencvimage.o $(OBJDIR)util.o $(OBJDIR)camera.o $(OBJDIR)cameratest.o $(DEFSCV) -o cameratest

roombacontroller: $(OBJDIR)roombasensors.o $(OBJDIR)serialport.o $(OBJDIR)roombacontrol.o $(OBJDIR)util.o $(OBJDIR)main.o
	$(CXX) $(OBJDIR)roombasensors.o $(OBJDIR)serialport.o $(OBJDIR)roombacontrol.o $(OBJDIR)util.o $(OBJDIR)main.o $(DEFSTH) -o roombacontroller

roombaweb: $(OBJDIR)roombasensors.o $(OBJDIR)camera.o $(OBJDIR)mjpegserver.o $(OBJDIR)serialport.o $(OBJDIR)roombacontrol.o $(OBJDIR)opencvimage.o $(OBJDIR)util.o $(OBJDIR)wtutil.o $(OBJDIR)roombawebserver.o $(OBJDIR)roombawebimagepush.o $(OBJDIR)roombawebapplication.o $(OBJDIR)webmain.o
	 $(CXX) $(OBJDIR)roombasensors.o $(OBJDIR)camera.o $(OBJDIR)mjpegserver.o $(OBJDIR)serialport.o $(OBJDIR)roombacontrol.o $(OBJDIR)opencvimage.o $(OBJDIR)util.o $(OBJDIR)wtutil.o $(OBJDIR)roombawebserver.o $(OBJDIR)roombawebimagepush.o $(OBJDIR)roombawebapplication.o $(OBJDIR)webmain.o $(DEFSWT) -o roombaweb

mjpegserver: $(OBJDIR)camera.o $(OBJDIR)mjpegserver.o $(OBJDIR)opencvimage.o $(OBJDIR)mjpegservermain.o 
	$(CXX) $(OBJDIR)camera.o $(OBJDIR)mjpegserver.o $(OBJDIR)opencvimage.o $(OBJDIR)mjpegservermain.o $(DEFSTH) -o $@
