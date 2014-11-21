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

#ifndef __INCLUDE_ADABOOSTER_VALIDATION__
#define __INCLUDE_ADABOOSTER_VALIDATION__

#include <map>

#include <libxml/xmlreader.h>
#include <libxml/xmlwriter.h>

#ifndef __INCLUDE_ADABOOST_COMMON__
#include "common.h"
#endif

#ifndef __INCLUDE_ADABOOST_UTILITY__
#include "utility.h"
#endif

// AdaBoost名前空間の始まり
_ADABOOST_BEGIN

struct validation_data
{
	typedef size_t size_type;			///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef ptrdiff_t difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ

	difference_type x;
	difference_type y;
	difference_type w;
	difference_type h;
	difference_type category;
	difference_type tmp;
	double weight;
	double minw;
	double maxw;
	size_type count;
	difference_type id;

	validation_data( difference_type xx = 0, difference_type yy = 0, difference_type ww = 0, difference_type hh = 0, difference_type c = 1, double we = 1.0, size_type cc = 1, double minW = 1.0e100, double maxW = -1.0e100, difference_type ID = -1 )
		: x( xx ), y( yy ), w( ww ), h( hh ), category( c ), tmp( 0 ), weight( we ), minw( minW ), maxw( maxW ), count( cc ), id( ID )
	{
	}

	validation_data( const validation_data &f ) : x( f.x ), y( f.y ), w( f.w ), h( f.h ), category( f.category ), tmp( f.tmp ), weight( f.weight ), minw( f.minw ), maxw( f.maxw ), count( f.count ), id( f.id )
	{
	}

	bool empty( ) const
	{
		return( w == 0 || h == 0 );
	}

	bool is_inside( difference_type xx, difference_type yy ) const
	{
		return( x <= xx && xx < x + w && y <= yy && yy < y + h );
	}

	bool operator <( const validation_data &v ) const
	{
		if( id < v.id )
		{
			return( true );
		}
		else if( id == v.id )
		{
			if( x < v.x )
			{
				return( true );
			}
			else if( x == v.x )
			{
				if( y < v.y )
				{
					return( true );
				}
				else if( y == v.y )
				{
					if( w < v.w )
					{
						return( true );
					}
					else if( w == v.w )
					{
						return( h < v.h );
					}
					else
					{
						return( false );
					}
				}
				else
				{
					return( false );
				}
			}
			else
			{
				return( false );
			}
		}
		else
		{
			return( false );
		}
	}

	double overlap_ratio( double xx, double yy, double ww, double hh ) const
	{
		double x1 = x > xx ? x : xx;
		double y1 = y > yy ? y : yy;
		double x2 = x + w < xx + ww ? x + w : xx + ww;
		double y2 = y + h < yy + hh ? y + h : yy + hh;
		if( x1 < x2 && y1 < y2 )
		{
			double a = ( x2 - x1 ) * ( y2 - y1 );
			return( a / ( w * h + ww * hh - a ) );
		}
		else
		{
			return( 0.0 );
		}
	}

	double overlap_ratio( const validation_data &v ) const
	{
		return( overlap_ratio( static_cast< double >( v.x ), static_cast< double >( v.y ), static_cast< double >( v.w ), static_cast< double >( v.h ) ) );
	}
};

/// @brief 指定されたストリームに評価用データを整形して出力する
inline ::std::ostream &operator <<( ::std::ostream &out, const validation_data &v )
{
	out << v.x << ",";
	out << v.y << ",";
	out << v.w << ",";
	out << v.h << ",";
	out << v.weight;

	return( out );
}


namespace __io__
{
	/// @brief libxml2のAPIを利用してタグの属性を取得する関数
	inline std::string get_value( xmlTextReaderPtr reader, const std::string &default_value = "" )
	{
		const xmlChar * value = xmlTextReaderConstValue( reader );
		if( value == NULL )
		{
			return( default_value );
		}
		else
		{
			return( std::string( ( const char * )value ) );
		}
	}

	/// @brief libxml2のAPIを利用してタグの属性を取得する関数
	inline std::string get_attribute( xmlTextReaderPtr reader, const std::string &attr, const std::string &default_value = "" )
	{
		// ノードのアトリビュートを持つノードを取得する
		xmlChar *value = xmlTextReaderGetAttribute( reader, ( const xmlChar * )attr.c_str( ) );

		if( value == NULL )
		{
			return( default_value );
		}
		else
		{
			return( std::string( ( const char * )value ) );
		}
	}

