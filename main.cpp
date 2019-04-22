#include <iostream>
#include "stack.h"
#include <queue>
using namespace std;

struct Cell {
  int cellStatus; // 0 = wall/background, 1 = foreground, 2+ = respective image components
  int imgCompNum; // keeps track of what number cell current cell is in its image component
  int row; //row position of cell in grid
  int col; //col position of cell in grid

  Cell(): cellStatus(0), imgCompNum(0), row(0), col(0)
  {
    //intentionally empty
  }
  Cell(int theRow, int theCol): row(theRow), col(theCol), cellStatus(0), imgCompNum(0)
  {
    //intentionally empty
  }
  Cell(int theCellStatus, int theRow, int theCol): cellStatus(theCellStatus), row(theRow), col(theCol), imgCompNum(0)
  {
    //intentionally empty
  }
};

void inputGridParameters(int &dimension, double &density) //gets dimension/density values from user and sets them in main
{
  string defIn;
  do {
    cout << "Use default dimension (15x15) and density (0.5)? (y/n): ";

    cin >> defIn;
  }
  while (defIn != "y" && defIn != "Y" && defIn != "n" && defIn != "N");

  cout << endl;

  if (defIn == "y" || defIn == "Y") {
    dimension = 15;
    density = 0.5;
  }
  else if (defIn == "n" || defIn == "N") {

    int dimIn;
    do {
      cout << "Enter the desired dimension (between 5 and 20): ";
      cin >> dimIn;

      dimension = dimIn;
    }
    while(!(dimIn >= 5) || !(dimIn <= 20));

    cout << endl;

    double denIn;
    do {
      cout << "Enter the desired density (decimal between 0.2 and 0.8): ";
      cin >> denIn;

      density = denIn;
    }
    while(!(denIn >= 0.2) || !(denIn <= 0.8));
  }
}

void createGrids(Cell** &dfsGrid, Cell** &bfsGrid, int dimension, double density)
{
  for (int i = 0; i < dimension+2; i++)
  {
    dfsGrid[i] = new Cell[dimension+2];
    bfsGrid[i] = new Cell[dimension+2];
  }

  for (int row = 0; row < 1; row++) {
    for (int col = 0; col < dimension + 2; col++) {
      dfsGrid[row][col] = Cell(row,col);
      bfsGrid[row][col] = Cell(row,col);
    }
   }
  for (int row = 0; row < dimension + 2; row++) {
    for (int col = 0; col < 1; col++) {
      dfsGrid[row][col] = Cell(row,col);
      bfsGrid[row][col] = Cell(row,col);
    }
  }
  for (int row = dimension + 1; row < dimension + 2; row++) {
    for (int col = 0; col < dimension + 2; col++) {
      dfsGrid[row][col] = Cell(row,col);
      bfsGrid[row][col] = Cell(row,col);
    }
  }
  for (int row = 0; row < dimension + 2; row++) {
    for (int col = dimension + 1; col < dimension + 2; col++) {
      dfsGrid[row][col] = Cell(row,col);
      bfsGrid[row][col] = Cell(row,col);
    }
  }

  for (int row = 1; row <= dimension; row++) {
    for (int col = 1; col <= dimension; col++) {
      int r = rand();
      double myR = (double)r/RAND_MAX;
      if (myR < density) {
        dfsGrid[row][col] = Cell(1, row,col);
        bfsGrid[row][col] = Cell(1, row,col);
      }
      else {
        dfsGrid[row][col] = Cell(row,col);
        bfsGrid[row][col] = Cell(row,col);
      }
    }
  }
}

void displayGrid(Cell** &grid, int dimension, string gridName)
{
  cout << gridName << endl;
  for (int row = 0; row < dimension + 2; row++) {
    for (int col = 0; col < dimension + 2; col++) {
      if (grid[row][col].cellStatus >= 10 && grid[row][col].imgCompNum >= 10) {
        cout << "[" << grid[row][col].cellStatus << "," << grid[row][col].imgCompNum << "]" << " ";
      }
      else if (grid[row][col].cellStatus >= 10 && grid[row][col].imgCompNum < 10) {
        cout << "[" << grid[row][col].cellStatus << ", " << grid[row][col].imgCompNum << "]" << " ";
      }
      else if (grid[row][col].cellStatus < 10 && grid[row][col].imgCompNum >= 10) {
        cout << "[ " << grid[row][col].cellStatus << "," << grid[row][col].imgCompNum << "]" << " ";
      }
      else {
        cout << "[ " << grid[row][col].cellStatus << ", " << grid[row][col].imgCompNum << "]" << " ";
      }
    }
    cout << endl;
  }
  cout << endl;
}

