#include <iostream>
#include <vector>
#include "Polynom.h"
#include "Decoder.h"
using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");
	// Ввод начальных данных
	vector<int> g1 = { 1, 0, 1, 1 };
    int r = 3, 
		l, n;
    double E;
	Polynom gx(g1);
	Polynom rx(gx.getMaxDeg(), false);
	cout << "Введите длину кодируемой последовательности: ";
	cin >> l;
	cout << endl;
	n = l + r;
	cout << "Введите требуемую точность Е: ";
	cin >> E;
	cout << endl;
    cout << "max степень многочлена g(x): " << r << endl;;
	cout << "полином g(x) в GF(2): " << gx;
    cout << "; l = " << l << ", n = " << n << ", E = " << E << endl;

	//Оценка вероятности ошибки декодирования c помощью имитационного моделирования
	int p = 0;
	int Ne, N = (int)(9 / (4 * E * E));
	vector<int> tmp, a(n), b(n), c(r), c2(r);
	//10 вероятностей ошибок от 0 до 1
	for (size_t i = 0; i < 10; i++) {
		p += 10;
		Ne = 0;
		//Выполнение N шагов алгоритма
		for (size_t j = 0; j < N; j++) {
			Polynom mx(l - 1, true); //рандомное сообщение
			Polynom cx = mx * rx;
			cx = cx.mod(gx); 
			Polynom ax = mx * rx + cx; //Закодированное сообщение
			Polynom ex(n - 1, p); // формируем вектор ошибок
			Polynom bx = ax + ex;

			//Альтернативный вариант

			//Работа идет с векторами и тут было много возьми
			//с адресами и указателями, которые корректно не работали,
			//поэтому здесь странный код, который делает очень простую вещь,
			//а именно отделяет информационные биты и биты констрольной суммы у сообщения (b)
			vector<int> tmp = bx.getVector();
			if (tmp.size() <= r) {
				for (size_t k = 0; k < tmp.size(); k++) {
					b[k + b.size() - tmp.size()] = tmp[k];
				}
			}
			else b = tmp;
			vector<int> m(b.size() - r);
			for (size_t k = 0; k < b.size(); k++) {
				if (k < b.size() - r) m[k] = b[k]; //В векторе(m) храниться информациооная часть сообщения(b)
				if (k >= b.size() - r) c[k - (b.size() - r)] = b[k]; //В векторе (с) храниться контрольная сумма сообщения (b)
			}

			//Инофрмационная часть (m) от закодированного сообщения (b) снова идет в кодер
			Polynom mx2(m);
			Polynom cx2 = mx2 * rx;
			cx2 = cx2.mod(gx);
			Polynom ax2 = mx2 * rx + cx2; //Закодированное сообщение

			//Снова отделение информационных битов и битов констрольной суммы
			tmp = ax2.getVector();
			if (tmp.size() <= r) {
				for (size_t k = 0; k < tmp.size(); k++) {
					a[k + a.size() - tmp.size()] = tmp[k];
				}
			}
			else a = tmp;
			vector<int> m2(a.size() - r);
			for (size_t k = 0; k < a.size(); k++) {
				if (k < a.size() - r) m2[k] = a[k]; //В векторе(m2) храниться информациооная часть сообщения(m)
				if (k >= a.size() - r) c2[k - (a.size() - r)] = a[k]; //В векторе (с2) храниться контрольная сумма сообщения (m)
			}

			//Сравнение контрольных сумм и поиск ошибки
			Polynom e(c);
			Polynom e2(c2);
			if (e == e2) {
				if (ex.getWeight() != 0) {
					Ne++;
				}
			}
			
			//Основа
			// Выявление ошибки
			/*if (!decod(bx, gx)) {
				if (ex.getWeight() != 0) {
					Ne++;
				}
			}*/
		}
		double Pe = (double)Ne / (double)N;
		cout << "Ошибок было: " << Ne << endl;
		cout << "Проходов было: " << N << endl;
		cout << "Вероятность ошибки декодированя: " << Pe << " при вероятности ошибки в канале: " << (double)p/100 << endl;
	}
    return 0;
}