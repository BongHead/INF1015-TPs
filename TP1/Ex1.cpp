/*#include <iostream>
#include <string>
#include <vector>
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
void computeStatsWords(vector<string> wordsList, string &longest, string &shortest, double &average) {
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
*/