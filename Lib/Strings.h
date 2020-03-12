#ifndef __STRINGS_H__
#define __STRINGS_H__

#include "Memstar.h"

#define Snprintf _snprintf
#define SnprintfVarArg _vsnprintf

#define __STRING2_SIZE__ ( 64 )

/*
	String is default String class

	String2 has a __STRING2_SIZE__ byte buffer on the stack so you can
	use it for small items without malloc'ing. For things like finding keys
	in a hash table, use String2() explicitly to construct your char* values to
	avoid memory allocation
*/

inline size_t smallestpow2( size_t amt, size_t smallest ) {
	if ( amt < smallest )
		return ( smallest );
	
	size_t val = 1;
	while ( val < amt )
		val <<= 1;
	return ( val );
}

class String {
public:
	String( ) : mStr(NULL), mLen(0), mAlloc(0), mNoFree(true) { }	
	String( const char *str ) : mStr(NULL), mLen(0), mAlloc(0), mNoFree(true) { Assign( strlen( str ), str ); }
	String( const char *str, size_t size )  : mStr(NULL), mLen(0), mAlloc(0), mNoFree(true) { Assign( size, str ); }	
	String( const String &str )  : mStr(NULL), mLen(0), mAlloc(0), mNoFree(true) { Assign( str.Strlen(), str.mStr ); }		
	~String( ) { if ( !mNoFree ) free( mStr ); mStr = ( NULL );	}
	
	String &Append( const char *fmt, ... ) {
		// returns # of bytes not counting the null
		va_list args;
		va_start( args, fmt );		
		size_t req_bytes = ( SnprintfVarArg( NULL, 0, fmt, args ) + 1 );
		va_end( args ); 

		char stack[ 1024 ], *buffer = stack;
		if ( req_bytes > 1024 )
			buffer = new char[ req_bytes ];

		if ( buffer ) {
			va_start( args, fmt );
			SnprintfVarArg( buffer, req_bytes, fmt, args );
			va_end( args );

			Append( req_bytes - 1, buffer );
			if ( buffer != stack )
				delete[] buffer;
		}

		return ( *this );
	}

	String &Append( char ch ) {
		if ( Resize( mLen + 1 ) ) {
			mStr[ mLen++ ] = ( ch );
			AppendNull( );
		}

		return ( *this );
	}

	String &Append( size_t len, const char *str ) {
		bool aliased = ( IsAliased( str ) );
		size_t offs = aliased ? ( str - mStr ) : 0;

		if ( Resize( mLen + len ) ) {
			if ( aliased )
				str = ( mStr + offs );
			memcpy( mStr + mLen, str, len );
			mLen += ( len );
			AppendNull( );
		}

		return ( *this );
	}

	String &Assign( const char *fmt, ... ) {	
		// returns # of bytes not counting the null
		va_list args;
		va_start( args, fmt );		
		size_t req_bytes = ( SnprintfVarArg( NULL, 0, fmt, args ) + 1 );
		va_end( args ); 

		char stack[ 1024 ], *buffer = stack;
		if ( req_bytes > 1024 )
			buffer = new char[ req_bytes ];

		if ( buffer ) {
			va_start( args, fmt );
			SnprintfVarArg( buffer, req_bytes, fmt, args );
			va_end( args );

			Assign( req_bytes - 1, buffer );
			if ( buffer != stack )
				delete[] buffer;
		}

		return ( *this );
	}

	String &Assign( char c ) {
		if ( Resize( mLen + 1 ) ) {
			mStr[ 0 ] = c;
			mLen = ( 1 );
			AppendNull( );
		}

		return ( *this );
	}

	String &Assign( size_t len, const char *str ) {
		// assignment aliasing isn't a problem because resize never shrinks
		if ( Resize( len ) ) {
			memmove( mStr, str, len );
			mLen = ( len );
			AppendNull( );
		}

		return ( *this );
	}

	const char *c_str( ) const { 
		return ( mStr ); 
	}

	String &Clear( ) {
		mLen = ( 0 );
		AppendNull( );
		return ( *this );
	}	

	String &Compact( ) {
		size_t tmp = ( mAlloc );
		mAlloc = ( 0 );
		if ( !Resize( mLen ) )
			mAlloc = ( tmp );
		return ( *this );
	}

	void Free( ) {
		if ( !mNoFree ) 
			free( mStr ); 
		mStr = ( NULL );
		mLen = ( 0 );
		mAlloc = ( 0 );
	}

	bool Equals( const char *b ) const { return ( strcmp( mStr, b ) == 0 ); }
	bool Equals( const String &b ) const { return ( Equals( b.c_str() ) ); }
	bool IEquals( const char *b ) const { return ( _stricmp( mStr, b ) == 0 ); }
	bool IEquals( const String &b ) const { return ( IEquals( b.c_str() ) ); }

	size_t Length( ) const {
		return ( mStr ) ? ( Strlen( ) + 1 ) : 0;
	}

	String Left( size_t len ) const { 
		return ( Mid( 0, len ) ); 
	}

	String Right( size_t len ) const { 
		size_t end = ( Strlen( ) ); 
		return ( Mid( end - len, end ) ); 
	}

	String Mid( size_t start, size_t end ) const {
		size_t last = ( Strlen( ) );
		start = ( start < 0 ) ? 0 : ( start >= last ) ? last : start;
		end = ( end < 0 ) ? 0 : ( end >= last ) ? last : end;
		size_t len = ( end - start );

		return ( String( mStr + start, len ) );
	}

