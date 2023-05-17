# run params
W = 800
H = 800
MODEL = diablo.obj

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
OBJ_DEBUG   := $(patsubst %.cpp, $(OBJ_DIR)debug/%.o, $(patsubst $(SRC_DIR)%, %, $(wildcard $(SRC))))

all: release debug

release: LDFLAGS += -s
release: CPPFLAGS += -O3 -D RELEASE
release: $(DST_DIR)$(TARGET_RELEASE)
release: $(RES)

debug: LDFLAGS += -g -ggdb -pg
debug: CPPFLAGS += -g -ggdb -pg -O0 -D DEBUG -D PROFILE
debug: $(DST_DIR)$(TARGET_DEBUG)
debug: $(RES)

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

$(RES): $(DST_DIR)%: $(RES_DIR)%
	rsync --mkpath -ar $< $@

run: MODE = rt
run:
	(cd $(DST_DIR) && ./$(TARGET_RELEASE) $(MODE) $(MODEL) $(W) $(H))

render: MODE=tga
render: run
	eog $(DST_DIR)output.tga &> /dev/null &

tests: MODE=tests
tests:
	(cd $(DST_DIR) && ./$(TARGET_DEBUG) $(MODE) $(MODEL) $(W) $(H))
	eog $(DST_DIR)output.tga $(DST_DIR)lines.tga $(DST_DIR)triangles.tga &> /dev/null &

profile:
	(cd $(DST_DIR) && ./$(TARGET_DEBUG) $(MODE) $(MODEL) $(W) $(H))
	gprof -b -P10 $(DST_DIR)$(TARGET_DEBUG) $(DST_DIR)gmon.out

clean:
	-rm -rf $(DST_DIR)
	-rm -rf $(OBJ_DIR)