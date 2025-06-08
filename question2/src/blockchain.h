#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <time.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    char previous_hash[64];
    char hash[64];
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
void add_transaction(Block* block, const char* sender, const char* receiver, double amount);
void add_block(Blockchain* chain);
void calculate_block_hash(Block* block);
int validate_chain(Blockchain* chain);
void print_block(Block* block);
void print_blockchain(Blockchain* chain);
void save_blockchain(Blockchain* chain, const char* filename);
Blockchain* load_blockchain(const char* filename);
void free_blockchain(Blockchain* chain);

#endif // BLOCKCHAIN_H 