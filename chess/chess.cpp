#include <iostream>
#include <vector>
#include <string>
#include <vector>
#include <cmath>
#include <stdlib.h>
using namespace std;

// === structures ===

struct pieceData {
	// we have white and black color for squares which have piece and we have "" for others
	string color;
	// it is the name of piece that is located in that square
	// it could be one of theses status 
	// (
	// " " : it means there is no pieces in this sqaure,		
	// R : rook,
	// N : knight,
	// B : bishop,
	// Q : queen,
	// K : king,
	// P : pawn
	// )
	string currentPiece;
};

struct boardSquares
{
	// each square has own color (black or white)
	bool isWhite;
	// a number between (1 to 8)
	int rankNum;
	// one of these letters (a, b, c, d, e, f, g, h)
	string fileName;
	pieceData pieceData;
	bool isUnderWhiteAttack;
	bool isUnderBlackAttack;
};

struct piecesColor {
	string right;
	string left;
};

struct checkStruct {
	bool isBlackCheck;
	bool isWhiteCheck;
};

// === global data === 
string fileNames[] = { "a", "b", "c", "d", "e", "f", "g", "h" };
//vector<string> names = givePlayerNames();
vector<string> names = { "playerOne", "playerTwo" };
// we have these specialMoves (pawnPromotion, enPassant, kingSideCastle, qweenSideCastle)
string specialMove = "none";

// === functions ===
void makeMove(string playerMove, boardSquares** chessBoard, bool isWhiteToMove, checkStruct checkStatus);
checkStruct playerCheckStatus(boardSquares** chessBoard);
bool doesMoveGetPlayerOutOfCheck(boardSquares** chessBoard, int startFile, int startRank, int targetFile, int targetRank, checkStruct checkStatus);

//vector<string> givePlayerNames() {
//    string whitePlayerName, blackPlayerName;
//
//    cout << "please enter white player name" << endl;
//    cin >> whitePlayerName;
//    cout << "please enter black player name" << endl;
//    cin >> blackPlayerName;
//
//    vector<string> names = { whitePlayerName , blackPlayerName };
//    return names;
//}

string giveFirstPieceArrangment(int i, int j) {
	string piece = " ";
	if (i == 1 || i == 6) {
		piece = "P";
	}
	else if ((i == 0 || i == 7) && (j == 0 || j == 7)) {
		piece = "R";
	}
	else if ((i == 0 || i == 7) && (j == 1 || j == 6)) {
		piece = "N";
	}
	else if ((i == 0 || i == 7) && (j == 2 || j == 5)) {
		piece = "B";
	}
	else if ((i == 0 || i == 7) && j == 3) {
		piece = "Q";
	}
	else if ((i == 0 || i == 7) && j == 4) {
		piece = "K";
	}
	return piece;
}

string givePiecesColor(int i) {
	string color = "";
	if (i < 2) {
		color = "white";
	}
	else if (i > 5) {
		color = "black";
	}
	return color;
}

boardSquares** makeChessBoard() {
	boardSquares** chessBoard = new boardSquares * [8];

	for (int i = 0; i < 8; i++) {
		chessBoard[i] = new boardSquares[8];
	}

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			chessBoard[i][j].isWhite = (i + j) % 2 == 0 ? false : true;
			chessBoard[i][j].rankNum = i + 1;
			chessBoard[i][j].fileName = fileNames[j];
			chessBoard[i][j].isUnderBlackAttack = false;
			chessBoard[i][j].isUnderWhiteAttack = false;
			//chessBoard[i][j].pieceData.currentPiece = giveFirstPieceArrangment(i, j);
			//chessBoard[i][j].pieceData.color = givePiecesColor(i);

			chessBoard[i][j].pieceData.currentPiece = " ";

			if (i == 0 && j == 0) {
				chessBoard[i][j].pieceData.currentPiece = "K";
				chessBoard[i][j].pieceData.color = "black";
			}
			if (i == 2 && j == 4) {
				chessBoard[i][j].pieceData.currentPiece = "B";
				chessBoard[i][j].pieceData.color = "black";
			}
			//if (i == 7 && j == 1) {
			//	chessBoard[i][j].pieceData.currentPiece = "R";
			//	chessBoard[i][j].pieceData.color = "black";
			//}
			if (i == 1 && j == 7) {
				chessBoard[i][j].pieceData.currentPiece = "R";
				chessBoard[i][j].pieceData.color = "white";
			}
			if (i == 5 && j == 6) {
				chessBoard[i][j].pieceData.currentPiece = "Q";
				chessBoard[i][j].pieceData.color = "white";
			}
			if (i == 5 && j == 5) {
				chessBoard[i][j].pieceData.currentPiece = "K";
				chessBoard[i][j].pieceData.color = "white";
			}
			if (i == 5 && j == 4) {
				chessBoard[i][j].pieceData.currentPiece = "B";
				chessBoard[i][j].pieceData.color = "white";
			}
		}
	}

	return chessBoard;
}

piecesColor givePieceColorSymbol(string color) {
	piecesColor symbol;
	if (color == "white") {
		symbol.left = " +";
		symbol.right = "+ ";
	}
	else {
		symbol.left = "  ";
		symbol.right = "  ";
	}
	return symbol;
}

