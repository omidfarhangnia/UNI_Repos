#include <iostream>
#include <vector>
#include <string>
#include <vector>
using namespace std;

// === structures ===

//struct pieceData {
//	// all pieces are white or black
//	bool isWhite;
//
//};
//
//struct boardSquares
//{
//	// each square has own color (black or white)
//	bool isWhite;
//	// a number between (1 to 8)
//	int rankNum;
//	// one of these letters (a, b, c, d, e, f, g, h)
//	string fileName;
//	// it is the name of piece that is located in that square
//	// it could be one of theses status 
//	// (
//	// "" : it means there is no pieces in this sqaure, 
//	// R : rock,
//	// N : knight,
//	// B : bishop,
//	// Q : queen,
//	// K : king,
//	// P : pawn
//	// )
//	string currentPiece;
//};

struct boardSquares
{
	// each square has own color (black or white)
	bool isWhite;
	// a number between (1 to 8)
	int rankNum;
	// one of these letters (a, b, c, d, e, f, g, h)
	string fileName;
	struct pieceData {
		// we have white and black color for squares which have piece and we have null for others
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
	string color = " ";
	if (i < 2) {
		color = "white";
	}else if (i > 5) {
		color = "black";
	}
	return color;
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

	return stringOfBoard + "\n\t" + borders;
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

void makeMove(string playerMove, boardSquares** chessBoard) {
	// movement syntax (K, k) e1 ==> h4
	string pieceName(1, toupper(playerMove.at(0)));
	int startFile = tolower(playerMove.at(2)) - 97;
	int startRank = playerMove.at(3) - 49;
	int targetFile = tolower(playerMove.at(9)) - 97;
	int targerRank = playerMove.at(10) - 49;

	if (isMoveSyntaxCorrect(pieceName, startFile, startRank, targetFile, targerRank)) {
		cout << "correct syntax";
	}
	else {
		cout << "\tPlease use syntax structure correctly." << endl;
		cout << "\tExample : K a3 ==> f6" << endl;
		cout << "\tK is piece name, a and f are file names in chess board, 3 and 6 are rank numbers in chess board.";
	}
}


int main() {
	string fileNames[] = { "a", "b", "c", "d", "e", "f", "g", "h" };
	boardSquares** chessBoard = makeChessBoard(fileNames);
	//string playerMove;
	string playerMove = "p f2 ==> d8";
	
	//vector<string> names = givePlayerNames();
	vector<string> names = { "playerOne", "playerTwo" };
	bool isWhiteToMove = false;


	string currentPlayerName = isWhiteToMove ? names.at(0) : names.at(1);
	cout << "\n";
	cout << "\t" << currentPlayerName << " is your turn " << endl << endl;
	cout << "\n";
	cout << "\n";
	cout << "\n";
	cout << "\t" << showBoard(chessBoard, fileNames);
	cout << "\n";
	cout << "\n";
	//cin >> playerMove;
	makeMove(playerMove, chessBoard);
	cout << "\n";
	cout << "\n";

	return 0;
}
