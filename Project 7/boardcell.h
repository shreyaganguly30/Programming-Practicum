// boardcell.h
//
// BoardCell class starter code for course project
// University of Illinois at Chicago
// CS 211 - Programming Practicum
// Original Author: Professor Scott Reckinger
//

#ifndef _BOARDCELL_H
#define _BOARDCELL_H

using namespace std;

// First, the BoardCell abstract base class 

class BoardCell {
	
    public:  
		BoardCell() {} // default contstructor (do nothing)
        virtual ~BoardCell() {} // destructor (do nothing)
        
        virtual char display( ) = 0; // pure virtual function; this is an abstract base class

    	virtual void attemptMoveTo(size_t& newR, size_t& newC, size_t hRow, size_t hCol) {            
            // new row and column is set to myRow and myCol
            newR = getRow();
            newC = getCol();
        }
        
        virtual bool isHero( ) {return false;}
    	virtual bool isBaddie( ) {return false;}
    	virtual bool isSpace( ) {return false;}
        virtual bool isBarrier( ) {return false;}
        virtual bool isHole( ) {return false;}
    	virtual bool isExit( ) {return false;}
    	virtual bool isStatic( ) {return true;}

        virtual void setPower(int num) {}
        virtual int getPower() {return 1;}
        virtual void setNextMove(char inChar) {}
        
        void setMoved(bool m) {moved = m;}
        bool getMoved() {return moved;}
        void setRow(size_t r) {myRow = r;}
        size_t getRow() {return myRow;}
        void setCol(size_t c) {myCol = c;}
        size_t getCol() {return myCol;}
        void setPos(size_t r, size_t c) {
            setRow(r);
            setCol(c);
        }

    private:
        size_t myRow; // current row for this board cell in a 2D grid
        size_t myCol; // current column for this board cell in a 2D grid
        bool moved;   // true = this board cell already moved in the current round

}; // BoardCell (abstract base class)


// Then, all the derived classes

class Hero: public BoardCell {
	
    public:
    	Hero(size_t r, size_t c) {
            setRow(r);
            setCol(c);
        }
        
        virtual bool isHero( ) {return true;}
        virtual bool isStatic( ) {return false;}
        virtual char display( ) {return 'H';}
        
        virtual void setNextMove(char inChar ) {
            nextMove = inChar;
        }
    	
        // top left of the grid is (0, 0)
        // if you want to move down, you increase the y value number
        virtual void attemptMoveTo(size_t& newR, size_t& newC, size_t hRow, size_t hCol) {
            // Since top left is (0, 0)
            // up: row - 1
            // down: row + 1
            // left: column - 1
            // right: column + 1

            // q moves up and left
            if(nextMove == 'q'){
                newR = hRow - 1;
                newC = hCol - 1;
            }
            // w moves up
            else if(nextMove == 'w'){
                newR = hRow - 1;
                newC = hCol;
            }
            // e moves up and right
            else if(nextMove == 'e'){
                newR = hRow - 1;
                newC = hCol + 1;
            }
            // a moves left
            else if(nextMove == 'a'){
                newR = hRow;
                newC = hCol - 1;
            }
            // d moves right
            else if(nextMove == 'd'){
                newR = hRow;
                newC = hCol + 1;
            }
            // z moves down and left
            else if(nextMove == 'z'){
                newR = hRow + 1;
                newC = hCol - 1;
            }
            // x moves down
            else if(nextMove == 'x'){
                newR = hRow + 1;
                newC = hCol;
            }
            // c moves down and right
            else if(nextMove == 'c'){
                newR = hRow + 1;
                newC = hCol + 1;
            }
            // other, stay
            else{
                newR = hRow;
                newC = hCol;
            }
        }

    private:
        char nextMove;

}; // Hero


class Monster: public BoardCell {
    
    public:
		Monster(size_t r, size_t c) {
            setRow(r);
            setCol(c);
            power = 1;
        }
        
        virtual bool isBaddie( ) {return true;}
        virtual bool isStatic( ) {return false;}
        
        virtual char display( ) {
            if (power == 2) {
                return 'M'; // Super Monster
            } else {
                return 'm'; // Monster
            } 
        }

        virtual void setPower(int num) {
            power = num;
        }

        virtual int getPower() {
            return power;
        }
        
    	virtual void attemptMoveTo(size_t& newR, size_t& newC, size_t hRow, size_t hCol) {
            // newRow and newColumn will originally be set to myRow and myCol
            newR = getRow();
            newC = getCol();

            // if it's a regular monster (power is 1)
            if(this->power == 1){
                // if myRow is less than hero's row, move baddie down by 1
                if(newR < hRow){
                    newR++;
                }
                // if myRow is more than hero's row, move baddie up by 1
                else if(newR > hRow){
                    newR--;
                }

                // if myCol is less than hero's row, move baddie right by 1
                if(newC < hCol){
                    newC++;
                }
                // if myCol is more than hero's row, move baddie left by 1
                else if(newC > hCol){
                    newC--;
                }
            }
            
            // if it's a super monster (power is 2)
            if(this->power == 2){
                // if myRow is less than hero's row, move baddie down by 2
                if(newR < hRow){
                    newR += 2;
                }
                // if myRow is more than hero's row, move baddie up by 2
                else if(newR > hRow){
                    newR -= 2;
                }

                // if myCol is less than hero's row, move baddie right by 2
                if(newC < hCol){
                    newC += 2;
                }
                // if myCol is more than hero's row, move baddie left by 2
                else if(newC > hCol){
                    newC -= 2;
                }
            }
        }
    
    private:
        int power; // power = 1 for Monster, power = 2 for SuperMonster

}; // Monster

class Bat: public BoardCell {
    public:
		Bat(size_t r, size_t c) {
            setRow(r);
            setCol(c);
        }
        virtual bool isBaddie( ) {return true;}
        virtual bool isStatic( ) {return false;}
        virtual char display( ) {return '~';}
        
    	virtual void attemptMoveTo(size_t& newR, size_t& newC, size_t hRow, size_t hCol) {
            // bat will stay in the same row
            newR = getRow();
            // bat will go to hero's column
            newC = hCol;
        }

}; // Bat


class Abyss: public BoardCell {
	public:
    	Abyss(size_t r, size_t c) {
            setRow(r);
            setCol(c);
        }
        virtual char display( ) {return '#';}
        virtual bool isHole( ) {return true;}
}; // Abyss


class Wall: public BoardCell {
	public:
    	Wall(size_t r, size_t c) {
            setRow(r);
            setCol(c);
        }
        virtual char display( ) {return '+';}
    	virtual bool isBarrier( ) {return true;}
}; // Wall


class Nothing: public BoardCell {
	public:
    	Nothing(size_t r, size_t c) {
            setRow(r);
            setCol(c);
        }
        virtual char display( ) {return ' ';}
    	virtual bool isSpace( ) {return true;}
}; // Nothing


class EscapeLadder: public BoardCell {
	public:
    	EscapeLadder(size_t r, size_t c) {
            setRow(r);
            setCol(c);
        }
        virtual char display( ) {return '*';}
    	virtual bool isExit( ) {return true;}
}; // EscapeLadder


#endif //_BOARDCELL_H
