#include "Polynom.h"
#include <random>
using namespace std;

// Создание полинома по вектору
Polynom::Polynom(vector<int> other) {
	int count = 0;
	for (size_t i = 0; i < other.size(); i++) {
		this->polynom.push_back(other.at(i));
	}
	for (size_t i = 0; i < other.size(); i++) {
		if (this->polynom[i] == 0) count++;
	}
	if(this->polynom.size() != count) correctDeg();
	this->max_deg = polynom.size() - 1;
}

// Создание полинома с случайными числами rand == 1
// Создание полинома вида x^deg rand == 0
Polynom::Polynom(int maxdeg, bool rand) {
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dist(0, 1);

	if(rand) {
		for (size_t i = 0; i <= maxdeg; i++) {
			int k = dist(gen);
			this->polynom.push_back(k);
		}
		correctDeg();
	}
	else {
		this->max_deg = maxdeg;
		polynom.push_back(1);
		for (size_t i = 1; i <= maxdeg; i++) {
			polynom.push_back(0);
		}
	}
}

// Создание вектора ошибок
Polynom::Polynom(int maxdeg, int p) {
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dist(0, 100);

	this->max_deg = maxdeg;
	for (size_t i = 0; i <= maxdeg; i++) {
		int q = dist(gen);
		if (q <= p) {
			this->polynom.push_back(1);
		}
		else {
			this->polynom.push_back(0);
		}
	}
	correctDeg();
}

Polynom::~Polynom() {
	this->polynom.clear();
}

Polynom& Polynom::mod(const Polynom& modul) {
	Polynom diff(this->polynom);
	Polynom mod(modul.polynom);
	mod.correctDeg();
	// это сдвиг делителя до большей степени
	int shift = this->max_deg - mod.max_deg;
	while (shift >= 0) {
		vector<int> tmp;
		tmp.push_back(1);
		for (size_t i = 0; i < shift; i++) {
			tmp.push_back(0);
		}
		// На сколько надо умножить делитель до максимальной степени
		Polynom tmp2(tmp);
		// Делитель со сдвигом
		Polynom sub = mod * tmp2;
		//Сложение по модулю 2, а вообще процесс деления на одном шаге
		diff = diff + sub;
		diff.correctDeg();
		//Новый сдвиг для следующего шага
		shift = diff.max_deg - mod.max_deg;
	}

	Polynom* res = new Polynom(diff.polynom);
	res->correctDeg();
	return *res;
}

int Polynom::getMaxDeg() {
	return this->max_deg;
}

int Polynom::getWeight() {
	int weight = 0;
	for (size_t i = 0; i <= this->max_deg; i++) {
		if (this->polynom[i] == 1) weight++;
	}
	return weight;
}
vector<int> Polynom::getVector() {
	return this->polynom;
}

//Я работаю с векторами вида 1 0 1 0
//То есть первый элемент вектора всегда должен быть равен 1
//Если вдруг вектор имеет вид 0 0 1 1 0, то эта функция переделает его 
//в 1 1 0. При этом будет уменьшен размер вектора
void Polynom::correctDeg() {
	auto begin = this->polynom.cbegin();
	size_t i = 0;
	while (this->polynom[i] == 0 && i < this->polynom.size() - 1) i++;
	this->polynom.erase(begin, begin + i);
	this->max_deg = polynom.size() - 1;
}

Polynom& Polynom::operator=(const Polynom& other) {
	if (this != &other) {
		this->max_deg = other.max_deg;
		this->polynom.clear();
		for (int i = 0; i <= this->max_deg; i++) {
			this->polynom.push_back(other.polynom.at(i));
		}
	}
	this->correctDeg();
	return *this;
}

Polynom& Polynom::operator+(const Polynom& other) const {
	vector<int> oth;
	vector<int> res;
	if (this->polynom.size() >= other.polynom.size()) {
		res = this->polynom;
		oth = other.polynom;
	}
	else {
		res = other.polynom;
		oth = this->polynom;
	}
	for (int i = 0; i < oth.size(); i++) {
		res.at(res.size() - oth.size() + i) = (res.at(res.size() - oth.size() + i) + oth.at(i)) % 2;
	}
	Polynom* Res = new Polynom(res);
	Res->correctDeg();
	return *Res;
}

Polynom& Polynom::operator*(const Polynom& other) const {
	vector<int> res(this->max_deg + other.max_deg + 1);
	Polynom* Res = new Polynom(res);
	for (size_t i = 0; i <= this->max_deg; i++) {
		for (size_t j = 0; j <= other.max_deg; j++) {
			Res->polynom[i + j] = (Res->polynom[i + j] + this->polynom[i] * other.polynom[j]) % 2;
		}
	}
	Res->correctDeg();
	return *Res;
}

bool Polynom::operator==(const Polynom& other) const {
	if (this->max_deg == other.max_deg) {
		for (size_t i = 0; i < this->max_deg; i++) {
			if (this->polynom[i] != other.polynom[i]) {
				return false;
			}
		}
		return true;
	}
	else return false;
}

ostream& operator << (ostream& out, Polynom& other) {
	for (int i = 0; i < other.max_deg + 1; i++) {
		int number = other.polynom.at(i);
		if (number != 0 && i != other.max_deg) out << "x^" << other.max_deg - i;
		if (i < other.max_deg) {
			int tmp = other.polynom.at(i + 1);
			if (tmp != 0) out << " + ";
		}
		if (number != 0 && i == other.max_deg) out << "1 ";
	}
	return out;
}