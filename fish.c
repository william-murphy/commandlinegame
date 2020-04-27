#include <stdio.h>

int startingGameBoard[6][6] = {{1, 1, 1, 1, 1, 1}, {1, 2, 2, 2, 2, 1}, {1, 2, 3, 3, 2, 1}, {1, 2, 3, 3, 2, 1}, {1, 2, 2, 2, 2, 1}, {1, 1, 1, 1, 1, 1}};
int gameBoard[6][6]; //this variable holds the gameboard, it is an int 2-D array with 0 = '.', 4 = 'P', 5 = 'A'
int totalPointsPlayer[20] = {1}; //this variable holds the scores from each round of the player
int totalPointsAI[20] = {1}; //this variable holds the score from each round of the AI
//both totaLPointsPlayer and totalPointsAI have '1' as the first element because the first element points to the next available element

int abs(int num) { //returns the absolute value of the number 'num' passed in
  if (num < 0) {
    num *= -1;
  }
  return num;
}

void copyArray(int oldArr[][6], int newArr[][6]) { //copies all elements of a 2-D array into a fresh 2-D array
  for (int i=0; i<6; i++) {
    for (int j=0; j<6; j++) {
      newArr[i][j] = oldArr[i][j];
    }
  }
}

void getWinner(int playerScore[], int aiScore[]) { //decides the winner and prints the appropriate message
  int sum1 = 0;
  int sum2 = 0;
  for (int i=1; i<playerScore[0]; i++) {
    sum1 += playerScore[i];
  }
  for (int j=1; j<aiScore[0]; j++) {
    sum2 += aiScore[j];
  }
  if (sum1 > sum2) {
    printf("Player wins!\n");
  }else if (sum1 < sum2) {
    printf("AI wins!\n");
  }else {
    printf("It's a tie!\n");
  }
}

void addScore(int totalPoints[], int score) { //adds the correct score to the end of the array
  /*the way this works is by adding the score to the index equal to the number at totalPoints[0],
  which keeps track of the next available space in the array by incrementing totalPoints[0] by 1 each
  time a score is added*/
  totalPoints[totalPoints[0]] = score;
  totalPoints[0] += 1;
}

void displayScore(int totalPoints[]) { //prints the current scores
  int sum = 0;
  printf("%i", totalPoints[1]);
  sum += totalPoints[1];
   for (int i=2; i<totalPoints[0]; i++) {
     printf(" + %i", totalPoints[i]);
     sum += totalPoints[i];
   }
   printf(" = %i\n", sum);
}

void createGameBoard(int board[][6]) { //prints the gameboard given a 2-d array
  printf ("  1 2 3 4 5 6\n");
  for (int i=0; i<6; i++) {
    printf("%i ", i + 1);
    for (int j=0; j<6; j++) {
      switch(board[i][j]) {
        case 0:
          printf(". ");
          break;
        case 4:
          printf("P ");
          break;
        case 5:
          printf("A ");
          break;
        default:
          printf("%i ", board[i][j]);
          break;
      }
    }
    printf("\n");
  }
}

void modifyGameBoard(int board[][6], int row, int col, int type) { //changes the given location on the gameboard to the given type
  board[row][col] = type;
}

int isValid(int board[][6], int row, int col) { // checks if the octagon is within bounds and not occupied or otherwise unavailable
  if ((row < 6 && row >= 0) && (col < 6 && col >= 0) && board[row][col] != 0 && board[row][col] != 4 && board[row][col] != 5) {
    return 1;
  }else {
    return 0;
  }
}

