// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#include "Precompiled.h"
#include "Om/ClassBuilder.h"

OM_USING_NAMESPACE_OM();

struct TestProperties : testing::Test
{
	void Test();

	void TestBuilder();
};

struct PropTest
{
	Field<int> p_num;
	int num;
	Object object;
};

OM_BEGIN
OM_TRAITS_NUMBER_NAME_OPS(PropTest, 1003, "PropTest", Operations::None);
OM_END

TEST_F(TestProperties, Test)
{
	Registry R;
}

TEST_F(TestProperties, TestBuilder)
{
	Registry R;
	R.AddClass<int>();
	R.AddClass<Vector>();

	ClassBuilder<PropTest>(R)
		.Methods
		.Properties
			("num", &PropTest::num)
			("p_num", &PropTest::p_num)
			("object", &PropTest::object)
		;
	Pointer<PropTest> prop_test = R.New<PropTest>();
	ASSERT_TRUE(prop_test);
	ClassBase const *klass = prop_test.GetClass();
	ASSERT_TRUE(klass->HasProperty("num"));
	ASSERT_TRUE(klass->HasProperty("p_num"));
	ASSERT_TRUE(prop_test.Has("num"));
	ASSERT_FALSE(prop_test.Has("p_num"));

	Pointer<int> N1 = R.New(123);
	Pointer<int> N2 = R.New(456);
	Pointer<int> N3 = R.New(789);

	ASSERT_FALSE(N1.HasParent());
	prop_test->p_num = N1;
	ASSERT_TRUE(N1.HasParent());
	ASSERT_TRUE(N1.GetParentHandle() == prop_test.GetHandle());
	
	prop_test.Set("num", N2);
	ASSERT_TRUE(prop_test->num == *N2);
	ASSERT_FALSE(N2.HasParent());
	prop_test.SetValue("num", 666);
	ASSERT_TRUE(prop_test->num == 666);
	ASSERT_FALSE(N2.HasParent());


	ASSERT_TRUE(N1.HasParent());
	ASSERT_FALSE(N3.HasParent());
	prop_test.Set("p_num", N3);
	ASSERT_FALSE(N1.HasParent());
	ASSERT_TRUE(N3.HasParent());
	ASSERT_TRUE(N3.GetParentHandle() == prop_test.GetHandle());

	prop_test.SetValue("p_num", 42);
	EXPECT_EQ(*N3, 42);

	prop_test.SetValue("num", 13);
	EXPECT_EQ(prop_test->num, 13);
}

//EOF
