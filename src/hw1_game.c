#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "hw1.h"

int main(int argc, char **argv) {
    assert(argc == 4); //we want a size, board state, and a list of keys
    int initial = initialize_board(argv[2], argv[3], (int)strtoul(argv[1], NULL, 10));
	if(initial == 0) {
        printf("%s", "Invalid initial board state.");
    }
     
    else if(initial == 1){
    
    extern int length;
    extern char board[MAX_LENGTH][MAX_LENGTH];
    char choice;
    int piece;
    int row;
    int column;
    bool quit = false;
    bool valid;
    while(winning_board() == false || quit == true) {
        valid = false;
        current_board();
        printf("Choose a piece (1-%d) or q to quit: ", length);
        while(valid == false) {
            scanf(" %c", &choice);
            if(choice == 'q') {
                valid = true;
                quit = true;
                break;
            }
            else if(choice >= '0' && choice <= '9') {
                piece = choice - '0';
                if(piece >= 1 && piece <= length) {
                    valid = true;
                    break;
                }
            }
            printf("Invalid choice. Choose a piece (1-%d) or q to quit: ", length);
        }

        if(quit == true) {
            break;
        }
        valid = false;
        printf("Choose a row (0-%d) : ", length - 1);
        while(valid == false) {
            if (scanf("%d", &row) == 1) {
                if(row >= 0 && row <= length - 1) {
                    valid = true;
                    break;
                }
            }
        printf("Invalid choice. Choose a row (0-%d) : ", length - 1);
        }

        valid = false;
        printf("Choose a column (0-%d) : ", length - 1);
        while(valid == false) {
            if (scanf("%d", &column) == 1) {
                if(column >= 0 && column <= length -1) {
                    valid = true;
                    break;
                }
            }
        printf("Invalid choice. Choose a column (0-%d) : ", length - 1);
        }

        if(board[row][column] != '-') {
            printf("Invalid choice. That space is already occupied.\n");
            continue;
        }

        if(duplicate_piece(piece + '0', row, column)) {
            printf("Invalid choice. There is already a building with that height in that row or column.\n");
            continue;
        }

        if(key_violation(piece + '0', row, column)) {
            printf("Invalid choice. You violate one of the key requirements.\n");
            continue;
        }
            board[row][column] = '0' + piece;
        }

     if (winning_board() == 1 ) {
        printf("Congratulations, you have filled the board!\n");
        current_board();
    }

    }  
    return 0;
}