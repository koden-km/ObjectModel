// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#include "Precompiled.h"
#include "Om/Object.h"
#include "Om/ObjectModel.h"

OM_USING_NAMESPACE_OM();

/// Unit-test for the multi-cast event system
struct TestRegistry : testing::Test
{
	/// test function delegates
	void Test();

	/// test method delegates
	void TestMethods();
};

TEST_F(TestRegistry, Test)
{
	Registry R;
	Pointer<int> P = R.New(42);

	ASSERT_TRUE(P.Exists());
	ASSERT_FALSE(P.IsEmpty());
	EXPECT_EQ(P.GetTypeNumber().value, Type::Traits<int>::TypeNumber.value);
	EXPECT_EQ(P.GetClassBase(), R.GetClassFromLabel("int"));
	EXPECT_EQ(Deref<int>(P), 42);
	EXPECT_EQ(*P, 42);
}

//EOF

