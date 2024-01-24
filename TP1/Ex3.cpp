/*
#include <iostream>
#include <string>
using namespace std;

int getUserInputInt() {
	string input;
	int output;
	bool isValidInput = false;
	while (!isValidInput) {
		try {
			cout << "Entrez un entier: ";
			cin >> input;
			output = stoi(input);
			isValidInput = true;
		}
		catch (exception &error) {
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
*/