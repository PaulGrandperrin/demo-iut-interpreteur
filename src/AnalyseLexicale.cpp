#include "AnalyseLexicale.hpp"
#include <ctype.h>
#include <string.h>
#include <iostream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
LecteurSymbole::LecteurSymbole ( string nomFich ) :
		lc ( nomFich ), symCour ( "" )
{
	suivant();
}

////////////////////////////////////////////////////////////////////////////////
void LecteurSymbole::suivant()
{

	// on est maintenant positionne sur le premier caractère d'un symbole
	sauterSeparateurs();
	while ( lc.getCarCour() =='#' )
	{
		sauterCommentaire();
		sauterSeparateurs();
	}
	ligne=lc.getLigne();
	colonne=lc.getColonne();
	symCour=Symbole ( motSuivant() ); // on reconstruit symCour avec le nouveau mot lu

	#if DEBUG>0
		cout << "-Debug-Symbole lu : "<<symCour.getChaine()<< endl;
	#endif
}

////////////////////////////////////////////////////////////////////////////////
void LecteurSymbole::sauterSeparateurs()
{
			char c=lc.getCarCour();
			while (!isalpha(c) && !isdigit(c) && c!='=' && c!='!' && c!=';' && c!='-' && c!='+' && c!='*' && c!='/'&& c!='%' && c!='#' && c!='(' && c!=')' && c!='<'&& c!= '>' && c!= '"' && c!=EOF)
			{
				if(c!=' ' && c!='\t' && c!='\r' && c!='\n')
					// c'est un Warning, il ne pertube en rien l'analyse et ne nécéssite donc pas d'exception
					cout << "Attention " << endl<< lc.getLigne() <<":" << lc.getColonne() << ": " << "caractère non autorisé : "<<lc.getCarCour()<< endl<<endl;
				lc.suivant();
				c=lc.getCarCour();
			}

}

////////////////////////////////////////////////////////////////////////////////
void LecteurSymbole::sauterCommentaire()
{
	while ( lc.getCarCour() !='\n' )
		lc.suivant();
}

////////////////////////////////////////////////////////////////////////////////
string LecteurSymbole::motSuivant()
{
	static string  s;
	s="";
	if ( isdigit ( lc.getCarCour() ) )
		// c'est le début d'un entier
		do
		{
			s=s+lc.getCarCour();
			lc.suivant();
		}
		while ( isdigit ( lc.getCarCour())||lc.getCarCour()=='.' );

	else if ( isalpha ( lc.getCarCour() ) )
		// c'est le début d'un mot
		do
		{
			s=s+lc.getCarCour();
			lc.suivant();
		}
		while ( isalpha ( lc.getCarCour() )  ||
				isdigit ( lc.getCarCour() ) ||
				lc.getCarCour() =='_' );
	else if ( lc.getCarCour() =='"' )
	{
		do
		{
			s=s+lc.getCarCour();
			lc.suivant();
		}
		while ( lc.getCarCour() !='"' );
		s+=lc.getCarCour();
		lc.suivant();
	}
	else if ( lc.getCarCour() !=EOF )
		// c'est un caractere special
	{
		s=lc.getCarCour();
		lc.suivant();
		if ( ( s=="=" || s=="<" || s==">" || s=="!" ) && lc.getCarCour() =='=' )
		{
			s+='=';
			lc.suivant();
		}
	}

	return s;
}
