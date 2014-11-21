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

#ifndef __INCLUDE_ADABOOST_COMMON__
#define __INCLUDE_ADABOOST_COMMON__

#define _ADABOOST_BEGIN		namespace adaboost{			///< @brief AdaBoost名前空間の始まり
#define _ADABOOST_END		}							///< @brief AdaBoost名前空間の始まり

#include <mist/mist.h>
#include <mist/config/color.h>
#include <mist/integral_image.h>
#include <mist/random.h>

#include <vector>

// AdaBoost名前空間の始まり
_ADABOOST_BEGIN

#define __ADABOOST_CLASSIFIER_VERSION__			__adabooster_version__( )	///< @brief 本プログラムのバージョンを示す文字列

#define __ADABOOST_OUTPUT_DEBUG_STRING__		1		///< @brief コンソールに学習時のデバッグ情報を出力するかどうか
#define __FEATURE_SCALING_FACTOR__				1.25	///< @brief スケール方向に何倍ずつ拡大しながら検出を行うか


/// @brief 使用する特徴量のタイプ
//! 
//!  1: Haar-Like特徴量
//!  2: LRP特徴量
//!  3: MLBP特徴量
//! 
#define __ADABOOST_FEATURE_TYPE__				3



#define __NUMBER_OF_LRP_RANKS__					2		///< @brief LRP特徴量で組み合わせるランクの数（2もしくは3のみ対応）
#define __USE_FAST_WEAK_LEARNER__				1		///< @brief ヒストグラムを使用した高速化を行うバージョン（ただし、特徴の表現力が下がる）
#define __USE_COLOR_FOR_COMPUTING_FEATURE__		3		///< @brief 色成分を特徴量計算に利用するかどうか（0: Grayscale, 1: RGB, 2: RGB+rgb, 3: Log Chromaticity+rgb+RGB）
#define __FEATURE_NORMALIZED_TRAINING__			1		///< @brief RealAdaBoostの弱識別器を学習する際に平滑化を施すかどうか
#define __SHRINK_IMAGE_WHEN_DETECTION__			0		///< @brief 検出を行う際にスケールに応じて検出枠の配置間隔を調整するかどうか（1にすると間隔を調整して検出速度が向上する）
//#define __ADABOOST_ASYMMETRIC_LOSS__			0.8		///< @brief RealAdaBoostの学習時にポジティブとネガティブでAsymmetricな学習を行うかどうか（0.5の場合はSymmetricな学習を行う）
#define __ROTATION_INVARIANT_MLBP__				0		///< @brief 回転不変なLBP特徴量を使用するかどうか（1: 回転不変のみ，2: 回転不変＋コントラスト）


#define __ADABOOST_CLASSIFIER_TYPE__			2		///< @brief Boostingの種類を指定する（0: Discrete, 1: Real, 2: Gentle）
#define __ADABOOST_CASCADE_TYPE__				3		///< @brief カスケードの種類を指定する（0: None, 1: Viola's Cascade, 2: Nested Cascade, 3: Multi-exit Cascade）


#if defined( __ADABOOST_CASCADE_TYPE__ ) && __ADABOOST_CASCADE_TYPE__ == 1

	#if defined( __ADABOOST_CLASSIFIER_TYPE__ ) && __ADABOOST_CLASSIFIER_TYPE__ == 1
		#define __ADABOOST_CLASSIFIER_STRING__			"Real AdaBoost (Viola's Cascade)"
		#define __DEFINE_ADABOOST_CLASSIFIER_TYPE__		typedef adaboost::real::cascaded_classifier adaboost_classifier_type;
	#elif defined( __ADABOOST_CLASSIFIER_TYPE__ ) && __ADABOOST_CLASSIFIER_TYPE__ == 2
		#define __ADABOOST_CLASSIFIER_STRING__			"Gentle AdaBoost (Viola's Cascade)"
		#define __DEFINE_ADABOOST_CLASSIFIER_TYPE__		typedef adaboost::gentle::cascaded_classifier adaboost_classifier_type;
	#else
		#define __ADABOOST_CLASSIFIER_STRING__			"Discrete AdaBoost (Viola's Cascade)"
		#define __DEFINE_ADABOOST_CLASSIFIER_TYPE__		typedef adaboost::discrete::cascaded_classifier adaboost_classifier_type;
	#endif

