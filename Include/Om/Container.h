// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

#include <map>
#include <list>
#include <vector>

OM_BEGIN

/// common functionality for all sequence containers
template <class Cont>
struct SequenceContainer : Debug::ITraceable, IMarkable
{
	typedef Cont Container;
	typedef typename Container::value_type value_type;
	typedef typename Container::iterator iterator;
	typedef typename Container::const_iterator const_iterator;

	Container  container;

	bool empty() const { return container.empty(); }
	void clear() { container.clear(); }

	iterator begin() { return container.begin(); }
	iterator end() { return container.end(); }
	const_iterator begin() const { return container.begin(); }
	const_iterator end() const { return container.end(); }
	size_t size() const { return container.size(); }
	void push_back(value_type const &X) { container.push_back(X); }
	void pop_back() 
	{ 
		if (container.empty())
		{
			OM_THROW_0(EmptyContainer);
		}
		container.pop_back(); 
	}
	value_type const &front() const 
	{ 
		if (container.empty())
		{
			OM_THROW_0(EmptyContainer);
		}
		return container.front(); 
	}
	value_type const &back() const 
	{ 
		if (container.empty())
		{
			OM_THROW_0(EmptyContainer);
		}
		return container.back(); 
	}

	value_type pop()
	{
		value_type top = back();
		pop_back();
		return top;
	}

	void Trace(Debug::StringStream &S) const
	{
		S << indent << endl;
		foreach_delim (Object const &object, container, 
			S << object)
		S << outdent << endl;
	}

};

template <class Cont>
inline StringStream& operator<<(StringStream& S, SequenceContainer<Cont> const &C)
{
	foreach_delim (typename Cont::value_type const &X, C,
		S << X);
	return S;
}

template <class Cont>
struct ReflectedSequence : SequenceContainer<Cont>, ReflectedBase
{
	void SetMarked(MarkingContext &mark) const
	{
		foreach (Object const &object, container)
		{
			object.SetMarked(mark);
		}
	}

	friend bool operator==(ReflectedSequence<Cont> const &A, ReflectedSequence<Cont> const &B)
	{
		return A.size() == B.size() && std::equal(A.begin(), A.end(), B.begin(), Equiv);
	}
	friend bool operator<(ReflectedSequence<Cont> const &A, ReflectedSequence<Cont> const &B)
	{
		return std::lexicographical_compare(A.begin(), A.end(), B.begin(), B.end(), Less);
	}

};

/// a wrapper for a std::vector<Object>
struct Vector : ReflectedSequence<std::vector<Object> >
{
	typedef ReflectedSequence<std::vector<Object> > Parent;

	bool empty() const { return Parent::empty(); }
	size_t size() const { return (int)Parent::size(); }
	Object const &front() const { return Parent::front(); }
	Object const &back() const { return Parent::back(); }
	void push_back(Object const &Q) { Parent::push_back(Q); }
	void pop_back() { Parent::pop_back(); }
	Object const &at(size_t N) const { return container.at(N); }
	Object pop() 
	{ 
		Object top = back();
		pop_back();
		return top;
	}
	
	static void Register(Registry &);
};

/// a wrapper for a std::list<Object>
struct List : ReflectedSequence<std::list<Object> >
{
	typedef ReflectedSequence<std::list<Object> > Parent;
	bool empty() const { return Parent::empty(); }
	size_t size() const { return (int)Parent::size(); }
	Object const &front() const { return Parent::front(); }
	Object const &back() const { return Parent::back(); }
	void push_back(Object const &Q) { Parent::push_back(Q); }
	void pop_back() { Parent::pop_back(); }
	Object pop() 
	{ 
		Object top = back();
		pop_back();
		return top;
	}
	static void Register(Registry &);
};

OM_TRAITS_OPS(Vector, Operations::Streaming | Operations::Relational | Operations::Assign);
OM_TRAITS_OPS(List,Operations::Streaming | Operations::Relational | Operations::Assign);

OM_END

//EOF
