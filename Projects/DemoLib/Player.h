// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

#include "Om/ObjectModel.h"
#include "./GameType.h"

struct Vector2
{
	int x, y;
	static void Register(Om::Registry &);
};

struct Item
{
	String name;
	int level;

	String AddToName(String const &postfix) const;
	static void Register(Om::Registry &);
};

class Player : public Om::Reflected<Player>
{
	String name;
	Vector2 position;
	std::list<Item> items;

public:
	void AddItem(Item const &);
	static void Register(Om::Registry &);
};

void RegisterDemoTypes(Om::Registry &);

//EOF
