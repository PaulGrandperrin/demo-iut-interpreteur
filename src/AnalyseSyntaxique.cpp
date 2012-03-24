#include "AnalyseSyntaxique.hpp"

#include <stdlib.h>
#include <iostream>


using namespace std;

#define DEBUG 0

void LecteurPhraseSimple::traitementErreur(const SyntaxError& e)
{

	cerr << e.what() << endl; //On affiche l'erreur
	nbError++;

	while(!(ls.getSymCour()=="<FINDEFICHIER>"||ls.getSymCour() =="<VARIABLE>" || ls.getSymCour() =="si" || ls.getSymCour() =="tantque"|| ls.getSymCour() =="repeter" || ls.getSymCour() =="lire" || ls.getSymCour() =="ecrire"||ls.getSymCour()=="fin"||ls.getSymCour()=="fintantque"||ls.getSymCour()=="sinonsi"||ls.getSymCour()=="sinon"||ls.getSymCour()=="finsi"||ls.getSymCour()=="jusqua" ))
	{
		ls.suivant(); //On avance de manière brute : cad on utilise pas this->suivant() qui peut a nouveau lever des exceptions
	}

	#if DEBUG>1
		cout << "-Debug-Symbole de reprise après erreur : "<<ls.getSymCour().getChaine()<< endl;
	#endif
}

////////////////////////////////////////////////////////////////////////////////
LecteurPhraseSimple::LecteurPhraseSimple ( string nomFich ) : ls ( nomFich )
{
	nbError=0;
}

////////////////////////////////////////////////////////////////////////////////
unsigned int LecteurPhraseSimple::analyse()
{
	arbre=programme();
	return nbError;
}

////////////////////////////////////////////////////////////////////////////////
Noeud* LecteurPhraseSimple::programme()
{
// <programme> ::= debut <seqInst> fin FIN_FICHIER

	Noeud* si=NULL;
	sauterSymCour ( "debut" ); //Si ca plante ici, l'exception stoppera le programme car on est surement pas dans un fichier source
	while(ls.getSymCour()!="<FINDEFICHIER>" )
	{
		si = seqInst();
		try
		{
			sauterSymCour ( "fin" );
		}
		catch(const SyntaxError& e)
		{
			traitementErreur(e);
			ls.suivant(); //Il a du y'avoir un block de fin en trop, on le saute alors
			if(ls.getSymCour()==";")
				ls.suivant(); //On saute aussi son point virgule s'il en a un
		}
	}
	return si;
}

