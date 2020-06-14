ROOTCFLAGS    = $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOTLIBS      = $(shell $(ROOTSYS)/bin/root-config --libs)
ROOTGLIBS     = $(shell $(ROOTSYS)/bin/root-config --glibs)

#ROOTCFLAGS    = $(shell /usr/bin/root-config --cflags)
#ROOTLIBS      = $(shell /usr/bin/root-config --libs)
#ROOTGLIBS     = $(shell /usr/bin/root-config --glibs)

CXX           = g++
CXXFLAGS      = -g -Wall -fPIC -Wno-deprecated

NGLIBS         = $(ROOTGLIBS) 
NGLIBS        += -lMinuit

CXXFLAGS      += $(ROOTCFLAGS)
CXX           += -I./	
LIBS           = $(ROOTLIBS) 

GLIBS          = $(filter-out -lNew, $(NGLIBS))

CXX	      += -I./obj/
OUTLIB	      = ./obj/
.SUFFIXES: .C
.PREFIXES: ./obj/

#----------------------------------------------------#

all: convertUSBWC2root8Channels convertUSBWC2root8ChannelsBin convertUSBWCMeas2root8Channels convertUSBWCMeas2root8ChannelsBin convertUSBWCRates2root8Channels convertUSBWCRates2root8ChannelsBin runL1 runL1Bin runL2 runMeasures

runL1: runL1.cpp obj/wfSim.o obj/waveformSimple.o obj/anaL1.o
	g++ -o runL1 runL1.cpp $(OUTLIB)*.o `root-config --cflags --glibs`

runL1Bin: runL1Bin.cpp obj/wfSim.o obj/waveformSimple.o obj/anaL1Bin.o
	g++ -o runL1Bin runL1Bin.cpp $(OUTLIB)*.o `root-config --cflags --glibs`

runL2: runL2.cpp obj/wfSim.o obj/anaL2.o
	g++ -o runL2 runL2.cpp $(OUTLIB)*.o `root-config --cflags --glibs`

runMeasures: runMeasures.cpp obj/wfSim.o obj/anaMeasures.o
	g++ -o runMeasures runMeasures.cpp $(OUTLIB)*.o `root-config --cflags --glibs`

obj/anaL1.o: src/anaL1.cpp src/anaL1.hh
	$(CXX) $(CXXFLAGS) -c -I. -o $(OUTLIB)anaL1.o $<

obj/anaL1Bin.o: src/anaL1Bin.cpp src/anaL1Bin.hh
	$(CXX) $(CXXFLAGS) -c -I. -o $(OUTLIB)anaL1Bin.o $<

obj/anaL2.o: src/anaL2.cpp src/anaL2.hh
	$(CXX) $(CXXFLAGS) -c -I. -o $(OUTLIB)anaL2.o $<

obj/anaMeasures.o: src/anaMeasures.cpp src/anaMeasures.hh
	$(CXX) $(CXXFLAGS) -c -I. -o $(OUTLIB)anaMeasures.o $<

obj/wfSim.o: wfSimulation/src/wfSim.cpp wfSimulation/src/wfSim.hh
	$(CXX) $(CXXFLAGS) -c -I. -o $(OUTLIB)wfSim.o $<

obj/waveformSimple.o: wfSimulation/src/waveformSimple.cpp wfSimulation/src/waveformSimple.hh
	$(CXX) $(CXXFLAGS) -c -I. -o $(OUTLIB)waveformSimple.o $<

obj/waveform.o: wfSimulation/src/waveform.cpp wfSimulation/src/waveform.hh
	$(CXX) $(CXXFLAGS) -c -I. -o $(OUTLIB)waveform.o $<

convertUSBWC2root8Channels: convertUSBWC2root8Channels.cc
	g++ -o convertUSBWC2root8Channels convertUSBWC2root8Channels.cc `root-config --cflags --glibs`

convertUSBWC2root8ChannelsBin: convertUSBWC2root8ChannelsBin.cc
	g++ -o convertUSBWC2root8ChannelsBin convertUSBWC2root8ChannelsBin.cc `root-config --cflags --glibs`

convertUSBWCMeas2root8Channels: convertUSBWCMeas2root8Channels.cc
	g++ -o convertUSBWCMeas2root8Channels convertUSBWCMeas2root8Channels.cc `root-config --cflags --glibs`

convertUSBWCMeas2root8ChannelsBin: convertUSBWCMeas2root8ChannelsBin.cc
	g++ -o convertUSBWCMeas2root8ChannelsBin convertUSBWCMeas2root8ChannelsBin.cc `root-config --cflags --glibs`

convertUSBWCRates2root8Channels: convertUSBWCRates2root8Channels.cc
	g++ -o convertUSBWCRates2root8Channels convertUSBWCRates2root8Channels.cc `root-config --cflags --glibs`

convertUSBWCRates2root8ChannelsBin: convertUSBWCRates2root8ChannelsBin.cc
	g++ -o convertUSBWCRates2root8ChannelsBin convertUSBWCRates2root8ChannelsBin.cc `root-config --cflags --glibs`

clean:
	rm -f runL1
	rm -f runL1Bin
	rm -f runL2
	rm -f runMeasures
	rm -f convertUSBWC2root8Channels
	rm -f convertUSBWC2root8ChannelsBin
	rm -f convertUSBWCMeas2root8Channels
	rm -f convertUSBWCMeas2root8ChannelsBin
	rm -f convertUSBWCRates2root8Channels
	rm -f convertUSBWCRates2root8ChannelsBin
	rm -f *~
	rm -f src/*~
	rm -f $(OUTLIB)*.o
