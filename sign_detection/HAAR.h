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

#ifndef __INCLUDE_ADABOOST_HAAR__
#define __INCLUDE_ADABOOST_HAAR__

#ifndef __INCLUDE_ADABOOST_COMMON__
#include "common.h"
#endif

#ifndef __INCLUDE_ADABOOST_FEATURE__
#include "feature.h"
#endif

#include <vector>

#if !defined( __NO_ADABOOST_NAMESPACE__ ) || __NO_ADABOOST_NAMESPACE__ == 0
// AdaBoost名前空間の始まり
_ADABOOST_BEGIN
#endif


/// @brief Haar-like 特徴量1
//! 
//! □■ のパターン
//! 
#define __HAAR_PATTERN1__		0

/// @brief Haar-like 特徴量2
//! 
//! ■
//! □ のパターン
//! 
#define __HAAR_PATTERN2__		1

/// @brief Haar-like 特徴量3
//! 
//! □■□ のパターン
//! 
#define __HAAR_PATTERN3__		2

/// @brief Haar-like 特徴量4
//! 
//! □
//! ■
//! □ のパターン
//! 
#define __HAAR_PATTERN4__		3

/// @brief Haar-like 特徴量5
//! 
//! □
//! 　■ のパターン
//! 
#define __HAAR_PATTERN5__		4

/// @brief Haar-like 特徴量6
//! 
//! 　□
//! ■　 のパターン
//! 
#define __HAAR_PATTERN6__		5

/// @brief Haar-like 特徴量7
//! 
//! □■
//! ■□ のパターン
//! 
#define __HAAR_PATTERN7__		6

/// @brief Haar-like 特徴量7
//! 
//! □□□
//! □■□ のパターン
//! □□□
//! 
#define __HAAR_PATTERN8__		7


struct feature_info : public feature_info_base
{
	typedef feature_info_base base;

	size_type x;
	size_type y;
	size_type w;
	size_type h;

	feature_info( size_type m = 0, size_type xx = 0, size_type yy = 0, size_type ww = 1, size_type hh = 1 )
		: base( m ), x( xx ), y( yy ), w( ww ), h( hh )
	{
	}

	feature_info( const feature_info &f ) : base( f ), x( f.x ), y( f.y ), w( f.w ), h( f.h )
	{
	}

	size_type number_of_bins( ) const
	{
#if defined( __HISTOGRAM_CLASSIFIER_NUM_BINS__ ) && __HISTOGRAM_CLASSIFIER_NUM_BINS__ == 256
		return( 256 );
#else
		return( 512 );
#endif
	}

	std::string get_pattern_string( ) const
	{
		std::string str;
		switch( __GET_ADABOOST_FEATURE_PATTERN__( mode ) )
		{
		case __HAAR_PATTERN1__:
			str = "□■";
			break;

		case __HAAR_PATTERN2__:
			str = "■\n□";
			break;

		case __HAAR_PATTERN3__:
			str = "□■□";
			break;

		case __HAAR_PATTERN4__:
			str = "□\n■\n□";
			break;

		case __HAAR_PATTERN5__:
			str = "□\n　■";
			break;

		case __HAAR_PATTERN6__:
			str = "　□\n■";
			break;

		case __HAAR_PATTERN7__:
			str = "□■\n■□";
			break;

		case __HAAR_PATTERN8__:
		default:
			str = "□□□\n□■□\n□□□";
			break;
		}

		return( str );
	}

	std::string to_string( ) const
	{
		char buff[ 256 ];
		sprintf( buff, "x=%ld, y=%ld, w=%ld, h=%ld, type=%ld, ", x, y, w, h, __GET_ADABOOST_FEATURE_PATTERN__( mode ) + 1 );

		return( std::string( buff ) + base::to_string( ) );
	}
};


