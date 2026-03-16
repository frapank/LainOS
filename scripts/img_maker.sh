#!/usr/bin/env bash
set -euo pipefail

REQUIRED_TOOLS=(
    dd
    losetup
    fdisk
    mkfs.ext2
    lsblk
)

usage() {
    echo "Usage: $0 <disk.img> <size_mb>"
    exit 1
}

log() {
    echo "[*] $1"
}

check_sudo() {
    if [[ $EUID -ne 0 ]]; then
        echo "This script requires root privileges."
        exit 1
    fi
}

check_tools() {
    for tool in "${REQUIRED_TOOLS[@]}"; do
        if ! command -v "$tool" >/dev/null 2>&1; then
            echo "Error: required tool '$tool' not found"
            exit 1
        fi
    done
}

parse_args() {
    if [[ $# -ne 2 ]]; then
        usage
    fi

    IMAGE="$1"
    SIZE_MB="$2"
}

create_image() {
    log "Creating ${SIZE_MB}MB disk image..."
    dd if=/dev/zero of="$IMAGE" bs=1M count="$SIZE_MB" status=none
}

attach_loop() {
    log "Attaching loop device..."
    LOOPDEV=$(losetup -fP --show "$IMAGE")
    log "Loop device: $LOOPDEV"
}

partition_disk() {
    log "Partitioning disk..."
    fdisk "$LOOPDEV" >/dev/null 2>&1 <<EOF
o
n
p
1
2048
+1M
n
p
2


w
EOF
}

reload_loop() {
    log "Reloading loop device to detect partitions..."
    losetup -d "$LOOPDEV"
    LOOPDEV=$(losetup -fP --show "$IMAGE")
    log "Loop device reattached: $LOOPDEV"
}

format_partition() {
    PART2="${LOOPDEV}p2"
    log "Formatting $PART2 as ext2..."
    mkfs.ext2 -q "$PART2"
}

verify_layout() {
    log "Verifying partition layout..."
    lsblk "$LOOPDEV"
}

cleanup() {
    chmod 777 "$IMAGE"
    losetup -d "$LOOPDEV"

    echo ""
    echo "Disk image successfully created!"
    echo "Image file: $IMAGE"
    echo "Loop device: $LOOPDEV"
    echo
    echo "Partitions:"
    echo "  ${LOOPDEV}p1  -> reserved (stage2)"
    echo "  ${LOOPDEV}p2  -> ext2 filesystem"
    echo ""
}

main() {
    check_sudo
    parse_args "$@"
    check_tools
    create_image
    attach_loop
    partition_disk
    reload_loop
    format_partition
    verify_layout
    cleanup
}

main "$@"
