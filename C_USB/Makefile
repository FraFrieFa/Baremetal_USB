CC = arm-none-eabi-gcc

OBJ_COPY = arm-none-eabi-objcopy

BUILD_DIR = ./build
SOURCE_DIR = ./src

C_FILES = $(wildcard $(SOURCE_DIR)/*.c)

OBJ_FILES = $(C_FILES:$(SOURCE_DIR)/%.c=$(BUILD_DIR)/%.o)

DEP_FILES = $(wildcard $(BUILD_DIR)/*.d)

CLEAN_COMMAND = rm $(BUILD_DIR)/* -v

all: $(BUILD_DIR)/kernel.bin

$(BUILD_DIR)/kernel.bin: $(BUILD_DIR)/kernel.elf
	$(OBJ_COPY) $< -O binary $@

$(BUILD_DIR)/kernel.elf: $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -nostdlib -o $@ -Wl,-Tlink.ld -ggdb -O0 -Wl,--print-memory-usage

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c
	$(shell mkdir -p $(dir $@))
	$(CC) -c $< -o $@ -mcpu=cortex-m4 -mthumb -nostdinc -MMD -O0 -ggdb

program: $(BUILD_DIR)/kernel.bin
	bossac -o 0x4000 -w $(BUILD_DIR)/kernel.bin -U -R

clean:
	$(CLEAN_COMMAND)

-include $(DEP_FILES)