#elif defined( __ADABOOST_CASCADE_TYPE__ ) && __ADABOOST_CASCADE_TYPE__ == 2

	#if defined( __ADABOOST_CLASSIFIER_TYPE__ ) && __ADABOOST_CLASSIFIER_TYPE__ == 1
		#define __ADABOOST_CLASSIFIER_STRING__			"Real AdaBoost (Nested Cascade)"
		#define __DEFINE_ADABOOST_CLASSIFIER_TYPE__		typedef adaboost::real::cascaded_classifier adaboost_classifier_type;
	#elif defined( __ADABOOST_CLASSIFIER_TYPE__ ) && __ADABOOST_CLASSIFIER_TYPE__ == 2
		#define __ADABOOST_CLASSIFIER_STRING__			"Gentle AdaBoost (Nested Cascade)"
		#define __DEFINE_ADABOOST_CLASSIFIER_TYPE__		typedef adaboost::gentle::cascaded_classifier adaboost_classifier_type;
	#else
		#define __ADABOOST_CLASSIFIER_STRING__			"Discrete AdaBoost (Nested Cascade)"
		#define __DEFINE_ADABOOST_CLASSIFIER_TYPE__		typedef adaboost::discrete::cascaded_classifier adaboost_classifier_type;
	#endif

#elif defined( __ADABOOST_CASCADE_TYPE__ ) && __ADABOOST_CASCADE_TYPE__ == 3

	#if defined( __ADABOOST_CLASSIFIER_TYPE__ ) && __ADABOOST_CLASSIFIER_TYPE__ == 1
		#define __ADABOOST_CLASSIFIER_STRING__			"Real AdaBoost (Multi-exit Cascade)"
		#define __DEFINE_ADABOOST_CLASSIFIER_TYPE__		typedef adaboost::real::multi_exit_classifier adaboost_classifier_type;
	#elif defined( __ADABOOST_CLASSIFIER_TYPE__ ) && __ADABOOST_CLASSIFIER_TYPE__ == 2
		#define __ADABOOST_CLASSIFIER_STRING__			"Gentle AdaBoost (Multi-exit Cascade)"
		#define __DEFINE_ADABOOST_CLASSIFIER_TYPE__		typedef adaboost::gentle::multi_exit_classifier adaboost_classifier_type;
	#else
		#define __ADABOOST_CLASSIFIER_STRING__			"Discrete AdaBoost (Multi-exit Cascade)"
		#define __DEFINE_ADABOOST_CLASSIFIER_TYPE__		typedef adaboost::discrete::multi_exit_classifier adaboost_classifier_type;
	#endif

#else

	#if defined( __ADABOOST_CLASSIFIER_TYPE__ ) && __ADABOOST_CLASSIFIER_TYPE__ == 1
		#define __ADABOOST_CLASSIFIER_STRING__			"Real AdaBoost"
		#define __DEFINE_ADABOOST_CLASSIFIER_TYPE__		typedef adaboost::real::boosted_classifier adaboost_classifier_type;
	#elif defined( __ADABOOST_CLASSIFIER_TYPE__ ) && __ADABOOST_CLASSIFIER_TYPE__ == 2
		#define __ADABOOST_CLASSIFIER_STRING__			"Gentle AdaBoost"
		#define __DEFINE_ADABOOST_CLASSIFIER_TYPE__		typedef adaboost::gentle::boosted_classifier adaboost_classifier_type;
	#else
		#define __ADABOOST_CLASSIFIER_STRING__			"Discrete AdaBoost"
		#define __DEFINE_ADABOOST_CLASSIFIER_TYPE__		typedef adaboost::discrete::boosted_classifier adaboost_classifier_type;
	#endif

#endif


#if  defined( __USE_COLOR_FOR_COMPUTING_FEATURE__ ) && __USE_COLOR_FOR_COMPUTING_FEATURE__ == 1
	#define __ADABOOST_COLOR_STRING__			"RGB"
#elif defined( __USE_COLOR_FOR_COMPUTING_FEATURE__ ) && __USE_COLOR_FOR_COMPUTING_FEATURE__ == 2
	#define __ADABOOST_COLOR_STRING__			"RGB + Normalized RGB"
#elif defined( __USE_COLOR_FOR_COMPUTING_FEATURE__ ) && __USE_COLOR_FOR_COMPUTING_FEATURE__ == 3
	#define __ADABOOST_COLOR_STRING__			"RGB + Normalized RGB + Log Chromaticity Color"
#else
	#define __ADABOOST_COLOR_STRING__			"Grayscale"
#endif

