# make params
COMPILER = clang++
CXXFLAGS = -std=c++17 -stdlib=libstdc++
LDFLAGS  =
LIBS     = -lm -lX11
DST_DIR  = bin/
SRC_DIR  = src/
OBJ_DIR  = obj/
RES_DIR  = res/

TARGET_RELEASE  = simple-rasterizer
TARGET_DEBUG    = simple-rasterizer-debug

SRC := $(shell find $(SRC_DIR) -type f -name '*.cpp')
RES := $(shell find $(RES_DIR) -type f -name '*')

OBJ_RELEASE := $(patsubst %.cpp, $(OBJ_DIR)release/%.o, $(patsubst $(SRC_DIR)%, %, $(wildcard $(SRC))))
OBJ_DEBUG := $(patsubst %.cpp, $(OBJ_DIR)debug/%.o, $(patsubst $(SRC_DIR)%, %, $(wildcard $(SRC))))
RESOURCES := $(patsubst $(RES_DIR)%, $(DST_DIR)%, $(wildcard $(RES)))

all: release debug

.depend_release:
	$(COMPILER) $(CXXFLAGS) -MM $(wildcard $(SRC)) > .depend_release
	awk '{ ORS = (/\\$$/ ? "" : RS); sub(/\\$$/, ""); print }' .depend_release > .tmp_release && mv .tmp_release .depend_release
	awk '{gsub(/  +/," "); sub(/^ /,""); sub(/ $$/,"")}1' .depend_release > .tmp_release && mv .tmp_release .depend_release
	awk '{print "obj/release/" $$0}' .depend_release > .tmp_release && mv .tmp_release .depend_release

.depend_debug:
	$(COMPILER) $(CXXFLAGS) -MM $(wildcard $(SRC)) > .depend_debug
	awk '{ ORS = (/\\$$/ ? "" : RS); sub(/\\$$/, ""); print }' .depend_debug > .tmp_debug && mv .tmp_debug .depend_debug
	awk '{gsub(/  +/," "); sub(/^ /,""); sub(/ $$/,"")}1' .depend_debug > .tmp_debug && mv .tmp_debug .depend_debug
	awk '{print "obj/debug/" $$0}' .depend_debug > .tmp_debug && mv .tmp_debug .depend_debug

include .depend_release
include .depend_debug

release: LDFLAGS += -s
release: CXXFLAGS += -O3 -D RELEASE
release: $(DST_DIR)$(TARGET_RELEASE) $(RESOURCES)

debug: LDFLAGS += -g -ggdb -pg
debug: CXXFLAGS += -O0 -g -ggdb -pg -O0 -D DEBUG -D PROFILE
debug: $(DST_DIR)$(TARGET_DEBUG) $(RESOURCES)

$(DST_DIR)$(TARGET_RELEASE): $(OBJ_RELEASE)
	mkdir -p $(@D)
	$(COMPILER) -Wall $(LDFLAGS) -o $(DST_DIR)$(TARGET_RELEASE) $(OBJ_RELEASE) $(LIBS)

$(DST_DIR)$(TARGET_DEBUG): $(OBJ_DEBUG)
	mkdir -p $(@D)
	$(COMPILER) -Wall $(LDFLAGS) -o $(DST_DIR)$(TARGET_DEBUG) $(OBJ_DEBUG) $(LIBS)

$(OBJ_DIR)release/%.o: $(SRC_DIR)%.cpp
	mkdir -p $(@D)
	$(COMPILER) -Wall $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)debug/%.o: $(SRC_DIR)%.cpp
	mkdir -p $(@D)
	$(COMPILER) -Wall $(CXXFLAGS) -c $< -o $@

$(RESOURCES): $(DST_DIR)%: $(RES_DIR)%
	rsync --mkpath -ar $< $@

.PHONY: clean

clean:
	-rm -f .release .debug
	-rm -rf $(DST_DIR)
	-rm -rf $(OBJ_DIR)

include .run