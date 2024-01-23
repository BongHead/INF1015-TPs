#include <iostream>
#include <string>
#include <vector>
using namespace std;

vector<string> splitString(string userInput) {
	vector<string> wordsList;
	string word = "";
	for (char lettre : userInput) {
		if (lettre == ' ') {
			wordsList.push_back(word);
			word = "";
		}
		else {
			word += lettre;
		}
	}
	wordsList.push_back(word);
	return wordsList;
}

string getUserInput() { //a valider
	string userInput;
	cout << "Saisissez une phrase : ";
	getline(cin, userInput);
	cout << endl;
	return userInput;
}
void ComputeStatsWords(vector<string> wordsList, string &longest, string &shortest, double &sum) { //inline
	for (string word : wordsList) { // on connait le type que contient le vecteur wordList
		if (word.length() > longest.length())
			longest = word;
		if (word.length() < shortest.length())
			shortest = word;
		sum] += word.length();
	}
	sum /= wordsList.size(); //calcul de moyenne
}
/*
int main() {//tres clean
	double average = 0.0; //oof
	string user_input = getUserInput();

	vector<string> wordsList = splitString(user_input);
	string shortestWord = wordsList[0];
	string longestWord = "";

	ComputeStatsWords(wordsList, longestWord, shortestWord, average);

	cout << "Le mot le plus court est : " << shortestWord << endl;
	cout << "Le mot le plus long est : " << longestWord << endl;
	cout << "La longueur moyenne est : " << average << " lettres" << endl;
	return 0;
}*/