// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#include "Precompiled.h"
#include "Om/ObjectModel.h"

OM_USING_NAMESPACE_OM();

struct TestReference : testing::Test
{
	void Test();
};

TEST_F(TestReference, Test)
{
	Registry R;
	int n = 1;
	Pointer<int> P = R.Ref(n);
	*P = 2;
	ASSERT_EQ(n, 2);

	//Pointer<const int> Q = R.ConstRef(n);
	//bool caught = false;
	//try
	//{
	//	*Q = 42;
	//}
	//catch (ConstError &)
	//{
	//	caught = true;
	//}
	//ASSERT_TRUE(caught);


}

//EOF

