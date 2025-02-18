#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "hw1.h"

int main(int argc, char **argv) {
    assert(argc == 4); //we want a size, board state, and 4 clue vectors
    int size = (int)strtoul(argv[1], NULL, 10);
	solve(argv[2], argv[3], size);	
    return 0;
}