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

#include <iostream>
#include <fstream>
#include <ctime>
#include <string>

#ifdef _OPENMP
#include <omp.h>
#endif

#include <mist/mist.h>
#include <mist/io/image.h>
#include <mist/utility/options.h>

#include <mist/timer.h>
#include <mist/random.h>
#include <mist/interpolate.h>

#include "utility.h"
#include "validation.h"

template < class T > inline T minimum( const T &v1, const T &v2 ){ return( v1 < v2 ? v1 : v2 ); }
template < class T > inline T maximum( const T &v1, const T &v2 ){ return( v1 > v2 ? v1 : v2 ); }
template < class T > inline T minimum( const T &v1, const T &v2, const T &v3 ){ return( minimum( minimum( v1, v2 ), v3 ) ); }
template < class T > inline T maximum( const T &v1, const T &v2, const T &v3 ){ return( maximum( maximum( v1, v2 ), v3 ) ); }
template < class T > inline T minimum( const T &v1, const T &v2, const T &v3, const T &v4 ){ return( minimum( minimum( v1, v2 ), minimum( v3, v4 ) ) ); }
template < class T > inline T maximum( const T &v1, const T &v2, const T &v3, const T &v4 ){ return( maximum( maximum( v1, v2 ), maximum( v3, v4 ) ) ); }