string showBoard(boardSquares** chessBoard, bool isWhiteToMove) {
	string currentPlayerName = isWhiteToMove ? names.at(0) : names.at(1);
	string guide = "\n\n\t  " + currentPlayerName + " is your turn " + "\n\n\n";
	string borders = "\t     ";
	for (int i = 0; i < 8; i++) {
		borders = borders + fileNames[i] + "      ";
	}
	string stringOfBoard = guide + borders + "\n \t  --------------------------------------------------------";

	// i had to use this shape of loop for different between the start point in matrix and chess board
	for (int i = 7; i >= 0; i--) {
		// colorLine is a string containing a basic style for showing black and white squares
		string colorLine = "\n\t  ";
		for (int j = 0; j < 8; j++) {
			if (chessBoard[i][j].isWhite) {
				colorLine = colorLine + "|#   #|";
				//colorLine = colorLine + "|#" + (chessBoard[i][j].isUnderWhiteAttack ? "w" : " ") + " " + (chessBoard[i][j].isUnderBlackAttack ? "b" : " ") + "#|";
			}
			else {
				colorLine = colorLine + "|     |";
				//colorLine = colorLine + "| " + (chessBoard[i][j].isUnderWhiteAttack ? "w" : " ") + " " + (chessBoard[i][j].isUnderBlackAttack ? "b" : " ") + " |";
			}
		}
		colorLine = colorLine + "\n\t";

		stringOfBoard = stringOfBoard + colorLine + to_string(i + 1) + " ";
		for (int j = 0; j < 8; j++) {
			piecesColor symbol = givePieceColorSymbol(chessBoard[i][j].pieceData.color);

			//stringOfBoard = stringOfBoard + "| " + chessBoard[i][j].fileName + "" + to_string(chessBoard[i][j].rankNum) + " |";
			stringOfBoard = stringOfBoard + "|" + symbol.left + chessBoard[i][j].pieceData.currentPiece + symbol.right + "|";
		}
		stringOfBoard = stringOfBoard + " " + to_string(i + 1) + colorLine;
		stringOfBoard = stringOfBoard + "  --------------------------------------------------------\t";
	}

	return stringOfBoard + "\n" + borders + "\n\n\n";
}

bool isMoveSyntaxCorrect(string pieceName, int startFile, int startRank, int targetFile, int targerRank) {
	vector<string> acceptablePieceStr = { "R", "N", "B", "Q", "K", "P" };
	bool isOk = true;
	auto findPieceName = find(acceptablePieceStr.begin(), acceptablePieceStr.end(), pieceName);

	if (distance(acceptablePieceStr.begin(), findPieceName) == 6) {
		isOk = false;
	}
	else if (startFile < 0 || startFile > 7) {
		isOk = false;
	}
	else if (targetFile < 0 || targetFile > 7) {
		isOk = false;
	}
	else if (startRank < 0 || startRank > 7) {
		isOk = false;
	}
	else if (targerRank < 0 || targerRank > 7) {
		isOk = false;
	}

	return isOk;
}

bool isPlayerPiece(bool isWhiteToMove, boardSquares** chessBoard, int startFile, int startRank) {
	string legalColor = (isWhiteToMove ? "white" : "black");
	if (chessBoard[startRank][startFile].pieceData.color == legalColor) {
		return true;
	}
	else { return false; }
}

bool isPieceInLocation(string pieceName, boardSquares** chessBoard, int startFile, int startRank) {
	if (chessBoard[startRank][startFile].pieceData.currentPiece == pieceName) {
		return true;
	}
	else { return false; }
}

bool isTreason(bool isWhiteToMove, boardSquares** chessBoard, int targetFile, int targerRank) {
	string playerColor = (isWhiteToMove ? "white" : "black");
	if (chessBoard[targerRank][targetFile].pieceData.currentPiece != " ") {
		if (chessBoard[targerRank][targetFile].pieceData.color != playerColor) {
			return false;
		}
		else {
			return true;
		}
	}
	else {
		return false;
	}
}

bool canRookOrQweenSaveKing(boardSquares** chessBoard, int startRank, int startFile, checkStruct checkStatus) {
	// in rook or qween move (only vertical or only horizontal moves) we need to check just ranks or just files
	// checking higher ranks
	for (int a = startRank + 1; a <= 7; a++) {
		if (chessBoard[a][startFile].pieceData.currentPiece == " ") {
			if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, startFile, a, checkStatus)) {
				return true;
			}
		}
	}
	// checking lower ranks
	for (int a = startRank - 1; a >= 0; a--) {
		if (chessBoard[a][startFile].pieceData.currentPiece == " ") {
			if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, startFile, a, checkStatus)) {
				return true;
			}
		}
	}
	// checking higher files
	for (int a = startFile + 1; a <= 7; a++) {
		if (chessBoard[startRank][a].pieceData.currentPiece == " ") {
			if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, a, startRank, checkStatus)) {
				return true;
			}
		}
	}
	// checking lower files
	for (int a = startFile - 1; a >= 0; a--) {
		if (chessBoard[startRank][a].pieceData.currentPiece == " ") {
			if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, a, startRank, checkStatus)) {
				return true;
			}
		}
	}
	return false;
}

bool canBishopOrQweenSaveKing(boardSquares** chessBoard, int startRank, int startFile, checkStruct checkStatus) {
	// in bishop or qween move (both vertical and horizontal at the same time) we need to check both ranks and files
    // checking higher ranks and higher files
	for (int a = startRank + 1, b = startFile + 1; a <= 7; a++, b++) {
		if (chessBoard[a][b].pieceData.currentPiece == " ") {
			if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, b, a, checkStatus)) {
				return true;
			}
		}
	}
	// checking higher ranks and lower files
	for (int a = startRank + 1, b = startFile - 1; a <= 7; a++, b--) {
		if (chessBoard[a][b].pieceData.currentPiece == " ") {
			if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, b, a, checkStatus)) {
				return true;
			}
		}
	}
	// checking lower ranks and higher files
	for (int a = startRank - 1, b = startFile + 1; a >= 0; a--, b++) {
		if (chessBoard[a][b].pieceData.currentPiece == " ") {
			if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, b, a, checkStatus)) {
				return true;
			}
		}
	}
	// checking lower ranks and lower files
	for (int a = startRank - 1, b = startFile - 1; a >= 0; a--, b--) {
		if (chessBoard[a][b].pieceData.currentPiece == " ") {
			if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, b, a, checkStatus)) {
				return true;
			}
		}
	}

	return false;
}

//bool canKnightSaveKing(boardSquares** chessBoard, int startRank, int startFile) {
//	return false;
//}
//
//bool canPawnSaveKing(boardSquares** chessBoard, int startRank, int startFile) {
//	return false;
//}

