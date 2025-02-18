#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include <string.h>

#include "hw1.h"

int top_key[MAX_LENGTH] = {0};
int bottom_key[MAX_LENGTH] = {0};
int left_key[MAX_LENGTH] = {0};
int right_key[MAX_LENGTH] = {0};

char board[MAX_LENGTH][MAX_LENGTH] = {0};
int constraint_list[MAX_LENGTH][MAX_LENGTH][MAX_LENGTH] = {0};

int length = 5;
int sequence_count = 0;
int valid_sequences[MAX_SEQUENCE_CAP][MAX_LENGTH] = {0};


void clear_array(int arr[], int size) {
	for(int i = 0; i < size; i++) {
		arr[i] = 0;
	}
}


void clear_constraints(int i, int j) {
	int val = constraint_list[i][j][0];
	clear_array(constraint_list[i][j], length);
	constraint_list[i][j][0] = val;
}
void print_constraint_lists() {
	for(int i=0; i < length; i++) {
		for(int j=0; j < length; j++) {
			for(int z = 0; z < length; z++) {
				printf("%d ", constraint_list[i][j][z]);
			}
			printf("  ");
		}
		printf("\n");
	}
}

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
	


	// initilize constraint list
	for(int i=0; i < size; i++) {
		for( int j=0; j < size; j++) {
			for(int z = 0; z < length; z++) {
				constraint_list[i][j][z] = z+1;
			}
		}
	}
	// initilize board
	int k = 0;
	for(int i=0; i < size; i++) {
		for( int j=0; j < size; j++) {
			if(initial_state[k] != '-') {
				constraint_list[i][j][0] = initial_state[k] - '0';
				clear_constraints(i, j);

			}
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

	// printf("DEBUGGING CVONTRAINT LIST\n\n");
	// print_constraint_lists();

	
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
	if(c == 0) {
		return length + 1;
	}
	int val = length - c + 2 + d;
	return val;
}

int min(int top, int bottom, int left, int right) {
	// printf("%d, %d, %d, %d\n", top, bottom, left, right);
	int min_1 = (top < bottom) ? top : bottom;
	int min_2 = (left < right) ? left : right;

	return (min_1 < min_2) ? min_1 : min_2;
}



void remove_constraints(int row, int col, int val) {
	// Removes value from all other cells in the given row
	for(int j = 0; j < length; j++) {
		
			for(int z = 0; z < length; z++) {
				if(constraint_list[row][j][z] == val) {
					for(int k = z; k < length - 1; k++) {
						constraint_list[row][j][k] = constraint_list[row][j][k + 1];
					}
					constraint_list[row][j][length - 1] = 0;
				}
			}
		
	}

	// Removes value from all other cells in the given column
	for(int i = 0; i < length; i++) {
		
			for(int z = 0; z < length; z++) {
				if(constraint_list[i][col][z] == val) {
					for(int k = z; k < length - 1; k++) {
						constraint_list[i][col][k] = constraint_list[i][col][k + 1];
					}
					constraint_list[i][col][length - 1] = 0;
				}
			}
		
	}
}



// Function to propagate constraints
void propogate_constraints() {
    
	for(int i=0; i < length; i++) {
		for(int j=0; j < length; j++) {
			if(constraint_list[i][j][0] != 0 && constraint_list[i][j][1] == 0 ) {
				// printf("DEBUGGGG: row: %d, col: %d\n\n", i, j);
				// printf("DEBUG please, i: %d, j: %d\n", i, j);
				int val = constraint_list[i][j][0];
				board[i][j] = val + '0';
		 
				remove_constraints(i, j, val);
				constraint_list[i][j][0] = val;
	

			  
						}
					}
				
}
}



void elimination() {
	int counter[MAX_LENGTH + 1] = {0};
	int row_index[MAX_LENGTH + 1] = {0};
	int col_index[MAX_LENGTH + 1] = {0};
	// Elimination on rows.
	for(int i = 0; i < length; i++) {
		clear_array(counter, MAX_LENGTH + 1);
		clear_array(row_index, MAX_LENGTH + 1);
		clear_array(col_index, MAX_LENGTH + 1);
		for(int j = 0; j < length; j++) {
			if(board[i][j] == '-') {
				for(int z = 0; z < length; z++) {
					if(constraint_list[i][j][z] != 0){
						int val = constraint_list[i][j][z];
						counter[val]++;
						row_index[val] = i;
						col_index[val] = j;
					}
				}
			}
		}

		for(int k = 1; k <= length; k++) {
			if(counter[k] == 1) {
				int row = row_index[k];
				int col = col_index[k];
				board[row][col] = k + '0';
				// printf("%d\n, row: %d, col: %d, piece: %c\n", k, row, col, board[row][col]);
			
				clear_constraints(row, col);
				constraint_list[row][col][0] = k;
				remove_constraints(row, col, k);
			}
		}
	}

	// Elimination on columns.
	for(int j = 0; j < length; j++) {
		clear_array(counter, MAX_LENGTH + 1);
		clear_array(row_index, MAX_LENGTH + 1);
		clear_array(col_index, MAX_LENGTH + 1);
		for(int i = 0; i < length; i++) {
			if(board[i][j] == '-') {
				for(int z = 0; z < length; z++) {
					if(constraint_list[i][j][z] != 0){
						int val = constraint_list[i][j][z];
						counter[val]++;
						row_index[val] = i;
						col_index[val] = j;
					}
				}
			}
		}

		for(int k = 1; k <= length; k++) {
			if(counter[k] == 1) {
				int row = row_index[k];
				int col = col_index[k];
				board[row][col] = k + '0';
				clear_constraints(row, col);
				constraint_list[row][col][0] = k;
				remove_constraints(row, col, k);
			}
		}
	}

}

bool is_valid_sequence(int *current, int key, bool is_reversed) { 
    if (key == 0) {
        return true;
    }
    
    int vis = 0;
    int current_height = 0;
    
    if(is_reversed) {
		for(int k = length -1 ; k >= 0; k--){
			if(current[k] > current_height) {
				vis++;
				current_height = current[k];
			}
		}
	}
	
    for (int k = 0; k < length; k++) {
        if (current[k] > current_height) {
            vis++;
            current_height = current[k];
        }
    }

    return key == vis;
}


void generate_col_sequences(int key_index) {
    int indices[MAX_LENGTH] = {0};  
    int valid_counts[MAX_LENGTH];   
    

    
    for (int i = 0; i < length; i++) {
        valid_counts[i] = 0;
        for (int j = 0; j < length; j++) {
            if (constraint_list[i][key_index][j] != 0) {
                valid_counts[i]++;
            }
        }
    }

    while (1) {
        
        bool seen[MAX_LENGTH + 1] = {false};  
        bool is_duplicate = false;

        for (int i = 0; i < length; i++) {
            int value = constraint_list[i][key_index][indices[i]];
            if (seen[value]) {  
                is_duplicate = true;
                break;
            }
            seen[value] = true;  
        }

        if (!is_duplicate) {  
            for (int i = 0; i < length; i++) {
                valid_sequences[sequence_count][i] = constraint_list[i][key_index][indices[i]];
            }
            sequence_count++;
        }

        
        int i = length - 1;
        while (i >= 0) {
            indices[i]++;
            if (indices[i] < valid_counts[i]) {
                break;  
            }
            indices[i] = 0; 
            i--;
        }
        if (i < 0) break;  
    }
}



void generate_row_sequences(int key_index) {
    int indices[MAX_LENGTH] = {0};  
    int valid_counts[MAX_LENGTH];   
   

    
    for (int i = 0; i < length; i++) {
        valid_counts[i] = 0;
        for (int j = 0; j < length; j++) {
            if (constraint_list[key_index][i][j] != 0) {
                valid_counts[i]++;
            }
        }
    }

    while (1) {
        
        bool seen[MAX_LENGTH + 1] = {false};  
        bool is_duplicate = false;

        for (int i = 0; i < length; i++) {
            int value = constraint_list[key_index][i][indices[i]];
            if (seen[value]) {  
                is_duplicate = true;
                break;
            }
            seen[value] = true; 
        }

        if (!is_duplicate) {  
            for (int i = 0; i < length; i++) {
                valid_sequences[sequence_count][i] = constraint_list[key_index][i][indices[i]];
            }
            sequence_count++;
        }

       
        int i = length - 1;
        while (i >= 0) {
            indices[i]++;
            if (indices[i] < valid_counts[i]) {
                break;  
            }
            indices[i] = 0;  
            i--;
        }
        if (i < 0) break;  
    }
}
void clear_invalid_sequences(int key, bool is_reversed) {
    int new_sequence_count = 0;  
    for (int i = 0; i < sequence_count; i++) {
        if (is_valid_sequence(valid_sequences[i], key, is_reversed)) {
            if (i != new_sequence_count) {
                for (int j = 0; j < length; j++) {
                    valid_sequences[new_sequence_count][j] = valid_sequences[i][j];
                }
            }
            new_sequence_count++;
        }
    }
    sequence_count = new_sequence_count;
}

void remove_from_constraint_list(int val, int row, int col) {
	for(int z = 0; z < length; z++) {
		if(constraint_list[row][col][z] == val) {
			if(z == length - 1) {
				constraint_list[row][col][z] = 0;
			}
			else {
				constraint_list[row][col][z] = constraint_list[row][col][z+1];
			}
		}
	}
}


void print_stored_sequences(bool is_bottom) {
    printf("Stored Sequences:\n");

    if (is_bottom) {  
        for (int i = sequence_count - 1; i >= 0; i--) {
            for (int j = 0; j < length; j++) { 
                printf("%d ", valid_sequences[i][j]);
            }
            printf("\n");
        }
    } else {  
        for (int i = 0; i < sequence_count; i++) {
            for (int j = 0; j < length; j++) {  
                printf("%d ", valid_sequences[i][j]);
            }
            printf("\n");
        }
    }
}

void clear_valid_sequences() {
	for(int i = 0; i < sequence_count; i++) {
		for(int j = 0; j < length; j++) {
			valid_sequences[i][j] = 0;
		}
	}
}
void clue_elimination() {
	
    // Top keys
    for (int k = 0; k < length; k++) {
		clear_valid_sequences();
		sequence_count = 0;
        generate_col_sequences(k);
		bool value_tracker[MAX_LENGTH][MAX_LENGTH + 1] = {false};
		for(int i = 0; i < sequence_count; i++) {
			for(int j = 0; j < length; j++) {
				value_tracker[j][valid_sequences[i][j]] = true;
			}
		}
		for(int n = 0; n < length; n++) {
			for(int m = 1; m < length + 1; m++) {
				if(value_tracker[n][m] == false){
					remove_from_constraint_list(m, n, k);
					propogate_constraints();
					elimination();
				}
			}
		}
    }

    // Bottom keys
    for (int k = 0; k < length; k++) {
		clear_valid_sequences();
		sequence_count = 0;
        generate_col_sequences(k);
		bool value_tracker[MAX_LENGTH][MAX_LENGTH + 1] = {false};
		for(int i = 0; i < sequence_count; i++) {
			for(int j = 0; j < length; j++) {
				value_tracker[j][valid_sequences[i][j]] = true;
			}
		}
		for(int n = 0; n < length; n++) {
			for(int m = 1; m < length + 1; m++) {
				if(value_tracker[n][m] == false){
					remove_from_constraint_list(m, length - 1 - n, k);
					propogate_constraints();
					elimination();
				}
			}
		}
    }

    // Left keys
    for (int k = 0; k < length; k++) {
		clear_valid_sequences();
		sequence_count = 0;
        generate_row_sequences(k);
		bool value_tracker[MAX_LENGTH][MAX_LENGTH + 1] = {false};
		for(int i = 0; i < sequence_count; i++) {
			for(int j = 0; j < length; j++) {
				value_tracker[j][valid_sequences[i][j]] = true;
			}
		}
		for(int n = 0; n < length; n++) {
			for(int m = 1; m < length + 1; m++) {
				if(value_tracker[n][m] == false){
					remove_from_constraint_list(m, n, k);
					propogate_constraints();
					elimination();
				}
			}
		}
    }

    // Right keys
    for (int k = 0; k < length; k++) {
		clear_valid_sequences();
		sequence_count = 0;
        generate_row_sequences(k);
		bool value_tracker[MAX_LENGTH][MAX_LENGTH + 1] = {false};
		for(int i = 0; i < sequence_count; i++) {
			for(int j = 0; j < length; j++) {
				value_tracker[j][valid_sequences[i][j]] = true;
			}
		}
		for(int n = 0; n < length; n++) {
			for(int m = 1; m < length + 1; m++) {
				if(value_tracker[n][m] == false){
					remove_from_constraint_list(m, length - 1 - n, k);
					propogate_constraints();
					elimination();
				}
			}
		}
    }
}


void solve(const char *initial_state, const char *keys, int size) {
	initialize_board(initial_state, keys, size);
	current_board();
	
	printf("START\n\n\n");
	print_constraint_lists();
	printf("\n\n\n");
	// Edge Clue Initialization
	int least_val;
	for(int i = 0; i < length; i++) {
		for(int j = 0; j < length; j++) {
			// if(i ==0 && j ==1){
			// 	printf("PLEASE!!!\n");
			// 	print_constraint_lists();
			// 	printf("\n\n\n");
			// }
			if(top_key[j] == length) {
				for(int y = 0; y < length; y++) {
					board[y][j] = y + 1 + '0';
					constraint_list[y][j][0] =  y + 1;
					// printf("Test 1\n");
					clear_constraints(y, j);
					// print_constraint_lists();
				}
				current_board();
			}
			
			if(top_key[j] == 1) {
				board[0][j] = length + '0';
				constraint_list[0][j][0] = length;
				
				printf("Test 2, row: %d, col: %d\n", i, j);
				clear_constraints(0, j);
				current_board();
				print_constraint_lists();
			}
			if(bottom_key[j] == length) {
				for(int y = length - 1; y >= 0; y--) {
					board[y][j] = length - y + '0';
					constraint_list[y][j][0] =  length - y;
					printf("Test 3\n");
					clear_constraints(y, j);
					print_constraint_lists();
				}
				current_board();
			}
			if(bottom_key[j] == 1) {
				board[length - 1][j] = length + '0';
				constraint_list[length - 1][j][0] =  length;
				printf("Test 4, row: %d, col: %d\n", i, j);
				// clear_constraints(length - 1, j);
				current_board();
				print_constraint_lists();
			}
			if(left_key[i] == length) {
				for(int y = 0; y < length; y++) {
					board[i][y] = y + 1 + '0';
					constraint_list[i][y][0] =  y + 1;
					printf("Test 5\n");
					clear_constraints(i, y);
					print_constraint_lists();
				}
				current_board();
			}
			if(left_key[i] == 1) {
				board[i][0] = length + '0';
				constraint_list[i][0][0] =  length;
				printf("Test 6\n");
				clear_constraints(i, 0);
				current_board();
				print_constraint_lists();
			}
			if(right_key[i] == length) {
				for(int y = length - 1; y >= 0; y--) {
					board[i][y] = length - y + '0';
					constraint_list[i][y][0] =  length - y;
					printf("Test 7\n");
					clear_constraints(i, y);
					print_constraint_lists();
				}
				current_board();
			}
			if(right_key[i] == 1) {
				board[i][length - 1] = length + '0';
				constraint_list[i][length - 1][0] =  length;
				printf("Test 8\n");
				clear_constraints(i, length - 1);
				current_board();
				print_constraint_lists();
			}
			
			if(board[i][j] == '-') {
				least_val = min(get_constraint(top_key[j], i), get_constraint(bottom_key[j], length - 1 - i), 
				get_constraint(left_key[i], j), get_constraint(right_key[i], length -1 - j));
				// printf("%d\n", least_val);
				if(i ==0 && j ==0) {
					printf("Least Val: %d\n\n\n\n\n\n", least_val);
				}
				if(least_val > length){
					continue;
				}
				clear_constraints(i, j);
				for(int k = 1, l = 0; k <= length; k++) {
					if(k < least_val) {
						constraint_list[i][j][l++] = k;
					}
				}
				if(i == 0 && j == 4){
					printf("START DEBUG!!!: \n\n\n\n\n");
					print_constraint_lists();
				}
				if(i == 0 && j == 2){
					printf("END DEBUG!!!: int val: %d\n\n\n\n\n", least_val);
					print_constraint_lists();
				}
			}
		}

	}	

		printf("FIRST BOARD\n\n");
		current_board();
		print_constraint_lists();

		propogate_constraints();
		printf("SECOND BOARD\n\n");
		current_board();
		print_constraint_lists();
		current_board();
		clue_elimination();
		propogate_constraints();
		elimination();
		clue_elimination();
		
		

		printf("\n\n");
		print_constraint_lists();
		current_board();
		
	
		// print_constraint_lists();
		// printf("board1\n");
		// current_board();
		
		// // Constraint Propagation
		// propogate_constraints();
		
		// // Process of Elimination
		
		// propogate_constraints();
	
		// print_constraint_lists();
		// printf("board2\n");
		// current_board();
		

		// clue_elimination();
		// print_constraint_lists();
		// printf("board3\n");
		// current_board();

		// propogate_constraints();
		// print_constraint_lists();
		// printf("BOARD\n");
		// current_board();

		// elimination();
		// printf("BOARD2\n");
		// current_board();
		// clue_elimination();

		// printf("board4\n");

		// elimination();
		// propogate_constraints();
		// current_board();
		// printf("board5\n");


		// for (int i = 0; i < MAX_SEQUENCE_CAP; i++) {
		// 	if (valid_sequences[i][0] == 0) break;  // Stop printing when sequences end
		// 	for (int j = 0; j < length; j++) {
		// 		printf("%d ", valid_sequences[i][j]);
		// 	}
		// 	printf("\n");
		// }
		// for(int j = 0; j < length; j++) {
		// 	generate_valid_sequences(valid_sequences, bottom_key[j]);
	   	// }
		// for(int i = 0; i < length; i++) {
		// 	generate_valid_sequences(valid_sequences, left_key[i]);
		// }
		// for(int i = 0; i < length; i++) {
		// 	generate_valid_sequences(valid_sequences, right_key[i]);
		// }
	
}
