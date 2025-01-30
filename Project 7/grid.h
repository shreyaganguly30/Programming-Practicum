#pragma once

#include <iostream>
#include <exception>
#include <stdexcept>
#include <algorithm>

using namespace std;

template<typename T>
class Grid {
private:
  struct ROW {
    T* Cols;     // an array of column elements
    size_t NumCols; // holds the total number of columns
  };

  ROW* Rows;     // an array of ROWs
  size_t  NumRows;  // holds the total number of rows
    
public:
    // Default Constructor
  Grid() {
    // Default to 4 rows
    Rows = new ROW[4];
    NumRows = 4;

    // for each row
    for (size_t r = 0; r < NumRows; ++r) {
      // initialize each row to have 4 columns
      Rows[r].Cols = new T[4];
      Rows[r].NumCols = 4;

      // initialize the elements to their default value of type T
      for (size_t c = 0; c < Rows[r].NumCols; ++c) {
        Rows[r].Cols[c] = T();
      }
    }
  }

  // Parameterized Constructor
  Grid(size_t R, size_t C) {
    // allocate space for each row
    this->Rows = new ROW[R]; 
    // set the number of rows in col as specified by the paramter
    this->NumRows = R;

    for (size_t rowIndex = 0; rowIndex < NumRows; rowIndex++) {
        // allocate space for each column
        Rows[rowIndex].Cols = new T[C];
        // set the number of cols as specified by the paramter
        Rows[rowIndex].NumCols = C;
            
        // set each col to default value of T
        for (size_t colIndex = 0; colIndex < Rows[rowIndex].NumCols; colIndex++) {
            Rows[rowIndex].Cols[colIndex] = T();  
        }
    }
  }
  
  // Destructor
  virtual ~Grid() {
    // delete each col
    for(size_t rowIndex = 0; rowIndex < this->NumRows; rowIndex++){
        delete[] Rows[rowIndex].Cols;
    }
    // delete the rows array
    delete[] Rows;
  }
  
  // Copy Constructor
  Grid(const Grid<T>& other) {
    // allocate space for same number rows as other
    this->Rows = new ROW[other.NumRows];
    this->NumRows = other.NumRows;
    for(size_t rowIndex = 0; rowIndex < NumRows; rowIndex++){
        // get other's number of columns
        size_t other_numcols = other.Rows[rowIndex].NumCols;
        // allocate space for other_numcols
        this->Rows[rowIndex].Cols = new T[other_numcols];
        this->Rows[rowIndex].NumCols = other_numcols;
        // for each index, copy the data over
        for (size_t colIndex = 0; colIndex < other_numcols; colIndex++) {
            this->Rows[rowIndex].Cols[colIndex] = other.Rows[rowIndex].Cols[colIndex];
        }
    }
  }
  
  // Operator =
  Grid& operator=(const Grid& other) {
    // if the paramter is the same as yours, return yours
    if (this == &other) {
        return *this;
    }
    
    // free any space already in your Rows array
    for (size_t rowIndex = 0; rowIndex < NumRows; rowIndex++) {
        delete[] this->Rows[rowIndex].Cols;
    }
    delete[] this->Rows;
    
    // copy the row info into your variable
    this->Rows = new ROW[other.NumRows];
    this->NumRows = other.NumRows;
    // for each row, set the column info
    for(size_t rowIndex = 0; rowIndex < other.NumRows; rowIndex++){
        // set the number of columns
        size_t other_numcols = other.Rows[rowIndex].NumCols;
        this->Rows[rowIndex].Cols = new T[other_numcols];
        this->Rows[rowIndex].NumCols = other_numcols;
        // for each element, copy the element from the parameter
        for (size_t colIndex = 0; colIndex < other_numcols; colIndex++) {
            this->Rows[rowIndex].Cols[colIndex] = other.Rows[rowIndex].Cols[colIndex];
        }
    }
    // return yourself
    return *this;
  }

  // returns number of rows
  size_t numrows() const {
    return this->NumRows;
  }
  
  // returns number of columns in a given row
  size_t numcols(size_t r) const {
    // if the row is out of bounds, throw an error
    if(r < 0 || r >= this->NumRows){
        throw invalid_argument("grid.c::numcols: invalid row index");
    }

    return this->Rows[r].NumCols;
  }

  // returns the size of the matrix (rows * cols)
  size_t size() const {
    return (this->NumRows) * (this->Rows[0].NumCols);
  }
  
  // returns the element at a given row an column index
  T& operator()(size_t r, size_t c) {
    // if the row is out of bounds, throw an error
    if(r < 0 || r >= this->NumRows){
        throw invalid_argument("grid.c::operator(): invalid row index");
    }
    // if the column is out of bounds, throw an error
    if(c < 0 || c >= this->Rows[r].NumCols){
        throw invalid_argument("grid.c::operator(): invalid column index");
    }

    return this->Rows[r].Cols[c];
  }
  
  // outputs the grid
  void _output() {
    // for each row
    for(size_t rowIndex = 0; rowIndex < this->NumRows; rowIndex++){
        // output a line left border line
        cout << "| ";
        // print the element at thet row and column with a line followong
        for(size_t colIndex = 0; colIndex < this->Rows[rowIndex].NumCols; colIndex++){
            cout << this->Rows[rowIndex].Cols[colIndex] << " | ";
        }
        // new line once row has ended
        cout << endl;
    }
  }

};