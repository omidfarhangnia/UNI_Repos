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

struct currentPiecesStruct {
	int numOfWhiteQ = 0;
	int numOfWhiteR = 0;
	int numOfWhiteN = 0;
	int numOfWhiteB = 0;
	int numOfWhiteP = 0;
	int numOfBlackQ = 0;
	int numOfBlackR = 0;
	int numOfBlackN = 0;
	int numOfBlackB = 0;
	int numOfBlackP = 0;
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
bool isLegalMoveForP(boardSquares** chessBoard, int startFile, int startRank, int targetFile, int targetRank);

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

			if (i == 3 && j == 0) {
				chessBoard[i][j].pieceData.currentPiece = "K";
				chessBoard[i][j].pieceData.color = "white";
			}
			//if (i == 0 && j == 7) {
			//	chessBoard[i][j].pieceData.currentPiece = "Q";
			//	chessBoard[i][j].pieceData.color = "white";
			//}

			if (i == 5 && j == 5) {
				chessBoard[i][j].pieceData.currentPiece = "K";
				chessBoard[i][j].pieceData.color = "black";
			}
			if (i == 2 && j == 6) {
				chessBoard[i][j].pieceData.currentPiece = "P";
				chessBoard[i][j].pieceData.color = "white";
			}
			if (i == 5 && j == 3) {
				chessBoard[i][j].pieceData.currentPiece = "B";
				chessBoard[i][j].pieceData.color = "black";
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
		else {
			// attacking the piece that got black team check
			if (chessBoard[a][startFile].pieceData.color == "white") {
				if (checkStatus.isBlackCheck) {
					if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, startFile, a, checkStatus)) {
						return true;
					}
				}
			}
			// attacking the piece that got white team check
			else {
				if (checkStatus.isWhiteCheck) {
					if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, startFile, a, checkStatus)) {
						return true;
					}
				}
			}

			break;
		}
	}
	// checking lower ranks
	for (int a = startRank - 1; a >= 0; a--) {
		if (chessBoard[a][startFile].pieceData.currentPiece == " ") {
			if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, startFile, a, checkStatus)) {
				return true;
			}
		}
		else {
			// attacking the piece that got black team check
			if (chessBoard[a][startFile].pieceData.color == "white") {
				if (checkStatus.isBlackCheck) {
					if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, startFile, a, checkStatus)) {
						return true;
					}
				}
			}
			// attacking the piece that got white team check
			else {
				if (checkStatus.isWhiteCheck) {
					if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, startFile, a, checkStatus)) {
						return true;
					}
				}
			}

			break;
		}
	}
	// checking higher files
	for (int a = startFile + 1; a <= 7; a++) {
		if (chessBoard[startRank][a].pieceData.currentPiece == " ") {
			if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, a, startRank, checkStatus)) {
				return true;
			}
		}
		else {
			// attacking the piece that got black team check
			if (chessBoard[startRank][a].pieceData.color == "white") {
				if (checkStatus.isBlackCheck) {
					if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, a, startRank, checkStatus)) {
						return true;
					}
				}
			}
			// attacking the piece that got white team check
			else {
				if (checkStatus.isWhiteCheck) {
					if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, a, startRank, checkStatus)) {
						return true;
					}
				}
			}

			break;
		}
	}
	// checking lower files
	for (int a = startFile - 1; a >= 0; a--) {
		if (chessBoard[startRank][a].pieceData.currentPiece == " ") {
			if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, a, startRank, checkStatus)) {
				return true;
			}
		}
		else {
			// attacking the piece that got black team check
			if (chessBoard[startRank][a].pieceData.color == "white") {
				if (checkStatus.isBlackCheck) {
					if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, a, startRank, checkStatus)) {
						return true;
					}
				}
			}
			// attacking the piece that got white team check
			else {
				if (checkStatus.isWhiteCheck) {
					if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, a, startRank, checkStatus)) {
						return true;
					}
				}
			}

			break;
		}
	}
	return false;
}

