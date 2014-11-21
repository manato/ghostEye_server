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

#ifndef __INCLUDE_ADABOOST_FEATURE__
#define __INCLUDE_ADABOOST_FEATURE__

#ifndef __INCLUDE_ADABOOST_COMMON__
#include "common.h"
#endif

// AdaBoost名前空間の始まり
_ADABOOST_BEGIN

#define __FEATURE_TYPE_MASK__		0xff00
#define __FEATURE_FLAG_MASK__		0x00ff
#define __GET_FEATURE_FLAG__( t ) ( t & __FEATURE_FLAG_MASK__ )
#define __GET_FEATURE_TYPE__( t ) ( ( t & __FEATURE_TYPE_MASK__ ) >> 8 )
#define __PUT_FEATURE_TYPE__( t ) ( ( t << 8 ) & __FEATURE_TYPE_MASK__ )


#define __FEATURE_FLAG_NEW__		0x30	///< @brief 新規データを表すフラグ
#define __FEATURE_FLAG_VALID__		0xc0	///< @brief 学習に利用するデータを表すフラグ
#define __FEATURE_FLAG_POSITIVE__	0x01	///< @brief ポジティブサンプルを表すフラグ
#define __FEATURE_FLAG_DEFAULT__	( __FEATURE_FLAG_NEW__ | __FEATURE_FLAG_VALID__ | __FEATURE_FLAG_POSITIVE__ )	///< @brief 新規サンプルのデフォルトフラグ


typedef unsigned short feature_flag_type;

class empty_featre_idmap
{
private:
	size_t dim_;

public:
	empty_featre_idmap( size_t dim ) : dim_( dim ){ }

	size_t operator[]( const size_t indx ) const { return( indx ); }
	size_t size( ) const { return( dim_ ); }
};

/// @brief 機械学習で利用する２カテゴリの特徴量を管理するクラス（内部でのみ使用）
class feature_one
{
public:
	typedef size_t size_type;			///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef ptrdiff_t difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ

private:
	feature_value_type  value_;			///< @brief 特徴量の値
	feature_flag_type   flag_;			///< @brief 学習データのカテゴリ名（true か false）などを保存するフラグ
	feature_weight_type weight_;		///< @brief 学習データに対する重み

public:
	/// @brief デフォルトのコンストラクタ
	feature_one( ) : value_( 0 ), flag_( __FEATURE_FLAG_DEFAULT__ ), weight_( 0 )
	{
	}

	/// @brief 次元数を指定して特長量を初期化
	//! 
	//! @param[in]  val … 特徴量
	//! @param[in]  flg … フラグ
	//! @param[in]  wei … 重み
	//! 
	feature_one( feature_value_type val, feature_flag_type flg, feature_weight_type wei ) : value_( val ), flag_( flg ), weight_( wei )
	{
	}

	/// @brief コピーコンストラクタ
	feature_one( const feature_one &f ): value_( f.value_ ), flag_( f.flag_ ), weight_( f.weight_ )
	{
	}

	/// @brief 他の特徴量をコピーする
	feature_one &operator =( const feature_one &f )
	{
		if( &f != this )
		{
			value_  = f.value_;
			flag_   = f.flag_;
			weight_ = f.weight_;
		}

		return( *this );
	}

	/// @brief 他の特徴量と値の大小を比較する
	bool operator <( const feature_one &f ) const
	{
		return( value_ < f.value_ );
	}

	/// @brief 特徴量を取得
	feature_value_type value( ) const
	{
		return( value_ );
	}

	/// @brief 特徴量を設定
	void value( feature_value_type v )
	{
		value_ = v;
	}

	/// @brief 特徴量の重みを取得
	double weight( ) const
	{
		return( weight_ );
	}

	/// @brief 特徴量の重みを設定
	void weight( feature_weight_type w )
	{
		weight_ = w;
	}

	/// @brief サンプルの状態を表すフラグを取得
	feature_flag_type flag( ) const
	{
		return( flag_ );
	}

	/// @brief サンプルの状態を表すフラグを設定
	void flag( feature_flag_type f )
	{
		flag_ = f;
	}

	/// @brief 有効なサンプルであるかどうかを取得
	bool is_valid( ) const
	{
		return( ( flag_ & __FEATURE_FLAG_VALID__ ) != 0 );
	}

