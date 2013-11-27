// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

OM_BEGIN

/// a textual object identifier
struct Label : Debug::ITraceable, IToString
{
	String text;

	Label() { }
	Label(const char *S, bool quote = false);
	Label(String const &S, bool quote = false);
	Label& operator=(Label const &);

	bool Quoted;

	void Trace(Debug::StringStream &) const;

	String const &GetText() const { return text; }
	String ToString() const { return GetText(); }

	friend Label operator+(Label const &A, Label const &B) { return Label(A.text + B.text, A.Quoted || B.Quoted); }
	friend bool operator<(Label const &A, Label const &B) { return A.text < B.text; }
	friend bool operator==(Label const &A, Label const &B) { return A.text == B.text; }

protected:
	void SetText(const  char *);
};

BinaryStream& operator<<(BinaryStream&, Label const &);
BinaryStream& operator>>(BinaryStream&, Label &);

OM_TRAITS_NUMBER_NAME_OPS(Label, Number::Label, "Label"
	, Operations::Streaming | Operations::Plus | Operations::Relational | Operations::Assign);


struct IHasLabel : BaseBase
{
	virtual Label const &GetLabel() const =  0;

protected:
	virtual void SetLabel(Label const &L)  = 0;
};

/// derive from this to get a label property
struct HasLabel : IHasLabel
{
private:
	Label label;

public:
	void SetLabel(Label const &L) { label = L; }
	Label const &GetLabel() const { return label; }

protected:
	HasLabel() { }
	HasLabel(Label const &L) : label(L) { }
};

OM_EXCEPTION_1(Unresolved, Label, label);
OM_EXCEPTION_1(UnknownPropertyOrMethod, Label, label);
OM_EXCEPTION_1(UnknownClass, Label, label);
OM_EXCEPTION_1(UnknownProperty, Label, label);

OM_END

//EOF
