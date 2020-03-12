#ifndef __SORT_H__
#define __SORT_H__

#include "Comparisons.h"

template < class type >
__inline
void Swap( type *a, type *b ) {
	type temp = *a;
	*a = *b;
	*b = temp;
}


/*
	InsertionSort
*/

template < class type, class Compare >
void InsertionSort_SortReckless( type *lower, type *upper, Compare comp ) {
	for ( type *i = lower; i <= upper; ++i ) {
		type *next = i - 1, tmp = *i;
		
		while ( comp( tmp, *next ) ) {
			*( next + 1 ) = *next;
			next--;
		}

		*( next + 1 ) = tmp;
	}
}

template < class type, class Compare >
void InsertionSort_Sort( type *lower, type *upper, Compare comp ) {
	for ( type *i = lower + 1; i <= upper; ++i ) {
		type *last = i, *next = last - 1, tmp = *i;
		
		while ( last > lower && comp( tmp, *next ) ) {
			*last = *next;
			last = next--;
		}

		*last = tmp;
	}
}

template < class type >
void InsertionSort_Sort( type *lower, type *upper ) {
	InsertionSort_Sort( lower, upper, Less<type>() );
}

/*
	Heap Sort
*/

template < class type, class Compare >
void HeapSort_Float( type *arr, int start, Compare comp ) {
	int child = start, root, remainder;

	while ( child > 0 ) {
		remainder = ( child - 1 ) & 1;
		root = ( ( child - 1 ) - remainder ) / 2;
		if ( comp( arr[ root ], arr[ child ] ) ) {
			Swap( &arr[ root ], &arr[ child ] );
			child = root;
		} else {
			break;
		}
	}
}

template < class type, class Compare >
void HeapSort_Sink( type *arr, int start, int count, Compare comp ) { 
	int root = start, child;

	while ( ( root * 2 ) + 1 < count ) {
		child = ( root * 2 ) + 1;
		if ( child < ( count - 1 ) && comp( arr[ child ], arr[ child + 1 ] ) )
			++child;

		if ( comp( arr[ root ], arr[ child ] ) ) {
			Swap( &arr[ root ], &arr[ child ] );
			root = child;
		} else {
			break;
		}
	}
}

template < class type, class Compare > 
void HeapSort_Sort( type *arr, int count, Compare comp ) {
	int start = 0, end = ( count - 1 );

	while ( start <= ( count - 2 ) ) {
		++start;
		HeapSort_Float( arr, start, comp );
	}

	while ( end > 0 ) {
		Swap( &arr[ end ], &arr[ 0 ] );
		HeapSort_Sink( arr, 0, end, comp );
		end--;
	}
} 

template < class type > 
void HeapSort_Sort( type *arr, int count ) {
	HeapSort_Sort( arr, count, Less<type>() );
}

/*
	http://www.cs.rpi.edu/~musser/gp/introsort.ps


	IntroSort
*/

#define INTROSORT_THRESHOLD 16

__inline
size_t log2( size_t n ) {
	size_t pow = 0;
	
	while ( n > 1 ) {
		n >>= 1;
		++pow;
	}

	return ( pow );
}

template < class type, class Compare >
__inline type *median( type &a, type &b, type &c, Compare comp ) {
	if ( comp( a, b ) ) {
		if ( comp( b, c ) ) {
			return &b;
		} else if ( comp( a, c ) ) {
			return &c;
		} else {
			return &a;
		}
	} else if ( comp( a, c ) ) {
		return &a;
	} else if ( comp( b, c ) ) {
		return &c;
	} else {
		return &b;
	}
}

template < class type, class Compare >
type *IntroSort_Partition( type *lower, type *upper, Compare comp ) {
    type pivot = *median( *lower, *( lower + ( upper - lower + 1 ) / 2 ), *upper, comp );

	while ( true ) {
		while ( comp( *lower, pivot ) )
			++lower;
		while ( comp( pivot, *upper ) )
			--upper;
		if ( lower >= upper )
			return ( lower );

		Swap( upper, lower );
		++lower;
		--upper;
	}
}

template < class type, class Compare >
void IntroSort_SortLoop( type *lower, type *upper, size_t depth_limit, Compare comp ) {
	while ( upper - lower >= INTROSORT_THRESHOLD ) {
        if ( depth_limit <= 0 ) {
			HeapSort_Sort( lower, (int )( upper - lower + 1 ), comp );
            return;
        }

		depth_limit--;
		type *mid = IntroSort_Partition( lower, upper, comp );
		IntroSort_SortLoop( mid, upper, depth_limit, comp );
		upper = mid - 1;
    }
}

template < class type, class Compare >
void IntroSort_Sort( type *lower, type *upper, Compare comp ) {
	size_t items = ( upper - lower ) + 1;
	if ( items <= 1 )
		return;

	IntroSort_SortLoop( lower, upper, log2( items ) * 2, comp );

	if ( items > INTROSORT_THRESHOLD ) {
		InsertionSort_Sort( lower, lower + INTROSORT_THRESHOLD - 1, comp );
		InsertionSort_SortReckless( lower + INTROSORT_THRESHOLD, upper, comp );
	} else {
		InsertionSort_Sort( lower, upper, comp );
	}
}

template < class type >
void IntroSort_Sort( type *lower, type *upper ) {
	IntroSort_Sort( lower, upper, Less<type>() );
}


#endif // __SORT_H__