int findPath(int board[][6], int curRow, int curCol, int newRow, int newCol) {
  /*this function decides if a path to the desired row and column exists
  if it does, it returns the point value from said path
  if no path exists, it returns 0
  the way it works is by first deciding if the path is a straight line,
  then it modifies the game board at each space along the path*/
  int rowCount, colCount;
  int potentialBoard[6][6];
  int points;
  copyArray(board, potentialBoard);
  points = 0;
  if (curRow == newRow && curCol == newCol) { //check if the user selected the octagon they are already in
    return points;
  }else if (curRow == newRow && curCol != newCol) { //check if the user selected a horizontal line path
    if (curCol < newCol) { //check if the path goes left to right
      for (int i=curCol; i<newCol; i++) {
        if (isValid(potentialBoard, curRow - 1, i)) {
          points += potentialBoard[curRow - 1][i];
          modifyGameBoard(potentialBoard, curRow - 1, i, 0);
        }else {
          points = 0;
          return points;
        }
      }
    }else if (curCol > newCol) { //check if path goes right to left
      for (int j=curCol-2; j >= newCol-1; j--) {
        if (isValid(potentialBoard, curRow - 1, j)) {
          points += potentialBoard[curRow - 1][j];
          modifyGameBoard(potentialBoard, curRow - 1, j, 0);
        }else {
          points = 0;
          return points;
        }
      }
    }
  }else if (curRow != newRow && curCol == newCol) { //check if the user selected a vertical line path
    if (curRow < newRow) { //check if path goes down
      for (int i=curRow; i<newRow; i++) {
        if (isValid(potentialBoard, i, curCol - 1)) {
          points += potentialBoard[i][curCol - 1];
          modifyGameBoard(potentialBoard, i, curCol - 1, 0);
        }else {
          points = 0;
          return points;
        }
      }
    }else if (curRow > newRow) { //check if path goes up
      for (int j=curRow-2; j>=newRow-1; j--) {
        if (isValid(potentialBoard, j, curCol - 1)) {
          points += potentialBoard[j][curCol - 1];
          modifyGameBoard(potentialBoard, j, curCol - 1, 0);
        }else {
          points = 0;
          return points;
        }
      }
    }
  }else if (abs(newRow - curRow) == abs(newCol - curCol)) { //check if the user selected a diagonal line path
    if ((newRow - curRow > 0) && (newCol - curCol > 0)) {
      rowCount = curRow;
      colCount = curCol;
      while (rowCount < newRow && colCount < newCol) {
        if (isValid(potentialBoard, rowCount, colCount)) {
          points += potentialBoard[rowCount][colCount];
          modifyGameBoard(potentialBoard, rowCount, colCount, 0);
        }else {
          points = 0;
          return points;
        }
        rowCount++;
        colCount++;
      }
    }else if ((newRow - curRow < 0) && (newCol - curCol > 0)) {
      rowCount = curRow - 2;
      colCount = curCol;
      while (rowCount >= newRow - 1 && colCount < newCol) {
        if (isValid(potentialBoard, rowCount, colCount)) {
          points += potentialBoard[rowCount][colCount];
          modifyGameBoard(potentialBoard, rowCount, colCount, 0);
        }else {
          points = 0;
          return points;
        }
        rowCount--;
        colCount++;
      }
    }else if ((newRow - curRow < 0) && (newCol - curCol < 0)) {
      rowCount = curRow - 2;
      colCount = curCol - 2;
      while (rowCount >= newRow - 1 && colCount >= newCol - 1) {
        if (isValid(potentialBoard, rowCount, colCount)) {
          points += potentialBoard[rowCount][colCount];
          modifyGameBoard(potentialBoard, rowCount, colCount, 0);
        }else {
          points = 0;
          return points;
        }
        rowCount--;
        colCount--;
      }
    }else if ((newRow - curRow > 0) && (newCol - curCol < 0)) {
      rowCount = curRow;
      colCount = curCol - 2;
      while (rowCount < newRow && colCount >= newCol - 1) {
        if (isValid(potentialBoard, rowCount, colCount)) {
          points += potentialBoard[rowCount][colCount];
          modifyGameBoard(potentialBoard, rowCount, colCount, 0);
        }else {
          points = 0;
          return points;
        }
        rowCount++;
        colCount--;
      }
    }
  }else {
    points = 0;
    return points;
  }
  modifyGameBoard(potentialBoard, curRow - 1, curCol - 1, 0);
  modifyGameBoard(potentialBoard, newRow - 1, newCol - 1, 4);
  copyArray(potentialBoard, board);
  return points;
}

