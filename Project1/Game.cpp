﻿#include "Game.h"
#include <string> 

Game::Game()
{
	//tworzenie okna
	sf::ContextSettings settings;
	settings.antialiasingLevel = 1;
	window.create(sf::VideoMode(300, 480), "Gra w zycie", sf::Style::Close, settings);
	window.setFramerateLimit(framerate);
	sf::Image icon;
	icon.loadFromFile("icon.png");
	window.setIcon(64, 64, icon.getPixelsPtr());
}

Game::~Game()
{
}

void Game::start()
{
	window.clear();

	//wyświetlenie menu
	menu();

	//zmiana rozmiaru okna 
	int y = board.getSizeY(), x = board.getSizeX();
	int h = (y + 2) * (fieldSize+1) - 1, w = (x + 2) * (fieldSize+1) - 1;
	int windowHeight = sf::VideoMode::getDesktopMode().height, 
		windowWidth = sf::VideoMode::getDesktopMode().width;
	int desktopHeight = sf::VideoMode::getDesktopMode().height, 
		desktopWidth = sf::VideoMode::getDesktopMode().width;

	if (w > (windowWidth - 100) || h > (windowHeight - 200)) {
		windowWidth -= 100;
		windowHeight -= 200;
	}else if (h < 200 ) {
		windowHeight = 410;
	}else {
		windowHeight = h;
	}	
	if (w > (windowWidth - 100)) {
		windowWidth -= 100;
	}else if (w < 410 || h < 200 ) {
		windowWidth = 410;
	}else {
		windowWidth = w;
	}

	windowHeight += 100;


	window.setSize(sf::Vector2u(windowWidth, windowHeight));
	window.clear();
	
	int px = desktopWidth / 2 - windowWidth / 2;
	int py = desktopHeight / 2 - windowHeight / 2 -30;

	sf::Vector2i position(px,py);
	window.setPosition(position);


	//tworzenie widoku
	w = (w / 2) - (windowWidth / 2);
	h = (h / 2) - (windowHeight / 2);
	view.reset(sf::FloatRect(w, h-50, windowWidth, windowHeight));

	//wyświetlenie planszy
	window.setView(view);
	window.requestFocus();
	board.draw(window);
	window.display();
}

void Game::play()
{
	sf::Clock clock;
	float lastTime = 0;

	tgui::Gui gui(window);
	gui.setView(view);
	loadMenu(gui);

	sf::Vector2f posOnBoard;
	sf::Vector2i posMouse;
	
	sf::Event event;
	while (window.isOpen()) {

		float currentTime = clock.restart().asSeconds();
		float fps = 1.f / (currentTime - lastTime);
		lastTime = currentTime;

		while (window.pollEvent(event)) {
			gui.handleEvent(event);
			switch (event.type) {
			case sf::Event::Closed: //obsługuje zamknięcie okna
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Space) { //pauza
					state = !state;
				}
				else if (event.key.code == sf::Keyboard::Escape) { //zamyka okno
					window.close();
				}
				else if (event.key.code == sf::Keyboard::F1) { pattern = 1;	}
				else if (event.key.code == sf::Keyboard::F2) { pattern = 2;	}
				else if (event.key.code == sf::Keyboard::F3) { pattern = 3;	}
				else if (event.key.code == sf::Keyboard::F4) { pattern = 4;	}
				else if (event.key.code == sf::Keyboard::F5) { pattern = 5;	}
				else if (event.key.code == sf::Keyboard::A) { view.move(-109.f, 0.f); }	//przesuwa widok w prawo
				else if (event.key.code == sf::Keyboard::W) { view.move(0.f, -109.f); }	//przesuwa widok w górę
				else if (event.key.code == sf::Keyboard::D) { view.move(109.f, 0.f); }	//przesuwa widok w lewo
				else if (event.key.code == sf::Keyboard::S) { view.move(0.f, 109.f); }	//przesuwa widok w dół
				else if (event.key.code == sf::Keyboard::E) { view.zoom(8 / 4.f); }		//powiększa widok 2x
				else if (event.key.code == sf::Keyboard::Q) { view.zoom(2 / 4.f); }		//zmniejsza widok 2x
				window.setView(view);
				break;
			case sf::Event::MouseButtonPressed: 
				posMouse = sf::Mouse::getPosition(window);
				posOnBoard = window.mapPixelToCoords(posMouse);
				int x = (int)(posOnBoard.x) / (fieldSize + 1);
				int y = (int)(posOnBoard.y) / (fieldSize + 1);
				if (pattern != 0) {
					board.patterns(pattern, x, y);
					pattern = 0;
				}else {
					if (x > 0 && x <= board.getSizeX() && y > 0 && y <= board.getSizeY())
						board.changeFieldState(x, y);
				}
				break;
			}
		}
		
		if (state) { //następny krok
			board.nextStep();
		}
		gen->setText(std::to_string(board.getGen()) + "\n " + std::to_string((int)(1 / lastTime)));

		window.clear();
		board.draw(window);
		gui.draw();
		window.display();
	}
}

