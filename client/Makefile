PROTO_LIB_PATH = /usr/lib/
PROTO_CPP_PATH = ../src/protocpp/

CXX 	:= g++
LD 		:= g++
CC 		:= gcc

CFLAGS  := -g -W -c -Wall -lpthread -std=c++11  
LDFLAGS := -lpthread -std=c++11
PROTOFLAGS := -I $(PROTO_CPP_PATH) -L $(PROTO_LIB_PATH) -lprotoc -lprotobuf


VPATH = $(PROTO_CPP_PATH)\


INCLUDES = $(PROTO_CPP_PATH)Makefile.mk\
		   

include $(INCLUDES)	

OBJECTS = main.o\
		  ../net/MessageBuffer.o\

TARGET = client

all:$(TARGET)

$(TARGET):$(OBJECTS) $(PROTOCPPOBJECTS)
	$(CXX) $^ -o $@ $(LDFLAGS) $(PROTOFLAGS)
	rm -f $(OBJECTS) $(PROTOCPPOBJECTS)

$(OBJECTS):%.o:%.cpp
	$(CXX) $(CFLAGS)  -o $@ $< 

$(PROTOCPPOBJECTS):%.o:%.cc
	$(CXX) $(CFLAGS) $(PROTOFLAGS) -o $@ $<


run:
	./$(TARGET) 127.0.0.1 1255 1337

.PHONY:clean blackup startredis
clean:
	rm -f $(OBJECTS)  $(TARGET) 

