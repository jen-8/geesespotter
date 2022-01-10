#include "geesespotter_lib.h"

// g++ -o main geesespotter.cpp geesespotter_lib.cpp -std=c++11



char *createBoard(std::size_t xdim, std::size_t ydim) {
    
    char *game_board = new char [xdim*ydim];                //pointer keeps track of the address of the array

    for (unsigned long i{0}; i < xdim*ydim; i++) {
        game_board[i] = 0x00;
    }

    return game_board;
}


void computeNeighbors(char *board, std::size_t xdim, std::size_t ydim) {

   for (int position{0}; position < (xdim*ydim); position++) {

       if (board[position] == 0x09) {
           int xloc = position % xdim;
           int yloc = position / xdim;
           int new_position{0};

           for (int y{-1}; y < 2; y++) { //iterates through the y values for the yshift
               for (int x{-1}; x < 2; x++ ) { //iterates through the x values for the xshift
                   
                   new_position = position + (xdim * y + x); //adds the shifts to the position with the goose

                   if (((xloc + x) >= 0) && ((xloc + x) < xdim)) { //ensures the new position is a valid x coordinate
                       if (((yloc + y) >= 0) && ((yloc + y) < ydim)) { //ensures the new position is a valid y coordinate
                           if (board[new_position] != 0x09) { //if the new position is already a 9 it shoudn't add a 1
                               board[new_position] += 1; //adds a 1 to each of the positions around the goose
                           }
                       }
                   }
                   
                }
            }
       }
       
   }
   
}

void hideBoard(char *board, std::size_t xdim, std::size_t ydim) {

    for (unsigned long i{0}; i < xdim*ydim; i++) {
        board[i] |= hiddenBit();
    }    

}

void cleanBoard(char *board) {
    delete[] board;
    board = nullptr;

}

void printBoard(char *board, std::size_t xdim, std::size_t ydim) {
    int count{0};

    for (unsigned long i{0}; i < xdim*ydim; i++) {
        char num = (board[i] & valueMask()) + 48; 
        
        if ((board[i] & markedBit()) > 0) {
            std::cout << "M";
        } else if ((board[i] & hiddenBit()) > 0) {
            std::cout << "*";
        } 

        if (((board[i] & hiddenBit()) == 0) && ((board[i] & markedBit()) == 0)) {

            std::cout << num;

            if (num == 0x09 ) {
                std::cout << "*";
            }
        }

        count++;

        if (count == xdim) {
            count = 0;
            std::cout << "\n";
        }
    }

}

int reveal (char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc,std::size_t yloc) {
    unsigned long index = (xdim*yloc) + xloc;

    if ((board[index] & markedBit()) > 0 ) {
        return 1;
    } else if ((board[index] & hiddenBit()) == 0) {
        return 2;
    } else if ((board[index] & valueMask())== 0x09) {
        board[index] &= ~hiddenBit(); 
        return 9;
    } else if ((board[index] & hiddenBit()) > 0){
        board[index] &= ~hiddenBit(); 

        if ((board[index] & valueMask()) == 0x00) {
           int new_position{0};

           for (int y{-1}; y < 2; y++) { //iterates through the y values for the yshift
               for (int x{-1}; x < 2; x++ ) { //iterates through the x values for the xshift
                   
                   new_position = index + (xdim * y + x); //adds the shifts to the position with the goose


                   if (((xloc + x) >= 0) && ((xloc + x) < xdim)) { //ensures the new position is a valid x coordinate
                       if (((yloc + y) >= 0) && ((yloc + y) < ydim)) { //ensures the new position is a valid y coordinate
                           if (board[new_position] != 0x09) { //if the new position is already a 9 it shoudn't add a 1
                               board[new_position] &= ~hiddenBit(); //removes hidden bit on each of the positions around the 0
                           }
                       }
                   }
                   
                }
           }
        }

    }

    return 0;
    
}

int mark(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc,std::size_t yloc) {
    unsigned long index = (xdim*yloc) + xloc;


    if ((board[index] & hiddenBit()) == 0) {
        return 2;
    } else {
        //if the element is not marked, it marks it
        if ((board[index] & markedBit()) == 0) {
            board[index] |= markedBit(); 
        } else if ((board[index] & markedBit()) > 0) { //if the element is marked, it unmarks it and checks if it is hidden. if it isn't, it hides it
            board[index] ^= markedBit();

            if ((board[index] & hiddenBit()) == 0) {
                board[index] |= hiddenBit();
                
            }
    }

        return 0;
    }
        

}

bool isGameWon(char *board, std::size_t xdim, std::size_t ydim) {

    for (int i{0}; i < (xdim*ydim); i++) {
        if (((board[i] & hiddenBit()) > 0) && ((board[i] & valueMask()) != 0x09)) {
            return false;
        }
    }

    return true;
}
