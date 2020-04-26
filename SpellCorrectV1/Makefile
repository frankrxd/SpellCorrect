INC_DIR:= ./ ./Threadpool ./templateMylogger 
SRC_DIR:= ./
SRC_DIR2:= ./Threadpool

SRCS:=$(wildcard ./*.cc ./Threadpool/*.cc ./templateMylogger/*.cc)
OBJS:= $(patsubst %.cc, %.o, $(SRCS))
LIBS:= -llog4cpp -lpthread -lhiredis

CXX:=g++

CXXFLAGS:= -w -g -std=c++11 $(addprefix -I, $(INC_DIR)) $(LIBS) 

EXE:=./server.exe

$(EXE):$(OBJS)
	$(CXX) -o $(EXE) $(OBJS) $(CXXFLAGS)

clean:
	rm -rf $(EXE)
	rm -rf $(OBJS)
	rm -rf *.log
