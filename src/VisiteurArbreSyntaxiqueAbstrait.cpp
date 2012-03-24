#include "VisiteurArbreSyntaxiqueAbstrait.hpp"





string ecrireNoeud()
{
	return "\x1b" "[01;33;36m" "Noeud" "\x1b" "[00;33;00m";
}

string ecrireFeuille()
{
	return "\x1b" "[33;34m" "Feuille" "\x1b" "[33;00m";
}


VisiteurAffichage::VisiteurAffichage()
{
	indentation=0;
}

bool VisiteurAffichage::visitEnter ( NoeudSeqInst* noeud )
{
	cout << setw ( 4*indentation ) << " " << ecrireNoeud() <<" - Sequence de " "\x1b" "[01;33;33m" << noeud->getTabInst().size() << "\x1b" "[00;33;00m"" instruction(s)" << endl;
	indentation++;
	return true;
}
bool VisiteurAffichage::visitEnter ( NoeudAffectation* )
{
	cout << setw ( 4*indentation ) << " " << ecrireNoeud() <<" - Affectation" << endl;
	indentation++;
	return true;
}
bool VisiteurAffichage::visitEnter ( NoeudOperateurBinaire* )
{
	cout << setw ( 4*indentation ) << " " << ecrireNoeud() <<" - Operateur Binaire " << endl;
	indentation++;
	return true;
}
bool VisiteurAffichage::visitEnter ( NoeudOperateurBooleen* )
{
	cout << setw ( 4*indentation ) << " " << ecrireNoeud() <<" - Operateur Booleen " << endl;
	indentation++;
	return true;
}
bool VisiteurAffichage::visitEnter ( NoeudSi* )
{
	cout << setw ( 4*indentation ) << " " << ecrireNoeud() <<" - Si " << endl;
	indentation++;
	return true;
}
bool VisiteurAffichage::visitEnter ( NoeudTantQue* noeud )
{
	cout << setw ( 4*indentation ) << " " << ecrireNoeud() <<" - Tant Que avec condition " << ( noeud->getConditionDebut() ?"au debut":"à la fin" ) << endl;
	indentation++;
	return true;
}
bool VisiteurAffichage::visitEnter ( NoeudLire* )
{
	cout << setw ( 4*indentation ) << " " << ecrireNoeud() <<" - Lire : " <<endl;
	indentation++;
	return true;
}
bool VisiteurAffichage::visitEnter ( NoeudEcrire* )
{
	cout << setw ( 4*indentation ) << " " << ecrireNoeud() <<" - Ecrire : "<< endl;
	indentation++;
	return true;
}

//---------------------------------------------------------------------------

bool VisiteurAffichage::visitLeave ( NoeudSeqInst* )
{
	indentation--;
	return true;
}

bool VisiteurAffichage::visitLeave ( NoeudAffectation* )
{
	indentation--;
	return true;
}

bool VisiteurAffichage::visitLeave ( NoeudOperateurBinaire* )
{
	indentation--;
	return true;
}

bool VisiteurAffichage::visitLeave ( NoeudOperateurBooleen*)
{
	indentation--;
	return true;
}

bool VisiteurAffichage::visitLeave ( NoeudSi* )
{
	indentation--;
	return true;
}

bool VisiteurAffichage::visitLeave ( NoeudTantQue* )
{
	indentation--;
	return true;
}

bool VisiteurAffichage::visitLeave ( NoeudLire* )
{
	indentation--;
	return true;
}

bool VisiteurAffichage::visitLeave ( NoeudEcrire* )
{
	indentation--;
	return true;
}

//--------------------------------------------------------------------------------------------------------------------


bool VisiteurAffichage::visit ( SymboleValue* noeud )
{
	cout << setw ( 4*indentation ) << " " <<  ecrireFeuille() <<" - Symbole value : "  "\x1b" "[01;33;31m"<< noeud->getChaine() <<"\x1b" "[00;33;00m"<< endl;
	return true;
}
bool VisiteurAffichage::visit ( Symbole* noeud )
{
	cout << setw ( 4*indentation ) << " " <<  ecrireFeuille() <<" - Symbole : " "\x1b" "[33;31m"<< noeud->getChaine() <<"\x1b" "[33;00m"<< endl;
	return true;
}



//##################################################################################################################################
//##################################################################################################################################
//##################################################################################################################################
//##################################################################################################################################

string VisiteurGenerationC::nline()
{
	string s="\n";
	for ( int i=0; i<indentation;i++ )
	{
		s+="\t";
	}
	return s;
}

VisiteurGenerationC::VisiteurGenerationC()
{
	indentation=1;

}

