// 
// Copyright (c) 2003-2011, MIST Project, Nagoya University
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
// 
// 3. Neither the name of the Nagoya University nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
// FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
// IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 

#ifndef __INCLUDE_ADABOOST_UTILITY__
#define __INCLUDE_ADABOOST_UTILITY__


#ifndef __INCLUDE_ADABOOST_COMMON__
#include "common.h"
#endif

// AdaBoost名前空間の始まり
_ADABOOST_BEGIN


namespace utility
{
	inline int ToInt( const std::string &val )
	{
		return( atoi( val.c_str( ) ) );
	}

	inline int ToUInt( const std::string &val )
	{
		return( static_cast< unsigned int >( ToInt( val ) ) );
	}

	inline double ToDouble( const std::string &val )
	{
		return( atof( val.c_str( ) ) );
	}

	inline std::string Trim( const std::string &val )
	{
		std::string::difference_type i;
		for( i = val.size( ) - 1 ; i >= 0 ; i-- )
		{
			if( val[ i ] != '0' && val[ i ] != ' ' )
			{
				break;
			}
		}

		if( i > 0 )
		{
			if( val[ i ] == '.' )
			{
				i--;
			}
			else if( val[ i ] == '+' || val[ i ] == '-' )
			{
				i++;
			}
		}

		return( val.substr( 0, i + 1 ) );
	}

	inline std::string Trim2( const std::string &val )
	{
		std::string::difference_type i, j, num = val.size( );

		for( i = 0 ; i < num ; i++ )
		{
			if( val[ i ] != ' ' && val[ i ] != '\t' && val[ i ] != '\r' && val[ i ] != '\n' )
			{
				break;
			}
		}

		for( j = num - 1 ; j > i ; j-- )
		{
			if( val[ j ] != ' ' && val[ j ] != '\t' && val[ j ] != '\r' && val[ j ] != '\n' )
			{
				break;
			}
		}

		return( val.substr( i, j - i + 1 ) );
	}

	inline std::string Trim0( const std::string &val )
	{
		std::string::size_type indx = val.find( '.' );

		if( indx != val.npos )
		{
			std::string::difference_type eid = indx;
			std::string::difference_type i = val.size( ) - 1;
			for( ; i > eid ; i-- )
			{
				if( val[ i ] != '0' )
				{
					break;
				}
			}

			if( i > eid )
			{
				return( val.substr( 0, i + 1 ) );
			}
			else if( i == eid )
			{
				return( val.substr( 0, i ) );
			}
		}

		return( val );
	}

	template < class T >
	inline std::string ToString( const T &v, const std::string &format )
	{
		static char buff[ 256 ];
		sprintf( buff, format.c_str( ), v );
		return( Trim0( std::string( buff ) ) );
	}

	template < class T >
	inline std::string ToString( const T &v )
	{
		static char buff[ 256 ];
		sprintf( buff, "%ld", static_cast< long int >( v ) );
		return( std::string( buff ) );
	}

	inline std::string ToString( float v )
	{
		static char buff[ 256 ];
		sprintf( buff, "%.10f", v );
		return( Trim( buff ) );
	}

	inline std::string ToString( double v )
	{
		static char buff[ 256 ];
		sprintf( buff, "%.10lf", v );
		return( Trim( buff ) );
	}

	inline std::string ToString( const std::string &str )
	{
		return( Trim2( str ) );
	}

	template < class T >
	inline std::string ToString2( const T &v )
	{
		return( "\"" + ToString( v ) + "\"" );
	}

	inline void replace_string( std::string &source, const std::string &target, const std::string &repstr )
	{
		typedef std::string::size_type size_type;

		size_type indx = source.find( target );
		if( indx != source.npos )
		{
			// 目的の文字列が見つかったので置き換える
			source.replace( indx, target.size( ), repstr );
		}
	}

	inline void replace_string_all( std::string &source, const std::string &target, const std::string &repstr )
	{
		typedef std::string::size_type size_type;

		size_type indx = source.find( target );
		while( indx != source.npos )
		{
			// 目的の文字列が見つかったので置き換える
			source.replace( indx, target.size( ), repstr );
			indx = source.find( target, indx + repstr.size( ) );
		}
	}

	template < template < typename T, typename A > class Array, class Allocator >
	inline void split_string( const std::string &source, Array< std::string, Allocator > &list, const std::string &separator = "," )
	{
		typedef std::string::size_type size_type;

		size_type sindx = 0;
		size_type indx  = source.find( separator );
		while( indx != source.npos )
		{
			// 目的の文字列が見つかったので置き換える
			std::string str = Trim2( source.substr( sindx, indx - sindx ) );
			if( !str.empty( ) )
			{
				list.push_back( str );
			}
			sindx = indx + separator.size( );
			indx = source.find( separator, sindx );
		}

		if( sindx < source.length( ) )
		{
			std::string str = Trim2( source.substr( sindx ) );
			if( !str.empty( ) )
			{
				list.push_back( str );
			}
		}
	}

