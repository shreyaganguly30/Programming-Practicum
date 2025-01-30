// gameboard.h
//
// GameBoard class starter code for course project
// University of Illinois at Chicago
// CS 211 - Programming Practicum
// Original Author: Professor Scott Reckinger
//

/*-------------------------------------------
Program 7: Outlast the Baddies & Avoid the Abyss

Description: This project creates hero, monster, bat, abyss, wall, and nothing objects. These represent
board pieces. Each object has a polymorphic function that allows the object to move. The gameboard.h
file handles the movement and any collisions, such as with an obstance or any empty spot. In this game, both
the hero and the baddies move. The hero's goal is to get to the escape ladder, but the baddies move towards
the hero. If the hero successfully gets to the ladder, then the hero wins. If the monster gets to the hero,
then the hero loses. 

Course: CS 211, Fall 2024, UIC

Author: Shreya Ganguly
-------------------------------------------*/

#ifndef _GAMEBOARD_H
#define _GAMEBOARD_H

#include <cstdlib>
#include <iostream>
#include <string>
#include <ctime>
#include <stdexcept>

#include "boardcell.h"
#include "grid.h"

using namespace std;

class GameBoard {
	private: 
	    Grid<BoardCell*> board;
        size_t numRows;
        size_t numCols;
        size_t HeroRow; // Hero's position row
	    size_t HeroCol; // Hero's position column
        int numMonsters;
        int numSuperMonsters;
        int numAbysses;
        int numBats;
        bool wonGame; // false, unless the Hero reached the exit successfully

		
	public: 
		/* default constructor */
        GameBoard() {
            numMonsters = 4;
            numSuperMonsters = 2;
            numAbysses = 50;
            numBats = 2;
            wonGame = false;
            
            this -> numRows = 15;
            this -> numCols = 40;
            
            Grid<BoardCell*> boardnew(numRows, numCols);
            board = boardnew;
            
            blankBoard();
        }
        
        /* param constructor */
        GameBoard(size_t numRows, size_t numCols) {
            numMonsters = 4;
            numSuperMonsters = 2;
            numAbysses = 20;
            numBats = 3;
            wonGame = false;
            
            this -> numRows = numRows;
            this -> numCols = numCols;
            
            Grid<BoardCell*> boardnew(numRows, numCols);
            board = boardnew;
            
            blankBoard();
        }
        
        /* destructor */
        virtual ~GameBoard() {
            for (size_t row = 0; row < board.numrows(); row++) {
                for (size_t col = 0; col < board.numcols(row); col++) {
                    delete board(row, col);
                }
            }
        }

        void blankBoard() {
            for (size_t row = 0; row < board.numrows(); row++) {
                for (size_t col = 0; col < board.numcols(row); col++) {
                    board(row, col) = new Nothing(row,col);
                }
            }
        }

        char getCellDisplay(size_t r, size_t c) {
            return board(r,c)->display();
        }

        void setCell(BoardCell* myCell, size_t r, size_t c) {
            board(r,c) = myCell;
        }
    
        void freeCell(size_t r, size_t c) {
            delete board(r,c);
        }

        // fills board with by randomly placing...
        //  - Hero (H) in the first three columns
        //  - EscapeLadder (*) in last three columns
        //  - 3 vertical Walls (+), each 1/2 of board height, in middle segment
        //  - Abyss cells (#), quantity set by numAbysses, in middle segment
        //  - Baddies [Monsters (m), Super Monsters (M), & Bats (~)] in middle segment;
        //    number of Baddies set by numMonsters, numSuperMonsters, & numBats
        void setupBoard(int seed) {
            srand(seed);
            size_t r,c;

            r = rand() % numRows;
            c = rand() % 3;
            delete board(r,c);
            board(r,c) = new Hero(r,c);
            HeroRow = r;
            HeroCol = c;

            r = rand() % numRows;
            c = numCols - 1 - (rand() % 3);
            delete board(r,c);
            board(r,c) = new EscapeLadder(r,c);
            
            int sizeMid = numCols - 6;

            c = 3 + (rand() % sizeMid);
            for (r = 0; r < numRows/2; ++r) {
                delete board(r,c);
                board(r,c) = new Wall(r,c);
            }
            size_t topc = c;

            while (c == topc || c == topc-1 || c == topc+1) {
                c = 3 + (rand() % sizeMid);
            }
            for (r = numRows-1; r > numRows/2; --r) {
                delete board(r,c);
                board(r,c) = new Wall(r,c);           
            }
            size_t botc = c;

            while (c == topc || c == topc-1 || c == topc+1 || c == botc || c == botc-1 || c == botc+1) {
                c = 3 + (rand() % sizeMid);
            }
            for (r = numRows/4; r < 3*numRows/4; ++r) {
                delete board(r,c);
                board(r,c) = new Wall(r,c);
            }

            for (int i = 0; i < numMonsters; ++i) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                while (board(r,c)->display() != ' ') {
                    r = rand() % numRows;
                    c = 3 + (rand() % sizeMid);
                }
                delete board(r,c);
                board(r,c) = new Monster(r,c);  
                board(r,c)->setPower(1);        
            }

