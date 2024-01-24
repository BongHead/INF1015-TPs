/*
#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;


double recurrentSeries(double value) { // value = U
	return sqrt(2 + value);
}
double computeLimitSeries(int precision) {
	double currentValue = 1.0;
	double difference;
	const double minDifference = pow(10, -precision);

	do {
		double tempValue = currentValue;
		currentValue = recurrentSeries(currentValue);
		difference = currentValue - tempValue;
	} 
	while (difference > minDifference);
	return currentValue;
}

void problem2() {
	const int precision = 5;
	const int displayDecimals = 20;
	cout << std::fixed << std::showpoint;
	cout << setprecision(displayDecimals) << computeLimitSeries(precision) << endl;
}
*/