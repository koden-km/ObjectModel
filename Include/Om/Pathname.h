// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

OM_BEGIN

struct Pathname : IToString
{
	struct Element
	{
		enum Type
		{
			None = 0,
			Seperator,
			This,
			Parent,
			Ident,
			Last
		};
		Type type;
		Label label;
		Element(Label const &L) : type(Ident) { }
		Element(Type T = None) : type(T) { }
		friend bool operator==(Element const &A, Element const &B);
		friend bool operator<(Element const &A, Element const &B);
	};
	typedef std::vector<Element> Elements;
	static const boost::array<char, Element::Last> ElemChars;

private:
	Elements elements;
	bool absolute;

public:
	Pathname();
	Pathname(String const &);

	bool IsRelative() const;
	bool IsAbsolute() const;

	String ToString() const;

	Elements const &GetElements() const { return elements; }

	template <class Fun>
	void ForEachElement(Fun &fun) const
	{
		foreach (Element const &element, elements)
		{
			fun(element);
		}
	}

	friend bool operator==(Pathname const &A, Pathname const &B);
	friend bool operator<(Pathname const &A, Pathname const &B);

private:
	void FromString(String const &);
};

OM_TRAITS_OPS(Pathname, Operations::Streaming | Operations::Relational | Operations::Assign);

OM_END

//EOF
