#pragma once
#include <vector>
#include <iostream>
#include "Field.h"

class Board
{
public:
	Board(int size);
	~Board();
	void initialize(int Y, int X); //tworzy plansze o podanych rozmiarze
	void draw(sf::RenderWindow& window); // rysuje plansze
	void nextStep(); //oblicza nastepny krok
	void changeFieldState(int x, int y); //zmienia stan wybranego pola
	void randomize(int n); // wype�nienie n p�l na planszy losowo
	int getSizeX() { return sizeX; }; //zwraca szeroko�� planszy 
	int getSizeY() { return sizeY; }; //zwraca wysoko�� planszy
	int getGen() { return generation; }; //zwraca numer generacji
	void patterns(int pattern, int x, int y); //wstawia wybrany wz�r 
	void reset(); //resetuje plansze
private:
	const bool wrap = true; //zawijanie krawedzi
	int sizeX, sizeY; //rozmiar planszy
	int fieldSize; //rozmiar pola w px (+1px ramki )
	unsigned int generation = 0; //numer iteracji
	std::vector<std::vector<Field>> grid; //pola planszy
	std::vector<std::vector<Field>> gridCopy; //kopia p�l planszy dla wyznaczania nast�pnej generacji
	void copyGrid() {gridCopy = grid;}; //kopiuje plansze grid do gridCopy
	static const int tab[418];
	static const int tab2[396];
	static const int tab3[169];
	static const int tab4[126];
	static const int tab5[143];
};