	inline int write_validation_data( xmlTextWriterPtr writer, const std::multimap< std::string, validation_data > &data, std::string path )
	{
		if( writer == NULL )
		{
			return( -1 );
		}

		std::multimap< std::string, validation_data >::const_iterator ite = data.begin( );

		// カスケード型識別器の開始タグ
		if( xmlTextWriterStartElement( writer, BAD_CAST "images" ) < 0 )
		{
			std::cerr << "Failed to write <images> tag." << std::endl;
			return( -1 );
		}

#ifdef _WIN32
		const char __separator__ = '\\';
#else
		const char __separator__ = '/';
#endif

		if( path.length( ) > 0 && path[ path.length( ) - 1 ] != __separator__ )
		{
			path += __separator__;
		}

		if( xmlTextWriterWriteAttribute( writer, BAD_CAST "path", BAD_CAST path.c_str( ) ) < 0 )
		{
			std::cerr << "Failed to add a property to XML tag." << std::endl;
			return( -1 );
		}

		for( ; ite != data.end( ) ; )
		{
			std::multimap< std::string, validation_data >::const_iterator lite = ite;
			std::multimap< std::string, validation_data >::const_iterator uite = data.upper_bound( ite->first );

			std::string src = ite->first;

			if( utility::begin_with( src, path ) )
			{
				src = src.substr( path.length( ) );
			}

			ite = uite;

			// カスケード型識別器の開始タグ
			if( xmlTextWriterStartElement( writer, BAD_CAST "image" ) < 0 )
			{
				std::cerr << "Failed to write <image> tag." << std::endl;
				return( -1 );
			}

			if( xmlTextWriterWriteAttribute( writer, BAD_CAST "src", BAD_CAST src.c_str( ) ) < 0 )
			{
				std::cerr << "Failed to add a property to XML tag." << std::endl;
				return( -1 );
			}

			for( ; lite != uite ; ++lite )
			{
				if( !lite->second.empty( ) )
				{
					if( xmlTextWriterStartElement( writer, BAD_CAST "rect" ) < 0 )
					{
						std::cerr << "Failed to write <rect> tag." << std::endl;
						return( -1 );
					}

					if( lite->second.id >= 0 && xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "id", "%ld", lite->second.id ) < 0 )
					{
						std::cerr << "Failed to add a property to XML tag." << std::endl;
						return( -1 );
					}

					if( xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "x", "%ld", lite->second.x ) < 0 )
					{
						std::cerr << "Failed to add a property to XML tag." << std::endl;
						return( -1 );
					}

					if( xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "y", "%ld", lite->second.y ) < 0 )
					{
						std::cerr << "Failed to add a property to XML tag." << std::endl;
						return( -1 );
					}

					if( xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "w", "%ld", lite->second.w ) < 0 )
					{
						std::cerr << "Failed to add a property to XML tag." << std::endl;
						return( -1 );
					}