	size_t Strlen( ) const {
		return ( mLen );
	}

	// []
	char operator[]( size_t index ) const { if ( ( index >= 0 ) && ( index < Strlen( ) ) ) return ( mStr[ index ] ); return ( 0 ); }

	// =
	void operator=  ( char c )  { Assign( c ); }
	void operator=  ( int i )   { Assign( "%d", i ); }
	void operator=  ( float f ) { Assign( "%f", f ); }
	void operator=  ( const char *str ) { Assign( strlen( str ), str ); }
	void operator=  ( const String &str ) { Assign( str.Strlen(), str.mStr ); }	

	// +=
	void operator+= ( char c ) { Append( c ); }
	void operator+= ( const char *str ) { Append( strlen( str ), str ); }
	void operator+= ( const String &str ) { Append( str.Strlen(), str.mStr ); }
	
	// ==
	bool operator== ( const char *str ) const { return ( Equals( str ) ); }	
	bool operator== ( const String &str ) const { return ( Equals( str ) ); }

protected:
	// for String2
	String( size_t stack_alloc, char *stack_ptr ) : mStr(stack_ptr), mLen(0), mAlloc(stack_alloc), mNoFree(true) { }

	void AppendNull( ) { 
		if ( mStr ) 
			mStr[ mLen ] = '\x0'; 
	}

	bool IsAliased( const char *str ) {
		return ( ( str >= mStr ) && str <= ( mStr + mLen ) );
	}

	virtual bool Resize( size_t new_size )  {
		// add the implicit null
		++new_size;
		if ( mAlloc >= new_size )
			return ( true ); 

		new_size = ( smallestpow2( new_size, 8 ) );
		char *tmp = (char *)realloc( mStr, new_size );
		
		if ( tmp ) {
			mStr = ( tmp );
			mAlloc = ( new_size );
			mNoFree = ( false );
		}

		return ( tmp != NULL );
	}


protected:
	char *mStr;
	size_t mLen, mAlloc;
	bool mNoFree;
};

class String2 : public String {
public:
	String2( ) : String( __STRING2_SIZE__, mStack ) { AppendNull( ); }	
	String2( const char *str ) : String( __STRING2_SIZE__, mStack ) { Assign( strlen( str ), str ); }
	String2( const char *str, size_t size ) : String( __STRING2_SIZE__, mStack ) { Assign( size, str ); }
	String2( const String &str ) : String( __STRING2_SIZE__, mStack ) { Assign( str.Strlen(), str.c_str() ); }

	void Free( ) {
		if ( !mNoFree ) 
			free( mStr ); 
		mStr = ( mStack );
		mNoFree = ( true );
	}

	/*
		very evil: if the string object physically moves, call re-align to 
		set mStr back to mStack if needed, otherwise mStr could point to bad
		memory.

		ex: List<>'s of mString2's which may get realloc'd
	*/
	String2 &Realign( ) {
		if ( mNoFree )
			mStr = ( mStack );
		return ( *this );
	}

	void operator= ( char c ) { Assign( c ); }
	void operator= ( int i ) { Assign( "%d", i ); }
	void operator= ( float f ) { Assign( "%f", f ); }
	void operator= ( const char *str ) { Assign( strlen( str ), str ); }
	void operator= ( const String &str ) { Assign( str.Strlen(), str.c_str() ); }
	void operator= ( const String2 &str ) { Assign( str.Strlen(), str.c_str() ); }	

protected:
	virtual bool Resize( size_t new_size ) {
		// add the implicit null
		++new_size;
		if ( mAlloc >= new_size )
			return ( true ); 

		// swap to heap based memory
		new_size = ( smallestpow2( new_size, 8 ) );	
		char *tmp = (char *)realloc( ( mStr == mStack ) ? NULL : mStr, new_size );
		
		if ( tmp ) {
			if ( mStr == mStack ) {
				memcpy( tmp, mStack, Strlen() );
				tmp[ mLen ] = '\x0';
			}
			mNoFree = ( false );
			mStr = ( tmp );
			mAlloc = ( new_size );
		}

		return ( tmp != NULL );
	}

	// DooM III trick with stack based buffer for small strings to avoid memory allocation
	char mStack[ __STRING2_SIZE__ ];
};


/*
	Horribly bloated, but no other way around this. Append() with 
	format strings would be better to use if you didn't mind losing fancy 
	syntax tricks
*/

String operator+ ( const String &str, char c );
String operator+ ( const String &str, int i );
String operator+ ( const String &str, float f );
String operator+ ( const String &str, char *s );
String operator+ ( const String &str, const String &str2 );
String operator+ ( const String &str, const String2 &str2 );

String2 operator+ ( const String2 &str, char c );
String2 operator+ ( const String2 &str, int i );
String2 operator+ ( const String2 &str, float f );
String2 operator+ ( const String2 &str, char *s );
String2 operator+ ( const String2 &str, const String &str2 );
String2 operator+ ( const String2 &str, const String2 &str2 );

void htoa(const char *src, char *dst, int src_len, int max_dst);
int atoh( const char *src, char *dst, int max_dst );
bool atohhl(const char *src, unsigned int *dst);
const char *stristr(const char *haystack, const char *needle);

#endif // __STRING_H__