	/// @brief 有効なサンプルであるかどうかを設定
	void is_valid( bool value )
	{
		if( !value )
		{
			flag_ &= ~__FEATURE_FLAG_VALID__;
		}
		else
		{
			// 再度有効に設定された場合は新規データ扱いとする
			if( !is_valid( ) )
			{
				is_new( true );
			}

			flag_ |= __FEATURE_FLAG_VALID__;
		}
	}

	/// @brief 新規サンプルであるかどうかを取得
	bool is_new( ) const
	{
		return( ( flag_ & __FEATURE_FLAG_NEW__ ) != 0 );
	}

	/// @brief 新規サンプルであるかどうかを設定
	void is_new( bool value )
	{
		if( !value )
		{
			flag_ &= ~__FEATURE_FLAG_NEW__;
		}
		else
		{
			flag_ |= __FEATURE_FLAG_NEW__;
		}
	}

	/// @brief ポジティブサンプルであるかどうかを取得
	bool is_positive( ) const
	{
		return( ( flag_ & __FEATURE_FLAG_POSITIVE__ ) != 0 );
	}

	/// @brief ポジティブサンプルであるかどうかを設定
	void is_positive( bool value )
	{
		if( value )
		{
			flag_ |= __FEATURE_FLAG_POSITIVE__;
		}
		else
		{
			flag_ &= ~__FEATURE_FLAG_POSITIVE__;
		}
	}

	/// @brief ネガティブサンプルであるかどうかを取得
	bool is_negative( ) const
	{
		return( !is_positive( ) );
	}

	/// @brief ネガティブサンプルであるかどうかを設定
	void is_negative( bool value )
	{
		is_positive( !value );
	}

	/// @brief 新規サンプルであるかどうかを取得
	int type( ) const
	{
		return( __GET_FEATURE_TYPE__( flag_ ) );
	}

	/// @brief 新規サンプルであるかどうかを設定
	void type( int value )
	{
		flag_ = __GET_FEATURE_FLAG__( flag_ ) | __PUT_FEATURE_TYPE__( value );
	}
};

/// @brief 機械学習で利用する多次元の特徴量を管理するクラス
class feature : public std::vector< feature_value_type >
{
public:
	typedef std::vector< feature_value_type > base;	///< @brief 特徴量を保持する基底クラス
	typedef base::value_type value_type;			///< @brief MISTのコンテナ内に格納するデータ型．mist::array< data > の data と同じ
	typedef base::size_type size_type;				///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef base::difference_type difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ

private:
	feature_weight_type weight_;	///< @brief 学習データに対する重み
	feature_flag_type   flag_;		///< @brief 学習データのカテゴリ名（true か false）などを保存するフラグ

public:
	/// @brief デフォルトのコンストラクタ
	feature( ) : base( ), weight_( 0 ), flag_( __FEATURE_FLAG_DEFAULT__ )
	{
	}

	/// @brief 次元数を指定して特長量を初期化
	//! 
	//! @param[in]  dimension … 特徴量の次元
	//! 
	feature( size_type dimension ) : base( dimension ), weight_( 0 ), flag_( __FEATURE_FLAG_DEFAULT__ )
	{
	}

	/// @brief カテゴリ名を指定して特長量を初期化
	//! 
	//! @param[in]  cate      … カテゴリ名徴量の次元
	//! 
	feature( bool cate ) : base( ), weight_( 0 ), flag_( __FEATURE_FLAG_DEFAULT__ )
	{
		is_positive( cate );
	}

	/// @brief 次元数とカテゴリ名を指定して特長量を初期化
	//! 
	//! @param[in]  dimension … 特徴量の次元
	//! @param[in]  cate      … カテゴリ名徴量の次元
	//! 
	feature( size_type dimension, bool cate ) : base( dimension, 0 ), weight_( 0 ), flag_( __FEATURE_FLAG_DEFAULT__ )
	{
		is_positive( cate );
	}

	/// @brief コピーコンストラクタ
	feature( const feature &f ): base( f ), weight_( f.weight_ ), flag_( f.flag_ )
	{
	}

