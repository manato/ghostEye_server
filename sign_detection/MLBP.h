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

#ifndef __INCLUDE_ADABOOST_MLBP__
#define __INCLUDE_ADABOOST_MLBP__

#ifndef __INCLUDE_ADABOOST_COMMON__
#include "common.h"
#endif

#ifndef __INCLUDE_ADABOOST_FEATURE__
#include "feature.h"
#endif

#include <mist/limits.h>
#include <vector>

#if !defined( __NO_ADABOOST_NAMESPACE__ ) || __NO_ADABOOST_NAMESPACE__ == 0
// AdaBoost名前空間の始まり
_ADABOOST_BEGIN
#endif


struct feature_info : public feature_info_base
{
	typedef feature_info_base base;

	size_type x;
	size_type y;
	size_type w;
	size_type h;
	size_type sx;
	size_type sy;
	difference_type th;

	feature_info( size_type m = 0, size_type xx = 0, size_type yy = 0, size_type ww = 1, size_type hh = 1, size_type ssx = 0, size_type ssy = 0, difference_type t = 0 )
		: base( m ), x( xx ), y( yy ), w( ww ), h( hh ), sx( ssx ), sy( ssy ), th( t )
	{
	}

	feature_info( const feature_info &f ) : base( f ), x( f.x ), y( f.y ), w( f.w ), h( f.h ), sx( f.sx ), sy( f.sy ), th( f.th )
	{
	}

	size_type number_of_bins( ) const
	{
#if defined( __ROTATION_INVARIANT_MLBP__ ) && __ROTATION_INVARIANT_MLBP__ == 1
		return( 10 );
#else
		return( 256 );
#endif
	}

	std::string to_string( ) const
	{
		char buff[ 256 ];
		sprintf( buff, "x=%ld, y=%ld, cw=%ld, ch=%ld, csx=%ld, csy=%ld, th=%ld, ", x, y, w, h, sx, sy, th );

		return( std::string( buff ) + base::to_string( ) );
	}
};

template < class T >
inline int __sign__( const T &v )
{
	return( v >= 0 ? 1 : 0 );
}

