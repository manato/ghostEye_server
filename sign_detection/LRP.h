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

#ifndef __INCLUDE_ADABOOST_LRP__
#define __INCLUDE_ADABOOST_LRP__

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

	feature_info( size_type m = 0, size_type xx = 0, size_type yy = 0, size_type ww = 1, size_type hh = 1, size_type ssx = 0, size_type ssy = 0 )
		: base( m ), x( xx ), y( yy ), w( ww ), h( hh ), sx( ssx ), sy( ssy )
	{
	}

	feature_info( const feature_info &f ) : base( f ), x( f.x ), y( f.y ), w( f.w ), h( f.h ), sx( f.sx ), sy( f.sy )
	{
	}

	size_type number_of_bins( ) const
	{
#if defined( __NUMBER_OF_LRP_RANKS__ ) && __NUMBER_OF_LRP_RANKS__ == 3
		return( 81 );
#else
		return( 729 );
#endif
	}

	std::string to_string( ) const
	{
		size_type indx = __GET_ADABOOST_FEATURE_PATTERN__( mode );
		char buff[ 256 ];
#if defined( __NUMBER_OF_LRP_RANKS__ ) && __NUMBER_OF_LRP_RANKS__ == 3
		size_type v1 = ( indx >> 8 ) & 0x0f;
		size_type v2 = ( indx >> 4 ) & 0x0f;
		size_type v3 = indx & 0x0f;
		sprintf( buff, "x=%ld, y=%ld, cw=%ld, ch=%ld, csx=%ld, csy=%ld, %ld-%ld-%ld, ", x, y, w, h, sx, sy, v1, v2, v3 );
#else
		size_type v1 = ( indx >> 4 ) & 0x0f;
		size_type v2 = indx & 0x0f;
		sprintf( buff, "x=%ld, y=%ld, cw=%ld, ch=%ld, csx=%ld, csy=%ld, %ld-%ld, ", x, y, w, h, sx, sy, v1, v2 );
#endif

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

	size_type fx0 = static_cast< size_type >( f.x * scale ) + offset_x;
	size_type fy0 = static_cast< size_type >( f.y * scale ) + offset_y;
	size_type fw  = static_cast< size_type >( f.w * scale );
	size_type fh  = static_cast< size_type >( f.h * scale );
	size_type fsx = static_cast< size_type >( f.sx * scale );
	size_type fsy = static_cast< size_type >( f.sy * scale );

	size_type fx1 = fx0 + fw + fsx;
	size_type fy1 = fy0 + fh + fsy;
	size_type fx2 = fx1 + fw + fsx;
	size_type fy2 = fy1 + fh + fsy;

	value_type v[ 9 ];

	const IntegralImage &iimg = in[ __GET_ADABOOST_FEATURE_COLOR__( f.mode ) ];
	v[ 0 ] = iimg( fx0, fy0, fw, fh );
	v[ 1 ] = iimg( fx1, fy0, fw, fh );
	v[ 2 ] = iimg( fx2, fy0, fw, fh );
	v[ 3 ] = iimg( fx0, fy1, fw, fh );
	v[ 4 ] = iimg( fx1, fy1, fw, fh );
	v[ 5 ] = iimg( fx2, fy1, fw, fh );
	v[ 6 ] = iimg( fx0, fy2, fw, fh );
	v[ 7 ] = iimg( fx1, fy2, fw, fh );
	v[ 8 ] = iimg( fx2, fy2, fw, fh );

	size_type indx = __GET_ADABOOST_FEATURE_PATTERN__( f.mode );

#if defined( __NUMBER_OF_LRP_RANKS__ ) && __NUMBER_OF_LRP_RANKS__ == 3
	value_type v1 = v[ ( indx >> 8 ) & 0x0f ];
	value_type v2 = v[ ( indx >> 4 ) & 0x0f ];
	value_type v3 = v[ indx & 0x0f ];

	size_type c1 = 0, c2 = 0, c3 = 0;

	for( size_type i = 0 ; i < 9 ; i++ )
	{
		if( v[ i ] < v1 )
		{
			c1++;
		}
		if( v[ i ] < v2 )
		{
			c2++;
		}
		if( v[ i ] < v3 )
		{
			c3++;
		}
	}

	return( static_cast< feature_value_type >( c1 * 81 + c2 * 9 + c3 ) );
#else
	value_type v1 = v[ ( indx >> 4 ) & 0x0f ];
	value_type v2 = v[ indx & 0x0f ];

	size_type c1 = 0, c2 = 0;

	for( size_type i = 0 ; i < 9 ; i++ )
	{
		value_type vv = v[ i ];

		if( vv < v1 )
		{
			c1++;
		}
		if( vv < v2 )
		{
			c2++;
		}
	}

	return( static_cast< feature_value_type >( c1 * 9 + c2 ) );
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

	f.type( __ADABOOST_FEATURE_LRP__ );
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

	// LRPのパターンを列挙
	for( size_type h = 1 ; h <= nH ; h++ )
	{
		for( size_type w = 1 ; w <= nW ; w++ )
		{
			//if( w * h == 1 )
			//{
			//	continue;
			//}

			size_type iW = imgW - 3 * w;
			size_type iH = imgH - 3 * h;
			for( size_type j = 0 ; j <= iH ; j++ )
			{
				for( size_type i = 0 ; i <= iW ; i++ )
				{
					size_type Sx = ( iW - i ) / 2;
					size_type Sy = ( iH - j ) / 2;
					for( size_type sy = 0 ; sy <= Sy ; sy++ )
					{
						for( size_type sx = 0 ; sx <= Sx ; sx++ )
						{
#if defined( __NUMBER_OF_LRP_RANKS__ ) && __NUMBER_OF_LRP_RANKS__ == 3
							for( size_type n = 0 ; n < 9 ; n++ )
							{
								for( size_type m = n + 1 ; m < 9 ; m++ )
								{
									for( size_type l = m + 1 ; l < 9 ; l++ )
									{
#else
							{
								size_type n = 0;
								for( size_type m = 0 ; m < 9 ; m++ )
								{
									for( size_type l = m + 1 ; l < 9 ; l++ )
									{
#endif
										size_type indx = ( n << 8 ) | ( m << 4 ) | ( l );

										list.push_back( FeatureInfo( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_LRP__, indx, __ADABOOST_COLOR1__ ), i, j, w, h, sx, sy ) );

#if defined( __USE_COLOR_FOR_COMPUTING_FEATURE__ ) && __USE_COLOR_FOR_COMPUTING_FEATURE__ != 0
										list.push_back( FeatureInfo( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_LRP__, indx, __ADABOOST_COLOR2__ ), i, j, w, h, sx, sy ) );
										list.push_back( FeatureInfo( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_LRP__, indx, __ADABOOST_COLOR3__ ), i, j, w, h, sx, sy ) );
										list.push_back( FeatureInfo( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_LRP__, indx, __ADABOOST_COLOR4__ ), i, j, w, h, sx, sy ) );

#if __USE_COLOR_FOR_COMPUTING_FEATURE__ >= 2
										list.push_back( FeatureInfo( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_LRP__, indx, __ADABOOST_COLOR5__ ), i, j, w, h, sx, sy ) );
										list.push_back( FeatureInfo( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_LRP__, indx, __ADABOOST_COLOR6__ ), i, j, w, h, sx, sy ) );
										list.push_back( FeatureInfo( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_LRP__, indx, __ADABOOST_COLOR7__ ), i, j, w, h, sx, sy ) );
#endif

#if __USE_COLOR_FOR_COMPUTING_FEATURE__ >= 3
										list.push_back( FeatureInfo( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_LRP__, indx, __ADABOOST_COLOR8__ ), i, j, w, h, sx, sy ) );
										list.push_back( FeatureInfo( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_LRP__, indx, __ADABOOST_COLOR9__ ), i, j, w, h, sx, sy ) );
										list.push_back( FeatureInfo( __ADABOOST_PATTERN__( __ADABOOST_FEATURE_LRP__, indx, __ADABOOST_COLOR10__ ), i, j, w, h, sx, sy ) );
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
	}
}


#if !defined( __NO_ADABOOST_NAMESPACE__ ) || __NO_ADABOOST_NAMESPACE__ == 0
// AdaBoost名前空間の終わり
_ADABOOST_END
#endif

#endif	// __INCLUDE_ADABOOST_LRP__
