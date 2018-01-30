SUB_DIR = $(shell ls -l | grep ^d | awk '{print $$9}')

CPP_LIST = $(wildcard *.cpp)
CPP_OBJ_LIST = $(patsubst %.cpp, %.o, $(CPP_LIST))

CC_LIST = $(wildcard *.c)
CC_OBJ_LIST = $(patsubst %.c, %.o, $(CC_LIST))


all:$(SUB_DIR) $(CPP_OBJ_LIST) $(CC_OBJ_LIST)

$(SUB_DIR):ECHO
	make -C $@

$(CPP_OBJ_LIST):%.o:%.cpp
	$(CXX) $(CFLAGS) $^ -o $(OBJ_DIR)/$@

$(CC_OBJ_LIST):%.o:%.c
	$(CC) $(CFLAGS) $^ -o $(OBJ_DIR)/$@

ECHO:
	@echo $(SUB_DIR)
