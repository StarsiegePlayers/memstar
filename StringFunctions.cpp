#include "Console.h"
#include "Strings.h"

static const u32 stringBufferSize = ( 65535 );
static char stringBuffer[ stringBufferSize + 1 ];

const char *substrhelper( const char *str, int start, int len, int srclen ) {
	if ( ( start < 0 ) || ( len <= 0 ) || ( start >= srclen ) )
		return "";

	if ( start + len > srclen )
		len = ( srclen - start );
	if ( len >= stringBufferSize )
		len = ( stringBufferSize - 1 );

	memcpy( stringBuffer, str + start, len );
	stringBuffer[ len ] = '\x0';
	return ( stringBuffer );
}

const char *padhelper( const char *str, int padlen, char padchar, char padjust ) {
	int srclen = ( strlen( str ) );
	int needs = ( padlen - srclen );
	if ( needs <= 0 )
		return ( str );

	int left = 0, right = 0;
	switch ( padjust ) {
		case 'c': left = ( ( needs + 1 ) / 2 ); right = needs - left; break;
		case 'l': left = needs; break;
		case 'r': right = needs; break;
	}
	
	char *out = stringBuffer;
	if ( left ) {
		memset( out, padchar, left );
		out += left;
	}
	memcpy( out, str, srclen );
	out += srclen;
	if ( right ) {
		memset( out, padchar, right );
		out += right;
	}
	*out = '\x0';
	return ( stringBuffer );
}

BuiltInFunction( "String::findSubStr", stringFindSubStr ) {
	if (argc != 2) {
		Console::echo( "wrong argcount to %s( str, find );", self );
		return "-1";
	}

	const char *start = stristr( argv[0], argv[1] );   
	if ( !start )
		return ( "-1" );
	return tostring( start - argv[0] );
}

BuiltInFunction( "getWord", getWord ) {
	if ( argc != 2 ) {
		Console::echo( "wrong argcount to %s(String,nWord)", self );
		return "-1";
	}

	s32 word = atoi(argv[1]);
	if (word < 0)
		return "-1";

	const char *in = argv[0];
	for ( int words = 0; *in; ++words ) {
		while ( *in == ' ' )
			in++;
		
		if ( !*in ) {
			return "-1";
		} else if ( words == word ) {
			char *out = stringBuffer, *end = stringBuffer + stringBufferSize;
			while ( *in != ' ' && *in && ( out < end ) )
				*out++ = *in++;
			*out = NULL;
			return ( stringBuffer );
		} else {
			while ( *in && *in != ' ' )
				in++;
		}
	}

	return "-1";
}


BuiltInFunction( "String::getSubStr", stringGetSubStr ) {
	if (argc != 3) {
		Console::echo( "wrong argcount to %s( str, start, length );", self );
		return "";
	}

	const char *str = ( argv[0] );
	int start = fromstring<int>( argv[1] ), len = fromstring<int>( argv[2] ), srclen = ( strlen( str ) );
	return ( substrhelper( str, start, len, srclen ) );
}

BuiltInFunction( "String::Compare", stringCompare ) {
	if (argc != 2) {
		Console::echo( "wrong argcount to %s( str1, str2 );", self );
		return "";
	}

	return tostring( strcmp(argv[0], argv[1]) );
}

BuiltInFunction( "String::NCompare", stringNCompare ) {
	if (argc != 3) {
		Console::echo( "wrong argcount to %s( str1, str2, length );", self );
		return "";
	}

	return tostring( strncmp(argv[0], argv[1], fromstring<int>(argv[2])) );
}

BuiltInFunction( "String::ICompare", stringICompare ) {
	if (argc != 2) {
		Console::echo( "wrong argcount to %s( str1, str2 );", self );
		return "";
	}

	return tostring( _stricmp(argv[0], argv[1]) );
}

BuiltInFunction( "String::empty", stringEmpty ) {
	if (argc != 1) {
		Console::echo( "wrong argcount to %s( str );", self );
		return "true";
	}
	
	const char *tempPtr = argv[0];
	while (*tempPtr == ' ')
		tempPtr++;

	return ( !*tempPtr ) ? "true" : "false";
}

// just replaces all spaces with the '_' char
BuiltInFunction( "String::convertSpaces", stringConvertSpaces ) {
	if(argc != 1 ) {
		Console::echo( "wrong argcount to %s( str );", self );
		return ( "" );
	}
   
	const char *src = ( argv[0] );
	char *dest = stringBuffer;
	while(*src)
		*dest++ = ( *src == ' ' ) ? '_' : *src++;
	*dest = '\x0';
	return ( stringBuffer );
}

// Zear extensions

BuiltInFunction( "String::left", stringLeft ) {
	if ( argc != 2 ) {
		Console::echo( "wrong argcount to %s( str, length );", self );
		return "";
	}

	const char *str = ( argv[0] );
	int start = 0, len = fromstring<int>( argv[1] ), srclen = ( strlen( str ) );
	return ( substrhelper( str, start, len, srclen ) );
}

