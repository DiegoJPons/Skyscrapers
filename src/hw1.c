#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

#include "hw1.h"

int top_key[MAX_LENGTH] = {0};
int bottom_key[MAX_LENGTH] = {0};
int left_key[MAX_LENGTH] = {0};
int right_key[MAX_LENGTH] = {0};

char board[MAX_LENGTH][MAX_LENGTH] = {0};
int constraint_list[MAX_LENGTH][MAX_LENGTH][MAX_LENGTH] = {0};

int length = 5;


int initialize_board(const char *initial_state, const char *keys, int size) {

	length = size;

	for(int i=0; i < size*4; i++) { 
		if(i < size) {
			top_key[i] = keys[i] - '0';
		}
		else if (i < size*2 ) {
			bottom_key[i - size] = keys[i] - '0';
				}
		else if (i < size*3 ) {
			left_key[i - size*2] = keys[i] - '0';
				}
		else {
			right_key[i - size*3] = keys[i] -'0';
			}	
	} 
	
	int k = 0;
	for(int i=0; i < size; i++) {
		for( int j=0; j < size; j++) {
			board[i][j] = initial_state[k++];
		}
	}

	k = 0;
	for(int i=0; i < size; i++) {
		for( int j=0; j < size; j++) {
			if(duplicate_piece(initial_state[k],i, j) || key_violation(initial_state[k], i, j)) {
				return 0;
			}
			k++;
		}
	}
	

	
	return 1;
}



void current_board () {
        printf("    ");
        for (int i = 0; i < length; i++) {
            printf("%d ", top_key[i]);
        }
        printf("\n    ");
		for(int i=0; i < length; i++) {
			printf("v ");
		}
		printf("\n");

        for (int i = 0; i < length; i++) {
            printf("%d > ", left_key[i]);
            for(int j = 0; j < length; j++){ 
                printf("%c ", board[i][j]);                
            }
            printf("< %d \n", right_key[i]);
        }
        printf("    ");
		for(int i=0; i < length; i++) {
			printf("^ ");
		}
		printf("\n");
        printf("    ");
        for (int i = 0; i < length; i++) {
            printf("%d ", bottom_key[i]);
        }
        printf("\n");
    }

    bool winning_board() {
        for(int i = 0; i < length; i++) {
            for(int j = 0; j < length; j++) {
                if(board[i][j] == '-') {
                    return false;
                }
            }
        }
        return true;
    }

	bool duplicate_piece(char piece, int row, int col) {
		if(piece == '-') {
			return false;
		}
		for(int i=0; i < length; i++) {
			if (board[i][col] == piece && i != row) {
				return true;
			}
		}
		for (int j=0; j < length; j++) {
			if (board[row][j] == piece && j != col) {
				return true;
			}
		}
		return false;

	}

	bool last_piece_by_row(int row, int col) {
		int missing_pieces = 0;
		for(int i = 0; i < length; i++) {
			if(i != row) {
				if(board[i][col] == '-') {
					missing_pieces += 1;
				}
			}
		}

		if(missing_pieces == 0) {
			return true;
		}
		return false;
	}

	bool last_piece_by_col(int row, int col) {
		int missing_pieces = 0;
		for(int j=0; j < length; j++) {
			if(j != col) {
				if(board[row][j] == '-') {
					missing_pieces += 1;
				}
			}
		}

		if(missing_pieces == 0) {
			return true;
		}

		return false;
	}

	bool key_violation(char piece, int row, int col) {
		if(piece == '-') {
			return false;
		}
		int vis_from_top = 1;
		int vis_from_bottom = 1;
		int vis_from_left = 1;
		int vis_from_right = 1;
		int current_height;


		if(last_piece_by_row(row, col)) {
			// Checks key violation for top key.
			if(0 == row) {
				current_height = piece;
			}
			else {
				current_height = board[0][col];
			}
			for (int i = 1; i < length; i++) {
				if(i == row) {
					if(current_height < piece) {
						vis_from_top += 1;
						current_height = piece;
					}
				}
				else if (current_height < board[i][col]) {
					vis_from_top += 1;
					current_height = board[i][col];
				}
			
			}
				if(top_key[col] != 0 && vis_from_top != top_key[col]) {
					return true;
				}
			
			
			// Checks key violation for bottom key;
			if(length - 1 == row) {
				current_height = piece - '0';
			}
			else { 
				current_height = board[length-1][col] - '0';
			}
			for (int i = length-2; i >= 0; i--) {
				if(i == row) {
					if(current_height < piece - '0') {
						vis_from_bottom += 1;
						current_height = piece - '0';
					}
				}
				else if (current_height < board[i][col] - '0') {
					vis_from_bottom += 1;
					current_height = board[i][col] - '0';
				}
				
			}
				if(bottom_key[col] != 0 && vis_from_bottom != bottom_key[col]) {
					return true;
				}
			
		}

		if(last_piece_by_col(row, col)) { 

			// Checks key violation for left key;
			if(0 == col) {
				current_height = piece ;
			}
			else {
				current_height = board[row][0];
			}
			for (int j = 1; j < length; j++) {
				if(j == col) {
					if(current_height < piece) {
						vis_from_left += 1;
						current_height = piece;
					}
				}
				else if (current_height < board[row][j]) {
					vis_from_left += 1;
					current_height = board[row][j];
				}
				
			}

				if(left_key[row] != 0 && vis_from_left != left_key[row]) {
					return true;
				}

			// Checks key violation for right key;
			if(length - 1 == col) {
				current_height = piece;
			}
			else {
				current_height = board[row][length-1];
			}
			for (int j = length - 2; j >=0; j--) {
				if(j == col) {
					if(current_height < piece) {
						vis_from_right += 1;
						current_height = piece;
					}
				}
				else if (current_height < board[row][j]) {
					vis_from_right += 1;
					current_height = board[row][j];
				}
				
			}

				if(right_key[row] != 0 && vis_from_right != right_key[row]) {
					return true;
				}
		}

		return false;
	}

