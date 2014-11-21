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

#ifndef __INCLUDE_MIST_COLOREX_H__
#define __INCLUDE_MIST_COLOREX_H__

namespace __color_utility__
{
	template < bool >
	struct __normalized_color__
	{
		template < class T >
		static void compute( const T &r, const T &g, const T &b, T &R, T &G, T &B )
		{
			T sum = r + g + b;

			sum = sum > 0 ? 1 / sum : 1;

			R = r * sum;
			G = g * sum;
			B = b * sum;
		}
	};

	template < >
	struct __normalized_color__< false >
	{
		template < class T >
		static void compute( const T &r, const T &g, const T &b, T &R, T &G, T &B )
		{
			int sum = r + g + b;

			sum = sum > 0 ? ( 255 << 10 ) / sum : ( 255 << 10 );

			R = ( r * sum ) >> 10;
			G = ( g * sum ) >> 10;
			B = ( b * sum ) >> 10;
		}
	};

	template < bool >
	struct __log_chromaticity_color__
	{
		template < class T >
		static void compute( const T &r, const T &g, const T &b, T &V1, T &V2, T &V3 )
		{
			double rr = std::log( 1.0 + r );
			double gg = std::log( 1.0 + g );
			double bb = std::log( 1.0 + b );

			V1 = rr - gg;
			V2 = bb - gg;
			V3 = rr - bb;
		}
	};

	template < >
	struct __log_chromaticity_color__< false >
	{
		template < class T >
		static void compute( const T &r, const T &g, const T &b, T &V1, T &V2, T &V3 )
		{
			const double __range__ = std::log( 256.0 );
			double rr = std::log( 1.0 + r );
			double gg = std::log( 1.0 + g );
			double bb = std::log( 1.0 + b );

			V1 = static_cast< T >( ( rr - gg + __range__ ) / ( __range__ * 2.0 ) * 255.0 + 0.5 );
			V2 = static_cast< T >( ( bb - gg + __range__ ) / ( __range__ * 2.0 ) * 255.0 + 0.5 );
			V3 = static_cast< T >( ( rr - bb + __range__ ) / ( __range__ * 2.0 ) * 255.0 + 0.5 );
		}
	};
}

template < class ColorImage, class IntegralImage >
inline void construct_gray_integral_image( const ColorImage &in, IntegralImage &out )
{
	typedef typename ColorImage::value_type color_pixel_type;
	typedef typename color_pixel_type::value_type pixel_type;
	typedef typename ColorImage::template rebind< pixel_type >::other temp_image_type;

	temp_image_type tmp( in.width( ), in.height( ), in.reso1( ), in.reso2( ) );

	int size = static_cast< int >( in.size( ) );

	#pragma omp parallel for
	for( int i = 0 ; i < size ; i++ )
	{
		tmp[ i ] = static_cast< pixel_type >( in[ i ].get_value( ) );
	}

	out.construct_integral_array( tmp );
}

template < class ColorImage, class IntegralImage >
inline void construct_rgb_integral_image( const ColorImage &in, IntegralImage &R, IntegralImage &G, IntegralImage &B )
{
	typedef typename ColorImage::value_type color_pixel_type;
	typedef typename color_pixel_type::value_type pixel_type;
	typedef typename ColorImage::template rebind< pixel_type >::other temp_image_type;

	temp_image_type red( in.width( ), in.height( ), in.reso1( ), in.reso2( ) );
	temp_image_type green( in.width( ), in.height( ), in.reso1( ), in.reso2( ) );
	temp_image_type blue( in.width( ), in.height( ), in.reso1( ), in.reso2( ) );

	int size = static_cast< int >( in.size( ) );

	#pragma omp parallel
	{
		#pragma omp for
		for( int i = 0 ; i < size ; i++ )
		{
			const color_pixel_type &p = in[ i ];

			red[ i ]   = static_cast< pixel_type >( p.r );
			green[ i ] = static_cast< pixel_type >( p.g );
			blue[ i ]  = static_cast< pixel_type >( p.b );
		}

		#pragma omp sections
		{
			#pragma omp section
			R.construct_integral_array( red );
			#pragma omp section
			G.construct_integral_array( green );
			#pragma omp section
			B.construct_integral_array( blue );
		}
	}
}

