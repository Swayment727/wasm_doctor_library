TARGET = libwasmdoctor
TARGET_TEST = build/test/test

CC = gcc
CFLAGS = -Wall -pedantic

SOURCE_DIR = src
BUILD_DIR = build

SOURCE_DIR_TEST = test
BUILD_DIR_TEST = build/test

MKDIR = mkdir -p

.PHONY: all
all: compile test

.PHONY: compile
compile: $(BUILD_DIR)/$(TARGET).a
	@echo "compile"

$(BUILD_DIR)/$(TARGET).a: $(BUILD_DIR)/wasm_doctor.o $(BUILD_DIR)/mem_addr_validator.o
	ar rcs $@ $^

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c $(SOURCE_DIR)/%.h
	$(MKDIR) $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: test
test: $(TARGET_TEST)
	@echo "test"

$(TARGET_TEST): $(BUILD_DIR_TEST)/mem_addr_validator_test.o
	$(CC) $(CFLAGS) -o $@ $^ -L$(BUILD_DIR) -lwasmdoctor

$(BUILD_DIR_TEST)/%.o: $(SOURCE_DIR_TEST)/%.c
	$(MKDIR) $(BUILD_DIR_TEST)
	$(CC) $(CFLAGS) -o $@ -c $< -I$(SOURCE_DIR)

clean:
	rm -rf $(BUILD_DIR)
