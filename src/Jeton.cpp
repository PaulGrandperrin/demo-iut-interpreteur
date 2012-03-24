#include "Jeton.hpp"
#include <stdlib.h>
#include "VisiteurArbreSyntaxiqueAbstrait.hpp"
#include <typeinfo>
#include "exception.hpp"
////////////////////////////////////////////////////////////////////////////////
SymboleValue::SymboleValue ( Symbole s ) :
		Symbole ( s.getChaine() )
{
	if ( s == "<ENTIER>" )
	{
		valeur = new ValeurEntiere(atoi ( s.getChaine().c_str() ));
	}
	else if( s == "<CHAINE>" )
	{
		valeur = new ValeurChaine(s.getChaine());
	}
	else if( s == "<REEL>" )
	{
		valeur = new ValeurReelle(atof(s.getChaine().c_str()));
	}
	else
	{
		valeur = new ValeurIndefinie();
	}
}

bool SymboleValue::accept ( VisiteurNoeud& v )
{
	v.visit ( this );
	return true;
}


Valeur* SymboleValue::getValeur()
{
	Valeur* v=this->valeur;
	if(typeid(*v)==typeid(ValeurEntiere))
		return new ValeurEntiere(*(ValeurEntiere*)v);
	if(typeid(*v)==typeid(ValeurReelle))
		return new ValeurReelle(*(ValeurReelle*)v);
	if(typeid(*v)==typeid(ValeurChaine))
		return new ValeurChaine(*(ValeurChaine*)v);
	if(typeid(*v)==typeid(ValeurIndefinie))
	{
		throw RunTimeError("Tentative de lecture de la variable "+ this->getChaine()+ " alors qu'elle est indéfinie");
	}
	return NULL;//HACK Pour le compilateur
}


////////////////////////////////////////////////////////////////////////////////
// Attention : cette fonction (operator) n'est pas membre de la classe SymboleValue
ostream & operator << ( ostream & cout, SymboleValue symb )
{
	cout << ( Symbole ) symb << "\t\t - Valeur=";

	if(typeid(*symb.valeur)==typeid(ValeurEntiere))
		cout << ((ValeurEntiere*)(symb.valeur))->getValeur() << " ";
	else if(typeid(*symb.valeur)==typeid(ValeurIndefinie))
		cout << "indefinie ";
	else if(typeid(*symb.valeur)==typeid(ValeurChaine))
		cout << ((ValeurChaine*)(symb.valeur))->getValeur() << " ";
	else
		cout << ((ValeurReelle*)(symb.valeur))->getValeur()<< " ";
	return cout;
}



