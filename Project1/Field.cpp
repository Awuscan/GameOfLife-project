#include "Field.h"



Field::Field(int x, int y, int size): coordX(x), coordY(y), state(false)
{
	block.setSize(sf::Vector2f(size, size));
	block.setFillColor(colorDead);
	block.setPosition(size*coordX+ coordX, size*coordY+ coordY);
}


Field::~Field()
{
}

void Field::reset()
{
	block.setFillColor(colorDead);
	wasAlive = false;
	state = false;
}

Field & Field::operator=(const bool & newState)
{
	state = newState;
	if (state) {
		block.setFillColor(colorAlive);
		wasAlive = true;
	}else {
		if (wasAlive) {
			block.setFillColor(colorWasAlive);
		}else {
			block.setFillColor(colorDead);
		}
	}
	return *this;
}

bool Field::operator=(const Field & p)
{
	state = p;
	return state;
}