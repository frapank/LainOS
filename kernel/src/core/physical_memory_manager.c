#include "core/physical_memory_manager.h"

#include "core/memory.h"

#define PHMM_BLOCK_SIZE 4096
#define PHMM_FIND_ERROR 4294967294

typedef uint32_t physical_address;

/*
 * bit 0 = free
 * bit 1 = used
 */
static u32 *block_bit_map = 0;
static u32 blocks_max = 0;
static u32 used_blocks = 0;

static inline bool is_block_occupied(u32 bit) 
{
    return block_bit_map[bit/32] & ( 1 << (bit % 32));
}

static inline void set_block(u32 bit)
{
    // bit / 32 = block of the byte
    // bit % 32 = bit offset
    block_bit_map[bit/32] |= (1 << (bit % 32));
}

static inline void unset_block(u32 bit)
{
    block_bit_map[bit/32] &= ~(1 << (bit % 32));
}

// Return the index of the first block with the defined size
static u32 find_first_free_blocks(u32 block_num) 
{
    if(block_num == 0) return PHMM_FIND_ERROR;

    // Test 32 bit at time
    for(u32 i = 0; i < blocks_max / 32; i++) {
        if(block_bit_map[i] == 0xFFFFFFFF) continue;

        // Find the first free bit
        for(u32 j = 0; j < 32; j++) {
            u32 bit = 1 << j;
            
            // Skip if bit is setted
            if((block_bit_map[i] & bit)) continue;

            u32 start_bit = i*32 + bit;
            u32 free_blocks = 0;

            for(u32 count = 0; count < block_num; count++) {
                if(!is_block_occupied(start_bit+count)) free_blocks++;
                else break;
                
                // Check if enough blocks
                if(free_blocks == block_num)
                    return i*32 + j;
            }
        }
    }

    return PHMM_FIND_ERROR;
}

void phmm_mark_region_free(u32 init_address, u32 size)
{
    u32 block_address = init_address / PHMM_BLOCK_SIZE;
    u32 block_cover = size / PHMM_BLOCK_SIZE;

    while(block_cover > 0) {
        unset_block(block_address++);
        used_blocks--;
        block_cover--;
    }
}

void phmm_mark_region_used(u32 init_address, u32 size)
{
    u32 block_address = init_address / PHMM_BLOCK_SIZE;
    u32 block_cover = size / PHMM_BLOCK_SIZE;

    while(block_cover > 0) {
        set_block(block_address++);
        used_blocks++;
        block_cover--;
    }
}

u32* phmm_alloc_blocks(u32 num_blocks) 
{
    if((blocks_max - used_blocks) <= num_blocks) return NULL;

    u32 block_address = find_first_free_blocks(num_blocks);
    if(block_address == PHMM_FIND_ERROR) return NULL;

    for(u32 i = 0; i < num_blocks; i++)
        set_block(block_address + i);

    used_blocks += num_blocks;

    return (u32*)(block_address * PHMM_BLOCK_SIZE);
}

void phmm_free_blocks(u32* addr, u32 num_blocks) 
{
    u32 block_address = (u32)addr / PHMM_BLOCK_SIZE;
    
    for(u32 i = 0; i < num_blocks; i++)
        unset_block(block_address + i);

    used_blocks -= num_blocks;

}

// Return size of the bitmap
u32 phmm_init(u32 init_address, u32 size)
{
    block_bit_map = (u32*)init_address;
    blocks_max = size / PHMM_BLOCK_SIZE;
    used_blocks = 0;

    kmemset(block_bit_map, 0x00, blocks_max / 8);

    u32 bitmap_size_bytes = (blocks_max + 7) / 8;
    return bitmap_size_bytes;
}
