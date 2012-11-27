#include<iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;
int main() {
	int poziom, pion;
	cin >> poziom >> pion;
	cout << "4 1 10 960 1 0 0 640 10 0 0 0 640 960 630 0 950 640 960 0 0\n";
	
	cout << poziom * pion << "\n";
	srand ( time(NULL) );



	int maxtobeat=3;
	int start=(960-poziom*70)/2;
	for(int i=0; i<pion; i++) {
		for(int j=0; j<poziom; j++) {
			srand ( time(NULL) );
			cout << start+j*70 << " " << start+i*38+38 << " " << start+j*70+70 << " " << start+i*38 << " " << rand() % maxtobeat << "\n";
		}
	}
	return 0;
}
