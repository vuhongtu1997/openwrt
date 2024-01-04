CC ?= gcc
CXX ?= g++
OBJEXT ?= .o
BUILD_PATH = build

ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif

LINKEDLIBS = -lssl -lcrypto -lcurl -lpthread -lmosquitto -luci -ljson-c -lsqlite3 # Librarys to include in the link

DEVICPPESRC += $(wildcard src/*.cpp)
DEVICPPESRC += $(wildcard src/BuildCmdUart/*.cpp)
DEVICPPESRC += $(wildcard src/Curtain/*.cpp)
DEVICPPESRC += $(wildcard src/Include/*.cpp)
DEVICPPESRC += $(wildcard src/JsonHandle/*.cpp)
DEVICPPESRC += $(wildcard src/Light/*.cpp)
DEVICESRC += $(wildcard src/logging/*.c)
DEVICPPESRC += $(wildcard src/Mqtt/*.cpp)
DEVICPPESRC += $(wildcard src/ProcessUart/*.cpp)
DEVICESRC += $(wildcard src/ProcessUart/*.c)
DEVICPPESRC += $(wildcard src/rapidjson/*.cpp)
DEVICPPESRC += $(wildcard src/Remote/*.cpp)
DEVICPPESRC += $(wildcard src/RemoteMultiButtons/*.cpp)
DEVICPPESRC += $(wildcard src/ScreenTouch/*.cpp)
DEVICPPESRC += $(wildcard src/Sensor/*.cpp)
DEVICPPESRC += $(wildcard src/SwitchOnOff/*.cpp)
DEVICPPESRC += $(wildcard src/BackupBle/*.cpp)

CPPSRC = $(wildcard *.cpp) $(DEVICPPESRC)
CPPOBJ = $(DEVICPPESRC:.cpp=$(OBJEXT))
CSRC = $(wildcard *.c) $(DEVICESRC)
COBJ = $(DEVICESRC:.c=$(OBJEXT))
BUILTOBJ = $(addprefix $(BUILD_PATH)/,$(CPPOBJ) $(COBJ))

APP = SYSTEM

all: $(APP)

.PHONY: all $(APP) clean

$(APP): $(BUILTOBJ)
	$(CXX) $(LINKFLAGS) -o $@ $(BUILTOBJ) $(LINKEDLIBS)

$(BUILD_PATH)/%.o: %.cpp
	mkdir -p $(@D)
	$(CC) $(COMPFLAGS) $(INCLUDES) -c $< -o $@
	
$(BUILD_PATH)/%.o: %.c
	mkdir -p $(@D)
	$(CC) $(COMPFLAGS) $(INCLUDES) -c $< -o $@
	
install: $(APP)
	install -d $(DESTDIR)$(PREFIX)/bin/
	install -m 644 $(APP) $(DESTDIR)$(PREFIX)/bin/

clean: 
	rm -rf $(APP) $(BUILD_PATH)