BuiltInFunction( "String::right", stringRight ) {
	if ( argc != 2 ) {
		Console::echo( "wrong argcount to %s( str, length );", self );
		return "";
	}

	const char *str = ( argv[0] );
	int len = fromstring<int>( argv[1] ), srclen = ( strlen( str ) ), start = ( srclen - len );
	if ( start < 0 )
		start = 0;

	return ( substrhelper( str, start, len, srclen ) );
}

BuiltInFunction( "String::starts", stringStarts ) {
	if ( argc != 2 ) {
		Console::echo( "wrong argcount to %s( str, start );", self );
		return "";
	}

	const char *str = ( argv[0] ), *starts = ( argv[1] );
	return ( stristr( str, starts ) == str ) ? "true" : "false";
}

BuiltInFunction( "String::ends", stringEnds ) {
	if ( argc != 2 ) {
		Console::echo( "wrong argcount to %s( str, end );", self );
		return "";
	}


	const char *str = ( argv[0] ), *ends = ( argv[1] );
	int srclen = strlen( str ), endlen = strlen( ends );
	return ( stristr( str, ends ) == ( str + ( srclen - endlen ) ) ) ? "true" : "false";
}

BuiltInFunction( "String::insert", stringInsert ) {
	if ( argc != 3 ) {
		Console::echo( "wrong argcount to %s( str, insert, index );", self );
		return "";
	}

	const char *str = ( argv[0] ), *insert = ( argv[1] );
	int index = ( fromstring<int>( argv[2] ) );
	int srclen = strlen( str ), ilen = strlen( insert);

	if ( index > srclen )
		index = srclen;

	// copy leading bit
	if ( index > 0 )
		memcpy( stringBuffer, str, index );
	
	// copy inserted bit
	memcpy( stringBuffer + index, insert, ilen );

	// copy tail bit
	if ( index < srclen )
		memcpy( stringBuffer + index + ilen, str + index, srclen - index );

	stringBuffer[ ilen + srclen ] = NULL;
    return ( stringBuffer );
}

BuiltInFunction( "String::len", stringLen ) {
	if ( argc != 1 ) {
		Console::echo( "wrong argcount to %s( str );", self );
		return "";
	}

	return ( tostring( strlen( argv[0] ) ) );
}

BuiltInFunction( "String::length", stringLength ) {
	if ( argc != 1 ) {
		Console::echo( "wrong argcount to %s( str );", self );
		return "";
	}

	return ( tostring( strlen( argv[0] ) ) );
}

BuiltInFunction( "chr", stringChr ) {
	if ( argc != 1 ) {
		Console::echo( "wrong argcount to %s( ordinal );", self );
		return "";
	}

	stringBuffer[0] = (unsigned char)(fromstring<int>( argv[0] ));
	stringBuffer[1] = '\x0';
	return ( stringBuffer );
}

BuiltInFunction( "ord", stringOrd ) {
	if ( argc != 1 ) {
		Console::echo( "wrong argcount to %s( char );", self );
		return "";
	}

	return ( tostring( argv[0][0] ) );
}

BuiltInFunction( "String::rpad", stringRPad ) {
	if ( ( argc < 2 ) || ( argc > 3 ) ) {
		Console::echo( "wrong argcount to %s( str, width, <padchar> );", self );
		return "";
	}

	const char *str = argv[0];
	int width = fromstring<int>( argv[1] );
	char pad = ( argc == 3 ) ? argv[2][0] : ' ';

	return ( padhelper( str, width, pad, 'r' ) );
}

BuiltInFunction( "String::lpad", stringLPad ) {
	if ( ( argc < 2 ) || ( argc > 3 ) ) {
		Console::echo( "wrong argcount to %s( str, width, <padchar> );", self );
		return "";
	}

	const char *str = argv[0];
	int width = fromstring<int>( argv[1] );
	char pad = ( argc == 3 ) ? argv[2][0] : ' ';

	return ( padhelper( str, width, pad, 'l' ) );
}

BuiltInFunction( "String::cpad", stringCPad ) {
	if ( ( argc < 2 ) || ( argc > 3 ) ) {
		Console::echo( "wrong argcount to %s( str, width, <padchar> );", self );
		return "";
	}

	const char *str = argv[0];
	int width = fromstring<int>( argv[1] );
	char pad = ( argc == 3 ) ? argv[2][0] : ' ';

	return ( padhelper( str, width, pad, 'c' ) );
}

BuiltInFunction( "String::trim", stringTrim ) {
	if ( argc != 1 ) {
		Console::echo( "wrong argcount to %s( str );", self );
		return "";
	}

	const char *str = argv[0], *front = str, *back = str + strlen( str ) - 1;
	while ( *front == ' ' )
		++front;
	while ( back >= str && *back == ' ' )
		--back;

	if ( front > back )
		return ( "" );

	memcpy( stringBuffer, front, back - front + 1 );
	stringBuffer[ back - front + 1 ] = '\x0';
	return ( stringBuffer );
}


BuiltInFunction( "String::toupper", stringToupper ) {
	if ( argc != 1 ) {
		Console::echo( "wrong argcount to %s( str );", self );
		return "";
	}

	const char *str = argv[0];
	char *out = stringBuffer;
	while ( *str )
		*out++ = toupper( *str++ );
	*out = NULL;
	return ( stringBuffer );
}

