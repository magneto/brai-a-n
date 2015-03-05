#pragma once

# ifdef _WIN32_WINNT
#  pragma comment(lib, "ws2_32.lib")
#  include <WinSock2.h>
# else
#  include <arpa/inet.h>
# endif //_WIN32_WINNT
# include <cstdint>
# include "MsgTypes.hpp"
# include "TerrariaInfo.hpp"

# define messageType(t) (static_cast<int8>(MsgType::t))

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
	int8	type;
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

/*****************************************************************************
**	RESPONSES (client<-server)
*****************************************************************************/

struct Accepted : public Message
{
	int8	playerSlot;
};

struct MapInfo : public Message
{
	int32	time;
	int8	dayTime;
	int8	moonPhase;
	int8	bloodMoon;
	int8	eclipse;
	int32	mapWidth;
	int32	mapHeight;
	int32	spawnX;
	int32	spawnY;
	int8	misc[61];
	char	worldName[sizeof(WORLD_NAME) - 1];
};

// packing is reset to 8
# pragma pack(pop)

