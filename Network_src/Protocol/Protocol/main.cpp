#include <iostream>
#include <stdexcept>
#include <array>
#include "Client.h"
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
	char	type;
};

struct connectReq
{
	int		length;
	char	type;
	char	version[sizeof(CLIENT_VERSION)];
};

struct connectRep
{
	header	h;
	char	slot;
};

#pragma pack(pop)

void	connectionReq(boost::asio::ip::tcp::socket& socket, boost::asio::ip::udp::endpoint& serverEp)
{
	connectReq	c;

	c.length = htons(sizeof(c) - sizeof(int));
	c.type = 1;
	std::strcpy(c.version, CLIENT_VERSION);

	//socket.write(boost::asio::buffer(&c, sizeof(c)), serverEp);
	std::cout << "sending request..." << std::endl;
	socket.send(boost::asio::buffer(&c, sizeof(c)));
	std::cout << "sent connect request" << std::endl;
}

static boost::asio::io_service			ios;
static boost::asio::ip::udp::socket		s(ios);

void	connectionRep(boost::asio::ip::tcp::socket& socket)
{
	connectRep	c;

	std::cout << "receiving response..." << std::endl;

	socket.receive(boost::asio::buffer(&c, sizeof(c)));
	//socket.read_some(boost::asio::buffer(&c, sizeof(c)));

	/*
	boost::asio::ip::udp::endpoint		remote;
	boost::array<char, 5>	buf;
	s.receive_from(boost::asio::buffer(&c, sizeof(c)), remote);
	*/

	//auto& b = boost::asio::buffer(buf);
	//s.receive_from(b, remote);

	std::cout << "end" << std::endl;
}

int	main()
{
	s.open(udp::v4());
	boost::asio::ip::udp::endpoint	serverEp(boost::asio::ip::address_v4::from_string("127.0.0.1"), 7777);
	boost::asio::ip::tcp::socket	socket(ios);

	try
	{
		socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4::from_string("127.0.0.1"), 7777));
		//connectionReq(socket, serverEp);
		//connectionRep(socket);
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	std::getchar();

	socket.shutdown(udp::socket::shutdown_send);
	//socket.close();
}