bool isCheckMate(boardSquares** chessBoard, checkStruct checkStatus) {
	string checkedTeamColor = (checkStatus.isWhiteCheck ? "white" : "black");
	int kingStartRank, kingStartFile;
	for (int i = 0; i <= 7; i++) {
		for (int j = 0; j <= 7; j++) {
			if (chessBoard[i][j].pieceData.currentPiece == "K" && chessBoard[i][j].pieceData.color == checkedTeamColor) {
				kingStartRank = i;
				kingStartFile = j;
			}
		}
	}

	// checking for scape squares
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			int kingTargetRank = kingStartRank + i;
			int kingTargetFile = kingStartFile + j;

			if (kingTargetRank <= 7 && kingTargetRank >= 0) {
				if (kingTargetFile <= 7 && kingTargetFile >= 0) {
					if (checkedTeamColor == "white") {
						// squares that are near white king but those are not under black attack
						if (!chessBoard[kingTargetRank][kingTargetFile].isUnderBlackAttack) {
							// checking that it is a good square for scaping
							if (doesMoveGetPlayerOutOfCheck(chessBoard, kingStartFile, kingStartRank, kingTargetFile, kingTargetRank, checkStatus)) {
								return false;
							}
						}
					}
					else {
						// squares that are near black king but those are not under white attack
						if (!chessBoard[kingTargetRank][kingTargetFile].isUnderWhiteAttack) {
							// checking that it is a good square for scaping
							if (doesMoveGetPlayerOutOfCheck(chessBoard, kingStartFile, kingStartRank, kingTargetFile, kingTargetRank, checkStatus)) {
								return false;
							}
						}
					}
				}
			}
		}
	}

	// checking for moving one of piece between king and attacker
	for (int i = 0; i <= 7; i++) {
		for (int j = 0; j <= 7; j++) {
			if (chessBoard[i][j].pieceData.currentPiece != " ") {
				if (chessBoard[i][j].pieceData.color == checkedTeamColor) {
					if (chessBoard[i][j].pieceData.currentPiece == "Q") {
						if (canRookOrQweenSaveKing(chessBoard, i, j, checkStatus) || canBishopOrQweenSaveKing(chessBoard, i, j, checkStatus)) {
							return false;
						}
					}
					else if (chessBoard[i][j].pieceData.currentPiece == "R") {
						if (canRookOrQweenSaveKing(chessBoard, i, j, checkStatus)) {
							return false;
						}
					}
					else if (chessBoard[i][j].pieceData.currentPiece == "B") {
						if (canBishopOrQweenSaveKing(chessBoard, i, j, checkStatus)) {
							return false;
						}
					}
					//else if (chessBoard[i][j].pieceData.currentPiece == "N") {
					//	if (canKnightSaveKing(chessBoard, i, j, checkStatus)) {
					//		return false;
					//	}
					//}
					//else if (chessBoard[i][j].pieceData.currentPiece == "P") {
					//	if (canPawnSaveKing(chessBoard, i, j, checkStatus)) {
					//		return false;
					//	}
					//}
				}
			}
		}
	}

	return true;
}

void showWinner(bool isWhiteWinner) {
	cout << "\t *********** WINNER WINNER CHICKEN DINNER ***********" << endl << endl;
	cout << "\t congratulation the " << (isWhiteWinner ? (names[0] + " ( white player )") : (names[1] + " ( black player )")) << " is our winner." << endl << endl;
	cout << "\t ****************************************************" << endl;
}

void continueTheGame(boardSquares** chessBoard, bool isWhiteToMove, checkStruct checkStatus) {
	string playerMove;
	if (!checkStatus.isBlackCheck && !checkStatus.isWhiteCheck) {
		getline(cin, playerMove);
		makeMove(playerMove, chessBoard, isWhiteToMove, checkStatus);
	}
	else {
		if (isCheckMate(chessBoard, checkStatus)) {
			// winner is the person who played last turn
			showWinner(!isWhiteToMove);
		}
		else {
			getline(cin, playerMove);
			makeMove(playerMove, chessBoard, isWhiteToMove, checkStatus);
		}
	}
}

void tryAnotherMove(boardSquares** chessBoard, bool isWhiteToMove, string errorMessage, checkStruct checkStatus) {
	//system("CLS");
	cout << "\t" << showBoard(chessBoard, isWhiteToMove);
	cout << errorMessage << endl;
	continueTheGame(chessBoard, isWhiteToMove, checkStatus);
}

bool isUnderKingAttack(int i, int j, boardSquares** chessBoard, string attackerColor) {
	for (int a = -1; a <= 1; a++) {
		for (int b = -1; b <= 1; b++) {
			if (i + a < 0 || i + a > 7) continue;
			if (j + b < 0 || j + b > 7) continue;

			if (chessBoard[i + a][j + b].pieceData.currentPiece == "K") {
				// setting attack color depends on pieces
				if (chessBoard[i + a][j + b].pieceData.color == attackerColor) {
					return true;
				}
			}
		}
	}

	return false;
}

