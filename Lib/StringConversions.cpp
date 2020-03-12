#include "Strings.h"
#include "StringConversions.h"

namespace StringConversions {

static const int conversionBufferCount = 32;
static const int conversionBufferSize = 1024;
static char conversionBuffers[conversionBufferCount][ conversionBufferSize + 1 ];
static int conversionBufferOn = 0;

char *getConversionBuffer() {
	return ( conversionBuffers[ (conversionBufferOn++) & (conversionBufferCount-1) ] );
}

int getConversionBufferSize() {
	return ( conversionBufferSize );
}

/*
	Specialized strcpy, returns end of copied string (doesn't add null)
*/

char *strcpywalk( char *dst, const char *src ) {
	while ( *src ) 
		*dst++ = *src++;
	return ( dst );
}

char *strcatspace( char *dst ) {
	*dst++ = ' ';
	return ( dst );
}

/*
	Converting TO a string
*/

template<> const char *tostring( const bool &value ) {
	return ( value ) ? "true" : "false";
}

template<> const char *tostring( const s32 &value ) {
	char *buffer = getConversionBuffer();
	return _itoa( value, buffer, 10 );
}

template<> const char *tostring( const u32 &value ) {
	char *buffer = getConversionBuffer();
	return _itoa( value, buffer, 10 );
}

template<> const char *tostring( const char &value ) {
	char *buffer = getConversionBuffer();
	return _itoa( value, buffer, 10 );
}

template<> const char *tostring( const s8 &value ) {
	char *buffer = getConversionBuffer();
	return _itoa( value, buffer, 10 );
}

template<> const char *tostring( const u8 &value ) {
	char *buffer = getConversionBuffer();
	return _itoa( value, buffer, 10 );
}

template<> const char *tostring( const unsigned long &value ) {
	char *buffer = getConversionBuffer();
	return _itoa( value, buffer, 10 );
}

template<> const char *tostring( const f64 &value, char *buffer, int buffersize ) {
	int size = Snprintf( buffer, buffersize, "%f", value );
	buffer[ size ] = NULL;
	char *tail = ( buffer + size - 1 );
	while ( tail > buffer ) {
		if ( *tail != '0' )
			break;
		tail--;
	}
	if ( *tail == '.' )
		tail[0] = '\x0';
	else
		tail[1] = '\x0';
	return ( buffer );
}

template<> const char *tostring( const f32 &value, char *buffer, int buffersize ) {
	return ( tostring<double>( value, buffer, buffersize ) );
}

template<> const char *tostring( const f32 &value ) {
	return ( tostring<double>( value, getConversionBuffer(), conversionBufferSize ) );
}

template<> const char *tostring( const f64 &value ) {
	return ( tostring( value, getConversionBuffer(), conversionBufferSize ) );
}

template<> const char *tostring( const Vector3f &value ) {
	char format[ 128 ];
	char *buffer = getConversionBuffer(), *out = buffer;
	out = strcpywalk( out, tostring( value.x, format, 127 ) );
	out = strcatspace( out );
	out = strcpywalk( out, tostring( value.y, format, 127 ) );
	out = strcatspace( out );
	out = strcpywalk( out, tostring( value.z, format, 127 ) );
	*out = NULL;
	return ( buffer );
}

template<> const char *tostring( const Vector2f &value ) {
	char format[ 128 ];
	char *buffer = getConversionBuffer(), *out = buffer;
	out = strcpywalk( out, tostring( value.x, format, 127 ) );
	out = strcatspace( out );
	out = strcpywalk( out, tostring( value.y, format, 127 ) );
	*out = NULL;
	return ( buffer );
}

template<> const char *tostring( const Vector2i &value ) {
	char *buffer = getConversionBuffer(), *out = buffer;
	out = strcpywalk( out, tostring( value.x ) );
	out = strcatspace( out );
	out = strcpywalk( out, tostring( value.y ) );
	*out = NULL;
	return ( buffer );
}


/*
	Converting FROM a string
*/

template<> void fromstring( const char *string, f32 &value ) {	
	value = (f32)atof( string ); 
}

template<> void fromstring( const char *string, f64 &value ) {	
	value = (f64)atof( string ); 
}

template<> void fromstring( const char *string, s32 &value ) {	
	value = atoi( string ); 
}

template<> void fromstring( const char *string, Vector3f &value ) {
	sscanf( string, "%f %f %f", &value.x, &value.y, &value.z );
}

template<> void fromstring( const char *string, Vector2f &value ) {
	sscanf( string, "%f %f", &value.x, &value.y );
}

template<> void fromstring( const char *string, Vector2i &point ) {
   sscanf( string, "%d %d", &point.x, &point.y );
}

template<> bool fromstring( const char *string ) {
	return (!_stricmp(string, "true") || atof(string));
}

template<> f32 fromstring( const char *string ) {	
	return (f32)atof( string ); 
}

template<> f64 fromstring( const char *string ) {	
	return atof( string ); 
}

template<> s32 fromstring( const char *string ) {	
	return atoi( string );
}

template<> u32 fromstring( const char *string ) {	
	return atoi( string );
}

template<> Vector3f fromstring( const char *string ) {	
	Vector3f v;
	fromstring( string, v );
	return ( v );
}

template<> Vector2f fromstring( const char *string ) {	
	Vector2f v;
	fromstring( string, v );
	return ( v );
}


/*
	Round an ascii string
*/

// breaks on scientific retardation format!
const char *roundascii( const char *value, int digits ) {
	const char *in = value;
	digits = ( digits > 32 ) ? 32 : digits;
	char *buffer = getConversionBuffer(), *out = buffer;
		
	while ( *in && ( *in != '.' ) )
		*out++ = *in++;
	
	if ( digits <= 0 ) {
		*out = NULL;
		return ( buffer );
	}

	if ( *in == '.' ) {
		const char *end = in + digits;
		while ( *in && ( in <= end ) )
			*out++ = *in++;

		while ( in++ <= end )
			*out++ = '0';
	} else {
		*out++ = '.';
		while ( digits-- )
			*out++ = '0';
	}

	*out = NULL;
	return ( buffer );
}

/*
	A short lived formatted string which won't require any allocating
*/

const char *format( const char *fmt, ... ) {
	char *buffer = getConversionBuffer();

	va_list args;
	va_start( args, fmt );
	Snprintf( buffer, conversionBufferSize, fmt, args );
	va_end( args );

	return( buffer );
}

const char *format( int buffersize, char *buffer, const char *fmt, ... ) {
	va_list args;
	va_start( args, fmt );
	Snprintf( buffer, buffersize, fmt, args );
	va_end( args );

	return( buffer );
}

const char *format( int buffersize, char *buffer, va_list &args, const char *fmt ) {
	Snprintf( buffer, buffersize, fmt, args );
	return( buffer );
}

char *escapestring( const char *src, u32 dstlen, char *dst ) {
	char *out = dst, *end = out + dstlen - 1;
	for ( ; *src && out < end; src++ ) {
		u8 c = *src;

		if ( c == '\"' || c == '\\' || c == '\r' || c == '\n' || c == '\t' ) {
			if ( out + 2 >= end )
				break;
			 *out++ = '\\';
			 switch ( c ) {
				case '\"': *out++ = '"'; break;
				case '\\': *out++ = '\\'; break;
				case '\r': *out++ = 'r'; break;
				case '\n': *out++ = 'n'; break;
				case '\t': *out++ = 't'; break;
			 }
		} else if ( ( c < 32 ) || ( c > 127 ) ) {
			if ( out + 4 >= end )
				break;
			*out++ = '\\';
			*out++ = 'x';
			u8 dig1 = c >> 4, dig2 = c & 0xf;
			dig1 += ( dig1 < 10 ) ? '0' : 'A' - 10;
			dig2 += ( dig2 < 10 ) ? '0' : 'A' - 10;
			*out++ = dig1;
			*out++ = dig2;
		} else {
			*out++ = c;
		}
   }
   *out = NULL;
   return dst;
}

char *escapestring( const char *src ) {
	char *buffer = getConversionBuffer();
	return escapestring( src, conversionBufferSize, buffer );
}


}; // namespace StringConversions
