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

#ifndef __INCLUDE_ADABOOST_CLUSTERING__
#define __INCLUDE_ADABOOST_CLUSTERING__

#include <vector>
#include <list>
#include <algorithm>

#include <libxml/xmlreader.h>
#include <libxml/xmlwriter.h>

#include <mist/filter/labeling.h>
#include <mist/filter/linear.h>

#ifndef __INCLUDE_ADABOOST_COMMON__
#include "common.h"
#endif

#ifndef __INCLUDE_ADABOOST_UTILITY__
#include "utility.h"
#endif

#ifndef __INCLUDE_ADABOOSTER_VALIDATION__
#include "validation.h"
#endif


// AdaBoost名前空間の始まり
_ADABOOST_BEGIN


struct __clustering_data__
{
	typedef size_t size_type;			///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef ptrdiff_t difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ

	double x;
	double y;
	double w;
	double h;
	double weight;
	double minw;
	double maxw;
	size_type count;

	__clustering_data__( double xx = 0, double yy = 0, double ww = 0, double hh = 0, double we = 0, size_type cc = 0, double minW = 1.0e100, double maxW = -1.0e100 )
		: x( xx ), y( yy ), w( ww ), h( hh ), weight( we ), minw( minW ), maxw( maxW ), count( cc )
	{
	}

	__clustering_data__( const __clustering_data__ &f ) : x( f.x ), y( f.y ), w( f.w ), h( f.h ), weight( f.weight ), minw( f.minw ), maxw( f.maxw ), count( f.count )
	{
	}