bool isUnderRookAndQweenAttack(int i, int j, boardSquares** chessBoard, string attackerColor) {
	// in rook or qween move (only vertical or only horizontal moves) we need to check just ranks or just files
	// checking higher ranks
	for (int a = i + 1; a <= 7; a++) {
		if (chessBoard[a][j].pieceData.currentPiece != " ") {
			if (chessBoard[a][j].pieceData.currentPiece == "R" || chessBoard[a][j].pieceData.currentPiece == "Q") {
				// setting attack color depends on pieces
				if (chessBoard[a][j].pieceData.color == attackerColor) {
					return true;
				}
				else {
					break;
				}
			}
			else {
				break;
			}
		}
	}
	// checking lower ranks
	for (int a = i - 1; a >= 0; a--) {
		if (chessBoard[a][j].pieceData.currentPiece != " ") {
			if (chessBoard[a][j].pieceData.currentPiece == "R" || chessBoard[a][j].pieceData.currentPiece == "Q") {
				// setting attack color depends on pieces
				if (chessBoard[a][j].pieceData.color == attackerColor) {
					return true;
				}
				else {
					break;
				}
			}
			else {
				break;
			}
		}
	}
	// checking higher files
	for (int a = j + 1; a <= 7; a++) {
		if (chessBoard[i][a].pieceData.currentPiece != " ") {
			if (chessBoard[i][a].pieceData.currentPiece == "R" || chessBoard[i][a].pieceData.currentPiece == "Q") {
				// setting attack color depends on pieces
				if (chessBoard[i][a].pieceData.color == attackerColor) {
					return true;
				}
				else {
					break;
				}
			}
			else {
				break;
			}
		}
	}
	// checking lower files
	for (int a = j - 1; a >= 0; a--) {
		if (chessBoard[i][a].pieceData.currentPiece != " ") {
			if (chessBoard[i][a].pieceData.currentPiece == "R" || chessBoard[i][a].pieceData.currentPiece == "Q") {
				// setting attack color depends on pieces
				if (chessBoard[i][a].pieceData.color == attackerColor) {
					return true;
				}
				else {
					break;
				}
			}
			else {
				break;
			}
		}
	}
	return false;
}

bool isUnderBishopAndQweenAttack(int i, int j, boardSquares** chessBoard, string attackerColor) {
	// in bishop or qween move (both vertical and horizontal at the same time) we need to check both ranks and files
	// checking higher ranks and higher files
	for (int a = i + 1, b = j + 1; a <= 7; a++, b++) {
		if (chessBoard[a][b].pieceData.currentPiece != " ") {
			if (chessBoard[a][b].pieceData.currentPiece == "B" || chessBoard[a][b].pieceData.currentPiece == "Q") {
				if (chessBoard[a][b].pieceData.color == attackerColor) {
					return true;
				}
				else {
					break;
				}
			}
			else {
				break;
			}
		}
	}
	// checking higher ranks and lower files
	for (int a = i + 1, b = j - 1; a <= 7; a++, b--) {
		if (chessBoard[a][b].pieceData.currentPiece != " ") {
			if (chessBoard[a][b].pieceData.currentPiece == "B" || chessBoard[a][b].pieceData.currentPiece == "Q") {
				if (chessBoard[a][b].pieceData.color == attackerColor) {
					return true;
				}
				else {
					break;
				}
			}
			else {
				break;
			}
		}
	}
	// checking lower ranks and higher files
	for (int a = i - 1, b = j + 1; a >= 0; a--, b++) {
		if (chessBoard[a][b].pieceData.currentPiece != " ") {
			if (chessBoard[a][b].pieceData.currentPiece == "B" || chessBoard[a][b].pieceData.currentPiece == "Q") {
				if (chessBoard[a][b].pieceData.color == attackerColor) {
					return true;
				}
				else {
					break;
				}
			}
			else {
				break;
			}
		}
	}
	// checking lower ranks and lower files
	for (int a = i - 1, b = j - 1; a >= 0; a--, b--) {
		if (chessBoard[a][b].pieceData.currentPiece != " ") {
			if (chessBoard[a][b].pieceData.currentPiece == "B" || chessBoard[a][b].pieceData.currentPiece == "Q") {
				if (chessBoard[a][b].pieceData.color == attackerColor) {
					return true;
				}
				else {
					break;
				}
			}
			else {
				break;
			}
		}
	}
	return false;
}

bool isUnderPawnAttack(int i, int j, boardSquares** chessBoard, string attackerColor) {
	int top = i - 1, bottom = i + 1, right = j - 1, left = j + 1;
	if (top >= 0 && bottom <= 7) {
		if (left >= 0 && right <= 7) {
			// white pawns attak to (one block top,one block right and one block top, one block left) sqaures
			if (attackerColor == "white") {
				if (chessBoard[top][right].pieceData.currentPiece == "P") {
					if (chessBoard[top][right].pieceData.color == "white") {
						return true;
					}
				}
				if (chessBoard[top][left].pieceData.currentPiece == "P") {
					if (chessBoard[top][left].pieceData.color == "white") {
						return true;
					}
				}
			}
			// black pawns attak to (one block bottom,one block right and one block bottom, one block left) sqaures
			else {
				if (chessBoard[bottom][right].pieceData.currentPiece == "P") {
					if (chessBoard[bottom][right].pieceData.color == "black") {
						return true;

					}
				}
				else if (chessBoard[bottom][left].pieceData.currentPiece == "P") {
					if (chessBoard[bottom][left].pieceData.color == "black") {
						return true;

					}
				}
			}
		}
	}
	return false;
}

bool isUnderKnightAttack(int i, int j, boardSquares** chessBoard, string attackerColor) {
	// each knight can attack 8 squares (2 top right, 2 top left, 2 bottom right, 2 bottom left)
	int twoLeft = j + 2,
		twoRight = j - 2,
		oneLeft = j + 1,
		oneRight = j - 1,
		oneTop = i - 1,
		oneBottom = i + 1,
		twoTop = i - 2,
		twoBottom = i + 2;

	if (oneTop >= 0) {
		if (twoLeft <= 7) {
			if (chessBoard[oneTop][twoLeft].pieceData.currentPiece == "N") {
				if (chessBoard[oneTop][twoLeft].pieceData.color == attackerColor) {
					return true;
				}
			}
		}
		if (twoRight >= 0) {
			if (chessBoard[oneTop][twoRight].pieceData.currentPiece == "N") {
				if (chessBoard[oneTop][twoRight].pieceData.color == attackerColor) {
					return true;
				}
			}
		}
	}
	if (twoTop >= 0) {
		if (oneLeft <= 7) {
			if (chessBoard[twoTop][oneLeft].pieceData.currentPiece == "N") {
				if (chessBoard[twoTop][oneLeft].pieceData.color == attackerColor) {
					return true;
				}
			}
		}
		if (oneRight >= 0) {
			if (chessBoard[twoTop][oneRight].pieceData.currentPiece == "N") {
				if (chessBoard[twoTop][oneRight].pieceData.color == attackerColor) {
					return true;
				}
			}
		}
	}
	if (oneBottom <= 7) {
		if (twoLeft <= 7) {
			if (chessBoard[oneBottom][twoLeft].pieceData.currentPiece == "N") {
				if (chessBoard[oneBottom][twoLeft].pieceData.color == attackerColor) {
					return true;
				}
			}
		}
		if (twoRight >= 0) {
			if (chessBoard[oneBottom][twoRight].pieceData.currentPiece == "N") {
				if (chessBoard[oneBottom][twoRight].pieceData.color == attackerColor) {
					return true;
				}
			}
		}
	}
	if (twoBottom <= 7) {
		if (oneLeft <= 7) {
			if (chessBoard[twoBottom][oneLeft].pieceData.currentPiece == "N") {
				if (chessBoard[twoBottom][oneLeft].pieceData.color == attackerColor) {
					return true;
				}
			}
		}
		if (oneRight >= 0) {
			if (chessBoard[twoBottom][oneRight].pieceData.currentPiece == "N") {
				if (chessBoard[twoBottom][oneRight].pieceData.color == attackerColor) {
					return true;
				}
			}
		}
	}

	return false;
}

