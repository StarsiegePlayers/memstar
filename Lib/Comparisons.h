#ifndef __COMPARISONS_H__
#define __COMPARISONS_H__

template < class type >
class Comparison {
public:
	bool operator()( const type &x, const type &y ) const {
		return ( false );
	}

};

template < class type >
class Less : public Comparison<type> {
public:
	bool operator()( const type &x, const type &y ) const {
		return ( x < y );
	}
};

template < class type >
class Greater : public Comparison<type> {
public:
	bool operator()( const type &x, const type &y ) const {
		return ( x > y );
	}
};

template < class type >
class Equals : public Comparison<type> {
public:
	bool operator()( const type &x, const type &y ) const {
		return ( x == y );
	}
};

#endif // __COMPARISONS_H__
