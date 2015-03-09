#include <iostream>
#include <stdexcept>
#include <array>
#include "Client.hpp"
#include <boost/array.hpp>

int	main(void)
{
	try
	{
	  Client	cl("192.168.1.32");

		cl.run();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	std::getchar();
}