            for (int i = 0; i < numSuperMonsters; ++i) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                while (board(r,c)->display() != ' ') {
                    r = rand() % numRows;
                    c = 3 + (rand() % sizeMid);
                }
                delete board(r,c);
                board(r,c) = new Monster(r,c); 
                board(r,c)->setPower(2);               
            }

            for (int i = 0; i < numBats; ++i) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                while (board(r,c)->display() != ' ') {
                    r = rand() % numRows;
                    c = 3 + (rand() % sizeMid);
                }
                delete board(r,c);
                board(r,c) = new Bat(r,c); 
            }

            for (int i = 0; i < numAbysses; ++i) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                while (board(r,c)->display() != ' ') {
                    r = rand() % numRows;
                    c = 3 + (rand() % sizeMid);
                }
                delete board(r,c);
                board(r,c) = new Abyss(r,c);              
            }
        }

        // neatly displaying the game board 
		void display( ) {
            cout << '-';
            for (size_t col = 0; col < board.numcols(0); col++) {
                cout << '-';
            }
            cout << '-';
            cout << endl;
            for (size_t row = 0; row < board.numrows(); row++) {
                cout << '|';
                for (size_t col = 0; col < board.numcols(row); col++) {
                    cout << board(row,col)->display();
                }
                cout << '|';
                cout << endl;
            }
            cout << '-';
            for (size_t col = 0; col < board.numcols(0); col++) {
                cout << '-';
            }
            cout << '-';
            cout << endl;
            
        }
		
        bool getWonGame() {
            return wonGame;
        }
        
        // distributing total number of monsters so that 
        //  ~1/3 of num are Super Monsters (M), and
        //  ~2/3 of num are Regular Monsters (m)
        void setNumMonsters(int num) {
            numSuperMonsters = num/3;
            numMonsters = num - numSuperMonsters;
        }

        void setNumAbysses(int num) {
            numAbysses = num;
        }

        void setNumBats(int num) {
            numBats = num;
        }

        size_t getNumRows() {
            return numRows;
        }

        size_t getNumCols() {
            return numCols;
        }

        
        //---------------------------------------------------------------------------------
        // void getHeroPosition(size_t& row, size_t& col)
        //
        // getter for Hero's position, which are private data members
        //      int HeroRow;
	    //      int HeroCol;
        // note: row and col are passed-by-reference
        //---------------------------------------------------------------------------------
        void getHeroPosition(size_t& row, size_t& col) {
            row = this->HeroRow;
            col = this->HeroCol;
        }

        
        //---------------------------------------------------------------------------------
        // void setHeroPosition(size_t row, size_t col)
        //
        // setter for Hero's position, which are private data members
        //      int HeroRow;
	    //      int HeroCol;
        //---------------------------------------------------------------------------------
        void setHeroPosition(size_t row, size_t col) {
            this->HeroRow = row;
            this->HeroCol = col;
        }

        void findHero() {
            // intitally set hero position to (-1,-1)
            this->HeroRow = -1;
            this->HeroCol = -1;

            // go to each position and check if hero is there
             for (size_t row = 0; row < numRows; ++row) {
                for (size_t col = 0; col < numCols; ++col) {
                    // if the display function returns H for that board position, then the Hero is there
                    if (board(row, col)->display() == 'H') {
                        // reset the hero row and column
                        HeroRow = row;
                        HeroCol = col;
                        // stop the function bc you found the hero
                        return;
                    }
                }
            }
        }

       bool makeMoves(char HeroNextMove) {
            // reset moves for grid
            for(int row = 0; row < numRows; row++){
                for(int col = 0; col < numCols; col++){
                    board(row, col)->setMoved(false);
                }
            }
           
            // determine where hero proposes to move to
            size_t newHeroRow, newHeroCol;
            board(HeroRow, HeroCol)->setNextMove(HeroNextMove);
            board(HeroRow, HeroCol)->attemptMoveTo(newHeroRow, newHeroCol, HeroRow, HeroCol);

            // attempted move out-of-bounds: change row &/or col to stay on board
            try {
                // hero attempts to move out-of-bounds in rows
                if (newHeroRow < 0 || newHeroRow >= numRows) { 
                    throw runtime_error("Hero trying to move out-of-bounds with an invalid row");
                } 
            }catch (runtime_error& excpt) {
                cout << excpt.what() << endl;
                newHeroRow = HeroRow;
                cout << "Changing row for Hero position to stay in-bounds" << endl;
            }
          
            try {
                // hero attempts to move out-of-bounds in columns
                if(newHeroCol < 0 || newHeroCol >= numCols){
                    throw runtime_error("Hero trying to move out-of-bounds with an invalid column");
                }
            }catch (runtime_error& excpt) {
                cout << excpt.what() << endl;
                newHeroCol = HeroCol;
                cout << "Changing col for Hero position to stay in-bounds" << endl;
            }

            // attempted move into a barrier: change row &/or col to stay off barrier
            try {
                if(board(newHeroRow, newHeroCol)->isBarrier()){
                    throw runtime_error("Hero trying to move into a wall");
                }
            }catch (runtime_error& excpt) {
                cout << excpt.what() << endl;
                // recall: diagonal attempted moves that end on a Wall cell are resolved by first
                // ignoring the horizontal component of the attempted move and checking for a valid move,
                // only ignoring the vertical component of the attempted move if also necessary

                // if the hero made a diagonal move
                if(HeroNextMove == 'q' || HeroNextMove == 'e' || HeroNextMove == 'z' || HeroNextMove == 'c'){
                    // only change the column
                    newHeroCol = HeroCol;
                    // if still collision, change only the row
                    if (board(newHeroRow, newHeroCol)->isBarrier()) {
                        newHeroRow = HeroRow;
                    }
                }
                // if it's not diagonal, don't move
                else {
                    newHeroRow = HeroRow;
                    newHeroCol = HeroCol;
                }
            }

            // attempted move to the exit: remove hero from board, hero escaped!
            if(board(newHeroRow, newHeroCol)->isExit()){
                // remove hero from board
                freeCell(HeroRow, HeroCol);
                // put nothing in place of the hero
                board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
                // set won game to true
                this->wonGame = true;
                // update hero position
                findHero();
                return false;
            }

            // attempted move into a hole: remove hero from board, hero did not escape
            if(board(newHeroRow, newHeroCol)->isHole()){
                // remove hero from board
                freeCell(HeroRow, HeroCol);
                // put nothing in place of the hero
                board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
                // update hero position
                findHero();
                return false;
            }

            // attempted move to a baddie: remove hero from board, hero did not escape
            if(board(newHeroRow, newHeroCol)->isBaddie()){
                // remove hero from board
                freeCell(HeroRow, HeroCol);
                // put nothing in place of the hero
                board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
                // update hero position
                findHero();
                return false;
            }

            // attempted move to an empty space: actual move is the attempted move
            if(board(newHeroRow, newHeroCol)->isSpace()){
                // delete the old block
                freeCell(HeroRow, HeroCol);
                // put nothing in place of the where the hero was
                board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
                // remove the new block
                freeCell(newHeroRow, newHeroCol);
                // move hero to cell
                board(newHeroRow, newHeroCol) = new Hero(newHeroRow, newHeroCol);
                // reset hero position
                findHero();
            }

           bool monsterCatchesHero = false;


            for(int originalRow = 0; originalRow < numRows; originalRow++){
                for(int originalCol = 0; originalCol < numCols; originalCol++){
                    // if the cell is a Baddie and has not moved
                    if(board(originalRow, originalCol)->isBaddie() && !(board(originalRow, originalCol)->getMoved())){
                        
                        size_t newBaddieRow, newBaddieCol;
                        // get the attempted move, which is newBaddieRow and newBaddieCol
                        board(originalRow, originalCol)->attemptMoveTo(newBaddieRow, newBaddieCol, HeroRow, HeroCol);

                        try {
                            // baddie attempts to move out-of-bounds in rows
                            if (newBaddieRow < 0 || newBaddieRow >= numRows) { 
                                throw runtime_error("Baddie trying to move out-of-bounds with an invalid row");
                            } 
                        }catch (runtime_error& excpt) {
                            cout << excpt.what() << endl;
                            newBaddieRow = originalRow;
                            cout << "Changing row for Baddie position to stay in-bounds" << endl;
                        }

                        try {
                            // baddie attempts to move out-of-bounds in columns
                            if (newBaddieCol < 0 || newBaddieCol >= numCols) { 
                                throw runtime_error("Baddie trying to move out-of-bounds with an invalid column");
                            } 
                        }catch (runtime_error& excpt) {
                            cout << excpt.what() << endl;
                            newBaddieCol = originalCol;
                            cout << "Changing column for Baddie position to stay in-bounds" << endl;
                        }

                        // attempted move into a barrier: change row &/or col to stay off barrier
                        try {
                            if(board(newBaddieRow, newBaddieCol)->isBarrier()){
                                throw runtime_error("Baddie trying to move into a wall");
                            }
                        }catch (runtime_error& excpt) {
                            if(newBaddieRow != originalRow && newBaddieCol != originalCol){
                                // keep the row the same
                                if(!board(originalRow, newBaddieCol)->isBarrier()){
                                    newBaddieRow = originalRow;
                                }
                                // or keep the column the same
                                else if(!board(newBaddieRow, originalCol)->isBarrier()){
                                    newBaddieCol = originalCol;
                                }
                                // or keep both the same
                                else{
                                    newBaddieRow = originalRow;
                                    newBaddieCol = originalCol;
                                }
                            }
                        }

                        if(board(newBaddieRow, newBaddieCol)->isHole()){
                            // remove baddie from board
                            freeCell(originalRow, originalCol);
                            // put nothing in place of the baddie
                            board(originalRow, originalCol) = new Nothing(originalRow, originalCol);
                        }

                        if(board(newBaddieRow, newBaddieCol)->isHero()){
                            // remove hero from board
                            freeCell(HeroRow, HeroCol);
                            // move the baddie
                            board(newBaddieRow, newBaddieCol) = board(originalRow, originalCol);
                            // reset baddie position
                            board(newBaddieRow, newBaddieCol)->setPos(newBaddieRow, newBaddieCol);

                            // delete baddie's old spot
                            board(originalRow, originalCol) = new Nothing(originalRow, originalCol);
                            monsterCatchesHero = true;
                        }

                        if(board(newBaddieRow, newBaddieCol)->isBaddie()){
                            // if the baddie collides with another baddie, go back to original position
                            newBaddieRow = originalRow;
                            newBaddieCol = originalCol;
                        }

                        if(board(newBaddieRow, newBaddieCol)->isSpace()){
                            // store the current baddie
                            BoardCell* oldBaddie = board(originalRow, originalCol);
                            // reset baddie position
                            board(originalRow, originalCol)->setPos(newBaddieRow, newBaddieCol);
                            // put nothing in place of the where the baddie was
                            board(originalRow, originalCol) = new Nothing(originalRow, originalCol);
                            // delete the new block
                            freeCell(newBaddieRow, newBaddieCol);
                            // move baddie to cell
                            board(newBaddieRow, newBaddieCol) = oldBaddie;
                        }

                        // set that the baddie moved
                        board(newBaddieRow, newBaddieCol)->setMoved(true);
                   }
               }
           }

           // check if the baddies moved into the hero
            if(monsterCatchesHero){
                return false;
            }else{
                return true;
            }
       }
};

#endif //_GAMEBOARD_H