	/// @brief 他の特徴量をコピーする
	feature &operator =( const feature &f )
	{
		if( &f != this )
		{
			base::operator =( f );
			weight_ = f.weight_;
			flag_   = f.flag_;
		}

		return( *this );
	}

public:
	/// @brief 特徴量の重みを取得
	double weight( ) const
	{
		return( weight_ );
	}

	/// @brief 特徴量の重みを設定
	void weight( feature_weight_type w )
	{
		weight_ = w;
	}

	/// @brief サンプルの状態を表すフラグを取得
	feature_flag_type flag( ) const
	{
		return( flag_ );
	}

	/// @brief サンプルの状態を表すフラグを設定
	void flag( feature_flag_type f )
	{
		flag_ = f;
	}

	/// @brief 有効なサンプルであるかどうかを取得
	bool is_valid( ) const
	{
		return( ( flag_ & __FEATURE_FLAG_VALID__ ) != 0 );
	}

	/// @brief 有効なサンプルであるかどうかを設定
	void is_valid( bool value )
	{
		if( !value )
		{
			flag_ &= ~__FEATURE_FLAG_VALID__;
		}
		else
		{
			// 再度有効に設定された場合は新規データ扱いとする
			if( !is_valid( ) )
			{
				is_new( true );
			}

			flag_ |= __FEATURE_FLAG_VALID__;
		}
	}

	/// @brief 新規サンプルであるかどうかを取得
	bool is_new( ) const
	{
		return( ( flag_ & __FEATURE_FLAG_NEW__ ) != 0 );
	}

	/// @brief 新規サンプルであるかどうかを設定
	void is_new( bool value )
	{
		if( !value )
		{
			flag_ &= ~__FEATURE_FLAG_NEW__;
		}
		else
		{
			flag_ |= __FEATURE_FLAG_NEW__;
		}
	}

	/// @brief ポジティブサンプルであるかどうかを取得
	bool is_positive( ) const
	{
		return( ( flag_ & __FEATURE_FLAG_POSITIVE__ ) != 0 );
	}

	/// @brief ポジティブサンプルであるかどうかを設定
	void is_positive( bool value )
	{
		if( value )
		{
			flag_ |= __FEATURE_FLAG_POSITIVE__;
		}
		else
		{
			flag_ &= ~__FEATURE_FLAG_POSITIVE__;
		}
	}

	/// @brief ネガティブサンプルであるかどうかを取得
	bool is_negative( ) const
	{
		return( !is_positive( ) );
	}

	/// @brief ネガティブサンプルであるかどうかを設定
	void is_negative( bool value )
	{
		is_positive( !value );
	}

	/// @brief 新規サンプルであるかどうかを取得
	int type( ) const
	{
		return( __GET_FEATURE_TYPE__( flag_ ) );
	}

	/// @brief 新規サンプルであるかどうかを設定
	void type( int value )
	{
		flag_ = __GET_FEATURE_FLAG__( flag_ ) | __PUT_FEATURE_TYPE__( value );
	}

	/// @brief 特徴量の次元を取得
	size_type number_of_dimentions( ) const
	{
		return( base::size( ) );
	}
};

/// @brief 機械学習で利用する特徴量を参照経由でアクセスするためのクラス
class feature_ref
{
public:
	typedef feature feature_type;
	typedef feature_type::value_type value_type;			///< @brief MISTのコンテナ内に格納するデータ型．mist::array< data > の data と同じ
	typedef feature_type::size_type size_type;				///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef feature_type::difference_type difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ

private:
	feature_type *f_;	///< @brief 特徴量の参照元

public:
	/// @brief デフォルトのコンストラクタ
	feature_ref( ) : f_( NULL )
	{
	}

	/// @brief デフォルトのコンストラクタ
	feature_ref( feature_type &f ) : f_( &f )
	{
	}

public:
	/// @brief 指定したインデックスに対応する特徴量の値を取得
	//! 
	//! @param[in]  index … 特徴量のインデックス
	//! 
	const value_type &operator []( size_type index ) const
	{
		return( ( *f_ )[ index ] );
	}

	/// @brief 指定したインデックスに対応する特徴量の値を取得
	//! 
	//! @param[in]  index … 特徴量のインデックス
	//! 
	value_type &operator []( size_type index )
	{
		return( ( *f_ )[ index ] );
	}

public:
	/// @brief 特徴量の重みを取得
	double weight( ) const
	{
		return( f_->weight( ) );
	}

