#include <stdio.h>
#include <string.h>
#include "sha256.h"
#include "blockchain.h"

void print_hash(const uint8_t* hash) {
    for (int i = 0; i < SHA256_DIGEST_SIZE; i++) {
        printf("%02x", hash[i]);
    }
    printf("\n");
}

int main() {
    // Task 1: SHA-256 Hashing
    printf("Task 1: SHA-256 Hashing\n");
    printf("=======================\n");
    
    const char* input = "Blockchain Cryptography";
    uint8_t hash[SHA256_DIGEST_SIZE];
    
    printf("Input: %s\n", input);
    sha256((uint8_t*)input, strlen(input), hash);
    printf("SHA-256 Hash: ");
    print_hash(hash);
    printf("\n");

    // Task 2: Simple Blockchain Simulation
    printf("Task 2: Simple Blockchain Simulation\n");
    printf("===================================\n");
    
    // Create a new blockchain
    Blockchain* chain = create_blockchain();
    if (!chain) {
        printf("Failed to create blockchain\n");
        return 1;
    }

    // Add some blocks
    add_block(chain, "First Block Data");
    add_block(chain, "Second Block Data");
    add_block(chain, "Third Block Data");

    // Print the blockchain
    printf("Blockchain Contents:\n");
    print_blockchain(chain);

    // Validate the chain
    printf("Validating blockchain...\n");
    if (validate_chain(chain)) {
        printf("Blockchain is valid!\n");
    } else {
        printf("Blockchain is invalid!\n");
    }

    // Clean up
    free_blockchain(chain);
    return 0;
} 