string VisiteurGenerationC::getCode ( TableSymboles ts )
{
	string s;
	s+="#include <stdio.h>\n#include <stdlib.h>\nmain()\n{\n";

	for ( unsigned int i=0; i<ts.getTable().size(); i++ )
	{
		if ( * ( ts.getTable() [i] ) =="<VARIABLE>" )
		{
			s+= "\tlong " + ts.getTable() [i]->getChaine() +"=0;\n";
		}
	}

	s+="\n\t"+code.top();

	s+="\n}\n\n";
	return s;


}

bool VisiteurGenerationC::visitEnter ( NoeudSeqInst* )
{
	indentation++;
	return true;
}
bool VisiteurGenerationC::visitEnter ( NoeudAffectation* )
{
	return true;
}
bool VisiteurGenerationC::visitEnter ( NoeudOperateurBinaire* )
{
	return true;
}
bool VisiteurGenerationC::visitEnter ( NoeudOperateurBooleen*)
{
	return true;
}
bool VisiteurGenerationC::visitEnter ( NoeudSi* )
{
	return true;
}
bool VisiteurGenerationC::visitEnter ( NoeudTantQue*)
{
	return true;
}
bool VisiteurGenerationC::visitEnter ( NoeudLire* )
{
	return true;
}
bool VisiteurGenerationC::visitEnter ( NoeudEcrire* )
{
	return true;
}

//---------------------------------------------------------------------------

bool VisiteurGenerationC::visitLeave ( NoeudSeqInst* noeud )
{
	string s;
	for ( unsigned int i=0;i<noeud->getTabInst().size();i++ )
	{
		s=nline() +code.top() +s;
		code.pop();
	}
	s="{"+s;
	indentation--;
	s+=nline() +"}";
	code.push ( s );

	return true;
}

bool VisiteurGenerationC::visitLeave ( NoeudAffectation* )
{
	string s,var,exp;
	exp=code.top();
	code.pop();
	var=code.top();
	code.pop();
	s+=var;
	s+="=";
	s+=exp;
	s+=";";
	code.push ( s );
	return true;
}

bool VisiteurGenerationC::visitLeave ( NoeudOperateurBinaire* )
{
	string s,o,g,d;
	d=code.top();
	code.pop();
	g=code.top();
	code.pop();
	o=code.top();
	code.pop();
	s+="(";
	s+=g;
	s+=o;
	s+=d;
	s+=")";
	code.push ( s );
	return true;
}

bool VisiteurGenerationC::visitLeave ( NoeudOperateurBooleen* )
{
	string s,o,g,d;
	d=code.top();
	code.pop();
	g=code.top();
	code.pop();
	o=code.top();
	code.pop();
	s+="(";
	s+=g;
	s+=o;
	s+=d;
	s+=")";
	code.push ( s );
	return true;
}

bool VisiteurGenerationC::visitLeave ( NoeudSi*  )
{
	string s,c,v,f;
	f=code.top();
	code.pop();
	v=code.top();
	code.pop();
	c=code.top();
	code.pop();
	s+="if (";
	s+=c;
	s+=")"+nline();
	s+=v;
	s+=nline() +"else"+nline();
	s+=f;
	code.push ( s );
	return true;
}

bool VisiteurGenerationC::visitLeave ( NoeudTantQue* noeud )
{
	string s,c,i;
	i=code.top();
	code.pop();
	c=code.top();
	code.pop();
	if ( noeud->getConditionDebut() )
	{
		s+="while (";
		s+=c;
		s+=")"+nline();
		s+=i;
	}
	else
	{
		s+="do"+nline();
		s+=i;
		s+=nline() +"while (";
		s+=c;
		s+=");";
	}
	code.push ( s );
	return true;
}

bool VisiteurGenerationC::visitLeave ( NoeudLire* )
{
	string s, v;
	v=code.top();
	code.pop();
	s+="scanf(\"%ld\",&";
	s+=v;
	s+=");";
	code.push ( s );
	return true;
}

bool VisiteurGenerationC::visitLeave ( NoeudEcrire*)
{
	string s, v;
	v=code.top();
	code.pop();
	s+="printf(\"%ld\\n\",";
	s+=v;
	s+=");fflush(stdout);";
	code.push ( s );
	return true;
}

//--------------------------------------------------------------------------------------------------------------------


bool VisiteurGenerationC::visit ( SymboleValue* noeud )
{
	code.push ( noeud->getChaine() );
	return true;
}
bool VisiteurGenerationC::visit ( Symbole* noeud )
{
	if ( noeud->getChaine() =="et" )
		code.push ( "&&" );
	else if ( noeud->getChaine() =="ou" )
		code.push ( "||" );
	else
		code.push ( noeud->getChaine() );
	return true;
}

