#pragma once

# include "TerrariaInfo.hpp"

class Player
{
private:
	bool	logged;
	int8	slot;

public:

	Player();
	Player(const Player&) = default;

	void	setSlot(int8 newVal);
	int8	getSlot() const;

	void	setLogged(bool status);
	bool	isLogged() const;
};