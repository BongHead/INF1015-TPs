/*
#include <iostream>
#include <string>
#include <vector>
using namespace std;


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
*/