template < class ColorImage, class IntegralImage >
inline void construct_normalized_rgb_integral_image( const ColorImage &in, IntegralImage &nR, IntegralImage &nG, IntegralImage &nB )
{
	typedef typename ColorImage::value_type color_pixel_type;
	typedef typename color_pixel_type::value_type pixel_type;
	typedef typename ColorImage::template rebind< pixel_type >::other temp_image_type;
	typedef __color_utility__::__normalized_color__< mist::is_float< pixel_type >::value > __color_normalizer__;

	temp_image_type nr( in.width( ), in.height( ), in.reso1( ), in.reso2( ) );
	temp_image_type ng( in.width( ), in.height( ), in.reso1( ), in.reso2( ) );
	temp_image_type nb( in.width( ), in.height( ), in.reso1( ), in.reso2( ) );

	int size = static_cast< int >( in.size( ) );

	#pragma omp parallel
	{
		#pragma omp for
		for( int i = 0 ; i < size ; i++ )
		{
			const color_pixel_type &p = in[ i ];

			__color_normalizer__::compute( p.r, p.g, p.b, nr[ i ], ng[ i ], nb[ i ] );
		}

		#pragma omp sections
		{
			#pragma omp section
			nR.construct_integral_array( nr );
			#pragma omp section
			nG.construct_integral_array( ng );
			#pragma omp section
			nB.construct_integral_array( nb );
		}
	}
}

template < class ColorImage, class IntegralImage >
inline void construct_log_chromaticity_integral_image( const ColorImage &in, IntegralImage &O1, IntegralImage &O2, IntegralImage &O3 )
{
	typedef typename ColorImage::value_type color_pixel_type;
	typedef typename color_pixel_type::value_type pixel_type;
	typedef typename ColorImage::template rebind< pixel_type >::other temp_image_type;
	typedef __color_utility__::__log_chromaticity_color__< mist::is_float< pixel_type >::value > __color_normalizer__;

	temp_image_type o1( in.width( ), in.height( ), in.reso1( ), in.reso2( ) );
	temp_image_type o2( in.width( ), in.height( ), in.reso1( ), in.reso2( ) );
	temp_image_type o3( in.width( ), in.height( ), in.reso1( ), in.reso2( ) );

	int size = static_cast< int >( in.size( ) );

	#pragma omp parallel
	{
		#pragma omp for
		for( int i = 0 ; i < size ; i++ )
		{
			const color_pixel_type &p = in[ i ];

			__color_normalizer__::compute( p.r, p.g, p.b, o1[ i ], o2[ i ], o3[ i ] );
		}

		#pragma omp sections
		{
			#pragma omp section
			O1.construct_integral_array( o1 );
			#pragma omp section
			O2.construct_integral_array( o2 );
			#pragma omp section
			O3.construct_integral_array( o3 );
		}
	}
}

