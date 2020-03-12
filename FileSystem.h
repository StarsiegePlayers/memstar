#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include "Fear.h"
#include "HashTable.h"
#include "List.h"
#include "Strings.h"
#include "Texture.h"
#include "zlib/unzip.h"

class FileInfo {
public:
	virtual const char *Path( ) const = 0;
	virtual bool Read( char *&buffer, int &size ) = 0;
};

class DiskFile : public FileInfo {
public:
	DiskFile( const String &path ) { mPath = path; }

	virtual const char *Path( ) const { 
		return ( mPath.c_str( ) ); 
	}

	virtual bool Read( char *&buffer, int &size ) {
		if ( !mPath.Strlen( ) )
			return ( false );
		
		HANDLE f = CreateFile( mPath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
		DWORD r;
		
		if ( f == INVALID_HANDLE_VALUE )
			return ( false );

		size = ( GetFileSize( f, NULL ) );
		buffer = new char[ size ];
		if ( buffer && ReadFile( f, buffer, size, &r, NULL ) ) {
			if ( r != size ) {
				delete[] buffer;
				buffer = ( NULL );
			}
		}
		CloseHandle( f );

		return ( buffer != NULL );
	}

private:
	String mPath;
};

class ZipFile : public FileInfo {
public:
	ZipFile( unzFile zip, int zip_size, int zip_offset ) : mZip(zip), mZipSize(zip_size), mZipOffset(zip_offset) { }

	virtual const char *Path( ) const { 
		return ( "" );
	}

	virtual bool Read( char *&buffer, int &size ) {
		size = ( mZipSize );
		buffer = new char[ mZipSize ];
		unzSetOffset( mZip, mZipOffset );
		if ( buffer && ( unzOpenCurrentFile( mZip ) == UNZ_OK ) ) {
			if ( unzReadCurrentFile( mZip, buffer, mZipSize ) != mZipSize ) {
				delete[] buffer;
				buffer = ( NULL );
			}
			unzCloseCurrentFile( mZip );
		}

		return ( buffer != NULL );
	}

private:
	int mZipSize;
	unzFile mZip;
	int mZipOffset;
};


class FileSystem {
public:
	typedef HashTable< 
		String, FileInfo*, 
		IKeyCmp,
		ValueDeleter<String,FileInfo*>
	> FileHash;

	typedef FileHash::Iterator Iterator;

	typedef List< unzFile > ZipList;
	typedef ZipList::Iterator ZipIterator;

	FileSystem( int hash_size ) : mFiles(hash_size) { }
	FileSystem( ) : mFiles(256) { }
	~FileSystem( ) { Clear( ); }

	Iterator Begin( ) {
		return ( mFiles.Begin( ) );
	}

	void Clear( ) {	
		mFiles.Clear( );

		ZipIterator iter = mZipHandles.Begin( );
		while ( iter != mZipHandles.End( ) ) {
			unzClose( iter.value() );
			++iter;
		}
		mZipHandles.Clear( );
	}

	Iterator End( ) {
		return ( mFiles.End( ) );
	}

	void Grok( const char *path ) {
		Clear( );

		Scan( path, false ); // read in non-zips first
		Scan( path, true );
	}

	void GrokNonZips( const char *path ) {
		Clear( );

		Scan( path, false );
	}

	bool Read( const String &name, char *&buffer, int &size ) {
		FileInfo **info = ( mFiles.Find( name ) );
		if ( !info )
			return ( false );

		return ( (*info)->Read( buffer, size ) );
	}

	bool ReadTGA( const String &name, Texture &texture ) {		
		char *buffer;
		int size;
		
		if ( !Read( name, buffer, size ) ) 
			return ( NULL );

		bool ok = ( texture.LoadTGA( (unsigned char *)buffer ) );
		delete[] buffer;
		return ( ok );
	}

private:
	void ProcessZip( const char *path );
	void Scan( const char *path, bool zip_scan );

	FileHash mFiles;
	ZipList mZipHandles;
};


#endif // __FILESYSTEM_H__
