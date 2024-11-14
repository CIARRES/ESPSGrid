PROJECT_BINARY_NAME = powerMeter

CXX = g++
CC = gcc
CP = cp

all:	$(PROJECT_BINARY_NAME)

####################
#    Libraries     #
####################

LIBIEC_HOME=libiec61850
LIBSIM_HOME=libsimlink
LIBPUG_HOME=libpugixml

include $(LIBIEC_HOME)/make/target_system.mk
include $(LIBIEC_HOME)/make/stack_includes.mk

LIB_SIM_NAME = $(LIBSIM_HOME)/libsimlink.a
INCLUDES += -I$(LIBSIM_HOME)

LIB_PUG_NAME = $(LIBPUG_HOME)/libpugixml.a

include $(LIBIEC_HOME)/make/common_targets.mk

LDLIBS += -lm

$(LIB_SIM_NAME):
	cd $(LIBSIM_HOME); $(MAKE) -f Makefile

$(LIB_PUG_NAME):
	cd $(LIBPUG_HOME); $(MAKE) -f Makefile

libs:	$(LIB_NAME) $(LIB_SIM_NAME) $(LIB_PUG_NAME)

####################
#  ICD Generator   #
####################

ICD_GENERATOR = modelGenerator
ICD_GENERATOR_SOURCES = modelGenerator.cpp

modelGenerator: $(LIB_PUG_NAME)
	$(CXX) $(CFLAGS) -o $(ICD_GENERATOR) $(ICD_GENERATOR_SOURCES) -I$(LIBPUG_HOME) $(LIB_PUG_NAME) $(LDLIBS)

####################
#   Power Meter    #
####################

# Source files

CXXFILES = powerMeter.cpp
CXXFILES += iec61850Server.cpp

CFILES = static_model.c

# Object files

OBJ = $(CXXFILES:.cpp=.o) $(CFILES:.c=.o)

PROJECT_SOURCES = $(CXXFILES) $(CFILES)

DEPS = powerMeter.h
DEPS += common.h
DEPS += iec61850Server.h
DEPS += ladder.h
DEPS += static_model.h

PROJECT_ICD_FILE = powerMeter.cid

model:	$(PROJECT_ICD_FILE)
	java -jar $(LIBIEC_HOME)/tools/model_generator/genmodel.jar $(PROJECT_ICD_FILE)

%.o: %.cpp $(DEPS)
	$(CXX) $(CFLAGS) $(LDFLAGS) -c $< -o $@ $(INCLUDES) $(LIB_NAME) $(LIB_SIM_NAME) $(LDLIBS)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES) $(LIB_NAME) $(LDLIBS)

$(PROJECT_BINARY_NAME):	$(OBJ) $(LIB_NAME) $(LIB_SIM_NAME)
	$(CXX) $(CFLAGS) $(LDFLAGS) -o $(PROJECT_BINARY_NAME) $(PROJECT_SOURCES) $(INCLUDES) $(LIB_NAME) $(LIB_SIM_NAME) $(LDLIBS)
	mkdir -p vmd-filestore
	$(CP) $(PROJECT_BINARY_NAME) vmd-filestore/IEDSERVER.BIN

clean:
	rm -f $(PROJECT_BINARY_NAME) $(OBJ)
	rm -rf vmd-filestore
	rm -rf $(ICD_GENERATOR)
	cd $(LIBPUG_HOME) && $(MAKE) -f Makefile clean; cd ..
	cd $(LIBSIM_HOME) && $(MAKE) -f Makefile clean; cd ..
	cd $(LIBIEC_HOME) && $(MAKE) -f Makefile clean; cd ..