////////////////////////////////////////////////////////////////////////////////
NoeudSeqInst* LecteurPhraseSimple::seqInst()
{
// <seqInst> ::= <inst> ; { <inst> ; }
	NoeudSeqInst* si = new NoeudSeqInst();

	for(;;) //HACK permet de reprendre si on tombe sur quelque chose qui est : ni une instruction ni un début de block ni une fin de block
			//Pas très propre de faire comme ça mais sinon je suis obligé de faire une condition pour le while absolument énorme
	{
		while ( ls.getSymCour() =="<VARIABLE>" || ls.getSymCour() =="si" || ls.getSymCour() =="tantque"|| ls.getSymCour() =="repeter" || ls.getSymCour() =="lire" || ls.getSymCour() =="ecrire" )
		{ //tant qu'on est au debut d'une instruction
			try
			{
				si->ajouteInstruction ( inst() );
				sauterSymCour ( ";" );
			}
			catch(const SyntaxError& e)
			{
				traitementErreur(e);
			}
		}

		if( ls.getSymCour()=="<FINDEFICHIER>"||ls.getSymCour()=="fin"||ls.getSymCour()=="fintantque"||ls.getSymCour()=="sinonsi"||ls.getSymCour()=="sinon"||ls.getSymCour()=="finsi"||ls.getSymCour()=="jusqua" )
			//Si on est sur un symbole indiquant la fin d'un block, on remonte d'un niveau
			return si;
		else // Y a un problème, on attendait une instruction ou une fin de block et on a autre chose
		{
			string buf;
			buf+="Attendu : Instruction ou Fin de block";
			buf+="\nTrouve  : ";
			buf+=ls.getSymCour().getChaine();
			traitementErreur(SyntaxError(ls.getLigne(),ls.getColonne(),buf));
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
Noeud* LecteurPhraseSimple::inst()
{
// <inst> ::= <affectation>

	if ( ls.getSymCour() =="si" )
	{
		return instSi();
	}
	else if ( ls.getSymCour() =="tantque" )
	{
		return instTantQue();
	}
	else if ( ls.getSymCour() =="repeter" )
	{
		return instRepeter();
	}
	else if ( ls.getSymCour() =="lire" )
	{
		return instLire();
	}
	else if ( ls.getSymCour() =="ecrire" )
	{
		return instEcrire();
	}
	else //c'est une variable
	{
		return affectation();
	}
}

Noeud* LecteurPhraseSimple::instSi()
{
	/** NOTE
	*	Le Si/SinonSi/Sinon est décomposé en Si/Sinon
	*	Le SinonSi est alors transformé en Sinon Si
	*	Un exemple:
	*		Si(a)
	*		SinonSi(b)
	*		Sinon
	*		FinSi
	*	sera transformé en arbre de cette forme:
	*		Si(a)
	*		Sinon
	*			Si(b)
	*			Sinon
	*			FinSi
	*		FinSi
	*
	*	Dans le code c++, 	-SeqInstVrai représente les instructions éxécutés si la condition est vrai
	*						-SeqInstFaux représente les instructions éxécutés si la condition est faux
	*
	*/

	bool sinonsi=ls.getSymCour() =="sinonsi";
	suivant();
	sauterSymCour ( "(" );
	Noeud* Condition=expBool();
	sauterSymCour ( ")" );
	NoeudSeqInst* seqInstVrai=seqInst();
	NoeudSeqInst* seqInstFaux;
	if ( ls.getSymCour() =="sinonsi" )
	{
		seqInstFaux=new NoeudSeqInst();
		seqInstFaux->ajouteInstruction ( instSi() );
	}
	else if ( ls.getSymCour() =="sinon" )
	{
		suivant();
		seqInstFaux=seqInst();
	}
	else
	{
		seqInstFaux=new NoeudSeqInst();
	}

	testerSymCour ( "finsi" );
	if ( !sinonsi )
		sauterSymCour ( "finsi" );

	return new NoeudSi ( Condition,seqInstVrai,seqInstFaux );
}

Noeud* LecteurPhraseSimple::instTantQue()
{
	suivant();
	sauterSymCour ( "(" );
	Noeud* condition=expBool();
	sauterSymCour ( ")" );
	NoeudSeqInst* boucle=seqInst();
	sauterSymCour ( "fintantque");
	NoeudTantQue* tantQue= new NoeudTantQue ( condition,boucle,true );
	return tantQue;
}

Noeud* LecteurPhraseSimple::instRepeter()
{
	suivant();
	NoeudSeqInst* boucle=seqInst();
	Noeud* condition;
	sauterSymCour ( "jusqua" );
	sauterSymCour ( "(" );
	condition=expBool();
	sauterSymCour ( ")" );
	NoeudTantQue* repeter= new NoeudTantQue ( condition,boucle,false ); //Le noeud TantQue gere les "tantque" et les "repeter"
	return repeter;
}


Noeud* LecteurPhraseSimple::instLire()
{
	suivant();
	sauterSymCour ( "(" );
	testerSymCour ( "<VARIABLE>" );
	NoeudLire* lire=new NoeudLire ( suivant() );
	sauterSymCour ( ")" );
	return lire;
}

Noeud* LecteurPhraseSimple::instEcrire()
{
	suivant();
	sauterSymCour ( "(" );
	NoeudEcrire* ecrire=new NoeudEcrire ( expression() );
	sauterSymCour ( ")" );
	return ecrire;
}


Noeud* LecteurPhraseSimple::affectation()
{
	testerSymCour ( "<VARIABLE>" );
	SymboleValue* var= suivant();
	sauterSymCour ( "=" );
	Noeud* exp= expression();
	return new NoeudAffectation ( var,exp );
}


Noeud* LecteurPhraseSimple::expression()
{
	Noeud* noeud=terme();
	while ( ls.getSymCour() =="+" || ls.getSymCour() =="-" )
	{
		Symbole* operateur= opAdd();
		Noeud* noeudDroit=terme();
		noeud=new NoeudOperateurBinaire ( operateur,noeud,noeudDroit );
	}
	return noeud;
}

Noeud* LecteurPhraseSimple::terme()
{
	Noeud* noeud=facteur();
	while ( ls.getSymCour() =="*" || ls.getSymCour() =="/" || ls.getSymCour() =="%")
	{
		Symbole* operateur = opMult();
		Noeud* noeudDroit=facteur();
		noeud=new NoeudOperateurBinaire ( operateur,noeud,noeudDroit );
	}
	return noeud;
}


Noeud* LecteurPhraseSimple::facteur() //TODO ajouter les opérateurs unaires
{
	if ( ls.getSymCour() =="<VARIABLE>" || ls.getSymCour() =="<ENTIER>" || ls.getSymCour()=="<CHAINE>"|| ls.getSymCour()=="<REEL>")
	{
		Noeud* noeud=suivant();
		return noeud;
	}
	else if ( ls.getSymCour() =="(" )
	{
		suivant();
		Noeud* noeud= expBool();
		sauterSymCour ( ")" );
		return noeud;
	}
	else
	{
		erreur ( "<facteur>" );
		return NULL; //HACK pour le compilateur
	}
}

Noeud* LecteurPhraseSimple::expBool()
{
	Noeud* noeud=relationBool();
	while ( ls.getSymCour() =="ou" )
	{
		Symbole* ou= opOu();
		Noeud* noeudDroit = relationBool();
		noeud= new NoeudOperateurBooleen ( ou, noeud, noeudDroit );
	}
	return noeud;
}

Symbole* LecteurPhraseSimple::opOu()
{
	Symbole* operateur=new Symbole();
	if ( ls.getSymCour() =="ou" )
	{
		*operateur=ls.getSymCour();
		suivant();
	}
	else
		erreur ( "<opOu>" );
	return operateur;
}

Noeud* LecteurPhraseSimple::relationBool()
{
	Noeud* noeud = relation();
	while ( ls.getSymCour() =="et" )
	{
		Symbole* et = opEt();
		Noeud* noeudDroit = relation();
		noeud = new NoeudOperateurBooleen ( et,noeud,noeudDroit );
	}
	return noeud;
}

Symbole* LecteurPhraseSimple::opEt()
{
	Symbole* operateur=new Symbole();
	if ( ls.getSymCour() =="et" )
	{
		*operateur=ls.getSymCour();
		suivant();
	}
	else
		erreur ( "<opEt>" );
	return operateur;
}

Noeud* LecteurPhraseSimple::relation()
{
	Noeud* noeud = expression();
	while ( ls.getSymCour() =="==" || ls.getSymCour() =="!=" || ls.getSymCour() =="<=" || ls.getSymCour() ==">=" || ls.getSymCour() =="<" || ls.getSymCour() ==">" )
	{
		Symbole* comparateur= opRel();
		Noeud* noeudDroit = expression();
		noeud = new NoeudOperateurBooleen ( comparateur,noeud,noeudDroit );
	}
	return noeud;
}

Symbole* LecteurPhraseSimple::opRel()
{
	Symbole* comparateur=new Symbole();
	if ( ls.getSymCour() =="==" || ls.getSymCour() =="!=" || ls.getSymCour() =="<=" || ls.getSymCour() ==">=" || ls.getSymCour() =="<" || ls.getSymCour() ==">" )
	{
		*comparateur=ls.getSymCour();
		suivant();
	}
	else
		erreur ( "<opRel>" );
	return comparateur;
}

Symbole* LecteurPhraseSimple::opAdd()
{
// <opBinaire> ::= + | -
	Symbole* operateur=new Symbole();
	if ( ls.getSymCour() =="+" || ls.getSymCour() =="-" )
	{
		*operateur=ls.getSymCour();
		suivant();
	}
	else
		erreur ( "<opAdd>" );
	return operateur;
}

Symbole* LecteurPhraseSimple::opMult()
{
// <opBinaire> ::= *  | /
	Symbole* operateur=new Symbole();
	if ( ls.getSymCour() =="*" || ls.getSymCour() =="/" || ls.getSymCour() =="%")
	{
		*operateur=ls.getSymCour();
		suivant();
	}
	else
		erreur ( "<opMult>" );
	return operateur;
}

void LecteurPhraseSimple::testerSymCour ( string ch )
{
	if ( ls.getSymCour() != ch )
	{
		erreur(ch);
	}
}

void LecteurPhraseSimple::sauterSymCour ( string ch )
{
	testerSymCour ( ch );
	suivant();
}

void LecteurPhraseSimple::erreur ( string mess )
{
	string buf;
	buf+="Attendu : ";
	buf+=mess;
	buf+="\nTrouve  : ";
	buf+=ls.getSymCour().getChaine();
	throw SyntaxError(ls.getLigne(),ls.getColonne(),buf);
}

SymboleValue* LecteurPhraseSimple::suivant()
{
	SymboleValue* sv=NULL;
	if ( ls.getSymCour() =="<VARIABLE>" || ls.getSymCour() =="<ENTIER>" || ls.getSymCour() =="<CHAINE>" ||ls.getSymCour()=="<REEL>")
		sv= ts.chercheAjoute ( ls.getSymCour() );
	ls.suivant();

	return sv;
}
