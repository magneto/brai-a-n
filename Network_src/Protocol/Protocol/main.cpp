#include <iostream>
#include <stdexcept>
#include <array>
#include "Client.hpp"
#include <boost/array.hpp>

/*
int	main()
{
	try
	{
		Client	cl;
		cl.run();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	std::getchar();
}
*/

#pragma pack(push, 1)
struct header
{
	int		length;
	unsigned char	type;
};

struct connectReq
{
	int		length;
	unsigned char	type;
	char	version[sizeof(CLIENT_VERSION)];
};

struct connectRep
{
	header			h;
	unsigned char	slot;
};

struct mapInfo
{
	int32	time;
	bool	dayTime;
	int8	moonPhase;
	bool	bloodMoon;
	bool	eclipse;
	int32	mapWidth;
	int32	mapHeight;
	int32	spawnX;
	int32	spawnY;
	int8	misc[61];
};
#pragma pack(pop)

static int i = sizeof(mapInfo);

void	connectionReq(boost::asio::ip::tcp::socket& socket, boost::asio::ip::udp::endpoint& serverEp)
{
	connectReq	c;

	c.length = sizeof(c) - sizeof(int);
	//c.length = htons(c.length);
	c.type = 6;
	std::strcpy(c.version, CLIENT_VERSION);

	//socket.write(boost::asio::buffer(&c, sizeof(c)), serverEp);
	std::cout << "sending request..." << std::endl;
	socket.send(boost::asio::buffer(&c, sizeof(c)));
	std::cout << "sent connect request" << std::endl;
}

static boost::asio::io_service			ios;
static boost::asio::ip::udp::socket		s(ios);
static boost::asio::ip::udp::endpoint	serverEp(boost::asio::ip::address_v4::from_string("127.0.0.1"), 7777);

void	connectionRep(boost::asio::ip::tcp::socket& socket)
{
	connectRep	c;

	std::cout << "receiving response..." << std::endl;

	std::size_t ret = socket.receive(boost::asio::buffer(&c, sizeof(c)));

	//socket.read_some(boost::asio::buffer(&c, sizeof(c)));

	/*
	boost::asio::ip::udp::endpoint		remote;
	boost::array<char, 5>	buf;
	s.receive_from(boost::asio::buffer(&c, sizeof(c)), remote);
	*/

	//auto& b = boost::asio::buffer(buf);
	//s.receive_from(b, remote);


	std::cout << "recieved " << ret << " bytes" << std::endl;
	std::cout << c.h.type << std::endl;
}

int	main()
{
	boost::asio::ip::tcp::socket	socket(ios);

	try
	{
		s.open(udp::v4());
		socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4::from_string("127.0.0.1"), 7777));

		connectionReq(socket, serverEp);
		connectionRep(socket);

		socket.shutdown(udp::socket::shutdown_send);
		socket.close();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	std::getchar();
}