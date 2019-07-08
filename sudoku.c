#include<sudoku.h>
#include<stdlib.h>

#define true 1
#define false 0

/* 
TODO:
    * Add more methods of solving and removing (x-wing)
    * Extre: see if board can be sent as pointer insted of value. (VL)

*/

int internal_check_row(int board[], int y_position, int number){
    // Check if row has the same number
    for (int i = 0; i < 9; i++){
        if (board[y_position * 9 + i] == number) {
            return true;
        }
    }
    return false;
}
int internal_check_column(int board[], int x_position, int number){
    // Check if column has the same number
    for (int i = 0; i < 9; i++){
        if (board[i * 9 + x_position] == number) {
            return true;
        }
    }
    return false;
}
int internal_check_square(int board[], int x_position, int y_position, int number){
    // Check if square contins number
    int square_x_start_pos = x_position - x_position%3;
    int square_y_start_pos = y_position - y_position%3;
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++) {
            if (board[(square_y_start_pos + i)* 9 + square_x_start_pos + j] == number){
                return true;
            }
        }
    }
    return false;
}

/*  Parameters:
        * self is the sudoku structure.
        * x_position is the x cordinate of number.
        * y_position is the y cordinate of number.
        * number is a number that is checked if it is valid to place.
    Returns:
        * true(=1) if number dosen't exist in the row, column or square.
        * false(=0) if number exists in either ther row, column or square.
*/
int internal_check_if_placment_is_valid(int board[9], int x_position, int y_position, int number){
    
    if (internal_check_row(board, y_position, number) == true) {
        return false;
    }
    if (internal_check_column(board, x_position, number) == true) {
        return false;
    }
    if (internal_check_square(board, x_position, y_position, number) == true) {
        return false;
    }
    return true;
}

void internal_sudoku_board_square_sort(Sudoku *self, int square){
    int missing_number[] = {0,0,0,0,0,0,0,0,0};
    for (int i = 0; i < 9; i++){
        missing_number[self->finished_board[(square%3) * 3 + (i/3) * 9 + (i%3) + (square/3) * 27] - 1] += 1;
    }

    int end = 18 - (square%3) * 9;
    for (int i = 0; i < end; i++){
        int pos = ((square%3) + 1) * 3 + (square/3) * 27 + (i%3) + ((i%9)/3) * 9 + (i/9) * 3;
        if(missing_number[self->finished_board[pos] - 1] == 0){
            for (int j = 0; j < 3; j++){
                int swap_pos = (square%3) * 3 + (square/3) * 27 + ((i%9)/3) * 9 + j;
                if (missing_number[self->finished_board[swap_pos] - 1] > 1){
                    int save = self->finished_board[pos];
                    missing_number[save - 1] += 1;
                    self->finished_board[pos] = self->finished_board[swap_pos];
                    self->finished_board[swap_pos] = save;
                    missing_number[self->finished_board[pos]-1] -= 1;      
                }
            }
        }
    }

    int unique = true;
    for (int i = 0; i < 9; i++){
        if (missing_number[i] != 1){
            unique = false;
            i = 9;
        }
    }
    if (unique == false){
        internal_sudoku_board_square_sort(self, square);
    }
}

void internal_sudoku_board_column_sort(Sudoku *self, int column){
    int missing_number[] = {0,0,0,0,0,0,0,0,0};
    for (int i = 0; i < 9; i++){
        missing_number[self->finished_board[i * 9 + column] - 1] += 1;
    }

    int end = 18 - (column%3) * 9;
    for (int i = 0; i < end; i++){
        int pos = column + 1 + (i%9) * 9  + (i/9);
        if (missing_number[self->finished_board[pos] - 1] == 0){
            int save = self->finished_board[pos];
            missing_number[save - 1] += 1;
            self->finished_board[pos] = self->finished_board[column + (i%9) * 9];
            self->finished_board[column + (i%9) * 9] = save;
            missing_number[self->finished_board[pos]-1] -= 1;
        }
    }
    int unique = true;
    for (int i = 0; i < 9; i++){
        if (missing_number[i] != 1){
            unique = false;
            i = 9;
        }
    }
    if (unique == false){
        internal_sudoku_board_column_sort(self, column);
    }
}