void Game::menu()
{
	//tworzenie gui
	tgui::Gui gui(window);
    tgui::Theme theme{ "themes/Black.txt" };

	auto sliderHeight = tgui::Slider::create();
	sliderHeight->setRenderer(theme.getRenderer("Slider"));
	sliderHeight->setPosition({ "15%", "25%" });
	sliderHeight->setMinimum(1);
	sliderHeight->setMaximum(500);
	sliderHeight->setSize({ "70%", "3%" });//setSize(200, 18);
	sliderHeight->setValue(100);
	gui.add(sliderHeight);

	auto labelHeight = tgui::Label::create();
	labelHeight->setRenderer(theme.getRenderer("Label"));
	labelHeight->setText("Board Height: " + std::to_string((int)sliderHeight->getValue()));
	labelHeight->setPosition({ "16%", "20%" });
	labelHeight->setTextSize(18);
	gui.add(labelHeight);

	auto sliderWidth = tgui::Slider::copy(sliderHeight);
	sliderWidth->setPosition({ "16%", "45%" });
	sliderWidth->setValue(180);
	gui.add(sliderWidth);

	auto labelWidth = tgui::Label::copy(labelHeight);
	labelWidth->setText("Board Width: " + std::to_string((int)sliderWidth->getValue()));
	labelWidth->setPosition({ "16%", "40%" });
	gui.add(labelWidth);

	auto button = tgui::Button::create("START");
	button->setRenderer(theme.getRenderer("Button"));
	button->setSize({ "50%", "16.67%" });
	button->setPosition({ "25%", "70%" });
	gui.add(button);

	while (window.isOpen())
	{
		int y = (int)sliderHeight->getValue();
		int x = (int)sliderWidth->getValue();
		labelHeight->setText("Board Height: " + std::to_string(y));
		labelWidth->setText("Board Width: " + std::to_string(x));

		sf::Event event;
		while (window.pollEvent(event))	{
			gui.handleEvent(event);
			if (event.type == sf::Event::Closed) {
				window.close();
			}else if (event.type == sf::Event::MouseButtonReleased) {
				sf::Vector2i posMouse = sf::Mouse::getPosition(window);
				sf::Vector2f pos = sf::Vector2f(posMouse);
				if (button->mouseOnWidget(pos)) {
					board.initialize(y,x);
					return;
				}
			}
		}

		window.clear();
		gui.draw();
		window.display();
	}

}