bool checkIsUnderAttack(int i, int j, boardSquares** chessBoard, string color) {
	if (isUnderKingAttack(i, j, chessBoard, color)) {
		return true;
	}
	else if (isUnderRookAndQweenAttack(i, j, chessBoard, color)) {
		return true;
	}
	else if (isUnderBishopAndQweenAttack(i, j, chessBoard, color)) {
		return true;
	}
	else if (isUnderPawnAttack(i, j, chessBoard, color)) {
		return true;
	}
	else if (isUnderKnightAttack(i, j, chessBoard, color)) {
		return true;
	}

	return false;
}

boardSquares** findUnderAttackSquares(boardSquares** chessBoard) {
	boardSquares** newChessBoard = chessBoard;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			// each sqaure have four options (being under white attack, black attack, both of them, none of them)
			newChessBoard[i][j].isUnderWhiteAttack = checkIsUnderAttack(i, j, chessBoard, "white");
			newChessBoard[i][j].isUnderBlackAttack = checkIsUnderAttack(i, j, chessBoard, "black");
		}
	}

	return newChessBoard;
}

void showNewBoard(boardSquares** chessBoard, string pieceName, int startFile, int startRank, int targetFile, int targetRank, bool isWhiteToMove) {
	string playerMove;
	boardSquares** newChessBoard = chessBoard;
	newChessBoard[targetRank][targetFile].pieceData = chessBoard[startRank][startFile].pieceData;
	newChessBoard[startRank][startFile].pieceData.color = "";
	newChessBoard[startRank][startFile].pieceData.currentPiece = " ";
	if (specialMove == "pawnPromotion") {
		newChessBoard[targetRank][targetFile].pieceData.currentPiece = "Q";
		specialMove = "none";
	}
	else if (specialMove == "enPassant") {
		newChessBoard[startRank][targetFile].pieceData.currentPiece = " ";
		specialMove = "none";
	}
	else if (specialMove == "kingSideCastle") {
		newChessBoard[startRank][5].pieceData = chessBoard[startRank][7].pieceData;
		newChessBoard[startRank][7].pieceData.currentPiece = " ";
		newChessBoard[startRank][7].pieceData.color = "";
		specialMove = "none";
	}
	else if (specialMove == "qweenSideCastle") {
		newChessBoard[startRank][3].pieceData = chessBoard[startRank][0].pieceData;
		newChessBoard[startRank][0].pieceData.currentPiece = " ";
		newChessBoard[startRank][0].pieceData.color = "";
		specialMove = "none";
	}
	// preparation of new round
	newChessBoard = findUnderAttackSquares(newChessBoard);
	checkStruct checkStatus = playerCheckStatus(newChessBoard);
	//system("CLS");
	cout << "\t" << showBoard(newChessBoard, !isWhiteToMove);
	continueTheGame(newChessBoard, !isWhiteToMove, checkStatus);
}

bool isLegalMoveForP(boardSquares** chessBoard, int startFile, int startRank, int targetFile, int targetRank) {
	string pieceColor = chessBoard[startRank][startFile].pieceData.color;

	// when pawn is going straight
	if (startFile == targetFile) {
		if (chessBoard[targetRank][targetFile].pieceData.currentPiece == " ") {
			// white pawn
			if (pieceColor == "white") {
				// white pawns always move to the top of the board
				if (targetRank > startRank) {
					// in the first move each pawn have to choice moving one square or two square
					if (startRank == 1 && ((targetRank - startRank) <= 2)) {
						return true;

					}
					// but after moving from first location pawns can only move on sqaure
					else if (startRank != 1 && ((targetRank - startRank) == 1)) {
						// this condition check is pawn going to be qween?
						if (startRank == 6) {
							specialMove = "pawnPromotion";
						}
						return true;
					}
				}
			}
			// black pawn
			else {
				// black pawns always move to the bottom of the board
				if (targetRank < startRank) {
					if (startRank == 6 && (startRank - targetRank) <= 2) {
						return true;
					}
					// but after moving from first location pawns can only move on sqaure
					else if (startRank != 6 && (startRank - targetRank) == 1) {
						// this condition check is pawn going to be qween?
						if (startRank == 1) {
							specialMove = "pawnPromotion";
						}
						return true;
					}
				}
			}
		}
	}
	// when pawn is attacking to other pieces
	else if (abs(targetFile - startFile) == 1 && abs(targetRank - startRank) == 1) {
		if (chessBoard[targetRank][targetFile].pieceData.currentPiece != " ") {
			// white pawn
			if (pieceColor == "white") {
				if (targetRank > startRank) {
					if (targetRank == 7) {
						specialMove = "pawnPromotion";
					}
					return true;
				}
			}
			// black pawn
			else {
				if (targetRank < startRank) {
					if (targetRank == 0) {
						specialMove = "pawnPromotion";
					}
					return true;
				}
			}
		}
		// the target square is empty (just checking for en passant)
		else {
			//is there any piece in right or left side of selected piece
			if (chessBoard[startRank][targetFile].pieceData.currentPiece == "P") {
				// white pawn
				if (pieceColor == "white") {
					if (targetRank > startRank) {
						if (targetRank == 5) {
							specialMove = "enPassant";
							return true;
						}
					}
				}
				// black pawn
				else {
					if (targetRank < startRank) {
						if (targetRank == 2) {
							specialMove = "enPassant";
							return true;
						}
					}
				}
			}
		}
	}

	return false;
}

