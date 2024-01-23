#include <iostream>
#include <string>
#include <array>
using namespace std;


int russianMultiplication(int num1, int num2) { //utilisation de unsigned preferable. peut etre static
	int result = 0;
	while (num1 > 0) {
		if (num1 & 1) // ne se divise par par 2
			result += num2;
		num2 <<= 1;	// multiplication entiere par 2
		num1 >>= 1; // division entiere par 2
	}
	return result;
}
struct Tests { // ???
	int num1;
	int num2;
	int expectedResult;
};

bool testRussianMultiplication(int num1, int num2) {
	int directResult = num1 * num2;
	int russianResult = russianMultiplication(num1, num2);
	return (directResult == russianResult);
}

/*
int main() {

	cout << russianMultiplication(37,129) << endl;




	return 0;
}
*/