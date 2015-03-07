#include <iostream>
#include <stdexcept>
#include <cstring>
#include <cassert>
#include <boost/bind.hpp>
#include "Client.hpp"

Client::Client(const char* host, short port):
	io_service(), socket(io_service), msgFactory()
{
	socket.connect(tcp::endpoint(boost::asio::ip::address_v4::from_string(host), port));
}

Client::~Client()
{
	socket.shutdown(tcp::socket::shutdown_send);
	socket.close();
}

/*
**	Starts asynchronous I/O if asynchronous sends/receives are performed
**
**	Runs server connection process (synchrone)
*/
void	Client::run()
{
	login();
	responseExpect();
	io_service.run();
}

/*
**	Stops asynchronous tasks
*/
void	Client::stop()
{
	io_service.stop();
}

/*
**	LOT TO DO HERE
*/

/*
**	Yields the server responses queue
*/
std::queue<MessagePtr>	Client::getQueue()
{
	return queue;
}

/*****************************************************************************
**	PRIVATE
*****************************************************************************/

/*
**	Launches login to server process (asynchronous)
*/
void	Client::login()
{
	connection();
	playerConfiguration();
	inventorySet();
	worldInfoFetch();
}

void	Client::connection()
{
	std::cout << "Connection... ";
	std::flush(std::cout);
	{
		Connect	c;
		c.type = static_cast<uint8>(MsgType::CONNECT);

		std::strncpy(c.version, CLIENT_VERSION, sizeof(c.version));
		syncRequest(&c, sizeof(c));
	}

	Accepted	a;
	syncResponse(&a, sizeof(a));
	syncResponseCheck(a, MsgType::ACCEPTED);

	player.setLogged(true);
	player.setSlot(a.playerSlot);
	std::cout << "\t OK" << std::endl;
}

void	Client::playerConfiguration()
{
	Appearance	a;
	Life		l;
	Mana		m;
	Buffs		b;

	std::cout << "Configuring player" << std::endl;
	setAppearance(a);
	a.playerSlot = player.getSlot();
	a.type = static_cast<uint8>(MsgType::APPEARANCE);
	a.difficulty = static_cast<uint8>(Difficulty::NORMAL);
	std::strncpy(a.playerName, PLAYER_NAME, sizeof(a.playerName));

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

	std::cout << "\tsetting player appearance" << std::endl;
	syncRequest(&a, sizeof(a));
	std::cout << "\tsetting player life" << std::endl;
	syncRequest(&l, sizeof(l));
	std::cout << "\tsetting player mana" << std::endl;
	syncRequest(&m, sizeof(m));
	std::cout << "\tsetting player buffs" << std::endl;
	syncRequest(&b, sizeof(b));
}

void	Client::inventorySet()
{
	InventoryItem	request;

	request.type = static_cast<uint8>(MsgType::INVENTORY);
	request.playerSlot = player.getSlot();
	request.itemPrefixId = 0;
	request.itemStack = 1;

	std::vector<ItemId>::const_iterator it = player.getInventory().begin();
	std::vector<ItemId>::const_iterator end = player.getInventory().end();

	for (request.inventorySlot = 0; request.inventorySlot < 60;
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
	}
}

void	Client::worldInfoFetch()
{
	{
		Message	request;
		request.type = static_cast<uint8>(MsgType::WORLD_REQ);

		syncRequest(&request, sizeof(request));
	}

	syncResponse(&worldInfo, sizeof(worldInfo));
	syncResponseCheck(worldInfo, MsgType::WORLD_INFO);
}

void	Client::syncRequest(Message *msg, std::size_t msgSize)
{
	msg->length = msgSize - sizeof(msg->length);
	socket.send(boost::asio::buffer(msg, msgSize));
}

void	Client::syncResponse(Message *msg, std::size_t msgSize)
{
	socket.read_some(boost::asio::buffer(msg, msgSize));
	responseTypeCheck(*msg);
}