int get_constraint(int c, int d) {
	// printf("key: %d, distance: %d\n", c, d );
	int val = length - c + 2 + d;
	if(val > length) {
		return 5;
	}
	return val;
}

int min(int top, int bottom, int left, int right) {
	// printf("%d, %d, %d, %d\n", top, bottom, left, right);
	int min_1 = (top < bottom) ? top : bottom;
	int min_2 = (left < right) ? left : right;

	return (min_1 < min_2) ? min_1 : min_2;
}

void propogate_constraints() {
	for(int i=0; i < length; i++) {
		for(int j=0; j < length; j++) {
			if(constraint_list[i][j][0] != 0 && constraint_list[i][j][1] == 0 ) {
				int row = i;
				int col = j;
				int val = constraint_list[i][j][0];
				board[row][col] = val + '0';
				// Removes value from all other cells in the given row
				for(int k = 0; k < length; k++) {
					for(int z = 0; z < length; z++) {
						if(constraint_list[row][k][z] == val) {
							constraint_list[row][k][z] = 0;
						}
					}
				}

				// Removes value from all other cells in the given column
				for(int m = 0; m < length; m++) {
					for(int n = 0; n < length; n++) {
						if(constraint_list[m][col][n] == val) {
							constraint_list[m][col][n] = 0;
						}
					}
				}
						}
					}
				}


}

void clear_array(int arr[], int size) {
	for(int i = 0; i < size; i++) {
		arr[i] = 0;
	}
}

