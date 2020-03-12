#ifndef __LIST_H__
#define __LIST_H__

#include "Memstar.h"
#include "Sort.h"

template< class type > 
class List {
public:
	class Iterator {
	public:
		Iterator( type *item ) : mItem(item) {}
		type &value() { return ( *mItem ); }
		type *pointer() { return ( mItem ); }
		type &operator* () { return ( *mItem ); }
		Iterator &operator++ () { ++mItem; return ( *this ); }
		Iterator &operator-- () { --mItem; return ( *this ); }
		bool operator== ( const Iterator &b ) { return ( mItem == b.mItem ); }
		bool operator!= ( const Iterator &b ) { return ( mItem != b.mItem ); }
		bool operator>= ( const Iterator &b ) { return ( mItem >= b.mItem ); }
		bool operator<= ( const Iterator &b ) { return ( mItem <= b.mItem ); }
		bool operator> ( const Iterator &b ) { return ( mItem > b.mItem ); }
		bool operator< ( const Iterator &b ) { return ( mItem < b.mItem ); }
	private:
		type *mItem;
	};

	List( ) {
		mHead = ( NULL );
		mItemCount = 0;
		mCurAlloc = 0;
	}

	List( size_t reserve ) {
		mHead = ( NULL );
		mItemCount = 0;
		mCurAlloc = 0;
		Resize( reserve );
	}

	~List( ) {
		Free( );
	}

	void Clear( ) {
		mItemCount = 0;
	}

	void Free( ) {
		if ( !mHead )
			return;
		Clear( );

		free( mHead );
		mHead = NULL;
		mCurAlloc = 0;
	}


	bool Add( const type &item ) {
		return ( Push( item ) );
	}

	Iterator Begin( ) const {
		return ( Iterator( mHead ) );
	}

	type *Allocate( size_t amount ) {
		if ( !ResizeUp( mItemCount + amount ) )
			return ( NULL );
		type *start = ( &mHead[ mItemCount ] );
		mItemCount += ( amount );
		return ( start );
	}

	void Compact( ) {
		Resize( mItemCount );
	}

	size_t Count( ) const {
		return ( mItemCount );
	}

	void Delete( Iterator &iter ) {
		type *tmp = ( iter.pointer() );
		if ( tmp < mHead || tmp >= mHead + mItemCount )
			return;

		if ( tmp < ( mHead + mItemCount - 1 ) )
			memcpy( tmp, mHead + mItemCount - 1, sizeof( type ) );
		ResizeDown( --mItemCount );
	}

	void Delete( const type &item ) {
		Delete( Find( item ) );
	}

	Iterator End( ) const {
		return ( Iterator( mHead + mItemCount ) );
	}

	Iterator Find( const type &item ) const {
		for ( size_t i = 0; i < mItemCount; i++ ) {
			if ( mHead[ i ] == item )
				return ( Iterator( mHead + i ) );
		}

		return ( End( ) );
	}

	type *First( ) {
		if ( !mItemCount )
			return ( NULL );
		return ( mHead );
	}

	type *Last( ) {
		if ( !mItemCount )
			return ( NULL );
		return ( mHead + mItemCount - 1 );
	}

	type *New( ) {
		if ( !ResizeUp( mItemCount + 1 ) )
			return ( NULL );
		
		return ( &mHead[ mItemCount++ ] );
	}

	type Pop( ) {
		if ( !mItemCount )
			return ( NULL );
		type item = mHead[ mItemCount - 1 ];
		mItemCount -= 1;
		return ( item );
	}

	bool Push( const type &item ) {
		if ( !ResizeUp( mItemCount + 1 ) )
			return ( false );
		
		mHead[ mItemCount++ ] = item;
		return ( true );
	}

	void Release( size_t amount ) {
		mItemCount -= ( amount );
		ResizeDown( mItemCount );
	}

	bool Resize( size_t new_size ) {
		type *tmp = (type *)realloc( mHead, new_size * sizeof( type ) );
		if ( tmp ) {
			mHead = tmp;
			mCurAlloc = new_size;
		}

		return ( tmp != NULL );
	}

	template< class Compare >
	void Sort( Compare comp ) {
		IntroSort_Sort( mHead, mHead + mItemCount - 1, comp );
	}

	void Sort( ) {
		Sort( Less<type>() );
	}

	bool ResizeUp( size_t new_size ) {
		if ( new_size <= mCurAlloc )
			return ( true );
		return ( Resize( new_size << 1 ) );
	}

	bool ResizeDown( size_t new_size ) {
		if ( new_size >= ( mCurAlloc >> 3 ) )
			return ( true );

		size_t resize = ( mCurAlloc >> 2 );
		return ( Resize( resize ) );
	}

	type &operator[]( const size_t index ) const {
		return ( *( mHead + index ) );
	}
	
	type &operator[]( const size_t index ) {
		return ( *( mHead + index ) );
	}

protected:
	type *mHead;
	size_t mItemCount, mCurAlloc;
};

#endif // __LIST_H__