bool isLegalMoveForR(boardSquares** chessBoard, int startFile, int startRank, int targetFile, int targetRank) {

	// we can only move in a rank or in file in each movement
	if (startRank == targetRank) {
		if (startFile > targetFile) {
			// Rook can not jump and if there is any pieces in the middle of way this move is not legal
			int distance = startFile - targetFile;

			for (int i = 1; i < distance; i++) {
				if (chessBoard[targetRank][i + targetFile].pieceData.currentPiece != " ") {
					return false;
				}
			}

			return true;
		}
		else {
			// Rook can not jump and if there is any pieces in the middle of way this move is not legal
			int distance = targetFile - startFile;

			for (int i = 1; i < distance; i++) {
				if (chessBoard[targetRank][i + startFile].pieceData.currentPiece != " ") {
					return false;
				}
			}

			return true;
		}
	}
	else if (startFile == targetFile) {
		if (startRank > targetRank) {
			// Rook can not jump and if there is any pieces in the middle of way this move is not legal
			int distance = startRank - targetRank;

			for (int i = 1; i < distance; i++) {
				if (chessBoard[i + targetRank][targetFile].pieceData.currentPiece != " ") {
					return false;
				}
			}

			return true;
		}
		else {
			// Rook can not jump and if there is any pieces in the middle of way this move is not legal
			int distance = targetRank - startRank;

			for (int i = 1; i < distance; i++) {
				if (chessBoard[i + startRank][targetFile].pieceData.currentPiece != " ") {
					return false;
				}
			}

			return true;
		}
	}

	return false;
}

bool isLegalMoveForB(boardSquares** chessBoard, int startFile, int startRank, int targetFile, int targetRank) {
	// we can only move in one color (black or white) for all of bishop life time this color would'nt change
	if (chessBoard[startRank][startFile].isWhite == chessBoard[targetRank][targetFile].isWhite) {
		if (abs(targetFile - startFile) == abs(targetRank - startRank)) {
			if (targetRank > startRank) {
				int distance = targetRank - startRank;
				// moving bishop right top
				if (targetFile > startFile) {
					for (int i = 1; i < distance; i++) {
						if (chessBoard[startRank + i][startFile + i].pieceData.currentPiece != " ") {
							return false;
						}
					}

					return true;
				}
				// moving bishop left top
				else {
					for (int i = 1; i < distance; i++) {
						if (chessBoard[startRank + i][startFile - i].pieceData.currentPiece != " ") {
							return false;
						}
					}

					return true;
				}
			}
			else {
				int distance = startRank - targetRank;
				// moving bishop right bottom
				if (targetFile > startFile) {
					for (int i = 1; i < distance; i++) {
						if (chessBoard[startRank - i][startFile + i].pieceData.currentPiece != " ") {
							return false;
						}
					}

					return true;
				}
				// moving bishop left bottom
				else {
					for (int i = 1; i < distance; i++) {
						if (chessBoard[startRank - i][startFile - i].pieceData.currentPiece != " ") {
							return false;
						}
					}

					return true;
				}
			}
		}
	}

	return false;
}

bool isLegalMoveForN(boardSquares** chessBoard, int startFile, int startRank, int targetFile, int targetRank) {
	if (abs(targetFile - startFile) == 2 && abs(targetRank - startRank) == 1) {
		return true;
	}
	else if (abs(targetFile - startFile) == 1 && abs(targetRank - startRank) == 2) {
		return true;
	}

	return false;
}

bool isLegalMoveForK(boardSquares** chessBoard, int startFile, int startRank, int targetFile, int targetRank) {
	// each move for king is right, left, top, bottom, right top, right bottom, left top, left bottom
	const int qweenSideCastleDistance = 3;
	const int kingSideCastleDistance = 2;
	const string pieceColor = chessBoard[startRank][startFile].pieceData.color;

	// king can not go to under attack squares
	// white king
	if (chessBoard[startRank][startFile].pieceData.color == "white") {
		// white king can't go to under black attack squares
		if (chessBoard[targetRank][targetFile].isUnderBlackAttack) {
			return false;
		}
	}
	// black king
	else {
		// black king can't go to under white attack squares
		if (chessBoard[targetRank][targetFile].isUnderWhiteAttack) {
			return false;
		}
	}

	// checking that movement range is just on block
	if (abs(targetRank - startRank) == 1) {
		// it can be just vertical or both (vertical and horizontal)
		if (abs(targetFile - startFile) == 1 || targetFile == startFile) {
			return true;
		}
	}
	else if (abs(targetFile - startFile) == 1) {
		// it can be just horizontal or both (vertical and horizontal)
		if (abs(targetRank - startRank) == 1 || targetRank == startRank) {
			return true;
		}
	}
	// checking for casteling
	else if (abs(targetFile - startFile) == 2) {
		// in castle we just move horizontally
		if (startRank == targetRank) {
			// white king
			if (chessBoard[startRank][startFile].pieceData.color == "white") {
				if (startRank == 0) {
					// king side castling
					if ((targetFile - startFile) == 2) {
						// checking rook status
						pieceData kingSideRook = chessBoard[0][7].pieceData;
						if (kingSideRook.color == "white" && kingSideRook.currentPiece == "R") {
							// we can not castle with pieces which are located among king and rook
							for (int i = 1; i <= kingSideCastleDistance; i++) {
								if (chessBoard[0][startFile + i].pieceData.currentPiece != " ") {
									return false;
								}
							}

							specialMove = "kingSideCastle";
							return true;
						}
					}
					// qween side castling
					else {
						// checking rook status
						pieceData kingSideRook = chessBoard[0][7].pieceData;
						if (kingSideRook.color == "white" && kingSideRook.currentPiece == "R") {
							// we can not castle with pieces which are located among king and rook
							for (int i = 1; i <= qweenSideCastleDistance; i++) {
								if (chessBoard[0][startFile - i].pieceData.currentPiece != " ") {
									return false;
								}
							}

							specialMove = "qweenSideCastle";
							return true;
						}
					}
				}
			}
			// black king
			else {
				if (startRank == 7) {
					// king side castling
					if ((targetFile - startFile) == 2) {
						// checking rook status
						pieceData kingSideRook = chessBoard[7][7].pieceData;
						if (kingSideRook.color == "black" && kingSideRook.currentPiece == "R") {
							// we can not castle with pieces which are located among king and rook
							for (int i = 1; i <= kingSideCastleDistance; i++) {
								if (chessBoard[7][startFile + i].pieceData.currentPiece != " ") {
									return false;
								}
							}

							specialMove = "kingSideCastle";
							return true;
						}
					}
					// qween side castling
					else {
						// checking rook status
						pieceData kingSideRook = chessBoard[7][7].pieceData;
						if (kingSideRook.color == "black" && kingSideRook.currentPiece == "R") {
							// we can not castle with pieces which are located among king and rook
							for (int i = 1; i <= qweenSideCastleDistance; i++) {
								if (chessBoard[7][startFile - i].pieceData.currentPiece != " ") {
									return false;
								}
							}

							specialMove = "qweenSideCastle";
							return true;
						}
					}
				}
			}
		}
	}

	return false;
}