void Game::loadMenu(tgui::Gui &gui)
{
	tgui::Theme theme{ "themes/Black.txt" };

	int sizeY = board.getSizeY();
	int sizeX = board.getSizeX();
	int h = (sizeY + 2) * (fieldSize + 1) - 1;
	int w = (sizeX + 2) * (fieldSize + 1) - 1;

	int windowHeight = window.getSize().y;
	int windowWidth = window.getSize().x;
	
	int y = (h / 2) - (windowHeight / 2);
	int x = (w / 2) - (windowWidth / 2);

	auto panel = tgui::Panel::create("panel");
	panel->setSize({ "100%",100 });
	panel->setRenderer(theme.getRenderer("Panel"));
	panel->setPosition({ x,y-50 });
	gui.add(panel);

	auto patterns = tgui::ComboBox::create();
	patterns->setRenderer(theme.getRenderer("comboBox"));
	patterns->setSize(200, 30);
	patterns->setPosition(50 + x, y - 40);
	patterns->addItem("Gospel Glider Gun", "1");
	patterns->addItem("Garden of Eden 1", "2");
	patterns->addItem("Flower of Eden 5", "3");
	patterns->addItem("Own Pattern 1", "4");
	patterns->addItem("Own Pattern 2", "5");
	patterns->setSelectedItem("Gospel Glider Gun");
	gui.add(patterns);

	auto button = tgui::Button::create("");
	button->setRenderer(theme.getRenderer("Button"));
	button->setSize({ 30, 30 });
	button->setPosition({ 10 + x, y - 40 });
	button->connect("pressed", &Game::makePattern, this, patterns);
	gui.add(button);

	auto randNumber = tgui::EditBox::create();
	randNumber->setRenderer(theme.getRenderer("EditBox"));
	randNumber->setSize({ 50, 30 });
	randNumber->setPosition({ 260 + x, y - 40 });
	randNumber->setInputValidator(tgui::EditBox::Validator::Int);
	gui.add(randNumber);

	auto rand = tgui::Button::create("Randomize");
	rand->setRenderer(theme.getRenderer("Button"));
	rand->setSize({ 80, 30 });
	rand->setPosition({ 320 + x, y - 40 });
	rand->connect("pressed", &Game::randomize, this, randNumber);
	gui.add(rand);

	auto speed = tgui::Slider::create();
	speed->setRenderer(theme.getRenderer("Slider"));
	speed->setSize({ 120, 16 });
	speed->setPosition({ 70 + x, y + 12 });
	speed->setMinimum(1);
	speed->setMaximum(120);
	speed->setValue(framerate);
	speed->connect("ValueChanged", &Game::playspeed, this, speed);
	gui.add(speed);

	auto play = tgui::BitmapButton::create();
	play->setRenderer(theme.getRenderer("Button"));
	play->setSize({ 40, 40 });
	play->setPosition({ 10 + x, y });
	play->setImage("themes/icons/play.png");
	play->setImageScaling(0.8);
	play->connect("pressed", &Game::playpause, this, play, speed);
	gui.add(play);

	auto next = tgui::BitmapButton::create();
	next->setRenderer(theme.getRenderer("Button"));
	next->setSize({ 40, 40 });
	next->setPosition({ 210 + x, y  });
	next->setImage("themes/icons/next.png");
	next->setImageScaling(0.8);
	next->connect("pressed", &Game::playnext, this, play, speed);
	gui.add(next);

	auto reset = tgui::Button::create("Reset");
	reset->setRenderer(theme.getRenderer("Button"));
	reset->setSize({ 50, 40 });
	reset->setPosition({ 260 + x, y });
	reset->connect("pressed", &Game::reset, this, play, speed);
	gui.add(reset);

	gen = tgui::Label::create();
	gen->setTextSize(16);
	gen->setPosition({ 320 + x, y});
	gen->setText(std::to_string(board.getGen()));
	gen->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Right);
	gen->setSize({ 80,40 });
	gui.add(gen);
}

void Game::randomize(tgui::EditBox::Ptr number)
{
	std::string str;
	str = number->getText().toAnsiString();
	if (str == "") return;
	int x = std::stoi(str);
	if(x!=NAN)board.randomize(x);
}

void Game::playpause(tgui::BitmapButton::Ptr play, tgui::Slider::Ptr speed)
{
	if(state){
		state = false;
		play->setImage("themes/icons/play.png");
		framerate = 10;
		speed->setValue(10);
	}else {
		state = true;
		play->setImage("themes/icons/pause.png");
		framerate = 10;
		speed->setValue(10);
	}
}

void Game::playspeed(tgui::Slider::Ptr slider)
{
	framerate = slider->getValue();
	window.setFramerateLimit(framerate);
}

void Game::playnext(tgui::BitmapButton::Ptr play, tgui::Slider::Ptr speed)
{
	state = false;
	board.nextStep();
	play->setImage("themes/icons/play.png");
	framerate = 10;
	gen->setText(std::to_string(board.getGen()) + "\n ");
	speed->setValue(10);
}

void Game::makePattern(tgui::ComboBox::Ptr patterns)
{
	std::string name = patterns->getSelectedItemId();
	pattern = std::stoi(name);
}

void Game::reset(tgui::BitmapButton::Ptr play, tgui::Slider::Ptr speed)
{
	play->setImage("themes/icons/play.png");
	board.reset();
	gen->setText("0");
	state = false;
	framerate = 10;
	speed->setValue(10);
}
