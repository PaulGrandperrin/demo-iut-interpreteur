#ifndef JETON_HPP
#define JETON_HPP
#include "Lexeme.hpp"

class VisiteurNoeud;

class SymboleValue : public Symbole
{  // et c'est aussi une feuille de l'arbre abstrait
public:
	SymboleValue ( Symbole s ); // construit un symbole valué à partir d'un symbole existant s
	~SymboleValue( ) {}

	Valeur*  getValeur();
	inline void setValeur ( Valeur* valeur ){delete this->valeur;this->valeur=valeur;};

	bool accept ( VisiteurNoeud& v );

	friend ostream & operator << ( ostream & cout, SymboleValue symb ); // affiche un symbole value sur cout

private:
	Valeur*  valeur;	// valeur du symbole si elle est définie, zéro sinon
};

#endif /* JETON_HPP */