template < class IntegralImage, class FeatureInfo >
inline feature_value_type compute_feature( const std::vector< IntegralImage > &in, const FeatureInfo &f, size_t offset_x = 0, size_t offset_y = 0, double scale = 1.0 )
{
	typedef IntegralImage                                 integral_image_type;
	typedef typename integral_image_type::value_type      value_type;
	typedef typename integral_image_type::size_type       size_type;
	typedef typename integral_image_type::difference_type difference_type;

	double val = 0, size = 1;
	size_type fx   = static_cast< size_type >( f.x * scale ) + offset_x;
	size_type fy   = static_cast< size_type >( f.y * scale ) + offset_y;
	size_type fw   = static_cast< size_type >( f.w * scale );
	size_type fh   = static_cast< size_type >( f.h * scale );

	const IntegralImage &iimg = in[ __GET_ADABOOST_FEATURE_COLOR__( f.mode ) ];

	switch( __GET_ADABOOST_FEATURE_PATTERN__( f.mode ) )
	{
	case __HAAR_PATTERN1__:
		// □■ のパターンを列挙
		val  = static_cast< double >( iimg( fx, fy, fw, fh ) - iimg( fx + fw, fy, fw, fh ) );
		size = static_cast< double >( fw * fh );
		break;

	case __HAAR_PATTERN2__:
		// ■
		// □ のパターンを列挙
		val  = static_cast< double >( iimg( fx, fy + fh, fw, fh ) - iimg( fx, fy, fw, fh ) );
		size = static_cast< double >( fw * fh );
		break;

	case __HAAR_PATTERN3__:
		// □■□ のパターンを列挙
		val  = static_cast< double >( iimg( fx, fy, fw, fh ) - 2 * iimg( fx + fw, fy, fw, fh ) + iimg( fx + fw * 2, fy, fw, fh ) );
		size = fw * fh * 2.0;
		break;

	case __HAAR_PATTERN4__:
		// □
		// ■
		// □ のパターンを列挙
		val  = static_cast< double >( iimg( fx, fy, fw, fh ) - 2 * iimg( fx, fy + fh, fw, fh ) + iimg( fx, fy + fh * 2, fw, fh ) );
		size = fw * fh * 2.0;
		break;

	case __HAAR_PATTERN5__:
		// □
		// 　■ のパターンを列挙
		val  = static_cast< double >( iimg( fx, fy, fw, fh ) - iimg( fx + fw, fy + fh, fw, fh ) );
		size = static_cast< double >( fw * fh );
		break;

	case __HAAR_PATTERN6__:
		// 　□
		// ■　 のパターンを列挙
		val  = static_cast< double >( iimg( fx + fw, fy, fw, fh ) - iimg( fx, fy + fh, fw, fh ) );
		size = static_cast< double >( fw * fh );
		break;

	case __HAAR_PATTERN7__:
		// □■
		// ■□ のパターンを列挙
		val  = static_cast< double >( iimg( fx, fy, fw, fh ) - iimg( fx + fw, fy, fw, fh ) - iimg( fx, fy + fh, fw, fh ) + iimg( fx + fw, fy + fh, fw, fh ) );
		size = fw * fh * 2.0;
		break;

	case __HAAR_PATTERN8__:
	default:
		// □□□
		// □■□ のパターンを列挙
		// □□□
		val  = static_cast< double >( iimg( fx, fy, fw * 3, fh * 3 ) - iimg( fx + fw, fy + fh, fw, fh ) * 8 );
		size = fw * fh * 8.0;
		break;
	}

#if defined( __USE_FAST_WEAK_LEARNER__ ) && __USE_FAST_WEAK_LEARNER__ == 0
	return( static_cast< feature_value_type >( val / ( scale * scale ) + 0.5 ) );
#else
	//val /= size;
	//val = val < -127.0 ? -127.0 : ( val > 128.0 ? 128.0 : val );
	//return( static_cast< feature_value_type >( ( val + 127.0 ) / 255.0 * ( __HISTOGRAM_CLASSIFIER_NUM_BINS__ - 1 ) + 0.5 ) );
	return( static_cast< feature_value_type >( ( val / size + 255.0 ) / 511.0 * ( __HISTOGRAM_CLASSIFIER_NUM_BINS__ - 1 ) + 0.5 ) );
#endif
}

