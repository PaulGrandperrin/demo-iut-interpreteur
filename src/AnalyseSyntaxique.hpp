#ifndef ANALYSESYNTAXIQUE_HPP
#define ANALYSESYNTAXIQUE_HPP

#include "AnalyseLexicale.hpp"
#include "ArbreSyntaxiqueAbstrait.hpp"
#include "TableJeton.hpp"
#include "exception.hpp"

class LecteurPhraseSimple
{
public:
	LecteurPhraseSimple ( string nomFich ); // Construit un lecteur de phrase pour vérifier
	// la syntaxe du programme dans le fichier nomFich

	unsigned int analyse();   // Si le contenu du fichier est conforme à la grammaire,
	// cette méthode se termine normalement et affiche un message "Syntaxe correcte".
	// Sinon, le programme sera interrompu (exit).
	inline Noeud* getArbre (){return arbre;};
	inline TableSymboles getTs (){return ts;};

private:
	LecteurSymbole ls;  // le lecteur de symboles utilisé pour lyser le fichier
	TableSymboles  ts;  // la table des symboles valués
	Noeud* arbre;

	// implémentation de la grammaire
	Noeud* programme();   //   <programme> ::= debut <seqInst> fin <EOF>
	NoeudSeqInst* seqInst();	    //     <seq_ins> ::= <inst> ; { <inst> ; }
	Noeud* inst();	    //        <inst> ::= <affectation>
	Noeud* affectation();	// <affectation> ::= <variable> = <expression>
	Noeud* expression();	//  <expression> ::= <facteur> { <opBinaire> <facteur> }
	Noeud* facteur();     //     <facteur> ::= <entier>  |  <variable>  |  - <facteur>  |  ( <expression> )
	Noeud* terme();
	Symbole* opAdd();
	Symbole* opMult();
	Symbole* opOu();
	Symbole* opEt();
	Symbole* opRel();
	Noeud* expBool();
	Noeud* instLire();
	Noeud* instEcrire();
	Noeud* relation() ;
	Noeud* relationBool() ;
	Noeud* instSi();
	Noeud* instTantQue();
	Noeud* instRepeter();

	// outils pour se simplifier l'analyse syntaxique
	void testerSymCour ( string ch );  // si symbole courant != ch, erreur : on arrete le programme, sinon rien
	void sauterSymCour ( string ch );  // si symbole courant == ch, on passe au symbole suivant, sinon erreur : on arrete
	void erreur ( string mess );    // affiche les message d'erreur mess et arrete le programme
	SymboleValue* suivant();
	SymboleValue* getSymValue();

	unsigned int nbError;

	void traitementErreur(const SyntaxError& e);
};

#endif /* ANALYSESYNTAXIQUE_HPP */
