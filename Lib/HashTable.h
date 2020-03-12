#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

#include "Hash.h"
#include "Strings.h"
#include "Comparisons.h"

/*
	Supports common types
*/

class HashKey {
public:
	unsigned int operator() ( unsigned int i ) { return ( i ); } // uint key assumed to be a hash
	unsigned int operator() ( const char *str ) { return ( HashString(str) ); }
	unsigned int operator() ( const String &str ) { return ( HashString(str.c_str()) ); }
};

class KeyCmp {
public:
	bool operator() ( int a, int b ) const { return ( a == b ); }
	bool operator() ( const char *a, const char *b ) const { return ( strcmp( a, b ) == 0 ); }
	bool operator() ( const String &a, const String &b ) const { return ( a == b ); }
};

class IKeyCmp {
public:
	bool operator() ( int a, int b ) const { return ( a == b ); }
	bool operator() ( const char *a, const char *b ) const { return ( _stricmp( a, b ) == 0 ); }
	bool operator() ( const String &a, const String &b ) const { return ( a.IEquals( b ) ); }
};

/*
	Use ValueDeleter<key_type,value_type> if your hash node value is a pointer and you 
	need it deleted when the node is deleted
*/

template< class key_type, class value_type >
class NoDeleter {
public:
	void operator() ( key_type &key, value_type &value ) {
	}
};

template< class key_type, class value_type >
class ValueDeleter {
public:
	void operator() ( key_type &key, value_type &value ) {
		delete value;
	}
};

template< class key_type, class value_type >
class KeyValueDeleter {
public:
	void operator() ( key_type &key, value_type &value ) {
		delete key;
		delete value;
	}
};

/*
	HashTable<
		key_type,
		value_type,
		compare object => KeyCmp,
		value deleter => Blank<value_type>,
		key hasher => HashKey
	>
*/

template < class key_type, class value_type, class equals = KeyCmp, class deleter = NoDeleter<key_type,value_type>, class hash_key = HashKey >
class HashTable {
public:
	/*
		Node
	*/

	class Node {
	public:
		Node( const key_type &key, const value_type &value, Node *next ) : mNext(next) { 
			mValue = ( value );
			mKey = ( key ); 
		}

		~Node( ) {
			deleter()( mKey, mValue );
		}

		Node *mNext;
		key_type mKey;
		value_type mValue;		
	};

	/*
		Iterator
	*/

	class Iterator {
	public:
		friend class HashTable;		

		Iterator( ): mNode(NULL) {}
		Iterator( HashTable *hash ) : mHash(hash), mBucketOn(-1), mNode(NULL) { SkipToNext(); }

		key_type &key() const { return ( mNode->mKey );	}
		value_type &value() const { return ( mNode->mValue ); }

		// ++ on End() will always exit
		Iterator &operator++ ( ) {
			if ( mNode ) {
				mNode = ( mNode->mNext );
				SkipToNext( );
			}

			return ( *this );
		}

		bool operator!= ( Iterator &b ) { return ( mNode != b.mNode ); }
		bool operator== ( Iterator &b ) { return ( mNode == b.mNode ); }


	protected:
		void SkipToNext( ) {
			while ( !mNode ) {
				if ( mBucketOn >= ( mHash->mBucketCount - 1 ) )
					break;

				mNode = ( mHash->mBuckets[ ++mBucketOn ] );
			}
		}

		Node *mNode;
		HashTable *mHash;		
		int mBucketOn;
	};

	
	/*
		Hash Table
	*/

	HashTable( int num_buckets ) : mItemCount(0) {
		mBucketCount = ( SmallestPow2( num_buckets ) );
		mBucketMask = ( mBucketCount - 1 );

		mBuckets = new Node*[ mBucketCount ];
		memset( mBuckets, 0, sizeof( Node* ) * mBucketCount );
	}

	~HashTable( ) {
		Clear( );
		delete[] mBuckets;
		mBuckets = ( NULL );
	}

	Iterator Begin( ) {
		if ( !mItemCount )
			return ( End( ) );
		
		return ( Iterator( this ) );
	}

	size_t Bucket( const key_type &key, int mask ) const {
		return ( hash_key()( key ) & mask );
	}

	size_t Bucket( const key_type &key ) const {
		return ( Bucket( key, mBucketMask ) );
	}

