
TARGET_EXEC ?= game
TARGET_DIR ?= ./bin
BUILD_DIR ?= ./build
SOURCE_DIRS ?= ./src
RESOURCES_DIR ?= ./resources

LIBRARY_COMPILER_FLAGS ?= $(shell pkg-config glew glfw3 --cflags)
LIBRARY_LINKER_FLAGS ?= $(shell pkg-config glew glfw3 --libs) -framework OpenGL

# production
# PREPROC_DEFINES ?= -DNDEBUG
# dev
PREPROC_DEFINES ?= -DDEBUG

COPY_RESOURCES ?= rsync -rvui --progress
MKDIR_P ?= mkdir -p
COMPILER ?= clang++

SOURCES := $(shell find $(SOURCE_DIRS) -name *.cpp -o -name *.c)

INCLUDE_DIRS := $(shell find $(SOURCE_DIRS) -type d)

OBJECTS := $(SOURCES:%=$(BUILD_DIR)/%.o)
DEPENDENCIES := $(OBJECTS:.o=.d)

INCLUDE_FLAGS := $(addprefix -I,$(INCLUDE_DIRS))
CPPFLAGS ?= $(INCLUDE_FLAGS) $(PREPROC_DEFINES) -MMD -MP -g -std=c++14
CFLAGS ?= $(LIBRARY_COMPILER_FLAGS) -O0
LDFLAGS ?= $(LIBRARY_LINKER_FLAGS) -Wl,-headerpad_max_install_names

.PHONY: clean

$(TARGET_DIR)/$(TARGET_EXEC): $(OBJECTS)
	$(MKDIR_P) $(TARGET_DIR)
	$(COMPILER) $(OBJECTS) -o $@ $(LDFLAGS)
	$(COPY_RESOURCES) $(RESOURCES_DIR)/ $(TARGET_DIR)/

$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(COMPILER) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	$(RM) -r $(BUILD_DIR)
	$(RM) -r $(TARGET_DIR)

-include $(DEPENDENCIES)
