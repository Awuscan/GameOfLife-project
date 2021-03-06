#pragma once
#include "Board.h"
#include <TGUI/TGUI.hpp>

class Game
{
public:
	Game();
	~Game();
	void start(); //inicjalizacja gry
	void play(); //rozpoczęcie
private:
	bool state = false; //status gry(w toku/pauza)
	int framerate = 10; 
	const int fieldSize = 7; //rozmiar pola
	sf::RenderWindow window; //objekt okna
	sf::View view; //objekt widoku w oknie
	Board board{ fieldSize }; //objekt planszy
	tgui::Label::Ptr gen;
	void menu(); //wyświetla menu startowe
	void loadMenu(tgui::Gui &gui);
	void randomize(tgui::EditBox::Ptr number);
	void playpause(tgui::BitmapButton::Ptr play, tgui::Slider::Ptr speed);
	void playspeed(tgui::Slider::Ptr speed);
	void playnext(tgui::BitmapButton::Ptr play, tgui::Slider::Ptr speed);
	void makePattern(tgui::ComboBox::Ptr patterns);
	void reset(tgui::BitmapButton::Ptr play, tgui::Slider::Ptr speed);
	int pattern = 0;
};