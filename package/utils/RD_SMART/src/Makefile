CC ?= gcc
CXX ?= g++
OBJEXT ?= .o
BUILD_PATH = build

ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif

COMPFLAGS =  -Wall -std=c++11 -I./src -Os -ffunction-sections -fdata-sections -Wl,--gc-sections -Wno-deprecated -Wno-deprecated-declarations -flto -fPIC
LINKFLAGS =  -Wall -std=c++11 -Os -ffunction-sections -fdata-sections -Wl,--gc-sections -flto

LINKEDLIBS = -lpthread -lsqlite3 -lmosquitto -luci # Librarys to include in the link

DEVICPPESRC += $(wildcard src/*.cpp)
DEVICPPESRC += $(wildcard src/RD/*.cpp)
DEVICPPESRC += $(wildcard src/RD/AHG/*.cpp)
DEVICESRC += $(wildcard src/RD/RINGBUFFER/*.c)
DEVICPPESRC += $(wildcard src/RD/AHG/JSON/*.cpp)
DEVICPPESRC += $(wildcard src/RD/AHG/JSON/CONFIG/*.cpp)
DEVICPPESRC += $(wildcard src/RD/AHG/JSON/CONTROL/*.cpp)
DEVICPPESRC += $(wildcard src/RD/GHA/*.cpp)
DEVICPPESRC += $(wildcard src/RD/GHA/JSON/*.cpp)
DEVICPPESRC += $(wildcard src/RD/GHA/JSON/GHA_RSP_CONTROL/*.cpp)
DEVICPPESRC += $(wildcard src/RD/GHA/JSON/GHA_RSP_REMOTE/*.cpp)
DEVICPPESRC += $(wildcard src/RD/GHA/JSON/GHA_RSP_SENSOR/*.cpp)
DEVICPPESRC += $(wildcard src/RD/GHA/JSON/GHA_RSP_AIBOX/*.cpp)
DEVICPPESRC += $(wildcard src/RD/MQTT/*.cpp)
DEVICPPESRC += $(wildcard src/RD/TIMER/*.cpp)
DEVICPPESRC += $(wildcard src/RD/DVSTATUS/*.cpp)
DEVICPPESRC += $(wildcard src/RD/SCENEDELAY/*.cpp)
DEVICPPESRC += $(wildcard src/RD/UDP/*.cpp)


CPPSRC = $(wildcard *.cpp) $(DEVICPPESRC)
CPPOBJ = $(DEVICPPESRC:.cpp=$(OBJEXT))
CSRC = $(wildcard *.c) $(DEVICESRC)
COBJ = $(DEVICESRC:.c=$(OBJEXT))
BUILTOBJ = $(addprefix $(BUILD_PATH)/,$(CPPOBJ) $(COBJ))

APP = RD_SMART

all: $(APP)

.PHONY: all $(APP) clean

$(APP): $(BUILTOBJ)
	$(CXX) $(LINKFLAGS) -o $@ $(BUILTOBJ) $(LINKEDLIBS)
	
$(BUILD_PATH)/%.o: %.cpp
	mkdir -p $(@D)
	$(CXX) $(COMPFLAGS) $(INCLUDES) -c $< -o $@
	
$(BUILD_PATH)/%.o: %.c
	mkdir -p $(@D)
	$(CC) $(COMPFLAGS) $(INCLUDES) -c $< -o $@
	
install: $(APP)
	install -d $(DESTDIR)$(PREFIX)/bin/
	install -m 644 $(APP) $(DESTDIR)$(PREFIX)/bin/

clean: 
	rm -rf $(APP) $(BUILD_PATH)