#if defined( __ADABOOST_FEATURE_TYPE__ ) && __ADABOOST_FEATURE_TYPE__ == 1

	#define __ADABOOST_FEATURE_BASE_STRING__	"Haar-Like feature"
	#define __HISTOGRAM_CLASSIFIER_NUM_BINS__		512

#elif defined( __ADABOOST_FEATURE_TYPE__ ) && __ADABOOST_FEATURE_TYPE__ == 2

	#define __ADABOOST_FEATURE_BASE_STRING__	"Local Rank Pattern"

	#if defined( __NUMBER_OF_LRP_RANKS__ ) && __NUMBER_OF_LRP_RANKS__ == 2
		#define __HISTOGRAM_CLASSIFIER_NUM_BINS__ 81
	#elif defined( __NUMBER_OF_LRP_RANKS__ ) && __NUMBER_OF_LRP_RANKS__ == 3
		#define __HISTOGRAM_CLASSIFIER_NUM_BINS__ 729
	#endif

#elif defined( __ADABOOST_FEATURE_TYPE__ ) && __ADABOOST_FEATURE_TYPE__ == 3

	#define __ADABOOST_FEATURE_BASE_STRING__	"Multi-Block Local Binary Pattern"

	#if defined( __ROTATION_INVARIANT_MLBP__ ) && __ROTATION_INVARIANT_MLBP__ == 1
	#define __HISTOGRAM_CLASSIFIER_NUM_BINS__ 10
	#else
	#define __HISTOGRAM_CLASSIFIER_NUM_BINS__ 256
	#endif

#endif


// 特徴量を表す文字列
#define __ADABOOST_FEATURE_STRING__		__ADABOOST_FEATURE_BASE_STRING__ " (" __ADABOOST_COLOR_STRING__ ")"


//-------------------------------------------------------------------------------------------//
/// @brief 色特徴の総数
#define __ADABOOST_NUMBER_OF_COLORS__	16

/// @brief グレースケール
#define __ADABOOST_COLOR1__				0

/// @brief 赤色成分
#define __ADABOOST_COLOR2__				1

/// @brief 緑色成分
#define __ADABOOST_COLOR3__				2

/// @brief 青色成分
#define __ADABOOST_COLOR4__				3

/// @brief 赤色成分（正規化）
#define __ADABOOST_COLOR5__				4

/// @brief 緑色成分（正規化）
#define __ADABOOST_COLOR6__				5

/// @brief 青色成分（正規化）
#define __ADABOOST_COLOR7__				6

/// @brief log(R/G)
#define __ADABOOST_COLOR8__				7

/// @brief log(B/G)
#define __ADABOOST_COLOR9__				8

/// @brief log(R/B)
#define __ADABOOST_COLOR10__			9

#define __ADABOOST_COLOR11__			10
#define __ADABOOST_COLOR12__			11
#define __ADABOOST_COLOR13__			12
#define __ADABOOST_COLOR14__			13
#define __ADABOOST_COLOR15__			14
#define __ADABOOST_COLOR16__			15
//-------------------------------------------------------------------------------------------//


//-------------------------------------------------------------------------------------------//
/// @brief Haar-like 特徴量の種類を取得するためのビットマスク
#define __ADABOOST_FEATURE_TYPE_MASK__	0xff000000

/// @brief Haar-like 特徴量のパターンを取得するためのビットマスク
#define __ADABOOST_FEATURE_PATTERN_MASK__	0x00ffff00

/// @brief Haar-like 特徴量の色成分のインデックスを取得するためのビットマスク
#define __ADABOOST_FEATURE_COLOR_MASK__		0x000000ff

/// @brief Haar-like 特徴量のパターンを取得する
#define __GET_ADABOOST_FEATURE_TYPE__( t ) ( ( t & __ADABOOST_FEATURE_TYPE_MASK__ ) >> 24 )

/// @brief Haar-like 特徴量のパターンを取得する
#define __GET_ADABOOST_FEATURE_PATTERN__( p ) ( ( p & __ADABOOST_FEATURE_PATTERN_MASK__ ) >> 8 )

/// @brief Haar-like 特徴量の色成分のインデックスを取得する
#define __GET_ADABOOST_FEATURE_COLOR__( c ) ( c & __ADABOOST_FEATURE_COLOR_MASK__ )

/// @brief Haar-like 特徴量の色成分のインデックスのマスクを計算する
#define __ADABOOST_PATTERN__( t, p, c ) ( ( ( t << 24 ) & __ADABOOST_FEATURE_TYPE_MASK__ ) | ( ( p << 8 ) & __ADABOOST_FEATURE_PATTERN_MASK__ ) | ( c & __ADABOOST_FEATURE_COLOR_MASK__ ) )

