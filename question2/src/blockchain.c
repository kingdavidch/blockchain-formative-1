#include "blockchain.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Blockchain* create_blockchain(int difficulty) {
    if (difficulty < 0) return NULL;

    Blockchain* chain = (Blockchain*)malloc(sizeof(Blockchain));
    if (!chain) return NULL;

    chain->difficulty = difficulty;
    chain->genesis = create_block();
    if (!chain->genesis) {
        free(chain);
        return NULL;
    }

    chain->latest = chain->genesis;
    return chain;
}

Block* create_block() {
    Block* block = (Block*)malloc(sizeof(Block));
    if (!block) return NULL;

    block->index = 0;
    block->timestamp = time(NULL);
    block->transaction_count = 0;
    memset(block->previous_hash, 0, SHA256_DIGEST_SIZE);
    block->next = NULL;

    calculate_block_hash(block);
    return block;
}

int add_transaction(Block* block, const char* sender, const char* receiver, double amount) {
    if (!block || !sender || !receiver || amount <= 0) return 0;
    if (block->transaction_count >= MAX_TRANSACTIONS) return 0;

    Transaction* tx = &block->transactions[block->transaction_count];
    strncpy(tx->sender, sender, 63);
    tx->sender[63] = '\0';
    strncpy(tx->receiver, receiver, 63);
    tx->receiver[63] = '\0';
    tx->amount = amount;
    tx->timestamp = time(NULL);
    
    block->transaction_count++;
    return 1;
}

void calculate_block_hash(Block* block) {
    if (!block) return;

    SHA256_CTX ctx;
    sha256_init(&ctx);
    
    // Hash the block's index
    sha256_update(&ctx, (uint8_t*)&block->index, sizeof(block->index));
    
    // Hash the timestamp
    sha256_update(&ctx, (uint8_t*)&block->timestamp, sizeof(block->timestamp));
    
    // Hash the transactions
    for (int i = 0; i < block->transaction_count; i++) {
        Transaction* tx = &block->transactions[i];
        sha256_update(&ctx, (uint8_t*)tx->sender, strlen(tx->sender));
        sha256_update(&ctx, (uint8_t*)tx->receiver, strlen(tx->receiver));
        sha256_update(&ctx, (uint8_t*)&tx->amount, sizeof(tx->amount));
        sha256_update(&ctx, (uint8_t*)&tx->timestamp, sizeof(tx->timestamp));
    }
    
    // Hash the previous block's hash
    sha256_update(&ctx, block->previous_hash, SHA256_DIGEST_SIZE);
    
    sha256_final(&ctx, block->hash);
}

void add_block(Blockchain* chain) {
    if (!chain || !chain->latest) return;

    Block* new_block = create_block();
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
        
        sha256_update(&ctx, (uint8_t*)&current->index, sizeof(current->index));
        sha256_update(&ctx, (uint8_t*)&current->timestamp, sizeof(current->timestamp));
        
        for (int i = 0; i < current->transaction_count; i++) {
            Transaction* tx = &current->transactions[i];
            sha256_update(&ctx, (uint8_t*)tx->sender, strlen(tx->sender));
            sha256_update(&ctx, (uint8_t*)tx->receiver, strlen(tx->receiver));
            sha256_update(&ctx, (uint8_t*)&tx->amount, sizeof(tx->amount));
            sha256_update(&ctx, (uint8_t*)&tx->timestamp, sizeof(tx->timestamp));
        }
        
        sha256_update(&ctx, current->previous_hash, SHA256_DIGEST_SIZE);
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

    printf("\nBlock #%u\n", block->index);
    printf("Timestamp: %ld\n", block->timestamp);
    printf("Previous Hash: ");
    for (int i = 0; i < SHA256_DIGEST_SIZE; i++) {
        printf("%02x", block->previous_hash[i]);
    }
    printf("\nHash: ");
    for (int i = 0; i < SHA256_DIGEST_SIZE; i++) {
        printf("%02x", block->hash[i]);
    }
    printf("\nTransactions:\n");
    
    for (int i = 0; i < block->transaction_count; i++) {
        Transaction* tx = &block->transactions[i];
        printf("  %d. %s -> %s: %.2f\n", 
               i + 1, tx->sender, tx->receiver, tx->amount);
    }
    printf("\n");
}

void print_blockchain(Blockchain* chain) {
    if (!chain || !chain->genesis) return;

    Block* current = chain->genesis;
    while (current) {
        print_block(current);
        current = current->next;
    }
}

int save_blockchain(Blockchain* chain, const char* filename) {
    if (!chain || !filename) return 0;

    FILE* file = fopen(filename, "wb");
    if (!file) return 0;

    // Write blockchain metadata
    if (fwrite(&chain->difficulty, sizeof(int), 1, file) != 1) {
        fclose(file);
        return 0;
    }

    // Write blocks
    Block* current = chain->genesis;
    while (current) {
        if (fwrite(&current->index, sizeof(uint32_t), 1, file) != 1 ||
            fwrite(&current->timestamp, sizeof(time_t), 1, file) != 1 ||
            fwrite(&current->transaction_count, sizeof(int), 1, file) != 1 ||
            fwrite(current->transactions, sizeof(Transaction), current->transaction_count, file) != current->transaction_count ||
            fwrite(current->previous_hash, sizeof(uint8_t), SHA256_DIGEST_SIZE, file) != SHA256_DIGEST_SIZE ||
            fwrite(current->hash, sizeof(uint8_t), SHA256_DIGEST_SIZE, file) != SHA256_DIGEST_SIZE) {
            fclose(file);
            return 0;
        }
        current = current->next;
    }

    fclose(file);
    return 1;
}

Blockchain* load_blockchain(const char* filename) {
    if (!filename) return NULL;

    FILE* file = fopen(filename, "rb");
    if (!file) return NULL;

    // Read blockchain metadata
    int difficulty;
    if (fread(&difficulty, sizeof(int), 1, file) != 1) {
        fclose(file);
        return NULL;
    }

    Blockchain* chain = create_blockchain(difficulty);
    if (!chain) {
        fclose(file);
        return NULL;
    }

    // Read blocks
    Block* current = chain->genesis;
    while (!feof(file)) {
        size_t read_size;
        
        read_size = fread(&current->index, sizeof(uint32_t), 1, file);
        if (read_size != 1) break;
        
        read_size = fread(&current->timestamp, sizeof(time_t), 1, file);
        if (read_size != 1) break;
        
        read_size = fread(&current->transaction_count, sizeof(int), 1, file);
        if (read_size != 1) break;
        
        read_size = fread(current->transactions, sizeof(Transaction), current->transaction_count, file);
        if (read_size != current->transaction_count) break;
        
        read_size = fread(current->previous_hash, sizeof(uint8_t), SHA256_DIGEST_SIZE, file);
        if (read_size != SHA256_DIGEST_SIZE) break;
        
        read_size = fread(current->hash, sizeof(uint8_t), SHA256_DIGEST_SIZE, file);
        if (read_size != SHA256_DIGEST_SIZE) break;

        // Check if we've reached the end of the file
        if (feof(file)) break;

        // Create next block
        Block* next = create_block();
        if (!next) {
            free_blockchain(chain);
            fclose(file);
            return NULL;
        }

        current->next = next;
        current = next;
        chain->latest = current;
    }

    fclose(file);
    return chain;
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