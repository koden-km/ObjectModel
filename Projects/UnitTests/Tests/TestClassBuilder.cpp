// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#include "Precompiled.h"
#include "Om/ClassBuilder.h"

OM_USING_NAMESPACE_OM();

struct TestClassBuilder : testing::Test
{
	/// test function delegates
	void Test();
};

struct Foobar
{
	int num;
	Field<String> str;

	void M0()
	{
	}
	void M0_C() const
	{
	}
	void M1(int n)
	{
	}
	void M1_C(int n) const
	{
	}
	void M2(int n, float F)
	{
	}
	void M2_C(int n, float F) const
	{
	}
	void M3(int n, float F, String const &S)
	{
	}
	void M3_C(int n, float F, String const &S) const
	{
	}

	int R_M0()
	{
		return 1;
	}
	float R_M0_C() const
	{
		return 1;
	}
	String R_M1(int n)
	{
		return "";
	}
	int R_M1_C(int n) const
	{
		return 1;
	}
	float R_M2(int n, float F)
	{
		return 1;
	}
	String s;
	String const &R_M2_C(int n, float F) const
	{
		return s;
	}
	int R_M3(int n, float F, String &S)
	{
		return 1;
	}
	float R_M3_C(int n, float &F, String const &S) const
	{
		return 1;
	}

	void G(Object const &Q)
	{
	}
	Object H1(Object)
	{
		return Object();
	}
	Object H2(Object &)
	{
		return Object();
	}
	Object H3(Object const &)
	{
		return Object();
	}
	Object dummy;
	Object &H4(Object const &)
	{
		return dummy;
	}
	Object const &H5(Object const &)
	{
		return dummy;
	}
};


OM_BEGIN
OM_TRAITS_NUMBER_NAME_OPS(Foobar, 1000, "Foobar", Operations::None);
OM_END

TEST_F(TestClassBuilder, Test)
{
	Registry R;
	R.AddClass<int>();
	R.AddClass<String>();
	R.AddClass<float>();

	ClassBuilder<Foobar>(R)
		.Methods
			("G", &Foobar::G)
			("M0", &Foobar::M0)
			("M0_C", &Foobar::M0_C)
			("M1", &Foobar::M1)
			("M1_C", &Foobar::M1_C)
			("M2", &Foobar::M2)
			("M2_C", &Foobar::M2_C)
			("M3", &Foobar::M3)
			("M3_C", &Foobar::M3_C)
			("R_M0", &Foobar::R_M0)
			("R_M0_C", &Foobar::R_M0_C)
			("R_M1", &Foobar::R_M1)
			("R_M1_C", &Foobar::R_M1_C)
			("R_M2", &Foobar::R_M2)
			("R_M2_C", &Foobar::R_M2_C)
			("R_M3", &Foobar::R_M3)
			("R_M3_C", &Foobar::R_M3_C)
			("H1", &Foobar::H1)
			("H2", &Foobar::H2)
			("H3", &Foobar::H3)
			("H4", &Foobar::H4)
			("H5", &Foobar::H5)
		.Properties
			("str", &Foobar::str)
		;
	ASSERT_TRUE(R.HasClass("Foobar"));
	Pointer<Foobar> foo = R.New<Foobar>();
	Pointer<Foobar> foo2 = R.NewFromClassName("Foobar");

	ASSERT_TRUE(foo);
	ASSERT_TRUE(foo2);
	ASSERT_FALSE(foo->str.Exists());
	ASSERT_FALSE(foo2->str.Exists());
	ASSERT_TRUE(foo.HasMethod("M0"));
	ASSERT_TRUE(foo2.HasMethod("M0"));

	MethodBase const *R_M3_C = foo.GetMethod("R_M3_C");
	ASSERT_TRUE(R_M3_C);
	String text = R_M3_C->ToString(R);
	EXPECT_EQ(text, "float R_M3_C(int, float &, String const &) const;");

	Pointer<String> S = R.New<String>();
	foo->str = S;
	ASSERT_TRUE(foo->str.Exists());
	ASSERT_TRUE(S.GetParentHandle() == foo.GetHandle());


}

//EOF