	/// @brief 特徴量の重みを設定
	void weight( feature_weight_type w )
	{
		f_->weight( w );
	}

	/// @brief サンプルの状態を表すフラグを取得
	feature_flag_type flag( ) const
	{
		return( f_->flag( ) );
	}

	/// @brief サンプルの状態を表すフラグを設定
	void flag( feature_flag_type f )
	{
		f_->flag( f );
	}

	/// @brief 有効なサンプルであるかどうかを取得
	bool is_valid( ) const
	{
		return( f_->is_valid( ) );
	}

	/// @brief 有効なサンプルであるかどうかを設定
	void is_valid( bool value )
	{
		f_->is_valid( value );
	}

	/// @brief 新規サンプルであるかどうかを取得
	bool is_new( ) const
	{
		return( f_->is_new( ) );
	}

	/// @brief 新規サンプルであるかどうかを設定
	void is_new( bool value )
	{
		f_->is_new( value );
	}

	/// @brief ポジティブサンプルであるかどうかを取得
	bool is_positive( ) const
	{
		return( f_->is_positive( ) );
	}

	/// @brief ポジティブサンプルであるかどうかを設定
	void is_positive( bool value )
	{
		f_->is_positive( value );
	}

	/// @brief ネガティブサンプルであるかどうかを取得
	bool is_negative( ) const
	{
		return( f_->is_negative( ) );
	}

	/// @brief ネガティブサンプルであるかどうかを設定
	void is_negative( bool value )
	{
		f_->is_negative( value );
	}

	/// @brief 新規サンプルであるかどうかを取得
	int type( ) const
	{
		return( f_->type( ) );
	}

	/// @brief 新規サンプルであるかどうかを設定
	void type( int value )
	{
		f_->type( value );
	}

	/// @brief 特徴量の次元を取得
	size_type number_of_dimentions( ) const
	{
		return( f_->size( ) );
	}
};

inline std::string get_feature_string( size_t feature_type )
{
	std::string str;
	switch( __GET_ADABOOST_FEATURE_TYPE__( feature_type ) )
	{
	case __ADABOOST_FEATURE_LRP__:
		str += "LRP";
		break;

	case __ADABOOST_FEATURE_MLBP__:
		str += "MLBP";
		break;

	case __ADABOOST_FEATURE_HAAR__:
	default:
		str += "HAAR";
		break;
	}

	return( str );
}

inline std::string get_color_string( size_t color_index )
{
	std::string str;

	switch( __GET_ADABOOST_FEATURE_COLOR__( color_index ) )
	{
	case __ADABOOST_COLOR2__:
		str = "Red";
		break;

	case __ADABOOST_COLOR3__:
		str = "Green";
		break;

	case __ADABOOST_COLOR4__:
		str = "Blue";
		break;

	case __ADABOOST_COLOR5__:
		str = "R/(R+G+B)";
		break;

	case __ADABOOST_COLOR6__:
		str = "G/(R+G+B)";
		break;

	case __ADABOOST_COLOR7__:
		str = "B/(R+G+B)";
		break;

	case __ADABOOST_COLOR8__:
		str = "log(R/G)";
		break;

	case __ADABOOST_COLOR9__:
		str = "log(B/G)";
		break;

	case __ADABOOST_COLOR10__:
		str = "log(R/B)";
		break;

	case __ADABOOST_COLOR1__:
	default:
		str = "Grayscale";
		break;
	}

	return( str );
}

struct feature_info_base
{
	typedef size_t size_type;			///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef ptrdiff_t difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ

	size_type mode;
	size_type x;
	size_type y;
	size_type w;
	size_type h;
	size_type sx;
	size_type sy;

	feature_info_base( size_type m = 0 ) : mode( m )
	{
	}

	feature_info_base( const feature_info_base &f ) : mode( f.mode )
	{
	}

	size_type number_of_bins( ) const { return( 0 ); }

	std::string get_pattern_string( ) const
	{
		return( "" );
	}

	std::string to_string( ) const
	{
		return( "[" + get_color_string( __GET_ADABOOST_FEATURE_COLOR__( mode ) ) + "]" );
	}
};

