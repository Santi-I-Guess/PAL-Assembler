# Two cars in every pot and a chicken in every garage.

PROJECT = final_project
SRC_DIRS  = src/assembler src/misc src/simulator src
SRC_FILES = $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.cpp))
H_FILES   = $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.h))
REZ_FILES = resources/pseudo_assembly_assembler.png
USERNAME  = santiago_sagastegui

# https://www.gnu.org/software/make/manual/html_node/File-Name-Functions.html
# this is why you read documentation, kids
OBJECTS = $(patsubst %.cpp, build/%.o, $(notdir $(SRC_FILES)))

CXX = g++
# remove fsanitize later
CXXFLAGS_DEBUG = -g -fsanitize=address
CXXFLAGS_WARN  = -Wall -Werror -Wextra -Wconversion -Wdouble-promotion \
				 -Wunreachable-code -Wshadow -Wpedantic
CPPVERSION = -std=c++17

ARCHIVE_EXTENSION = zip

ifeq ($(shell echo "Windows"), "Windows")
	TARGET = $(PROJECT).exe
	DEL = del
	ZIPPER = tar -a -c -f
	ZIP_NAME = $(PROJECT)_$(USERNAME).$(ARCHIVE_EXTENSION)
	Q =
	MKDIR = New-Item -Name "build" -ItemType "Directory" -Force
else
	TARGET = $(PROJECT)
	DEL = rm -f
	ZIPPER = tar -acf
	Q= "
	ifeq ($(shell tar --version | grep -o "GNU tar"), GNU tar)
		ARCHIVE_EXTENSION = tar.gz
	endif
	ZIP_NAME = $(PROJECT)_$(USERNAME).$(ARCHIVE_EXTENSION)
	MKDIR = mkdir -p build
endif

BUILD_DIR = build

# order only prerequisite: don't build again, but ensure they exist
all: | $(BUILD_DIR)
	@make -C src
	@make -C src/assembler
	@make -C src/misc
	@make -C src/simulator
	$(CXX) -o $(TARGET) $(OBJECTS)

$(BUILD_DIR):
	$(MKDIR)

clean:
	$(DEL) $(TARGET) $(OBJECTS)

depend:
	@sed --in-place=.bak '/^# DEPENDENCIES/,$$d' Makefile
	@$(DEL) sed*
	@echo $(Q)# DEPENDENCIES$(Q) >> Makefile
	@$(CXX) -MM $(SRC_FILES) >> Makefile

# last time i used mkdir -p, i wasn't entirely sure it was DOS compatible
submission:
	@echo "Creating submission file $(ZIP_NAME) ..."
	@echo "...Zipping source files:   $(SRC_FILES) ..."
	@echo "...Zipping header files:   $(H_FILES) ..."
	@echo "...Zipping resource files: $(REZ_FILES)..."
	@echo "...Zipping Makefile ..."
	$(ZIPPER) $(ZIP_NAME) $(SRC_FILES) $(H_FILES) $(REZ_FILES) $(BUILD_DIR) \
		Makefile
	@echo "...$(ZIP_NAME) done!"

debug:
	g++ $(SRC_FILES) -DDEBUG -fsanitize=address -o $(TARGET)

.PHONY: all clean depend submission debug
.DEFAULT: all

# DEPENDENCIES
blueprint.o: src/assembler/blueprint.cpp src/assembler/blueprint.h \
 src/assembler/common_values.h
tokenizer.o: src/assembler/tokenizer.cpp src/assembler/tokenizer.h
translation.o: src/assembler/translation.cpp \
 src/assembler/common_values.h src/assembler/translation.h
cmd_line_opts.o: src/misc/cmd_line_opts.cpp src/misc/cmd_line_opts.h
file_handling.o: src/misc/file_handling.cpp src/misc/file_handling.h
main.o: src/main.cpp src/assembler/blueprint.h \
 src/assembler/common_values.h src/assembler/common_values.h \
 src/assembler/tokenizer.h src/assembler/translation.h \
 src/misc/cmd_line_opts.h src/misc/file_handling.h
