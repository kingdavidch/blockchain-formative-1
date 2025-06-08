#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <time.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sha256.h"

#define MAX_TRANSACTIONS 100
#define MAX_DATA_SIZE 1024
#define MAX_TRANSACTION_SIZE 256

// Transaction structure
typedef struct {
    char sender[64];
    char receiver[64];
    double amount;
    time_t timestamp;
} Transaction;

// Block structure
typedef struct Block {
    uint32_t index;
    time_t timestamp;
    Transaction transactions[MAX_TRANSACTIONS];
    int transaction_count;
    uint8_t previous_hash[SHA256_DIGEST_SIZE];
    uint8_t hash[SHA256_DIGEST_SIZE];
    struct Block* next;
} Block;

// Blockchain structure
typedef struct {
    Block* genesis;
    Block* latest;
    int difficulty;
} Blockchain;

// Function declarations
Blockchain* create_blockchain(int difficulty);
Block* create_block();
int add_transaction(Block* block, const char* sender, const char* receiver, double amount);
void add_block(Blockchain* chain);
void calculate_block_hash(Block* block);
int validate_chain(Blockchain* chain);
void print_block(Block* block);
void print_blockchain(Blockchain* chain);
int save_blockchain(Blockchain* chain, const char* filename);
Blockchain* load_blockchain(const char* filename);
void free_blockchain(Blockchain* chain);

#endif // BLOCKCHAIN_H 