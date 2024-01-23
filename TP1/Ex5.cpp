#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;


int generateRandomNumber(int min, int max) {
	int randomNumber = min + (rand() % ((max - min) + 1));
	return randomNumber;
}
double readUserInputDouble(string display, int min, int max) {
	string input;
	double output;
	bool isValidInput = false;
	while (!isValidInput) { //conditions pour delimiter le min et le max
		try {
			cout << display;
			cin >> input;
			output = stod(input);
			isValidInput = (output >= min) && (output <= max);
		}
		catch (exception& error) {
			// rien faire
		}
	}
	return output;
}

void guessRandomNumberGame(string display, int min, int max) {
	int nGuesses = 0;
	int currentGuess = 0;
	int correctNumber = generateRandomNumber(min, max);
	while (currentGuess != correctNumber) {
		currentGuess = readUserInputDouble(display ,min, max); // conversion implicite
		if (currentGuess < correctNumber)
			cout << "Trop bas" << endl;
		else if (currentGuess > correctNumber)
			cout << "Trop haut" << endl;
		nGuesses++;
	}
	cout << "Bravo! Vous avez reussi en " << nGuesses << " tentatives!" << endl;
}

void problem5() {
	srand(time(0)); //assure un nouveau seed pour random
	const int minBoundary = 0;
	const int maxBoundary = 1000;
	string wantedDisplay = "Entrez un entier: "; // elimination de nombres magiques

	guessRandomNumberGame(wantedDisplay, minBoundary, maxBoundary);
}