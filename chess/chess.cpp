#include <iostream>
#include <vector>
#include <string>
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

// === functions ===

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

string makeFirstPieceArrangment(int i, int j) {
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

boardSquares** makeChessBoard(string fileNames[]) {
	boardSquares** chessBoard = new boardSquares*[8];
	
	for (int i = 0; i < 8; i++) {
		chessBoard[i] = new boardSquares[8];
	}

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			chessBoard[i][j].isWhite = (i + j) % 2 == 0 ? false : true;
			chessBoard[i][j].rankNum = i + 1;
			chessBoard[i][j].fileName = fileNames[j];
			chessBoard[i][j].currentPiece = makeFirstPieceArrangment(i, j);
		}
	}

	return chessBoard;
}

string showBoard(boardSquares** chessBoard, string fileNames[]) {
	string borders = "     ";
	for(int i = 0; i < 8; i++) {
		borders = borders + fileNames[i] + "      ";
	}
	string stringOfBoard = borders + "\n \t  --------------------------------------------------------";

	// i had to use this shape of loop for different between the start point in matrix and chess board
	for (int i = 7; i >= 0; i--) {
		// colorLine is a string containing a basic style for showing black and white squares
		string colorLine = "\n\t  ";
		for (int j = 0; j < 8; j++) {
			if (chessBoard[i][j].isWhite) {
				colorLine = colorLine + "|+++++|";
			}
			else {
				colorLine = colorLine + "|     |";
			}
		}
		colorLine = colorLine + "\n\t";

		stringOfBoard = stringOfBoard + colorLine + to_string(i + 1) + " ";
		for (int j = 0; j < 8; j++) {
			// + is the white sqaure sign and i want to show + in only white
			string gapLeft = (chessBoard[i][j].isWhite ? "+ " : "  ");
			string gapRight = (chessBoard[i][j].isWhite ? " +" : "  ");
			//stringOfBoard = stringOfBoard + "| " + chessBoard[i][j].fileName + "" + to_string(chessBoard[i][j].rankNum) + " |";
			stringOfBoard = stringOfBoard + "|" + gapLeft + chessBoard[i][j].currentPiece + gapRight + "|";
		}
		stringOfBoard = stringOfBoard + " " + to_string(i + 1) + colorLine;
		stringOfBoard = stringOfBoard + "  --------------------------------------------------------\t";
	}



	return stringOfBoard + "\n\t" + borders;
}

int main() {
	string fileNames[] = { "a", "b", "c", "d", "e", "f", "g", "h" };
	boardSquares** chessBoard = makeChessBoard(fileNames);
	
	//vector<string> names = givePlayerNames();
	vector<string> names = { "playerOne", "playerTwo" };
	bool isWhiteToMove = false;


	string currentPlayerName = isWhiteToMove ? names.at(0) : names.at(1);
	cout << "\n";
	cout << "\t" << currentPlayerName << " is your turn " << endl << endl;
	boardSquares member = chessBoard[4][5];
	cout << "\t" << (member.isWhite ? "white" : "black") << "\t" << member.fileName << "\t" << member.rankNum;
	cout << "\n";
	cout << "\n";
	cout << "\n";
	cout << "\t" << showBoard(chessBoard, fileNames);
	cout << "\n";
	cout << "\n";
	cout << "\n";
	cout << "\n";
	cout << "\n";

	return 0;
}
