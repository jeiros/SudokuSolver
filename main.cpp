#include <iostream>
#include <array>

int solveCalls = 0; // To keep track of how many times solveBoard() is called


/*Nicely formats a 9x9 sudoku board
  @param arr a std::array which contains 9 std::arrays of ints
  */
void printBoard(std::array <std::array <int, 9>, 9> arr)
{
  int row_no = 0;
  int col_no = 0;
  std::cout << "┌─────────┬─────────┬─────────┐" << std::endl;
  for (auto row: arr) {
    std::cout << "│";
    for (auto number: row) {
      std::cout << " " << number << " ";
      if (((col_no + 1 ) % 3) == 0) {
        std::cout << "│";
      }
      col_no++;
    }
    if (((row_no +1)%3) == 0 && (row_no != 8) ) {
      std::cout << std::endl << "├─────────┼─────────┼─────────┤";
    } else if (row_no == 8) {
      std::cout << std::endl << "└─────────┴─────────┴─────────┘" << std::endl;
    }
    row_no++;
    col_no = 0;
    std::cout << std::endl;
  }
  
}

/*Checks wether a number is present in a given row of a sudoku board
  @param row The row in which to check for repetitions
  @param trial The number for which to check repetitions for
  @return bool True if trial is present in row, else False
  */
bool repeatInRow(std::array < int, 9 > row, int trial)
{
  for (auto number: row) {
    if (number == trial) {
      // printf("Found a duplicate in the row\n");
      return true;
    }
  }
  return false;
}

/*Checks wether a number is present in a given column of a sudoku board
  @param arr The whole sudoku board 
  @param trial The number for which to check repetitions for
  @col_no The column number in which to check repetitions
  @return bool True if trial is present in the column, else False
  */
bool repeatInCol(std::array <std::array <int, 9>, 9> arr, int trial, int col_no)
{
  for (auto row: arr) {
    if (row[col_no] == trial) {
      // printf("Found a duplicate in the col\n");
      return true;
    }
  }
  return false;
}

/*Checks wether a number is present in a given sub Quadrant of a sudoku board
There are 9 subquadants in a sudoku board. We use 'coordinates' for them, 
[0, 0] being the top left one and [2, 2] the bottom right one

  @param arr The whole sudoku board 
  @param trial The number for which to check repetitions for
  @row_no The row number, used to locate in which subquadrant we have to look
  @col_no The column number, used to locate in which subquadrant we have to look
  @return bool True if trial is present in subquadrant, else False
  */
bool repeatInSubQ(std::array <std::array <int, 9>, 9> arr,
  int trial, int row_no, int col_no)
{
  // We compute in which subQ we are with row_no and col_no
  int x_coord_Q = static_cast < int > (row_no / 3);
  int y_coord_Q = static_cast < int > (col_no / 3);

  for (int i = 3 * x_coord_Q; i <= 3 * x_coord_Q + 2; ++i) {
    for (int j = 3 * y_coord_Q; j <= 3 * y_coord_Q + 2; ++j) {
      if (arr[i][j] == trial) {
        return true;
      }
    }
  }
  return false;
}

/*Finds 0 numbers in the sudoku board (numbers which still have not been set)
  @param arr The whole sudoku board
  @param &row_no The row number passed by ref since we'll update it with where we've found
    a 0
  @param &col_no The col number passed by ref since we'll update it with where we've found
    a 0
  @return bool True if we find a 0, else False
assigned*/
bool findUnassignedLocation(std::array <std::array <int , 9 >, 9 > arr,
                              int &row_no, int &col_no)
{
  for (auto row : arr) {
    for (auto number : row) {
      if (number == 0) {
        return true;
      }
      col_no++;
    }
    row_no++;
    col_no = 0;
  }
  return false;
}

/*Check if number is valid by looking into repetitions in it's row, column and subquadrant
  @param arr The whole sudoku board
  @param col_no The column number in which to check repetitions
  @param row_no The row number in which to check repetitions
  @param trial The number we're checking
  @return bool True if the number is valid and there are no conflicts with it, else False

*/  
bool noConflicts(std::array < std::array < int , 9 >, 9 > arr,
                  int row_no, int col_no, int trial)
{
  std::array < int, 9 > row = arr[row_no];
  if (repeatInRow(row, trial) || repeatInCol(arr, trial, col_no) || repeatInSubQ(arr, trial, row_no, col_no)) {
    return false;
  }
  return true;
}

/*
Bactracking algorithm to solve a sudoku board.
We recursively call this function. If no 0's are found, the board is solved.
If there are, try to put numbers in it from 1-9. If a number has no conflicts, we put it and check if this solved the board.
If it didn't, we work on the next 0 that we found doing the same thing. If for a position there are no available numbers that
have no conflicts, this means that somewhere in the previous trials we have a mistake. So the function 'backtracks' to the most recent previous
trials and keeps trying numbers (starting from where we left it).
  @param &arr The whole sudoku board, passed by reference to be able to modify it's contents.
  @return bool True if the board is solved (there are no 0 in it), else False
*/
bool solveBoard(std::array < std::array < int, 9 > , 9 > &arr)
{
  // Initialize the search at [0, 0]
  int row_no = 0;
  int col_no = 0;
  solveCalls++;
  if (!findUnassignedLocation(arr, row_no, col_no)){return true;} // All allocations successfully assigned!



  for (int trial = 1; trial <= 9; trial++) {  // options are 1-9
  
    bool no_conflicts = noConflicts(arr, row_no, col_no, trial); // Did we find any conflicts?
  
    if (no_conflicts) { // trial looks OK
      arr[row_no][col_no] = trial;  // try to assign number
  
      if (solveBoard(arr)) { // Did this move solve the board?
        return true; // recur if succeed stop
      } else {
        arr[row_no][col_no] = 0; // undo & try again
      }
  
    }
  }
  
  return false; // triggers backtracking from early decisions
}

int main(int argc, char const * argv[])
{
  std::string input =  "850002400720000009004000000000107002305000900040000000000080070017000000000036040";

  std::array < std::array < int, 9 > , 9 > board;  // 9x9 std::array is the board
  // Populate the board
  int letter = 0;
  for (int i = 0; i < 9; ++i) {
    for (int j = 0; j < 9; ++j) {
      int num = static_cast < int > (input[letter] - '0'); // Convert str to int
      board[i][j] = num;
      ++letter;
    }
  }

  printf("Initial board\n");
  printBoard(board);
  bool finished = solveBoard(board);

  if (finished) {
    printf("Finished! Took me %d iterations\n", solveCalls);
    printBoard(board);
  }


  return 0;
}
