#!/usr/bin/env bash
set -euo pipefail

log() {
    echo "[*] $1"
}

check_sudo() {
    if [[ $EUID -ne 0 ]]; then
        echo "This script requires root privileges. Relaunching with sudo..."
        exit 1
    fi
}

error_exit() {
    echo "Error: $1" >&2
    exit 1
}

usage() {
    echo "Usage: $0 <disk.img> <stage1.bin> <stage2.bin>"
    exit 1
}

check_tools() {
    REQUIRED_TOOLS=(dd losetup)

    for tool in "${REQUIRED_TOOLS[@]}"; do
        if ! command -v "$tool" >/dev/null 2>&1; then
            error_exit "Required tool '$tool' not found."
        fi
    done
}

main() {
    check_sudo
    check_tools

    if [[ $# -ne 4 ]]; then
        usage
    fi

    IMAGE="$1"
    STAGE1="$2"
    STAGE2="$3"
    KERNEL="$4"

    [[ -f "$IMAGE"  ]] || error_exit "Disk image '$IMAGE' not found."
    [[ -f "$STAGE1" ]] || error_exit "Stage1 binary '$STAGE1' not found."
    [[ -f "$STAGE2" ]] || error_exit "Stage2 binary '$STAGE2' not found."
    [[ -f "$KERNEL" ]] || error_exit "Kernel binary '$KERNEL' not found."


    dd if="$STAGE1" of="$IMAGE"     bs=446 count=1   conv=notrunc status=none
    dd if="$STAGE2" of="$IMAGE"     bs=512 seek=2048 conv=notrunc status=none

    LOOPDEV=$(losetup -fP --show "$IMAGE")

    rm -rf tmp/
    mkdir tmp/
    mount "${LOOPDEV}p2" tmp/
    mkdir -p tmp/boot
    cp "$KERNEL" tmp/boot/lain_kernel.bin
    umount tmp/
    rm -rf tmp/

    losetup -d "$LOOPDEV"
}

main "$@"