void internal_sudoku_generate_finished_board(Sudoku *self){
    // clear board with 0;
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            self->finished_board[i * 9 +j] = 0;
            self->board[i * 9 + j] = 0;
        }
    }

    // insert rows with one ut of the 9 number.
    for (int k = 0; k < 9; k++){
        for (int i = 0; i < 9; i++){
            int numbers[] = {1,2,3,4,5,6,7,8,9};
            int end = 8;
            for (int j = 0; j < 9; j++){
                int random_number = rand() % (end+1);
                self->finished_board[i * 9 + j] = numbers[random_number];
                numbers[random_number] = numbers[end];
                end--;
            }
        }
    }

    // Sort all squars.
    for (int i = 0; i < 9; i++){
        internal_sudoku_board_square_sort(self, i);
    }


    // Sort all columns.
    for (int i = 0; i < 9; i++){
        internal_sudoku_board_column_sort(self, i);
    }
}

int internal_sudoku_remove_one_number_frome_square(Sudoku *self, int square){
    int pos = 0;
    
    // Checks for full square.
    int full_square = true;
    for (int i = 0; i < 9; i++){
        pos = (square%3) * 3 + (square/3) * 27 + (i%3) + (i/3) * 9;
        if (self->board[pos] == 0) {
            full_square = false;
            i = 9;
        }
    }   
    if (full_square == true){
        int random_pos = rand()%9;
        random_pos = (square%3) * 3 + (square/3) * 27 + (random_pos%3) + (random_pos/3) * 9;
        self->board[random_pos] = 0;
        return 1;
    } 

    // Checks for full row.
    for (int i = 0; i < 3; i++){
        int full_row  = true;
        for (int j = 0; j < 9; j++){
            pos = (square/3) * 27 + i * 9 + j;
            if (self->board[pos] == 0){
                full_row = false;
                j = 9;
            }
        }
        if (full_row == true){
            int random_pos = rand()%3;
            random_pos = (square%3) * 3 + (square/3) * 27 + i * 9 + random_pos;
            self->board[random_pos] = 0;
            return 1;
        }
    }

    // Checks for full columns
    for (int i = 0; i < 3; i++){
        int full_column  = true;
        for (int j = 0; j < 9; j++){
            pos = (square%3) * 3 + i + j * 9;
            if (self->board[pos] == 0){
                full_column = false;
                j = 9;
            }
        }
        if (full_column == true){
            int random_pos = rand()%3;
            random_pos = (square%3) * 3 + (square/3) * 27 + random_pos * 9 + i;
            self->board[random_pos] = 0;
            return 1;
        }
    }

    // other number that can be removed.
    int square_empty_pos[] = {0,0,0,0,0,0,0,0,0};
    int square_empty_pos_length = 0;
    int posible_to_remove_pos[] =  {0,0,0,0,0,0,0,0,0};
    int posible_to_remove_pos_length = 0;
    for (int i = 0; i < 9; i++){
        pos = (square%3) * 3 + (square/3) * 27 + (i%3) + (i/3) * 9;
        if (self->board[pos] == 0) {
            square_empty_pos[square_empty_pos_length] = pos;
            square_empty_pos_length += 1;
        }
    }

    for (int i = 0; i < 9; i++){
        
        pos = (square%3) * 3 + (square/3) * 27 + (i%3) + (i/3) * 9;
        int number_to_remove = self->board[pos];
        self->board[pos] = 0;
        if(number_to_remove != 0) {    
           int ok_to_remove = true;
                for (int j = 0; j < square_empty_pos_length; j++){
                    int not_posibole_to_be_placed = true;
                    for (int k = 0; k < 9; k++){
                        int pos_row = (square_empty_pos[j] / 9)  * 9 + k;
                        int pos_column = (square_empty_pos[j]%9) + k * 9;
                        if (self->board[pos_row] == number_to_remove){
                            not_posibole_to_be_placed = false;
                            k = 9;
                        } else if (self->board[pos_column] == number_to_remove ){
                            not_posibole_to_be_placed = false;
                            k = 9;
                        }
                    }
                    if (not_posibole_to_be_placed == true){
                        ok_to_remove = false;
                        j = square_empty_pos_length;
                    }
                }
            if (ok_to_remove == true){
                //printf("Possiblae: %d %d \n", pos%9, pos/9);
                posible_to_remove_pos[posible_to_remove_pos_length] = pos;
                posible_to_remove_pos_length +=1;
            }
            self->board[pos] = number_to_remove;
        }
    }
    if (posible_to_remove_pos_length != 0){
        int random_number_to_remove = rand() % posible_to_remove_pos_length;
        //printf("vald: %d %d \n", posible_to_remove_pos[random_number_to_remove] %9, posible_to_remove_pos[random_number_to_remove] / 9 );
        //sudoku_draw_board(self);
        self->board[posible_to_remove_pos[random_number_to_remove]] = 0;
        
        return 1;
    }

    return 0;
}