bool canBishopOrQweenSaveKing(boardSquares** chessBoard, int startRank, int startFile, checkStruct checkStatus) {
	// in bishop or qween move (both vertical and horizontal at the same time) we need to check both ranks and files
    // checking higher ranks and higher files
	for (int a = startRank + 1, b = startFile + 1; (a <= 7 && b <= 7); a++, b++) {
		if (chessBoard[a][b].pieceData.currentPiece == " ") {
			if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, b, a, checkStatus)) {
				return true;
			}
		}
		else {
			// attacking the piece that got black team check
			if (chessBoard[a][b].pieceData.color == "white") {
				if (checkStatus.isBlackCheck) {
					if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, b, a, checkStatus)) {
						return true;
					}
				}
			}
			// attacking the piece that got white team check
			else {
				if (checkStatus.isWhiteCheck) {
					if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, b, a, checkStatus)) {
						return true;
					}
				}
			}

			break;
		}
	}
	// checking higher ranks and lower files
	for (int a = startRank + 1, b = startFile - 1; (a <= 7 && b >= 0); a++, b--) {
		if (chessBoard[a][b].pieceData.currentPiece == " ") {
			if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, b, a, checkStatus)) {
				return true;
			}
		}
		else {
			// attacking the piece that got black team check
			if (chessBoard[a][b].pieceData.color == "white") {
				if (checkStatus.isBlackCheck) {
					if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, b, a, checkStatus)) {
						return true;
					}
				}
			}
			// attacking the piece that got white team check
			else {
				if (checkStatus.isWhiteCheck) {
					if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, b, a, checkStatus)) {
						return true;
					}
				}
			}

			break;
		}
	}
	// checking lower ranks and higher files
	for (int a = startRank - 1, b = startFile + 1; (a >= 0 && b <= 7); a--, b++) {
		if (chessBoard[a][b].pieceData.currentPiece == " ") {
			if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, b, a, checkStatus)) {
				return true;
			}
		}
		else {
			// attacking the piece that got black team check
			if (chessBoard[a][b].pieceData.color == "white") {
				if (checkStatus.isBlackCheck) {
					if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, b, a, checkStatus)) {
						return true;
					}
				}
			}
			// attacking the piece that got white team check
			else {
				if (checkStatus.isWhiteCheck) {
					if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, b, a, checkStatus)) {
						return true;
					}
				}
			}

			break;
		}
	}
	// checking lower ranks and lower files
	for (int a = startRank - 1, b = startFile - 1; (a >= 0 && b >= 0); a--, b--) {
		if (chessBoard[a][b].pieceData.currentPiece == " ") {
			if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, b, a, checkStatus)) {
				return true;
			}
		}
		else {
			// attacking the piece that got black team check
			if (chessBoard[a][b].pieceData.color == "white") {
				if (checkStatus.isBlackCheck) {
					if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, b, a, checkStatus)) {
						return true;
					}
				}
			}
			// attacking the piece that got white team check
			else {
				if (checkStatus.isWhiteCheck) {
					if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, b, a, checkStatus)) {
						return true;
					}
				}
			}

			break;
		}
	}

	return false;
}

bool canKnightSaveKing(boardSquares** chessBoard, int startRank, int startFile, checkStruct checkStatus) {
	// each knight can attack 8 squares (2 top right, 2 top left, 2 bottom right, 2 bottom left)
	int twoLeft = startFile + 2,
		twoRight = startFile - 2,
		oneLeft = startFile + 1,
		oneRight = startFile - 1,
		oneTop = startRank - 1,
		oneBottom = startRank + 1,
		twoTop = startRank - 2,
		twoBottom = startRank + 2;

	if (oneTop >= 0) {
		if (twoLeft <= 7) {
			if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, twoLeft, oneTop, checkStatus)) {
				return true;
			}
		}
		if (twoRight >= 0) {
			if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, twoRight, oneTop, checkStatus)) {
				return true;
			}
		}
	}
	if (twoTop >= 0) {
		if (oneLeft <= 7) {
			if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, oneLeft, twoTop, checkStatus)) {
				return true;
			}
		}
		if (oneRight >= 0) {
			if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, oneRight, twoTop, checkStatus)) {
				return true;
			}
		}
	}
	if (oneBottom <= 7) {
		if (twoLeft <= 7) {
			if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, twoLeft, oneBottom, checkStatus)) {
				return true;
			}
		}
		if (twoRight >= 0) {
			if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, twoRight, oneBottom, checkStatus)) {
				return true;
			}
		}
	}
	if (twoBottom <= 7) {
		if (oneLeft <= 7) {
			if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, oneLeft, twoBottom, checkStatus)) {
				return true;
			}
		}
		if (oneRight >= 0) {
			if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, oneRight, twoBottom, checkStatus)) {
				return true;
			}
		}
	}

	return false;
}

