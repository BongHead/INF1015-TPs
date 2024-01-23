#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;



void removeSpaces(string &text) {
	for (int i = 0; i < text.length(); i++) {
		if (text[i] == ' ')
			text.erase(i, 1);
	}
}

char checkWinner(vector<vector<char>> board) {
	char winner;

	for (int i = 0; i < 3; i++) { // lignes
		if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
			winner = board[i][0];
			return winner;
		}
	}
	
	for (int i = 0; i < 3; i++) { // colonnes
		if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
			winner = board[0][i];
			return winner;
		}
	}

	if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) { // 1e diagonale
		winner = board[0][0];
		return winner;
	}

	if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) // 2e diagonale
	{
		winner = board[0][2];
		return winner;
	}

	return 'n';
}

void showTicTacToeWinner(string fileName) {
	ifstream file;
	string text;
	vector<vector<char>> board(3);
	file.open(fileName);
	int lines = 0;
	while (getline(file, text)) {
		removeSpaces(text);
		for (char letter : text) {
			board[lines].push_back(letter);
		}
		lines++;
	}
	file.close();

	char winner = checkWinner(board);
	switch (winner) {

		case 'x':
			cout << "Le joueur 1 gagne." << endl;
			break;
		case 'o':
			cout << "Le joueur 2 gagne." << endl;
			break;
		case 'n':
			cout << "Egalite" << endl;
			break;
		default:
			break;
	}
}


void problem6() {
	string userInput;
	cout << "Entrez le nom d'un fichier: ";
	cin >> userInput; //on assume que le fichier est valide
	cout << endl;
	showTicTacToeWinner(userInput);
}