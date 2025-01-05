#include <iostream>
#include <vector>
#include <string>
#include <vector>
#include <cmath>
#include <stdlib.h>
using namespace std;

// === structures ===

struct boardSquares
{
	// each square has own color (black or white)
	bool isWhite;
	// a number between (1 to 8)
	int rankNum;
	// one of these letters (a, b, c, d, e, f, g, h)
	string fileName;
	struct pieceData {
		// we have white and black color for squares which have piece and we have "" for others
		string color;
		// it is the name of piece that is located in that square
		// it could be one of theses status 
		// (
		// " " : it means there is no pieces in this sqaure,		
		// R : rock,
		// N : knight,
		// B : bishop,
		// Q : queen,
		// K : king,
		// P : pawn
		// )
		string currentPiece;
	};
	pieceData pieceData;

};

struct piecesColor {
	string right;
	string left;
};

// === global data === 
string fileNames[] = { "a", "b", "c", "d", "e", "f", "g", "h" };
//vector<string> names = givePlayerNames();
vector<string> names = { "playerOne", "playerTwo" };
string specialMove = "none";

// === functions ===
void makeMove(string playerMove, boardSquares** chessBoard, bool isWhiteToMove);

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
	}else if (i > 5) {
		color = "black";
	}
	return color;
}

boardSquares** makeChessBoard() {
	boardSquares** chessBoard = new boardSquares*[8];
	
	for (int i = 0; i < 8; i++) {
		chessBoard[i] = new boardSquares[8];
	}

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			chessBoard[i][j].isWhite = (i + j) % 2 == 0 ? false : true;
			chessBoard[i][j].rankNum = i + 1;
			chessBoard[i][j].fileName = fileNames[j];
			//chessBoard[i][j].pieceData.currentPiece = giveFirstPieceArrangment(i, j);
			//chessBoard[i][j].pieceData.color = givePiecesColor(i);

			// test code
			chessBoard[i][j].pieceData.currentPiece = " ";
			chessBoard[i][j].pieceData.color = "";
			if (i == 6 && j == 4) {
				chessBoard[i][j].pieceData.currentPiece = "P";
				chessBoard[i][j].pieceData.color = "black";
			}
			if (i == 4 && j == 5) {
				chessBoard[i][j].pieceData.currentPiece = "P";
				chessBoard[i][j].pieceData.color = "white";
			}
			if (i == 6 && j == 6) {
				chessBoard[i][j].pieceData.currentPiece = "P";
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
	}else {
		symbol.left = "  ";
		symbol.right = "  ";
	}
	return symbol;
}

string showBoard(boardSquares** chessBoard, bool isWhiteToMove) {
	string currentPlayerName = isWhiteToMove ? names.at(0) : names.at(1);
	string guide = "\n\n\t  " + currentPlayerName + " is your turn " + "\n\n\n";
	string borders = "\t     ";
	for(int i = 0; i < 8; i++) {
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
			}
			else {
				colorLine = colorLine + "|     |";
			}
		}
		colorLine = colorLine + "\n\t";

		stringOfBoard = stringOfBoard + colorLine + to_string(i + 1) + " ";
		for (int j = 0; j < 8; j++) {
			//// + is the white sqaure sign and i want to show + in only white
			//string gapLeft = (chessBoard[i][j].pieceData.color ? "+ " : "- ");
			//string gapRight = (chessBoard[i][j].pieceData.color ? " +" : " -");
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

bool isPlayerPiece(bool isWhiteToMove, boardSquares** chessBoard , int startFile, int startRank) {
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

void tryAnotherMove(boardSquares** chessBoard, bool isWhiteToMove, string errorMessage) {
	string playerMove;
	system("CLS");
	cout << "\t" << showBoard(chessBoard, isWhiteToMove);
	cout << errorMessage << endl;
	getline(cin, playerMove);
	makeMove(playerMove, chessBoard, isWhiteToMove);
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
	system("CLS");
	cout << "\t" << showBoard(newChessBoard, !isWhiteToMove);
	getline(cin, playerMove);
	makeMove(playerMove, newChessBoard, !isWhiteToMove);

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
	else if(abs(targetFile - startFile) == 1 && abs(targetRank - startRank) == 1) {
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

bool isMoveLegal(string pieceName, boardSquares** chessBoard, int startFile, int startRank, int targetFile, int targetRank) {
	bool isLegal;
	if (pieceName == "K") {

	}else if (pieceName == "Q") {
		isLegal = true;
	}else if (pieceName == "P") {
		isLegal = isLegalMoveForP(chessBoard, startFile, startRank, targetFile, targetRank);

	}else if (pieceName == "R") {

	}else if (pieceName == "B") {

	}else if (pieceName == "N") {

	}
	return isLegal;
}

void makeMove(string playerMove, boardSquares** chessBoard, bool isWhiteToMove) {
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
						showNewBoard(chessBoard, pieceName, startFile, startRank, targetFile, targetRank, isWhiteToMove);
					}
					else {
						string errorMessage = "\t This move is not legal try again";
						tryAnotherMove(chessBoard, isWhiteToMove, errorMessage);
					}
				}
				else {
					string errorMessage = "\t You can not attack to your own pieces";
					tryAnotherMove(chessBoard, isWhiteToMove, errorMessage);
				}
			}
			else {
				string errorMessage = "\t There is no " + pieceName + " in " + playerMove.at(2) + playerMove.at(3) + " choose again.";
				tryAnotherMove(chessBoard, isWhiteToMove, errorMessage);
			}
		}
		else {
			string errorMessage = "\t  Please select one of your pieces.";
			tryAnotherMove(chessBoard, isWhiteToMove, errorMessage);
		}
	}
	else {
		string errorMessage = "\t Please use syntax structure correctly.\n\tExample : K a3 ==> f6\n\tK is piece name, a and f are file names in chess board, 3 and 6 are rank numbers in chess board.";
		tryAnotherMove(chessBoard, isWhiteToMove, errorMessage);
	}
}

int main() {
	boardSquares** chessBoard = makeChessBoard();
	string playerMove;
	//string playerMove = "p e2 ==> e4";

	bool isWhiteToMove = false;

	cout << "\t" << showBoard(chessBoard, isWhiteToMove);
	getline(cin, playerMove);
	makeMove(playerMove, chessBoard, isWhiteToMove);
	cout << "\n";
	cout << "\n";

	return 0;
}