bool canPawnSaveKing(boardSquares** chessBoard, int startRank, int startFile, checkStruct checkStatus) {
	int oneTop = startRank + 1,
		twoTop = startRank + 2,
		oneRight = startFile + 1,
		oneLeft = startFile - 1,
		oneBottom = startRank - 1,
		twoBottom = startRank - 2;

	// when pawn is going straight
	if (oneTop <= 7) {
		// going right top
		if (oneRight <= 7) {
			if (isLegalMoveForP(chessBoard, startFile, startRank, oneRight, oneTop)) {
				if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, oneRight, oneTop, checkStatus)) {
					return true;
				}
			}
		}
		// going left top
		if (oneLeft >= 0) {
			if (isLegalMoveForP(chessBoard, startFile, startRank, oneLeft, oneTop)) {
				if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, oneLeft, oneTop, checkStatus)) {
					return true;
				}
			}
		}
		// going one square top
		if (isLegalMoveForP(chessBoard, startFile, startRank, startFile, oneTop)) {
			if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, startFile, oneTop, checkStatus)) {
				return true;
			}
		}
	}
    if (twoTop <= 7) {
		// going two square top
		if (isLegalMoveForP(chessBoard, startFile, startRank, startFile, twoTop)) {
			if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, startFile, twoTop, checkStatus)) {
				return true;
			}
		}
	}
	if (oneBottom >= 0) {
		// going right bottom
		if (oneRight <= 7) {
			if (isLegalMoveForP(chessBoard, startFile, startRank, oneRight, oneBottom)) {
				if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, oneRight, oneBottom, checkStatus)) {
					return true;
				}
			}
		}
		// going left bottom
		if (oneLeft >= 0) {
			if (isLegalMoveForP(chessBoard, startFile, startRank, oneLeft, oneBottom)) {
				if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, oneLeft, oneBottom, checkStatus)) {
					return true;
				}
			}
		}
		// going one square bottom
		if (isLegalMoveForP(chessBoard, startFile, startRank, startFile, oneBottom)) {
			if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, startFile, oneBottom, checkStatus)) {
				return true;
			}
		}
	}
	if (twoBottom >= 0) {
		// going two square bottom
		if (isLegalMoveForP(chessBoard, startFile, startRank, startFile, twoBottom)) {
			if (doesMoveGetPlayerOutOfCheck(chessBoard, startFile, startRank, startFile, twoBottom, checkStatus)) {
				return true;
			}
		}
	}

	return false;
}

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
								cout << kingTargetFile << " " << kingTargetFile << endl;
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
					else if (chessBoard[i][j].pieceData.currentPiece == "N") {
						if (canKnightSaveKing(chessBoard, i, j, checkStatus)) {
							return false;
						}
					}
					else if (chessBoard[i][j].pieceData.currentPiece == "P") {
						if (canPawnSaveKing(chessBoard, i, j, checkStatus)) {
							return false;
						}
					}
				}
			}
		}
	}

	return true;
}

