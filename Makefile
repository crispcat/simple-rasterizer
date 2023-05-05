COMPILER = clang++
CPPFLAGS = -std=c++14
LDFLAGS  =
LIBS     = -lm
TARGET   = simple-rasterizer
DST_DIR  = ./bin/
SRC_DIR  = ./src/
OBJ_DIR  = ./obj/

SRC := $(shell find $(SRC_DIR) -type f -name '*.cpp')
OBJ := $(patsubst %.cpp,$(OBJ_DIR)%.o,$(notdir $(wildcard $(SRC))))

all: LDFLAGS += -s
all: CPPFLAGS += -O3 -D RELEASE
all: $(DST_DIR)$(TARGET)

debug: LDFLAGS +=
debug: CPPFLAGS += -g -ggdb -O0 -D DEBUG
debug: $(DST_DIR)$(TARGET)

profile: LDFLAGS += -g -ggdb -pg
profile: CPPFLAGS += -g -ggdb -pg -O0 -D PROFILE
profile: $(DST_DIR)$(TARGET)

$(DST_DIR)$(TARGET): $(OBJ)
	mkdir -p $(@D)
	$(COMPILER) -Wall $(LDFLAGS) -o $(DST_DIR)$(TARGET) $(OBJ) $(LIBS)

$(OBJ): ./obj/%.o: ./src/%.cpp
	mkdir -p $(@D)
	$(COMPILER) -Wall $(CPPFLAGS) -c $(CFLAGS) $< -o $@

run:
	(cd $(DST_DIR) && ./$(TARGET))
	eog $(DST_DIR)output.tga &> /dev/null &

run-profile: run
	gprof -b $(DST_DIR)$(TARGET) $(DST_DIR)gmon.out

clean:
	-rm -rf $(DST_DIR)
	-rm -rf $(OBJ_DIR)

.PHONY: all profile debug run clean profile-run