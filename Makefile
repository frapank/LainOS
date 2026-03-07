# -- Vars --
include config.mk

BUILD_DIR 		:= build
BOOT_DIR 		:= boot
SRC_DIR 		:= kernel/src
INC_DIR 		:= kernel/include
LINK_FILE 		:= link.ld

ASM_DFLAGS 		:= -g -F dwarf
INCLUDE_FLAGS 	:= -I$(INC_DIR)

SRC_FILES 		:= $(shell find $(SRC_DIR) -type f -name "*.c")
OBJ_FILES 		:= $(SRC_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

RESET       = \033[0m
RED         = \033[31m
GREEN       = \033[32m
YELLOW      = \033[33m
MAGENTA     = \033[35m
CYAN        = \033[36m

# -- Compile --
all: boot kernel

# Files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "[$(CYAN)CC$(RESET)] $<"
	@$(CC) $(FLAGS) $(INCLUDE_FLAGS) -c $< -o $@

$(BUILD_DIR)/kernel_entry.o: $(BOOT_DIR)/kernel_entry.asm
	@echo "[$(RED)ASM$(RESET)]  $<"
	@$(ASM) -f elf32 $(ASM_DFLAGS) $< -o $@

$(BUILD_DIR)/isr_asm.o: $(SRC_DIR)/arch/interrupts/isr.asm
	@echo "[$(RED)ASM$(RESET)] $<"
	@$(ASM) -f elf32 $(ASM_DFLAGS) $< -o $@

# Kernel
boot:
	@mkdir -p $(BUILD_DIR)
	@echo "[$(RED)ASM$(RESET)] boot/boot_second.asm"
	@$(ASM) -f bin $(BOOT_DIR)/boot.asm -o $(BUILD_DIR)/boot.bin
	@$(ASM) -f bin $(BOOT_DIR)/boot_second.asm -o $(BUILD_DIR)/boot_second.bin

kernel: boot $(BUILD_DIR)/kernel_entry.o $(BUILD_DIR)/isr_asm.o $(OBJ_FILES)
	@echo "[$(GREEN)LD$(RESET)] Linking all"
	@$(LD) -T $(LINK_FILE) \
		$(BUILD_DIR)/kernel_entry.o \
		$(OBJ_FILES) \
		$(BUILD_DIR)/isr_asm.o \
	    -o $(BUILD_DIR)/full_kernel.elf \
	    -Map=$(BUILD_DIR)/linkmap.txt

	@echo "[$(MAGENTA)BIN$(RESET)] Generating everything.bin"
	@$(OBJCOPY) -O binary build/full_kernel.elf build/full_kernel.bin
	@$(CAT) build/boot.bin build/boot_second.bin build/full_kernel.bin > build/everything.bin

# Utils
run: all
	@echo "[$(YELLOW)QEMU$(RESET)] Starting QemuVM"
	@qemu-system-x86_64 -drive file=$(BUILD_DIR)/everything.bin,format=raw -m 3G \
		-enable-kvm -boot c -serial stdio

tools:
	@echo "[$(GREEN)TOOL$(RESET)] Installing in '~/usr/local/i386elfgcc'"
	@./tool_make.sh

clean:
	@echo "[$(RED)CLEAN$(RESET)] Remove 'build/'"
	@rm -rf $(BUILD_DIR)

.PHONY: all boot kernel run tools clean
