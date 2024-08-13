TARGET = libwasmdoctor
TARGET_TEST = build/test/test

CC = gcc
CFLAGS = -Wall -pedantic -fPIC -g

SOURCE_DIR = src
BUILD_DIR = build

SOURCE_DIR_TEST = test
BUILD_DIR_TEST = build/test

MKDIR = mkdir -p

.PHONY: all
all: compile test doc

.PHONY: compile
compile: $(BUILD_DIR)/$(TARGET).a

$(BUILD_DIR)/$(TARGET).a: $(BUILD_DIR)/wasm_doctor.o $(BUILD_DIR)/shadow_memory_validator.o $(BUILD_DIR)/heap_use_validator.o $(BUILD_DIR)/local_validator.o $(BUILD_DIR)/error_reporter.o $(BUILD_DIR)/wasm_state.o $(BUILD_DIR)/linear_stack_validator.o $(BUILD_DIR)/zero_address_access_validator.o $(BUILD_DIR)/report_options.o
	ar rcs $@ $^

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c $(SOURCE_DIR)/%.h
	$(MKDIR) $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: test
test: $(TARGET_TEST) run-test

.PHONY: run-test
run-test:
	./$(TARGET_TEST)

$(TARGET_TEST): $(BUILD_DIR_TEST)/wasm_doctor_test.o $(BUILD_DIR)/wasm_doctor.o $(BUILD_DIR)/shadow_memory_validator.o $(BUILD_DIR)/heap_use_validator.o $(BUILD_DIR)/local_validator.o $(BUILD_DIR)/error_reporter.o $(BUILD_DIR)/wasm_state.o $(BUILD_DIR)/linear_stack_validator.o $(BUILD_DIR)/zero_address_access_validator.o
	$(CC) $(CFLAGS) -o $@ $^ -L$(BUILD_DIR) -lwasmdoctor

$(BUILD_DIR_TEST)/%.o: $(SOURCE_DIR_TEST)/%.c
	$(MKDIR) $(BUILD_DIR_TEST)
	$(CC) $(CFLAGS) -o $@ -c $< -I$(SOURCE_DIR)

.PHONY: doc
doc: Doxyfile $(HEADERS)
	doxygen Doxyfile

clean:
	rm -rf $(BUILD_DIR) doc 2>/dev/null
