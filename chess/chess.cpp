#include <iostream>
#include <vector>
#include <string>
#include <vector>
#include <stdlib.h>
using namespace std;

// === global data === 
string fileNames[] = { "a", "b", "c", "d", "e", "f", "g", "h" };
//vector<string> names = givePlayerNames();
vector<string> names = { "playerOne", "playerTwo" };

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
		// "" : it means there is no pieces in this sqaure,		
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
			chessBoard[i][j].pieceData.currentPiece = giveFirstPieceArrangment(i, j);
			chessBoard[i][j].pieceData.color = givePiecesColor(i);
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
	system("CLS");
	cout << "\t" << showBoard(newChessBoard, !isWhiteToMove);
	getline(cin, playerMove);
	makeMove(playerMove, newChessBoard, !isWhiteToMove);

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
				cout << "good choice";
				showNewBoard(chessBoard, pieceName, startFile, startRank, targetFile, targetRank, isWhiteToMove);
			}
			else {
				string errorMessage = "\t there is no " + pieceName + " in " + playerMove.at(2) + playerMove.at(3) + " choose again.";
				tryAnotherMove(chessBoard, isWhiteToMove, errorMessage);
			}
		}
		else {
			string errorMessage = "\t  Please select one of your pieces.";
			tryAnotherMove(chessBoard, isWhiteToMove, errorMessage);
		}
		//isMoveLegal(chessBoard, pieceName, startFile, startRank, targetFile, targetRank);
	}
	else {
		string errorMessage = "\tPlease use syntax structure correctly.\n\tExample : K a3 ==> f6\n\tK is piece name, a and f are file names in chess board, 3 and 6 are rank numbers in chess board.";
		tryAnotherMove(chessBoard, isWhiteToMove, errorMessage);
	}
}

int main() {
	boardSquares** chessBoard = makeChessBoard();
	string playerMove;
	//string playerMove = "p e2 ==> e4";

	bool isWhiteToMove = true;

	cout << "\t" << showBoard(chessBoard, isWhiteToMove);
	getline(cin, playerMove);
	makeMove(playerMove, chessBoard, isWhiteToMove);
	cout << "\n";
	cout << "\n";

	return 0;
}
