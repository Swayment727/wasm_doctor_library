TARGET = wasm_doctor
TARGET_TEST = build/test/test

CC = gcc
MKDIR = mkdir -p

CFLAGS = -Wall -pedantic

SOURCE_DIR = src
TEST_SOURCE_DIR = test
BUILD_DIR = build

BUILD_DIR_TEST = build/test

SOURCES = $(wildcard $(SOURCE_DIR)/*.c)
TEST_SOURCES = $(wildcard $(TEST_SOURCE_DIR)/*.c)
HEADERS = $(wildcard $(SOURCE_DIR)/*.h)

OBJECTS = $(SOURCES:$(SOURCE_DIR)/%.c=$(BUILD_DIR)/%.o)

OBJECTS_TEST = $(TEST_SOURCES:$(TEST_SOURCE_DIR)/%.c=$(BUILD_DIR_TEST)/%.o)

.PHONY: all
all: compile test doc
	@echo "finished"

.PHONY: compile
compile: $(TARGET)
	@echo "compile"

$(TARGET): $(OBJECTS)
	$(CC) $^ -o $@

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c
	$(MKDIR) $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -c -o $@

.PHONY: test
test: compile-test run-test
	@echo "test finished..."

.PHONY: compile-test
compile-test: $(TARGET_TEST)
	@echo "compile test..."

.PHONY: run-test
run-test:
	./$(TARGET_TEST)

$(TARGET_TEST): $(OBJECTS_TEST) $(OBJECTS)
	$(CC) $^ -o $@

$(BUILD_DIR_TEST)/%.o: $(TEST_SOURCE_DIR)/%.c
	$(MKDIR) $(BUILD_DIR_TEST)
	$(CC) $(CFLAGS) $< -c -o $@

.PHONY: doc
doc: Doxyfile $(HEADERS)
	doxygen Doxyfile

.PHONY: clean
clean:
	rm -rf $(TARGET) $(TARGET_TEST) $(BUILD_DIR) $(BUILD_DIR_TEST) doc/ 2>/dev/null
