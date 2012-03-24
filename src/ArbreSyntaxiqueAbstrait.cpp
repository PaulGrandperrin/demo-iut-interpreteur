#include <stdlib.h>
#include "ArbreSyntaxiqueAbstrait.hpp"
#include "Lexeme.hpp"
#include "Jeton.hpp"
#include "VisiteurArbreSyntaxiqueAbstrait.hpp"
#include <typeinfo>
#include "exception.hpp"
#include <math.h>


void NoeudSeqInst::ajouteInstruction ( Noeud* instruction )
{
	tabInst.push_back ( instruction );
}

NoeudAffectation::NoeudAffectation ( Noeud* variable, Noeud* expression )
{
	this->variable=variable;
	this->expression=expression;
}

NoeudOperateurBinaire::NoeudOperateurBinaire ( Symbole* operateur,
		Noeud* operandeGauche,
		Noeud* operandeDroit )
{
	this->operateur=operateur;
	this->operandeGauche=operandeGauche;
	this->operandeDroit=operandeDroit;
}

NoeudOperateurBooleen::NoeudOperateurBooleen ( Symbole* operateur,
		Noeud* operandeGauche,
		Noeud* operandeDroit )
{
	this->operateur=operateur;
	this->operandeGauche=operandeGauche;
	this->operandeDroit=operandeDroit;
}

NoeudSi::NoeudSi ( Noeud* expBool, NoeudSeqInst* vrai, NoeudSeqInst* faux )
{
	this->expBool=expBool;
	this->seqInstVrai=vrai;
	this->seqInstFaux=faux;
}


NoeudTantQue::NoeudTantQue ( Noeud* expBool,NoeudSeqInst* seqInst,bool conditionDebut )
{
	this->expBool=expBool;
	this->seqInst=seqInst;
	this->conditionDebut=conditionDebut;
}

NoeudLire::NoeudLire ( Noeud* variable )
{
	this->variable=variable;
}

NoeudEcrire::NoeudEcrire ( Noeud* exp )
{
	this->exp=exp;
}

//######################################################################################

bool NoeudSeqInst::accept ( VisiteurNoeud& v )
{
	if ( v.visitEnter ( this ) )
	{
		for ( unsigned int i=0; i<tabInst.size(); i++ )
			tabInst[i]->accept ( v );
	}
	return v.visitLeave ( this );
}

bool NoeudAffectation::accept ( VisiteurNoeud& v )
{
	if ( v.visitEnter ( this ) )
	{
		variable->accept ( v );
		expression->accept ( v );
	}
	return v.visitLeave ( this );
}

bool NoeudOperateurBinaire::accept ( VisiteurNoeud& v )
{
	if ( v.visitEnter ( this ) )
	{
		operateur->accept ( v );
		operandeGauche->accept ( v );
		operandeDroit->accept ( v );
	}
	return v.visitLeave ( this );

}

bool NoeudOperateurBooleen::accept ( VisiteurNoeud& v )
{
	if ( v.visitEnter ( this ) )
	{
		operateur->accept ( v );
		operandeGauche->accept ( v );
		operandeDroit->accept ( v );
	}
	return v.visitLeave ( this );

}

bool NoeudSi::accept ( VisiteurNoeud& v )
{
	if ( v.visitEnter ( this ) )
	{
		expBool->accept ( v );
		seqInstVrai->accept ( v );
		seqInstFaux->accept ( v );
	}
	return v.visitLeave ( this );

}

bool NoeudTantQue::accept ( VisiteurNoeud& v )
{
	if ( v.visitEnter ( this ) )
	{
		expBool->accept ( v );
		seqInst->accept ( v );
	}
	return v.visitLeave ( this );
}

bool NoeudLire::accept ( VisiteurNoeud& v )
{
	if ( v.visitEnter ( this ) )
	{
		variable->accept ( v );
	}
	return v.visitLeave ( this );

}

bool NoeudEcrire::accept ( VisiteurNoeud& v )
{
	if ( v.visitEnter ( this ) )
	{
		exp->accept ( v );
	}
	return v.visitLeave ( this );

}


//######################################################################################

Valeur* NoeudSeqInst::getValeur()
{
	Valeur* v;

	for (unsigned int i=1; i<tabInst.size(); i++) // HACK probleme de unsigned int : 0-1= grand nombre
		delete tabInst[i-1]->getValeur();			//HACK

	if(tabInst.size()>0)
	{
		v=tabInst[tabInst.size()-1]->getValeur();
		return v;
	}
	return NULL;//HACK pour le compilateur
}

Valeur* NoeudAffectation::getValeur()
{
	Valeur* v = expression->getValeur();
	((SymboleValue*)variable)->setValeur(v);

	return ((SymboleValue*)variable)->getValeur();
}

