SUB_DIR 	= $(shell ls -l | grep ^d | awk '{print $$9}')
CC_LIST 	= $(wildcard *.cc)
CC_OBJ_LIST 	= $(patsubst %.cc, %.o, $(CC_LIST))

all:$(SUB_DIR) $(CC_OBJ_LIST)

$(SUB_DIR):ECHO
	make -C $@

$(CC_OBJ_LIST):%.o:%.cc
	$(CXX) $(CFLAGS) $(PROTOFLAGS) $^ -o $(OBJ_DIR)/$@

ECHO:
	@echo $(SUB_DIR)
