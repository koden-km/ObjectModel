// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#include "Precompiled.h"
#include "Om/Continuation.h"
#include "Om/Lambda.h"
#include "Om/Compiler.h"

// TODO: move to Om/Generated/*
#include "./RhoLexer.h"
#include "./RhoParser.h"
#include "./Tree.h"

#include <iostream>

OM_BEGIN

Pointer<Compiler> compiler;
void displayRecognitionError(struct ANTLR3_BASE_RECOGNIZER_struct * recognizer, pANTLR3_UINT8 * tokenNames);

/// debug stream number for the compiler
static int cds = Compiler::TraceNumber;

struct CompilerImpl
{
	pANTLR3_INPUT_STREAM input;
    pANTLR3_COMMON_TOKEN_STREAM tstream;
	pRhoLexer lxr;
    pRhoParser psr;
	pTree tree;
};

Compiler::Compiler() 
	: HasTraceStream(TraceNumber), impl(new CompilerImpl)
{
	HasTraceStream::Enable(false);
}

Compiler::~Compiler()
{
	delete impl;
}

void Compiler::Construct()
{
	//if (compiler)
	//	return;
	compiler = *Self;
	Reset();
	PrintTree = New(false);
	Debug = New(false);
	EnableTrace(false);
}

Object/*Pointer<Continuation> */Compiler::CompileFile(String const &filename)
{
	impl->input = antlr3AsciiFileStreamNew((pANTLR3_UINT8)filename.c_str());
	return LexParseWalk();
}

Object/*Pointer<Continuation> */Compiler::CompileText(String const &text)
{
	impl->input = antlr3NewAsciiStringInPlaceStream ((pANTLR3_UINT8)text.c_str(), (ANTLR3_UINT64) text.size(), NULL);
	return LexParseWalk();
}

void Compiler::Reset()
{
	code = New<Vector>();
	stack = New<Vector>();
}

void Compiler::Append(Object const &Q)
{
	OM_TRACE_1(cds, Q);
	code->push_back(Q);
}


void Compiler::AddLambda()
{
	Pointer<Continuation> body = code->pop();
	Pointer<Vector> args = code->pop();
	Pointer<Vector> capture = code->pop();

	Pointer<Lambda> lambda = New<Lambda>();
	foreach (Object const &label, *capture)
		lambda->capture.push_back(Deref<Label>(label));
	foreach (Object const &label, *args)
		lambda->arguments.push_back(Deref<Label>(label));
	lambda->continuation = body;
	
	Append(lambda);
}


Pointer<Continuation> Compiler::LexParseWalk()
{
	try
	{
		Reset();
		if ( impl->input == NULL)
		{
			//std::cerr << "no input";
			//OM_THROW_1(CompileError, "No Input");
			return null;
		}
		//  (file | memory | ?) --> inputstream -> lexer --> tokenstream --> parser --> treeparser
		impl->lxr = RhoLexerNew(impl->input);
		impl->tstream = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(impl->lxr));

		impl->psr = RhoParserNew(impl->tstream);

		// override error handlers
		impl->lxr->pLexer->rec->displayRecognitionError = displayRecognitionError;
		impl->psr->pParser->rec->displayRecognitionError = displayRecognitionError;

		// tokenise and parse
		RhoParser_entry_return ret = impl->psr->entry(impl->psr);
		bool fail = false;
		
		if (impl->lxr->pLexer->rec->state->errorCount > 0)
		{
			fail = true;
		}
		if (!fail && impl->psr->pParser->rec->state->errorCount > 0)
		{
			fail = true;
		}
		else if (!fail && ret.tree)
		{
			if (*PrintTree)
			{
				String tree = (const char *)ret.tree->toStringTree(ret.tree)->chars ;
				Debug::Write(tree + "\n");
			}

			pANTLR3_COMMON_TREE_NODE_STREAM nodes = antlr3CommonTreeNodeStreamNewTree(ret.tree, ANTLR3_SIZE_HINT);
			impl->tree = TreeNew(nodes);

			// walk the tree
			Push();
			impl->tree->entry(impl->tree);
			if (impl->tree->pTreeParser->rec->state->errorCount > 0)
			{
				fail = true;
			}
			else
			{
				PopContinuation();
			}

			nodes->free(nodes); 
			nodes = NULL;
			impl->tree->free(impl->tree);	    
			impl->tree = NULL;
		}

		// TODO: re-use lexer, parser and tree-walker for compiles
		impl->psr->free(impl->psr);	   
		impl->psr = NULL;
		impl->tstream->free(impl->tstream);	    
		impl->tstream = NULL;
		impl->lxr->free(impl->lxr);	    
		impl->lxr = NULL;
		impl->input->close(impl->input);	    
		impl->input = NULL;

		if (!fail && code && !code->empty())
		{
			assert(code->size() == 1);
			Pointer<Continuation> C = code->front();
			Reset();
			return C;
		}
	}
	catch (CompileError &)
	{
		Reset();
		throw;
	}
	Reset();
	return Object();
}