template < class Array1, class Array2 >
void resize_image( const Array1 &in, Array2 &out, typename Array1::difference_type sx, typename Array1::difference_type sy,
				 typename Array1::difference_type sw, typename Array1::difference_type sh, typename Array1::difference_type dw, typename Array1::difference_type dh, double angle = 0.0, bool norescale = false, int style = 0, typename Array1::size_type nthreads = 0 )
{
	typedef typename Array1::difference_type difference_type;
	typedef typename Array1::size_type       size_type;
	typedef typename Array1::value_type      value_type;

	if( angle != 0.0 && style != 0 )
	{
		const double pai = 3.1415926535897932384626433832795;
		const double c = std::cos( angle / 180.0 * pai );
		const double s = std::sin( angle / 180.0 * pai );
		const double cx = sw * 0.5;
		const double cy = sh * 0.5;

		double xx[ 4 ], yy[ 4 ], minX, maxX, minY, maxY;

		switch( style )
		{
		case 2:	// △
			xx[ 0 ] = ( - cx ) * c - ( + cy ) * s + sx + cx;
			yy[ 0 ] = ( - cx ) * s + ( + cy ) * c + sy + cy;
			xx[ 1 ] = ( + cx ) * c - ( + cy ) * s + sx + cx;
			yy[ 1 ] = ( + cx ) * s + ( + cy ) * c + sy + cy;
			xx[ 2 ] = - ( - cy ) * s + sx + cx;
			yy[ 2 ] = + ( - cy ) * c + sy + cy;

			minX = minimum( xx[ 0 ], xx[ 1 ], xx[ 2 ] );
			minY = minimum( yy[ 0 ], yy[ 1 ], yy[ 2 ] );
			maxX = maximum( xx[ 0 ], xx[ 1 ], xx[ 2 ] );
			maxY = maximum( yy[ 0 ], yy[ 1 ], yy[ 2 ] );
			break;

		case 3:	// □
			xx[ 0 ] = ( - cx ) * c - ( - cy ) * s + sx + cx;
			yy[ 0 ] = ( - cx ) * s + ( - cy ) * c + sy + cy;
			xx[ 1 ] = ( + cx ) * c - ( - cy ) * s + sx + cx;
			yy[ 1 ] = ( + cx ) * s + ( - cy ) * c + sy + cy;
			xx[ 2 ] = ( - cx ) * c - ( + cy ) * s + sx + cx;
			yy[ 2 ] = ( - cx ) * s + ( + cy ) * c + sy + cy;
			xx[ 3 ] = ( + cx ) * c - ( + cy ) * s + sx + cx;
			yy[ 3 ] = ( + cx ) * s + ( + cy ) * c + sy + cy;

			minX = minimum( xx[ 0 ], xx[ 1 ], xx[ 2 ], xx[ 3 ] );
			minY = minimum( yy[ 0 ], yy[ 1 ], yy[ 2 ], yy[ 3 ] );
			maxX = maximum( xx[ 0 ], xx[ 1 ], xx[ 2 ], xx[ 3 ] );
			maxY = maximum( yy[ 0 ], yy[ 1 ], yy[ 2 ], yy[ 3 ] );
			break;

		case 4:	// ◇
			xx[ 0 ] = ( - cx ) * c + sx + cx;
			yy[ 0 ] = ( - cx ) * s + sy + cy;
			xx[ 1 ] = - ( - cy ) * s + sx + cx;
			yy[ 1 ] = + ( - cy ) * c + sy + cy;
			xx[ 2 ] = - ( + cy ) * s + sx + cx;
			yy[ 2 ] = + ( + cy ) * c + sy + cy;
			xx[ 3 ] = ( + cx ) * c + sx + cx;
			yy[ 3 ] = ( + cx ) * s + sy + cy;

			minX = minimum( xx[ 0 ], xx[ 1 ], xx[ 2 ], xx[ 3 ] );
			minY = minimum( yy[ 0 ], yy[ 1 ], yy[ 2 ], yy[ 3 ] );
			maxX = maximum( xx[ 0 ], xx[ 1 ], xx[ 2 ], xx[ 3 ] );
			maxY = maximum( yy[ 0 ], yy[ 1 ], yy[ 2 ], yy[ 3 ] );
			break;

		case 1:	// ▽
		default:
			xx[ 0 ] = ( - cx ) * c - ( - cy ) * s + sx + cx;
			yy[ 0 ] = ( - cx ) * s + ( - cy ) * c + sy + cy;
			xx[ 1 ] = ( + cx ) * c - ( - cy ) * s + sx + cx;
			yy[ 1 ] = ( + cx ) * s + ( - cy ) * c + sy + cy;
			xx[ 2 ] = - ( + cy ) * s + sx + cx;
			yy[ 2 ] = + ( + cy ) * c + sy + cy;

			minX = minimum( xx[ 0 ], xx[ 1 ], xx[ 2 ] );
			minY = minimum( yy[ 0 ], yy[ 1 ], yy[ 2 ] );
			maxX = maximum( xx[ 0 ], xx[ 1 ], xx[ 2 ] );
			maxY = maximum( yy[ 0 ], yy[ 1 ], yy[ 2 ] );
			break;
		}

		sx = static_cast< difference_type >( minX );
		sy = static_cast< difference_type >( minY );
		sw = static_cast< difference_type >( maxX - minX + 1 + 0.5 );
		sh = static_cast< difference_type >( maxY - minY + 1 + 0.5 );
	}

	double asp1 = sw / static_cast< double >( sh );
	double asp2 = dw / static_cast< double >( dh );
	difference_type ddw = static_cast< difference_type >( dh * asp1 + 0.5 );

	if( dw > ddw )
	{
		double newW = sh * asp2;
		sx -= static_cast< difference_type >( ( newW - sw ) * 0.5 + 0.5 );
		sw = static_cast< difference_type >( newW + 0.5 );

		if( sx < 0 )
		{
			sx = 0;
		}
	}
	else if( dw < ddw )
	{
		double newH = sw / asp2;
		sy -= static_cast< difference_type >( ( newH - sh ) * 0.5 + 0.5 );
		sh = static_cast< difference_type >( newH + 0.5 );

		if( sy < 0 )
		{
			sy = 0;
		}
	}

	double scale = dw / static_cast< double >( sw );

	Array1 tmp( sw, sh );

	if( angle == 0.0 )
	{
		in.trim( tmp, sx, sy, sw, sh );
	}
	else
	{
		difference_type imgW = in.width( );
		difference_type imgH = in.height( );
		const double pai = 3.1415926535897932384626433832795;
		double c = std::cos( -angle / 180.0 * pai );
		double s = std::sin( -angle / 180.0 * pai );
		double cx = sw * 0.5;
		double cy = sh * 0.5;

		#pragma omp parallel for firstprivate( cx, cy, sx, sy, imgW, imgH, c, s )
		for( int y = 0 ; y < static_cast< int >( sh ) ; y++ )
		//for( difference_type y = 0 ; y < sh ; y++ )
		{
			for( difference_type x = 0 ; x < sw ; x++ )
			{
				double xx = ( x - cx ) * c - ( y - cy ) * s + sx + cx;
				double yy = ( x - cx ) * s + ( y - cy ) * c + sy + cy;
				difference_type xxx = static_cast< difference_type >( xx );
				difference_type yyy = static_cast< difference_type >( yy );

				if( 0 <= xxx && xxx < imgW - 1 && 0 <= yyy && yyy < imgH - 1 )
				{
					xx -= xxx;
					yy -= yyy;

					value_type val = static_cast< value_type >( ( in( xxx, yyy ) * ( 1.0 - xx ) + in( xxx + 1, yyy ) * xx ) * ( 1.0 - yy )
																+ ( in( xxx, yyy + 1 ) * ( 1.0 - xx ) + in( xxx + 1, yyy + 1 ) * xx ) * yy );
					tmp( x, y ) = val;
				}
			}
		}
	}

	if( norescale )
	{
		out = tmp;
	}
	else if( scale < 0.5 )
	{
		mist::mean::interpolate( tmp, out, dw, dh, nthreads );
	}
	else if( scale < 1.0 )
	{
		Array1 ttt;
		mist::cubic::interpolate( tmp, ttt, dw * 2, dh * 2, nthreads );
		mist::mean::interpolate( ttt, out, dw, dh, nthreads );
	}
	else
	{
		mist::cubic::interpolate( tmp, out, dw, dh, nthreads );
	}
}


