// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#include "Player.h"
#include "Om/ClassBuilder.h"

OM_USING_NAMESPACE_OM();

void Vector2::Register(Om::Registry &R)
{
	ClassBuilder<Vector2>(R)
		.Methods
		.Properties
			("x", &Vector2::x)
			("y", &Vector2::y)
		;
}

String Item::AddToName(String const &X) const
{
	return name + X;
}

void Item::Register(Om::Registry &R)
{
	ClassBuilder<Item>(R)
		.Methods
			("AddToName", &Item::AddToName)
		.Properties
			("name", &Item::name)
			("level", &Item::level)
		;
}

void Player::AddItem(Item const &item)
{
	items.push_back(item);
}

void Player::Register(Om::Registry &R)
{
	ClassBuilder<Player>(R)
		.Methods
			("AddItem", &Player::AddItem)
		.Properties
			("name", &Player::name)
			("position", &Player::position)
		;
}

void RegisterDemoTypes(Om::Registry &R)
{
	Vector2::Register(R);
	Item::Register(R);
	Player::Register(R);
}

//EOF
