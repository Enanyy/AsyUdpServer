SUB_DIR = $(shell ls -l | grep ^d | awk '{print $$9}')
SRC_LIST = $(wildcard *.cc)
OBJ_LIST = $(patsubst %.cc, %.o, $(SRC_LIST))

all:$(SUB_DIR) $(OBJ_LIST)

$(SUB_DIR):ECHO
	make -C $@

$(OBJ_LIST):%.o:%.cc
	$(CXX) $(CFLAGS) $(PROTOFLAGS) $^ -o $(OBJ_DIR)/$@

ECHO:
	@echo $(SUB_DIR)
