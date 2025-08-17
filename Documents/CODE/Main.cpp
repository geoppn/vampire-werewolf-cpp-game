#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>
#include <windows.h> // Library for delay
#include "Interface.h"

using namespace std;
int main() {
	HANDLE color = GetStdHandle(STD_OUTPUT_HANDLE); // Handle for colours
	srand(time(NULL));
	SetConsoleTextAttribute(color, 15); // Change handle value for colour 4=V (dark red) 3=W (dark blue)
	PrintMM();
	char a; // Action
	do {
		cout << "> ";
		cin >> a;
		a = tolower(a); // Convert input to lowercase
		if (a == 'b') {
			StartGame();
			continue;
		}
		else if (a == 'x') {
			TerminateGame();
			return 0;
		}
	} while (a != 'b'&& a != 'x'); // Wrong input
	return 0;
}