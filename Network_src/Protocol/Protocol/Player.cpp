#include "Player.hpp"

Player::Player() : logged(false), slot(0) {}

void	Player::setSlot(int8 newVal)
{
	slot = newVal;
}

int8	Player::getSlot() const
{
	return slot;
}

void	Player::setLogged(bool status)
{
	logged = status;
}

bool	Player::isLogged() const
{
	return logged;
}
