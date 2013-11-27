// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

// demo code using a C++ reflected object model

#include "Om/Runtime.h"
#include "Om/ClassBuilder.h"

using namespace Om;

/// a test class for demonstrating reflection to scripts.
/// note that it does not need to derive from any base class, allowing
/// builtin types and external types to be used.
struct Foo
{
	int num;
	String str;

	~Foo()
	{
		destroyed = true;
	}
	
	// a method that will be exposed to the runtime
	// exposed methods can be const or not, can return void or not,
	// can take any argument types, which can be const and/or reference types
	String spam(String const &S, int N) const
	{
		String R = S;
		while (N-- > 0)
		{
			R += S;
		}
		return R;
	}

	/// need to register the type with a registry before use
	static void Register(Registry &R)
	{
		ClassBuilder<Foo>(R)
			.Methods
				("spam", &Foo::spam)
			.Properties
				("num", &Foo::num)
				("str", &Foo::str)
			;
	}

	static bool destroyed;
};

// define the type-number and type-traits for the foo object
USER_TYPE_NUMBER(Foo, 1000);

int main()
{
	{
		// everything you need to use the system is in a Runtime. 
		// you can have multiple runtimes in one address space.
		Runtime runtime;

		// add our custom class
		runtime.Register<Foo>();

		// demonstrate expressions
		Pointer<String> result = runtime.Execute("\"Hello\" + \", World\";");
		assert(*result == "Hello, World");

		// object storage and retrieval
		Pointer<int> num = runtime.Execute("`n := 2; n*3 + 4;");
		assert(*num == 2*3 + 4);

		// create and use C++ types from script
		// can set, get any members and call any methods
		// this is a complete language; with loops, conditionals and lambdas
		Object object = runtime.Execute("\
			`foo := new Foo;				// create a new foo		\n\
			foo.num = 3;					// assign to a field	\n\
			foo.spam(\"bar\", foo.num);		// invoke a method		\n\
			");
		assert(object.IsType<String>());
		assert(Deref<String>(object) == "barbarbarbar");

		// demonstrate persistence; create a new foo and a packet
		Pointer<Foo> foo = runtime.New<Foo>();
		Pointer<BinaryStream> packet = runtime.New<BinaryStream>();

		// write to fields of foo
		foo->num = 123;
		foo->str = "grok";

		// insert foo into a packet, then extract an object from that packet
		*packet << foo;
		Pointer<Foo> out;
		*packet >> out;

		// ensure foo was read correctly
		assert(out->num == 123);
		assert(out->str == "grok");

		// complete reflection of members and methods
		MethodBase const *spam_method = foo.GetMethod("spam");
		assert(spam_method);
		assert(spam_method->ToString(runtime.registry) == "String spam(String const &, int) const;");
	}

	// there are no globals used by the system, so everything is on the stack and destroyed when the above
	// code block ends
	assert(Foo::destroyed);
	return 0;
}

bool Foo::destroyed = false;

//EOF
