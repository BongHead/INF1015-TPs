/**
* Programme qui exécute tous les programmes du TP1.
* \file   Main.cpp
* \author Song Ning Lan et Sheng He Ge
* \date   28 janvier 2024
* Créé le 15 janvier 2024
*/

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>
#include <cstdlib>
#include <fstream>
using namespace std;



vector<string> splitString(string userInput) {
	vector<string> wordsList;
	string word = "";
	for (char letter : userInput) {
		if (letter == ' ') {
			wordsList.push_back(word);
			word = "";
		}
		else {
			word += letter;
		}
	}
	wordsList.push_back(word);
	return wordsList;
}

string getUserInput() {
	string userInput;
	cout << "Saisissez une phrase : ";
	getline(cin, userInput);
	cout << endl;
	return userInput;
}
void computeStatsWords(vector<string> wordsList, string& longest, string& shortest, double& average) {
	for (string word : wordsList) {
		if (word.length() > longest.length())
			longest = word;
		if (word.length() < shortest.length())
			shortest = word;
		average += word.length();
	}
	average /= wordsList.size(); // calcul de moyenne
}

void problem1() {
	double average = 0.0;
	string user_input = getUserInput();

	vector<string> wordsList = splitString(user_input);
	string shortestWord = wordsList[0];
	string longestWord = "";

	computeStatsWords(wordsList, longestWord, shortestWord, average);

	cout << "Le mot le plus court est : " << shortestWord << endl;
	cout << "Le mot le plus long est : " << longestWord << endl;
	cout << "La longueur moyenne est : " << average << " lettres" << endl;
}





double recurrentSeries(double value) { // value = U
	return sqrt(2 + value);
}
double computeLimitSeries(int precision) {
	double currentValue = 1.0;
	double difference;
	const double minDifference = pow(10, -precision);

	do {
		double tempValue = currentValue;
		currentValue = recurrentSeries(currentValue);
		difference = currentValue - tempValue;
	} while (difference > minDifference);
	return currentValue;
}

void problem2() {
	const int precision = 5;
	const int displayDecimals = 20;
	cout << std::fixed << std::showpoint;
	cout << "La limite de la suite apres une precision de " << precision << " chiffres apres la virgule est: ";
	cout << setprecision(displayDecimals) << computeLimitSeries(precision) << endl;
}





int getUserInputInt() {
	string input;
	int output; // donne un warning C4701: potentially uninitialized local variable. L'initialiser à NULL serait comme
				// l'initaliser à 0, donc ce n'est pas tres utile. De toute facon, la boucle while et le try-catch garantit
				// une valeur int assignee à la variable output.
	bool isValidInput = false;
	while (!isValidInput) {
		try {
			cout << "Entrez un entier: ";
			cin >> input;
			output = stoi(input);
			isValidInput = true;
		}
		catch (...) { // afin de ne pas avoir une variable non referencee (exception& error)
			cout << "Entier invalide. Reessayez." << endl;
		}
	}
	return output;
}

void insertInSortedArray(int* arr, int insert, int size) {
	int i = 0;
	for (i = size - 1; i >= 0; i--) {
		if (arr[i] <= insert)
			break;
		arr[i + 1] = arr[i];
	}
	arr[i + 1] = insert;
}

void problem3() {
	int table[20] = { 1,3,4,7,9 }; // de taille suffisante pour contenir toutes les insertions
	const int size = 5;
	int userInput = getUserInputInt();
	insertInSortedArray(table, userInput, size);
	for (int i = 0; i < (size + 1); i++) {
		cout << table[i] << endl;
	}
}





int russianMultiplication(int num1, int num2) { // utilisation de unsigned preferable
	int result = 0;
	while (num1 > 0) {
		if (num1 & 1) // ne se divise par par 2
			result += num2;
		num2 <<= 1;	// multiplication par 2
		num1 >>= 1; // division entiere par 2
	}
	return result;
}

struct Test {
	int num1;
	int num2;
	int expectedResult;
};

void testRussianMultiplication() {
	Test toBeTested[] = {
		{37, 129, 4773},
		{25, 154, 3850},
		{69, 420, 28980}
	};

	int nTestsSuccess = 0;

	for (Test test : toBeTested) {
		int computedResult = russianMultiplication(test.num1, test.num2);
		if (computedResult == test.expectedResult)
			nTestsSuccess++;
	}
	int totalTests = sizeof(toBeTested) / sizeof(toBeTested[0]);
	cout << nTestsSuccess << "/" << totalTests << " tests passent." << endl;
}


void problem4() {
	testRussianMultiplication();
}





int generateRandomNumber(int min, int max) {
	int randomNumber = min + (rand() % ((max - min) + 1));
	return randomNumber;
}

double readUserInputDouble(string display, int min, int max) {
	string input;
	double output;  // donne un warning C4701: potentially uninitialized local variable. L'initialiser à NULL serait comme
					// l'initaliser à 0, donc ce n'est pas tres utile. De toute facon, la boucle while et le try-catch garantit
					// une valeur int assignee à la variable output.
	bool isValidInput = false;
	while (!isValidInput) { //conditions pour delimiter le min et le max
		try {
			cout << display;
			cin >> input;
			output = stod(input);
			isValidInput = (output >= min) && (output <= max);
		}
		catch (...) { // afin de ne pas avoir une variable non referencee (exception& error)
			// ne rien faire
		}
	}
	return output;
}

void guessRandomNumberGame(string display, int min, int max) {
	int nGuesses = 0;
	int currentGuess = 0;
	int correctNumber = generateRandomNumber(min, max);
	while (currentGuess != correctNumber) {
		currentGuess = (int) readUserInputDouble(display, min, max); // expliciter la conversion afin d'enlever le warning lors de la compilation
		if (currentGuess < correctNumber)
			cout << "Trop bas" << endl;
		else if (currentGuess > correctNumber)
			cout << "Trop haut" << endl;
		nGuesses++;
	}
	cout << "Bravo! Vous avez reussi en " << nGuesses << " tentatives!" << endl;
}

void problem5() {
	srand((unsigned int) time(0)); //assure un nouveau seed pour random
	const int minBoundary = 0;
	const int maxBoundary = 1000;
	string wantedDisplay = "Entrez un entier: "; // elimination de nombres magiques

	guessRandomNumberGame(wantedDisplay, minBoundary, maxBoundary);
}





void removeSpaces(string& text) {
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
	int boardSize = 3;
	vector<vector<char>> board(boardSize);
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

int main() {
	problem1();
	problem2();
	problem3();
	problem4();
	problem5();
	problem6();

	return 0;
}
