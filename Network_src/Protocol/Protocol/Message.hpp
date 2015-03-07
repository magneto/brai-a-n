#pragma once

# ifdef _WIN32_WINNT
#  pragma comment(lib, "ws2_32.lib")
#  include <WinSock2.h>
# else
#  include <arpa/inet.h>
# endif //_WIN32_WINNT
# include "MsgTypes.hpp"
# include "TerrariaInfo.hpp"

# define messageType(t) (static_cast<uint8>(MsgType::t))

// packing is now set to one
# pragma pack(push, 1)

/*
**	Message data struct is a P.O.D.
**
**	It wraps messages' header composed of:
**	- length:	Type + Payload size (in bytes)
**	- type:		byte reprensenting MessageType
*/
struct Message
{
	int32	length;
	uint8	type;
};


/*
**	Below are declared P.O.D. structs reprenting messages and
**	their "payloads" (i.e. messages data)
**
**	Messages are associated with Terraria's protocol
**	command types defined in "MessageTypes.h"
*/

/*****************************************************************************
**	REQUESTS (client->server)
*****************************************************************************/

struct Connect : public Message
{
	char	version[sizeof(CLIENT_VERSION) - 1];
};

struct Appearance : public Message
{
	uint8	playerSlot;

	uint8	hairStyle;
	Gender	gender;
	color	hair;
	color	skin;
	color	eye;
	color	shirt;
	color	undershirt;
	color	pants;
	color	shoes;

	uint8	difficulty;
	char	playerName[sizeof(PLAYER_NAME) - 1];
};

struct Life : public Message
{
	uint8	playerSlot;
	int16	currLife;
	int16	maxLife;
};

struct Mana : public Message
{
	uint8	playerSlot;
	int16	manaLevel;
	int16	maxMana;
};

struct Buffs : public Message
{
	uint8	playerSlot;
	uint8	buffTypes[10];
};

struct InventoryItem : public Message
{
	uint8	playerSlot;
	uint8	inventorySlot;
	int16	itemStack;
	uint8	itemPrefixId;
	int16	itemId;
};

/*****************************************************************************
**	RESPONSES (client<-server)
*****************************************************************************/

struct Accepted : public Message
{
	uint8	playerSlot;
};

struct WorldInfo : public Message
{
	int32	time;
	uint8	dayTime;
	uint8	moonPhase;
	uint8	bloodMoon;
	uint8	eclipse;
	int32	mapWidth;
	int32	mapHeight;
	int32	spawnX;
	int32	spawnY;
	uint8	misc[61];
	char	worldName[sizeof(WORLD_NAME) - 1];
};

/*****************************************************************************
**	BOTH (client<->server)
*****************************************************************************/

// packing is reset to 8
# pragma pack(pop)

