#ifndef VALEUR_HPP
#define VALEUR_HPP

#include <string>
using namespace std;

/*
class NonCopyable
{
	protected:
		NonCopyable(){};
		~NonCopyable(){};
	private:
		NonCopyable(const NonCopyable&){};
		NonCopyable& operator=(const NonCopyable&){};
};
*/

class Valeur //: public NonCopyable
{
	public:
		virtual ~Valeur(){};
};

class ValeurEntiere:public Valeur
{
public:
	ValeurEntiere(long val=0){this->val=val;};
	long getValeur(){return val;};
private:
	long val;
};

class ValeurReelle:public Valeur
{
public:
	ValeurReelle(double val=0.0){this->val=val;};
	double getValeur(){return val;};
private:
	double val;
};

class ValeurChaine:public Valeur
{
	private:
	string val;
public:
	ValeurChaine(string val=""){this->val=val;};
	string getValeur(){return val;};
};

class ValeurIndefinie:public Valeur
{
};
#endif /* VALEUR_HPP */