void	Client::request(MessagePtr msg, std::size_t msgSize)
{
	msg->length = msgSize - sizeof(msg->length);
	socket.async_send(boost::asio::buffer(msg.get(), msgSize),
		boost::bind(&Client::sendHandler, this, msg, boost::asio::placeholders::error)
	);
}

void	Client::sendHandler(MessagePtr msg, const boost::system::error_code& err)
{
	if (err)
	{
		std::fprintf(stderr, "send() error occured. Message type is %x.\n", msg->type);
		std::cerr << "\treason: " <<  err.message() << std::endl;
	}
}

void		Client::responseExpect()
{
	std::shared_ptr<int32>	length = std::make_shared<int32>();

	socket.async_receive(boost::asio::buffer(length.get(), sizeof(*(length.get()))),
		boost::bind(&Client::response, this, length, boost::asio::placeholders::error)
	);
}

void		Client::response(std::shared_ptr<int32> lengthPtr, const boost::system::error_code& err)
{
	recvErrCheck(err);
	int32 len = *(lengthPtr.get());
	if (!responseLenCheck(len))
		return ;

	boost::shared_array<uint8>	buff(new uint8[len + sizeof(len)]);
	std::memcpy(buff.get(), &len, sizeof(len));

	socket.async_receive(boost::asio::buffer(buff.get() + sizeof(len), len),
		boost::bind(&Client::recvHandler, this, buff, boost::asio::placeholders::error)
	);
}

void		Client::recvHandler(boost::shared_array<uint8> buf, const boost::system::error_code& err)
{
	recvErrCheck(err);

	auto msg = MessagePtr(reinterpret_cast<Message*>(buf.get())); // cancer ?
	responseTypeCheck(*msg);

	queue.push(msg);
	responseExpect();
}

template<typename T>
void	Client::syncResponseCheck(const T& res, MsgType type)
{
	if (res.type != static_cast<uint8>(type))
	{
		std::ostringstream os;

		os << "ERROR: unexpected response " << static_cast<int>(res.type)
			<< " (expected: " << static_cast<int>(type) << ").";
		throw std::runtime_error(os.str());
	}
	int32	expected = (sizeof(res) - sizeof(res.length));
	if (res.length != expected)
	{
		std::cerr << "WARNING: unexpected response size " << res.length
			<< " (expected: " << expected << " || type: " << static_cast<int>(res.type)
			<< ")." << std::endl;
	}
}

inline bool	Client::responseLenCheck(int32 len)
{
	if (len == 0)
	{
		responseExpect();
		return false;
	}
	else if (len < 0)
	{
		std::ostringstream	os;
		os << "Unexpected error occured : invalid len value in server message (" << len << ").";
		throw std::runtime_error(os.str());
	}
	return true;
}

inline void	Client::responseTypeCheck(const Message& msg)
{
	if (msg.type == static_cast<uint8>(MsgType::FATAL_ERR))
	{
		auto buf = reinterpret_cast<const char *>(&msg);
		throw std::runtime_error("Connection rejected: " + std::string(buf + sizeof(msg)));
	}
	/*
	else
	{
		std::cout
			<< "Receved server response." << std::endl 
			<< "\tmessageType: " << static_cast<int>(msg.type) << std::endl;
	}
	*/
}

inline void Client::recvErrCheck(const boost::system::error_code& err)
{
	if (err)
	{
		std::ostringstream os;

		os << "recv() error occured." << std::endl << "\treason: " << err.message() << std::endl;
		throw std::runtime_error(os.str());
	}
}

static inline void	setColor(color dst, const color src)
{
	std::memcpy(dst, src, sizeof(color));
}

void	Client::setAppearance(Appearance& a)
{
	a.hairStyle = 1; // random
	a.gender = Gender::MALE;
	setColor(a.hair, RGB::BLACK);
	setColor(a.skin, RGB::GREEN);
	setColor(a.eye, RGB::PINK);
	setColor(a.shirt, RGB::YELLOW);
	setColor(a.undershirt, RGB::WHITE);
	setColor(a.pants, RGB::PURPLE);
	setColor(a.shoes, RGB::BLUE);
}