template < class IntegralImage, class FeatureInfo >
inline feature_value_type compute_feature( const std::vector< IntegralImage > &in, const FeatureInfo &f, size_t offset_x = 0, size_t offset_y = 0, double scale = 1.0 )
{
	typedef IntegralImage                                 integral_image_type;
	typedef typename integral_image_type::value_type      value_type;
	typedef typename integral_image_type::size_type       size_type;
	typedef typename integral_image_type::difference_type difference_type;

	value_type gc, g[ 8 ];

	size_type fx0 = static_cast< size_type >( f.x * scale ) + offset_x;
	size_type fy0 = static_cast< size_type >( f.y * scale ) + offset_y;
	size_type fw  = static_cast< size_type >( f.w * scale );
	size_type fh  = static_cast< size_type >( f.h * scale );

	if( f.sx + f.sy > 0 )
	{
		size_type fsx = static_cast< size_type >( f.sx * scale );
		size_type fsy = static_cast< size_type >( f.sy * scale );

		size_type fx1 = fx0 + fw + fsx;
		size_type fy1 = fy0 + fh + fsy;
		size_type fx2 = fx1 + fw + fsx;
		size_type fy2 = fy1 + fh + fsy;

		const IntegralImage &iimg = in[ __GET_ADABOOST_FEATURE_COLOR__( f.mode ) ];
		g[ 0 ] = iimg( fx2, fy1, fw, fh );
		g[ 1 ] = iimg( fx2, fy0, fw, fh );
		g[ 2 ] = iimg( fx1, fy0, fw, fh );
		g[ 3 ] = iimg( fx0, fy0, fw, fh );
		gc     = iimg( fx1, fy1, fw, fh );
		g[ 4 ] = iimg( fx0, fy1, fw, fh );
		g[ 5 ] = iimg( fx0, fy2, fw, fh );
		g[ 6 ] = iimg( fx1, fy2, fw, fh );
		g[ 7 ] = iimg( fx2, fy2, fw, fh );
	}
	else
	{
		fx0 -= 1;
		fy0 -= 1;

		size_type fx1 = fx0 + fw;
		size_type fy1 = fy0 + fh;
		size_type fx2 = fx1 + fw;
		size_type fy2 = fy1 + fh;
		size_type fx3 = fx2 + fw;
		size_type fy3 = fy2 + fh;

		const IntegralImage &iimg = in[ __GET_ADABOOST_FEATURE_COLOR__( f.mode ) ];
		value_type i00 = iimg( fx0, fy0 );
		value_type i10 = iimg( fx1, fy0 );
		value_type i20 = iimg( fx2, fy0 );
		value_type i30 = iimg( fx3, fy0 );
		value_type i01 = iimg( fx0, fy1 );
		value_type i11 = iimg( fx1, fy1 );
		value_type i21 = iimg( fx2, fy1 );
		value_type i31 = iimg( fx3, fy1 );
		value_type i02 = iimg( fx0, fy2 );
		value_type i12 = iimg( fx1, fy2 );
		value_type i22 = iimg( fx2, fy2 );
		value_type i32 = iimg( fx3, fy2 );
		value_type i03 = iimg( fx0, fy3 );
		value_type i13 = iimg( fx1, fy3 );
		value_type i23 = iimg( fx2, fy3 );
		value_type i33 = iimg( fx3, fy3 );

		value_type i11_01 = i11 - i01;
		value_type i12_02 = i12 - i02;
		value_type i21_11 = i21 - i11;
		value_type i22_12 = i22 - i12;
		value_type i31_21 = i31 - i21;
		value_type i32_22 = i32 - i22;

		g[ 3 ] = i11_01 + i00 - i10;
		g[ 2 ] = i21_11 + i10 - i20;
		g[ 1 ] = i31_21 + i20 - i30;
		g[ 4 ] = i12_02 - i11_01   ;
		gc     = i22_12 - i21_11   ;
		g[ 0 ] = i32_22 - i31_21   ;
		g[ 5 ] = i13 - i12_02 - i03;
		g[ 6 ] = i23 - i22_12 - i13;
		g[ 7 ] = i33 - i32_22 - i23;
	}

#if defined( __ROTATION_INVARIANT_MLBP__ ) && __ROTATION_INVARIANT_MLBP__ == 1
	int s[ 8 ], U = 0;
	s[ 0 ] = __sign__( g[ 0 ] - gc );
	s[ 1 ] = __sign__( g[ 1 ] - gc );
	s[ 2 ] = __sign__( g[ 2 ] - gc );
	s[ 3 ] = __sign__( g[ 3 ] - gc );
	s[ 4 ] = __sign__( g[ 4 ] - gc );
	s[ 5 ] = __sign__( g[ 5 ] - gc );
	s[ 6 ] = __sign__( g[ 6 ] - gc );
	s[ 7 ] = __sign__( g[ 7 ] - gc );
	U += std::abs( s[ 0 ] - s[ 7 ] );
	U += std::abs( s[ 1 ] - s[ 0 ] );
	U += std::abs( s[ 2 ] - s[ 1 ] );
	U += std::abs( s[ 3 ] - s[ 2 ] );
	U += std::abs( s[ 4 ] - s[ 3 ] );
	U += std::abs( s[ 5 ] - s[ 4 ] );
	U += std::abs( s[ 6 ] - s[ 5 ] );
	U += std::abs( s[ 7 ] - s[ 6 ] );

	return( static_cast< feature_value_type >( U > 2 ? 9 : s[ 0 ] + s[ 1 ] + s[ 2 ] + s[ 3 ] + s[ 4 ] + s[ 5 ] + s[ 6 ] + s[ 7 ] ) );
#elif defined( __ROTATION_INVARIANT_MLBP__ ) && __ROTATION_INVARIANT_MLBP__ == 2
	int s[ 8 ], U = 0;
	s[ 0 ] = __sign__( g[ 0 ] - gc );
	s[ 1 ] = __sign__( g[ 1 ] - gc );
	s[ 2 ] = __sign__( g[ 2 ] - gc );
	s[ 3 ] = __sign__( g[ 3 ] - gc );
	s[ 4 ] = __sign__( g[ 4 ] - gc );
	s[ 5 ] = __sign__( g[ 5 ] - gc );
	s[ 6 ] = __sign__( g[ 6 ] - gc );
	s[ 7 ] = __sign__( g[ 7 ] - gc );
	U += std::abs( s[ 0 ] - s[ 7 ] );
	U += std::abs( s[ 1 ] - s[ 0 ] );
	U += std::abs( s[ 2 ] - s[ 1 ] );
	U += std::abs( s[ 3 ] - s[ 2 ] );
	U += std::abs( s[ 4 ] - s[ 3 ] );
	U += std::abs( s[ 5 ] - s[ 4 ] );
	U += std::abs( s[ 6 ] - s[ 5 ] );
	U += std::abs( s[ 7 ] - s[ 6 ] );

	double mu = ( g[ 0 ] + g[ 1 ] + g[ 2 ] + g[ 3 ] + g[ 4 ] + g[ 5 ] + g[ 6 ] + g[ 7 ] ) / 8.0;
	double var = 0.0;

	var += ( g[ 0 ] - mu ) * ( g[ 0 ] - mu );
	var += ( g[ 1 ] - mu ) * ( g[ 1 ] - mu );
	var += ( g[ 2 ] - mu ) * ( g[ 2 ] - mu );
	var += ( g[ 3 ] - mu ) * ( g[ 3 ] - mu );
	var += ( g[ 4 ] - mu ) * ( g[ 4 ] - mu );
	var += ( g[ 5 ] - mu ) * ( g[ 5 ] - mu );
	var += ( g[ 6 ] - mu ) * ( g[ 6 ] - mu );
	var += ( g[ 7 ] - mu ) * ( g[ 7 ] - mu );
	var = std::sqrt( var / 8.0 );

	int VAR = static_cast< int >( var ) / 8;

	int mlbp = U > 2 ? 9 : s[ 0 ] + s[ 1 ] + s[ 2 ] + s[ 3 ] + s[ 4 ] + s[ 5 ] + s[ 6 ] + s[ 7 ];
	return( static_cast< feature_value_type >( mlbp * 16 + VAR ) );
#else
	size_type mlbp = 0;
	value_type th = f.th * fw * fh;

	mlbp |= ( g[ 0 ] + th >= gc ) ?   1 : 0;
	mlbp |= ( g[ 1 ] + th >= gc ) ?   2 : 0;
	mlbp |= ( g[ 2 ] + th >= gc ) ?   4 : 0;
	mlbp |= ( g[ 3 ] + th >= gc ) ?   8 : 0;
	mlbp |= ( g[ 4 ] + th >= gc ) ?  16 : 0;
	mlbp |= ( g[ 5 ] + th >= gc ) ?  32 : 0;
	mlbp |= ( g[ 6 ] + th >= gc ) ?  64 : 0;
	mlbp |= ( g[ 7 ] + th >= gc ) ? 128 : 0;

	return( static_cast< feature_value_type >( mlbp ) );
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

	f.type( __ADABOOST_FEATURE_MLBP__ );
}

