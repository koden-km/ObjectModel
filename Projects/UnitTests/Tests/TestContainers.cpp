// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#include "Precompiled.h"
#include "Om/ClassBuilder.h"

OM_USING_NAMESPACE_OM();

struct TestContainers : testing::Test
{
	void Test();
	void TestFields();
};

TEST_F(TestContainers, Test)
{
	Registry R;
	Pointer<Vector> V = R.New<Vector>();
	ASSERT_TRUE(V->empty());
	Pointer<int> N = R.New(123);
	V->push_back(N);
	EXPECT_EQ(V->size(), 1);

	Pointer<List> L = R.New<List>();
	L->push_back(R.New(123));
	EXPECT_EQ(L->size(), 1);
}

namespace
{
	struct TestContainersStruct
	{
		Field<Vector> vec;
	};
}

OM_BEGIN
OM_TRAITS_NUMBER(TestContainersStruct, 6666);
OM_END

TEST_F(TestContainers, TestFields)
{
	Registry R;
	//R.EnableTrace();
	ClassBuilder<TestContainersStruct>(R)
		.Methods
		.Properties
			("vec", &TestContainersStruct::vec)
		;
	Pointer<TestContainersStruct> tcs = R.New<TestContainersStruct>();
	tcs->vec = R.New<Vector>();
	Pointer<int> N = R.New(123);
	tcs->vec->push_back(N);
	R.SetRoot(tcs);
	R.GarbageCollect();
	ASSERT_TRUE(tcs.Exists());
	ASSERT_TRUE(tcs->vec.Exists());
	ASSERT_TRUE(N.Exists());

	R.SetRoot(N);
	Object vec = tcs->vec;
	R.GarbageCollect();
	ASSERT_FALSE(tcs.Exists());
	ASSERT_FALSE(vec.Exists());
	ASSERT_TRUE(N.Exists());
}

//EOF

