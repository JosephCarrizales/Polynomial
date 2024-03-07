/*
   Polynomial.h - Header file for Polynomial arithmetic operations
   Author: Joseph Carrizales
   Project Description: This project implements polynomial arithmetic operations using C++ classes and templates.
   It defines two classes: Monomial and Polynomial, representing individual terms and collections of terms in polynomials, respectively.
   The Polynomial class provides functions for addition, subtraction, multiplication, evaluation, and comparison of polynomials.
   This project demonstrates the use of C++ classes, templates, operator overloading, and STL containers.
*/

#ifndef __POLYNOMIAL_H_
#define __POLYNOMIAL_H_
#include <iostream>
#include <list>
using namespace std;

///////////////////////////////////////////////////////// Monomial Class
// Monomial - class that creates the terms that get inserted in a Polynomial

template <typename NumT>
class Monomial {
public:
	Monomial<NumT>(NumT c = 0, int d = 0) : coeff(c), expo(d) { };
	NumT coefficient(void) const { return coeff; };
	int degree(void) const { return expo; };
	void assign_coefficient(const NumT c) { coeff = c; };
	void assign_degree(const int d) { expo = d; };

	bool operator==(const Monomial<NumT>& m) const {
		return (coeff == m.coeff && expo == m.expo);
	}

	bool operator!=(const Monomial<NumT>& m) const {
		return (coeff != m.coeff || expo != m.expo);
	}
private:
	NumT coeff; 
	int	expo;
};

//////////////////////////////////////////////////////Polynomial Class
template <typename NumberType>
class Polynomial
{
public:
	// Default constructor 
	Polynomial<NumberType>(NumberType c = 0, int d = 0) {
		const Monomial<NumberType> m(c, d);
		term_list.push_back(m); //creates at least one monomial
		number_of_terms = 1;
		highest_degree = d;
	}

	// Type conversion construct 
	Polynomial<NumberType>(const Monomial<NumberType>& m) {
		term_list.push_back(m);
		number_of_terms = 1;
		highest_degree = m.degree();
	}

	// Destructor - use default destructors and list's destructor
	~Polynomial<NumberType>() { term_list.clear(); }

	// Copy Constructor
	Polynomial<NumberType>(const Polynomial<NumberType>& rhs)
		: term_list(rhs.term_list),
		number_of_terms(rhs.number_of_terms),
		highest_degree(rhs.highest_degree) {}

	int gethighestdegree() const { return highest_degree; }

	
	const Polynomial<NumberType>& operator=(const Polynomial<NumberType>& rhs) {// copy assignment
		term_list = rhs.term_list;
		number_of_terms = rhs.number_of_terms;
		highest_degree = rhs.highest_degree;

		return *this;
	}
	const Polynomial<NumberType>& operator=(Polynomial<NumberType>&& rhs) {// move assignment
		term_list = rhs.term_list;
		number_of_terms = rhs.number_of_terms;
		highest_degree = rhs.highest_degree;

		rhs.number_of_terms = 0;
		rhs.highest_degree = 0;

		return *this;
	}

	// ADDITION
	Polynomial<NumberType> operator+=(const Monomial<NumberType>& m) { //1
		insert_in_poly(*this, m);
		return *this;
	}
	Polynomial<NumberType> operator+=(const Polynomial<NumberType>& rhs) { //2
		for (auto& term : rhs.term_list) {
			*this += term;
		}
		return *this;
	}
	const Polynomial<NumberType> operator+ (const Monomial<NumberType>& m)const {//3
		return Polynomial(*this) += m;
	}
	const Polynomial<NumberType> operator+ (const Polynomial<NumberType>& rhs) const {//4
		return Polynomial(*this) += rhs;
	}
	// SUBTRACTION
	Polynomial<NumberType> operator-=(const Monomial<NumberType>& m) {
		Monomial<NumberType> temp(m);
		temp.assign_coefficient(m.coefficient() * -1);
		insert_in_poly(*this, temp);

		return *this;
	}
	Polynomial<NumberType> operator-=(const Polynomial<NumberType>& rhs) {
		for (auto& term : rhs.term_list) {
			*this -= term;
		}
		return *this;
	}
	const Polynomial<NumberType> operator- (const Monomial<NumberType>& m)const {
		return Polynomial(*this) -= m;
	}
	const Polynomial<NumberType> operator- (const Polynomial<NumberType>& rhs) const {
		return Polynomial(*this) -= rhs;
	}

	// MULTIPLICATION
	Polynomial<NumberType> operator*=(const Monomial<NumberType>& m) {
		Monomial<NumberType> temp(m);
		auto tempPoly = Polynomial<NumberType>(temp.coefficient(), temp.degree());

		*this *= tempPoly;
		return *this;
	}
	Polynomial<NumberType> operator*=(const Polynomial<NumberType>& rhs) {
		Polynomial<NumberType> tempPoly;
		Monomial<NumberType> tempMoly;

		for (auto term : term_list) {
			for (auto i : rhs.term_list) {
				tempMoly.assign_coefficient(term.coefficient() * i.coefficient());
				tempMoly.assign_degree(term.degree() + i.degree());
				insert_in_poly(tempPoly, tempMoly);
			}
		}
		*this = tempPoly;
		return *this;
	}
	const Polynomial<NumberType> operator*(const Monomial<NumberType>& m) const {
		return Polynomial(*this) *= m;
	}
	const Polynomial<NumberType> operator*(const Polynomial<NumberType>& rhs)const {
		return Polynomial(*this) *= rhs;
	}

