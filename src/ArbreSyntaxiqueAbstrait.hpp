#ifndef ARBRESYNTAXIQUEABSTRAIT_HPP
#define ARBRESYNTAXIQUEABSTRAIT_HPP

#include <vector>
#include <iostream>
#include <iomanip>
#include "Valeur.hpp"

using namespace std;

class Symbole;
class VisiteurNoeud;

//----------------------------


class Noeud
{
public:
	virtual ~Noeud() {}
	virtual bool accept ( VisiteurNoeud& ) =0;
	virtual Valeur* getValeur(){return NULL;};
};

class NoeudComposite : public Noeud
{
public:
virtual ~NoeudComposite() {}
};

class NoeudFeuille: public Noeud
{
public:
virtual ~NoeudFeuille() {}
};


class NoeudSeqInst : public NoeudComposite
{
public:
	~NoeudSeqInst() {}

	void ajouteInstruction ( Noeud* instruction );
	bool accept ( VisiteurNoeud& v );
	Valeur* getValeur();

	vector<Noeud *> getTabInst()
	{
		return tabInst;
	};
private:
	vector<Noeud *> tabInst; // pour stocker les instructions de la séquence
};


class NoeudAffectation : public NoeudComposite
{

public:
	NoeudAffectation ( Noeud* variable, Noeud* expression );
	~NoeudAffectation() {}

	bool accept ( VisiteurNoeud& v );
	Valeur* getValeur();

	Noeud* getVariable()
	{
		return variable;
	};
	Noeud* getExpression()
	{
		return expression;
	};
private:
	Noeud* variable;
	Noeud* expression;
};


class NoeudOperateurBinaire : public NoeudComposite
{


public:
	NoeudOperateurBinaire ( Symbole* operateur, Noeud* operandeGauche, Noeud* operandeDroit );
	~NoeudOperateurBinaire() {};

	bool accept ( VisiteurNoeud& v );
	Valeur* getValeur();

	Symbole* getOperateur()
	{
		return operateur;
	};
	Noeud* getOperandeGauche()
	{
		return operandeGauche;
	};
	Noeud* getOperandeDroit()
	{
		return operandeDroit;
	};
private:
	Symbole* operateur;
	Noeud*  operandeGauche;
	Noeud*  operandeDroit;
};

class NoeudOperateurBooleen: public NoeudComposite
{
public:

	NoeudOperateurBooleen ( Symbole* operateur, Noeud* operandeGauche, Noeud* operandeDroit );
	~NoeudOperateurBooleen() {}

	bool accept ( VisiteurNoeud& v );
	Valeur* getValeur();

	Symbole* getOperateur()
	{
		return operateur;
	};
	Noeud* getOperandeGauche()
	{
		return operandeGauche;
	};
	Noeud* getOperandeDroit()
	{
		return operandeDroit;
	};
private:
	Symbole* operateur;
	Noeud* operandeGauche;
	Noeud* operandeDroit;
};

class NoeudSi: public NoeudComposite
{
public:
	NoeudSi ( Noeud* expBool, NoeudSeqInst* vrai, NoeudSeqInst* faux );
	~NoeudSi() {}

	bool accept ( VisiteurNoeud& v );
	Valeur* getValeur();

	inline Noeud* getExpBool(){return expBool;};
	inline NoeudSeqInst* getSeqInstVrai(){return seqInstVrai;};
	inline NoeudSeqInst* getSeqInstFaux(){return seqInstFaux;};
private:
	Noeud* expBool;
	NoeudSeqInst* seqInstVrai;
	NoeudSeqInst* seqInstFaux;
};

class NoeudTantQue: public NoeudComposite
{
public:
	NoeudTantQue ( Noeud* expBool,NoeudSeqInst* seqInst,bool conditionDebut );
	~NoeudTantQue() {}

	bool accept ( VisiteurNoeud& v );
	Valeur* getValeur();

	inline Noeud* getExpBool(){return expBool;};
	inline NoeudSeqInst* getSeqInstVrai(){return seqInst;};
	inline bool getConditionDebut(){return conditionDebut;};
private:
	Noeud* expBool;
	NoeudSeqInst* seqInst;
	bool conditionDebut;
};

class NoeudLire: public NoeudComposite
{
public:
	NoeudLire ( Noeud* variable );
	~NoeudLire() {}

	bool accept ( VisiteurNoeud& v );
	Valeur* getValeur();

	inline Noeud* getVariable(){return variable;};
private:
	Noeud* variable;
};

class NoeudEcrire: public NoeudComposite
{
public:
	NoeudEcrire ( Noeud* exp );
	~NoeudEcrire() {}

	bool accept ( VisiteurNoeud& v );
	Valeur* getValeur();

	inline Noeud* getExp(){return exp;};
private:
	Noeud* exp;
};

#endif /* ARBRESYNTAXIQUEABSTRAIT_HPP */
