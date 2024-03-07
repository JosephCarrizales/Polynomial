/*
   Polynomial.h - Header file for Polynomial arithmetic operations
   Author: Joseph Carrizales
   Project Description: This project implements polynomial arithmetic operations using C++ classes and templates.
   It defines two classes: Monomial and Polynomial, representing individual terms and collections of terms in polynomials, respectively.
   The Polynomial class provides functions for addition, subtraction, multiplication, evaluation, and comparison of polynomials.
   This project demonstrates the use of C++ classes, templates, operator overloading, and STL containers.
*/
#include <iostream>
#include <list>
#include "Polynomial.h"
using namespace std;

int main() {
	typedef Monomial<int> mono;
	typedef Polynomial<int> poly;
	Polynomial<int> p1;
	Polynomial<int> p2;
	Polynomial<int> p3;
	Polynomial<int> p4;
	Polynomial<int> p5;
	Polynomial<int> p6;

	cout << "\n*** P07 Polynomial Driver Sample Run ***\n\n";

	cout << "LINE 15 * Entry for p1: 1 2 3 4 5 6 0 =>  ";
	cin >> p1;
	cout << "p1: ";
	cout << p1;

	cout << "\nLINE 20 * Entry for p2: 1 2 3 4 5 6 0 =>  ";
	cin >> p2;
	cout << "p2: ";
	cout << p2;

	cout << "\nLINE 25 * (p1 == p2) *: " << boolalpha << (p1 == p2) << endl;

	p1 += p2;
	cout << "\nLINE 28 * p1 += p2 *: ";
	cout << p1;

	p1 -= p2;
	cout << "\nLINE 32 * p1 -= p2 *: ";
	cout << p1;

	p1 = mono(-10, 7);
	cout << "\nLINE 36 * p1 = mono(-10, 7) *: ";
	cout << p1;

	p1 -= mono(-10, 7);
	cout << "\nLINE 40 * p1 -= mono(-10, 7) *: ";
	cout << p1;

	p1 += mono(-10, 7);
	cout << "\nLINE 44 * p1 += mono(-10, 7) *: ";
	cout << p1;

	p1 = poly(mono(-10, 7)) - poly(mono(-10, 7)) + poly(mono(-10, 7));
	cout << "\nLINE 48 * p1 = poly(mono(-10, 7)) - poly(mono(-10,7)) + poly(mono(-10,7)) *: ";
	cout << p1;

	p1 *= Monomial<int>(2, 2);

	cout << "\nLINE 53 * Entry for p3: 1 2 5 1 1 0 0 =>  ";
	cin >> p3;
	cout << "p3: " << p3;

	cout << "\nLINE 57 * Entry for p4: 3 2 -10 1 15 0 0 =>  ";
	cin >> p4;
	cout << "p4: " << p4;

	p3 *= p4;
	cout << "\nLINE 62 * p3 *= p4 *: " << p3;

	cout << "\nLINE 64 * (p3 == p4) *: " << boolalpha << (p3 == p4) << endl;

	p5 = move(p2);
	cout << "\nLINE 67 * p5 = move(p2) *: " << p5;


	cout << "\nLINE 71 * Entry for p6: 1 2 -3 4 -1 6 0 =>   ";
	cin >> p6;
	cout << "p6: ";
	cout << p6;

	int result = p6.evaluate(2);
	cout << "\nLINE 76 * result = p6.evaluate(2) *: " << result << endl;

	return 0;

}