template < class IntegralImage, class FEATURE_LIST, class FEATURE_IDMAP, class FEATURE_TYPE >
inline void compute_feature( const std::vector< IntegralImage > &in, const FEATURE_LIST &flist, const FEATURE_IDMAP &fidmap, FEATURE_TYPE &f, size_t xx, size_t yy, double scale )
{
	unsigned int x = static_cast< unsigned int >( xx );
	unsigned int y = static_cast< unsigned int >( yy );

	#pragma omp parallel for firstprivate( x, y, scale ) schedule( guided )
	for( int l = 0 ; l < static_cast< int >( fidmap.size( ) ) ; l++ )
	{
		f[ l ] = compute_feature( in, flist[ fidmap[ l ] ], x, y, scale );
	}

	f.type( __ADABOOST_FEATURE_HAAR__ );
}

template < class Array, class FeatureInfo >
void construct_feature_list( const Array &in, std::vector< FeatureInfo > &list, typename Array::size_type sw = 1, typename Array::size_type sh = 1, typename Array::size_type mw = 0, typename Array::size_type mh = 0 )
{
	typedef Array                                image_type;
	typedef typename image_type::size_type       size_type;
	typedef typename image_type::difference_type difference_type;
	typedef FeatureInfo							 haar_like_feature;

	list.clear( );

	if( sw < 1 )
	{
		sw = 1;
	}
	if( sh < 1 )
	{
		sh = 1;
	}

	// □■ のパターンを列挙
	for( size_type j = mh ; j < in.height( ) - mh ; j++ )
	{
		for( size_type i = mw ; i < in.width( ) - mw ; i++ )
		{
			for( size_type n = j + sh ; n <= in.height( ) - mh ; n += sh )
			{
				for( size_type m = i + 2 * sw ; m <= in.width( ) - mw ; m += 2 * sw )
				{
					size_type w = ( m - i ) / 2;
					size_type h = n - j;

					//printf( "%d, %d, %d, %d\n", i, j, w, h );

					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN1__, __ADABOOST_COLOR1__ ), i, j, w, h ) );

#if defined( __USE_COLOR_FOR_COMPUTING_FEATURE__ ) && __USE_COLOR_FOR_COMPUTING_FEATURE__ != 0
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN1__, __ADABOOST_COLOR2__ ), i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN1__, __ADABOOST_COLOR3__ ), i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN1__, __ADABOOST_COLOR4__ ), i, j, w, h ) );

#if __USE_COLOR_FOR_COMPUTING_FEATURE__ >= 2
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN1__, __ADABOOST_COLOR5__ ), i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN1__, __ADABOOST_COLOR6__ ), i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN1__, __ADABOOST_COLOR7__ ), i, j, w, h ) );
#endif

#if __USE_COLOR_FOR_COMPUTING_FEATURE__ >= 3
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN1__, __ADABOOST_COLOR8__ ),  i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN1__, __ADABOOST_COLOR9__ ),  i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN1__, __ADABOOST_COLOR10__ ), i, j, w, h ) );
#endif

#endif
				}
			}
		}
	}

	// ■
	// □ のパターンを列挙
	for( size_type j = mh ; j < in.height( ) - mh ; j++ )
	{
		for( size_type i = mw ; i < in.width( ) - mw ; i++ )
		{
			for( size_type n = j + 2 * sh ; n <= in.height( ) - mh ; n += 2 * sh )
			{
				for( size_type m = i + sw ; m <= in.width( ) - mw ; m += sw )
				{
					size_type w = m - i;
					size_type h = ( n - j ) / 2;

					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN2__, __ADABOOST_COLOR1__ ), i, j, w, h ) );

#if defined( __USE_COLOR_FOR_COMPUTING_FEATURE__ ) && __USE_COLOR_FOR_COMPUTING_FEATURE__ != 0
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN2__, __ADABOOST_COLOR2__ ), i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN2__, __ADABOOST_COLOR3__ ), i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN2__, __ADABOOST_COLOR4__ ), i, j, w, h ) );

#if __USE_COLOR_FOR_COMPUTING_FEATURE__ >= 2
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN2__, __ADABOOST_COLOR5__ ), i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN2__, __ADABOOST_COLOR6__ ), i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN2__, __ADABOOST_COLOR7__ ), i, j, w, h ) );
#endif

