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
    add_transaction(chain->genesis, "Alice", "Bob", 10.5);
    add_transaction(chain->genesis, "Bob", "Charlie", 5.0);
    calculate_block_hash(chain->genesis);

    // Add a new block with transactions
    add_block(chain);
    add_transaction(chain->latest, "Charlie", "Alice", 7.5);
    add_transaction(chain->latest, "Bob", "Alice", 3.0);
    calculate_block_hash(chain->latest);

    // Add another block
    add_block(chain);
    add_transaction(chain->latest, "Alice", "Charlie", 2.5);
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
    save_blockchain(chain, "blockchain.dat");

    // Free the current blockchain
    free_blockchain(chain);

    // Load the blockchain from file
    printf("Loading blockchain from file...\n");
    chain = load_blockchain("blockchain.dat");
    if (chain) {
        printf("\nLoaded Blockchain Contents:\n");
        print_blockchain(chain);
        free_blockchain(chain);
    }
}

int main() {
    printf("Enhanced Blockchain Implementation\n");
    printf("================================\n\n");
    
    test_blockchain();
    
    return 0;
} 