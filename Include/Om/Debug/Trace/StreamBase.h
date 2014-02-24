#pragma once

#include "Om/Utility/Optional.h"

namespace Debug
{
	/// interface common to all trace streams
	struct IStream : IToString
	{
		virtual void Clear() = 0;

		virtual void Write(char) = 0;
		virtual void Write(char const *) = 0;
		virtual void Write(std::string const &) = 0;
	};

	/// structure common to all trace streams
	struct StreamBase : Optional, IStream
	{
		void NewLine();
		void Indent();
		void Outdent();
		void ParameterSeparator();
		void SetIndent(size_t N) { indent_level = (int)N; }

	protected:
		int indent_level;

		StreamBase(bool active = true);
		~StreamBase();
	};

	struct endl_struct {};
	struct indent_struct {};
	struct outdent_struct {};
	struct param_sep_struct {};

	extern endl_struct endl;
	extern indent_struct indent;
	extern outdent_struct outdent;
	extern param_sep_struct param_sep;

} // namespace Debug

using Debug::endl;
using Debug::indent;
using Debug::outdent;
using Debug::param_sep;

//EOF
