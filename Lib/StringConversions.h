#ifndef __STRINGCONVERSIONS_H__
#define __STRINGCONVERSIONS_H__

namespace StringConversions {

char *getConversionBuffer();
int getConversionBufferSize();

template< class type > const char *tostring( const type &value );
template< class type > const char *tostring( const type &value1, const type &value2 );
template< class type > const char *tostring( const type &value, char *buffer, int buffersize );

template< class type > type fromstring( const char *string );
template< class type > void fromstring( const char *string, type &value1 );
template< class type > void fromstring( const char *string, type &value1, type &value2 );

const char *roundascii( const char *value, int digits );

const char *format( const char *fmt, ... );
const char *format( int buffersize, char *buffer, const char *fmt, ... );
const char *format( int buffersize, char *buffer, va_list &args, const char *fmt );

char *escapestring( const char *src, u32 dstlen, char *dst );
char *escapestring( const char *src );

}; // namespace StringConversions

using namespace StringConversions;

#endif // __STRINGCONVERSIONS_H__