Valeur* NoeudOperateurBinaire::getValeur()
{
	Valeur* vog=operandeGauche->getValeur();
	Valeur* vod=operandeDroit->getValeur();
	//cout << "g : "<< typeid(*vog).name()<< " | d : "<< typeid(*vod).name() << endl;
	if(typeid(*vog)==typeid(ValeurEntiere) && typeid(*vod)==typeid(ValeurEntiere))
	{
		long e=0,eog=((ValeurEntiere*)vog)->getValeur(),eod=((ValeurEntiere*)vod)->getValeur();

		delete vog;delete vod;

		if      (*(this->operateur)=="+") e = eog+eod;
		else if (*(this->operateur)=="-") e = eog-eod;
		else if (*(this->operateur)=="*") e = eog*eod;
		else if (*(this->operateur)=="%") e = eog%eod;
		else if (*(this->operateur)=="/")
		{
			if (eod!=0)
				e = eog/eod;
			else
				throw RunTimeError("Diviser par zero, t'es pas fou?");
		}
		return new ValeurEntiere(e);
	}
	else if(typeid(*vog)==typeid(ValeurReelle) && typeid(*vod)==typeid(ValeurReelle))
	{
		double r,rog=((ValeurReelle*)vog)->getValeur(),rod=((ValeurReelle*)vod)->getValeur();

		delete vog;delete vod;

		if      (*(this->operateur)=="+") r = rog+rod;
		else if (*(this->operateur)=="-") r = rog-rod;
		else if (*(this->operateur)=="*") r = rog*rod;
		else if (*(this->operateur)=="/")
		{
			if (rod!=0)
				r = rog/rod;
			else
				throw RunTimeError("Diviser par zero, t'es pas fou?");
		}
		else
		{
			throw RunTimeError("Le modulo ne marche pas sur les réels");
			return NULL; // Pour le compilateur
		}

		return new ValeurReelle(r);
	}
	else if(typeid(*vog)==typeid(ValeurChaine) && typeid(*vod)==typeid(ValeurChaine))
	{
		string c="",cog=((ValeurChaine*)vog)->getValeur(),cod=((ValeurChaine*)vod)->getValeur();

		delete vog;delete vod;

		if(*(this->operateur)=="+")
		{
			c = cog+cod;
			return new ValeurChaine(c);
		}
		else
		{
			throw RunTimeError("Seule l'addition marche sur les chaines");
			return NULL; // Pour le compilateur
		}
	}
	else
	{
		throw RunTimeError("Operation impossible sur ces types");
		return NULL; //HACK pour le compilateur
	}
}

Valeur* NoeudOperateurBooleen::getValeur()
{
	Valeur* vog=operandeGauche->getValeur();
	Valeur* vod=operandeDroit->getValeur();

	if(typeid(*vog)==typeid(ValeurEntiere) && typeid(*vod)==typeid(ValeurEntiere))
	{
		long e=0,eog=((ValeurEntiere*)vog)->getValeur(),eod=((ValeurEntiere*)vod)->getValeur();

		delete vog;delete vod;

		if      (*(this->operateur)=="==") e = eog==eod;
		else if (*(this->operateur)=="!=") e = eog!=eod;
		else if (*(this->operateur)=="<") e = eog<eod;
		else if (*(this->operateur)==">") e = eog>eod;
		else if (*(this->operateur)=="<=") e = eog<=eod;
		else if (*(this->operateur)==">=") e = eog>=eod;
		else if (*(this->operateur)=="et") e = eog&&eod;
		else if (*(this->operateur)=="ou") e = eog||eod;

		return new ValeurEntiere(e);
	}
	else if(typeid(*vog)==typeid(ValeurReelle) && typeid(*vod)==typeid(ValeurReelle))
	{
		double r=0,rog=((ValeurEntiere*)vog)->getValeur(),rod=((ValeurEntiere*)vod)->getValeur();

		delete vog;delete vod;

		if      (*(this->operateur)=="==") r = rog==rod;
		else if (*(this->operateur)=="!=") r = rog!=rod;
		else if (*(this->operateur)=="<") r = rog<rod;
		else if (*(this->operateur)==">") r = rog>rod;
		else if (*(this->operateur)=="<=") r = rog<=rod;
		else if (*(this->operateur)==">=") r = rog>=rod;
		else if (*(this->operateur)=="et") r = rog&&rod;
		else if (*(this->operateur)=="ou") r = rog||rod;

		return new ValeurReelle(r);
	}
	else if(typeid(*vog)==typeid(ValeurChaine) && typeid(*vod)==typeid(ValeurChaine))
	{
		string cog=((ValeurChaine*)vog)->getValeur(),cod=((ValeurChaine*)vod)->getValeur();

		delete vog;delete vod;

		long e;

		if      (*(this->operateur)=="==") e = cog==cod;
		else if (*(this->operateur)=="!=") e = cog!=cod;
		else
		{
			throw RunTimeError("Seul == et != marchent sur les chaines");
			return NULL; //HACK Pour le compilateur
		}
		return new ValeurEntiere(e);

	}
	else
	{
		throw RunTimeError("Comparaison impossible sur ces types");
		return NULL; //HACK pour le compilateur
	}
}

