# -=== Vars ===-
ifeq ("$(wildcard config.mk)","")
$(error File config.mk not found! Run ./configure to generate it)
endif

include config.mk

BUILD_DIR 		:= build
BOOT_DIR 		:= boot
KERNEL_DIR		:= kernel
SRC_DIR 		:= $(KERNEL_DIR)/src
INC_DIR 		:= $(KERNEL_DIR)/include
LINK_FILE 		:= link.ld

ASM_DFLAGS 		:= -g -F dwarf
INCLUDE_FLAGS 	:= -I$(INC_DIR)

SRC_FILES 		:= $(shell find $(SRC_DIR) -type f -name "*.c")
OBJ_FILES 		:= $(SRC_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

DISK_IMG		:= disk.img
SCRIPTS_DIR		:= scripts
MAKE_IMG_SCRIPT	:= $(SCRIPTS_DIR)/img_maker.sh
RUN_SCRIPT		:= $(SCRIPTS_DIR)/img_run.sh
FLASH_SCRIPT	:= $(SCRIPTS_DIR)/img_flash.sh

# -=== MSG ===-
MESS			:= printf
RESET       	:= \033[0m
RED         	:= \033[31m
GREEN       	:= \033[32m
YELLOW      	:= \033[33m
MAGENTA     	:= \033[35m
CYAN        	:= \033[36m

ifneq ($(TERM),dumb)
  ifneq (, $(shell command -v tput 2>/dev/null))
    RESET  := $(shell tput sgr0)
    RED    := $(shell tput setaf 1)
    GREEN  := $(shell tput setaf 2)
    YELLOW := $(shell tput setaf 3)
    MAGENTA:= $(shell tput setaf 5)
    CYAN   := $(shell tput setaf 6)
  endif
endif

# -=== Compile ===-
all: boot kernel

# -=== Kernel ===-
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(MESS) '[$(CYAN)CC$(RESET)] %s\n' "$<"
	@$(CC) $(FLAGS) $(INCLUDE_FLAGS) -c $< -o $@

$(BUILD_DIR)/kernel_entry.o: $(SRC_DIR)/kernel_entry.asm
	@$(MESS) '[$(RED)ASM$(RESET)]  %s\n' "$<"
	@$(ASM) -f elf32 $(ASM_DFLAGS) $< -o $@

$(BUILD_DIR)/isr_asm.o: $(SRC_DIR)/arch/interrupts/isr.asm
	@$(MESS) '[$(RED)ASM$(RESET)] %s\n' "$<"
	@$(ASM) -f elf32 $(ASM_DFLAGS) $< -o $@

kernel: boot $(BUILD_DIR)/kernel_entry.o $(BUILD_DIR)/isr_asm.o $(OBJ_FILES)
	@$(MESS) '[$(GREEN)LD$(RESET)] %s\n' 'Linking all'
	@$(LD) -T $(LINK_FILE) \
		$(BUILD_DIR)/kernel_entry.o \
		$(OBJ_FILES) \
		$(BUILD_DIR)/isr_asm.o \
	    -o $(BUILD_DIR)/full_kernel.elf \
	    -Map=$(BUILD_DIR)/linkmap.txt

	@$(MESS) '[$(MAGENTA)BIN$(RESET)] %s\n' 'Generating full_kernel.bin'
	@$(OBJCOPY) -O binary build/full_kernel.elf build/full_kernel.bin

# -=== Boot ===-
boot:
	@mkdir -p $(BUILD_DIR)
	@$(MESS) '[$(RED)ASM$(RESET)] %s\n' 'boot/boot_second.asm'
	@$(ASM) -f bin $(BOOT_DIR)/boot.asm -o $(BUILD_DIR)/boot.bin
	@$(ASM) -f bin $(BOOT_DIR)/boot_second.asm -o $(BUILD_DIR)/boot_second.bin

# -=== VM ===-
img-clear:
	@$(MESS) '[$(RED)CLEAN$(RESET)] %s\n' 'Removing $(DISK_IMG)'
	@rm -f $(DISK_IMG)

img-create:
	@$(MESS) '[$(YELLOW)IMG$(RESET)] %s\n' 'Creating $(DISK_IMG)'
	@./$(MAKE_IMG_SCRIPT) $(DISK_IMG) $(VMDISK)

img-flash:
	@$(MESS) '[$(YELLOW)FLASH$(RESET)] %s\n' 'Flashing $(DISK_IMG)'
	@./$(FLASH_SCRIPT) $(DISK_IMG) $(BUILD_DIR)/boot.bin $(BUILD_DIR)/boot_second.bin

img-run:
	@$(MESS) '[$(YELLOW)QEMU$(RESET)] %s\n' 'Starting VM'
	@./$(RUN_SCRIPT) $(DISK_IMG) $(VMRAM)

# -=== Utils ===-
tools:
	@$(MESS) '[$(GREEN)TOOL$(RESET)] %s\n' "Installing in '~/usr/local/i386elfgcc'"
	@./tool_make.sh

clean:
	@$(MESS) '[$(RED)CLEAN$(RESET)] %s\n' "Remove 'build/'"
	@rm -rf $(BUILD_DIR)

# -=== PHONY ===-
.PHONY: clean tools img-run img-flash img-create img-clean kernel boot all
