CC = arm-eabi-gcc

OBJ_COPY = arm-eabi-objcopy

BUILD_DIR = ../build

C_FILES = $(wildcard *.c)

OBJ_FILES = $(patsubst %.c,$(BUILD_DIR)/%.o,$(C_FILES))

DEP_FILES = $(wildcard $(BUILD_DIR)/*.d)

ifeq ($(OS),Windows_NT)
	CLEAN_COMMAND = del $(subst /,\,$(BUILD_DIR)\*) /Q
else
	CLEAN_COMMAND = rm $(BUILD_DIR)/* -v
endif

all: $(BUILD_DIR)/kernel.bin

$(BUILD_DIR)/kernel.bin: $(BUILD_DIR)/kernel.elf
	$(OBJ_COPY) $< -O binary $@ 

$(BUILD_DIR)/kernel.elf: $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -nostdlib -o $@ -Wl,-Tlink.ld -ggdb -O0 -Wl,--print-memory-usage

$(BUILD_DIR)/%.o: %.c
	$(shell mkdir -p $(dir $@))
	$(CC) -c $< -o $@ -mcpu=cortex-m4 -mthumb -nostdinc -MMD -O0 -ggdb

program: $(BUILD_DIR)/kernel.bin
	sudo bossac -o 0x4000 -w $(BUILD_DIR)/kernel.bin -U -R

clean:
	$(CLEAN_COMMAND)

-include $(DEP_FILES)