int main( int argc, char *argv[] )
{
	typedef adaboost::color_type									color_type;
	typedef adaboost::image_type									image_type;
	typedef adaboost::color_image_type								color_image_type;
	typedef adaboost::integral_image_type							integral_image_type;
	typedef image_type::size_type									size_type;
	typedef image_type::difference_type								difference_type;
	typedef std::multimap< std::string, adaboost::validation_data >	validation_map_type;
	typedef validation_map_type::value_type							validation_value_type;

	std::string header;

	header += "【学習に使用するサンプル画像を切り出すプログラム】\n\n";
	header += "[使用例]\n";
	header += "AdaSampler [評価データセットのXMLファイル1] [評価データセットのXMLファイル2] ...\n";
	header += "※「評価データセットのXMLファイル」は複数渡すことができ，複数の場合はすべての合計を計算します．\n\n";
	header += "[オプション]";

	mist::options op( header );

	op.add( "odir", "検出結果を画像として出力するディレクトリのパス（指定しない場合は出力しない）", "" );
	op.add( "split", "切り出した画像を複数枚単位でフォルダにまとめる際の枚数（0以下の場合はサブフォルダを作成しない）", -1 );
	op.add( "w", "学習画像の横幅", 15 );
	op.add( "h", "学習画像の縦幅", 15 );
	op.add( "dx", "X方向の位置ずれの範囲（1を指定すると±1画素ずれた画像も生成）", 0 );
	op.add( "dy", "Y方向の位置ずれの範囲（1を指定すると±1画素ずれた画像も生成）", 0 );
	op.add( "da", "回転の範囲（1を指定すると±1°ずれた画像も生成，単位は°）", 0 );
	op.add( "xskip", "X方向のずらし幅", 1 );
	op.add( "yskip", "Y方向のずらし幅", 1 );
	op.add( "askip", "回転方向のずらし角（単位は°）", 1 );
	op.add( "margin", "切り出すときの片側マージン量の枠の横幅に対する割合", 0.0 );
	op.add( "minsize", "サンプルに使用する標識サイズの最小値", -1 );
	op.add( "norescale", "サイズ変更して出力しない場合に指定する（デフォルトはサイズ変更する）" );
	op.add( "style", "標識の形状を指定する（0:○, 1:▽, 2:△, 3:□, 4:◇, デフォルトは 0）", 0 );
	op.add( "verbose", "処理情報を標準出力に出力するかどうか（デフォルトは出力しない）" );
	op.add( "nshuffles", "サンプルをシャッフルしたリストを作成する際のリスト内のファイル数（デフォルトは出力しない）", -1 );
	op.add( "nthreads", "使用するスレッド数（-1を指定するとCPU数を自動設定）", 2 );

	if( !op.parse_args( argc, argv ) )
	{
		op.show_help( );
		return( 1 );
	}
	else if( op.size( ) < 1 )
	{
		std::cerr << "識別器のデータもしくは評価データセットのXMLファイルが指定されていません．" << std::endl << std::endl;
		op.show_help( );
		return( 1 );
	}

	std::string xml         = op[ 0 ];
	std::string output_path = op.get_string( "odir" );
	int split_dir           = op.get_int( "split" );
	int image_width         = op.get_int( "w" );
	int image_height        = op.get_int( "h" );
	int dx                  = op.get_int( "dx" );
	int dy                  = op.get_int( "dy" );
	int da                  = op.get_int( "da" );
	int xskip               = op.get_int( "xskip" );
	int yskip               = op.get_int( "yskip" );
	int askip               = op.get_int( "askip" );
	int sign_style          = op.get_int( "style" );
	int nshuffles           = op.get_int( "nshuffles" );
	int __nthreads__        = op.get_int( "nthreads" );
	bool norescale          = op.isset( "norescale" );
	double margin           = op.get_double( "margin" );
	int min_size            = op.get_int( "minsize" );
	//bool verbose            = op.isset( "verbose" );
	validation_map_type olist;

#if defined( _OPENMP )
	if( __nthreads__ > 0 )
	{
		omp_set_num_threads( __nthreads__ );
	}
#endif

	if( min_size < 1 )
	{
		min_size = 1;
	}

	// 出力用のフォルダを作成する
	output_path = adaboost::utility::convert_dir_path( output_path );
	adaboost::utility::make_dir( output_path );

	std::vector< std::string > fname_list;
	std::ofstream fout( adaboost::utility::combine_path( output_path, "list.txt" ).c_str( ) );

	std::string path = output_path;
	difference_type count = 0, dcount = 0;

	for( size_type i = 0 ; i < op.size( ) ; i++ )
	{
		validation_map_type vlist;

		if( !adaboost::read_validation_data( vlist, op[ i ] ) )
		{
			// ファイルが読めないみたい・・・
			std::cerr << "評価用XMLファイル [" << op[ i ] << "] の読み込みに失敗しました．" << std::endl;
			return( 1 );
		}

		validation_map_type::const_iterator site = vlist.begin( );
		for( ; site != vlist.end( ) ; )
		{
			std::string ifname = site->first;
			validation_map_type::const_iterator eite = vlist.upper_bound( site->first );

			if( site->second.w < 1 || site->second.h < 1 )
			{
				std::cout << "Skipping " << ifname << "                            \r" << std::flush;
				site = eite;
				continue;
			}
			else
			{
				std::cout << "Sampling from " << ifname << " ";
			}

			color_image_type out, tmp;

			if( !mist::read_image( out, ifname ) )
			{
				// ファイルが読めないみたい・・・
				std::cerr << "ファイル [" << ifname << "] の読み込みに失敗しました．" << std::endl;
				return( 1 );
			}

			int imgW = static_cast< int >( out.width( ) );
			int imgH = static_cast< int >( out.height( ) );

			// この画像に対する正解領域を読み込む
			for( ; site != eite ; ++site )
			{
				//const adaboost::validation_data &v = site->second;
				adaboost::validation_data v = site->second;

				if ( v.w < min_size || v.h < min_size )
				{
					continue;
				}

				{
					difference_type margin_size = static_cast< difference_type >( v.w * margin );
					v.x -= margin_size;
					v.y -= margin_size;
					v.w += margin_size * 2;
					v.h += margin_size * 2;
				}
					

				for( int y = -dy ; y <= dy ; y += yskip )
				{
					for( int x = -dx ; x <= dx ; x += xskip )
					{
						difference_type xx = v.x + x;
						difference_type yy = v.y + y;

						if( 0 < xx && xx < imgW && 0 < yy && imgH )
						{
							for( int a = -da ; a <= da ; a += askip )
							{
								resize_image( out, tmp, xx, yy, v.w, v.h, image_width, image_height, a, norescale, sign_style, __nthreads__ < 0 ? 0 : __nthreads__ );

								if( split_dir > 0 && ( count >= split_dir || dcount == 0 ) )
								{
									path = adaboost::utility::convert_dir_path( output_path + adaboost::utility::ToString( dcount, "%02d" ) );
									adaboost::utility::make_dir( path );
									dcount++;
									count = 0;
								}

								char buff[ 256 ];
								sprintf( buff, "%06ld.png", count++ );

								std::string fname = adaboost::utility::combine_path( path, buff );
								mist::write_image( tmp, fname );
								fout << fname << std::endl;

								if( nshuffles > 0 )
								{
									fname_list.push_back( fname );
								}

								std::cout << ".";
							}
						}
					}
				}
			}

			std::cout << std::endl;
		}
	}

	fout.close( );

	if( nshuffles > 0 )
	{
		mist::uniform::random rnd( static_cast< unsigned int >( std::time( NULL ) ) );

		std::random_shuffle( fname_list.begin( ), fname_list.end( ), rnd );
		std::ofstream fsout( adaboost::utility::combine_path( output_path, "shuffle.txt" ).c_str( ) );
		for( size_type i = 0 ; i < fname_list.size( ) && i < ( size_type )nshuffles ; i++ )
		{
			fsout << fname_list[ i ] << std::endl;
		}
		fsout.close( );
	}

	return( 0 );
}
