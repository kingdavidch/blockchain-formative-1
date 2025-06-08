#include "blockchain.h"
#include <openssl/sha.h>
#include <openssl/evp.h>

// Helper function to calculate SHA-256 hash
void calculate_sha256(const char* input, char* output) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, input, strlen(input));
    SHA256_Final(hash, &sha256);
    
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(output + (i * 2), "%02x", hash[i]);
    }
    output[64] = '\0';
}

Blockchain* create_blockchain(int difficulty) {
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
    memset(block->previous_hash, '0', 64);
    block->previous_hash[64] = '\0';
    block->next = NULL;

    calculate_block_hash(block);
    return block;
}

void add_transaction(Block* block, const char* sender, const char* receiver, double amount) {
    if (!block || block->transaction_count >= MAX_TRANSACTIONS) return;

    Transaction* tx = &block->transactions[block->transaction_count];
    strncpy(tx->sender, sender, 63);
    strncpy(tx->receiver, receiver, 63);
    tx->amount = amount;
    tx->timestamp = time(NULL);
    
    block->transaction_count++;
}

void calculate_block_hash(Block* block) {
    char input[MAX_DATA_SIZE];
    char transaction_data[MAX_TRANSACTION_SIZE];
    
    // Create input string with block data
    sprintf(input, "%u%ld", block->index, block->timestamp);
    
    // Add transactions to input
    for (int i = 0; i < block->transaction_count; i++) {
        Transaction* tx = &block->transactions[i];
        sprintf(transaction_data, "%s%s%f%ld", 
                tx->sender, tx->receiver, tx->amount, tx->timestamp);
        strcat(input, transaction_data);
    }
    
    // Add previous hash
    strcat(input, block->previous_hash);
    
    // Calculate hash
    calculate_sha256(input, block->hash);
}

void add_block(Blockchain* chain) {
    if (!chain || !chain->latest) return;

    Block* new_block = create_block();
    if (!new_block) return;

    new_block->index = chain->latest->index + 1;
    strcpy(new_block->previous_hash, chain->latest->hash);
    
    calculate_block_hash(new_block);
    
    chain->latest->next = new_block;
    chain->latest = new_block;
}

int validate_chain(Blockchain* chain) {
    if (!chain || !chain->genesis) return 0;

    Block* current = chain->genesis;
    char calculated_hash[65];

    while (current) {
        // Calculate hash of current block
        char input[MAX_DATA_SIZE];
        char transaction_data[MAX_TRANSACTION_SIZE];
        
        sprintf(input, "%u%ld", current->index, current->timestamp);
        
        for (int i = 0; i < current->transaction_count; i++) {
            Transaction* tx = &current->transactions[i];
            sprintf(transaction_data, "%s%s%f%ld", 
                    tx->sender, tx->receiver, tx->amount, tx->timestamp);
            strcat(input, transaction_data);
        }
        
        strcat(input, current->previous_hash);
        calculate_sha256(input, calculated_hash);

        // Compare calculated hash with stored hash
        if (strcmp(calculated_hash, current->hash) != 0) {
            return 0;
        }

        // If there's a next block, verify its previous hash matches current block's hash
        if (current->next && strcmp(current->hash, current->next->previous_hash) != 0) {
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
    printf("Previous Hash: %s\n", block->previous_hash);
    printf("Hash: %s\n", block->hash);
    printf("Transactions:\n");
    
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

void save_blockchain(Blockchain* chain, const char* filename) {
    if (!chain || !filename) return;

    FILE* file = fopen(filename, "wb");
    if (!file) return;

    // Write blockchain metadata
    fwrite(&chain->difficulty, sizeof(int), 1, file);

    // Write blocks
    Block* current = chain->genesis;
    while (current) {
        fwrite(&current->index, sizeof(uint32_t), 1, file);
        fwrite(&current->timestamp, sizeof(time_t), 1, file);
        fwrite(&current->transaction_count, sizeof(int), 1, file);
        fwrite(current->transactions, sizeof(Transaction), current->transaction_count, file);
        fwrite(current->previous_hash, sizeof(char), 65, file);
        fwrite(current->hash, sizeof(char), 65, file);
        current = current->next;
    }

    fclose(file);
}

Blockchain* load_blockchain(const char* filename) {
    if (!filename) return NULL;

    FILE* file = fopen(filename, "rb");
    if (!file) return NULL;

    // Read blockchain metadata
    int difficulty;
    fread(&difficulty, sizeof(int), 1, file);

    Blockchain* chain = create_blockchain(difficulty);
    if (!chain) {
        fclose(file);
        return NULL;
    }

    // Read blocks
    Block* current = chain->genesis;
    while (1) {
        fread(&current->index, sizeof(uint32_t), 1, file);
        fread(&current->timestamp, sizeof(time_t), 1, file);
        fread(&current->transaction_count, sizeof(int), 1, file);
        fread(current->transactions, sizeof(Transaction), current->transaction_count, file);
        fread(current->previous_hash, sizeof(char), 65, file);
        fread(current->hash, sizeof(char), 65, file);

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