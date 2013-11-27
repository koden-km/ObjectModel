// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

struct GameType
{
	enum Type
	{
		Vector2,
		Item,
		Player,
	};
};

#define GAME_TYPE(N) \
	OM_BEGIN \
	OM_TRAITS_NUMBER_OPS(N, OM_USER_TYPE_NUMBE(GameType::N), Operations::Assign); \
	OM_END

struct Vector2;
struct Item;
class Player;

GAME_TYPE(Vector2);
GAME_TYPE(Item);
GAME_TYPE(Player);

//EOF
