#include "blockchain.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Blockchain* create_blockchain() {
    Blockchain* chain = (Blockchain*)malloc(sizeof(Blockchain));
    if (!chain) return NULL;

    // Create genesis block
    chain->genesis = create_block("Genesis Block");
    if (!chain->genesis) {
        free(chain);
        return NULL;
    }

    chain->latest = chain->genesis;
    return chain;
}

Block* create_block(const char* data) {
    Block* block = (Block*)malloc(sizeof(Block));
    if (!block) return NULL;

    block->index = 0;
    block->timestamp = time(NULL);
    strncpy(block->data, data, MAX_DATA_SIZE - 1);
    block->data[MAX_DATA_SIZE - 1] = '\0';
    memset(block->previous_hash, 0, SHA256_DIGEST_SIZE);
    block->next = NULL;

    calculate_block_hash(block);
    return block;
}

void calculate_block_hash(Block* block) {
    SHA256_CTX ctx;
    sha256_init(&ctx);
    
    // Hash the block's data
    sha256_update(&ctx, (uint8_t*)block->data, strlen(block->data));
    
    // Hash the previous block's hash
    sha256_update(&ctx, block->previous_hash, SHA256_DIGEST_SIZE);
    
    // Hash the timestamp
    sha256_update(&ctx, (uint8_t*)&block->timestamp, sizeof(block->timestamp));
    
    // Hash the index
    sha256_update(&ctx, (uint8_t*)&block->index, sizeof(block->index));
    
    sha256_final(&ctx, block->hash);
}

void add_block(Blockchain* chain, const char* data) {
    if (!chain || !chain->latest) return;

    Block* new_block = create_block(data);
    if (!new_block) return;

    new_block->index = chain->latest->index + 1;
    memcpy(new_block->previous_hash, chain->latest->hash, SHA256_DIGEST_SIZE);
    
    calculate_block_hash(new_block);
    
    chain->latest->next = new_block;
    chain->latest = new_block;
}

int validate_chain(Blockchain* chain) {
    if (!chain || !chain->genesis) return 0;

    Block* current = chain->genesis;
    uint8_t calculated_hash[SHA256_DIGEST_SIZE];

    while (current) {
        // Calculate hash of current block
        SHA256_CTX ctx;
        sha256_init(&ctx);
        sha256_update(&ctx, (uint8_t*)current->data, strlen(current->data));
        sha256_update(&ctx, current->previous_hash, SHA256_DIGEST_SIZE);
        sha256_update(&ctx, (uint8_t*)&current->timestamp, sizeof(current->timestamp));
        sha256_update(&ctx, (uint8_t*)&current->index, sizeof(current->index));
        sha256_final(&ctx, calculated_hash);

        // Compare calculated hash with stored hash
        if (memcmp(calculated_hash, current->hash, SHA256_DIGEST_SIZE) != 0) {
            return 0;
        }

        // If there's a next block, verify its previous hash matches current block's hash
        if (current->next && memcmp(current->hash, current->next->previous_hash, SHA256_DIGEST_SIZE) != 0) {
            return 0;
        }

        current = current->next;
    }

    return 1;
}

void print_block(Block* block) {
    if (!block) return;

    printf("Block #%u\n", block->index);
    printf("Timestamp: %ld\n", block->timestamp);
    printf("Data: %s\n", block->data);
    printf("Previous Hash: ");
    for (int i = 0; i < SHA256_DIGEST_SIZE; i++) {
        printf("%02x", block->previous_hash[i]);
    }
    printf("\nHash: ");
    for (int i = 0; i < SHA256_DIGEST_SIZE; i++) {
        printf("%02x", block->hash[i]);
    }
    printf("\n\n");
}

void print_blockchain(Blockchain* chain) {
    if (!chain || !chain->genesis) return;

    Block* current = chain->genesis;
    while (current) {
        print_block(current);
        current = current->next;
    }
}

void free_blockchain(Blockchain* chain) {
    if (!chain) return;

    Block* current = chain->genesis;
    while (current) {
        Block* next = current->next;
        free(current);
        current = next;
    }
    free(chain);
} 