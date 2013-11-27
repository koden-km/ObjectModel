// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#include "Precompiled.h"
#include "Om/Pathname.h"

OM_USING_NAMESPACE_OM();

struct TestPathname : testing::Test
{
	void Test();
};

TEST_F(TestPathname, Test)
{
	Pathname p("foo");
	//EXPECT_EQ(p.IsRelative(), true);
	//EXPECT_EQ(p.IsRelative(), true);
	//ASSERT_TRUE(p.GetElements().size(), 1);
	//EXPECT_EQ(p.GetElements()[0].type, Pathname::Element::Ident);
	//EXPECT_EQ(p.GetElements()[0].label.ToString(), "foo");
}

//EOF
