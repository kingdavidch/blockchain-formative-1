#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <time.h>
#include <stdint.h>
#include "sha256.h"

#define MAX_DATA_SIZE 1024

typedef struct Block {
    uint32_t index;
    time_t timestamp;
    char data[MAX_DATA_SIZE];
    uint8_t previous_hash[SHA256_DIGEST_SIZE];
    uint8_t hash[SHA256_DIGEST_SIZE];
    struct Block* next;
} Block;

typedef struct {
    Block* genesis;
    Block* latest;
} Blockchain;

// Function declarations
Blockchain* create_blockchain();
Block* create_block(const char* data);
void add_block(Blockchain* chain, const char* data);
void calculate_block_hash(Block* block);
int validate_chain(Blockchain* chain);
void print_block(Block* block);
void print_blockchain(Blockchain* chain);
void free_blockchain(Blockchain* chain);

#endif // BLOCKCHAIN_H 