					if( xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "h", "%ld", lite->second.h ) < 0 )
					{
						std::cerr << "Failed to add a property to XML tag." << std::endl;
						return( -1 );
					}

					if( xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "category", "%ld", lite->second.category ) < 0 )
					{
						std::cerr << "Failed to add a property to XML tag." << std::endl;
						return( -1 );
					}

					if( xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "weight", "%lf", lite->second.weight ) < 0 )
					{
						std::cerr << "Failed to add a property to XML tag." << std::endl;
						return( -1 );
					}

					if( lite->second.count > 1 )
					{
						if( xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "minw", "%lf", lite->second.minw ) < 0 )
						{
							std::cerr << "Failed to add a property to XML tag." << std::endl;
							return( -1 );
						}

						if( xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "maxw", "%lf", lite->second.maxw ) < 0 )
						{
							std::cerr << "Failed to add a property to XML tag." << std::endl;
							return( -1 );
						}
					}

					if( xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "count", "%ld", lite->second.count ) < 0 )
					{
						std::cerr << "Failed to add a property to XML tag." << std::endl;
						return( -1 );
					}

					// カスケード型識別器の終了タグ
					if( xmlTextWriterEndElement( writer ) < 0 )
					{
						std::cerr << "Failed to write </rect> tag." << std::endl;
						return( -1 );
					}
				}
			}

			// カスケード型識別器の終了タグ
			if( xmlTextWriterEndElement( writer ) < 0 )
			{
				std::cerr << "Failed to write </image> tag." << std::endl;
				return( -1 );
			}
		}

		// カスケード型識別器の終了タグ
		if( xmlTextWriterEndElement( writer ) < 0 )
		{
			std::cerr << "Failed to write </images> tag." << std::endl;
			return( -1 );
		}

		return( 0 );
	}


	inline int read_rectangle_data( xmlTextReaderPtr reader, validation_data &v )
	{
		v.id       = utility::ToInt( get_attribute( reader, "id", "-1" ) );
		v.x        = utility::ToInt( get_attribute( reader, "x", "-1" ) );
		v.y        = utility::ToInt( get_attribute( reader, "y", "-1" ) );
		v.w        = utility::ToInt( get_attribute( reader, "w", "1" ) );
		v.h        = utility::ToInt( get_attribute( reader, "h", "1" ) );
		v.category = utility::ToInt( get_attribute( reader, "category", "-1" ) );
		v.weight   = utility::ToDouble( get_attribute( reader, "weight", "1" ) );
		v.minw     = utility::ToDouble( get_attribute( reader, "minw", "1.0e100" ) );
		v.maxw     = utility::ToDouble( get_attribute( reader, "maxw", "-1.0e100" ) );
		v.count    = utility::ToInt( get_attribute( reader, "count", "1" ) );

		return( v.x >= 0 && v.y >= 0 ? 1 : -1 );
	}

	inline int read_rectangle_data( xmlTextReaderPtr reader, std::multimap< std::string, validation_data > &data, const std::string &path )
	{
		if( reader == NULL )
		{
			return( -1 );
		}

		bool isEnd  = false;
		int  ret    = 1;
		int  sdepth = xmlTextReaderDepth( reader );
		std::string src = "";

		while( ret == 1 && !isEnd )
		{
			const xmlChar *name = xmlTextReaderConstName( reader );
			const int  depth    = xmlTextReaderDepth( reader );
			const bool isEmpty  = xmlTextReaderIsEmptyElement( reader ) != 0;
			//const bool hasValue = xmlTextReaderHasValue( reader ) != 0;

			switch( xmlTextReaderNodeType( reader ) )
			{
			case 1: // Element
				if( sdepth == depth )
				{
					if( xmlStrcasecmp( name, BAD_CAST "image" ) != 0 )
					{
						std::cerr << "Root element of XML should be started with <image> tag." << std::endl;
						return( -1 );
					}
					else
					{
						src = utility::convert_path( get_attribute( reader, "src", "" ) );
						src = utility::combine_path( path, src );

						if( isEmpty )
						{
							data.insert( std::multimap< std::string, validation_data >::value_type( src, validation_data( ) ) );
						}
					}
				}
				else if( sdepth + 1 == depth && xmlStrcasecmp( name, BAD_CAST "rect" ) == 0 )
				{
					// 弱識別器の情報を抽出する
					validation_data vd;
					ret = read_rectangle_data( reader, vd );

					if( ret == 1 )
					{
						data.insert( std::multimap< std::string, validation_data >::value_type( src, vd ) );
					}
				}
				else if( depth > sdepth )
				{
					std::cerr << "Failed to parse the XML file." << std::endl;
					return( -1 );
				}
				break;

			case 15: // EndElement
				if( sdepth == depth && xmlStrcasecmp( name, BAD_CAST "image" ) == 0 )
				{
					isEnd = true;
				}
				break;

			default:
			case 0: // None
			case 2: // Attribute
			case 3: // Text
			case 4: // CDATA
			case 5: // EntityReference
			case 6: // Entity
			case 7: // ProcessingInstruction
			case 8: // Comment
			case 9: // Document
			case 10: // DocumentType
			case 11: // DocumentFragment
			case 12: // Notation
			case 13: // Whitespace
			case 14: // SignificantWhitespace
			case 16: // EndEntity
			case 17: // XmlDeclaration
				break;
			}

			if( !isEnd )
			{
				ret = xmlTextReaderRead( reader );
			}
		}

		if( ret < 0 )
		{
			std::cerr << "Error occured when parsing the XML file." << std::endl;
			return( -1 );
		}

		return( 1 );
	}

	inline int read_validation_data( xmlTextReaderPtr reader, std::multimap< std::string, validation_data > &data )
	{
		if( reader == NULL )
		{
			return( -1 );
		}

		bool isEnd  = false;
		int  ret    = 1;
		int  sdepth = xmlTextReaderDepth( reader );
		std::string path = "";

		while( ret == 1 && !isEnd )
		{
			const xmlChar *name = xmlTextReaderConstName( reader );
			const int  depth    = xmlTextReaderDepth( reader );
			//const bool isEmpty  = xmlTextReaderIsEmptyElement( reader ) != 0;
			//const bool hasValue = xmlTextReaderHasValue( reader ) != 0;

			switch( xmlTextReaderNodeType( reader ) )
			{
			case 1: // Element
				if( sdepth == depth )
				{
					if( xmlStrcasecmp( name, BAD_CAST "images" ) != 0 )
					{
						std::cerr << "Root element of XML should be started with <images> tag." << std::endl;
						return( -1 );
					}
					else
					{
						path = utility::convert_path( get_attribute( reader, "path", "" ) );
					}
				}
				else if( sdepth + 1 == depth && xmlStrcasecmp( name, BAD_CAST "image" ) == 0 )
				{
					ret = read_rectangle_data( reader, data, path );
				}
				else if( depth > sdepth )
				{
					std::cerr << "Failed to parse the XML file." << std::endl;
					return( -1 );
				}
				break;

			case 15: // EndElement
				if( sdepth == depth && xmlStrcasecmp( name, BAD_CAST "images" ) == 0 )
				{
					isEnd = true;
				}
				break;

			default:
			case 0: // None
			case 2: // Attribute
			case 3: // Text
			case 4: // CDATA
			case 5: // EntityReference
			case 6: // Entity
			case 7: // ProcessingInstruction
			case 8: // Comment
			case 9: // Document
			case 10: // DocumentType
			case 11: // DocumentFragment
			case 12: // Notation
			case 13: // Whitespace
			case 14: // SignificantWhitespace
			case 16: // EndEntity
			case 17: // XmlDeclaration
				break;
			}

			if( !isEnd )
			{
				ret = xmlTextReaderRead( reader );
			}
		}

		if( ret < 0 )
		{
			std::cerr << "Error occured when parsing the XML file." << std::endl;
			return( -1 );
		}

		return( 1 );
	}
}