	// EVALUATION POLYNOMIAL
	const NumberType evaluate(NumberType x) const {
		int sum = 0;
		for (auto& term : term_list) {
			sum += term.coefficient() * power(x, term.degree());
		}
		return sum;
	}

	// EQUALITY TESTS
	bool operator==(const Polynomial<NumberType>& p) const {
		return equal(term_list.begin(), term_list.end(), p.term_list.begin(), [](auto& x, auto& y) {
			return (x.coefficient() == y.coefficient() && x.degree() == y.degree());
		});
	}
	bool operator!=(const Polynomial<NumberType>& p) const {
		return !(*this == p);
	}

	// CALLED FROM ISTREAM AND OSTREAM OVERLOADS
	// read() function has responsibility for getting input, creating Monomial object,
	//			and inserting into Polynomial object
	void read(istream& in = cin) {
		Monomial<NumberType> tempMoly;
		auto degree = tempMoly.degree();
		auto coefficient = tempMoly.coefficient();
		in >> coefficient;

		while (!(coefficient == 0)) {
			in >> degree;
			tempMoly.assign_coefficient(coefficient);
			tempMoly.assign_degree(degree);
			insert_in_poly(*this, tempMoly);
			in >> coefficient;
		}
	}

	
	void print(ostream& out = cout) const {
		out << "\nNumber Terms: " << number_of_terms << endl;
		for (auto p : term_list) {
			if (p.coefficient() == 0) {
				continue;
			}
			else {
				if (p.coefficient() < 0) {
					out << " - ";
				}
				else {
					if (p != *term_list.begin()) {
						out << " + ";
					}
				}
				if (abs(p.coefficient()) != 1 || p.degree() == 0) {
					out << abs(p.coefficient());
				}
				if (p.degree() > 0) {
					out << "x";
					if (p.degree() > 1) {
						out << "^" << p.degree();
					}
				}
			}
		}
		out << endl;
		/*out << "\nNumber Terms" << number_of_terms << endl;
		for (auto p : term_list) {
			if (highest_degree == p.degree()) {
				if (p.degree() == 0 && p.coefficient() == 0) {
					break;
				}
				else {
					if (p.coefficient() < 0) {
						out << "-";
					}
					if (abs(p.coefficient()) != 1) {
						out << abs(p.coefficient());
					}
				}
			}
			else {
				if (p.coefficient() < 0) {
					out << "+";
				}
				else {
					out << "+";
				}
				if ((p.coefficent()) != 1 || p.degree() == 0) {
					out << abs(p.coefficient());
				}
			}
			if (!(p.degree() == 0)) {
				if (p.degree() == 1) {
					out << "x^";
				}
				else {
					out << "x^" << p.degree();
				}
			}
		}
		out << endl;
		*/
	}

private:
	list<Monomial<NumberType> > term_list; //SORTED BY DECREASING DEGREES
	int number_of_terms;
	int highest_degree;

	//private helper member function -- review notes below function prototype
	void insert_in_poly(Polynomial<NumberType>& p, const Monomial<NumberType>& m) {
		int degreeM = m.degree();
		auto itr = p.term_list.begin();
		Monomial<NumberType> first = *itr;

		if (first.coefficient() == 0 && first.degree() == 0) {
			p.term_list.pop_front();
			p.term_list.push_back(m);
			p.highest_degree = m.degree();
			p.number_of_terms = 1;
			return;
		}
		if (p.number_of_terms == 0 && p.highest_degree == 0) {
			p.term_list.push_front(m);
			p.highest_degree = m.degree();
			p.number_of_terms = 1;
			return;
		}
		else {
			while (itr != p.term_list.end()) {
				Monomial<NumberType> currentM = *itr;
				if (currentM.degree() == degreeM) {
					if ((currentM.coefficient() + m.coefficient()) == 0) {
						itr = p.term_list.erase(itr);
						p.number_of_terms--;
						if (p.number_of_terms == 0) {
							p.highest_degree = 0;
							p.term_list.push_back(Monomial<NumberType>(0, 0));
						}
						else {
							if (p.highest_degree == degreeM) {
								p.highest_degree = (*itr).degree();
							}
						}
					}
					else {
						(*itr).assign_coefficient((currentM.coefficient() + m.coefficient()));
					}
					return;
				}
				else if (currentM.degree() < degreeM) {
					auto itrM = p.term_list.insert(itr, m);
					p.number_of_terms++;
					if (p.highest_degree <= currentM.degree()) {
						p.highest_degree = (*itrM).degree();
					}
					return;
				}
				else {
					itr++;
					if (itr == p.term_list.end()) {
						p.term_list.push_back(m);
						p.number_of_terms++;
						return;
					}
				}
			}
		}
		
	}

	// Used in evaluate member function
	NumberType power(NumberType x, int n) const {
		NumberType result = 1;
		for (int i = 0; i < n; i++) {
			result *= x;
		}
		return result;
	}
};


template<typename NumberType>
istream& operator>>(istream& in, Polynomial<NumberType>& rhs) {
	rhs.read();
	return in;
}


template<typename NumberType>
ostream& operator<<(ostream& out, const  Polynomial<NumberType>& rhs) {
	rhs.print();
	return out;
}

#endif

