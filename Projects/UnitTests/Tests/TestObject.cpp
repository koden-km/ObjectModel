// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#include "Precompiled.h"
#include "Om/ObjectModel.h"

OM_USING_NAMESPACE_OM();

struct TestObject : testing::Test
{
	void Test();
	void TestProxy();
};

TEST_F(TestObject, Test)
{
	Registry R;
	R.AddClass<int>();
	R.AddClass<void>();

	Object N = R.New<int>();
	ASSERT_TRUE(N.Exists());
	ASSERT_TRUE(N.IsType<int>());
	ASSERT_TRUE(N.GetTypeNumber() == Type::Traits<int>::TypeNumber);

	Pointer<int> P = N;
	ASSERT_TRUE(P.Exists());
	ASSERT_TRUE(P.GetHandle() == N.GetHandle());

	*P = 42;
	ASSERT_TRUE(Deref<int>(N) == 42);

	Pointer<Object> W = N;
	*W = N;
}

TEST_F(TestObject, TestProxy)
{
	Registry R;
	R.AddClass<void>();
	R.AddClass<int>();

	Pointer<void> Q = R.New<void>();
	Pointer<int> T = R.New(42);
	Q["T"] = T;
	ASSERT_TRUE(T.GetParentHandle() == Q.GetHandle());
	ASSERT_TRUE(Q.Has("T"));
	Object K = Q["T"];
	ASSERT_TRUE(K.GetHandle() == T.GetHandle());
	Deref<int>(Q["T"]) = 13;
	ASSERT_TRUE(Deref<int>(K) == 13);
	ASSERT_TRUE(*T == 13);
	ASSERT_TRUE(*Pointer<int>(Q["T"]) == 13);

	Pointer<int> U = R.New(123);
	ObjectProxy P = Q["T"];
	Object _T = P.Get();
	_T["U"] = U;
	ASSERT_TRUE(U.GetParentHandle() == T.GetHandle());
	ASSERT_TRUE(T.Has("U"));

	Pointer<int> V = R.New(456);
	Q["T"]["V"] = V;
	ASSERT_TRUE(V.GetParentHandle() == T.GetHandle());
	ASSERT_TRUE(T.Has("U"));


}

//EOF

