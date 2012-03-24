#include "AnalyseSyntaxique.hpp"
#include "VisiteurArbreSyntaxiqueAbstrait.hpp"
#include <iostream>
#include <string>
#include "exception.hpp"
using namespace std;

int main ( int argc, char* argv[] )
{
	try
	{
		string nomFich;
		if ( argc != 2 )
			throw FatalError("Veuillez donnez un nom de fichier en argument");

		nomFich=argv[1];

		LecteurPhraseSimple lp ( nomFich );
		unsigned int nbError=lp.analyse();
		if(nbError!=0)
		{
			ostringstream oss;
			oss << nbError;
			throw FatalError("Analyse Syntaxique finie avec "  +oss.str()+ " erreur(s) ");
		}
		VisiteurAffichage va;

		lp.getArbre()->accept ( va );


	}
	catch(const SyntaxError& e)
	{
		cerr << "Erreur Syntaxique irrécupérable: " << endl<< e.what()<<endl;;
	}
	catch(const FatalError& e)
	{
		cerr << "Erreur Fatale: " << endl<< e.what()<<endl;;
	}
	catch(const RunTimeError& e)
	{
		cerr << "Erreur d'execution: " << endl<< e.what()<<endl;;
	}
	catch(...)
	{
		cerr << "EVNI : Exception Volante Non Identifiée" <<endl;
	}

}
