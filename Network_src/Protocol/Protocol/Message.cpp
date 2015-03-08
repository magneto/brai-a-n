#include <bitset>
#include "Message.hpp"

void	TileInfo::init(TileInfo& self, const boost::shared_array<uint8>& buf)
{
	std::bitset<sizeof(self.flags)>	flags(self.flags);

	return;
}
