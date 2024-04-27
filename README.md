# Cache Mapping Algorithms

This repository contains C++ implementations of two cache mapping algorithms:
1. Direct Mapping
2. Fully Associative Mapping

## Direct Mapping
The `direct_map.cpp` file implements the direct mapping algorithm. It includes functions to convert decimal numbers to binary, perform cache mapping using direct mapping, and display cache contents.

To run the program:

    g++ direct_map.cpp -o direct_map
    ./direct_map


## Fully Associative Mapping
The `fully_associative.cpp` file implements the fully associative mapping algorithm. It includes functions for FIFO and LRU (Least Recently Used) replacement policies, cache mapping using fully associative mapping, and displaying cache contents.

To run the program:

    g++ fully_associative.cpp -o fully_associative
    ./fully_associative

### Usage
- When prompted, provide input for the size of main memory, cache memory, block size, number of addresses, replacement policy (FIFO or LRU), and instruction type (load/store).
- If the instruction type is store, provide input for write-back or write-through policy.

### Replacement Policies
- **FIFO (First In First Out)**: The cache replaces the oldest item in the cache when a new item needs to be added.
- **LRU (Least Recently Used)**: The cache replaces the least recently used item in the cache when a new item needs to be added.

## Contributions
Contributions to improve the functionality or performance of these algorithms are welcome. Feel free to open a pull request.
