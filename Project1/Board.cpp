﻿#include "Board.h"

Board::Board(int size)
{
	sizeField = size;
}


Board::~Board()
{
}

void Board::initialize()
{
	int Y, X;
	{
		std::cout << "Podaj rozmiar horyzontalny: ";
		std::cin >> X;

		std::cout << "Podaj rozmiar wertykalny: ";
		std::cin >> Y;
	} while (this->initialize(Y, X) == false);

}

void Board::draw(sf::RenderWindow & window)
{
	for (int i = 1; i < sizeY+1; i++)
	{
		for (int j = 1; j < sizeX+1; j++)
		{
			grid[i][j].draw(window);
		}
	}
}

void Board::nextStep()
{
	copyGrid();
	int alive;

	for (int i = 1; i < sizeY+1; i++)	{
		for (int j = 1; j < sizeX+1; j++)	{
			alive = 0;
			for (int c = -1; c < 2; c++) {
				for (int d = -1; d < 2; d++) {
					int a = i + c, b = j + d;
			//		if (!(c == 0 && d == 0) && ((a >= 0 && a < sizeY) && (b >= 0 && b < sizeX))) {
					if (!(c == 0 && d == 0)){
						if (gridCopy[a][b].getState()) {
							++alive;
						}
					}
				}
			}

			if (alive < 2) {
				grid[i][j].changeState(false);
			}
			else if (alive == 3) {
				grid[i][j].changeState(true);
			}
			else if (alive > 3)	{
				grid[i][j].changeState(false);
			}
		}
	}
}

bool Board::initialize(int Y, int X)
{
	if (X < 1 || Y < 1)
	{
		std::cout << std::endl;
		std::cout << "Niepoprawne dane!" << std::endl;
		std::cout << std::endl;
		return false;
	}
	else {
		sizeX = X;
		sizeY = Y;

		std::cout << std::endl;
		std::cout << "Stworzono plansze." << std::endl;
		std::cout << std::endl;



		for (int i = 0; i < sizeY+2; i++) {
			std::vector<Field> tmp;
			for (int j = 0; j < sizeX+2; j++) {
				Field field(j, i, sizeField);
				tmp.push_back(field);
			}
			grid.push_back(tmp);
		}

		return true;
	}
}

void Board::fillOut(sf::RenderWindow& window,sf::View& view)
{
	sf::Vector2f posOnBoard;
	sf::Vector2i posMouse;

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed: //obsługuje zamknięcie okna
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Escape) { //zamyka okno
					window.close();
				}
				else if (event.key.code == sf::Keyboard::A) { //przesuwa widok w prawo
					view.move(-109.f, 0.f);
				}
				else if (event.key.code == sf::Keyboard::W) { //przesuwa widok w górę
					view.move(0.f, -109.f);
				}
				else if (event.key.code == sf::Keyboard::D) { //przesuwa widok w lewo
					view.move(109.f, 0.f);
				}
				else if (event.key.code == sf::Keyboard::S) { //przesuwa widok w dół
					view.move(0.f, 109.f);
				}
				else if (event.key.code == sf::Keyboard::E) { //powiększa widok 2x
					view.zoom(8 / 4.f);
				}
				else if (event.key.code == sf::Keyboard::Q) { //zmniejsza widok 2x
					view.zoom(2 / 4.f);
				}
				else if (event.key.code == sf::Keyboard::Enter) {
					return;
				}
				window.setView(view);
				window.clear();
				break;
			case sf::Event::Resized: //obsługuje zmiane rozmiaru okna
				view.setSize(event.size.width, event.size.height);
				window.setView(view);
				window.clear();
				break;		
			case sf::Event::MouseButtonPressed: 
				posMouse = sf::Mouse::getPosition(window);
				posOnBoard = window.mapPixelToCoords(posMouse);
				int x = (int)(posOnBoard.x) / (sizeField + 1);
				int y = (int)(posOnBoard.y) / (sizeField + 1);
				std::cout << "plansza " << posOnBoard.x << " " << posOnBoard.y << std::endl;
				std::cout << "mysz " << posMouse.x << " " << posMouse.y << std::endl;
				std::cout << "kliksz na objekt o poz: " << x << " " << y << std::endl;
				if (x > 0 && x <= sizeX && y > 0 && y <= sizeY) {
					if (grid[y][x].getState()) {
						grid[y][x].changeState(false);
					}
					else {
						grid[y][x].changeState(true);
					}
				}
				draw(window);
				window.display();
			}
			 
			
			//else if(event.key.code == sf::Keyboard::F1){
			//	randomize();
			//	draw(window);
			//	window.display();
			//}
		}
	}
}

void Board::randomize()
{
	srand(time(NULL));
	int randSeedV, randSeedH;
	int i,n = 0;

	std::cout << "Podaj: ";
	std::cin >> i;

	while(i > sizeX*sizeY) {
		std::cout << "Za dużo" << std::endl;
		std::cout << "Podaj: ";
		std::cin >> i;
	}

	while (n < i)
	{
		randSeedV = rand() % sizeY +1;
		randSeedH = rand() % sizeX +1;
		if (!grid[randSeedV][randSeedH].getState())
		{
			grid[randSeedV][randSeedH].changeState(true);
			n++;
		}
	}
}

int Board::getSizeX()
{
	return sizeX;
}

int Board::getSizeY()
{
	return sizeY;
}

void Board::copyGrid()
{
	gridCopy = grid;
}
