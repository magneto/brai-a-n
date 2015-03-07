#pragma once

/*
**	Below are types & constants definitions
**	for client/server protocol
*/
# define DFL_HOST		"127.0.0.1"
# define DFL_PORT		7777
# define WORLD_NAME		"Braian"
# define CLIENT_VERSION	"Terraria94"
# define PLAYER_NAME	"Braian"

# include <cstdint>

typedef std::uint8_t	uint8;
typedef std::int16_t	int16;
typedef std::int32_t	int32;
typedef float			single;
typedef uint8			color[3];

enum class Gender : uint8
{
	FEMALE = 0x0,
	MALE
};

enum class Difficulty : uint8
{
	NORMAL = 0x0,
	MEDIUM,
	HARDCORE
};

enum class ItemId : int16
{
	PICKAXE = 0x1,
	BROADSWORD = 0x4,
	SHORTSWORD = 0x6,
	HAMMER = 0x7,
	TORCH = 0x8,
	AXE = 0x10
};

namespace RGB
{
	const color	RED = { 255, 0, 0 };
	const color	GREEN = { 0, 255, 0 };
	const color	BLUE = { 0, 0, 255 };
	const color	BLACK = { 0, 0, 0 };
	const color	GREY = { 128, 128, 128 };
	const color	WHITE = { 255, 255, 255 };
	const color	ORANGE = { 255, 128, 0 };
	const color	YELLOW = { 255, 255, 0 };
	const color	CYAN = { 0, 255, 255 };
	const color	PURPLE = { 127, 0, 255 };
	const color	PINK = { 255, 0, 127 };
};