// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#include "Precompiled.h"
#include "Om/ObjectModel.h"
#include "Om/Reflected.h"
#include "Om/Field.h"
#include "Om/Compiler.h"

OM_USING_NAMESPACE_OM();

struct TestCompiler : testing::Test
{
	void TestRho();
	void TestPi();
};

TEST_F(TestCompiler, TestPi)
{
	Registry R;
	RegisterBuiltins(R);

	Pointer<Continuation> C;
	
	C = compiler->CompileText("rpn[ {} ];");
	ASSERT_TRUE(C);
	ASSERT_TRUE(C->Code);
	ASSERT_EQ(C->Code->size(), 0);

	C = compiler->CompileText("rpn[ {{1 2 +}} ];");
	ASSERT_TRUE(C->Code);
	ASSERT_EQ(C->Code->size(), 1);
	EXPECT_TRUE(C->Code->at(0).IsType<Continuation>());
	Pointer<Continuation> C2 = C->Code->at(0);
	ASSERT_TRUE(C2);
	ASSERT_EQ(C2->Code->size(), 1);
	Pointer<Continuation> C3 = C2->Code->at(0);
	ASSERT_TRUE(C3);
	ASSERT_EQ(C3->Code->size(), 3);
	EXPECT_TRUE(C3->Code->at(0).IsType<int>());
	EXPECT_TRUE(C3->Code->at(1).IsType<int>());
	EXPECT_TRUE(C3->Code->at(2).IsType<Operation>());
}

TEST_F(TestCompiler, TestRho)
{
	Registry R;
	RegisterBuiltins(R);

	EXPECT_FALSE(compiler->CompileText(";"));
	EXPECT_FALSE(compiler->CompileText(";// this is a comment\n"));
	EXPECT_FALSE(compiler->CompileText(";/* this is a comment */"));
	EXPECT_FALSE(compiler->CompileText(";/* this is a // comment */"));
	EXPECT_FALSE(compiler->CompileText(";/* this is a \n comment */"));
	EXPECT_TRUE(compiler->CompileText("1+2;"));
	EXPECT_TRUE(compiler->CompileText("a.b();"));
	EXPECT_TRUE(compiler->CompileText("a.b.c();"));
	EXPECT_TRUE(compiler->CompileText("a=1;"));
	EXPECT_TRUE(compiler->CompileText("a.b=1;"));
	EXPECT_TRUE(compiler->CompileText("a.b.c=1;"));
	EXPECT_TRUE(compiler->CompileText("a.b.c=a;"));
	EXPECT_TRUE(compiler->CompileText("a.b.c=a.b;"));
	EXPECT_TRUE(compiler->CompileText("a.b.c=a.b();"));
	EXPECT_TRUE(compiler->CompileText("`a:=1;"));
	EXPECT_TRUE(compiler->CompileText("`a.b:=1;"));
	EXPECT_TRUE(compiler->CompileText("a(1,2,3);"));
	EXPECT_TRUE(compiler->CompileText("a.b(1,a.b());"));
	EXPECT_TRUE(compiler->CompileText("a.b(1,a.b()).a.b;"));
	EXPECT_TRUE(compiler->CompileText("a.b(1,a.b()).a.b();"));
	EXPECT_TRUE(compiler->CompileText("a.b(1,a.b()).a.b(1,2,3);"));


	EXPECT_TRUE(compiler->CompileText("{}"));
	EXPECT_TRUE(compiler->CompileText("{{}}"));
	EXPECT_TRUE(compiler->CompileText("{{{}}}"));
	EXPECT_TRUE(compiler->CompileText("{`a := 1; `b := a + 2; }"));
	EXPECT_TRUE(compiler->CompileText("{`a := new Vector; a.push_back(1); a.push_back(2); }"));
	//ASSERT_TRUE(compiler.CompileText("foo.'bar(\"spam\",42) + 4*(a + b)*12 + spam.baz.foo().grok.'lol();"));
}

//EOF

