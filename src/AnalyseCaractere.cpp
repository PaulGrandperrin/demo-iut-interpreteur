#include <stdlib.h>
#include <iostream>
#include "AnalyseCaractere.hpp"
#include "exception.hpp"
using namespace std;

LecteurCaractere::LecteurCaractere ( string nomFich ) : f ( nomFich.data() )
{
	ligne=1;
	colonne=0;
	if ( f.fail() )
	{
		throw FatalError("Fichier \""+nomFich + "\" non trouvé.");
	}
	else
		suivant();
}

void LecteurCaractere::suivant()
{
	if ( f.peek() ==EOF )
		carCour=EOF;
	else
	{
		if ( carCour=='\n' )
		{
			colonne=0;
			ligne++;
		}
		f.get ( carCour );
		colonne++;
	}
}