bool isMoveLegal(string pieceName, boardSquares** chessBoard, int startFile, int startRank, int targetFile, int targetRank) {
	bool isLegal;
	if (pieceName == "K") {
		isLegal = isLegalMoveForK(chessBoard, startFile, startRank, targetFile, targetRank);
	}
	else if (pieceName == "Q") {
		isLegal = (isLegalMoveForR(chessBoard, startFile, startRank, targetFile, targetRank) || isLegalMoveForB(chessBoard, startFile, startRank, targetFile, targetRank));
	}
	else if (pieceName == "P") {
		isLegal = isLegalMoveForP(chessBoard, startFile, startRank, targetFile, targetRank);
	}
	else if (pieceName == "R") {
		isLegal = isLegalMoveForR(chessBoard, startFile, startRank, targetFile, targetRank);
	}
	else if (pieceName == "B") {
		isLegal = isLegalMoveForB(chessBoard, startFile, startRank, targetFile, targetRank);
	}
	else if (pieceName == "N") {
		isLegal = isLegalMoveForN(chessBoard, startFile, startRank, targetFile, targetRank);
	}
	return isLegal;
}

checkStruct playerCheckStatus(boardSquares** chessBoard) {
	checkStruct checkStatus;
	checkStatus.isWhiteCheck = false;
	checkStatus.isBlackCheck = false;

	for (int i = 0; i <= 7; i++) {
		for (int j = 0; j <= 7; j++) {
			if (chessBoard[i][j].pieceData.currentPiece == "K") {
				// white king
				if (chessBoard[i][j].pieceData.color == "white") {
					if (chessBoard[i][j].isUnderBlackAttack) {
						// white king under black attack
						checkStatus.isWhiteCheck = true;
					}
				}
				// black king
				else if (chessBoard[i][j].pieceData.color == "black") {
					if (chessBoard[i][j].isUnderWhiteAttack) {
						// black king under white attack
						checkStatus.isBlackCheck = true;
					}
				}
			}
		}
	}

	return checkStatus;
}

bool doesMoveGetPlayerOutOfCheck(boardSquares** chessBoard, int startFile, int startRank, int targetFile, int targetRank, checkStruct checkStatus) {
	// copy the main chessBoard into newChessBoard
	boardSquares** newChessBoard = new boardSquares * [8];
	for (int i = 0; i < 8; i++) {
		newChessBoard[i] = new boardSquares[8];
	}
	for (int i = 0; i <= 7; i++) {
		for (int j = 0; j <= 7; j++) {
			newChessBoard[i][j] = chessBoard[i][j];
		}
	}

	newChessBoard[targetRank][targetFile].pieceData = chessBoard[startRank][startFile].pieceData;
	newChessBoard[startRank][startFile].pieceData.color = "";
	newChessBoard[startRank][startFile].pieceData.currentPiece = " ";
	if (specialMove == "pawnPromotion") {
		newChessBoard[targetRank][targetFile].pieceData.currentPiece = "Q";
		specialMove = "none";
	}
	else if (specialMove == "enPassant") {
		newChessBoard[startRank][targetFile].pieceData.currentPiece = " ";
		specialMove = "none";
	}

	checkStruct checkStatusAfterMove = playerCheckStatus(findUnderAttackSquares(newChessBoard));

	// both teams get out of check with the player move
	if (!checkStatusAfterMove.isBlackCheck && !checkStatusAfterMove.isWhiteCheck) {
		return true;
	}
	// may be one of players that is checked do (discover check) and get itself out of check and make other team checked
	// white plays discover check
	else if (checkStatus.isWhiteCheck && !checkStatusAfterMove.isWhiteCheck) {
		if (checkStatusAfterMove.isBlackCheck) {
			return true;
		}
	}
	// black plays discover check
	else if (checkStatus.isBlackCheck && !checkStatusAfterMove.isBlackCheck) {
		if (checkStatusAfterMove.isWhiteCheck) {
			return true;
		}
	}

	return false;
}