void scanByDfs(Cell** &dfsGrid, int row, int col, int componentLabel)
{
  int cellNumber = 1;

  stack<Cell> dfsStack;
  dfsGrid[row][col].cellStatus = componentLabel;
  dfsGrid[row][col].imgCompNum = cellNumber;
  cellNumber++;

  dfsStack.push(dfsGrid[row][col]);

  while (dfsStack.empty() == false) {
    if (dfsGrid[row][col+1].cellStatus == 1) { //right
      col++;
      dfsGrid[row][col].cellStatus = componentLabel;
      dfsGrid[row][col].imgCompNum = cellNumber;
      cellNumber++;
      dfsStack.push(dfsGrid[row][col]);
    }
    else if (dfsGrid[row+1][col].cellStatus == 1) { //down
      row++;
      dfsGrid[row][col].cellStatus = componentLabel;
      dfsGrid[row][col].imgCompNum = cellNumber;
      cellNumber++;
      dfsStack.push(dfsGrid[row][col]);
    }
    else if (dfsGrid[row][col-1].cellStatus == 1) { //left
      col--;
      dfsGrid[row][col].cellStatus = componentLabel;
      dfsGrid[row][col].imgCompNum = cellNumber;
      cellNumber++;
      dfsStack.push(dfsGrid[row][col]);
    }
    else if (dfsGrid[row-1][col].cellStatus == 1) { //up
      row--;
      dfsGrid[row][col].cellStatus = componentLabel;
      dfsGrid[row][col].imgCompNum = cellNumber;
      cellNumber++;
      dfsStack.push(dfsGrid[row][col]);
    }
    else{ // no 1 in any direction, retreat 1 step
      dfsStack.pop();
      if (dfsStack.empty() == false) {
        row = dfsStack.top().row;
        col = dfsStack.top().col;
      }
    }
  }
}

void scanByBfs(Cell** &bfsGrid, int row, int col, int componentLabel)
{
  int cellNumber = 1;

  queue<Cell> bfsQueue;
  bfsGrid[row][col].cellStatus = componentLabel;
  bfsGrid[row][col].imgCompNum = cellNumber;
  cellNumber++;

  bfsQueue.push(bfsGrid[row][col]);

  while (bfsQueue.empty() == false) {
    if (bfsGrid[bfsQueue.front().row][bfsQueue.front().col+1].cellStatus == 1) { //right
      bfsGrid[bfsQueue.front().row][bfsQueue.front().col+1].cellStatus = componentLabel;
      bfsGrid[bfsQueue.front().row][bfsQueue.front().col+1].imgCompNum = cellNumber;
      cellNumber++;
      bfsQueue.push(bfsGrid[bfsQueue.front().row][bfsQueue.front().col+1]);
    }
    if (bfsGrid[bfsQueue.front().row+1][bfsQueue.front().col].cellStatus == 1) { //down
      bfsGrid[bfsQueue.front().row+1][bfsQueue.front().col].cellStatus = componentLabel;
      bfsGrid[bfsQueue.front().row+1][bfsQueue.front().col].imgCompNum = cellNumber;
      cellNumber++;
      bfsQueue.push(bfsGrid[bfsQueue.front().row+1][bfsQueue.front().col]);
    }
    if (bfsGrid[bfsQueue.front().row][bfsQueue.front().col-1].cellStatus == 1) { //left
      bfsGrid[bfsQueue.front().row][bfsQueue.front().col-1].cellStatus = componentLabel;
      bfsGrid[bfsQueue.front().row][bfsQueue.front().col-1].imgCompNum = cellNumber;
      cellNumber++;
      bfsQueue.push(bfsGrid[bfsQueue.front().row][bfsQueue.front().col-1]);
    }
    if (bfsGrid[bfsQueue.front().row-1][bfsQueue.front().col].cellStatus == 1) { //up
      bfsGrid[bfsQueue.front().row-1][bfsQueue.front().col].cellStatus = componentLabel;
      bfsGrid[bfsQueue.front().row-1][bfsQueue.front().col].imgCompNum = cellNumber;
      cellNumber++;
      bfsQueue.push(bfsGrid[bfsQueue.front().row-1][bfsQueue.front().col]);
    }
    bfsQueue.pop();
  }
}

void scanGrids(Cell** &dfsGrid, Cell** &bfsGrid, int dimension)
{
  int componentLabel = 1;
  for (int row = 1; row < dimension + 1; row++) {
    for (int col = 1; col < dimension + 1; col++) {
      if (dfsGrid[row][col].cellStatus == 1) {
        componentLabel++;
        scanByDfs(dfsGrid, row, col, componentLabel);
        scanByBfs(bfsGrid, row, col, componentLabel);
      }
    }
  }
}

int main()
{
  cout << "Image Component Labeling Program" << endl << endl;

  int dimension;
  double density;

  inputGridParameters(dimension, density);

  Cell** dfsGrid = new Cell*[dimension + 2]; //depth first search grid (DFS grid) //+2 for walls
  Cell** bfsGrid = new Cell*[dimension + 2]; //breadth first search grid (BFS grid) //+2 for walls

  srand(time(NULL)); //srand(44) is temp const num for testing, use srand(time(NULL)) for real random #
  createGrids(dfsGrid, bfsGrid, dimension, density);

  displayGrid(dfsGrid, dimension, "Depth First Search Grid");
  displayGrid(bfsGrid, dimension, "Breadth First Search Grid");

  scanGrids(dfsGrid, bfsGrid, dimension);

  displayGrid(dfsGrid, dimension, "Depth First Search Grid (Post Labeling)");
  displayGrid(bfsGrid, dimension, "Breadth First Search Grid (Post Labeling)");

  return 0;
}
