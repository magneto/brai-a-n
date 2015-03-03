#pragma once

# ifdef _WIN32_WINNT
#  pragma comment(lib, "ws2_32.lib")
#  include <WinSock2.h>
# else
#  include <arpa/inet.h>
# endif //_WIN32_WINNT
# include <cstdint>
# include "MessageTypes.hpp"

/*
**	Below are types & constants definitions
**	for client/server protocol
*/

typedef std::int8_t		int8;
typedef std::int16_t	int16;
typedef std::int32_t	int32;
typedef std::float_t	single;
typedef int8			color[3];

#define CLIENT_VERSION	"Terraria37"


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
	struct Header
	{
		int32	length;
		int8	type;
	}			header;

	void	hostToNet()
	{
		header.length = ntohl(header.length);
	}

	void	netToHost()
	{
		header.length = htonl(header.length);
	}
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
	char	version[sizeof(CLIENT_VERSION)];
};

/*****************************************************************************
**	RESPONSES (client<-server)
*****************************************************************************/

struct Accepted : public Message
{
	int8	playerSlot;
};

// packing is reset to 8
# pragma pack(pop)

