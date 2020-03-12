#ifndef __BASETYPES_H__
#define __BASETYPES_H__

#include <math.h>

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
typedef double f64;

#define ISPOWOF2(n)  (!(n & (n - 1)))

#define E        2.71828182845904523536f
#define PI       3.14159265358979323846f
#define PI2      ( PI * 2 )

struct Vector2i {
	Vector2i( ) { }
	Vector2i( s32 x, s32 y ) : x(x), y(y) { }

	Vector2i operator+ ( const Vector2i &b ) const { return ( Vector2i( x + b.x, y + b.y ) ); }
	Vector2i operator- ( const Vector2i &b ) const { return ( Vector2i( x - b.x, y - b.y ) ); }

	s32 x, y;
};


struct Vector2f {
	Vector2f() {}
	Vector2f(f32 x, f32 y) : x(x), y(y) {}

	Vector2f Rotate(f32 radians) const { 
		return ( Rotate( sinf( radians ), cosf( radians ) ) );
	}

	Vector2f Rotate(f32 s, f32 c) const { 
		return Vector2f( x * c - y * s, x * s + y * c );
	}

	Vector2f operator- (const Vector2f &b) const {
		return Vector2f(x - b.x, y - b.y);
	}

	static void Rotate(Vector2f *a, s32 count, f32 radians) {
		f32 c = cosf( radians ), s = sinf( radians );
		for ( ; count > 0; --count, ++a )
			*a = a->Rotate( s, c );
	}

	f32 x, y;
};

struct Vector3f {
	Vector3f() {}
	Vector3f(f32 x, f32 y, f32 z) : x(x), y(y), z(z) {}
	f32 x, y, z;
};

struct RGB {
	u8 r, g, b;
};


class RGBf {
	f32 r, g, b;
};

struct RGBAf {
	RGBAf( ) { }
	RGBAf( f32 r, f32 g, f32 b, f32 a ) : r(r), g(g), b(b), a(a) {}
	RGBAf( u8 r, u8 g, u8 b, u8 a ) : r(r), g(g), b(b), a(a) {}
	
	f32 r, g, b, a;
};

struct RGBA {
	RGBA( ) { }
	RGBA(u8 r, u8 g, u8 b, u8 a) : r(r), g(g), b(b), a(a) {}
	RGB torgb() const { RGB color; color.r = r; color.g = g; color.b = b; return ( color ); }
	void operator= ( const RGBA &q ) { memcpy( this, &q, sizeof( *this ) );	}
	void operator+= ( const RGBA &q ) { r=min((r+q.r),255); g=min((g+q.g),255); b=min((b+q.b),255); a=min((a+q.a),255); }
	
	u8 r, g, b, a;
};

class BGRA {
public:
	u8 b, g, r, a;
};

template <typename T>
T Min(T a, T b) {
	return (a < b) ? a : b;
}

template <typename T>
T Max(T a, T b) {
	return (a > b) ? a : b;
}





#endif // __BASETYPES_H__