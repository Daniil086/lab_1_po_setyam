#include <iostream>
#include <vector>
#include "Polynom.h"
#include "Decoder.h"
using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");
	// ���� ��������� ������
	vector<int> g1 = { 1, 0, 1, 1 };
    int r = 3, 
		l, n;
    double E;
	Polynom gx(g1);
	Polynom rx(gx.getMaxDeg(), false);
	cout << "������� ����� ���������� ������������������: ";
	cin >> l;
	cout << endl;
	n = l + r;
	cout << "������� ��������� �������� �: ";
	cin >> E;
	cout << endl;
    cout << "max ������� ���������� g(x): " << r << endl;;
	cout << "������� g(x) � GF(2): " << gx;
    cout << "; l = " << l << ", n = " << n << ", E = " << E << endl;

	//������ ����������� ������ ������������� c ������� ������������� �������������
	int p = 0;
	int Ne, N = (int)(9 / (4 * E * E));
	vector<int> tmp, a(n), b(n), c(r), c2(r);
	//10 ������������ ������ �� 0 �� 1
	for (size_t i = 0; i < 10; i++) {
		p += 10;
		Ne = 0;
		//���������� N ����� ���������
		for (size_t j = 0; j < N; j++) {
			Polynom mx(l - 1, true); //��������� ���������
			Polynom cx = mx * rx;
			cx = cx.mod(gx); 
			Polynom ax = mx * rx + cx; //�������������� ���������
			Polynom ex(n - 1, p); // ��������� ������ ������
			Polynom bx = ax + ex;

			//�������������� �������

			//������ ���� � ��������� � ��� ���� ����� ������
			//� �������� � �����������, ������� ��������� �� ��������,
			//������� ����� �������� ���, ������� ������ ����� ������� ����,
			//� ������ �������� �������������� ���� � ���� ������������ ����� � ��������� (b)
			vector<int> tmp = bx.getVector();
			if (tmp.size() <= r) {
				for (size_t k = 0; k < tmp.size(); k++) {
					b[k + b.size() - tmp.size()] = tmp[k];
				}
			}
			else b = tmp;
			vector<int> m(b.size() - r);
			for (size_t k = 0; k < b.size(); k++) {
				if (k < b.size() - r) m[k] = b[k]; //� �������(m) ��������� �������������� ����� ���������(b)
				if (k >= b.size() - r) c[k - (b.size() - r)] = b[k]; //� ������� (�) ��������� ����������� ����� ��������� (b)
			}

			//�������������� ����� (m) �� ��������������� ��������� (b) ����� ���� � �����
			Polynom mx2(m);
			Polynom cx2 = mx2 * rx;
			cx2 = cx2.mod(gx);
			Polynom ax2 = mx2 * rx + cx2; //�������������� ���������

			//����� ��������� �������������� ����� � ����� ������������ �����
			tmp = ax2.getVector();
			if (tmp.size() <= r) {
				for (size_t k = 0; k < tmp.size(); k++) {
					a[k + a.size() - tmp.size()] = tmp[k];
				}
			}
			else a = tmp;
			vector<int> m2(a.size() - r);
			for (size_t k = 0; k < a.size(); k++) {
				if (k < a.size() - r) m2[k] = a[k]; //� �������(m2) ��������� �������������� ����� ���������(m)
				if (k >= a.size() - r) c2[k - (a.size() - r)] = a[k]; //� ������� (�2) ��������� ����������� ����� ��������� (m)
			}

			//��������� ����������� ���� � ����� ������
			Polynom e(c);
			Polynom e2(c2);
			if (e == e2) {
				if (ex.getWeight() != 0) {
					Ne++;
				}
			}
			
			//������
			// ��������� ������
			/*if (!decod(bx, gx)) {
				if (ex.getWeight() != 0) {
					Ne++;
				}
			}*/
		}
		double Pe = (double)Ne / (double)N;
		cout << "������ ����: " << Ne << endl;
		cout << "�������� ����: " << N << endl;
		cout << "����������� ������ ������������: " << Pe << " ��� ����������� ������ � ������: " << (double)p/100 << endl;
	}
    return 0;
}