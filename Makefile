# make params
COMPILER = clang++
CPPFLAGS = -std=c++14
LDFLAGS  =
LIBS     = -lm -lX11
DST_DIR  = bin/
SRC_DIR  = src/
OBJ_DIR  = obj/
RES_DIR  = res/

TARGET_RELEASE  = simple-rasterizer
TARGET_DEBUG    = simple-rasterizer-debug

SRC := $(shell find $(SRC_DIR) -type f -name '*.cpp')
RES := $(patsubst %, $(DST_DIR)%, $(notdir $(wildcard $(RES_DIR)*)))

OBJ_RELEASE := $(patsubst %.cpp, $(OBJ_DIR)release/%.o, $(patsubst $(SRC_DIR)%, %, $(wildcard $(SRC))))
OBJ_DEBUG := $(patsubst %.cpp, $(OBJ_DIR)debug/%.o, $(patsubst $(SRC_DIR)%, %, $(wildcard $(SRC))))

all: release debug

.depend_release:
	$(COMPILER) $(CPPFLAGS) $(CFLAGS) -MM $(wildcard $(SRC)) > .depend_release
	awk -i inplace '{ORS = /\\/? "": RS; sub(/\\$$/, ""); print}' .depend_release
	sed -i -e "s/^/obj\/release\//" .depend_release

.depend_debug:
	$(COMPILER) $(CPPFLAGS) $(CFLAGS) -MM $(wildcard $(SRC)) > .depend_debug
	awk -i inplace '{ORS = /\\/? "": RS; sub(/\\$$/, ""); print}' .depend_debug
	sed -i -e "s/^/obj\/debug\//" .depend_debug

include .depend_release
include .depend_debug

release: LDFLAGS += -s
release: CPPFLAGS += -O3 -D RELEASE
release: $(DST_DIR)$(TARGET_RELEASE) $(RES)

debug: LDFLAGS += -g -ggdb -pg
debug: CPPFLAGS += -g -ggdb -pg -O0 -D DEBUG -D PROFILE
debug: $(DST_DIR)$(TARGET_DEBUG) $(RES)

$(DST_DIR)$(TARGET_RELEASE): $(OBJ_RELEASE)
	mkdir -p $(@D)
	$(COMPILER) -Wall $(LDFLAGS) -o $(DST_DIR)$(TARGET_RELEASE) $(OBJ_RELEASE) $(LIBS)

$(DST_DIR)$(TARGET_DEBUG): $(OBJ_DEBUG)
	mkdir -p $(@D)
	$(COMPILER) -Wall $(LDFLAGS) -o $(DST_DIR)$(TARGET_DEBUG) $(OBJ_DEBUG) $(LIBS)

$(OBJ_DIR)release/%.o: $(SRC_DIR)%.cpp
	mkdir -p $(@D)
	$(COMPILER) -Wall $(CPPFLAGS) -c $(CFLAGS) $< -o $@

$(OBJ_DIR)debug/%.o: $(SRC_DIR)%.cpp
	mkdir -p $(@D)
	$(COMPILER) -Wall $(CPPFLAGS) -c $(CFLAGS) $< -o $@

$(RES): $(DST_DIR)%: $(RES_DIR)%
	rsync --mkpath -ar $< $@

.PHONY: clean

clean:
	-rm -f .release .debug
	-rm -rf $(DST_DIR)
	-rm -rf $(OBJ_DIR)

include .run