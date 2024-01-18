#include <iostream>
#include <string>
#include <vector>
using namespace std;

vector<string> splitString(string s) {
	vector<string> v;
	string word = "";
	for (char i : s) {
		if (i == ' ') {
			v.push_back(word);
			word = "";
		}
		else {
			word += i;
		}
	}
	v.push_back(word);
	return v;
}
/*
string getUserInput() {
	string
	cout << "Saisissez une phrase : ";
	getline(cin, user_input);
	cout << endl;
}
*/
int main() {
	string user_input;
	string longest = "";
	double average = 0;

	cout << "Saisissez une phrase : ";
	getline(cin, user_input);
	cout << endl;

	vector<string> v = splitString(user_input);
	string shortest = v[0];

	for (string i : v) {
		if (i.length() > longest.length())
			longest = i;
		if (i.length() < shortest.length())
			shortest = i;
		average += i.length();
	}
	average /= v.size();
	cout << "Le mot le plus court est : " << shortest << endl;
	cout << "Le mot le plus long est : " << longest << endl;
	cout << "La longueur moyenne est : " << average << endl;
	return 0;
}