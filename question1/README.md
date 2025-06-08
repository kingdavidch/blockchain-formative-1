# Blockchain Implementation - Question 1

This project implements a simple blockchain in C, including SHA-256 hashing and basic blockchain functionality.

## Features

1. SHA-256 Hashing Implementation
   - Custom implementation of the SHA-256 hashing algorithm
   - Test with the input "Blockchain Cryptography"

2. Simple Blockchain Simulation
   - Block structure with index, timestamp, data, and hashes
   - Chain validation
   - Block addition and verification

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

### SHA-256 Implementation

The SHA-256 implementation follows the standard specification and includes:
- Message padding
- Block processing
- Hash computation

### Blockchain Implementation

The blockchain implementation includes:
- Block structure with necessary fields
- Chain validation
- Block addition with proper linking
- Hash verification

## Testing

The program includes built-in tests for both the SHA-256 implementation and the blockchain functionality. The output will show:
1. The SHA-256 hash of "Blockchain Cryptography"
2. A demonstration of the blockchain with multiple blocks
3. Chain validation results 