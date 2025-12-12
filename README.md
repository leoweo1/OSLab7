# OS Lab 7: Parallel Matrix Operations with pthreads

# Name: Daniel Onwuka

## Overview
Multi-threaded matrix arithmetic using pthreads for:
- Matrix Addition
- Matrix Subtraction
- Matrix Multiplication

## Features
- Fixed 20x20 matrix size as per requirements
- 10 threads per operation
- Row-based work distribution
- Proper thread synchronization

## Usage
```bash
make
./matrix
Implementation Details
Threads: 10 threads per operation (sum, diff, product)

Matrix Size: Hardcoded to 20x20

Work Distribution:

Sum/Diff: Each thread processes 40 cells (2 rows)

Product: Each thread multiplies 2 rows

Synchronization: pthread_join for completion

Output
Initial matrices A and B (random values 1-10)

Results for all three operations

Properly formatted display