void Compiler::Trace(Debug::StringStream &S) const
{
	//OM_TRACE_REFLECTED(S);
}

void Compiler::Push()
{
	OM_TRACE_1(7, stack->size());

	stack->push_back(code);
	code = New<Vector>();
}

void Compiler::PopContinuation()
{
	OM_TRACE_1(7, stack->size());
	Pointer<Continuation> cont = New<Continuation>();
	cont->Set(code);
	code = stack->pop();
	code->push_back(cont);
	OM_TRACE_1(7, code);
}

void Compiler::PopSequence()
{
	Pointer<Vector> sequence = code;
	code = stack->pop();
	code->push_back(sequence);
}

//void Compiler::PopBlock()
//{
//	Result->push_back(Code);
//	Pop();
//}
//
void Compiler::Pop()
{
	code = stack->pop();
}

void Compiler::Register(Registry &R)
{
	ClassBuilder<Compiler>(R)
		.Methods
			("CompileFile", &Compiler::CompileFile)
			("CompileText", &Compiler::CompileText)
		.Properties
			("Code", &Compiler::code)
			("Stack", &Compiler::stack)
			("PrintTree", &Compiler::PrintTree)
			("Debug", &Compiler::Debug)
		;
	R.New<Compiler>();
}

void displayRecognitionError(struct ANTLR3_BASE_RECOGNIZER_struct * recognizer, pANTLR3_UINT8 * tokenNames)
{
	pANTLR3_PARSER			parser;
	pANTLR3_TREE_PARSER	    tparser;
	pANTLR3_INT_STREAM	    is;
	pANTLR3_STRING			ttext;
	pANTLR3_STRING			ftext;
	pANTLR3_EXCEPTION	    ex;
	pANTLR3_COMMON_TOKEN    theToken;
	pANTLR3_BASE_TREE	    theBaseTree;
	pANTLR3_COMMON_TREE	    theCommonTree;

	Debug::StringStream stream;

	// Retrieve some info for easy reading.
	//
	ex	    =		recognizer->state->exception;
	ttext   =		NULL;

	// See if there is a 'filename' we can use
	//
	if	(ex->streamName == NULL)
	{
		if	(((pANTLR3_COMMON_TOKEN)(ex->token))->type == ANTLR3_TOKEN_EOF)
		{
			stream << " end of input (";
		}
		else
		{
			stream << " unknown source (";
		}
	}
	else
	{
		String name = (const char *)ex->streamName->to8(ex->streamName)->chars;
		if (name == "-memory-")
			name = "text";
		stream << name << "(";
	}

	// Next comes the line number
	//

	stream << recognizer->state->exception->line << "): ";
	stream << " : error " << recognizer->state->exception->type << " : " << (pANTLR3_UINT8) (recognizer->state->exception->message);

	// How we determine the next piece is dependent on which thing raised the
	// error.
	//
	switch	(recognizer->type)
	{
	case	ANTLR3_TYPE_PARSER:

		// Prepare the knowledge we know we have
		//
		parser	    = (pANTLR3_PARSER) (recognizer->super);
		tparser	    = NULL;
		is			= parser->tstream->istream;
		theToken    = (pANTLR3_COMMON_TOKEN)(recognizer->state->exception->token);
		ttext	    = theToken->toString(theToken);

		stream << ", at offset " << recognizer->state->exception->charPositionInLine;
		if  (theToken != NULL)
		{
			if (theToken->type == ANTLR3_TOKEN_EOF)
			{
				stream << ", at <EOF>";
			}
			else
			{
				// Guard against null text in a token
				//
				//stream << " near ";
				//if (ttext)
				//	stream << (const char *)ttext->chars;
				//else
				//	stream << "<no text for the token>";
			}
		}
		break;

	case	ANTLR3_TYPE_TREE_PARSER:

		tparser		= (pANTLR3_TREE_PARSER) (recognizer->super);
		parser		= NULL;
		is			= tparser->ctnstream->tnstream->istream;
		theBaseTree	= (pANTLR3_BASE_TREE)(recognizer->state->exception->token);
		ttext		= theBaseTree->toStringTree(theBaseTree);

		if  (theBaseTree != NULL)
		{
			theCommonTree	= (pANTLR3_COMMON_TREE)	    theBaseTree->super;

			if	(theCommonTree != NULL)
			{
				theToken	= (pANTLR3_COMMON_TOKEN)    theBaseTree->getToken(theBaseTree);
			}
			stream << ", at offset " << theBaseTree->getCharPositionInLine(theBaseTree);
			stream << ", near " << ttext->chars;
		}
		break;

	default:

		//ANTLR3_FPRINTF(stderr, "Base recognizer function displayRecognitionError called by unknown parser type - provide override for this function\n");
		//return;
		stream << " lexer error ";
		break;

	}

	// Although this function should generally be provided by the implementation, this one
	// should be as helpful as possible for grammar developers and serve as an example
	// of what you can do with each exception type. In general, when you make up your
	// 'real' handler, you should debug the routine with all possible errors you expect
	// which will then let you be as specific as possible about all circumstances.
	//
	// Note that in the general case, errors thrown by tree parsers indicate a problem
	// with the output of the parser or with the tree grammar itself. The job of the parser
	// is to produce a perfect (in traversal terms) syntactically correct tree, so errors
	// at that stage should really be semantic errors that your own code determines and handles
	// in whatever way is appropriate.
	//
	switch  (ex->type)
	{
	case	ANTLR3_UNWANTED_TOKEN_EXCEPTION:

		// Indicates that the recognizer was fed a token which seesm to be
		// spurious input. We can detect this when the token that follows
		// this unwanted token would normally be part of the syntactically
		// correct stream. Then we can see that the token we are looking at
		// is just something that should not be there and throw this exception.
		//
		if	(tokenNames == NULL)
		{
			stream << " : fail";
		}
		else
		{
			if	(ex->expecting == ANTLR3_TOKEN_EOF)
			{
				stream << " : expected <EOF>";
			}
			else
			{
				stream << " : expected " << tokenNames[ex->expecting];
			}
		}
		break;

	case	ANTLR3_MISSING_TOKEN_EXCEPTION:

		// Indicates that the recognizer detected that the token we just
		// hit would be valid syntactically if preceeded by a particular 
		// token. Perhaps a missing ';' at line end or a missing ',' in an
		// expression list, and such like.
		//
		if	(tokenNames == NULL)
		{
			stream << " : Missing token " << ex->expecting;
		}
		else
		{
			if	(ex->expecting == ANTLR3_TOKEN_EOF)
			{
				stream << " : Missing <EOF>";
			}
			else
			{
				stream << " : Missing " << tokenNames[ex->expecting];
			}
		}
		break;

	case	ANTLR3_RECOGNITION_EXCEPTION:

		// Indicates that the recognizer received a token
		// in the input that was not predicted. This is the basic exception type 
		// from which all others are derived. So we assume it was a syntax error.
		// You may get this if there are not more tokens and more are needed
		// to complete a parse for instance.
		//
		stream << " : syntax error";    
		break;

	case    ANTLR3_MISMATCHED_TOKEN_EXCEPTION:

		// We were expecting to see one thing and got another. This is the
		// most common error if we coudl not detect a missing or unwanted token.
		// Here you can spend your efforts to
		// derive more useful error messages based on the expected
		// token set and the last token and so on. The error following
		// bitmaps do a good job of reducing the set that we were looking
		// for down to something small. Knowing what you are parsing may be
		// able to allow you to be even more specific about an error.
		//
		if	(tokenNames == NULL)
		{
			stream << " : syntax error";
		}
		else
		{
			if	(ex->expecting == ANTLR3_TOKEN_EOF)
			{
				stream << " : expected <EOF>";
			}
			else
			{
				stream << " : expected " << tokenNames[ex->expecting];
			}
		}
		break;

	case	ANTLR3_NO_VIABLE_ALT_EXCEPTION:

		// We could not pick any alt decision from the input given
		// so god knows what happened - however when you examine your grammar,
		// you should. It means that at the point where the current token occurred
		// that the DFA indicates nowhere to go from here.
		//
		stream << " : cannot match to any input";

		break;

	case	ANTLR3_MISMATCHED_SET_EXCEPTION:

		{
			ANTLR3_UINT32	  count;
			ANTLR3_UINT32	  bit;
			ANTLR3_UINT32	  size;
			ANTLR3_UINT32	  numbits;
			pANTLR3_BITSET	  errBits;

			// This means we were able to deal with one of a set of
			// possible tokens at this point, but we did not see any
			// member of that set.
			//
			stream << " : unexpected input";//...\n  expected one of : ");

			// What tokens could we have accepted at this point in the
			// parse?
			//
			count   = 0;
			errBits = antlr3BitsetLoad		(ex->expectingSet);
			numbits = errBits->numBits		(errBits);
			size    = errBits->size			(errBits);

			if  (0)//size > 0)
			{
				// However many tokens we could have dealt with here, it is usually
				// not useful to print ALL of the set here. I arbitrarily chose 8
				// here, but you should do whatever makes sense for you of course.
				// No token number 0, so look for bit 1 and on.
				//
				for	(bit = 1; bit < numbits && count < 8 && count < size; bit++)
				{
					// TODO: This doesn;t look right - should be asking if the bit is set!!
					//
					if  (tokenNames[bit])
					{
						ANTLR3_FPRINTF(stderr, "%s%s", count > 0 ? ", " : "", tokenNames[bit]); 
						count++;
					}
				}
				ANTLR3_FPRINTF(stderr, "\n");
			}
			else
			{
				//ANTLR3_FPRINTF(stderr, "Actually dude, we didn't seem to be expecting anything here, or at least\n");
				//ANTLR3_FPRINTF(stderr, "I could not work out what I was expecting, like so many of us these days!\n");
			}
		}
		break;

	case	ANTLR3_EARLY_EXIT_EXCEPTION:

		// We entered a loop requiring a number of token sequences
		// but found a token that ended that sequence earlier than
		// we should have done.
		//
		stream << " : missing elements";
		break;

	default:

		// We don't handle any other exceptions here, but you can
		// if you wish. If we get an exception that hits this point
		// then we are just going to report what we know about the
		// token.
		//
		stream << " : syntax not recognized";
		break;
	}

	// Here you have the token that was in error which if this is
	// the standard implementation will tell you the line and offset
	// and also record the address of the start of the line in the
	// input stream. You could therefore print the source line and so on.
	// Generally though, I would expect that your lexer/parser will keep
	// its own map of lines and source pointers or whatever as there
	// are a lot of specific things you need to know about the input
	// to do something like that.
	// Here is where you do it though :-).
	//

	OM_THROW_1(CompileError, stream.ToString());
}

OM_END

//EOF
