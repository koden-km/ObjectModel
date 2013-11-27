// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#include "Om/Precompiled.h"
#include "Om/ClassBuilder.h"
#include "Om/Lambda.h"

OM_BEGIN

void TraceDictionary(Object const &Q);

template <class Ret, class A0, class A1>
void Executor::BinaryOpRet(Ret (ClassBase::*method)(A0, A1) const)
{
	Object B = ResolvePop();
	Object A = ResolvePop();
	Push(New((A.GetClass()->*method)(A, B)));
}

template <class A0, class A1>
void Executor::BinaryOpVoid(void (ClassBase::*method)(A0, A1) const)
{
	Object B = ResolvePop();
	Object A = ResolvePop();
	(A.GetClass()->*method)(A, B);
}

template <class A0, class A1>
void Executor::BinaryOp(Object (ClassBase::*method)(A0, A1) const)
{
	Object B = ResolvePop();
	Object A = ResolvePop();
	Push((A.GetClass()->*method)(A, B));
}

void Executor::Perform(Operation op)
{
	switch (op.type)
	{
	case Operation::Break:
		{
			OM_NOT_IMPLEMENTED();
		}
		break;
	case Operation::Continue:
		{
			OM_NOT_IMPLEMENTED();
		}
		break;
	case Operation::Return:
		{
			Object result = DeepCopy(Pop());
			Leave();
			Push(result);
			Break();
		}
		break;
	case Operation::ForLoop:
		{
			Pointer<Continuation> body = Pop();
			Pointer<Continuation> step = Pop();
			Pointer<Continuation> cond = Pop();

			size_t level = context.size();
			PushContext(continuation);
			while (true)
			{
				ContinueOnly(cond);
				if (!Boolean(Pop()))
				{
					break;
				}
				ContinueOnly(body);
			 	ContinueOnly(step);
			}
			continuation = context.pop();
			size_t after = context.size();
			assert(after == level);
		}
		break;
	case Operation::PlusAssign:
		{
			OpAssign(Plus);
		}
		break;
	case Operation::MinusAssign:
		{
			OpAssign(Minus);
		}
		break;
	case Operation::MultiplyAssign:
		{
			OpAssign(Multiply);
		}
		break;
	case Operation::DivideAssign:
		{
			OpAssign(Divide);
		}
		break;
	case Operation::PreInc:
		{
			Object A = Pop();
			switch (A.GetTypeNumber().value)
			{
			case Type::Number::Int:
				++Deref<int>(A);
				break;
			case Type::Number::Size:
				++Deref<size_t>(A);
				break;
			case Type::Number::Float:
				++Deref<float>(A);
				break;
			default:
				OM_THROW(CannotPreInc);
				break;
			}
		}
		break;
	case Operation::Swap:
		{
			Object A = Pop();
			Object B = Pop();
			Push(A);
			Push(B);
		}
		break;
	case Operation::Suspend:
		PushContext(Current());
	case Operation::Replace:
		PushContext(Pop());
	case Operation::Resume:
		Break();
		break;
	case Operation::IfThenSuspend:
		{
			bool flag = Boolean(Pop());
			Pointer<Continuation> true_statements = Pop();
			if (flag)
			{
				PushContext(Current());
				PushContext(true_statements);
				Break();
			}
		}
		break;
	case Operation::IfThenSuspendElseSuspend:
		{
			bool flag = Boolean(Pop());
			Pointer<Continuation> false_statement = Pop();
			Pointer<Continuation> true_statement = Pop();
			PushContext(Current());
			PushContext(flag ? true_statement : false_statement);
			Break();
		}
		break;
	case Operation::Exists:
		{
			Push(New(Resolve(ConstDeref<Label>(Pop())).Exists()));
		}
		break;
	case Operation::Assert:
		{
			Object exp = Pop();
			if (!Boolean(exp))
			{
				OM_THROW(AssertionFailure);
			}
		}
		break;
	case Operation::This:
		{
			// TODO: add tree and scope
			Push(Self->GetRegistry().GetRoot());
		}
		break;
	case Operation::LogicalAnd:
		{
			bool A = Boolean(ResolvePop());
			Push(New(A && Boolean(ResolvePop())));
		}
		break;
	case Operation::LogicalOr:
		{
			// use short-circuiting or not? what impact on stack if so?

			//bool A = Boolean(ResolvePop());
			//Push(New(A || Boolean(ResolvePop())));
			bool A = Boolean(ResolvePop());
			bool B = Boolean(ResolvePop());
			Push(New(A || B));
		}
		break;
	case Operation::LogicalTrue:
		{
			Push(New(Boolean(ResolvePop())));
		}
		break;
	case Operation::LogicalNot:
		{
			Push(New(Not(ResolvePop())));
		}
		break;
	case Operation::Equiv:
		{
			BinaryOpRet(&ClassBase::Equiv);
		}
		break;
	case Operation::NotEquiv:
		{
			BinaryOpRet(&ClassBase::NotEquiv);
		}
		break;
	case Operation::Less:
		{
			BinaryOpRet(&ClassBase::Less);
		}
		break;
	case Operation::Assign:
		{
			Object A = Pop();
			Object B = Pop();
			// be nice about `a := foo; vs. `a = foo;
			if (B.IsType<Label>())
			{
				Label const &label = ConstDeref<Label>(B);
				Label foo(label.GetText(), false);
				continuation->Scope[foo] = A;
				assert(Resolve(foo));
			}
			else
			{
				B.GetClass()->Assign(B, A);
			}
		}
		break;
	case Operation::GetProperty:
		{
			Label const &label = Deref<Label>(Pop());
			Object object = Pop();

			ClassBase const *klass = object.GetClass();
			if (klass->HasProperty(label))
			{
				Push(klass->GetProperty(label)->Get(object));
				break;
			}
			if (MethodBase const *method = klass->GetMethod(label))
			{
				// TODO: dont create a new method object each time; store it in the class
				Push(New(method));
				// re-push the object onto the stack when getting a method as we are prolly gonna use it again soon
				Push(object);
				break;
			}
			if (Object child = object.Get(label))
			{
				Push(child);
				break;
			}
			OM_THROW_1(UnknownPropertyOrMethod, label);
		}
		break;
	case Operation::New:
		{
			Object const object = Pop();
			if (object.IsType<Label>())
				Push(Self->GetRegistry().NewFromClassName(Deref<Label>(object)));
			else
				Push(Self->GetRegistry().NewFromClass(Deref<ClassBase const *>(object)));
		}
		break;
	case Operation::Delete:
		{
			Pop().Delete();
		}
		break;
	case Operation::Typeof:
		{
			Push(New<ClassBase const *>(Pop().GetClass()));
		}
		break;
	case Operation::Enter:
		{
			enter_level.push_back(data.size());
		}
		break;
	case Operation::Leave:
		{
			Leave();
		}
		break;
	case Operation::Dup:
		{
			Push(DeepCopy(Top()));
		}
		break;
	case Operation::Drop:
		{
			Pop();
		}
		break;
	case Operation::Clear:
		{
			data.clear();
		}
		break;
	case Operation::DebugBreak:
		{
			TraceScopes();
			EnableTrace(true);
#ifdef WIN32
			__asm int 3
#endif
		}
		break;
	case Operation::Quit:
		{
			exit(0);
		}
		break;
	case Operation::Plus:
		{
			BinaryOp(&ClassBase::Plus);
		}
		break;
	case Operation::Minus:
		{
			BinaryOp(&ClassBase::Minus);
		}
		break;
	case Operation::Multiply:
		{
			BinaryOp(&ClassBase::Multiply);
		}
		break;
	case Operation::Modulo:
		{
			// quick hax; can only modulo ints
			Pointer<int> B = Pop();
			Pointer<int> A = Pop();
			Push(New(*A % *B));
		}
		break;
	case Operation::Divide:
		{
			BinaryOp(&ClassBase::Divide);
		}
		break;
	case Operation::MethodCall:
		{
			Object object = Pop();
			switch (object.GetTypeNumber().value)
			{
			case Type::Number::Lambda:
				{
					Lambda const &lambda = ConstDeref<Lambda>(object);
					lambda.Enter(*this);
				}
				break;
			case Type::Number::Continuation:
				{
					PushContext(Current());
					PushContext(object);
					Break();
				}
				break;
			default:
				{
					Pointer<MethodBase const *> method = Pop();
					(*method)->Invoke(object, data);
				}
				break;
			}
		}
		break;
	case Operation::Store:
		{
			Object object = Pop();
			Label const &label = Deref<Label>(Pop());
			continuation->Scope[label] = object;
		}
		break;
	case Operation::Retrieve:
		{
			Push(ResolvePop());
		}
		break;
	case Operation::Trace:
		{
			Object Q = Pop();
			Debug::Write(last_file_location.ToString() + Q.GetName().ToString() + ": " + Q.ToString() + "\n");
		}
		break;
	}
}

OM_END

//EOF
