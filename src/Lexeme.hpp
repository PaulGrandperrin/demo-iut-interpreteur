#ifndef LEXEME_HPP
#define LEXEME_HPP

#include <iostream>
#include <vector>
#include <string>

#include "ArbreSyntaxiqueAbstrait.hpp"

using namespace std;

#define FICHIER_MOTS_CLE "ListeLexeme.txt"
class VisiteurNoeud;
class Symbole : public NoeudFeuille
{
public:
	virtual ~Symbole() {}
	enum categorieSymbole { MOTCLE, VARIABLE, ENTIER, INDEFINI,CHAINE, REEL,FINDEFICHIER };
	Symbole ( string s=string ( "" ) ); // construit le symbole à partir de la chaine (string) ch lue par le lecteur de symbole dans un fichier
	bool operator == ( string ch ) ; // pour tester l'égalité entre le symbole et une chaîne

	inline bool operator != ( string ch ){return ! ( *this == ch );};
	inline string getChaine(){return this->chaine;};

	bool accept ( VisiteurNoeud& v );


	friend ostream & operator << ( ostream & cout, Symbole symb );  // fonction amie pour pouvoir afficher un symbole sur cout

private:
	string           chaine;              // chaine du symbole
	categorieSymbole categorie;           // categorie du symbole (voir type énuméré ci-dessus)
	static bool      isMotCle ( string s );  // méthode de classe : renvoie vrai si s est un mot clé du langage
};

#endif /* LEXEME_HPP */
