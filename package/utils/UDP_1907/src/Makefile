CC ?= gcc
CXX ?= g++
OBJEXT ?= .o
BUILD_PATH = build

ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif

COMPFLAGS =  -Wall -std=c++11 -I./src -Os -ffunction-sections -fdata-sections -Wl,--gc-sections -Wno-deprecated -Wno-deprecated-declarations -flto -fPIC
LINKFLAGS =  -Wall -std=c++11 -Os -ffunction-sections -fdata-sections -Wl,--gc-sections -flto

LINKEDLIBS = -lcurl # Librarys to include in the link

DEVICPPESRC += $(wildcard src/*.cpp)

CPPSRC = $(wildcard *.cpp) $(DEVICPPESRC)
CPPOBJ = $(DEVICPPESRC:.cpp=$(OBJEXT))
CSRC = $(wildcard *.c) $(DEVICESRC)
COBJ = $(DEVICESRC:.c=$(OBJEXT))
BUILTOBJ = $(addprefix $(BUILD_PATH)/,$(CPPOBJ) $(COBJ))

APP = UDP_1907

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

