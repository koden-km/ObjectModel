// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#include "Precompiled.h"
#include "Om/ObjectModel.h"
#include "Om/Compiler.h"
#include "Om/Runtime.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class HighResTimer
{
	long long performance_frequency;
public:

	HighResTimer();
	long long GetTime() const;
};

HighResTimer::HighResTimer()
{
	LARGE_INTEGER perf_freq;
	QueryPerformanceFrequency(&perf_freq);
	performance_frequency = perf_freq.QuadPart / 1000;
}

long long HighResTimer::GetTime() const
{
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);
	return counter.QuadPart / performance_frequency;
}

HighResTimer timer;

OM_USING_NAMESPACE_OM();

struct TestScripts : testing::Test
{
	void Test();

	void RunTestFile(const char *file, bool trace = false)
	{
		// TODO!
		return;

		Runtime runtime;
		try
		{
			runtime.compiler->PrintTree = runtime.New(trace);
			long long start = timer.GetTime();
			Pointer<Continuation> continuation = runtime.CompileFile(file);
			long long end = timer.GetTime();
			long long compile_delta_millis = end - start;
			//OM_TRACE_2(0, file, compile_delta_millis);
			ASSERT_TRUE(continuation);
			runtime.executor->EnableTrace(trace);
			start = timer.GetTime();
			runtime.Continue(continuation);
			end = timer.GetTime();
			long long run_delta_millis = end - start;
			//OM_TRACE_2(0, file, run_delta_millis);
		}
		catch (ExceptionBase &e)
		{
			FileLocation file_location = runtime.executor->GetLastFileLocation();
			file_location.file = file;
			OM_TRACE(0) << e << endl << file_location;// << endl << outdent;
			//ASSERT_TRUE(false);
		}
	}
};

TEST_F(TestScripts, Test)
{
	RunTestFile("Scripts/TestTrace.rho");
	RunTestFile("Scripts/TestGeneral.rho");
	RunTestFile("Scripts/TestPi.pi");
	RunTestFile("Scripts/TestConditionals.rho");
	RunTestFile("Scripts/TestArithmetic.rho");
	RunTestFile("Scripts/TestLoops.rho");
	RunTestFile("Scripts/TestScope.rho");
	RunTestFile("Scripts/TestLambda.rho");
}

//EOF