bool canRookOrQweenMove(boardSquares** chessBoard, int startRank, int startFile) {
	// in rook or qween move (only vertical or only horizontal moves) we need to check just ranks or just files
	// checking higher ranks
	for (int a = startRank + 1; a <= 7; a++) {
		if (chessBoard[a][startFile].pieceData.currentPiece == " ") {
			return true;
		}
		else {
			// attacking the opposing team's pieces
			if (chessBoard[a][startFile].pieceData.color != chessBoard[startRank][startFile].pieceData.color) {
				return true;
			}

			break;
		}
	}
	// checking lower ranks
	for (int a = startRank - 1; a >= 0; a--) {
		if (chessBoard[a][startFile].pieceData.currentPiece == " ") {
			return true;
		}
		else {
			// attacking the opposing team's pieces
			if (chessBoard[a][startFile].pieceData.color != chessBoard[startRank][startFile].pieceData.color) {
				return true;
			}

			break;
		}
	}
	// checking higher files
	for (int a = startFile + 1; a <= 7; a++) {
		if (chessBoard[startRank][a].pieceData.currentPiece == " ") {
			return true;
		}
		else {
			// attacking the opposing team's pieces
			if (chessBoard[startRank][a].pieceData.color != chessBoard[startRank][startFile].pieceData.color) {
				return true;
			}

			break;
		}
	}
	// checking lower files
	for (int a = startFile - 1; a >= 0; a--) {
		if (chessBoard[startRank][a].pieceData.currentPiece == " ") {
			return true;
		}
		else {
			// attacking the opposing team's pieces
			if (chessBoard[startRank][a].pieceData.color != chessBoard[startRank][startFile].pieceData.color) {
				return true;
			}

			break;
		}
	}

	return false;
}

bool canBishopOrQweenMove(boardSquares** chessBoard, int startRank, int startFile) {
	// in bishop or qween move (both vertical and horizontal at the same time) we need to check both ranks and files
	// checking higher ranks and higher files
	for (int a = startRank + 1, b = startFile + 1; (a <= 7 && b <= 7); a++, b++) {
		if (chessBoard[a][b].pieceData.currentPiece == " ") {
			return true;
		}
		else {
			// attacking the opposing team's pieces
			if (chessBoard[a][b].pieceData.color != chessBoard[startRank][startFile].pieceData.color) {
				return true;
			}

			break;
		}
	}
	// checking higher ranks and lower files
	for (int a = startRank + 1, b = startFile - 1; (a <= 7 && b >= 0); a++, b--) {
		if (chessBoard[a][b].pieceData.currentPiece == " ") {
			return true;
		}
		else {
			// attacking the opposing team's pieces
			if (chessBoard[a][b].pieceData.color != chessBoard[startRank][startFile].pieceData.color) {
				return true;
			}

			break;
		}
	}
	// checking lower ranks and higher files
	for (int a = startRank - 1, b = startFile + 1; (a >= 0 && b <= 7); a--, b++) {
		if (chessBoard[a][b].pieceData.currentPiece == " ") {
			return true;
		}
		else {
			// attacking the opposing team's pieces
			if (chessBoard[a][b].pieceData.color != chessBoard[startRank][startFile].pieceData.color) {
				return true;
			}

			break;
		}
	}
	// checking lower ranks and lower files
	for (int a = startRank - 1, b = startFile - 1; (a >= 0 && b >= 0); a--, b--) {
		if (chessBoard[a][b].pieceData.currentPiece == " ") {
			return true;
		}
		else {
			// attacking the opposing team's pieces
			if (chessBoard[a][b].pieceData.color != chessBoard[startRank][startFile].pieceData.color) {
				return true;
			}

			break;
		}
	}

	return false;
}

