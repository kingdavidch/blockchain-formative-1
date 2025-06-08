#include <stdio.h>
#include <string.h>
#include "blockchain.h"

void test_blockchain() {
    // Create a new blockchain with difficulty 4
    Blockchain* chain = create_blockchain(4);
    if (!chain) {
        printf("Failed to create blockchain\n");
        return;
    }

    // Add some transactions to the genesis block
    if (!add_transaction(chain->genesis, "King", "Jack", 10.5)) {
        printf("Failed to add transaction to genesis block\n");
        free_blockchain(chain);
        return;
    }
    if (!add_transaction(chain->genesis, "Jack", "Kraed", 5.0)) {
        printf("Failed to add transaction to genesis block\n");
        free_blockchain(chain);
        return;
    }
    calculate_block_hash(chain->genesis);

    // Add a new block with transactions
    add_block(chain);
    if (!add_transaction(chain->latest, "Kraed", "King", 7.5)) {
        printf("Failed to add transaction to block\n");
        free_blockchain(chain);
        return;
    }
    if (!add_transaction(chain->latest, "Jack", "King", 3.0)) {
        printf("Failed to add transaction to block\n");
        free_blockchain(chain);
        return;
    }
    calculate_block_hash(chain->latest);

    // Add another block
    add_block(chain);
    if (!add_transaction(chain->latest, "King", "Kraed", 2.5)) {
        printf("Failed to add transaction to block\n");
        free_blockchain(chain);
        return;
    }
    calculate_block_hash(chain->latest);

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

    // Save the blockchain to a file
    printf("\nSaving blockchain to file...\n");
    if (!save_blockchain(chain, "blockchain.dat")) {
        printf("Failed to save blockchain to file\n");
        free_blockchain(chain);
        return;
    }

    // Free the current blockchain
    free_blockchain(chain);

    // Load the blockchain from file
    printf("Loading blockchain from file...\n");
    chain = load_blockchain("blockchain.dat");
    if (!chain) {
        printf("Failed to load blockchain from file\n");
        return;
    }

    printf("\nLoaded Blockchain Contents:\n");
    print_blockchain(chain);
    free_blockchain(chain);
}

int main() {
    printf("Enhanced Blockchain Implementation\n");
    printf("================================\n\n");
    
    test_blockchain();
    
    return 0;
} 