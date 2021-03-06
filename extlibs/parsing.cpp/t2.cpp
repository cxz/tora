// Example of a grammar for parsing Java sources,
// Adapted from Java equivalent example, by Terence Parr
// Author: Jim Idle - April 2007
// Permission is granted to use this example code in any way you want, so long as
// all the original authors are cited.
//

// set ts=4,sw=4
// Tab size is 4 chars, indent is 4 chars

// Notes: Although all the examples provided are configured to be built
//        by Visual Studio 2005, based on the custom build rules
//        provided in $(ANTLRSRC)/code/antlr/main/runtime/C/vs2005/rulefiles/antlr3.rules
//        there is no reason that this MUST be the case. Provided that you know how
//        to run the antlr tool, then just compile the resulting .c files and this
//	  file together, using say gcc or whatever: gcc *.c -I. -o XXX
//	  The C code is generic and will compile and run on all platforms (please
//        report any warnings or errors to the antlr-interest newsgroup (see www.antlr.org)
//        so that they may be corrected for any platofrm that I have not specifically tested.
//
//	  The project settings such as addinotal library paths and include paths have been set
//        relative to the place where this source code sits on the ANTLR perforce system. You
//        may well need to change the settings to locate the includes and the lib files. UNIX
//        people need -L path/to/antlr/libs -lantlr3c (release mode) or -lantlr3cd (debug)
//
//        Jim Idle (jimi cut-this at idle ws)
//

// You may adopt your own practices by all means, but in general it is best
// to create a single include for your project, that will include the ANTLR3 C
// runtime header files, the generated header files (all of which are safe to include
// multiple times) and your own project related header files. Use <> to include and
// -I on the compile line (which vs2005 now handles, where vs2003 did not).
//

#include "utils.hpp"
#include "UserTraits.hpp"
#include "PLSQLLexer.hpp"
#include "PLSQLParser.hpp"
#include "PLSQLParser_PLSQLCommons.hpp"
#include "PLSQLParser_PLSQL_DMLParser.hpp"
#include "PLSQLParser_PLSQLKeys.hpp"
#include "PLSQLParser_SQLPLUSParser.hpp"

#include <sys/types.h>

#include <iostream>
#include <sstream>
#include <fstream>

using namespace Antlr3BackendImpl;
using namespace std;

// The lexer is of course generated by ANTLR, and so the lexer type is not upper case.
// The lexer is supplied with a pANTLR3_INPUT_STREAM from whence it consumes its
// input and generates a token stream as output.
//
static    PLSQLLexer*		    lxr;

// Main entry point for this example
//
int 
main	(int argc, char *argv[])
{
	// Create the input stream based upon the argument supplied to us on the command line
	// for this example, the input will always default to ./input if there is no explicit
	// argument, otherwise we are expecting potentially a whole list of 'em.
	//
	if (argc < 2 || argv[1] == NULL)
	{
		Utils::processDir("./input"); // Note in VS2005 debug, working directory must be configured
	}
	else
	{
		int i;

		for (i = 1; i < argc; i++)
		{
			Utils::processDir(argv[i]);
		}
	}

	printf("finished parsing OK\n");	// Finnish parking is pretty good - I think it is all the snow

	return 0;
}

