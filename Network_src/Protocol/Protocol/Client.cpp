#include <iostream>
#include <stdexcept>
#include <cstring>
#include <cassert>
#include "Client.hpp"

Client::Client(const char* host, short port):
	io_service(), connection(*this, io_service, host, port), msgFactory() {}

/*
**	Starts asynchronous I/O if asynchronous sends/receives are performed
**	Runs server connection process (synchronous)
*/
void	Client::run()
{
	loginProcess();
	connection.responseExpect();
	io_service.run();
}

/*
**	Stops asynchronous tasks
*/
void	Client::stop()
{
	io_service.stop();
}

void	Client::notify(MessagePtr msg)
{
	if (!filterResponse(msg))
		responseQueue.push(msg);
}

/*
**	Yields the server responses queue
*/
MsgQueue&	Client::getResponseQueue()
{
	return responseQueue;
}

MsgFactory&	Client::getMsgFactory()
{
	return msgFactory;
}

/*****************************************************************************
**	PRIVATE
*****************************************************************************/

bool	Client::filterResponse(MessagePtr msg)
{
	bool	filtered = false;

	if (!player.isPlaying())
	{
		if (msg->type == static_cast<uint8>(MsgType::SPAWN))
		{
			spawn(*(static_cast<Spawn *>(msg.get())));
			filtered = true;
		}
	}
	return filtered;
}

/*
**	Requests spawn (asynchronous)
*/
void	Client::spawn(const Spawn& sp)
{
	MessagePtr		msg = msgFactory.makeRequest(MsgType::SPAWN_PLAYER);
	SpawnPlayer*	s = static_cast<SpawnPlayer*>(msg.get());

	std::cout << "Spawn request...";
	std::flush(std::cout);
	s->playerSlot = player.getSlot();
	s->spawnX = sp.spawnX;
	s->spawnY = sp.spawnY;

	connection.request(msg, sizeof(*s));
	player.setPlaying(true);
	std::cout << "Spawn request..." << std::endl;
}

/*
**	Launches login to server process (synchronous)
*/
void	Client::loginProcess()
{
	login();
	appearanceSet();
	//keySend();
	playerConfiguration();
	inventorySet();
	worldInfoFetch();
}

void	Client::login()
{
	std::cout << "Loggin... ";
	std::flush(std::cout);
	{
		Connect	c;
		c.type = static_cast<uint8>(MsgType::CONNECT);

		std::strncpy(c.version, TERR_CLIENT_VERSION, sizeof(c.version));
		connection.syncRequest(&c, sizeof(c));
	}

	Accepted	a;
	connection.syncResponse(&a, sizeof(a), MsgType::ACCEPTED);

	player.setLogged(true);
	player.setSlot(static_cast<uint8>(a.playerSlot));
	std::cout << "\tSUCCESS" << std::endl;
}

void	Client::appearanceSet()
{
	Appearance	a;
	a.playerSlot = player.getSlot();
	a.type = static_cast<uint8>(MsgType::APPEARANCE);
	a.difficulty = static_cast<uint8>(Difficulty::NORMAL);
	std::strncpy(a.playerName, TERR_PLAYER_NAME, sizeof(a.playerName));

	a.hairStyle = 1; // random
	a.gender = Gender::MALE;
	RGB::setColor(a.hair, RGB::BLACK);
	RGB::setColor(a.skin, RGB::GREEN);
	RGB::setColor(a.eye, RGB::PINK);
	RGB::setColor(a.shirt, RGB::YELLOW);
	RGB::setColor(a.undershirt, RGB::WHITE);
	RGB::setColor(a.pants, RGB::PURPLE);
	RGB::setColor(a.shoes, RGB::BLUE);

	std::cout << "Setting difficulty and player appearance...";
	std::flush(std::cout);
	connection.syncRequest(&a, sizeof(a));
	std::cout << "\t SUCCESS" << std::endl;
}

void	Client::keySend()
{
	Key	k;

	std::cout << "Sending magic key...";
	std::flush(std::cout);
	k.type = static_cast<uint8>(MsgType::AUTH_KEY);
	std::strncpy(k.unknown_key, TERR_KEY, sizeof(k.unknown_key));
	connection.syncRequest(&k, sizeof(k));
	std::cout << "\t SUCCESS" << std::endl;
}

void	Client::playerConfiguration()
{
	Life		l;
	Mana		m;
	Buffs		b;

	std::cout << "Configuring player status..." << std::endl;

	l.playerSlot = player.getSlot();
	l.type = static_cast<uint8>(MsgType::PLAYER_LIFE);
	l.currLife = 100;
	l.maxLife = 100;

	m.playerSlot = player.getSlot();
	m.type = static_cast<uint8>(MsgType::PLAYER_MANA);
	m.manaLevel = 20;
	m.maxMana = 20;

	b.playerSlot = player.getSlot();
	b.type = static_cast<uint8>(MsgType::PLAYER_BUFFS);
	std::memset(b.buffTypes, 0, sizeof(b.buffTypes));

	std::cout << "\tsetting player life" << std::endl;
	connection.syncRequest(&l, sizeof(l));
	std::cout << "\tsetting player mana" << std::endl;
	connection.syncRequest(&m, sizeof(m));
	std::cout << "\tsetting player buffs" << std::endl;
	connection.syncRequest(&b, sizeof(b));
	std::cout << "SUCCESS: Player configured" << std::endl;
}

void	Client::inventorySet()
{
	InventoryItem	request;

	std::cout << "Setting inventory... " << std::endl;
	std::flush(std::cout);
	request.type = static_cast<uint8>(MsgType::INVENTORY);
	request.playerSlot = player.getSlot();
	//request.itemPrefixId = 0;
	request.itemStack = 1;

	std::vector<ItemId>::const_iterator it = player.getInventory().begin();
	std::vector<ItemId>::const_iterator end = player.getInventory().end();

	for (request.inventorySlot = 0; request.inventorySlot <= 59;
		++request.inventorySlot)
	{
		if (it != end)
		{
			request.itemId = static_cast<uint8>(*it);
			++it;
		}
		else if (request.itemStack)
		{
			request.itemStack = 0;
		}
		std::cout << "\tsetting item #" << static_cast<int>(request.inventorySlot) << std::endl;
		connection.syncRequest(&request, sizeof(request));
	}
	std::cout << "SUCCESS: Inventory set" << std::endl;
}

void	Client::worldInfoFetch()
{
	std::cout << "Requesting world information... ";
	std::flush(std::cout);
	{
		Message	request;
		request.type = static_cast<uint8>(MsgType::WORLD_REQ);

		connection.syncRequest(&request, sizeof(request));
	}

	worldInfo.type = 0;
	connection.syncResponse(&worldInfo, sizeof(worldInfo), MsgType::WORLD_INFO);
	std::cout << "\tSUCCESS" << std::endl;

	std::cout << "Requesting initial tile data... ";
	InitTileData	td;
	td.type = static_cast<uint8>(MsgType::INIT_TILE_DATA);
	td.spawnX = worldInfo.spawnX;
	td.spawnY = worldInfo.spawnY;
	connection.syncRequest(&td, sizeof(td));
	std::cout << "\tSUCCESS" << std::endl;

	char	name[sizeof(TERR_WORLD_NAME)];
	std::memset(name, 0, sizeof(name));
	std::strncpy(name, worldInfo.worldName, sizeof(worldInfo.worldName));
	std::cout << "World name is: " << name << std::endl;
}