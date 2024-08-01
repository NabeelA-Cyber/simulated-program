/*File	Distance
tsp5.txt	1448
tsp8.txt	1909
tsp10.txt	1352
tsp20.txt	194
tsp100.txt	22294
*/

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>
using namespace std;

struct city {
	float x, y;
};

typedef vector<int> State;

vector<city> cities;

bool readFile (string fname) {
	ifstream f;
	city s;
	int xMax, yMax;
	f.open (fname.c_str());
	if (!f) {
		cout << "File " << fname << " open error";
		return false;
	}
	f >> xMax >> yMax;
	do {
		f >> s.x >> s.y;
		cities.push_back(s);
	} while (!f.eof());
	cout << "cities.size= " << cities.size() << endl;
	for (unsigned int i=0; i<cities.size(); i++) {
		cout << "[" << i << "] " << cities[i].x << " " << cities[i].y << "     ";
	}
	cout << endl;
	return true;
}

float EuclidianDistance (city s1, city s2) {
	// Compute Euclidian distance between city locations s1 and s2
	// square of (s1.x - s2.x)^2 + (s1.y - s2.y)^2
	// use pow(x, 2) for x^2
    // put your code here...
	
	float dx = s1.x - s2.x;
	float dy = s1.y - s2.y;
	return sqrt(dx*dx + dy*dy);



	
}

float Eval (State s) {
	// Sum all Eucludian distance between s[i] and s[i+1]
    // put your code here...
	int sum = 0;
	for( int i=0; i<s.size()-1; i++)
		sum += EuclidianDistance(cities[s[i]],cities[s[i+1]]);
	return sum;
}

void print(State s) {
	// display on screen the indices in vector s.
    // put your code here...
	for(int i =0; i<s.size(); i++)
		cout << s[i] << " ";
	cout << endl;
}	

State swap (State s) {
	// Select randomly two indices and swap them.
	// Example: assume s = [0, 1, 3, 5, 4, 2, 8, 7, 6]
	//          swap index 2 and index 6
	//          new s: [0, 1, 8, 5, 4, 2, 3, 7, 6]
    // put your code here...
	int i = rand() % s.size();
	int p;
	do {
		
		p = rand() % s.size();
	} while(p == i);
	int temp = s[i];
	s[i] = s[p];
	s[p] = temp;
	return s;
}

bool exist(State s, int cityIndex) {
	// return true if cityIndex is in s
	// otherwise return false
	// Example: s = [0, 1, 3, 5]
	//          if cityIndex = 5 return true
	//          if cityIndex = 7 return false
    // put your code here...
	for(int i =0; i<s.size(); i++)
		if(cityIndex == s[i]) return true;
	return false;
}

void simulateAnnealing (State & S, float & E) {
	
	S.clear();
	int index;
	// Generate random state (route)
	while (S.size() < cities.size()) {
		while (true) {
			index = rand() % cities.size();
			if (!exist(S, index)) break;
		}
		S.push_back(index);
	}
	E = Eval (S);
    // put rest of your code here...
	float T = 100, E1,alpha = 0.0001;
	State S1;
	// Initial state: generate a random state (route) S
	E = Eval (S);

	while (T>0.0001){
		S1=swap(S);
		E1 = Eval (S1);
		if (E1 < E ){
          S = S1;  E = E1;
		}
		else {
			float rnd = rand() / RAND_MAX;  // get a random number between 0 and 1
			float p = exp(-(E1 - E) / T);
			if (p > rnd) {
				S = S1;  E = E1; 
			}
		}
		T = alpha*T;
	}

	
}

void search () {
	float optimalCost = 10000000, bestCost;
	State optimalState, bestState;
	
	srand(time(0));
	cout << "cities = " << cities.size() << endl;
	for (int i=0; i<10000; i++) { // restart 20000 times:
		// perform search
		simulateAnnealing (bestState, bestCost);
		if (i % 2000 == 0) {
			// display summary every multiple of 2000
			cout << "Iteration " << i << "   ";
			cout << "best cost: " << bestCost << "   ";
			cout << "optimal State: "; print(optimalState);
			cout << "optimal cost: " << optimalCost << endl;
		}
		if (bestCost < optimalCost) {
			// display summary when best Cost is found
			optimalCost = bestCost;
			optimalState = bestState;
			cout << "Iteration " << i << "   ";
			cout << "optimal State: "; print(optimalState);
			cout << "optimal cost: " << optimalCost << endl;
		}
	}
	print(optimalState);
	cout << "optimal cost = " << optimalCost << endl;
}

int main() {
	readFile("tsp100.txt");
	search();
	return 0;
}
	