	void Clear( ) {
		if ( !mItemCount )
			return;

		for ( int i = 0; i < mBucketCount; i++ ) {
			Node *node = ( mBuckets[ i ] );
			while ( node ) {
				Node *next = ( node->mNext );
				delete node;
				node = next;
			}
			
			mBuckets[ i ] = NULL;
		}

		mItemCount = 0;
	}

	void Compact( ) {	
		Resize( SmallestPow2( mItemCount ) );
	}

	int Count( ) const {
		return ( mItemCount );
	}

	void Delete( const key_type &key ) {
		if ( !mItemCount )
			return;

		Node **head = ( &mBuckets[ Bucket( key ) ] );
		Node *node = ( *head ), *prev = ( NULL );

		while ( node ) {
			Node *next = ( node->mNext );

			if ( mCompare( node->mKey, key ) ) {
				Unlink( head, prev, node );
				return;
			} else {
				prev = ( node );				
			}

			node = ( next );
		}
	}

	void Delete( Iterator &iter ) {
		if ( !iter.mNode )
			return;

		if ( iter.mBucketOn >= mBucketCount )
			return;

		Node **head = ( &mBuckets[ iter.mBucketOn ] ), *node = ( *head ), *prev = ( NULL );
		for ( ; node && ( node != iter.mNode ); prev = node, node = node->mNext ) {}

		if ( node != iter.mNode )
			return;

		iter.mNode = Unlink( head, prev, node );
		if ( !iter.mNode ) 
			iter.SkipToNext();
	}

	Iterator End( ) {
		return ( Iterator( ) );
	}

	value_type *Find( const key_type &key ) const {
		Node **head = ( &mBuckets[ Bucket( key ) ] );
		Node *node = ( *head );

		for ( node = *head; node; node = node->mNext ) {
			if ( mCompare( node->mKey, key ) )
				return ( &node->mValue );
		}

		return ( NULL );
	}


	value_type *Insert( const key_type &key, const value_type &value ) {
		CheckIncrease();
		Node **head = ( &mBuckets[ Bucket( key ) ] );
		value_type *ret = ( Link( head, key, value ) );
		return ( ret );
	}

	value_type *InsertUnique( const key_type &key, const value_type &value ) {
		CheckIncrease();

		Node **head = ( &mBuckets[ Bucket( key ) ] );
		Node *node = ( *head );

		for ( node = *head; node; node = node->mNext ) {
			if ( mCompare( node->mKey, key ) ) {
				return ( &node->mValue );
			}
		}
		
		value_type *ret = ( Link( head, key, value ) );
		return ( ret );
	}

	int Size( ) {
		return ( mItemCount );
	}

	int SmallestPow2( int cap ) const {
		int count = ( 1 );
		while ( count < cap )
			count <<= 1;
		return ( count );
	}

	value_type &operator[] ( const key_type &key ) {
		return ( *InsertUnique( key, value_type() ) );
	}


private:
	value_type *Link( Node **bucket, const key_type &key, const value_type &value ) {
		Node *node = new Node( key, value, *bucket );
		if ( node ) {
			*bucket = node;
			mItemCount++;
			return ( &node->mValue );
		} else {
			return ( NULL );
		}
	}

	Node *Unlink( Node **bucket, Node *prev, Node *node ) {
		Node *next = ( node->mNext );
		if ( prev )
			prev->mNext = ( next );
		else
			*bucket = ( next );

		delete node;
		mItemCount--;
		return ( next );
	}


	void CheckIncrease( ) {
		if ( mBucketCount <= mItemCount )
			Resize( mBucketCount << 1 );
	}

	void Resize( int bucket_count ) {
		if ( mBucketCount == bucket_count )
			return;

		Node **new_buckets = new Node*[ bucket_count ];
		if ( !new_buckets )
			return;
		memset( new_buckets, 0, sizeof( Node** ) * bucket_count );

		for ( int i = 0; i < mBucketCount; i++ ) {
			Node *node = ( mBuckets[ i ] );
			while ( node ) {
				Node *next = ( node->mNext );
				Node **head = &new_buckets[ Bucket( node->mKey, ( bucket_count - 1 ) ) ];
				node->mNext = ( *head );
				*head = node;
				node = ( next );
			}
		}

		delete[] mBuckets;
		mBuckets = ( new_buckets );
		mBucketCount = ( bucket_count );
		mBucketMask = ( bucket_count - 1 );
	}


	Node **mBuckets;
	int mBucketCount, mBucketMask, mItemCount;
	equals mCompare;
};


#endif // __HASHTABLE_H__