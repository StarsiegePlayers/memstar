#include "Strings.h"

String operator+ ( const String &str, char c )  { String tmp( str ); tmp.Append( c ); return ( tmp ); }
String operator+ ( const String &str, int i )   { String tmp( str ); tmp.Append( "%d", i ); return ( tmp ); }
String operator+ ( const String &str, float f ) { String tmp( str ); tmp.Append( "%8.8f", f ); return ( tmp ); }
String operator+ ( const String &str, char *s ) { String tmp( str ); tmp.Append( strlen( s ) + 1, s ); return ( tmp ); }
String operator+ ( const String &str, const String &str2 ) { String tmp( str ); tmp.Append( str2.Length() + 1, str2.c_str() ); return ( tmp ); }
String operator+ ( const String &str, const String2 &str2 ) { String tmp( str ); tmp.Append( str2.Length() + 1, str2.c_str() ); return ( tmp ); }

String2 operator+ ( const String2 &str, char c )  { String2 tmp( str ); tmp.Append( c ); return ( tmp ); }
String2 operator+ ( const String2 &str, int i )   { String2 tmp( str ); tmp.Append( "%d", i ); return ( tmp ); }
String2 operator+ ( const String2 &str, float f ) { String2 tmp( str ); tmp.Append( "%8.8f", f ); return ( tmp ); }
String2 operator+ ( const String2 &str, char *s ) { String2 tmp( str ); tmp.Append( strlen( s ) + 1, s ); return ( tmp ); }
String2 operator+ ( const String2 &str, const String &str2 ) { String2 tmp( str ); tmp.Append( str2.Length() + 1, str2.c_str() ); return ( tmp ); }
String2 operator+ ( const String2 &str, const String2 &str2 ) {	String2 tmp( str ); tmp.Append( str2.Length() + 1, str2.c_str() ); return ( tmp ); }

static char ctob_table[ 256 ];
static bool ctob_loaded = false;

/* character [0-0A-F] to binary [0..15] */
char ctob( char c ) {
	if ( c >= '0' && c <= '9' ) 
		c -= '0';
	else if ( c >= 'A' && c <= 'F' )
		c -= ( 'A' - 10 );
	else if ( c >= 'a' && c <= 'f' )
		c -= ( 'a' - 10 );
	else
		c = -1;

	return ( c );
}

/* raw bytes to hex ascii */
void htoa(const char *src, char *dst, int src_len, int max_dst) {
	static char *hex_table = "0123456789abcdef";
	int bytes_written = 0;

	while ( ( src_len-- > 0 ) && ( bytes_written + 3 <= max_dst ) ) {
		*dst++ = hex_table[ *(unsigned char*)src >>   4 ];
		*dst++ = hex_table[ *(unsigned char*)src  & 0xf ];
		src++;
		bytes_written += 2;
	}

	if ( bytes_written < max_dst ) 
		*dst = 0;
}

/* ascii hex to raw bytes */
int atoh( const char *src, char *dst, int max_dst ) {
	if (!ctob_loaded) {
		for (u32 i = 0; i < 256; i++) 
			ctob_table[i] = ctob(i);
		ctob_loaded = true;
	}

	int bytes_written = 0;
	char upper, lower;

	if ( !src )
		return ( bytes_written );
	
	while ( *src && *(src + 1) && ( bytes_written < max_dst ) ) {
		upper = ctob_table[*src++];
		lower = ctob_table[*src++];

		if ( upper == -1 || lower == -1 )
			break;

		dst[ bytes_written++ ] = ( upper << 4 ) | ( lower );
	}

	return ( bytes_written );
}

// ascii to hex - host long
bool atohhl(const char *src, unsigned int *dst) {
	unsigned char tmp[ 4 ];
	if (atoh(src, (char *)tmp, 4) != 4)
		return false;
	
	// swap to little endian
	*dst = ( ( tmp[ 3 ] ) | ( tmp[ 2 ] << 8 ) | ( tmp[ 1 ] << 16 ) | ( tmp[ 0 ] << 24 ) );
	return true;
}

const char *stristr(const char *haystack, const char *needle) {
	const char *hend;
	const char *a, *b;

	if ( *needle == 0 )
		return ( haystack );

	hend = haystack + strlen( haystack ) - strlen( needle ) + 1;

	while ( haystack < hend ) {
		if ( toupper( *haystack ) == toupper( *needle ) ) {
			a = haystack;
			b = needle;
	
			for (;;) {
				if ( *b == 0 ) 
					return ( haystack );

				if ( toupper( *a++ ) != toupper( *b++ ) )
					break;
			}
		}

		haystack++;
	}

	return ( 0 );
}