template < class Array, class FeatureInfo >
void construct_feature_list( const Array &in, std::vector< FeatureInfo > &list )
{
	typedef Array                                image_type;
	typedef typename image_type::size_type       size_type;
	typedef typename image_type::difference_type difference_type;

	list.clear( );

	size_type imgW = in.width( );
	size_type imgH = in.height( );
	size_type nW = imgW / 3;
	size_type nH = imgH / 3;

	// MLBPのパターンを列挙
	for( size_type h = 1 ; h <= nH ; h++ )
	{
		for( size_type w = 1 ; w <= nW ; w++ )
		{
			size_type iW = imgW - 3 * w;
			size_type iH = imgH - 3 * h;
			for( size_type j = 0 ; j <= iH ; j++ )
			{
				for( size_type i = 0 ; i <= iW ; i++ )
				{
#if 0
					{
						{
							size_type sx = 0, sy = 0;
#else
					size_type Sx = ( iW - i ) / 2;
					size_type Sy = ( iH - j ) / 2;
					for( size_type sy = 0 ; sy <= Sy ; sy++ )
					{
						for( size_type sx = 0 ; sx <= Sx ; sx++ )
						{
#endif
							for( difference_type th = -9 ; th <= 9 ; th += 3 )
							//for( difference_type th = 0 ; th <= 9 ; th += 3 )
							{
								list.push_back( FeatureInfo( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_MLBP__, 0, __ADABOOST_COLOR1__ ), i, j, w, h, sx, sy, th ) );

#if defined( __USE_COLOR_FOR_COMPUTING_FEATURE__ ) && __USE_COLOR_FOR_COMPUTING_FEATURE__ != 0
								list.push_back( FeatureInfo( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_MLBP__, 0, __ADABOOST_COLOR2__ ), i, j, w, h, sx, sy, th ) );
								list.push_back( FeatureInfo( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_MLBP__, 0, __ADABOOST_COLOR3__ ), i, j, w, h, sx, sy, th ) );
								list.push_back( FeatureInfo( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_MLBP__, 0, __ADABOOST_COLOR4__ ), i, j, w, h, sx, sy, th ) );

#if __USE_COLOR_FOR_COMPUTING_FEATURE__ >= 2
								list.push_back( FeatureInfo( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_MLBP__, 0, __ADABOOST_COLOR5__ ), i, j, w, h, sx, sy, th ) );
								list.push_back( FeatureInfo( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_MLBP__, 0, __ADABOOST_COLOR6__ ), i, j, w, h, sx, sy, th ) );
								list.push_back( FeatureInfo( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_MLBP__, 0, __ADABOOST_COLOR7__ ), i, j, w, h, sx, sy, th ) );
#endif

#if __USE_COLOR_FOR_COMPUTING_FEATURE__ >= 3
								list.push_back( FeatureInfo( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_MLBP__, 0, __ADABOOST_COLOR8__ ), i, j, w, h, sx, sy, th ) );
								list.push_back( FeatureInfo( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_MLBP__, 0, __ADABOOST_COLOR9__ ), i, j, w, h, sx, sy, th ) );
								list.push_back( FeatureInfo( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_MLBP__, 0, __ADABOOST_COLOR10__ ), i, j, w, h, sx, sy, th ) );
#endif

#endif
							}
						}
					}
				}
			}
		}
	}
}


#if !defined( __NO_ADABOOST_NAMESPACE__ ) || __NO_ADABOOST_NAMESPACE__ == 0
// AdaBoost名前空間の終わり
_ADABOOST_END
#endif

#endif	// __INCLUDE_ADABOOST_MLBP__