void clear_constraints(int i, int j) {
	for(int z = 0; z < length; z++) {
		constraint_list[i][j][z] = 0;
	}
}
void elimination() {
	int count[MAX_LENGTH + 1] = {0};
	int row_index[MAX_LENGTH + 1] = {0};
	int col_index[MAX_LENGTH + 1] = {0};
	// Elimination on rows.
	for(int i = 0; i < length; i++) {
		clear_array(count, MAX_LENGTH + 1);
		for(int j = 0; j < length; j++) {
			if(board[i][j] == '-') {
				for(int z = 0; z < length; z++) {
					if(constraint_list[i][j][z] != 0){
						int val = constraint_list[i][j][z];
						count[val]++;
						row_index[val] = i;
						col_index[val] = j;
					}
				}
			}
		}

		for(int k = 1; k <= length; k++) {
			if(count[k] == 1) {
				int row = row_index[k];
				int col = col_index[k];
				board[row][col] = k + '0';
				clear_constraints(row, col);
				constraint_list[row][col][0] = k;
			}
		}
	}

	// Elimination on columns.
	for(int j = 0; j < length; j++) {
		clear_array(count, MAX_LENGTH + 1);
		for(int i = 0; i < length; i++) {
			if(board[i][j] == '-') {
				for(int z = 0; z < length; z++) {
					if(constraint_list[i][j][z] != 0){
						int val = constraint_list[i][j][z];
						count[val]++;
						row_index[val] = i;
						col_index[val] = j;
					}
				}
			}
		}

		for(int k = 1; k <= length; k++) {
			if(count[k] == 1) {
				int row = row_index[k];
				int col = col_index[k];
				board[row][col] = k + '0';
				clear_constraints(row, col);
				constraint_list[row][col][0] = k;
			}
		}
	}

}
void solve(const char *initial_state, const char *keys, int size) {
	initialize_board(initial_state, keys, size);
	

	// Edge Clue Initialization
	int least_val;
	for(int i = 0; i < length; i++) {
		for(int j = 0; j < length; j++) {
			if(top_key[j] == length) {
				for(int y = 0; y < length; y++) {
					if(board[y][j] == '-') {
						board[y][j] = y + 1 + '0';
					}
				}
			}
			if(top_key[j] == 1 && board[0][j] == '-') {
				board[0][j] = length + '0';
			}
			if(bottom_key[j] == length) {
				for(int y = length - 1; y >= 0; y--) {
					if(board[y][j] == '-') {
						board[y][j] = length - y + '0';
					}
				}
			}
			if(bottom_key[j] == 1 && board[length - 1][j] == '-') {
				board[length - 1][j] = length + '0';
			}
			if(left_key[i] == length) {
				for(int y = 0; y < length; y++) {
					if(board[i][y] == '-') {
						board[i][y] = y + 1 + '0';
					}
				}
			}
			if(left_key[i] == 1 && board[0][j] == '-') {
				board[0][j] = length + '0';
			}
			if(right_key[i] == length) {
				for(int y = length - 1; y >= 0; y--) {
					if(board[i][y] == '-') {
						board[i][y] = length - y + '0';
					}
				}
			}
			if(right_key[i] == 1 && board[0][j] == '-') {
				board[i][length - 1] = length + '0';
			}
			
			if(board[i][j] == '-') {
				least_val = min(get_constraint(top_key[j], i), get_constraint(bottom_key[j], 3 - i), 
				get_constraint(left_key[i], j), get_constraint(right_key[i], 3 - j));
				// printf("%d\n", least_val);
				for(int k = 1, l = 0; k <= length; k++) {
					if(k < least_val) {
						constraint_list[i][j][l++] = k;
					}
				}
			}
		}

	}
	
		// for(int i=0; i < length; i++) {
		// 	for(int j=0; j < length; j++) {
		// 		for(int z = 0; z < length; z++) {
		// 			printf("%d ", constraint_list[i][j][z]);
		// 		}
		// 		printf("\n");
		// 	}
		// }

		// Constraint Propagation
		propogate_constraints();
		
		// Process of Elimination
		elimination();

		// Clue Elimination
	
}



