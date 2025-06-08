# Enhanced Blockchain Implementation - Question 2

This project implements an enhanced blockchain in C with transaction support and persistence.

## Features

1. Block Structure
   - Index, timestamp, and hash fields
   - Support for multiple transactions per block
   - Previous block hash linking

2. Transaction Support
   - Sender and receiver information
   - Transaction amount
   - Timestamp for each transaction

3. Blockchain Operations
   - Block creation and addition
   - Chain validation
   - Transaction addition to blocks
   - Blockchain persistence (save/load)

## Requirements

- GCC compiler
- Make

## Building the Project

To build the project, run:

```bash
make
```

This will create the necessary directories and compile the program.

## Running the Program

After building, run the program with:

```bash
./bin/blockchain
```

## Cleaning Up

To clean up the build files, run:

```bash
make clean
```

## Implementation Details

### Block Structure

Each block contains:
- Index number
- Timestamp
- Array of transactions
- Previous block's hash
- Current block's hash

### Transaction Structure

Each transaction includes:
- Sender address
- Receiver address
- Amount
- Timestamp

### Example Transactions

The program demonstrates transactions between three participants:
1. King -> Jack: 10.50
2. Jack -> Kraed: 5.00
3. Kraed -> King: 7.50
4. Jack -> King: 3.00
5. King -> Kraed: 2.50

### Blockchain Operations

The implementation supports:
1. Creating new blocks
2. Adding transactions to blocks
3. Calculating block hashes
4. Validating the entire chain
5. Saving the blockchain to a file
6. Loading the blockchain from a file

## Testing

The program includes built-in tests that demonstrate:
1. Creating a blockchain
2. Adding transactions to blocks
3. Adding new blocks
4. Validating the chain
5. Saving and loading the blockchain

## File Format

The blockchain is saved in a binary format that includes:
- Blockchain difficulty
- Block metadata (index, timestamp)
- Transaction data
- Hash information 