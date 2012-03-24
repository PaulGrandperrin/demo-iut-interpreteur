#ifndef TABLEJETON_HPP
#define TABLEJETON_HPP

#include "Jeton.hpp"
#include <vector>
#include <iostream>
using namespace std;

class TableSymboles
{
public:
	TableSymboles(); // Construit une table vide de pointeurs sur des symboles valués
	SymboleValue * chercheAjoute ( Symbole s );
	// si s est identique à un symbole valué déjà présent dans la table,
	// on renvoie un pointeur sur ce symbole valué
	// Sinon on insère un nouveau symbole valué correspondant à s
	// et on renvoie un pointeur sur le nouveau symbole valué inséré
	inline vector<SymboleValue *> getTable(){return table;};

	friend ostream & operator << ( ostream & cout, TableSymboles ts ); // affiche ts sur cout

private:
	vector<SymboleValue *> table; // la table des symboles valués, triée sur la chaine
};

#endif /* TABLEJETON_HPP */
