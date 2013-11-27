// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

OM_BEGIN

OM_EXCEPTION(NotImplemented);
OM_EXCEPTION(InvalidOperation);
OM_EXCEPTION(NoTracer);
OM_EXCEPTION(NoAssign);
OM_EXCEPTION(TypeMismatch);
OM_EXCEPTION(ClassAlreadyRegistered);
OM_EXCEPTION(EmptyStack);
OM_EXCEPTION(NullObject);
OM_EXCEPTION(NoLogicalNegation);
OM_EXCEPTION(NoBoolConversion);
OM_EXCEPTION(ConstError);
OM_EXCEPTION(EmptyContainer);
OM_EXCEPTION(AssertionFailure);
OM_EXCEPTION(CannotPreInc);


#define OM_NOT_IMPLEMENTED() \
	OM_THROW(NotImplemented)

OM_END

//EOF
