// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

#include "Om/ObjectModel.h"
#include "Om/Method.h"
#include "Om/Property.h"

OM_BEGIN

/// utility system for convenient creation of class definitions
template <class Klass>
struct ClassBuilder
{
private:
	Class<Klass> *klass;			///< the klass that is being created

public:
	ClassBuilder(Registry &R)
	{
		klass = R.AddClass<Klass>();
		Methods.klass = Methods.Properties.klass = klass;
	}

	struct AddMethods
	{
		Class<Klass> *klass;
		struct AddProperties
		{
			Class<Klass> *klass;
			template <class T>
			AddProperties &operator()(Label const &label, T (Klass::*prop))
			{
				klass->AddProperty(label, new Property<Klass,T>(prop, label));
				return *this;
			}
		};
		AddProperties Properties;

		template <class Method>
		AddMethods &operator()(Label const &label, Method method)
		{
			klass->AddMethod(label, Detail::MakeMethod(method));
			return *this;
		}
	};

	AddMethods Methods;
};

OM_END

//EOF