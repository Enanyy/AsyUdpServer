PROTO_LIB_PATH = /usr/lib/
PROTO_CPP_PATH = protocpp/
NETWORK_PATH = network/
COMMON_PATH = common/

CXX 	:= g++
LD 		:= g++
CC 		:= gcc

CFLAGS  := -g -W -c -Wall -lpthread -std=c++11  
LDFLAGS := -lpthread -std=c++11
PROTOFLAGS := -I $(PROTO_CPP_PATH) -L $(PROTO_LIB_PATH) -lprotoc -lprotobuf


VPATH = $(PROTO_CPP_PATH)\
		$(NETWORK_PATH)\
		$(COMMON_PATH)\


INCLUDES = Makefile.mk\
		   $(PROTO_CPP_PATH)Makefile.mk\
		   $(NETWORK_PATH)Makefile.mk\
		   $(COMMON_PATH)Makefile.mk\

include $(INCLUDES)	

ALLOBJECTS = $(OBJECTS)\
			 $(COMMONOBJECTS)\
			 $(NETWORKOBJECTS)\


TARGET = bin/server

all:$(TARGET)

$(TARGET):$(ALLOBJECTS) $(PROTOCPPOBJECTS)
	$(CXX) $^ $(LDFLAGS) $(PROTOFLAGS) -o $@	
	rm -f *.o

$(ALLOBJECTS):%.o:%.cpp
	$(CXX) $(CFLAGS)  -o $@ $< 

$(PROTOCPPOBJECTS):%.o:%.cc
	$(CXX) $(CFLAGS) $(PROTOFLAGS) -o $@ $<


run:
	./$(TARGET) 

.PHONY:clean blackup startredis
clean:
	rm -f *.o $(TARGET)



proto:
	sudo mount -t vboxsf UbuntuShareFile /mnt/bdshare/ && sudo cp -rf /mnt/bdshare/protos ../mm/
protocpp:
	cd protocpp && protoc ../protos/scene.txt ../protos/action.txt  --cpp_out=.
startredis:
	redis-server ../3rd/redis-3.2.1/redis.conf

backup:
	sudo mount -t vboxsf UbuntuShareFile /mnt/bdshare/ && sudo cp ~/wordspace/mm/ /mnt/bdshare/


SRCDIRS = $(shell ls -l | grep ^d | awk '{if($$9 != "protocpp") print $$9 }')
aa:
	@echo $(SRCDIRS)
