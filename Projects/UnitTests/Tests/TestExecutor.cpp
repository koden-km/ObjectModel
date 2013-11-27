// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#include "Precompiled.h"
#include "Om/ClassBuilder.h"

OM_USING_NAMESPACE_OM();

struct TestExecutor : testing::Test
{
	void Test();
};

struct Foo
{
	void Bar()
	{
	}
	int Spam()
	{
		return 42;
	}
	void Baz() const
	{
	}
	float Grok() const
	{
		return 3.14f;
	}

	void Bar2(int N)
	{
	}
	int Spam2(float F)
	{
		return 42;
	}
	std::string Baz2(std::string const &S) const
	{
		return S + S;
	}
	float Grok2(int ) const
	{
		return 3.14f;
	}
};

OM_BEGIN
OM_TRAITS_NUMBER_NAME_OPS(Foo, 1001, "Foo", Operations::None);
OM_END

TEST_F(TestExecutor, Test)
{
	Registry R;
	RegisterBuiltins(R);

	ClassBuilder<Foo>(R)
		.Methods
			("Bar", &Foo::Bar)
			("Spam", &Foo::Spam)
			("Baz", &Foo::Baz)
			("Grok", &Foo::Grok)
			("Bar2", &Foo::Bar2)
			("Spam2", &Foo::Spam2)
			("Baz2", &Foo::Baz2)
			("Grok2", &Foo::Grok2)
		;


	Pointer<int> K = R.New(1);
	ASSERT_TRUE(K.Exists());
	ASSERT_TRUE(!K.IsEmpty());
	ASSERT_TRUE(K.IsType<int>());
	ASSERT_TRUE(*K == 1);

	Pointer<int> L = K.GetClassBase()->Plus(K, K);
	ASSERT_TRUE(L.Exists());
	ASSERT_TRUE(*L == 2);

	Pointer<Vector> code = R.New<Vector>();
	code->push_back(R.New(2));
	code->push_back(R.New(2));
	code->push_back(R.New(Operation(Operation::Plus)));

	Pointer<Executor> E = R.New<Executor>();
	Pointer<Continuation> C = R.New<Continuation>();
	C->Set(code);
	E->Continue(C);
	ASSERT_TRUE(Deref<int>(E->Pop()) == 4);

	Pointer<Foo> foo = R.New<Foo>();

	code->clear();
	code->push_back(foo);
	code->push_back(R.New(Label("`Spam")));
	code->push_back(R.New(Operation(Operation::GetProperty)));
	code->push_back(R.New(Operation(Operation::MethodCall)));
	C->Set(code);
	//E->EnableTrace();
/*
	E->Continue(C);
	ASSERT_TRUE(Deref<int>(E->Pop()) == 42);


	code->clear();
	code->push_back(R.New(String("lol")));
	code->push_back(foo);
	code->push_back(R.New(Label("`Baz2")));
	code->push_back(R.New(Operation(Operation::GetProperty)));
	code->push_back(R.New(Operation(Operation::MethodCall)));
	C->Set(code);
	E->Continue(C);
	ASSERT_TRUE(Deref<String>(E->Pop()) == "lollol");
	*/
}

//EOF
