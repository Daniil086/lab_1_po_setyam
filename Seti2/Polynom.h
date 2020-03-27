#pragma once
#include <vector>
#include <iostream>
using namespace std;
class Polynom {
private:
	int max_deg;
	vector<int> polynom;
public:
	Polynom(vector<int> other);
	Polynom(int maxdeg, bool rand);
	Polynom(int maxdeg, int p);
	~Polynom();
	Polynom& mod(const Polynom& modul);
	
	int getMaxDeg();
	int getWeight();
	vector<int> getVector();
	void correctDeg();

	Polynom& operator+(const Polynom& other) const;
	Polynom& operator*(const Polynom& other) const;
	Polynom& operator=(const Polynom& other);
	bool operator==(const Polynom& other) const;

	friend ostream& operator << (ostream& out, Polynom& other);
};