BuiltInFunction( "String::tolower", stringTolower ) {
	if ( argc != 1 ) {
		Console::echo( "wrong argcount to %s( str );", self );
		return "";
	}

	const char *str = argv[0];
	char *out = stringBuffer;
	while ( *str )
		*out++ = tolower( *str++ );
	*out = NULL;
	return ( stringBuffer );
}

BuiltInFunction( "String::char", stringChar ) {
	if ( argc != 2 ) {
		Console::echo( "wrong argcount to %s( str, charIndex );", self );
		return "";
	}

	const char *str = argv[0];
	char *out = stringBuffer;
	int idx = fromstring<int>( argv[1] );
	int len = strlen( str );
	if ( idx < 0 || idx >= len )
		return ( "" );
	*out++ = str[ idx ];
	*out = NULL;
	return ( stringBuffer );
}


BuiltInFunction( "String::Replace", stringReplace ) {
	if ( argc != 3 ) {
		Console::echo( "wrong argcount to %s( str, search, replace );", self );
		return "";
	}

	const char *str = argv[0], *search = argv[1], *replace = argv[2], *in = str, *found;
	char *out = stringBuffer, *end = out + stringBufferSize;
	size_t searchlen = strlen( search ), replen = strlen( replace );
	
	
	while (out < end) {
		found = stristr( in, search );
		if ( !found )
			break;
		
		size_t tocopy = Min(found - in, end - out);
		memcpy(out, in, tocopy);
		out += tocopy;
		tocopy = Min<size_t>(replen, end - out);
		memcpy(out, replace, tocopy);
		out += tocopy;
		in = found + searchlen;
	}

	while ((out < end) && *in)
		*out++ = *in++;
	*out = '\x0';
	return ( stringBuffer );
}

BuiltInFunction( "sprintf", sprintf ) {
	if ( ( argc < 1 ) || ( argc > 11 ) ) {
		Console::echo( "wrong argcount to %s(string, [Arg1 ... Arg9] )", self );
		return ( "false" );
	}

	const char *in = argv[0];
	char *out = stringBuffer, *end = stringBuffer + stringBufferSize;

	while ( *in && ( out < end ) ) {
		if ( *in == '%' ) {
			s32 idx = ( in[1] - 48 );

			// make sure the index refers to an actual argument
			if ( ( idx >= 1 && idx <= 9 ) && ( idx <= ( argc - 1 ) ) ) {
				const char *str = argv[ idx ];
				while ( *str && ( out < end ) )
					*out++ = *str++;
			} else if ( in[1] == '%' ) {
				*out++ = '%';
			}
			
			// don't inc if it's the end of the string
			in += ( in[1] ) ? 2 : 1;
		}

		while ( *in && ( *in != '%' ) && ( out < end ) )
			*out++ = *in++;
	}

	*out = NULL;
	return ( stringBuffer );
}


BuiltInFunction( "strcat", strcat ) {
	char *out = stringBuffer, *end = stringBuffer + stringBufferSize;
	for ( s32 i = 0; i < argc; i++ ) {
		for ( const char *in = argv[i]; *in && ( out < end ); )
			*out++ = *in++;
	}
	*out = NULL;
	return stringBuffer;
}

BuiltInFunction( "escapeString", escapeString ) {
	if ( argc != 1 ) {
		Console::echo( "wrong argcount to %s( String )", self );
		return ( "" );
	}

	return escapestring( argv[0], stringBufferSize, stringBuffer );
}

BuiltInFunction( "String::Escape", stringEscape ) {
	if ( argc != 1 ) {
		Console::echo( "wrong argcount to %s( String )", self );
		return ( "" );
	}

	return escapestring( argv[0], stringBufferSize, stringBuffer );
}

BuiltInFunction( "String::Dup", stringDup ) {
	if ( argc != 2 ) {
		Console::echo( "wrong argcount to %s( String, DupCount )", self );
		return ( "" );
	}

	const char *str = argv[0];
	int len = strlen( str ), reps = fromstring<int>( argv[1] );
	char *out = stringBuffer, *end = stringBuffer + stringBufferSize;
	while ( ( reps > 0 ) && ( out + len < end ) ) {
		memcpy( out, str, len );
		out += len;
		--reps;
	}

	*out = NULL;
	return ( stringBuffer );
}

BuiltInFunction( "String::explode", stringExplode ) {
	if ( argc != 3 ) {
		Console::echo( "wrong argcount to %s( String, Seperator, ArrayName )", self );
		return "0";
	}
	
	const char *in = argv[0], *sep = argv[1], *arrayname = argv[2];
	int seplen = strlen( sep ), inlen = strlen( in ), items = 0;

	while ( inlen > 0 ) {
		const char *pos = strstr( in, sep );
		int piecelen = ( pos ) ? pos - in : inlen;
		Console::setVariable( format( "$%s%d", arrayname, items++ ), substrhelper( in, 0, piecelen, inlen ) );
		in += piecelen + seplen;
		inlen -= piecelen + seplen;
	}

	return ( tostring( items ) );
}
