#include <iostream>
#include <string>
#include <array>
using namespace std;

int getUserInput() { //peut etre static
	string input;
	int output;
	bool isValidInput = false;
	while (!isValidInput) {
		try {
			cout << "Entrez un entier: " << endl;
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

void insertInSortedArray(int arr[], int insert, int size) {
	size++;
	int flag = 0;
	for (int i = 0; i < size; i++) {
		if (arr[i] > insert) {

		}
	}

}

int main() {
	int table[100] = {1,3,4,7,9}; //de taille suffisante pour contenir toutes les insertions
	int size = 5;

	cout << size << endl;



	return 0;
}
