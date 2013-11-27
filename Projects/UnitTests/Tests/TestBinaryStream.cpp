// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#include "Precompiled.h"
#include "Om/ClassBuilder.h"
#include "Om/BinaryStream.h"

OM_USING_NAMESPACE_OM();

struct TestBinaryStream : testing::Test
{
	void Test();
	void TestCustom();
};

namespace
{
	struct TestCustonBinaryStream
	{
		Field<Vector> vec;
		String name;

		static void Register(Registry &R)
		{
			ClassBuilder<TestCustonBinaryStream>(R)
				.Methods
				.Properties
					("vec", &TestCustonBinaryStream::vec)
					("name", &TestCustonBinaryStream::name)
				;
		}
	};
}

OM_BEGIN
OM_TRAITS_NUMBER(TestCustonBinaryStream, 6000);//, Operations::BinaryStreaming);
OM_END

TEST_F(TestBinaryStream, TestCustom)
{
	Registry R;
	RegisterBuiltins(R);
	TestCustonBinaryStream::Register(R);
	Pointer<BinaryStream> B = R.New<BinaryStream>();
	Pointer<TestCustonBinaryStream> P = R.New<TestCustonBinaryStream>();

	P->name = "foobar";
	P->vec = R.New<Vector>();
	P->vec->push_back(R.New(123));
	P->vec->push_back(R.New(String("spam")));
	P["num"] = R.New(456);

	B->clear();
	*B << "foobar";
	String foobar;
	*B >> foobar;
	EXPECT_EQ(foobar, "foobar");

	*B << P;
	Object Q;
	*B >> Q;

	ASSERT_TRUE(Q);
	ASSERT_TRUE(Q.IsType<TestCustonBinaryStream>());
	EXPECT_EQ(Deref<TestCustonBinaryStream>(Q).name, "foobar");
	Pointer<Vector> vec = Q["vec"];
	Pointer<String> name = Q["name"];
	Pointer<int> num = Q["num"];
	ASSERT_FALSE(vec.IsConst());
	ASSERT_FALSE(num.IsConst());
	ASSERT_TRUE(name.IsConst());
	ASSERT_TRUE(num);
	ASSERT_TRUE(vec);
	ASSERT_TRUE(name);
	EXPECT_EQ(ConstDeref<String>(name), "foobar");
	ASSERT_EQ(*num, 456);
	ASSERT_EQ(vec->size(), 2);
	EXPECT_EQ(Deref<int>(vec->at(0)), 123);
	EXPECT_EQ(Deref<String>(vec->at(1)), "spam");
}

TEST_F(TestBinaryStream, Test)
{
	Registry R;
	BinaryStream::Register(R);
	Pointer<int> N = R.New(123);
	Pointer<BinaryStream> B = R.New<BinaryStream>();

	*B << N;
	Object N2;
	*B >> N2;
	
	ASSERT_TRUE(N2.Exists());
	ASSERT_TRUE(N2.IsType<int>());
	ASSERT_TRUE(Deref<int>(N2) == 123);
	
	Pointer<float> F = R.New(3.0f);
	B->clear();
	*B << N << F << N;
	Pointer<int> A0, A2;
	Pointer<float> A1;
	*B >> A0 >> A1 >> A2;

	ASSERT_TRUE(A0.Exists());
	ASSERT_TRUE(A1.Exists());
	ASSERT_TRUE(A2.Exists());
	
	EXPECT_EQ(*A0, 123);
	EXPECT_EQ(*A1, 3.0f);
	EXPECT_EQ(*A2, 123);

	Pointer<Vector> V0 = R.New<Vector>();
	Pointer<List> L0 = R.New<List>();

	V0->push_back(R.New(1));
	V0->push_back(R.New(2));
	V0->push_back(R.New(3));
	L0->push_back(R.New(1));
	L0->push_back(R.New(2));
	L0->push_back(R.New(3 ));

	*B << V0 << L0;
	Pointer<Vector> V1;
	Pointer<List> L1;
	*B >> V1 >> L1;

	ASSERT_TRUE(V1.Exists());
	ASSERT_TRUE(L1.Exists());
	ASSERT_TRUE(V1->size() == 3);
	ASSERT_TRUE(L1->size() == 3);
	int nums[] = { 1, 2, 3 };

	int const &(*deref)(Object const &) = Deref<int>;
	ASSERT_TRUE(std::equal(nums, nums + 3, V0->begin(), boost::bind(std::equal_to<int>(), _1, boost::bind(deref, _2))));
	ASSERT_TRUE(std::equal(nums, nums + 3, L0->begin(), boost::bind(std::equal_to<int>(), _1, boost::bind(deref, _2))));

}

