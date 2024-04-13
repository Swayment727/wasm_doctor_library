TARGET = libwasmdoctor
TARGET_TEST = build/test/test

CC = gcc
CFLAGS = -Wall -pedantic

SOURCE_DIR = src
BUILD_DIR = build

SOURCE_DIR_TEST = test
BUILD_DIR_TEST = build/test

.PHONY: all
all: compile test

.PHONY: compile
compile: $(BUILD_DIR)/$(TARGET).a
	@echo "compile"

$(BUILD_DIR)/$(TARGET).a: $(BUILD_DIR)/wasm_doctor.o $(BUILD_DIR)/mem_addr_validator.o
	ar rcs $@ $^

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c $(SOURCE_DIR)/%.h
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: test
test: $(TARGET_TEST)
	@echo "test"

$(TARGET_TEST): $(BUILD_DIR_TEST)/mem_addr_validator_test.o
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR_TEST)/%.o: $(SOURCE_DIR_TEST)/%.c
	$(CC) $(CFLAGS) -o $@ -c $< -L$(BUILD_DIR) -lwasmdoctor

clean:
	rm -rf $(BUILD_DIR)/*
