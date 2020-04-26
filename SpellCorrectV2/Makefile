INC_DIR:= ./include 
SRC_DIR:= ./src

SRC_DIR2:=./client

SRCS:=$(wildcard ./src/*.cc)
OBJS:= $(patsubst %.cc, %.o, $(SRCS))
LIBS:= -llog4cpp -lpthread -lhiredis

SRCS2:=$(wildcard ./client/*.cc)
OBJS2:= $(patsubst %.cc, %.o, $(SRCS2))

CXX:=g++

CXXFLAGS:= -w -g -std=c++11 $(addprefix -I, $(INC_DIR)) $(LIBS) 

EXE1:=./bin/server.exe
EXE2:=./bin/client.exe


all:$(EXE1) $(EXE2)

$(EXE1):$(OBJS)
	$(CXX) -o $(EXE1) $(OBJS) $(CXXFLAGS)
$(EXE2):$(OBJS2)
	$(CXX) -o $(EXE2) $(OBJS2) $(CXXFLAGS)
clean:
	rm -rf $(EXE1) $(EXE2)
	rm -rf $(OBJS) $(OBJS2)
	rm -rf ./log/*.log
