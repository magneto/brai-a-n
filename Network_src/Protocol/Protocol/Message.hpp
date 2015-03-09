#pragma once

# ifdef _WIN32_WINNT
#  pragma comment(lib, "ws2_32.lib")
#  include <WinSock2.h>
# else
#  include <arpa/inet.h>
# endif //_WIN32_WINNT
# include <utility>
# include <boost/shared_array.hpp>
# include "MsgTypes.hpp"
# include "TerrariaInfo.hpp"

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

typedef std::shared_ptr<Message>	MessagePtr;

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
	char	version[sizeof(TERR_CLIENT_VERSION) - 1];
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
	char	playerName[sizeof(TERR_PLAYER_NAME) - 1];
};

struct Key : public Message
{
	char	unknown_key[sizeof(TERR_KEY) - 1];
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
	//uint8	itemPrefixId;
	int16	itemId;
};

struct InitTileData : public Message
{
	int32	spawnX;
	int32	spawnY;
};

struct SpawnPlayer : public Message
{
	uint8	playerSlot;
	int32	spawnX;
	int32	spawnY;
};
/*****************************************************************************
**	RESPONSES (client<-server)
*****************************************************************************/

struct Accepted : public Message
{
	int32	playerSlot;
};

struct WorldInfo : public Message
{
	int32	time;
	uint8	dayTime;
	uint8	moon[2];
	int32	mapWidth;
	int32	mapHeight;
	int32	spawnX;
	int32	spawnY;
	uint8	worldConfRawData[13];
	char	worldName[sizeof(TERR_WORLD_NAME) - 1];
};



struct Spawn : public Message
{
	int32	spawnX;
	int32	spawnY;
};

struct NPCUpdate : public Message
{
	int16	NPCSlot;
	single	posX;
	single	posY;
	single	velX;
	single	velY;
	uint8	target;
	uint8	flags;
	int32	life;
	single	ai[4];
	int16	npcId;
};

struct NPCName : public Message
{
	uint8	npcSlot;
	char	npcName[TERR_MAX_NAME];
};

enum class TileFlags : bool
{
	TILE_ACTIVE = 0x0,
	NOT_USED,
	HAS_WALL,
	HAS_LIQUID,
	HAS_RED_WIRE,
	IS_HALF_BRICK,
	HAS_ACTUATOR,
	IN_ACTIVE,
	HAS_GREEN_WIRE,
	HAS_BLUE_WIRE,
	HAS_COLOR,
	HAS_WALL_COLOR,
	SLOPE_A,
	SLOPE_B
};

struct TileInfo
{
	uint8	flags[2];
	uint8	color;
	uint8	wallColor;
	uint8	tileType;
	uint8	tileWall;
	uint8	liquidAmount;
	int16	tileU;
	int16	tileV;
	uint8	liquidType;

	static void	init(TileInfo& self, const boost::shared_array<uint8>& buf);
};

/*
** $0A galère !!
*/
struct TileRowData : public Message
{
	int16		width;
	int32		tileX;
	int32		tileY;

	TileInfo	tileData;
	int16		RLEAmount;
};

/*****************************************************************************
**	BOTH (client<->server)
*****************************************************************************/

struct ItemUpdate : public Message
{
	int16	itemSlot;
	single	posX;
	single	posY;
	single	velX;
	single	velY;
	int16	stackAmount;
	uint8	prefixId;
	uint8	ownIgnore;
	int16	itemID;
};

struct ItemOwnerSet : public Message
{
	int16	itemSlot;
	uint8	ownerPlayerSlot;
};

/***************************************************
IGNORED
***************************************************/

struct StatusBar : public Message // balek $09
{
	int32	nbMsg;
	char	statusText[TERR_MAX_STR];
};

struct BalanceStats : public Message // balek $39
{
	uint8	hallowedAmount;
	uint8	corruptionAmount;
};

// packing is reset to 8
# pragma pack(pop)

