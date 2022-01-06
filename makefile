.PHONY: help
help:
	@echo "Makefile to build the EPIC - \"Efficient Power Index Computation\" application"
	@echo "The following rules are available:"
	@echo "all:   compile the application"
	@echo "docs:  create the documentation ($(DOC_TARGET), docs/html/index.html)"
	@echo "clean: remove all compilation and documentation products"
	@echo "help:  shows this useful message"


#
# BUILD epic executable
#
TARGET=epic

BUILD_DIR=build
SRC_DIR=src
INC_DIR=include

SRCS=$(shell find $(SRC_DIR) -name "*.cpp") # find all source files (.cpp)
OBJS=$(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS)) # generate list of object files (.o)


CFLAGS= -Ofast -Wall -std=c++17 -I$(INC_DIR)

# Add the path to your manual GMP installation to the variable below using the -L prefix (e.g. LDFLAGS=-Lpath/to/gmp-lib -lgmpxx -lgmp)
LDFLAGS=-lgmpxx -lgmp

.PHONY: all
all:
	@cd $(SRC_DIR); find * -type d -exec mkdir -p -- ../$(BUILD_DIR)/{} \;
	$(MAKE) $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) -c -o $@ $< $(CFLAGS)


#
#
#
TEST_DIR=unittests
TESTS=$(shell find $(TEST_DIR) -name "*.cpp") # find all test files (.cpp)
TEST_OBJS=$(filter-out $(BUILD_DIR)/main.o,$(OBJS)) # copy OBJS without original main file
TEST_OBJS+=$(patsubst $(TEST_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(TESTS)) # add test specific .o files
TEST_TARGET=testtarget

.PHONY: test
test:
	@echo $(TEST_OBJS)
	@cd $(SRC_DIR); find * -type d -exec mkdir -p -- ../$(BUILD_DIR)/{} \; # create directory-hierarchy from SRC_DIR in BUILD_DIR
	@cd $(TEST_DIR); find * -type d -exec mkdir -p -- ../$(BUILD_DIR)/{} \; # create directory-hierarchy from TEST_DIR in BUILD_DIR
	$(MAKE) $(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)


$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp
	$(CXX) -c -o $@ $< $(CFLAGS)


#
# CREATE epic documentation (.html/.pdf)
#
DOC_TARGET=docs.pdf

DOC_DIR=docs
DOC_DOXYFILE=Doxyfile
DOC_LATEX_DIR=$(DOC_DIR)/latex

.PHONY: docs
docs:
	$(MAKE) $(DOC_TARGET)

$(DOC_LATEX_DIR)/makefile:
	doxygen $(DOC_DOXYFILE)

$(DOC_TARGET): $(DOC_LATEX_DIR)/makefile
	cd $(DOC_LATEX_DIR) && $(MAKE)
	cp $(DOC_LATEX_DIR)/refman.pdf ./$(DOC_TARGET)


#
# CLEAN
#
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
	rm -f $(TARGET)
	rm -rf $(DOC_DIR)
	rm -f $(DOC_TARGET)
