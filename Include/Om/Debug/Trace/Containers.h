#pragma once

#include <map>
#include <list>
#include <set>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>
#include "Om/Utility/Foreach.h"

/// CJS fail
#ifdef WIN32
#define foreach_delim_(A,B,C,D) \
	{							\
		bool first = true;		\
		foreach(A, B)			\
		{						\
			if (!first)			\
				S << D;			\
			C;					\
			first = false;		\
		}						\
	}

#define foreach_delim(A,B,C) foreach_delim_(A,B,C,param_sep)
#else

#define foreach_delim( A__, B__, C__ )

#endif

namespace Debug
{
	namespace detail
	{
		template <class T, class Stream, class Cont>
		void WriteContainer(Stream &S, Cont const &C)
		{
			foreach_delim (T const &X, C, 
				S << X)
		}

		template <class T, class Stream, class Cont>
		void WriteContainerDeref(Stream &S, Cont const &C)
		{
			foreach_delim (T const &X, C, 
				if (X) 
					S << *X; 
				else 
					S << "null";)
		}

		template <class Stream, class K, class T, class P, class Al>
		void WriteContainerDeref(Stream &S, std::map<K, T*, P, Al> const &C)
		{
			typedef std::map<K, T*, P, Al> map;
			foreach_delim (map::value_type const &X, C,
				S << "pair(" << X.first << ", " << *X.second << ')';)
		}
		
		template <class Cont>
		struct DerefWrapper 
		{ 
			Cont const &container; 
			DerefWrapper(Cont const &X) : container(X) { } 
		};

		template <class T>
		DerefWrapper<T> DerefPointers(T const &C) { return DerefWrapper<T>(C); }

	} // namespace detail

	template <class Stream, class V, size_t N>
	void Write(Stream &S, boost::array<V,N> const &array)
	{
		OM_TRACE_OBJECT_1("array", S, array.size());
		S << param_sep;
		detail::WriteContainer<V>(S, array);
	}

	template <class Stream, class K, class V, class Pr, class Al>
	void Write(Stream &S, std::map<K, V, Pr, Al> const &map)
	{
		OM_TRACE_OBJECT_1("map", S, map.size());
		S << param_sep;
		detail::WriteContainer<std::pair<const K, V> >(S, map);
	}

	template <class Stream, class V, class Pr, class Al>
	void Write(Stream &S, std::set<V, Pr, Al> const &set)
	{
		OM_TRACE_OBJECT_1("set", S, set.size());
		S << param_sep;
		detail::WriteContainer<V>(S, set);
	}
	template <class Stream, class T, class Al>
	void Write(Stream &S, std::list<T,Al> const &V)
	{
		OM_TRACE_OBJECT_1("list", S, V.size());
		S << param_sep;
		detail::WriteContainer<T>(S, V);
	}
	template <class Stream, class T, class Al>
	void Write(Stream &S, std::vector<T,Al> const &V)
	{
		OM_TRACE_OBJECT_1("vector", S, V.size());
		S << param_sep;
		detail::WriteContainer<T>(S, V);
	}
	template <class Stream, class T, class Al>
	void Write(Stream &S, boost::ptr_vector<T,Al> const &V)
	{
		OM_TRACE_OBJECT_1("ptr_vector", S, V.size());
		S << param_sep;
		detail::WriteContainer<T>(S, V);
	}
	template <class Stream, class T, class Al>
	void Write(Stream &S, detail::DerefWrapper<std::vector<T,Al> > const &V)
	{
		OM_TRACE_OBJECT_1("vector", S, V.container.size());
		S << param_sep;
		detail::WriteContainerDeref<T>(S, V.container);
	}
	template <class Stream, class T, class Al>
	void Write(Stream &S, detail::DerefWrapper<std::vector<boost::shared_ptr<T>,Al> > const &V)
	{
		OM_TRACE_OBJECT_1("vector", S, V.container.size());
		S << param_sep;
		detail::WriteContainerDeref<boost::shared_ptr<T> >(S, V.container);
	}
	template <class Stream, class T, class Al, class Pr>
	void Write(Stream &S, detail::DerefWrapper<std::set<T *,Pr,Al> > const &V)
	{
		OM_TRACE_OBJECT_1("set", S, V.container.size());
		S << param_sep;
		detail::WriteContainerDeref<T>(S, V.container);
	}

	template <class Stream, class T, class Al, class Pr>
	void Write(Stream &S, detail::DerefWrapper<std::set<boost::shared_ptr<T>,Pr,Al> > const &V)
	{
		OM_TRACE_OBJECT_1("set", S, V.container.size());
		S << param_sep;
		detail::WriteContainerDeref<boost::shared_ptr<T> >(S, V.container);
	}

	template <class Stream, class K, class T, class P, class Al>
	void Write(Stream &S, detail::DerefWrapper<std::map<K, T*, P, Al> > const &V)
	{
		OM_TRACE_OBJECT_1("map", S, V.container.size());
		S << param_sep;
		detail::WriteContainerDeref(S, V.container);
	}
} // namespace Debug

//EOF

