#pragma once

#include "Om/Utility/BaseBase.h"

struct Optional : virtual BaseBase
{
private:
	bool enabled;

public:
	Optional(bool E = true) : enabled(E) { }

	bool Enabled() const { return enabled; }
	void Enable(bool E = true) { enabled = E; }
	bool Disabled() const { return !enabled; }
	void Disable(bool E = true) { enabled = !E; }
};

//EOF
