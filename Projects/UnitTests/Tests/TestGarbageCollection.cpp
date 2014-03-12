// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#include "Precompiled.h"
#include "Om/ClassBuilder.h"

OM_USING_NAMESPACE_OM();

struct TestGarbageCollection : testing::Test
{
	// test that child objects are correctly managed
	void Test();

	// test that objects containers in containers are marked
	void TestContainer();

	// test properties are marked, including properties that are containers
	void TestProperties();
};

TEST_F(TestGarbageCollection, Test)
{
	Registry R;
	R.AddClass<int>();
	Object N = R.New(42);

	ASSERT_TRUE(N.Exists());
	R.GarbageCollect();
	ASSERT_FALSE(N.Exists());

	Object root = R.New(12);
	R.SetRoot(root);

	Object M = R.New(123);
	root.Set("M", M);

	ASSERT_TRUE(root.Exists());
	ASSERT_TRUE(M.Exists());
	R.GarbageCollect();
	ASSERT_FALSE(N.Exists());
	ASSERT_TRUE(root.Exists());
	ASSERT_TRUE(M.Exists());

	root.Remove("M");
	R.GarbageCollect();
	ASSERT_FALSE(N.Exists());
	ASSERT_TRUE(root.Exists());
	ASSERT_FALSE(M.Exists());
}

TEST_F(TestGarbageCollection, TestContainer)
{
	Registry R;

	Pointer<Vector> V = R.New<Vector>();
	Object N = R.New(123);
	V->push_back(N);
	R.SetRoot(V);
	R.GarbageCollect();
	ASSERT_TRUE(V.Exists());
	ASSERT_TRUE(N.Exists());

	Pointer<Vector> V2 = R.New<Vector>();
	Object N2 = R.New(456);
	V2->push_back(N2);
	V->push_back(V2);
	R.GarbageCollect();
	
	ASSERT_TRUE(V.Exists());
	ASSERT_TRUE(N.Exists());

	ASSERT_TRUE(V2.Exists());
	ASSERT_TRUE(N2.Exists());

}

static struct Foobar2
{
	Field<int> pnum;
	Field<Vector> vec;
	int num;
	Field<String> str;
	Object nothing;
	Field<int> dangling;
};

OM_BEGIN
OM_TRAITS_NUMBER_NAME_OPS(Foobar2, 1002, "Foobar", Operations::None);
OM_END

TEST_F(TestGarbageCollection, TestProperties)
{
//	ASSERT_TRUE(false);
	Registry R;
	R.AddClass<int>();
	R.AddClass<Vector>();
	R.AddClass<String>();

	ClassBuilder<Foobar2>(R)
		.Methods
		.Properties
			("num", &Foobar2::num)
			("pnum", &Foobar2::pnum)
			("vec", &Foobar2::vec)
			("nothing", &Foobar2::nothing)
			// intentionally left dangling:
			//("dangling", &Foobar2::dangling)
		;
	Pointer<Foobar2> foo = R.New<Foobar2>();
	//return;

	foo->pnum = R.New<int>();
	foo->vec = R.New<Vector>();
	foo->dangling = R.New<int>();
	Object in_vec = R.New<int>();
	foo->vec->push_back(in_vec);

	Object root = R.New<int>(123);
	R.SetRoot(root);
	root.Set("foo", foo);

	R.GarbageCollect();
	ASSERT_TRUE(foo.Exists());
	ASSERT_TRUE(foo->pnum.Exists());
	ASSERT_TRUE(foo->vec.Exists());
	ASSERT_TRUE(in_vec.Exists());
	ASSERT_FALSE(foo->dangling.Exists());

	foo->vec->clear();
	R.GarbageCollect();
	ASSERT_FALSE(in_vec.Exists());

	R.SetRoot(Object());
	R.GarbageCollect();
	EXPECT_EQ(R.NumObjects(), 0);
}

//EOF

