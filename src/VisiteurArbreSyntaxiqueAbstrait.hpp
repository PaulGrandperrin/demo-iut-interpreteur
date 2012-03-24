#ifndef VISITEURARBRESYNTAXIQUEABSTRAIT_HPP
#define VISITEURARBRESYNTAXIQUEABSTRAIT_HPP

#include "ArbreSyntaxiqueAbstrait.hpp"
#include "Jeton.hpp"
#include <stack>
#include <string>
#include "TableJeton.hpp"
//On utilise le "Hierarchical Visitor Pattern"

using namespace std;




class VisiteurNoeud
{
public:

	virtual ~VisiteurNoeud() {};

	virtual bool visit ( SymboleValue* noeud ) =0;
	virtual bool visit ( Symbole* noeud ) =0;

	virtual bool visitEnter ( NoeudSeqInst* noeud ) =0;
	virtual bool visitEnter ( NoeudAffectation* noeud ) =0;
	virtual bool visitEnter ( NoeudOperateurBinaire* noeud ) =0;
	virtual bool visitEnter ( NoeudOperateurBooleen* noeud ) =0;
	virtual bool visitEnter ( NoeudSi* noeud ) =0;
	virtual bool visitEnter ( NoeudTantQue* noeud ) =0;
	virtual bool visitEnter ( NoeudLire* noeud ) =0;
	virtual bool visitEnter ( NoeudEcrire* noeud ) =0;

	virtual bool visitLeave ( NoeudSeqInst* noeud ) =0;
	virtual bool visitLeave ( NoeudAffectation* noeud ) =0;
	virtual bool visitLeave ( NoeudOperateurBinaire* noeud ) =0;
	virtual bool visitLeave ( NoeudOperateurBooleen* noeud ) =0;
	virtual bool visitLeave ( NoeudSi* noeud ) =0;
	virtual bool visitLeave ( NoeudTantQue* noeud ) =0;
	virtual bool visitLeave ( NoeudLire* noeud ) =0;
	virtual bool visitLeave ( NoeudEcrire* noeud ) =0;

};


class VisiteurAffichage: public VisiteurNoeud
{

public:
	VisiteurAffichage();
	virtual ~VisiteurAffichage() {};

	bool visit ( SymboleValue* noeud );
	bool visit ( Symbole* noeud );


	bool visitEnter ( NoeudSeqInst* noeud );
	bool visitEnter ( NoeudAffectation* noeud );
	bool visitEnter ( NoeudOperateurBinaire* noeud );
	bool visitEnter ( NoeudOperateurBooleen* noeud );
	bool visitEnter ( NoeudSi* noeud );
	bool visitEnter ( NoeudTantQue* noeud );
	bool visitEnter ( NoeudLire* noeud );
	bool visitEnter ( NoeudEcrire* noeud );

	bool visitLeave ( NoeudSeqInst* noeud );
	bool visitLeave ( NoeudAffectation* noeud );
	bool visitLeave ( NoeudOperateurBinaire* noeud );
	bool visitLeave ( NoeudOperateurBooleen* noeud );
	bool visitLeave ( NoeudSi* noeud );
	bool visitLeave ( NoeudTantQue* noeud );
	bool visitLeave ( NoeudLire* noeud );
	bool visitLeave ( NoeudEcrire* noeud );

private:
	int indentation;

};

class VisiteurGenerationC: public VisiteurNoeud
{

public:
	VisiteurGenerationC();
	virtual ~VisiteurGenerationC() {};

	bool visit ( SymboleValue* noeud );
	bool visit ( Symbole* noeud );


	bool visitEnter ( NoeudSeqInst* noeud );
	bool visitEnter ( NoeudAffectation* noeud );
	bool visitEnter ( NoeudOperateurBinaire* noeud );
	bool visitEnter ( NoeudOperateurBooleen* noeud );
	bool visitEnter ( NoeudSi* noeud );
	bool visitEnter ( NoeudTantQue* noeud );
	bool visitEnter ( NoeudLire* noeud );
	bool visitEnter ( NoeudEcrire* noeud );

	bool visitLeave ( NoeudSeqInst* noeud );
	bool visitLeave ( NoeudAffectation* noeud );
	bool visitLeave ( NoeudOperateurBinaire* noeud );
	bool visitLeave ( NoeudOperateurBooleen* noeud );
	bool visitLeave ( NoeudSi* noeud );
	bool visitLeave ( NoeudTantQue* noeud );
	bool visitLeave ( NoeudLire* noeud );
	bool visitLeave ( NoeudEcrire* noeud );

	string getCode ( TableSymboles ts );

private:
	int indentation;
	stack<string> code;

	string nline();

};

#endif /* VISITEURARBRESYNTAXIQUEABSTRAIT_HPP */
