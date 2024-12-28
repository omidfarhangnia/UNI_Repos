#include <iostream>
#include <vector>
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

boardSquares** makeChessBoard(string fileNames[]) {
	boardSquares** chessBoard = new boardSquares*[8];
	
	for (int i = 0; i < 8; i++) {
		chessBoard[i] = new boardSquares[8];
	}

	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			chessBoard[i][j].isWhite = (i + j) % 2 == 0 ? false : true;
			chessBoard[i][j].rankNum = i + 1;
			chessBoard[i][j].fileName = fileNames[j];
		}
	}

	return chessBoard;

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
	//cout << "\t" << boardStatus();
	boardSquares member = chessBoard[4][5];
	cout << "\t" << (member.isWhite ? "white" : "black") << "\t" << member.fileName << "\t" << member.rankNum;
	cout << "\n";

	return 0;
}