void internal_sudoku_generate_board(Sudoku *self, int diffeculty){
    int square_where_no_number_left_to_posible_remove[] = {1,1,1,1,1,1,1,1,1};
    int squares_left = 9;    
    int random_number_of_number_to_remove = diffeculty + rand()%6;
    for (int i = 0; i < random_number_of_number_to_remove; i++){
        int square_with_number_posible_to_remove = false;
        int random_square = rand()%9;
        while (square_with_number_posible_to_remove == false){
            random_square += 1;
            if (random_square > 8){
                random_square = 0;
            }
            if (square_where_no_number_left_to_posible_remove[random_square] == 1){
                square_with_number_posible_to_remove = true;
            }
        }
        if (internal_sudoku_remove_one_number_frome_square(self, random_square) == 0){
            square_where_no_number_left_to_posible_remove[random_square] = 0;
            squares_left -=1;
            if (squares_left <= 0){
                return;
            }
        }
    }
}

int internal_sudoku_fill_square(Sudoku *self, int empty_pos){
    int square = (empty_pos%9)/3 + ((empty_pos/9)/3)  * 3;
    int numbers_not_missing[] = {0,0,0,0,0,0,0,0,0};
    int numbers_not_missing_length = 0;
    for (int i = 0; i < 9; i++){
        int pos = (square%3) * 3 + (square/3) * 27 + (i%3) + (i/3) * 9;
        if (self->board[pos] != 0) {
            numbers_not_missing[self->board[pos] - 1] = 1;
            numbers_not_missing_length += 1;
        }
    }
    if (numbers_not_missing_length == 8){
        for (int i = 0; i < 9; i++){
            if (numbers_not_missing[i] == 0){
                self->board[empty_pos] = i + 1;
                return true;
            }
        }
    }
    return false;
}

int internal_sudoku_fill_row(Sudoku *self, int empty_pos){
    int row = empty_pos/9;
    int numbers_not_missing[] = {0,0,0,0,0,0,0,0,0};
    int numbers_not_missing_length = 0;
    for (int i = 0; i < 9; i++){
        int pos = row * 9 + i;
        if (self->board[pos] != 0) {
            numbers_not_missing[self->board[pos] - 1] = 1;
            numbers_not_missing_length += 1;
        }
    }
    if (numbers_not_missing_length == 8){
        for (int i = 0; i < 9; i++){
            if (numbers_not_missing[i] == 0){
                self->board[empty_pos] = i + 1;
                return true;
            }
        }
    }
    return false;
}

int internal_sudoku_fill_column(Sudoku *self, int empty_pos){
    int column = empty_pos%9;
    int numbers_not_missing[] = {0,0,0,0,0,0,0,0,0};
    int numbers_not_missing_length = 0;
    for (int i = 0; i < 9; i++){
        int pos = column + i * 9;
        if (self->board[pos] != 0) {
            numbers_not_missing[self->board[pos] - 1] = 1;
            numbers_not_missing_length += 1;
        }
    }
    if (numbers_not_missing_length == 8){
        for (int i = 0; i < 9; i++){
            if (numbers_not_missing[i] == 0){
                self->board[empty_pos] = i + 1;
                return true;
            }
        }
    }
    return false;
}