template < class INTEGRAL_IMAGE >
struct feature_calculator
{
	typedef INTEGRAL_IMAGE integral_image_type;
	typedef size_t size_type;						///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef ptrdiff_t difference_type;				///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ

	const std::vector< integral_image_type > &integral_images_;

	template < template < typename T, typename A > class Array, class Allocator, class FeatureInfo >
	feature_calculator( const std::vector< integral_image_type > &iimgs, const Array< FeatureInfo, Allocator > & /* flist */ ) : integral_images_( iimgs )
	{
	}

	void initialize( )
	{
	}

	template < class FeatureInfo >
	feature_value_type operator ()( const FeatureInfo &f, size_type offset_x = 0, size_type offset_y = 0, double scale = 1.0 ) const
	{
		return( compute_feature( integral_images_, f, offset_x, offset_y, scale ) );
	}

	template < template < typename T, typename A > class Array, class Allocator, class FeatureInfo >
	feature_value_type operator ()( size_type index, const Array< FeatureInfo, Allocator > &flist, size_type offset_x, size_type offset_y, double scale ) const
	{
		return( compute_feature( integral_images_, flist[ index ], offset_x, offset_y, scale ) );
	}

	template < template < typename T, typename A > class Array, class Allocator, class FeatureInfo, class FIDMap >
	feature_value_type operator ()( size_type index, const Array< FeatureInfo, Allocator > &flist, const FIDMap &fidmap, size_type offset_x, size_type offset_y, double scale ) const
	{
		return( compute_feature( integral_images_, flist[ fidmap[ index ] ], offset_x, offset_y, scale ) );
	}
};

template < class Classifier, class FeatureList >
inline unsigned int compute_used_colors_flag( const Classifier &classifier, const FeatureList &flist )
{
	bool used_colors[ __ADABOOST_NUMBER_OF_COLORS__ ];

	for( size_t i = 0 ; i < __ADABOOST_NUMBER_OF_COLORS__ ; i++ )
	{
		used_colors[ i ] = false;
	}

	// 次元削減をした場合は弱識別器のインデックスを振りなおす
#if defined( __ADABOOST_CASCADE_TYPE__ ) && __ADABOOST_CASCADE_TYPE__ != 0

#if defined( __ADABOOST_CASCADE_TYPE__ ) && __ADABOOST_CASCADE_TYPE__ == 3
	// 弱識別器に特徴量の情報を設定する
	for( size_t i = 0 ; i < classifier.classifiers( ).size( ) ; i++ )
	{
		used_colors[ __GET_ADABOOST_FEATURE_COLOR__( flist[ classifier.classifiers( )[ i ].index( ) ].mode ) ] = true;
	}
#else
	// 弱識別器に特徴量の情報を設定する
	for( size_type j = 0 ; j < classifier.classifiers( ).size( ) ; j++ )
	{
		for( size_type i = 0 ; i < classifier.classifiers( )[ j ].weaks( ).size( ) ; i++ )
		{
			used_colors[ __GET_ADABOOST_FEATURE_COLOR__( flist[ classifier.classifiers( )[ j ].weaks( )[ i ].index( ) ].mode ) ] = true;
		}
	}
#endif

#else
	for( size_type i = 0 ; i < classifier.weaks( ).size( ) ; i++ )
	{
		used_colors[ __GET_ADABOOST_FEATURE_COLOR__( flist[ classifier.weaks( )[ i ].index( ) ].mode ) ] = true;
	}
#endif

	unsigned int flag = 0, bit = 1;
	for( size_t i = 0 ; i < __ADABOOST_NUMBER_OF_COLORS__ ; i++ )
	{
		if( used_colors[ i ] )
		{
			flag |= bit;
		}

		bit <<= 1;
	}

	return( flag );
}

// AdaBoost名前空間の終わり
_ADABOOST_END

	
#if defined( __ADABOOST_FEATURE_TYPE__ )

	#if __ADABOOST_FEATURE_TYPE__ == 2
		#include "LRP.h"
	#elif __ADABOOST_FEATURE_TYPE__ == 3
		#include "MLBP.h"
	#else
		#include "HAAR.h"
	#endif

#endif


#endif	// __INCLUDE_ADABOOST_FEATURE__