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

	// checks for valid board initilization
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
	if(c == 0) {
		return length + 1;
	}
	int val = length - c + 2 + d;
	return val;
}

int min(int top, int bottom, int left, int right) {
	int min_1 = (top < bottom) ? top : bottom;
	int min_2 = (left < right) ? left : right;
	return (min_1 < min_2) ? min_1 : min_2;
}

void remove_constraints(int row, int col, int val) {
	// Removes value from rows
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

	// Removes value from cols
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

void propogate_constraints() {
	for(int i = 0; i < length; i++) {
		for(int j = 0; j < length; j++) {
			if(constraint_list[i][j][0] != 0 && constraint_list[i][j][1] == 0 ) {
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
				clear_constraints(row, col);
				constraint_list[row][col][0] = k;
				propogate_constraints();
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
				propogate_constraints();
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
    int index = -1;

    for (int z = 0; z < length; z++) {
        if (constraint_list[row][col][z] == val) {
            index = z;
            break;
        }
    }

    if (index != -1) {
        for (int z = index; z < length - 1; z++) {
            constraint_list[row][col][z] = constraint_list[row][col][z + 1];
        }
        constraint_list[row][col][length - 1] = 0;  
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
	if(winning_board()){
		return;
	}
    // Top keys
    for (int k = 0; k < length; k++) {
		clear_valid_sequences();
		sequence_count = 0;
        generate_col_sequences(k);
		clear_invalid_sequences(top_key[k], false);
		bool value_tracker[MAX_LENGTH][MAX_LENGTH + 1] = {false};
		for(int i = 0; i < sequence_count; i++) {
			for(int j = 0; j < length; j++) {
				value_tracker[j][valid_sequences[i][j]] = true;
			}
		}
		for(int n = 0; n < length; n++) {
			for(int m = 1; m < length + 1; m++) {
				if(value_tracker[n][m] == false && !winning_board()){
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
		clear_invalid_sequences(bottom_key[k], true);
		bool value_tracker[MAX_LENGTH][MAX_LENGTH + 1] = {false};
		for(int i = 0; i < sequence_count; i++) {
			for(int j = 0; j < length; j++) {
				value_tracker[j][valid_sequences[i][j]] = true;
			}
		}
		for(int n = 0; n < length; n++) {
			for(int m = 1; m < length + 1; m++) {
				if(value_tracker[n][m] == false && !winning_board()){
					remove_from_constraint_list(m, n, k);
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
		clear_invalid_sequences(left_key[k], false);
		bool value_tracker[MAX_LENGTH][MAX_LENGTH + 1] = {false};
		for(int i = 0; i < sequence_count; i++) {
			for(int j = 0; j < length; j++) {
				value_tracker[j][valid_sequences[i][j]] = true;
			}
		}
		for(int n = 0; n < length; n++) {
			for(int m = 1; m < length + 1; m++) {
				if(value_tracker[n][m] == false && !winning_board()){
					remove_from_constraint_list(m, k, n);
					propogate_constraints();
					elimination();
				}
			}
		}
    }

    // // Right keys
    for (int k = 0; k < length; k++) {
		clear_valid_sequences();
		sequence_count = 0;
        generate_row_sequences(k);
		clear_invalid_sequences(right_key[k], true);
		bool value_tracker[MAX_LENGTH][MAX_LENGTH + 1] = {false};
		for(int i = 0; i < sequence_count; i++) {
			for(int j = 0; j < length; j++) {
				value_tracker[j][valid_sequences[i][j]] = true;
			}
		}
		for(int n = 0; n < length; n++) {
			for(int m = 1; m < length + 1; m++) {
				if(value_tracker[n][m] == false && !winning_board()){
					remove_from_constraint_list(m, k, n);
					propogate_constraints();
					elimination();
				}
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
					board[y][j] = y + 1 + '0';
					constraint_list[y][j][0] =  y + 1;
					clear_constraints(y, j);
				}
			}
			
			if(top_key[j] == 1) {
				board[0][j] = length + '0';
				constraint_list[0][j][0] = length;
				clear_constraints(0, j);
			}
			if(bottom_key[j] == length) {
				for(int y = length - 1; y >= 0; y--) {
					board[y][j] = length - y + '0';
					constraint_list[y][j][0] =  length - y;
					clear_constraints(y, j);
				}
			}
			if(bottom_key[j] == 1) {
				board[length - 1][j] = length + '0';
				constraint_list[length - 1][j][0] =  length;
				clear_constraints(length - 1, j);
			}
			if(left_key[i] == length) {
				for(int y = 0; y < length; y++) {
					board[i][y] = y + 1 + '0';
					constraint_list[i][y][0] =  y + 1;
					clear_constraints(i, y);
				}
			}
			if(left_key[i] == 1) {
				board[i][0] = length + '0';
				constraint_list[i][0][0] =  length;
				clear_constraints(i, 0);
			}
			if(right_key[i] == length) {
				for(int y = length - 1; y >= 0; y--) {
					board[i][y] = length - y + '0';
					constraint_list[i][y][0] =  length - y;
					clear_constraints(i, y);
				}
			}
			if(right_key[i] == 1) {
				board[i][length - 1] = length + '0';
				constraint_list[i][length - 1][0] =  length;
				clear_constraints(i, length - 1);
			}
			
			if(board[i][j] == '-') {
				int top = get_constraint(top_key[j], i);
				int bottom = get_constraint(bottom_key[j], length - 1 - i);
				int left = get_constraint(left_key[i], j);
				int right = get_constraint(right_key[i], length -1 - j);

				least_val = min(top, bottom, left, right);
				if(least_val > length){
					continue;
				}
				clear_constraints(i, j);
				for(int k = 1, l = 0; k <= length; k++) {
					if(k < least_val) {
						constraint_list[i][j][l++] = k;
					}
				}
			}
		}

	}	
		propogate_constraints();
		elimination();
		clue_elimination();
}