	bool operator <( const __clustering_data__ &c ) const
	{
		return( weight > c.weight );
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

	double overlap_ratio( const __clustering_data__ &c ) const
	{
		return( overlap_ratio( c.x, c.y, c.w, c.h ) );
	}
};

inline std::ostream &operator <<( std::ostream &out, const __clustering_data__ &c )
{
	out << "(" << c.x << ", " << c.y << ", " << c.w << ", " << c.h << ") = " << c.weight << " [" << c.count << "]";

	return( out );
}

template < template < typename, typename > class LIST, class Allocator >
inline void clustering_results( LIST< validation_data, Allocator > &list, size_t image_width, size_t image_height, double threshold_for_detections = 0.0, double overlap_threshold = 0.7, bool _use_weight_for_clustering_ = true )
{
	if( list.empty( ) )
	{
		return;
	}
	else if( list.size( ) > 0 && list[ 0 ].w <= 0 && list[ 0 ].h <= 0 )
	{
		list.clear( );
		return;
	}

	typedef LIST< validation_data, Allocator > detection_list_type;
	mist::array2< unsigned int > work( image_width, image_height );
	mist::array2< unsigned int > label( image_width, image_height );

	for( typename detection_list_type::const_iterator ite = list.begin( ) ; ite != list.end( ) ; ++ite )
	{
		int cx = static_cast< int >( ite->x + ite->w / 2 );
		int cy = static_cast< int >( ite->y + ite->h / 2 );

		const int r = 2;
		for( int y = cy - r ; y <= cy + r ; y++ )
		{
			for( int x = cx - r ; x <= cx + r ; x++ )
			{
				if( 0 <= x && x < static_cast< int >( work.width( ) ) && 0 < y && y < static_cast< int >( work.height( ) ) )
				{
					work( x, y ) = 1;
				}
			}
		}
	}

	size_t lnum = mist::he::labeling8( work, label );

	std::vector< __clustering_data__ > C( lnum );

	for( typename detection_list_type::const_iterator ite = list.begin( ) ; ite != list.end( ) ; ++ite )
	{
		int cx = static_cast< int >( ite->x + ite->w / 2 );
		int cy = static_cast< int >( ite->y + ite->h / 2 );
		int l = label( cx, cy ) - 1;

		if( l >= 0 )
		{
			__clustering_data__ &c = C[ l ];
			double w = _use_weight_for_clustering_ ? ite->weight : 1.0;
			c.x += ite->x * w;
			c.y += ite->y * w;
			c.w += ite->w * w;
			c.h += ite->h * w;
			c.weight += w;
		}
	}

	for( size_t i = 0 ; i < C.size( ) ; i++ )
	{
		__clustering_data__ &c = C[ i ];
		double weight = c.weight > 0 ? c.weight : 1;
		c.x /= weight;
		c.y /= weight;
		c.w /= weight;
		c.h /= weight;
	}

	// Mean-Shiftクラスタリングを行う
	for( size_t i = 0 ; i < C.size( ) ; i++ )
	{
		__clustering_data__ &c = C[ i ];

		double err = 1.0e100;
		while( true )
		{
			__clustering_data__ cc;

			for( typename detection_list_type::const_iterator ite = list.begin( ) ; ite != list.end( ) ; ++ite )
			{
				if( c.overlap_ratio( static_cast< double >( ite->x ), static_cast< double >( ite->y ), static_cast< double >( ite->w ), static_cast< double >( ite->h ) ) > 0.2 )
				{
					double w = _use_weight_for_clustering_ ? ite->weight : 1.0;
					cc.x += ite->x * w;
					cc.y += ite->y * w;
					cc.w += ite->w * w;
					cc.h += ite->h * w;
					cc.weight += w;
				}
			}

			double weight = cc.weight > 0 ? cc.weight : 1;
			cc.x /= weight;
			cc.y /= weight;
			cc.w /= weight;
			cc.h /= weight;

			double e = std::abs( cc.x - c.x ) + std::abs( cc.y - c.y ) + std::abs( cc.w - c.w ) + std::abs( cc.h - c.h );
			if( err <= e )
			{
				break;
			}
			else
			{
				err = e;
				c = cc;
			}
		}
	}

	std::sort( C.begin( ), C.end( ) );

	// 枠の重なりが大きい物は除外する
	for( size_t i = 0 ; i < C.size( ) ; i++ )
	{
		__clustering_data__ &c0 = C[ i ];

		if( c0.weight <= 0 )
		{
			continue;
		}

		__clustering_data__ c = c0;
		c.x *= c.weight;
		c.y *= c.weight;
		c.w *= c.weight;
		c.h *= c.weight;

		for( size_t j = i + 1 ; j < C.size( ) ; j++ )
		{
			__clustering_data__ &c1 = C[ j ];

			if( c1.weight <= 0 )
			{
				continue;
			}

			double s = c0.overlap_ratio( c1 );

			if( s > overlap_threshold )
			{
				c.x += c1.x * c1.weight;
				c.y += c1.y * c1.weight;
				c.w += c1.w * c1.weight;
				c.h += c1.h * c1.weight;
				c.weight += c1.weight;
				c1.weight = -1;
			}
		}

		double weight = c.weight > 0 ? c.weight : 1;
		c.x /= weight;
		c.y /= weight;
		c.w /= weight;
		c.h /= weight;

		c0 = c;
	}

	{
		std::vector< __clustering_data__ > CTMP( C );
		C.clear( );
		for( size_t i = 0 ; i < CTMP.size( ) ; i++ )
		{
			if( CTMP[ i ].weight > 0 )
			{
				const __clustering_data__ &c = CTMP[ i ];
				C.push_back( __clustering_data__( c.x, c.y, c.w, c.h, 0, 0 ) );
			}
		}
	}

	// 個数を再カウントする
	for( typename detection_list_type::const_iterator ite = list.begin( ) ; ite != list.end( ) ; ++ite )
	{
		size_t indx = 0;
		double mlap = 0;
		for( size_t i = 0 ; i < C.size( ) ; i++ )
		{
			const __clustering_data__ &c = C[ i ];
			double s = c.overlap_ratio( static_cast< double >( ite->x ), static_cast< double >( ite->y ), static_cast< double >( ite->w ), static_cast< double >( ite->h ) );
			if( s > mlap )
			{
				mlap = s;
				indx = i;
			}
		}

		// 各枠の確信度を積算する
		C[ indx ].weight += ite->weight;
		C[ indx ].count++;

		if( C[ indx ].minw > ite->weight )
		{
			C[ indx ].minw = ite->weight;
		}
		if( C[ indx ].maxw < ite->weight )
		{
			C[ indx ].maxw = ite->weight;
		}
	}

	list.clear( );

	for( size_t i = 0 ; i < C.size( ) ; i++ )
	{
		__clustering_data__ &c = C[ i ];
		//if( c.weight >= threshold_for_detections )
		if( c.count >= threshold_for_detections )
		{
			list.push_back( validation_data( static_cast< int >( c.x ), static_cast< int >( c.y ), static_cast< int >( c.w ), static_cast< int >( c.h ), 1, c.weight, c.count, c.minw, c.maxw ) );
		}
	}
}


// AdaBoost名前空間の終わり
_ADABOOST_END


#endif	// __INCLUDE_ADABOOST_CLUSTERING__
