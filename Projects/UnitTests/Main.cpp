// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#include "Precompiled.h"

#include <iostream>

#ifdef _DEBUG
#pragma comment(lib, "C:/Dev/External/gtest-1.7.0/msvc/gtest/Debug/gtestd.lib")
#else
#pragma comment(lib, "C:/Dev/External/gtest-1.7.0/msvc/gtest/Debug/gtest.lib")
#endif

int main(int argc, char* argv[])
{
	OM_DEBUG_STREAM(4).Disable();
	OM_DEBUG_STREAM(5).Disable();

	testing::InitGoogleTest(&argc, argv);
	int result = RUN_ALL_TESTS();
	char buffer[12];
	std::cin.getline(buffer, 12);
}

//EOF