void parseFile(const char* fName, int fd)
{
	// Now we declare the ANTLR related local variables we need.
	// Note that unless you are convinced you will never need thread safe
	// versions for your project, then you should always create such things
	// as instance variables for each invocation.
	// -------------------

	// The ANTLR3 character input stream, which abstracts the input source such that
	// it is easy to privide inpput from different sources such as files, or 
	// memory strings.
	//
	// For an ASCII/latin-1 memory string use:
	//	    input = antlr3NewAsciiStringInPlaceStream (stringtouse, (ANTLR3_UINT64) length, NULL);
	//
	// For a UCS2 (16 bit) memory string use:
	//	    input = antlr3NewUCS2StringInPlaceStream (stringtouse, (ANTLR3_UINT64) length, NULL);
	//
	// For input from a file, see code below
	//
	// Note that this is essentially a pointer to a structure containing pointers to functions.
	// You can create your own input stream type (copy one of the existing ones) and override any
	// individual function by installing your own pointer after you have created the standard 
	// version.
	//
	PLSQLTraits::InputStreamType*    input;


	// The token stream is produced by the ANTLR3 generated lexer. Again it is a structure based
	// API/Object, which you can customise and override methods of as you wish. a Token stream is
	// supplied to the generated parser, and you can write your own token stream and pass this in
	// if you wish.
	//
	PLSQLTraits::TokenStreamType*	tstream;

	// The C parser is also generated by ANTLR and accepts a token stream as explained
	// above. The token stream can be any source in fact, so long as it implements the 
	// ANTLR3_TOKEN_SOURCE interface. In this case the parser does not return anything
	// but it can of course specify any kind of return type from the rule you invoke
	// when calling it.
	//
	PLSQLParser*			psr;

	// Create the input stream using the supplied file name
	// (Use antlr3AsciiFileStreamNew for UCS2/16bit input).
	//
	///byIvan input	= new PLSQLTraits::InputStreamType(fName, ANTLR_ENC_8BIT);
	string data = Utils::slurp(fd);
	input	= new PLSQLTraits::InputStreamType((const ANTLR_UINT8 *)data.c_str(),
						   ANTLR_ENC_8BIT,
						   data.length(),
						   (ANTLR_UINT8*)fName);

	input->setUcaseLA(true);
    
	// Our input stream is now open and all set to go, so we can create a new instance of our
	// lexer and set the lexer input to our input stream:
	//  (file | memory | ?) --> inputstream -> lexer --> tokenstream --> parser ( --> treeparser )?
	//
	if (lxr == NULL)
	{
		lxr	    = new PLSQLLexer(input);	    // javaLexerNew is generated by ANTLR
	}
	else
	{
		lxr->setCharStream(input);
	}

	// Our lexer is in place, so we can create the token stream from it
	// NB: Nothing happens yet other than the file has been read. We are just 
	// connecting all these things together and they will be invoked when we
	// call the parser rule. ANTLR3_SIZE_HINT can be left at the default usually
	// unless you have a very large token stream/input. Each generated lexer
	// provides a token source interface, which is the second argument to the
	// token stream creator.
	// Note tha even if you implement your own token structure, it will always
	// contain a standard common token within it and this is the pointer that
	// you pass around to everything else. A common token as a pointer within
	// it that should point to your own outer token structure.
	//
	tstream = new PLSQLTraits::TokenStreamType(ANTLR_SIZE_HINT, lxr->get_tokSource());

	// Finally, now that we have our lexer constructed, we can create the parser
	//
	psr	    = new PLSQLParser(tstream);  // javaParserNew is generated by ANTLR3

	// We are all ready to go. Though that looked complicated at first glance,
	// I am sure, you will see that in fact most of the code above is dealing
	// with errors and there isn't really that much to do (isn't this always the
	// case in C? ;-).
	//
	// So, we now invoke the parser. All elements of ANTLR3 generated C components
	// as well as the ANTLR C runtime library itself are pseudo objects. This means
	// that they are represented as pointers to structures, which contain any
	// instance data they need, and a set of pointers to other interfaces or
	// 'methods'. Note that in general, these few pointers we have created here are
	// the only things you will ever explicitly free() as everything else is created
	// via factories, that allocate memory efficiently and free() everything they use
	// automatically when you close the parser/lexer/etc.
	//
	// Note that this means only that the methods are always called via the object
	// pointer and the first argument to any method, is a pointer to the structure itself.
	// It also has the side advantage, if you are using an IDE such as VS2005 that can do it
	// that when you type ->, you will see a list of tall the methods the object supports.
	//
	//putc('L', stdout); fflush(stdout);
	//{
	//	ANTLR3_INT32 T;

	//	T = 0;
	//	while	(T != ANTLR3_TOKEN_EOF)
	//	{
	//		T = tstream->tstream->istream->_LA(tstream->tstream->istream, 1);
	//		tstream->tstream->istream->consume(tstream->tstream->istream);
	//		printf("%d %s\n", T,  (psr->pParser->rec->state->tokenNames)[T]);
	//	}
	//}

	tstream->_LT(1);	// Don't do this mormally, just causes lexer to run for timings here
	//putc('P', stdout); fflush(stdout);
	psr->seq_of_statements();
	//putc('F', stdout); fflush(stdout);
	//putc('*', stdout); fflush(stdout);


	delete psr; 
	delete tstream; 
	delete lxr; lxr = NULL;
	delete input; 
}
