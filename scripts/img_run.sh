#!/usr/bin/env bash
set -euo pipefail

error_exit() {
    echo "Error: $1" >&2
    exit 1
}

check_qemu() {
    if ! command -v qemu-system-x86_64 >/dev/null 2>&1; then
        error_exit "qemu-system-x86_64 is not installed."
    fi
}

usage() {
    echo "Usage: $0 <disk.img> <ram>"
    echo
    echo "Example:"
    echo "  $0 disk.img 512M"
    echo "  $0 disk.img 1G"
    exit 1
}

validate_ram() {
    if ! [[ "$1" =~ ^[0-9]+[MG]$ ]]; then
        error_exit "RAM must be a number followed by M or G (examples: 512M, 1G)."
    fi
}

main() {
    check_qemu

    if [[ $# -ne 2 ]]; then
        usage
    fi

    DISK_IMG="$1"
    VMRAM="$2"

    if [[ ! -f "$DISK_IMG" ]]; then
        error_exit "Disk image '$DISK_IMG' not found."
    fi

    validate_ram "$VMRAM"

    exec qemu-system-x86_64 \
        -machine pc \
        -drive file="$DISK_IMG",format=raw \
        -m "$VMRAM" \
        -enable-kvm \
        -boot c \
        -serial stdio
}

main "$@"
