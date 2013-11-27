// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#include "Precompiled.h"
#include "Om/ObjectModel.h"
#include "Om/Reflected.h"

OM_BEGIN

OM_END

OM_USING_NAMESPACE_OM();

struct TestReflected : testing::Test
{
	void Test();
};

namespace
{
	bool create_called;
	bool destroy_called;
	Pointer<int> num;

	struct ReflectedTestStruct : Reflected<ReflectedTestStruct>
	{
		ReflectedTestStruct()
		{
			create_called = destroy_called = false;
		}
		void Construct()
		{
			create_called = true;
			num = New(123);
		}
		void Destroy()
		{
			destroy_called = true;
		}
	};
}

OM_BEGIN
OM_TRAITS_NUMBER(ReflectedTestStruct, 1005);
OM_END

TEST_F(TestReflected, Test)
{
	Registry R;

	ASSERT_FALSE(create_called);
	ASSERT_FALSE(destroy_called);
	ASSERT_FALSE(num.Exists());

	Pointer<ReflectedTestStruct> P = R.New<ReflectedTestStruct>();

	ASSERT_TRUE(P->Self == &P.GetStorage());
	ASSERT_TRUE(create_called);
	ASSERT_TRUE(num.Exists());
	ASSERT_FALSE(destroy_called);
	R.GarbageCollect();
	ASSERT_TRUE(destroy_called);

}

//EOF

