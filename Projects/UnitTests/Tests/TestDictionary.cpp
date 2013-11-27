// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#include "Precompiled.h"
#include "Om/Object.h"
#include "Om/ObjectModel.h"

OM_USING_NAMESPACE_OM();

struct TestDictionary : testing::Test
{
	void Test();
};

TEST_F(TestDictionary, Test)
{
	Registry R;
	Pointer<int> P = R.New(42);
	Pointer<int> Q = R.New(123);

	ASSERT_FALSE(P.Has("Q"));
	P.Set("Q", Q);

	ASSERT_TRUE(P.Has("Q"));
	ASSERT_TRUE(Q.GetParent().GetHandle() == P.GetHandle());
	P.Remove("Q");
	ASSERT_FALSE(P.Has("Q"));
	ASSERT_TRUE(Q.GetParent().GetHandle() == 0);
}

//EOF
