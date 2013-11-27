// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#include "Precompiled.h"
#include "Om/ObjectModel.h"

OM_USING_NAMESPACE_OM();

struct TestRecursiveMarking : testing::Test
{
	void Test();
};

TEST_F(TestRecursiveMarking, Test)
{
	Registry R;

	Pointer<Vector> v = R.New<Vector>();
	Pointer<Vector> u = R.New<Vector>();
	v->push_back(R.New<void>());
	u->push_back(v);
	R.SetRoot(v);

	// without some way to deal with cyclic references, this would recurse endlessly
	R.GarbageCollect();

	Pointer<void> Q = R.New<void>();
	Q["v"] = v;
	R.SetRoot(Q);

	// without some way to deal with cyclic references, this would recurse endlessly
	R.GarbageCollect();
	
	// test that we can clean properly
	R.SetRoot(Object());
	//OM_DEBUG_STREAM(4).Enable();
	R.GarbageCollect();
	ASSERT_TRUE(R.NumObjects() == 0);
}

//EOF