Valeur* NoeudSi::getValeur()
{
	Valeur* v;
	Valeur* cond=expBool->getValeur();
	if(typeid(*cond)==typeid(ValeurEntiere))
	{
		long conde=((ValeurEntiere*)cond)->getValeur();

		delete cond;

		if(conde)
			v=seqInstVrai->getValeur();
		else
			v=seqInstFaux->getValeur();

		return v;
	}
	else if(typeid(*cond)==typeid(ValeurReelle))
	{
		long condr=((ValeurReelle*)cond)->getValeur();

		delete cond;

		if(condr)
			v=seqInstVrai->getValeur();
		else
			v=seqInstFaux->getValeur();

		return v;
	}
	else
	{
		throw RunTimeError("Condition sur type pas entier impossible");
		return NULL; //HACK pour le compilateur
	}
}

Valeur* NoeudLire::getValeur()
{
	string c;
	cout << "<<";
	cin >> c;

	Valeur* v;
	double r=atof(c.c_str());
	if(c.find_first_of(".")!=string::npos) //FIXME On peut pas entrer 0 mais 0.0 marche
		v=new ValeurReelle(r);
	else if(r==0)
		v=new ValeurChaine(c);
	else
		v=new ValeurEntiere(r);

	((SymboleValue*)variable)->setValeur(v);
	return ((SymboleValue*)variable)->getValeur();
}

Valeur* NoeudEcrire::getValeur()
{
	Valeur* v = exp->getValeur();

	if(typeid(*v)==typeid(ValeurEntiere))
	{
		cout << ">>" << ((ValeurEntiere*)v)->getValeur() << endl;
		return v;
	}
	else if(typeid(*v)==typeid(ValeurReelle))
	{
		cout << ">>" << ((ValeurReelle*)v)->getValeur() << endl;
		return v;
	}
	else if(typeid(*v)==typeid(ValeurChaine))
	{
		cout << ">>" << ((ValeurChaine*)v)->getValeur() << endl;
		return v;
	}
	else
	{
		throw RunTimeError("Ecriture de type pas entier impossible");
		return NULL; //HACK pour le compilateur
	}
}


Valeur* NoeudTantQue::getValeur()
{
	Valeur* v=new ValeurIndefinie(); //NOTE pour simplifier l'algorithme
	Valeur* cond;
	long conde;
	double condr;
  bool continuer;

	if(!conditionDebut)
	{
		do
		{
			delete v;
			v= seqInst->getValeur();
			cond=expBool->getValeur();

			if(typeid(*cond)==typeid(ValeurEntiere))
			{
				conde=((ValeurEntiere*)cond)->getValeur();
				delete cond;
        continuer=conde;
			}
			else if(typeid(*cond)==typeid(ValeurReelle))
			{
				condr=((ValeurReelle*)cond)->getValeur();
				delete cond;
        continuer=condr;
			}
			else
			{
				throw RunTimeError("Condition sur type pas entier ou réel impossible");
				return NULL; //HACK pour le compilateur
			}
		}
		while(continuer);
	}
	else
	{
		cond=expBool->getValeur();

		if(typeid(*cond)==typeid(ValeurEntiere))
		{
			conde=((ValeurEntiere*)cond)->getValeur();
			delete cond;
      continuer=conde;
		}
		else if(typeid(*cond)==typeid(ValeurReelle))
		{
			condr=((ValeurReelle*)cond)->getValeur();
			delete cond;
      continuer=condr;
		}
		else
		{
			throw RunTimeError("Condition sur type pas entier ou réel impossible");
			return NULL; //HACK pour le compilateur
		}

		while(continuer)
		{
			delete v;
			v= seqInst->getValeur();
			cond=expBool->getValeur();

			if(typeid(*cond)==typeid(ValeurEntiere))
			{
				conde=((ValeurEntiere*)cond)->getValeur();
				delete cond;
        continuer=conde;
			}
			else if(typeid(*cond)==typeid(ValeurReelle))
			{
				condr=((ValeurReelle*)cond)->getValeur();
				delete cond;
        continuer=condr;
			}
			else
			{
				throw RunTimeError("Condition type pas entier ou réel impossible");
				return NULL; //HACK pour le compilateur
			}
		}
	}
	return v;
}