int internal_sudoku_fill_by_elimination(Sudoku *self, int empty_pos){
    int square = (empty_pos/9)/3 * 3 + (empty_pos%9)/3;
    int empty_positions[9];
    int empty_positions_length = 0;
    int numbers_not_missing[] = {0,0,0,0,0,0,0,0,0};
    int pos = 0;
    for (int i = 0; i < 9; i++){
        pos = (square%3) * 3 + (square/3) * 27 + (i/3) * 9 + i%3;
        if (self->board[pos] == 0){
            if (pos != empty_pos){
                empty_positions[empty_positions_length] = pos;
                empty_positions_length += 1;
            }
        } else {
            numbers_not_missing[self->board[pos] - 1] = 1;
        }
    }
    int numbers_not_missing_length = 0;
    for (int i = 0; i < 9; i++){
        if (numbers_not_missing[i] == 0){
            numbers_not_missing[numbers_not_missing_length] = i + 1;
            numbers_not_missing_length += 1;
        }
    }
    for (int i = 0; i < numbers_not_missing_length; i++){
        int missing_number = numbers_not_missing[i];
        int found_valid_number = true;
        for (int j = 0; j < empty_positions_length; j++){
            pos = empty_positions[j];
            if (internal_check_column(self->board, pos%9, missing_number) == false) {
                if (internal_check_row(self->board, pos/9, missing_number) == false){
                    found_valid_number = false;
                    j = empty_positions_length;
                }
            }
        }
        if (found_valid_number == true){
            self->board[empty_pos] = missing_number;
            return true;
        }
    }
    return false;
}



void sudoku_generate_board(Sudoku *self, int diffeculty){
    internal_sudoku_generate_finished_board(self);
    for (int i = 0; i < 81; i++){
        self->board[i] = self->finished_board[i];
    }
    internal_sudoku_generate_board(self, diffeculty);
}

void sudoku_solver(Sudoku *self){
    int empty_pos[81];
    int empty_pos_length = 0;
    for (int i = 0; i < 81; i++){
        if (self->board[i] == 0){
            empty_pos[empty_pos_length] = i;
            empty_pos_length += 1;
        }
    }

    int timeout = 0;
    int empty_pos_ptr = 0;
    while (empty_pos_length > 0){
        if (internal_sudoku_fill_square(self, empty_pos[empty_pos_ptr]) != true){
            if (internal_sudoku_fill_row(self, empty_pos[empty_pos_ptr]) != true) {
                if (internal_sudoku_fill_column(self, empty_pos[empty_pos_ptr]) != true){
                    if (internal_sudoku_fill_by_elimination(self, empty_pos[empty_pos_ptr]) != true){
                        // Noting happens becous it cant find the number that is supose to be in this spot.
                    }else {
                        empty_pos[empty_pos_ptr] = empty_pos[empty_pos_length - 1];
                        empty_pos_length -= 1;

                    }
                } else {
                    empty_pos[empty_pos_ptr] = empty_pos[empty_pos_length - 1];
                    empty_pos_length -= 1;
                }
            } else {
                empty_pos[empty_pos_ptr] = empty_pos[empty_pos_length - 1];
                empty_pos_length -= 1;
            }
            
        } else {
            empty_pos[empty_pos_ptr] = empty_pos[empty_pos_length - 1];
            empty_pos_length -= 1;
        }

        empty_pos_ptr += 1;
        if (empty_pos_length <= empty_pos_ptr) {
            empty_pos_ptr = 0;
            timeout += 1;
            if (timeout > 81){
                return;
            }
            
        }
    }
}

int sudoku_check_for_win(Sudoku *self){
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            int save = self->board[i * 9 + j];
            self->board[i * 9 + j] = 0;
            if (internal_check_if_placment_is_valid(self->board, j, i, save) == false) {
               self->board[i * 9 + j] = save; // might be cause a bug
               return false;
            }
            self->board[i * 9 + j] = save;
        }
    }
    return true;
}

int sudoku_add_number_to_board(Sudoku *self, int x_position, int y_position, int number){
    if (number == 0) {
        self->board[y_position * 9 + x_position] = number;
        return true;
    } else{
        int valid = internal_check_if_placment_is_valid(self->board, x_position, y_position, number);
        if (valid == true){
            self->board[y_position * 9 + x_position] = number;
            return true;
        }
    }
    return false;
}