#if __USE_COLOR_FOR_COMPUTING_FEATURE__ >= 3
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN2__, __ADABOOST_COLOR8__ ),  i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN2__, __ADABOOST_COLOR9__ ),  i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN2__, __ADABOOST_COLOR10__ ), i, j, w, h ) );
#endif

#endif
				}
			}
		}
	}

	// □■□ のパターンを列挙
	for( size_type j = mh ; j < in.height( ) - mh ; j++ )
	{
		for( size_type i = mw ; i < in.width( ) - mw ; i++ )
		{
			for( size_type n = j + sh ; n <= in.height( ) - mh ; n += sh )
			{
				for( size_type m = i + 3 * sw ; m <= in.width( ) - mw ; m += 3 * sw )
				{
					size_type w = ( m - i ) / 3;
					size_type h = n - j;

					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN3__, __ADABOOST_COLOR1__ ), i, j, w, h ) );

#if defined( __USE_COLOR_FOR_COMPUTING_FEATURE__ ) && __USE_COLOR_FOR_COMPUTING_FEATURE__ != 0
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN3__, __ADABOOST_COLOR2__ ), i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN3__, __ADABOOST_COLOR3__ ), i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN3__, __ADABOOST_COLOR4__ ), i, j, w, h ) );

#if __USE_COLOR_FOR_COMPUTING_FEATURE__ >= 2
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN3__, __ADABOOST_COLOR5__ ), i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN3__, __ADABOOST_COLOR6__ ), i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN3__, __ADABOOST_COLOR7__ ), i, j, w, h ) );
#endif

#if __USE_COLOR_FOR_COMPUTING_FEATURE__ >= 3
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN3__, __ADABOOST_COLOR8__ ),  i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN3__, __ADABOOST_COLOR9__ ),  i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN3__, __ADABOOST_COLOR10__ ), i, j, w, h ) );
#endif

#endif
				}
			}
		}
	}

	// □
	// ■
	// □ のパターンを列挙
	for( size_type j = mh ; j < in.height( ) - mh ; j++ )
	{
		for( size_type i = mw ; i < in.width( ) - mw ; i++ )
		{
			for( size_type n = j + 3 * sh ; n <= in.height( ) - mh ; n += 3 * sh )
			{
				for( size_type m = i + sw ; m <= in.width( ) - mw ; m += sw )
				{
					size_type w = m - i;
					size_type h = ( n - j ) / 3;

					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN4__, __ADABOOST_COLOR1__ ), i, j, w, h ) );

#if defined( __USE_COLOR_FOR_COMPUTING_FEATURE__ ) && __USE_COLOR_FOR_COMPUTING_FEATURE__ != 0
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN4__, __ADABOOST_COLOR2__ ), i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN4__, __ADABOOST_COLOR3__ ), i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN4__, __ADABOOST_COLOR4__ ), i, j, w, h ) );

#if __USE_COLOR_FOR_COMPUTING_FEATURE__ >= 2
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN4__, __ADABOOST_COLOR5__ ), i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN4__, __ADABOOST_COLOR6__ ), i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN4__, __ADABOOST_COLOR7__ ), i, j, w, h ) );
#endif

#if __USE_COLOR_FOR_COMPUTING_FEATURE__ >= 3
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN4__, __ADABOOST_COLOR8__ ),  i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN4__, __ADABOOST_COLOR9__ ),  i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN4__, __ADABOOST_COLOR10__ ), i, j, w, h ) );
#endif

#endif
				}
			}
		}
	}

	// □
	// 　■ のパターンを列挙
	//
	// 　□
	// ■　 のパターンを列挙
	//
	// □■
	// ■□ のパターンを列挙
	for( size_type j = mh ; j < in.height( ) - mh ; j++ )
	{
		for( size_type i = mw ; i < in.width( ) - mw ; i++ )
		{
			for( size_type n = j + 2 * sh ; n <= in.height( ) - mh ; n += 2 * sh )
			{
				for( size_type m = i + 2 * sw ; m <= in.width( ) - mw ; m += 2 * sw )
				{
					size_type w = ( m - i ) / 2;
					size_type h = ( n - j ) / 2;

					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN5__, __ADABOOST_COLOR1__ ), i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN6__, __ADABOOST_COLOR1__ ), i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN7__, __ADABOOST_COLOR1__ ), i, j, w, h ) );