/// @brief 特徴量の種類の総数
#define __ADABOOST_NUMBER_OF_FEATURES__		6
#define __ADABOOST_FEATURE_HAAR__			0
#define __ADABOOST_FEATURE_LRP__			1
#define __ADABOOST_FEATURE_MLBP__			2
//-------------------------------------------------------------------------------------------//



// 弱識別器の学習方法に応じて特徴量の型を変更する
#if defined( __USE_FAST_WEAK_LEARNER__ ) && __USE_FAST_WEAK_LEARNER__ == 1
	// ヒストグラムを利用した高速な弱識別器の学習を行う場合
	#if defined( __HISTOGRAM_CLASSIFIER_NUM_BINS__ ) && __HISTOGRAM_CLASSIFIER_NUM_BINS__ <= 256
		typedef unsigned char feature_value_type;	///< @brief 機械学習で利用する特徴量を表現するデータ型
	#else
		typedef short feature_value_type;			///< @brief 機械学習で利用する特徴量を表現するデータ型
	#endif
	typedef double feature_weight_type;				///< @brief 機械学習で利用するサンプルの重み表現するデータ型
#else
	// 通常の学習方法
	typedef double feature_value_type;				///< @brief 機械学習で利用する特徴量を表現するデータ型
	typedef double feature_weight_type;				///< @brief 機械学習で利用するサンプルの重み表現するデータ型
#endif

typedef mist::array2< unsigned char >		image_type;
typedef mist::array2< float >				float_image_type;
typedef mist::rgb< unsigned char >			color_type;
typedef mist::array2< color_type >			color_image_type;
typedef mist::integral_image< image_type >	integral_image_type;


/// @brief 本プロジェクトのバージョンを返す関数
std::string __adabooster_version__( );


// 色変換関連の関数をインポートする
#include "color.h"


inline void convert_image( const color_image_type &in, std::vector< integral_image_type > &out )
{
	out.clear( );

#if defined( __ADABOOST_FEATURE_TYPE__ ) && __ADABOOST_FEATURE_TYPE__ == 5

	out.insert( out.end( ), 9, integral_image_type( ) );

	construct_cslbp_integral_image( in, out );

#elif defined( __ADABOOST_FEATURE_TYPE__ ) && __ADABOOST_FEATURE_TYPE__ == 12

	out.insert( out.end( ), 10, integral_image_type( ) );

	construct_gray_integral_image( in, out[ 0 ] );
	construct_cslbp_integral_image( in, out );

#elif defined( __USE_COLOR_FOR_COMPUTING_FE0TURE__ ) && __USE_COLOR_FOR_COMPUTING_FEATURE__ == 1

	out.insert( out.end( ), 4, integral_image_type( ) );

	construct_gray_integral_image( in, out[ 0 ] );
	construct_rgb_integral_image( in, out[ 1 ], out[ 2 ], out[ 3 ] );

#elif defined( __USE_COLOR_FOR_COMPUTING_FEATURE__ ) && __USE_COLOR_FOR_COMPUTING_FEATURE__ == 2

	out.insert( out.end( ), 7, integral_image_type( ) );

	construct_gray_integral_image( in, out[ 0 ] );
	construct_rgb_integral_image( in, out[ 1 ], out[ 2 ], out[ 3 ] );
	construct_normalized_rgb_integral_image( in, out[ 4 ], out[ 5 ], out[ 6 ] );

#elif defined( __USE_COLOR_FOR_COMPUTING_FEATURE__ ) && __USE_COLOR_FOR_COMPUTING_FEATURE__ == 3

	out.insert( out.end( ), 10, integral_image_type( ) );

	construct_gray_integral_image( in, out[ 0 ] );
	construct_rgb_integral_image( in, out[ 1 ], out[ 2 ], out[ 3 ] );
	construct_normalized_rgb_integral_image( in, out[ 4 ], out[ 5 ], out[ 6 ] );
	construct_log_chromaticity_integral_image( in, out[ 7 ], out[ 8 ], out[ 9 ] );

#else

	out.push_back( integral_image_type( ) );
	construct_gray_integral_image( in, out.back( ) );

#endif
}

// AdaBoost名前空間の終わり
_ADABOOST_END

#endif	// __INCLUDE_ADABOOST_COMMON__