bool canKnightMove(boardSquares** chessBoard, int startRank, int startFile) {
	// each knight can attack 8 squares (2 top right, 2 top left, 2 bottom right, 2 bottom left)
	int twoLeft = startFile + 2,
		twoRight = startFile - 2,
		oneLeft = startFile + 1,
		oneRight = startFile - 1,
		oneTop = startRank - 1,
		oneBottom = startRank + 1,
		twoTop = startRank - 2,
		twoBottom = startRank + 2;

	if (oneTop >= 0) {
		if (twoLeft <= 7) {
			if (chessBoard[oneTop][twoLeft].pieceData.currentPiece == " ") {
				return true;
			}
			else {
				if (chessBoard[oneTop][twoLeft].pieceData.color != chessBoard[startRank][startFile].pieceData.color) {
					return true;
				}
			}
		}
		if (twoRight >= 0) {
			if (chessBoard[oneTop][twoRight].pieceData.currentPiece == " ") {
				return true;
			}
			else {
				if (chessBoard[oneTop][twoRight].pieceData.color != chessBoard[startRank][startFile].pieceData.color) {
					return true;
				}
			}
		}
	}
	if (twoTop >= 0) {
		if (oneLeft <= 7) {
			if (chessBoard[twoTop][oneLeft].pieceData.currentPiece == " ") {
				return true;
			}
			else {
				if (chessBoard[twoTop][oneLeft].pieceData.color != chessBoard[startRank][startFile].pieceData.color) {
					return true;
				}
			}
		}
		if (oneRight >= 0) {
			if (chessBoard[twoTop][oneRight].pieceData.currentPiece == " ") {
				return true;
			}
			else {
				if (chessBoard[twoTop][oneRight].pieceData.color != chessBoard[startRank][startFile].pieceData.color) {
					return true;
				}
			}
		}
	}
	if (oneBottom <= 7) {
		if (twoLeft <= 7) {
			if (chessBoard[oneBottom][twoLeft].pieceData.currentPiece == " ") {
				return true;
			}
			else {
				if (chessBoard[oneBottom][twoLeft].pieceData.color != chessBoard[startRank][startFile].pieceData.color) {
					return true;
				}
			}
		}
		if (twoRight >= 0) {
			if (chessBoard[oneBottom][twoRight].pieceData.currentPiece == " ") {
				return true;
			}
			else {
				if (chessBoard[oneBottom][twoRight].pieceData.color != chessBoard[startRank][startFile].pieceData.color) {
					return true;
				}
			}
		}
	}
	if (twoBottom <= 7) {
		if (oneLeft <= 7) {
			if (chessBoard[twoBottom][oneLeft].pieceData.currentPiece == " ") {
				return true;
			}
			else {
				if (chessBoard[twoBottom][oneLeft].pieceData.color != chessBoard[startRank][startFile].pieceData.color) {
					return true;
				}
			}
		}
		if (oneRight >= 0) {
			if (chessBoard[twoBottom][oneRight].pieceData.currentPiece == " ") {
				return true;
			}
			else {
				if (chessBoard[twoBottom][oneRight].pieceData.color != chessBoard[startRank][startFile].pieceData.color) {
					return true;
				}
			}
		}
	}

	return false;
}

bool canPawnMove(boardSquares** chessBoard, int startRank, int startFile){
	int oneTop = startRank + 1,
		twoTop = startRank + 2,
		oneRight = startFile + 1,
		oneLeft = startFile - 1,
		oneBottom = startRank - 1,
		twoBottom = startRank - 2;

	// when pawn is going straight
	if (oneTop <= 7) {
		// going right top
		if (oneRight <= 7) {
			if (isLegalMoveForP(chessBoard, startFile, startRank, oneRight, oneTop)) {
				return true;
			}
		}
		// going left top
		if (oneLeft >= 0) {
			if (isLegalMoveForP(chessBoard, startFile, startRank, oneLeft, oneTop)) {
				return true;
			}
		}
		// going one square top
		if (isLegalMoveForP(chessBoard, startFile, startRank, startFile, oneTop)) {
			return true;
		}
	}
	if (twoTop <= 7) {
		// going two square top
		if (isLegalMoveForP(chessBoard, startFile, startRank, startFile, twoTop)) {
			return true;
		}
	}
	if (oneBottom >= 0) {
		// going right bottom
		if (oneRight <= 7) {
			if (isLegalMoveForP(chessBoard, startFile, startRank, oneRight, oneBottom)) {
				return true;
			}
		}
		// going left bottom
		if (oneLeft >= 0) {
			if (isLegalMoveForP(chessBoard, startFile, startRank, oneLeft, oneBottom)) {
				return true;
			}
		}
		// going one square bottom
		if (isLegalMoveForP(chessBoard, startFile, startRank, startFile, oneBottom)) {
			return true;
		}
	}
	if (twoBottom >= 0) {
		// going two square bottom
		if (isLegalMoveForP(chessBoard, startFile, startRank, startFile, twoBottom)) {
			return true;
		}
	}

	return false;
}