	inline bool begin_with( const std::string &str, const std::string &s )
	{
		if( str.size( ) < s.size( ) )
		{
			return( false );
		}

		for( size_t i = 0 ; i < s.size( ) ; i++ )
		{
			if( str[ i ] != s[ i ] )
			{
				return( false );
			}
		}
		return( true );
	}

	inline const std::string convert_path( std::string path )
	{
		if( path.empty( ) )
		{
			return( path );
		}

#ifdef _WIN32
		if( path[ 0 ] == '/' )
		{
			path = "\\\\red" + path;
		}

		size_t indx = path.find( '/' );
		while( indx != path.npos )
		{
			// 目的の文字列が見つかったので置き換える
			path[ indx ] = '\\';
			indx = path.find( '/', indx + 1 );
		}
#else
		if( begin_with( path, "\\\\red" ) )
		{
			path = path.substr( 5 );
		}

		size_t indx = path.find( '\\' );
		while( indx != path.npos )
		{
			// 目的の文字列が見つかったので置き換える
			path[ indx ] = '/';
			indx = path.find( '\\', indx + 1 );
		}
#endif
		return( path );
	}

	inline const std::string convert_dir_path( std::string path )
	{
		path = convert_path( path );

#ifdef _WIN32
		unsigned char s = '\\';
#else
		unsigned char s = '/';
#endif

		if( path.size( ) > 0 && path[ path.size( ) - 1 ] != s )
		{
			path += s;
		}

		return( path );
	}

	inline std::string combine_path( const std::string &path1, const std::string &path2 )
	{
		std::string path = path1;

#ifdef _WIN32
		const char sep = '\\';
#else
		const char sep = '/';
#endif

		if( path.length( ) > 0 )
		{
			if( path[ path.length( ) - 1 ] != sep )
			{
				path = path + sep + path2;
			}
			else
			{
				path = path + path2;
			}
		}

		return( path );
	}

	inline std::string get_directory_name( const std::string &path )
	{
		if( path.empty( ) )
		{
			return( "" );
		}

#ifdef _WIN32
		std::string::size_type npos = path.rfind( '\\' );
#else
		std::string::size_type npos = path.rfind( '/' );
#endif

		if( npos == path.npos )
		{
			if( path.find( ':' ) == path.npos )
			{
				return( "" );
			}
			else
			{
				return( convert_dir_path( path ) );
			}
		}
		else
		{
			return( convert_dir_path( path.substr( 0, npos + 1 ) ) );
		}
	}

	inline std::string get_filename( const std::string &path )
	{
		if( path.empty( ) )
		{
			return( "" );
		}

#ifdef _WIN32
		std::string::size_type npos = path.rfind( '\\' );
#else
		std::string::size_type npos = path.rfind( '/' );
#endif

		if( npos == path.npos )
		{
			return( path );
		}
		else
		{
			return( path.substr( npos + 1 ) );
		}
	}

	inline std::string get_filename_without_ext( const std::string &path )
	{
		std::string filename = get_filename( path );

		if( filename.empty( ) )
		{
			return( "" );
		}

		std::string::size_type npos = filename.rfind( '.' );

		if( npos == filename.npos )
		{
			return( filename );
		}
		else
		{
			return( filename.substr( 0, npos ) );
		}
	}

	inline std::string get_ext( const std::string &path )
	{
		std::string filename = get_filename( path );

		if( filename.empty( ) )
		{
			return( "" );
		}

		std::string::size_type npos = filename.rfind( '.' );

		if( npos == filename.npos )
		{
			return( filename );
		}
		else
		{
			return( filename.substr( npos ) );
		}
	}

	inline const std::string _Q( const std::string &path )
	{
		return( "\"" + path + "\"" );
	}

	inline bool make_dir( std::string path, bool is_dir_path = true )
	{
		if( !is_dir_path )
		{
			path = get_directory_name( path );
		}

		int ret = 1;

		if( !path.empty( ) )
		{
#ifdef _WIN32
			ret = system( ( "mkdir " + _Q( convert_dir_path( path ) ) ).c_str( ) );
#else
			ret = system( ( "mkdir -p " + _Q( convert_dir_path( path ) ) ).c_str( ) );
#endif
		}

		return( ret == 0 );
	}

	inline bool copy_file( const std::string &from, const std::string &to )
	{
#ifdef _WIN32
		int ret = system( ( "copy /Y " + _Q( convert_path( from ) ) + " " + _Q( convert_path( to ) ) ).c_str( ) );
#else
		int ret = system( ( "/bin/cp -f " + _Q( convert_path( from ) ) + " " + _Q( convert_path( to ) ) ).c_str( ) );
#endif

		return( ret == 0 );
	}
}

// AdaBoost名前空間の終わり
_ADABOOST_END


#endif	// __INCLUDE_ADABOOST_UTILITY__