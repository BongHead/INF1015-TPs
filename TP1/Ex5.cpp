#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;


int generateRandomNumber(int min, int max) {
	int randomNumber = min + (rand() % ((max - min) + 1));
	return randomNumber;
}
double readUserInputDouble() {
	string input;
	double output;
	bool isValidInput = false;
	while (!isValidInput) {
		try {
			cout << "Entrez un reel: ";
			cin >> input;
			output = stod(input);
			isValidInput = true;
		}
		catch (exception& error) {
			// rien faire
		}
	}
	return output;
}

void guessRandomNumberGame(int min, int max) {
	int numGuesses = 0;
	double currentGuess = 0; //ou int
	int correctNum = generateRandomNumber(min, max);
	while (currentGuess != correctNum) {
		currentGuess = readUserInputDouble(); //conversion implicite
		if (currentGuess < correctNum)
			cout << "Trop bas" << endl;
		else if (currentGuess > correctNum)
			cout << "Trop haut" << endl;
		numGuesses++;
	}
	cout << "Bravo! Vous avez reussi en " << numGuesses << " tentatives!" << endl;
}
//faire une fonction qui fait le main au complet

int main() {
	guessRandomNumberGame(0, 1000);
}