bool canKingMove(boardSquares** chessBoard, int startRank, int startFile) {
	string kingColor = chessBoard[startRank][startFile].pieceData.color;
	// checking for scape squares
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			int kingTargetRank = startRank + i;
			int kingTargetFile = startFile + j;

			if (kingTargetRank <= 7 && kingTargetRank >= 0) {
				if (kingTargetFile <= 7 && kingTargetFile >= 0) {
					if (chessBoard[kingTargetRank][kingTargetFile].pieceData.currentPiece == " ") {
						if (kingColor == "white") {
							if (!chessBoard[kingTargetRank][kingTargetFile].isUnderBlackAttack) {
								return true;
							}
						}
						else {
							if (!chessBoard[kingTargetRank][kingTargetFile].isUnderWhiteAttack) {
								return true;
							}
						}
					}
					else {
                        // opposing team's pieces
						if (chessBoard[kingTargetRank][kingTargetFile].pieceData.color != kingColor) {
							if (kingColor == "white") {
								if (!chessBoard[kingTargetRank][kingTargetFile].isUnderBlackAttack) {
									return true;
								}
							}
							else {
								if (!chessBoard[kingTargetRank][kingTargetFile].isUnderWhiteAttack) {
									return true;
								}
							}
						}
					}
				}
			}
		}
	}

	return false;
}

bool isStaleMate(boardSquares** chessBoard, bool isWhiteToMove) {
	// stalemate happens in two situations (there is no move for pieces, there is no pieces)
	string currentTeam = (isWhiteToMove ? "white" : "black");

	currentPiecesStruct pieces;
	bool canWhiteMove = false, canBlackMove = false;
	for (int i = 0; i <= 7; i++) {
		for (int j = 0; j <= 7; j++) {
			if (chessBoard[i][j].pieceData.currentPiece != " " && chessBoard[i][j].pieceData.currentPiece != "K") {
				// white pieces
				if (chessBoard[i][j].pieceData.color == "white") {
					if (chessBoard[i][j].pieceData.currentPiece == "Q") { pieces.numOfWhiteQ++; }
					else if (chessBoard[i][j].pieceData.currentPiece == "R") { pieces.numOfWhiteR++; }
					else if (chessBoard[i][j].pieceData.currentPiece == "B") { pieces.numOfWhiteB++; }
					else if (chessBoard[i][j].pieceData.currentPiece == "N") { pieces.numOfWhiteN++; }
					else { pieces.numOfWhiteP++; }
				}
				// black pieces
				else {
					if (chessBoard[i][j].pieceData.currentPiece == "Q") { pieces.numOfBlackQ++; }
					else if (chessBoard[i][j].pieceData.currentPiece == "R") { pieces.numOfBlackR++; }
					else if (chessBoard[i][j].pieceData.currentPiece == "B") { pieces.numOfBlackB++; }
					else if (chessBoard[i][j].pieceData.currentPiece == "N") { pieces.numOfBlackN++; }
					else { pieces.numOfWhiteP++; }
				}
			}
		}
	}

	// checking stalemate status because of lack of enough pieces
	if (!pieces.numOfWhiteQ && !pieces.numOfBlackQ) {
		// no qween on the board
		if (!pieces.numOfWhiteR && !pieces.numOfBlackR) {
			// no rook on the board
			if (!pieces.numOfWhiteP && !pieces.numOfBlackP) {
				// no pawn on the board
				if (!pieces.numOfWhiteB && !pieces.numOfBlackB && !pieces.numOfWhiteN && !pieces.numOfBlackN) {
					// no bishop and no knight on the board
					// there is nothing on the board except (white and black king)
					return true;
				}
				else if((pieces.numOfWhiteB + pieces.numOfWhiteN) <= 1 && (pieces.numOfBlackN + pieces.numOfBlackB) <= 1) {
				    // there is just one bishop or one knight one the board for each team
					// there is no checkmate with this kind of pieces
					return true;
				}
			}
		}
	}

	// checking stalemate status because of inability to move pieces
	for (int i = 0; i <= 7; i++) {
		for (int j = 0; j <= 7; j++) {
			if (chessBoard[i][j].pieceData.currentPiece != " ") {
				if (chessBoard[i][j].pieceData.currentPiece == "Q") {
					if (canRookOrQweenMove(chessBoard, i, j) || canBishopOrQweenMove(chessBoard, i, j)) {
						if (chessBoard[i][j].pieceData.color == "white") {
							canWhiteMove = true;
						}
						else {
							canBlackMove = true;
						}
					}
				}
				else if (chessBoard[i][j].pieceData.currentPiece == "R") {
					if (canRookOrQweenMove(chessBoard, i, j)) {
						if (chessBoard[i][j].pieceData.color == "white") {
							canWhiteMove = true;
						}
						else {
							canBlackMove = true;
						}
					}
				}
				else if (chessBoard[i][j].pieceData.currentPiece == "B") {
					if (canBishopOrQweenMove(chessBoard, i, j)) {
						if (chessBoard[i][j].pieceData.color == "white") {
							canWhiteMove = true;
						}
						else {
							canBlackMove = true;
						}
					}
				}
				else if (chessBoard[i][j].pieceData.currentPiece == "N") {
					if (canKnightMove(chessBoard, i, j)) {
						if (chessBoard[i][j].pieceData.color == "white") {
							canWhiteMove = true;
						}
						else {
							canBlackMove = true;
						}
					}
				}
				else if (chessBoard[i][j].pieceData.currentPiece == "P") {
					if (canPawnMove(chessBoard, i, j)) {
						if (chessBoard[i][j].pieceData.color == "white") {
							canWhiteMove = true;
						}
						else {
							canBlackMove = true;
						}
					}
				}
				else {
					if (canKingMove(chessBoard, i, j)) {
						if (chessBoard[i][j].pieceData.color == "white") {
							canWhiteMove = true;
						}
						else {
							canBlackMove = true;
						}
					}
				}
			}

			if (canWhiteMove && canBlackMove) {
				return false;
			}

		}
	}

	// it is white turn and can't move or it is black turn and can't move
	if (isWhiteToMove) {
		if (!canWhiteMove) {
			return true;
		}
	}
	else {
		if (!canBlackMove) {
			return true;
		}
	}

	// checking stalemate status because of repeating same move for both teams


	return false;
}

