#ifndef SUDOKU_H
#define SUDOKU_H

typedef struct My_sudoku Sudoku;
struct My_sudoku
{
    int finished_board[81];
    int board[81];
};

void sudoku_generate_board(Sudoku *self, int diffeculty);
void sudoku_solver(Sudoku *self);

int sudoku_check_for_win(Sudoku *self);

int sudoku_add_number_to_board(Sudoku *self, int x_position, int y_position, int number);

#endif