inline bool read_validation_data( std::multimap< std::string, validation_data > &data, const std::string &filename )
{
	// ライブラリを初期化し、コンパイル時とDLLのバージョンの違いによるAPIの非互換性をテストする。
	LIBXML_TEST_VERSION

	// XMLReader を初期化する
	xmlTextReaderPtr reader = xmlReaderForFile( filename.c_str( ), NULL, 0 );
	if( reader == NULL )
	{
		std::cerr << "Failed to open a file [" << filename << "].";
		return( false );
	}

	// 識別器を初期化する
	data.clear( );

	// 最初のデータを読み取る
	int ret = xmlTextReaderRead( reader );

	if( ret >= 0 )
	{
		// XMLファイルからデータを読み取る
		ret = __io__::read_validation_data( reader, data );
	}

	// XMLReaderが確保しているリソースを解放する
	xmlFreeTextReader( reader );

	// XML ライブラリのクリーンアップを実行する。
	xmlCleanupParser( );

	if( ret < 0 )
	{
		// テスト用にメモリをダンプする
		xmlMemoryDump( );

		return( false );
	}
	else
	{
		return( true );
	}
}

inline bool write_validation_data( const std::multimap< std::string, validation_data > &data, const std::string &filename )
{
	// ライブラリを初期化し、コンパイル時とDLLのバージョンの違いによるAPIの非互換性をテストする。
	LIBXML_TEST_VERSION

	// XMLReader を初期化する
	xmlTextWriterPtr writer = xmlNewTextWriterFilename( filename.c_str( ), 0 );
	if( writer == NULL )
	{
		std::cerr << "Failed to create a file [" << filename << "].";
		return( false );
	}

	int ret = -1;

	// XMLファイルのインデントを設定する
	ret = xmlTextWriterSetIndent( writer, 1 );
	if( ret < 0 )
	{
		std::cerr << "Failed to enable XML indentatation." << std::endl;
	}

	// XMLファイルのインデント文字をタブに設定する
	ret = xmlTextWriterSetIndentString( writer, BAD_CAST "\t" );
	if( ret < 0 )
	{
		std::cerr << "Failed to change XML indentat character." << std::endl;
	}

	ret = xmlTextWriterStartDocument( writer, NULL, "utf-8", NULL );
	if( ret < 0 )
	{
		std::cerr << "Failed to write XML tag." << std::endl;
	}

	if( ret >= 0 && !data.empty( ) )
	{
		// 全体が同じパスを共有しているかどうかを調べる
		std::string path = "";
		{
			std::multimap< std::string, validation_data >::const_iterator ite = data.begin( );
			for( ; ite != data.end( ) ; )
			{
				if( path.empty( ) )
				{
					path = utility::get_directory_name( ite->first );
				}
				else
				{
					std::string p = utility::get_directory_name( ite->first );
					size_t i = 0;
					for( ; i < p.length( ) && i < path.length( ) ; i++ )
					{
						if( p[ i ] != path[ i ] )
						{
							break;
						}
					}

					path = path.substr( 0, i );
				}

				ite = data.upper_bound( ite->first );
			}

			path = utility::get_directory_name( path );
		}

		ret = __io__::write_validation_data( writer, data, path );
	}

	if( ret >= 0 && xmlTextWriterEndDocument( writer ) < 0 )
	{
		std::cerr << "Failed to write XML tag." << std::endl;
		ret = -1;
	}


	// XMLReaderが確保しているリソースを解放する
	xmlFreeTextWriter( writer );

	// XMLReaderが確保しているリソースを解放する
	xmlCleanupParser( );

	if( ret < 0 )
	{
		// テスト用にメモリをダンプする
		xmlMemoryDump( );
	}

	return( true );
}

// AdaBoost名前空間の終わり
_ADABOOST_END


#endif	// __INCLUDE_ADABOOSTER_VALIDATION__