template < class ColorImage, class IntegralImage >
inline void construct_cslbp_integral_image( const ColorImage &in, std::vector< IntegralImage > &iimgs )
{
	typedef typename ColorImage::value_type color_pixel_type;
	typedef typename color_pixel_type::value_type pixel_type;
	typedef typename ColorImage::template rebind< unsigned int >::other temp_image_type;

	temp_image_type lbps[ 9 ], tmp( in.width( ) + 2, in.height( ) + 2 );

	for( int i = 0 ; i < 9 ; i++ )
	{
		lbps[ i ].resize( in.width( ), in.height( ) );
	}

	#pragma omp parallel for
	for( int j = 0 ; j < static_cast< int >( in.height( ) ) ; j++ )
	{
		for( size_t i = 0 ; i < in.width( ) ; i++ )
		{
			tmp( i + 1, j + 1 ) = static_cast< unsigned int >( in( i, j ).get_value( ) );
		}
	}

	#pragma omp parallel for
	for( int j = 0 ; j < static_cast< int >( tmp.height( ) ) ; j++ )
	{
		tmp( 0, j ) = tmp( 1, j );
		tmp( tmp.width( ) - 1, j ) = tmp( tmp.width( ) - 2, j );
	}

	#pragma omp parallel for
	for( int i = 0 ; i < static_cast< int >( tmp.width( ) ) ; i++ )
	{
		tmp( i, 0 ) = tmp( i, 1 );
		tmp( i, tmp.height( ) - 1 ) = tmp( i, tmp.height( ) - 2 );
	}

	const unsigned int th = 3;

	#pragma omp parallel for
	for( int j = 1 ; j < static_cast< int >( tmp.height( ) ) - 1 ; j++ )
	{
		for( size_t i = 1 ; i < tmp.width( ) - 1 ; i++ )
		{
			const double _1_2 = 0.70710678118654752440084436210485;
#if 1
			double p0 = tmp( i + 1, j );
			double p1 = tmp( i, j - 1 );
			double p2 = tmp( i - 1, j );
			double p3 = tmp( i, j + 1 );
			double dx = p0 - p2;
			double dy = p3 - p1;

			double val = std::sqrt( dx * dx + dy * dy ) * _1_2;
#else
			mist::rgb< double > p0 = in( i + 1, j );
			mist::rgb< double > p1 = in( i, j - 1 );
			mist::rgb< double > p2 = in( i - 1, j );
			mist::rgb< double > p3 = in( i, j + 1 );
			mist::rgb< double > dx = p0 - p2;
			mist::rgb< double > dy = p3 - p1;

			double dxR = dx.r;
			double dyR = dy.r;
			double dxG = dx.g;
			double dyG = dy.g;
			double dxB = dx.b;
			double dyB = dy.b;
			double nR  = dxR * dxR + dyR * dyR;
			double nG  = dxG * dxG + dyG * dyG;
			double nB  = dxB * dxB + dyB * dyB;

			double val;
			if( nR >= nG && nR >= nB )
			{
				val = std::sqrt( nR ) * _1_2;
			}
			else if( nG >= nR && nG >= nB )
			{
				val = std::sqrt( nG ) * _1_2;
			}
			else
			{
				val = std::sqrt( nB ) * _1_2;
			}
#endif

			unsigned int g[ 8 ];
			const unsigned int tbl[ 16 ] = {
				0, //0000 uniform
				1, //0001 uniform
				8, //0010
				2, //0011 uniform
				8, //0100
				8, //0101
				8, //0110
				3, //0111 uniform
				4, //1000 uniform
				8, //1001
				8, //1010
				8, //1011
				5, //1100 uniform
				8, //1101
				6, //1110 uniform
				7, //1111 uniform
			};

			g[ 0 ] = tmp( i + 1, j     );
			g[ 1 ] = tmp( i + 1, j - 1 );
			g[ 2 ] = tmp( i    , j - 1 );
			g[ 3 ] = tmp( i - 1, j - 1 );
			g[ 4 ] = tmp( i - 1, j     );
			g[ 5 ] = tmp( i - 1, j + 1 );
			g[ 6 ] = tmp( i    , j + 1 );
			g[ 7 ] = tmp( i + 1, j + 1 );

			size_t lbp = 0;

			lbp |= ( g[ 0 ] >= g[ 4 ] + th ) ? 1 : 0;
			lbp |= ( g[ 1 ] >= g[ 5 ] + th ) ? 2 : 0;
			lbp |= ( g[ 2 ] >= g[ 6 ] + th ) ? 4 : 0;
			lbp |= ( g[ 3 ] >= g[ 7 ] + th ) ? 8 : 0;

			lbps[ tbl[ lbp ] ]( i - 1, j - 1 ) = static_cast< unsigned int >( val * 0.787 );
			lbps[ tbl[ ( ( lbp << 1 ) & 0x000e ) | ( ( lbp >> 3 ) & 0x0001 ) ] ]( i - 1, j - 1 ) = static_cast< unsigned int >( val * 0.106  );
			lbps[ tbl[ ( ( lbp << 2 ) & 0x000c ) | ( ( lbp >> 2 ) & 0x0003 ) ] ]( i - 1, j - 1 ) = static_cast< unsigned int >( val * 0.0005 );
			lbps[ tbl[ ( ( lbp >> 1 ) & 0x0007 ) | ( ( lbp << 3 ) & 0x0008 ) ] ]( i - 1, j - 1 ) = static_cast< unsigned int >( val * 0.106  );
			lbps[ tbl[ ( ( lbp >> 2 ) & 0x0003 ) | ( ( lbp << 2 ) & 0x000c ) ] ]( i - 1, j - 1 ) = static_cast< unsigned int >( val * 0.0005 );
		}
	}

	#pragma omp parallel for
	for( int i = 0 ; i < 9 ; i++ )
	{
		iimgs[ iimgs.size( ) - i - 1 ].construct_integral_array( lbps[ i ] );
	}

	//for( int i = 0 ; i < 9 ; i++ )
	//{
	//	char buff[ 256 ];
	//	sprintf( buff, "%d.bmp", i );
	//	mist::write_bmp( lbps[ i ], buff );
	//}
}

#endif // __INCLUDE_MIST_COLOR_H__
