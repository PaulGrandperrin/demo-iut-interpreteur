#ifndef ANALYSECARACTERE_HPP
#define ANALYSECARACTERE_HPP

#include <fstream>
using namespace std;

class LecteurCaractere
{

public :
	LecteurCaractere ( string nomFich );
	inline char         getCarCour(){return carCour;};
	inline unsigned int getLigne(){return ligne;};
	inline unsigned int getColonne(){return colonne;};
	void suivant();

private :
	ifstream f;
	char carCour;
	unsigned int ligne;
	unsigned int colonne;
};

#endif /* ANALYSECARACTERE_HPP */