bool isThePiecePinned(boardSquares** chessBoard, int startFile, int startRank, int targetFile, int targetRank, bool isWhiteToMove) {
	// if the player move one of his pieces and make itself check that piece was pinned
	// and player can not move pinned pieces
	// copy the main chessBoard into newChessBoard
	boardSquares** newChessBoard = new boardSquares * [8];
	for (int i = 0; i < 8; i++) {
		newChessBoard[i] = new boardSquares[8];
	}
	for (int i = 0; i <= 7; i++) {
		for (int j = 0; j <= 7; j++) {
			newChessBoard[i][j] = chessBoard[i][j];
		}
	}

	newChessBoard[targetRank][targetFile].pieceData = chessBoard[startRank][startFile].pieceData;
	newChessBoard[startRank][startFile].pieceData.color = "";
	newChessBoard[startRank][startFile].pieceData.currentPiece = " ";
	if (specialMove == "pawnPromotion") {
		newChessBoard[targetRank][targetFile].pieceData.currentPiece = "Q";
	}
	else if (specialMove == "enPassant") {
		newChessBoard[startRank][targetFile].pieceData.currentPiece = " ";
	}
	else if (specialMove == "kingSideCastle") {
		newChessBoard[startRank][5].pieceData = chessBoard[startRank][7].pieceData;
		newChessBoard[startRank][7].pieceData.currentPiece = " ";
		newChessBoard[startRank][7].pieceData.color = "";
	}
	else if (specialMove == "qweenSideCastle") {
		newChessBoard[startRank][3].pieceData = chessBoard[startRank][0].pieceData;
		newChessBoard[startRank][0].pieceData.currentPiece = " ";
		newChessBoard[startRank][0].pieceData.color = "";
	}

	checkStruct checkStatusAfterMove = playerCheckStatus(findUnderAttackSquares(newChessBoard));

	if (isWhiteToMove) {
		if (checkStatusAfterMove.isWhiteCheck) {
			return true;
		}
	}
	else {
		if (checkStatusAfterMove.isBlackCheck) {
			return true;
		}
	}

	return false;
}

void makeMove(string playerMove, boardSquares** chessBoard, bool isWhiteToMove, checkStruct checkStatus) {
	// all string shorter than 11 are not valid for movement syntax
	if (playerMove.length() == 11) {
		// movement syntax (K, k) e1 ==> h4
		string pieceName(1, toupper(playerMove.at(0)));
		int startFile = tolower(playerMove.at(2)) - 97;
		int startRank = playerMove.at(3) - 49;
		int targetFile = tolower(playerMove.at(9)) - 97;
		int targetRank = playerMove.at(10) - 49;

		if (isMoveSyntaxCorrect(pieceName, startFile, startRank, targetFile, targetRank)) {
			if (isPlayerPiece(isWhiteToMove, chessBoard, startFile, startRank)) {
				if (isPieceInLocation(pieceName, chessBoard, startFile, startRank)) {
					if (!isTreason(isWhiteToMove, chessBoard, targetFile, targetRank)) {
						if (isMoveLegal(pieceName, chessBoard, startFile, startRank, targetFile, targetRank)) {
							//checking check status for the kings
							if (!checkStatus.isBlackCheck && !checkStatus.isWhiteCheck) {
								if (!isThePiecePinned(chessBoard, startFile, startRank, targetFile, targetRank, isWhiteToMove)) {
									showNewBoard(chessBoard, pieceName, startFile, startRank, targetFile, targetRank, isWhiteToMove);
								}
								else {
									string errorMessage;
									// moving king in a checked square
									if (pieceName == "K") {
										errorMessage = "\t This move is not legal try again.";
									}
									// moving one of pieces and making king checked after that move
									else {
										errorMessage = "\t The piece you want to move is pinned choose another move.";
									}

									tryAnotherMove(chessBoard, isWhiteToMove, errorMessage, checkStatus);
								}
							}
							else {
								if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, targetFile, targetRank, checkStatus)) {
									showNewBoard(chessBoard, pieceName, startFile, startRank, targetFile, targetRank, isWhiteToMove);
								}
								else {
									string errorMessage;
									// moving king in a checked square
									if (pieceName == "K") {
										errorMessage = "\t This move is not legal try again.";
									}
									// moving one of pieces and doing nothing when king is checked
									else {
										errorMessage = "\t You are checked you should do something about it.";
									}
									tryAnotherMove(chessBoard, isWhiteToMove, errorMessage, checkStatus);
								}
							}
						}
						else {
							string errorMessage = "\t This move is not legal try again.";
							tryAnotherMove(chessBoard, isWhiteToMove, errorMessage, checkStatus);
						}
					}
					else {
						string errorMessage = "\t You can not attack on your own pieces.";
						tryAnotherMove(chessBoard, isWhiteToMove, errorMessage, checkStatus);
					}
				}
				else {
					string errorMessage = "\t There is no " + pieceName + " in " + playerMove.at(2) + playerMove.at(3) + " choose again.";
					tryAnotherMove(chessBoard, isWhiteToMove, errorMessage, checkStatus);
				}
			}
			else {
				string errorMessage = "\t  Please select one of your pieces.";
				tryAnotherMove(chessBoard, isWhiteToMove, errorMessage, checkStatus);
			}
		}
		else {
			string errorMessage = "\tPlease use syntax structure correctly.\n\tExample : K a3 ==> f6\n\tK is piece name, a and f are file names in chess board, 3 and 6 are rank numbers in chess board.";
			tryAnotherMove(chessBoard, isWhiteToMove, errorMessage, checkStatus);
		}
	}
	else {
		string errorMessage = "\tPlease use syntax structure correctly.\n\tExample : K a3 ==> f6\n\tK is piece name, a and f are file names in chess board, 3 and 6 are rank numbers in chess board.";
		tryAnotherMove(chessBoard, isWhiteToMove, errorMessage, checkStatus);
	}
}

int main() {
	boardSquares** chessBoard = findUnderAttackSquares(makeChessBoard());
	string playerMove;

	bool isWhiteToMove = true;
	checkStruct checkStatus = playerCheckStatus(chessBoard);

	cout << "\t" << showBoard(chessBoard, isWhiteToMove);
	continueTheGame(chessBoard, isWhiteToMove, checkStatus);
	cout << "\n";
	cout << "\n";

	return 0;
}