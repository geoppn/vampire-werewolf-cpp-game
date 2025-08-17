#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <string>
#include <windows.h> // Library for delay
#include "Interface.h"

using namespace std;
int main() {
	srand(time(NULL));
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