int ai (int board[][6]) {
  //return 0 means there are no moves left for the ai
  //return any other number means thats the score the ai got on this turn
  //first find where the ai is
  int potentialBoard[6][6];
  copyArray(board, potentialBoard);
  int aiRow, aiCol, newAiRow, newAiCol, aiMove;
  for (int i=0; i<6; i++) {
    for (int j=0; j<6; j++) {
      if (board[i][j] == 5) {
        aiRow = i;
        aiCol = j;
      }
    }
  }
  //try every spot that has a valid path to it and choose the one that returns the highest point value
  aiMove = 0;
  for (int k=0; k<6; k++) {
    for (int l=0; l<6; l++) {
      if (isValid(potentialBoard, k, l) == 1) {
        if (findPath(potentialBoard, aiRow + 1, aiCol + 1, k + 1, l + 1) > aiMove) {
          newAiRow = k;
          newAiCol = l;
          aiMove = potentialBoard[k][l];
        }
        copyArray(board, potentialBoard);
      }
    }
  }
  //at this point the location of the best move is found
  //need to reset the gameBoard then modify it with the new path
  if (aiMove > 0) {
    aiMove = findPath(board, aiRow + 1, aiCol + 1, newAiRow + 1, newAiCol + 1);
    modifyGameBoard(board, newAiRow, newAiCol, 5);
    return aiMove;
  }else {
    return 0;
  }
}

int main(void) {

  int gameState = 0; //used to control the game state, 0 -> pre-game or game over, 1 -> in game
  int curRow, curCol, newRow, newCol; //used to hold the current and new row, column values on each of the player's turns
  int validMove; //used to loop until the user selects a valid row column
  int playerPath, aiPath; //hold the point values of the player/ai paths each turn
  int aiOutOfMoves = 0; //used as a boolean to check if the ai is out of moves
  int playerOutOfMoves = 0; //used as a boolean to check if the player is out of moves
  copyArray(startingGameBoard, gameBoard);
  createGameBoard(gameBoard);

  while (gameState == 0) { //this loop is to get the player's beginning row and column, and the AI's accordingly
    printf("Please type your desired row, followed by a space, then your desired column, then hit enter. Your starting position must have a value of 1.\n");
    scanf("%i", &newRow);
    scanf(" %i", &newCol);
    if (gameBoard[newRow-1][newCol-1] == 1) {
      gameState = 1;
      modifyGameBoard(gameBoard, newRow - 1, newCol - 1, 4);
      addScore(totalPointsPlayer, 1);
      createGameBoard(gameBoard);
      printf("The AI will now choose a starting spot.\n");
      if (gameBoard[0][0] == 4) {
        modifyGameBoard(gameBoard, 0, 5, 5);
      }else {
        modifyGameBoard(gameBoard, 0, 0, 5);
      }
      addScore(totalPointsAI, 1);
      createGameBoard(gameBoard);
    }else {
      printf("\nYour starting position must be an octagon with a value of 1.\n");
    }
  }

  while (gameState == 1) { //this loop is running while the game is still going, it breaks when both AI and player are out of moves

    curRow = newRow;
    curCol = newCol;

    if (aiOutOfMoves == 1 && playerOutOfMoves == 1) {
      printf("Player's points: ");
      displayScore(totalPointsPlayer);
      printf("AI's points: ");
      displayScore(totalPointsAI);
      getWinner(totalPointsPlayer, totalPointsAI);
      gameState = 0;
    }

    validMove = 0;
    if (playerOutOfMoves == 0) {
      while (validMove == 0) {
        printf("Please type your desired row, followed by a space, then your desired column, then hit enter.\n");
        scanf("%i", &newRow);
        scanf(" %i", &newCol);
        printf("\n");
        playerPath = findPath(gameBoard, curRow, curCol, newRow, newCol);
        createGameBoard(gameBoard);
        if (playerPath > 0) {
          validMove = 1;
          addScore(totalPointsPlayer, playerPath);
        }else {
          printf("Please select an octagon with a valid path to it.\n");
        }
      }
    }
    if (aiOutOfMoves == 0) {
      aiPath = ai(gameBoard);
      if (aiPath == 0) {
        printf("The AI has run out of moves.\n");
        aiOutOfMoves = 1;
      }else {
        printf("Now the AI will do its move.\n");
        createGameBoard(gameBoard);
        addScore(totalPointsAI, aiPath);
      }
    }

    //check here if the player has any moves left
    if (isValid(gameBoard, newRow-2, newCol-2) == 0 && isValid(gameBoard, newRow-2, newCol-1) == 0 && isValid(gameBoard, newRow-2, newCol) == 0 && isValid(gameBoard, newRow-1, newCol) == 0 && isValid(gameBoard, newRow, newCol) == 0 && isValid(gameBoard, newRow, newCol-1) == 0 && isValid(gameBoard, newRow, newCol-2) == 0 && isValid(gameBoard, newRow-1, newCol-2) == 0) {
      playerOutOfMoves = 1;
    }
  }
}
