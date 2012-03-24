ObjInterpreteur = AnalyseCaractere.o AnalyseLexicale.o AnalyseSyntaxique.o ArbreSyntaxiqueAbstrait.o Jeton.o Lexeme.o TableJeton.o VisiteurArbreSyntaxiqueAbstrait.o

all: interpreteur afficheArbre compilateur

clean:
	rm -f $(ObjInterpreteur) Interpreteur.o afficheArbre.o compilateur.o interpreteur afficheArbre compilateur gmon.out

AnalyseCaractere.o: AnalyseCaractere.cpp AnalyseCaractere.hpp exception.hpp
	$(CC) $(CFLAGS) -c AnalyseCaractere.cpp

AnalyseLexicale.o: AnalyseLexicale.cpp AnalyseLexicale.hpp AnalyseCaractere.hpp Lexeme.hpp exception.hpp
	$(CC) $(CFLAGS) -c AnalyseLexicale.cpp

AnalyseSyntaxique.o:AnalyseSyntaxique.cpp AnalyseSyntaxique.hpp AnalyseLexicale.hpp ArbreSyntaxiqueAbstrait.hpp TableJeton.hpp exception.hpp  Valeur.hpp
	$(CC) $(CFLAGS) -c AnalyseSyntaxique.cpp

ArbreSyntaxiqueAbstrait.o: ArbreSyntaxiqueAbstrait.cpp ArbreSyntaxiqueAbstrait.hpp Jeton.hpp Lexeme.hpp Valeur.hpp exception.hpp  Valeur.hpp
	$(CC) $(CFLAGS) -c ArbreSyntaxiqueAbstrait.cpp

Jeton.o: Jeton.cpp Jeton.hpp  exception.hpp  Valeur.hpp
	$(CC) $(CFLAGS) -c Jeton.cpp

Lexeme.o: Lexeme.cpp Lexeme.hpp  exception.hpp
	$(CC) $(CFLAGS) -c Lexeme.cpp

TableJeton.o: TableJeton.cpp TableJeton.hpp Jeton.hpp exception.hpp  Valeur.hpp
	$(CC) $(CFLAGS) -c TableJeton.cpp

Interpreteur.o: Interpreteur.cpp AnalyseSyntaxique.hpp exception.hpp
	$(CC) $(CFLAGS) -c Interpreteur.cpp

afficheArbre.o: afficheArbre.cpp AnalyseSyntaxique.hpp exception.hpp
	$(CC) $(CFLAGS) -c afficheArbre.cpp

compilateur.o: compilateur.cpp AnalyseSyntaxique.hpp exception.hpp
	$(CC) $(CFLAGS) -c compilateur.cpp

VisiteurArbreSyntaxiqueAbstrait.o: VisiteurArbreSyntaxiqueAbstrait.cpp VisiteurArbreSyntaxiqueAbstrait.hpp ArbreSyntaxiqueAbstrait.hpp Jeton.hpp exception.hpp  Valeur.hpp
	$(CC) $(CFLAGS) -c VisiteurArbreSyntaxiqueAbstrait.cpp


interpreteur: $(ObjInterpreteur) Interpreteur.o
	$(CC) $(LDFLAGS) -o interpreteur $(ObjInterpreteur) Interpreteur.o

afficheArbre: $(ObjInterpreteur) afficheArbre.o
	$(CC) $(LDFLAGS) -o afficheArbre $(ObjInterpreteur) afficheArbre.o

compilateur: $(ObjInterpreteur) compilateur.o
	$(CC) $(LDFLAGS) -o compilateur $(ObjInterpreteur) compilateur.o
