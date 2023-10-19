CC = arm-none-eabi-gcc

OBJ_COPY = arm-none-eabi-objcopy

BUILD_DIR = ../build

C_FILES = $(shell find . -name "*.c")

OBJ_FILES = $(C_FILES:./%.c=$(BUILD_DIR)/%.o)

DEP_FILES = $(C_FILES:./%.c=$(BUILD_DIR)/%.d)

all: $(BUILD_DIR)/kernel.bin

$(BUILD_DIR)/kernel.bin: $(BUILD_DIR)/kernel.elf
	$(OBJ_COPY) $< -O binary $@ 

$(BUILD_DIR)/kernel.elf: $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -nostdlib -o $@ -Wl,-Tlink.ld -ggdb -O0 -Wl,--print-memory-usage

$(BUILD_DIR)/%.o: %.c
	$(CC) -c $< -o $@ -mcpu=cortex-m4 -mthumb -nostdinc -MMD -O0 -ggdb

program: $(BUILD_DIR)/kernel.bin
	sudo bossac -o 0x4000 -w $(BUILD_DIR)/kernel.bin -U -R

clean:
	rm $(BUILD_DIR)/* -v

-include $(DEP_FILES)