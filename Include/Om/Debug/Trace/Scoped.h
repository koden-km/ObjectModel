#pragma once

namespace Debug
{
	class Scoped
	{
		StringStream &stream;
		const char *name;

	public:
		Scoped(StringStream& stream, const char *name);
		~Scoped();
	};
}

//EOF
