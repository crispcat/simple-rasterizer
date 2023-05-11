COMPILER = clang++
CPPFLAGS = -std=c++14
LDFLAGS  =
LIBS     = -lm
DST_DIR  = bin/
SRC_DIR  = src/
OBJ_DIR  = obj/
RES_DIR  = res/

TARGET_RELEASE  = simple-rasterizer
TARGET_DEBUG    = simple-rasterizer-debug

SRC := $(shell find $(SRC_DIR) -type f -name '*.cpp')
RES := $(notdir $(wildcard $(RES_DIR)*))

OBJ_RELEASE := $(patsubst %.cpp,$(OBJ_DIR)release/%.o,$(patsubst $(SRC_DIR)%,%,$(wildcard $(SRC))))
OBJ_DEBUG   := $(patsubst %.cpp,$(OBJ_DIR)debug/%.o,$(patsubst $(SRC_DIR)%,%,$(wildcard $(SRC))))

.PHONY: all run profile clean

.all: release debug

release: LDFLAGS += -s
release: CPPFLAGS += -O3 -D RELEASE
release: $(DST_DIR)$(TARGET_RELEASE) $(DST_DIR)$(RES)

debug: LDFLAGS += -g -ggdb -pg
debug: CPPFLAGS += -g -ggdb -pg -O0 -D DEBUG -D PROFILE
debug: $(DST_DIR)$(TARGET_DEBUG) $(DST_DIR)$(RES)

$(DST_DIR)$(TARGET_RELEASE): $(OBJ_RELEASE)
	mkdir -p $(@D)
	$(COMPILER) -Wall $(LDFLAGS) -o $(DST_DIR)$(TARGET_RELEASE) $(OBJ_RELEASE) $(LIBS)

$(DST_DIR)$(TARGET_DEBUG): $(OBJ_DEBUG)
	mkdir -p $(@D)
	$(COMPILER) -Wall $(LDFLAGS) -o $(DST_DIR)$(TARGET_DEBUG) $(OBJ_DEBUG) $(LIBS)

$(OBJ_RELEASE): $(OBJ_DIR)release/%.o: $(SRC_DIR)%.cpp
	mkdir -p $(@D)
	$(COMPILER) -Wall $(CPPFLAGS) -c $(CFLAGS) $< -o $@

$(OBJ_DEBUG): $(OBJ_DIR)debug/%.o: $(SRC_DIR)%.cpp
	mkdir -p $(@D)
	$(COMPILER) -Wall $(CPPFLAGS) -c $(CFLAGS) $< -o $@

$(DST_DIR)$(RES): $(RES_DIR)$(RES)
	rsync -ar $(RES_DIR) $(DST_DIR)

MODEL=./african_head.obj
W=800
H=800

run:
	(cd $(DST_DIR) && ./$(TARGET_RELEASE) $(MODEL) $(W) $(H))
	eog $(DST_DIR)output.tga &> /dev/null &

profile:
	(cd $(DST_DIR) && ./$(TARGET_DEBUG) $(MODEL) $(W) $(H))
	eog $(DST_DIR)output.tga &> /dev/null &
	gprof -b $(DST_DIR)$(TARGET_DEBUG) $(DST_DIR)gmon.out

clean:
	-rm -rf $(DST_DIR)
	-rm -rf $(OBJ_DIR)