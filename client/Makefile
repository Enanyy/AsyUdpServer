ROOT_DIR = $(shell pwd)
PROTO_LIB_DIR = /usr/lib/
PROTO_CPP_DIR = $(ROOT_DIR)/protocpp/


CXX 		:= g++
CC 		:= gcc

CFLAGS  	:= -g -W -c -Wall -lpthread -std=c++11  
LFLAGS 		:= -lpthread -std=c++11
PROTOFLAGS 	:= -I $(PROTO_CPP_DIR) -L $(PROTO_LIB_DIR) -lprotoc -lprotobuf

SUB_DIR = $(shell ls -l | grep ^d | awk '{if($$9 != "bin") print $$9}')

OBJ_DIR = $(ROOT_DIR)/bin/debug/obj
TARGET  = $(ROOT_DIR)/bin/debug/client

$(shell mkdir -p $(OBJ_DIR))

export CXX CC CFLAGS PROTOFLAGS OBJ_DIR

CPP_LIST = $(wildcard *.cpp)
CPP_OBJ_LIST = $(patsubst %.cpp, %.o, $(CPP_LIST))

CC_LIST = $(wildcard *.c)
CC_OBJ_LIST = $(patsubst %.c, %.o, $(CC_LIST))

all:$(SUB_DIR)\
	$(CPP_OBJ_LIST)\
	$(CC_OBJ_LIST)\
	$(TARGET)

$(SUB_DIR):ECHO
	make -C $@

$(TARGET):$(wildcard $(OBJ_DIR)/*.o)
	$(CXX) $^ $(LFLAGS) $(PROTOFLAGS) -o $@

ECHO:
	@echo $(SUB_DIR)

$(CPP_OBJ_LIST):%.o:%.cpp
	$(CXX) $(CFLAGS) $^ -o $(OBJ_DIR)/$@

$(CC_OBJ_LIST):%.o:%.c
	$(CC) $(CFLAGS) $^ -o $(OBJ_DIR)/$@

run:
	$(TARGET)

gdb:
	@gdb $(TARGET)

clean:
	@rm -f $(wildcard $(OBJ_DIR)/*.o)
	@rm -f $(TARGET)

