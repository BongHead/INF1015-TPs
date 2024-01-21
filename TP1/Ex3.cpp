#include <iostream>
#include <string>
#include <array>
using namespace std;

int getUserInt() { //peut etre static
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

void insertInSortedArray(int* arr, int insert, int size) { // ne marche pas avec 0
	for (int i = size - 1; i >= 0; i--) {
		if (arr[i] > insert)
			arr[i + 1] = arr[i];
		else {
			arr[i + 1] = insert;
			break; // voir guide de codage pls
		}
	}
}
/*
int main() {
	int table[20] = { 1,3,4,7,9 }; // de taille suffisante pour contenir toutes les insertions
	int size = 5;
	int userInput = getUserInt();
	insertInSortedArray(table, userInput, size);
	for (auto i : table) {
		cout << i << endl;
	}
	return 0;
}
*/