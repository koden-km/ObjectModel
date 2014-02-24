#pragma once

namespace Debug
{
	struct ITraceable
	{
		virtual void Trace(StringStream&) const = 0;
	};
} // namespace Debug

//EOF