void showGameResult(string resultStatus, bool isWhiteWinner) {
	if (resultStatus == "win") {
		cout << "\t *************** WINNER WINNER CHICKEN DINNER ***************" << endl << endl;
		cout << "\t congratulation the " << (isWhiteWinner ? (names[0] + " ( white player )") : (names[1] + " ( black player )")) << " is our winner." << endl << endl;
		cout << "\t ************************************************************" << endl;
	}
	else if (resultStatus == "stalemate") {
		cout << "\t *********************** DRAW ***********************" << endl << endl;
		cout << "\t there is no winner " << names[0] << " : 0.5 , " << names[1] << " : 0.5" << endl << endl;
		cout << "\t ****************************************************" << endl;
	}
}

void continueTheGame(boardSquares** chessBoard, bool isWhiteToMove, checkStruct checkStatus) {
	string playerMove;
	if (!checkStatus.isBlackCheck && !checkStatus.isWhiteCheck) {
		if (isStaleMate(chessBoard, isWhiteToMove)) {
			// in stalemate there is no winner or looser
			showGameResult("stalemate", isWhiteToMove);
		}
		else {
			getline(cin, playerMove);
			makeMove(playerMove, chessBoard, isWhiteToMove, checkStatus);
		}
	}
	else {
		if (isCheckMate(chessBoard, checkStatus)) {
			// winner is the person who played last turn
			showGameResult("win", !isWhiteToMove);
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
	// player can not attack on himself/herself
	if (chessBoard[targetRank][targetFile].pieceData.currentPiece != " ") {
		// black king
		if (checkStatus.isBlackCheck) {
			// black king attack on black pieces (its not legal)
			if (chessBoard[targetRank][targetFile].pieceData.color == "black") {
				return false;
			}
		}
		// white king
		else {
			// white king attack on white pieces (its not legal)
			if (chessBoard[targetRank][targetFile].pieceData.color == "white") {
				return false;
			}
		}
	}
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