#if defined( __USE_COLOR_FOR_COMPUTING_FEATURE__ ) && __USE_COLOR_FOR_COMPUTING_FEATURE__ != 0
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN5__, __ADABOOST_COLOR2__ ), i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN5__, __ADABOOST_COLOR3__ ), i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN5__, __ADABOOST_COLOR4__ ), i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN6__, __ADABOOST_COLOR2__ ), i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN6__, __ADABOOST_COLOR3__ ), i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN6__, __ADABOOST_COLOR4__ ), i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN7__, __ADABOOST_COLOR2__ ), i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN7__, __ADABOOST_COLOR3__ ), i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN7__, __ADABOOST_COLOR4__ ), i, j, w, h ) );

#if __USE_COLOR_FOR_COMPUTING_FEATURE__ >= 2
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN5__, __ADABOOST_COLOR5__ ), i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN5__, __ADABOOST_COLOR6__ ), i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN5__, __ADABOOST_COLOR7__ ), i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN6__, __ADABOOST_COLOR5__ ), i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN6__, __ADABOOST_COLOR6__ ), i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN6__, __ADABOOST_COLOR7__ ), i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN7__, __ADABOOST_COLOR5__ ), i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN7__, __ADABOOST_COLOR6__ ), i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN7__, __ADABOOST_COLOR7__ ), i, j, w, h ) );
#endif

#if __USE_COLOR_FOR_COMPUTING_FEATURE__ >= 3
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN5__, __ADABOOST_COLOR8__ ),  i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN5__, __ADABOOST_COLOR9__ ),  i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN5__, __ADABOOST_COLOR10__ ), i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN6__, __ADABOOST_COLOR8__ ),  i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN6__, __ADABOOST_COLOR9__ ),  i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN6__, __ADABOOST_COLOR10__ ), i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN7__, __ADABOOST_COLOR8__ ),  i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN7__, __ADABOOST_COLOR9__ ),  i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN7__, __ADABOOST_COLOR10__ ), i, j, w, h ) );
#endif

#endif
				}
			}
		}
	}


	// □□□
	// □■□
	// □□□ のパターンを列挙
	for( size_type j = mh ; j < in.height( ) - mh ; j++ )
	{
		for( size_type i = mw ; i < in.width( ) - mw ; i++ )
		{
			for( size_type n = j + 3 * sh ; n <= in.height( ) - mh ; n += 3 * sh )
			{
				for( size_type m = i + 3 * sw ; m <= in.width( ) - mw ; m += 3 * sw )
				{
					size_type w = ( m - i ) / 3;
					size_type h = ( n - j ) / 3;

					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN8__, __ADABOOST_COLOR1__ ), i, j, w, h ) );

#if defined( __USE_COLOR_FOR_COMPUTING_FEATURE__ ) && __USE_COLOR_FOR_COMPUTING_FEATURE__ != 0
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN8__, __ADABOOST_COLOR2__ ), i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN8__, __ADABOOST_COLOR3__ ), i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN8__, __ADABOOST_COLOR4__ ), i, j, w, h ) );

#if __USE_COLOR_FOR_COMPUTING_FEATURE__ >= 2
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN8__, __ADABOOST_COLOR5__ ), i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN8__, __ADABOOST_COLOR6__ ), i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN8__, __ADABOOST_COLOR7__ ), i, j, w, h ) );
#endif

#if __USE_COLOR_FOR_COMPUTING_FEATURE__ >= 3
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN8__, __ADABOOST_COLOR8__ ),  i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN8__, __ADABOOST_COLOR9__ ),  i, j, w, h ) );
					list.push_back( haar_like_feature( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_HAAR__, __HAAR_PATTERN8__, __ADABOOST_COLOR10__ ), i, j, w, h ) );
#endif

#endif
				}
			}
		}
	}
}

#if !defined( __NO_ADABOOST_NAMESPACE__ ) || __NO_ADABOOST_NAMESPACE__ == 0
// AdaBoost名前空間の終わり
_ADABOOST_END
#endif

#endif	// __INCLUDE_ADABOOST_HAAR__
