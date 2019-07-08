#include<sudoku.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

/*
TODO: 
    * Make a playable game of sudoku.
*/

void internal_clearterminal(){
    printf("\e[1;1H\e[2J");
}

void internal_draw_board(int board[]){
    for (int i = 0; i < 9; i++){
            
        if (i == 0 || i == 3 || i == 6){
            for (int k = 0; k < 19; k++){
                printf("-");
                }

            printf("\n");
        } 
    
        for (int j = 0; j < 9; j++){
            if (j == 0 || j == 3 || j == 6){
                printf("|");   
            }else{
                printf(" ");       
            }
            printf("%d",board[i * 9 + j ]);
        }

        printf("|");
        printf("\n");
    }
    
    for (int k = 0; k < 19; k++){
        printf("-");
    }
    printf("\n");
}


void sudoku_draw_board(Sudoku *self){
    internal_draw_board((self->board));
}

int main(){
    internal_clearterminal();
    srand(time(NULL));
    Sudoku a;
    unsigned input_int = 0;

    while (input_int == 0){
        printf("Input 1 to play or 2 to solve a sudoku: ");
        scanf("%u", &input_int);
        if (input_int > 2 || input_int < 1){
            input_int = 0;
        }
    }
    internal_clearterminal();
    if (input_int == 2){
        for (int i = 0; i < 81; i++){
            input_int = 10;
            while (input_int > 9) {
                printf("Input the sudoku board. 0 is for empty spot. column=%d row=%d: ",(i%9) + 1, (i/9) + 1);
                scanf("%u", &input_int);
            }
            a.board[i] = input_int;
        }
        sudoku_solver(&a);
        sudoku_draw_board(&a);
        return 0;
    }

    input_int = 0;
    while (input_int == 0){
        printf("Input a difficult level between 10 and 50: ");
        scanf("%u", &input_int);
        if (input_int > 50 || input_int < 10){
            input_int = 0;
        }
    }
    sudoku_generate_board(&a, input_int);
    internal_clearterminal();
   
    while (sudoku_check_for_win(&a) != 1){
        int input_int = 0;
        int ok_input = 0;
        internal_clearterminal();

        while (ok_input == 0){
            sudoku_draw_board(&a);
            printf("Input column row number(ddd): ");
            scanf("%u", &input_int);

            if (input_int/1000 == 0){
                if (input_int/100 != 0){
                    if ((input_int%100)  > 9){
                        ok_input = 1;
                        if (sudoku_add_number_to_board(&a, input_int/100 -1, (input_int%100)/10 -1, (input_int%10)) == 0){
                            ok_input = 0;
                        }    
                    }
                } 
            } 

            if (ok_input == 0){
                printf("Not a valid placement \n");
            }
        }
        internal_clearterminal();
    }

    sudoku_draw_board(&a);
    printf("win \n");

    return 0;
}
