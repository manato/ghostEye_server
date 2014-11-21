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

#ifndef __INCLUDE_ADABOOST_CLASSIFIER_BASE__
#define __INCLUDE_ADABOOST_CLASSIFIER_BASE__


#ifndef __INCLUDE_ADABOOST_COMMON__
#include "common.h"
#endif

#ifndef __INCLUDE_ADABOOST_FEATURE__
#include "feature.h"
#endif


// AdaBoost名前空間の始まり
_ADABOOST_BEGIN

struct empty_info_functor
{
	std::string operator ()( size_t /* indx */ ) const { return( std::string( ) ); }
};

class empty_classifier
{
private:
	bool is_positive_;

public:
	empty_classifier( bool is_positive = true ) : is_positive_( is_positive )
	{
	}

	/// @brief 他の弱識別器のデータをコピーする
	empty_classifier& operator =( const empty_classifier& o )
	{
		if( this != &o )
		{
			is_positive_ = o.is_positive_;
		}

		return( *this );
	}

public:
	/// @brief 学習済みの識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f … 分類する特徴量
	//! 
	//! @return 分類結果
	//! 
	template < class FEATURE >
	bool operator ()( const FEATURE &f ) const
	{
		return( evaluate( f ) >= 0 );
	}

	/// @brief 学習済みの識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f … 特徴量を計算する関数オブジェクト
	//! @param[in]  p … 特徴量計算に使用するパラメータ
	//! 
	//! @return 分類結果
	//! 
	template < class FUNCTOR, class PARAMS >
	bool operator ()( const FUNCTOR &f, const PARAMS &p ) const
	{
		return( evaluate( p, f ) >= 0 );
	}

	/// @brief 学習済みの弱識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2 >
	bool operator ()( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2 ) const
	{
		return( evaluate( f, p1, p2 ) >= 0 );
	}

	/// @brief 学習済みの弱識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  p3 … 特徴量計算に使用するパラメータ3
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3 >
	bool operator ()( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3 ) const
	{
		return( evaluate( f, p1, p2, p3 ) >= 0 );
	}

	/// @brief 学習済みの弱識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  p3 … 特徴量計算に使用するパラメータ3
	//! @param[in]  p4 … 特徴量計算に使用するパラメータ4
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4 >
	bool operator ()( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, const PARAM4 &p4 ) const
	{
		return( evaluate( f, p1, p2, p3, p4 ) >= 0 );
	}

	/// @brief 学習済みの弱識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  p3 … 特徴量計算に使用するパラメータ3
	//! @param[in]  p4 … 特徴量計算に使用するパラメータ4
	//! @param[in]  p5 … 特徴量計算に使用するパラメータ5
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5 >
	bool operator ()( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, const PARAM4 &p4, const PARAM5 &p5 ) const
	{
		return( evaluate( f, p1, p2, p3, p4, p5 ) >= 0 );
	}

	/// @brief 学習済みの識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f … 分類する特徴量
	//! 
	//! @return 分類結果
	//! 
	template < class FEATURE >
	double evaluate( const FEATURE &f ) const
	{
		return( is_positive_ ? 1.0 : -1.0 );
	}

	/// @brief 学習済みの識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f … 特徴量を計算する関数オブジェクト
	//! @param[in]  p … 特徴量計算に使用するパラメータ
	//! 
	//! @return 分類結果
	//! 
	template < class FUNCTOR, class PARAMS >
	double evaluate( const FUNCTOR &f, const PARAMS &p ) const
	{
		return( is_positive_ ? 1.0 : -1.0 );
	}

	/// @brief 学習済みの識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! 
	//! @return 分類結果
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2 >
	double evaluate( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2 ) const
	{
		return( is_positive_ ? 1.0 : -1.0 );
	}

	/// @brief 学習済みの識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  p3 … 特徴量計算に使用するパラメータ3
	//! 
	//! @return 分類結果
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3 >
	double evaluate( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3 ) const
	{
		return( is_positive_ ? 1.0 : -1.0 );
	}

	/// @brief 学習済みの識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  p3 … 特徴量計算に使用するパラメータ3
	//! @param[in]  p4 … 特徴量計算に使用するパラメータ4
	//! 
	//! @return 分類結果
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4 >
	double evaluate( const FUNCTOR & /* f */, const PARAM1 & /* p1 */, const PARAM2 & /* p2 */, const PARAM3 & /* p3 */, const PARAM4 & /* p4 */ ) const
	{
		return( is_positive_ ? 1.0 : -1.0 );
	}

	/// @brief 学習済みの識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  p3 … 特徴量計算に使用するパラメータ3
	//! @param[in]  p4 … 特徴量計算に使用するパラメータ4
	//! @param[in]  p5 … 特徴量計算に使用するパラメータ5
	//! 
	//! @return 分類結果
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5 >
	double evaluate( const FUNCTOR & /* f */, const PARAM1 & /* p1 */, const PARAM2 & /* p2 */, const PARAM3 & /* p3 */, const PARAM4 & /* p4 */, const PARAM5 & /* p4 */ ) const
	{
		return( is_positive_ ? 1.0 : -1.0 );
	}
};


/// @brief AdaBoost で利用するしきい値処理を用いた弱識別器
class classifier_base
{
public:
	typedef feature::value_type      value_type;		///< @brief MISTのコンテナ内に格納するデータ型．mist::array< data > の data と同じ
	typedef feature::size_type       size_type;			///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef feature::difference_type difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ

private:
	std::string comment_;
	double positive_weight_;
	double negative_weight_;

public:
	/// @brief デフォルトのコンストラクタ
	classifier_base( ) : comment_( ), positive_weight_( 0.5 ), negative_weight_( 0.5 )
	{
	}

	/// @brief コピーコンストラクタ
	classifier_base( const classifier_base & w ) : comment_( w.comment_ ), positive_weight_( w.positive_weight_ ), negative_weight_( w.negative_weight_ )
	{
	}

	/// @brief 他の弱識別器のデータをコピーする
	classifier_base& operator =( const classifier_base& o )
	{
		if( this != &o )
		{
			comment_         = o.comment_;
			positive_weight_ = o.positive_weight_;
			negative_weight_ = o.negative_weight_;
		}

		return( *this );
	}

public:
	/// @brief 識別器に対して設定されたコメントを取得する
	std::string comment( ) const { return( comment_ ); }

	/// @brief 識別器に対してコメントを設定する
	//! 
	//! @param[in] c … コメント
	//! 
	void comment( const std::string &c ){ comment_ = c; }

	/// @brief 識別器の構築に使用するポジティブサンプルの重みを取得する
	double positive_weight( ) const { return( positive_weight_ ); }

	/// @brief 識別器の構築に使用するポジティブサンプルの重みを設定する
	//! 
	//! @param[in] w … 重み
	//! 
	void positive_weight( const double w ){ positive_weight_ = w; }

	/// @brief 識別器の構築に使用するネガティブサンプルの重みを取得する
	double negative_weight( ) const { return( negative_weight_ ); }

	/// @brief 識別器の構築に使用するネガティブサンプルの重みを設定する
	//! 
	//! @param[in] w … 重み
	//! 
	void negative_weight( const double w ){ negative_weight_ = w; }

	/// @brief 識別器の情報を保存したXMLファイルから設定を復元する
	//! 
	//! @param[in] filename … 識別器の情報を保存したXMLファイル名
	//! 
	virtual bool load( const std::string & /* filename */ ){ return( false ); }

	/// @brief 識別器の情報をXML形式でファイルに保存する
	//! 
	//! @param[in] filename … 識別器の情報を保存するXMLファイル名
	//! 
	virtual bool save( const std::string & /* filename */, const std::string & /* comment */ = "" ) const { return( false ); }
};


/// @brief AdaBoost で利用するしきい値処理を用いた弱識別器
template < class DERIVED_CLASSIFIER >
class weak_classifier_base : public classifier_base
{
private:
	typedef classifier_base				base;				///< @brief 基底クラス
	typedef DERIVED_CLASSIFIER			derived_type;		///< @brief 派生クラス

public:
	typedef feature::value_type			value_type;			///< @brief MISTのコンテナ内に格納するデータ型．mist::array< data > の data と同じ
	typedef feature::size_type			size_type;			///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef feature::difference_type	difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ

	size_type index_;										///< @brief 使用する特徴量の番号
	double weight_;											///< @brief 弱識別器の重み
	double lambda_c_;										///< @brief オンライン学習で使用する重み（正しく分類できたもの）
	double lambda_w_;										///< @brief オンライン学習で使用する重み（誤って分類されたもの）

public:
	/// @brief デフォルトのコンストラクタ
	weak_classifier_base( size_type indx = 0 ) : index_( indx ), weight_( 0.0 ), lambda_c_( 0.0 ), lambda_w_( 0.0 )
	{
	}

	/// @brief コピーコンストラクタ
	weak_classifier_base( const weak_classifier_base &w ) : base( w ), index_( w.index_ ), weight_( w.weight_ ), lambda_c_( w.lambda_c_ ), lambda_w_( w.lambda_w_ )
	{
	}

	/// @brief 弱識別器が使用する特徴量のインデックスを取得する
	size_type index( ) const { return( index_ ); }

	/// @brief 弱識別器が使用する特徴量のインデックスを設定する
	void index( size_type i ){ index_ = i; }

	/// @brief 弱識別器の重みを取得
	double weight( ) const { return( weight_ ); }

	/// @brief 弱識別器の重みを設定
	void weight( double w ){ weight_ = w; }


	/// @brief オンライン学習で使用する重み（正しく分類できたもの）を設定する関数
	//! 
	//! @param[in]  value … 確率密度関数へ設定する値
	//! 
	void lambda_c( double value )
	{
		lambda_c_ = value;
	}

	/// @brief オンライン学習で使用する重み（正しく分類できたもの）を取得する関数
	//! 
	double lambda_c( ) const
	{
		return( lambda_c_ );
	}

	/// @brief オンライン学習で使用する重み（誤って分類されたもの）を設定する関数
	//! 
	//! @param[in]  value … 確率密度関数へ設定する値
	//! 
	void lambda_w( double value )
	{
		lambda_w_ = value;
	}

	/// @brief オンライン学習で使用する重み（誤って分類されたもの）を取得する関数
	//! 
	double lambda_w( ) const
	{
		return( lambda_w_ );
	}

	/// @brief 他の弱識別器のデータをコピーする
	weak_classifier_base& operator =( const weak_classifier_base& o )
	{
		if( this != &o )
		{
			base::operator =( o );

			index_    = o.index_;
			weight_   = o.weight_;
			lambda_c_ = o.lambda_c_;
			lambda_w_ = o.lambda_w_;
		}

		return( *this );
	}

public:
	/// @brief 学習済みの弱識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f … 分類する特徴量ベクトル
	//! 
	template < class FEATURE >
	double operator ()( const FEATURE &f ) const
	{
		return( evaluate( f ) );
	}

	/// @brief 学習済みの弱識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f … 分類する特徴量
	//! 
	double operator ()( double f ) const
	{
		return( evaluate( f ) );
	}

	/// @brief 学習済みの弱識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f … 特徴量を計算する関数オブジェクト
	//! @param[in]  p … 特徴量計算に使用するパラメータ
	//! 
	template < class FUNCTOR, class PARAMS >
	double operator ()( const FUNCTOR &f, const PARAMS &p ) const
	{
		return( evaluate( f, p ) );
	}

	/// @brief 学習済みの弱識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2 >
	double operator ()( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2 ) const
	{
		return( evaluate( f, p1, p2 ) );
	}

	/// @brief 学習済みの弱識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  p3 … 特徴量計算に使用するパラメータ3
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3 >
	double operator ()( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3 ) const
	{
		return( evaluate( f, p1, p2, p3 ) );
	}

	/// @brief 学習済みの弱識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  p3 … 特徴量計算に使用するパラメータ3
	//! @param[in]  p4 … 特徴量計算に使用するパラメータ4
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4 >
	double operator ()( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, const PARAM4 &p4 ) const
	{
		return( evaluate( f, p1, p2, p3, p4 ) );
	}

	/// @brief 学習済みの弱識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  p3 … 特徴量計算に使用するパラメータ3
	//! @param[in]  p4 … 特徴量計算に使用するパラメータ4
	//! @param[in]  p5 … 特徴量計算に使用するパラメータ5
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5 >
	double operator ()( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, const PARAM4 &p4, const PARAM5 &p5 ) const
	{
		return( evaluate( f, p1, p2, p3, p4, p5 ) );
	}

	/// @brief 学習済みの弱識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f … 分類する特徴量ベクトル
	//! 
	template < class FEATURE >
	double evaluate( const FEATURE &f ) const
	{
		return( ( ( const derived_type * ) this )->evaluate_( f[ index_ ] ) );
	}

	/// @brief 学習済みの弱識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f … 分類する特徴量
	//! 
	double evaluate( double f ) const
	{
		return( ( ( const derived_type * ) this )->evaluate_( f ) );
	}

	/// @brief 学習済みの弱識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f … 特徴量を計算する関数オブジェクト
	//! @param[in]  p … 特徴量計算に使用するパラメータ
	//! 
	template < class FUNCTOR, class PARAMS >
	double evaluate( const FUNCTOR &f, const PARAMS &p ) const
	{
		return( ( ( const derived_type * ) this )->evaluate_( f( index_, p ) ) );
	}

	/// @brief 学習済みの弱識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2 >
	double evaluate( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2 ) const
	{
		return( ( ( const derived_type * ) this )->evaluate_( f( index_, p1, p2 ) ) );
	}

	/// @brief 学習済みの弱識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  p3 … 特徴量計算に使用するパラメータ3
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3 >
	double evaluate( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3 ) const
	{
		return( ( ( const derived_type * ) this )->evaluate_( f( index_, p1, p2, p3 ) ) );
	}

	/// @brief 学習済みの弱識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  p3 … 特徴量計算に使用するパラメータ3
	//! @param[in]  p4 … 特徴量計算に使用するパラメータ4
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4 >
	double evaluate( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, const PARAM4 &p4 ) const
	{
		return( ( ( const derived_type * ) this )->evaluate_( f( index_, p1, p2, p3, p4 ) ) );
	}

	/// @brief 学習済みの弱識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  p3 … 特徴量計算に使用するパラメータ3
	//! @param[in]  p4 … 特徴量計算に使用するパラメータ4
	//! @param[in]  p5 … 特徴量計算に使用するパラメータ5
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5 >
	double evaluate( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, const PARAM4 &p4, const PARAM5 &p5 ) const
	{
		return( ( ( const derived_type * ) this )->evaluate_( f( index_, p1, p2, p3, p4, p5 ) ) );
	}

	/// @brief 学習済みの弱識別器を用いて特徴量を分類する
	double evaluate_( double f ) const
	{
		return( f >= 0 ? weight( ) : -weight( ) );
	}

	/// @brief 追加の教師データを用いて弱識別器を更新する
	//! 
	//! @param[in]  f … 分類する特徴量ベクトル
	//! @param[in]  weight … 学習する重み
	//! 
	template < class FEATURE >
	void update( const FEATURE &f, const double weight, bool is_positive )
	{
		return( ( ( derived_type * ) this )->update_( f[ index_ ], weight, is_positive ) );
	}

	/// @brief 追加の教師データを用いて弱識別器を更新する
	//! 
	//! @param[in]  f … 分類する特徴量
	//! @param[in]  weight … 学習する重み
	//! 
	void update( double f, const double weight, bool is_positive )
	{
		return( ( ( derived_type * ) this )->update_( f, weight, is_positive ) );
	}

	/// @brief 追加の教師データを用いて弱識別器を更新する
	//! 
	//! @param[in]  f … 特徴量を計算する関数オブジェクト
	//! @param[in]  p … 特徴量計算に使用するパラメータ
	//! @param[in]  weight … 学習する重み
	//! 
	template < class FUNCTOR, class PARAMS >
	void update( const FUNCTOR &f, const PARAMS &p, const double weight, bool is_positive )
	{
		return( ( ( derived_type * ) this )->update_( f( index_, p ), weight, is_positive ) );
	}

	/// @brief 追加の教師データを用いて弱識別器を更新する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  weight … 学習する重み
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2 >
	void update( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const double weight, bool is_positive )
	{
		return( ( ( derived_type * ) this )->update_( f( index_, p1, p2 ), weight, is_positive ) );
	}

	/// @brief 追加の教師データを用いて弱識別器を更新する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  p3 … 特徴量計算に使用するパラメータ3
	//! @param[in]  weight … 学習する重み
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3 >
	void update( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, const double weight, bool is_positive )
	{
		return( ( ( derived_type * ) this )->update_( f( index_, p1, p2, p3 ), weight, is_positive ) );
	}

	/// @brief 追加の教師データを用いて弱識別器を更新する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  p3 … 特徴量計算に使用するパラメータ3
	//! @param[in]  p4 … 特徴量計算に使用するパラメータ4
	//! @param[in]  weight … 学習する重み
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4 >
	void update( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, const PARAM4 &p4, const double weight, bool is_positive )
	{
		return( ( ( derived_type * ) this )->update_( f( index_, p1, p2, p3, p4 ), weight, is_positive ) );
	}

	/// @brief 追加の教師データを用いて弱識別器を更新する
	//! 
	//! @param[in]  f      … 追加の教師データ
	//! @param[in]  weight … 学習する重み
	//! 
	void update_( double f, const double weight, bool is_positive )
	{
	}
};


/// @brief Boostingによる学習を行う強識別器の基底クラス
template < class DERIVED_CLASSIFIER, class BASE_WEAK_CLASSIFIER >
class boosted_classifier_base : public classifier_base
{
private:
	typedef classifier_base						base;					///< @brief 基底クラス
	typedef DERIVED_CLASSIFIER                  derived_type;			///< @brief 派生クラス

public:	
	typedef feature::value_type                 value_type;				///< @brief MISTのコンテナ内に格納するデータ型．mist::array< data > の data と同じ
	typedef feature::size_type                  size_type;				///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef feature::difference_type            difference_type;		///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef BASE_WEAK_CLASSIFIER                base_classifier_type;	///< @brief AdaBoostで利用する弱識別器
	typedef std::vector< base_classifier_type > classifier_list_type;	///< @brief AdaBoostで利用する弱識別器のリスト

protected:
	classifier_list_type	classifiers_;	///< @brief 弱識別器のリスト
	double					threshold_;		///< @brief 識別に使用するしきい値
	std::string				info_;			///< @brief 識別器を学習した際の情報を保持する文字列
	std::string				tag_;			///< @brief ユーザーが独自に設定可能な文字列

public:
	/// @brief デフォルトのコンストラクタ
	boosted_classifier_base( ) : threshold_( 0.0 ), info_( "" ){}

	/// @brief コピーコンストラクタ
	boosted_classifier_base( const boosted_classifier_base &b ) : base( b ), classifiers_( b.classifiers_ ), threshold_( b.threshold_ ), info_( b.info_ ), tag_( b.tag_ ){}

	/// @brief デストラクタ
	~boosted_classifier_base( ){}

	/// @brief 他のAdaBoost識別器の情報をコピーする
	boosted_classifier_base& operator =( const boosted_classifier_base& o )
	{
		if( this != &o )
		{
			base::operator =( o );

			classifiers_ = o.classifiers_;
			threshold_   = o.threshold_;
			info_        = o.info_;
			tag_         = o.tag_;
		}

		return( *this );
	}

public:
	/// @brief 弱識別器の識別境界を決定する際に使用するしきい値を取得する
	double threshold( ) const { return( threshold_ ); }
	/// @brief 弱識別器の識別境界を決定する際に使用するしきい値を設定する
	void threshold( double threshold ){ threshold_ = threshold; }

	/// @brief 識別器に対してユーザーが設定可能な情報を表す文字列を取得する
	std::string tag( ) const { return( tag_ ); }
	/// @brief 識別器に対してユーザーが設定可能な情報を表す文字列を設定する
	void tag( const std::string &str ){ tag_ = str; }

	/// @brief 識別器を学習した際の情報を保持する文字列を取得する
	std::string info( ) const { return( info_ ); }
	/// @brief 識別器を学習した際の情報を保持する文字列を設定する
	void info( const std::string &str ){ info_ = str; }

	/// @brief AdaBoost識別器を構築する弱識別器の数を取得する
	size_type number_of_classifiers( ) const { return( classifiers_.size( ) ); }

	/// @brief AdaBoost識別器を構築する弱識別器の数を取得する
	size_type total_number_of_classifiers( ) const { return( classifiers_.size( ) ); }

	/// @brief 識別器を構築する弱識別器のリストを取得する
	classifier_list_type & classifiers( ){ return( classifiers_ ); }

	/// @brief 識別器を構築する弱識別器のリストを取得する
	const classifier_list_type & classifiers( ) const { return( classifiers_ ); }

	/// @brief 新しく弱識別器を追加する
	bool add_classifier( const base_classifier_type &weak )
	{
		classifiers_.push_back( weak );

		return( true );
	}

public:
	/// @brief 教師データを用いてAdaBoostアルゴリズムにより強識別器を構築する
	//! 
	//! @param[in,out]  features                   … 学習に用いる教師データ
	//! @param[in]      num_max_base_classifiers   … 強識別器の構築に使用する弱識別器の最大数（デフォルトは100）
	//! @param[in]      min_true_positive_rate     … 強識別器の構築時に満たすべきTPの最小値（デフォルトは-1）．負の値を指定した場合は num_max_weak_classifiers まで弱識別器を追加する．
	//! @param[in]      max_false_positive_rate    … 強識別器の構築時に満たすべきFPの最大値（デフォルトは-1）．負の値を指定した場合は num_max_weak_classifiers まで弱識別器を追加する．
	//! @param[in]      _output_debug_string_      … 学習の進捗状況を出力するかどうか（デフォルトはfalse）
	//! 
	template < template < typename, typename > class FEATURE_LIST, class FEATURE, class ALLOCATOR >
	bool learn( FEATURE_LIST< FEATURE, ALLOCATOR > & features, size_type num_max_base_classifiers = 100, double min_true_positive_rate = -1.0, double max_false_positive_rate = -1.0, bool _output_debug_string_ = false )
	{
		if( !initialize( features ) )
		{
			return( false );
		}

		// 識別器を構築する
		add_classifiers( features, num_max_base_classifiers, min_true_positive_rate, max_false_positive_rate, _output_debug_string_ );

		return( true );
	}

	/// @brief AdaBoost識別器の設定を初期化する
	bool initialize( )
	{
		// 強識別器を初期化する
		classifiers_.clear( );

		// しきい値を初期化する
		threshold_ = 0.0;

		return( true );
	}

	/// @brief 識別器の設定と教師データを初期化する
	//! 
	//! @param[in,out]  features … 学習に用いる教師データ
	//! 
	template < template < typename, typename > class FEATURE_LIST, class FEATURE, class ALLOCATOR >
	bool initialize( FEATURE_LIST< FEATURE, ALLOCATOR > & features )
	{
		typedef FEATURE feature_type;
		typedef typename FEATURE_LIST< FEATURE, ALLOCATOR >::iterator iterator;

		if( features.empty( ) )
		{
			return( false );
		}

		// 強識別器を初期化する
		initialize( );

		// 重みを初期化する
		size_type positives = 0;
		size_type negatives = 0;
		for( iterator ite = features.begin( ) ; ite != features.end( ) ; ++ite )
		{
			const feature_type &f = *ite;
			if( f.is_valid( ) )
			{
				if( f.is_positive( ) )
				{
					positives++;
				}
				else
				{
					negatives++;
				}
			}
		}

		double posw = 0.5 / static_cast< double >( positives );
		double negw = 0.5 / static_cast< double >( negatives );

		base::positive_weight( posw );
		base::negative_weight( negw );

		for( iterator ite = features.begin( ) ; ite != features.end( ) ; ++ite )
		{
			feature_type &f = *ite;
			if( f.is_valid( ) )
			{
				f.weight( f.is_positive( ) ? posw : negw );
			}
		}

		return( true );
	}

	/// @brief 現在の強識別器に弱識別器を追加する
	//! 
	//! @param[in,out]  features                   … 学習に用いる教師データ
	//! @param[in]      num_max_base_classifiers   … 強識別器へ追加する弱識別器の数（デフォルトは100）
	//! @param[in]      min_true_positive_rate     … 強識別器の構築時に満たすべきTPの最小値（デフォルトは-1）．負の値を指定した場合は num_max_base_classifiers まで弱識別器を追加する．
	//! @param[in]      max_false_positive_rate    … 強識別器の構築時に満たすべきFPの最大値（デフォルトは-1）．負の値を指定した場合は num_max_base_classifiers まで弱識別器を追加する．
	//! @param[in]      _output_debug_string_      … 学習の進捗状況を出力するかどうか（デフォルトはfalse）
	//! 
	template < template < typename, typename > class FEATURE_LIST, class FEATURE, class ALLOCATOR >
	bool add_classifiers( FEATURE_LIST< FEATURE, ALLOCATOR > & features, size_type num_max_base_classifiers = 100, double min_true_positive_rate = -1.0, double max_false_positive_rate = -1.0, bool _output_debug_string_ = false )
	{
		return( add_classifiers( features, empty_info_functor( ), num_max_base_classifiers, min_true_positive_rate, max_false_positive_rate, _output_debug_string_ ) );
	}

	/// @brief 現在の強識別器に弱識別器を追加する
	//! 
	//! @param[in,out]  features                   … 学習に用いる教師データ
	//! @param[in]      num_max_base_classifiers   … 強識別器へ追加する弱識別器の数（デフォルトは100）
	//! @param[in]      min_true_positive_rate     … 強識別器の構築時に満たすべきTPの最小値（デフォルトは-1）．負の値を指定した場合は num_max_base_classifiers まで弱識別器を追加する．
	//! @param[in]      max_false_positive_rate    … 強識別器の構築時に満たすべきFPの最大値（デフォルトは-1）．負の値を指定した場合は num_max_base_classifiers まで弱識別器を追加する．
	//! @param[in]      _output_debug_string_      … 学習の進捗状況を出力するかどうか（デフォルトはfalse）
	//! 
	template < template < typename, typename > class FEATURE_LIST, class FEATURE, class ALLOCATOR, class INFOFUNCTOR >
	bool add_classifiers( FEATURE_LIST< FEATURE, ALLOCATOR > & features, INFOFUNCTOR finfo, size_type num_max_base_classifiers, double min_true_positive_rate, double max_false_positive_rate, bool _output_debug_string_ )
	{
		// 派生クラス側で処理させる
		return( ( ( derived_type * ) this )->add_classifiers( features, finfo, num_max_base_classifiers, min_true_positive_rate, max_false_positive_rate, _output_debug_string_ ) );
	}

public:
	/// @brief 学習済みの識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f … 分類する特徴量
	//! 
	//! @return 分類結果
	//! 
	template < class FEATURE >
	bool operator ()( const FEATURE &f ) const
	{
		return( ( ( const derived_type * ) this )->evaluate( f ) >= 0.0 );
	}

	/// @brief 学習済みの識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f … 特徴量を計算する関数オブジェクト
	//! @param[in]  p … 特徴量計算に使用するパラメータ
	//! 
	//! @return 分類結果
	//! 
	template < class FUNCTOR, class PARAMS >
	bool operator ()( const FUNCTOR &f, const PARAMS &p ) const
	{
		return( ( ( const derived_type * ) this )->evaluate( f, p ) >= 0.0 );
	}

	/// @brief 学習済みの弱識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2 >
	bool operator ()( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2 ) const
	{
		return( ( ( const derived_type * ) this )->evaluate( f, p1, p2 ) >= 0.0 );
	}

	/// @brief 学習済みの弱識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  p3 … 特徴量計算に使用するパラメータ3
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3 >
	bool operator ()( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3 ) const
	{
		return( ( ( const derived_type * ) this )->evaluate( f, p1, p2, p3 ) >= 0.0 );
	}

	/// @brief 学習済みの弱識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  p3 … 特徴量計算に使用するパラメータ3
	//! @param[in]  p4 … 特徴量計算に使用するパラメータ4
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4 >
	bool operator ()( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, const PARAM4 &p4 ) const
	{
		return( ( ( const derived_type * ) this )->evaluate( f, p1, p2, p3, p4 ) >= 0.0 );
	}

	/// @brief 学習済みの弱識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  p3 … 特徴量計算に使用するパラメータ3
	//! @param[in]  p4 … 特徴量計算に使用するパラメータ4
	//! @param[in]  p5 … 特徴量計算に使用するパラメータ5
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5 >
	bool operator ()( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, const PARAM4 &p4, const PARAM5 &p5 ) const
	{
		return( ( ( const derived_type * ) this )->evaluate( f, p1, p2, p3, p4, p5 ) >= 0.0 );
	}

	/// @brief 学習済みの識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f … 分類する特徴量
	//! 
	//! @return 分類結果
	//! 
	template < class FEATURE >
	double evaluate( const FEATURE &f ) const
	{
		double value = 0.0;

		for( size_type t = 0 ; t < classifiers_.size( ) ; t++ )
		{
			const base_classifier_type &weak = classifiers_[ t ];
			double v = weak.evaluate( f );
			value += v > 0 ? v : 0;
		}

		return( threshold_ <= value ? 1.0 : -1.0 );
	}

	/// @brief 学習済みの識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f … 特徴量を計算する関数オブジェクト
	//! @param[in]  p … 特徴量計算に使用するパラメータ
	//! 
	//! @return 分類結果
	//! 
	template < class FUNCTOR, class PARAMS >
	double evaluate( const FUNCTOR &f, const PARAMS &p ) const
	{
		double value = 0.0;

		for( size_type t = 0 ; t < classifiers_.size( ) ; t++ )
		{
			const base_classifier_type &weak = classifiers_[ t ];
			double v = weak.evaluate( f, p );
			value += v > 0 ? v : 0;
		}

		return( threshold_ <= value ? 1.0 : -1.0 );
	}

	/// @brief 学習済みの識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! 
	//! @return 分類結果
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2 >
	double evaluate( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2 ) const
	{
		double value = 0.0;

		for( size_type t = 0 ; t < classifiers_.size( ) ; t++ )
		{
			const base_classifier_type &weak = classifiers_[ t ];
			double v = weak.evaluate( f, p1, p2 );
			value += v > 0 ? v : 0;
		}

		return( threshold_ <= value ? 1.0 : -1.0 );
	}

	/// @brief 学習済みの識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  p3 … 特徴量計算に使用するパラメータ3
	//! 
	//! @return 分類結果
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3 >
	double evaluate( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3 ) const
	{
		double value = 0.0;

		for( size_type t = 0 ; t < classifiers_.size( ) ; t++ )
		{
			const base_classifier_type &weak = classifiers_[ t ];
			double v = weak.evaluate( f, p1, p2, p3 );
			value += v > 0 ? v : 0;
		}

		return( threshold_ <= value ? 1.0 : -1.0 );
	}

	/// @brief 学習済みの識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  p3 … 特徴量計算に使用するパラメータ3
	//! @param[in]  p4 … 特徴量計算に使用するパラメータ4
	//! 
	//! @return 分類結果
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4 >
	double evaluate( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, const PARAM4 &p4 ) const
	{
		double value = 0.0;

		for( size_type t = 0 ; t < classifiers_.size( ) ; t++ )
		{
			const base_classifier_type &weak = classifiers_[ t ];
			double v = weak.evaluate( f, p1, p2, p3, p4 );
			value += v > 0 ? v : 0;
		}

		return( threshold_ <= value ? 1.0 : -1.0 );
	}

	/// @brief 学習済みの識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  p3 … 特徴量計算に使用するパラメータ3
	//! @param[in]  p4 … 特徴量計算に使用するパラメータ4
	//! @param[in]  p5 … 特徴量計算に使用するパラメータ5
	//! 
	//! @return 分類結果
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5 >
	double evaluate( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, const PARAM4 &p4, const PARAM5 &p5 ) const
	{
		double value = 0.0;

		for( size_type t = 0 ; t < classifiers_.size( ) ; t++ )
		{
			const base_classifier_type &weak = classifiers_[ t ];
			double v = weak.evaluate( f, p1, p2, p3, p4, p5 );
			value += v > 0 ? v : 0;
		}

		return( threshold_ <= value ? 1.0 : -1.0 );
	}
	
public:
	/// @brief 追加の教師データを用いて弱識別器を更新する
	//! 
	//! @param[in]  f           … 分類する特徴量ベクトル
	//! @param[in]  is_positive … ポジティブサンプルかどうか
	//! @param[in]  lambda      … 更新に用いる重みの初期値
	//! 
	template < class FEATURE >
	double update( const FEATURE &f, bool is_positive, double lambda = 1.0 )
	{
		double weight = is_positive ? base::positive_weight( ) : base::negative_weight( );
		for( size_type t = 0 ; t < classifiers_.size( ) ; t++ )
		{
			base_classifier_type &weak = classifiers_[ t ];

			weak.update( f, lambda * weight, is_positive );

			double v = weak.evaluate( f );
			if( ( is_positive ? +1.0 : -1.0 ) * v >= 0.0 )
			{
				weak.lambda_c( weak.lambda_c( ) + lambda * std::abs( v ) );

				double err = weak.lambda_w( ) / ( weak.lambda_c( ) + weak.lambda_w( ) );
				lambda /= 2.0 * ( 1.0 - err );
			}
			else
			{
				weak.lambda_w( weak.lambda_w( ) + lambda * std::abs( v ) );

				double err = weak.lambda_w( ) / ( weak.lambda_c( ) + weak.lambda_w( ) );
				lambda /= 2.0 * err;
			}
		}

		return( lambda );
	}

	/// @brief 追加の教師データを用いて弱識別器を更新する
	//! 
	//! @param[in]  f … 特徴量を計算する関数オブジェクト
	//! @param[in]  p … 特徴量計算に使用するパラメータ
	//! @param[in]  is_positive … ポジティブサンプルかどうか
	//! @param[in]  lambda      … 更新に用いる重みの初期値
	//! 
	template < class FUNCTOR, class PARAMS >
	double update( const FUNCTOR &f, const PARAMS &p, bool is_positive, double lambda = 1.0 )
	{
		double weight = is_positive ? base::positive_weight( ) : base::negative_weight( );
		for( size_type t = 0 ; t < classifiers_.size( ) ; t++ )
		{
			base_classifier_type &weak = classifiers_[ t ];

			weak.update( f, p, lambda * weight, is_positive );

			double v = weak.evaluate( f, p );
			if( ( is_positive ? +1.0 : -1.0 ) * v >= 0.0 )
			{
				weak.lambda_c( weak.lambda_c( ) + lambda * std::abs( v ) );

				double err = weak.lambda_w( ) / ( weak.lambda_c( ) + weak.lambda_w( ) );
				lambda /= 2.0 * ( 1.0 - err );
			}
			else
			{
				weak.lambda_w( weak.lambda_w( ) + lambda * std::abs( v ) );

				double err = weak.lambda_w( ) / ( weak.lambda_c( ) + weak.lambda_w( ) );
				lambda /= 2.0 * err;
			}
		}

		return( lambda );
	}

	/// @brief 追加の教師データを用いて弱識別器を更新する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  is_positive … ポジティブサンプルかどうか
	//! @param[in]  lambda      … 更新に用いる重みの初期値
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2 >
	double update( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, bool is_positive, double lambda = 1.0 )
	{
		double weight = is_positive ? base::positive_weight( ) : base::negative_weight( );
		for( size_type t = 0 ; t < classifiers_.size( ) ; t++ )
		{
			base_classifier_type &weak = classifiers_[ t ];

			weak.update( f, p1, p2, lambda * weight, is_positive );

			double v = weak.evaluate( f, p1, p2 );
			if( ( is_positive ? +1.0 : -1.0 ) * v >= 0.0 )
			{
				weak.lambda_c( weak.lambda_c( ) + lambda * std::abs( v ) );

				double err = weak.lambda_w( ) / ( weak.lambda_c( ) + weak.lambda_w( ) );
				lambda /= 2.0 * ( 1.0 - err );
			}
			else
			{
				weak.lambda_w( weak.lambda_w( ) + lambda * std::abs( v ) );

				double err = weak.lambda_w( ) / ( weak.lambda_c( ) + weak.lambda_w( ) );
				lambda /= 2.0 * err;
			}
		}

		return( lambda );
	}

	/// @brief 追加の教師データを用いて弱識別器を更新する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  p3 … 特徴量計算に使用するパラメータ3
	//! @param[in]  is_positive … ポジティブサンプルかどうか
	//! @param[in]  lambda      … 更新に用いる重みの初期値
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3 >
	double update( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, bool is_positive, double lambda = 1.0 )
	{
		double weight = is_positive ? base::positive_weight( ) : base::negative_weight( );
		for( size_type t = 0 ; t < classifiers_.size( ) ; t++ )
		{
			base_classifier_type &weak = classifiers_[ t ];

			weak.update( f, p1, p2, p3, lambda * weight, is_positive );

			double v = weak.evaluate( f, p1, p2, p3 );
			if( ( is_positive ? +1.0 : -1.0 ) * v >= 0.0 )
			{
				weak.lambda_c( weak.lambda_c( ) + lambda * std::abs( v ) );

				double err = weak.lambda_w( ) / ( weak.lambda_c( ) + weak.lambda_w( ) );
				lambda /= 2.0 * ( 1.0 - err );
			}
			else
			{
				weak.lambda_w( weak.lambda_w( ) + lambda * std::abs( v ) );

				double err = weak.lambda_w( ) / ( weak.lambda_c( ) + weak.lambda_w( ) );
				lambda /= 2.0 * err;
			}
		}

		return( lambda );
	}

	/// @brief 追加の教師データを用いて弱識別器を更新する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  p3 … 特徴量計算に使用するパラメータ3
	//! @param[in]  p4 … 特徴量計算に使用するパラメータ4
	//! @param[in]  is_positive … ポジティブサンプルかどうか
	//! @param[in]  lambda      … 更新に用いる重みの初期値
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4 >
	double update( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, const PARAM4 &p4, bool is_positive, double lambda = 1.0 )
	{
		double weight = is_positive ? base::positive_weight( ) : base::negative_weight( );
		for( size_type t = 0 ; t < classifiers_.size( ) ; t++ )
		{
			base_classifier_type &weak = classifiers_[ t ];

			weak.update( f, p1, p2, p3, p4, lambda * weight, is_positive );

			double v = weak.evaluate( f, p1, p2, p3, p4 );
			if( ( is_positive ? +1.0 : -1.0 ) * v >= 0.0 )
			{
				weak.lambda_c( weak.lambda_c( ) + lambda * std::abs( v ) );

				double err = weak.lambda_w( ) / ( weak.lambda_c( ) + weak.lambda_w( ) );
				lambda /= 2.0 * ( 1.0 - err );
			}
			else
			{
				weak.lambda_w( weak.lambda_w( ) + lambda * std::abs( v ) );

				double err = weak.lambda_w( ) / ( weak.lambda_c( ) + weak.lambda_w( ) );
				lambda /= 2.0 * err;
			}
		}

		return( lambda );
	}

public:
	/// @brief 特徴量のリストからTP率を計算する
	//! 
	//! @param[in]  features             … TP率の計算に使用する特徴量のリスト
	//! @param[in]  force_calculate_rate … 無効に設定されている特徴量も計算に使用するかどうか（デフォルトはfalse）
	//! 
	template < template < typename, typename > class FEATURE_LIST, class FEATURE, class ALLOCATOR >
	double true_positive_rate( const FEATURE_LIST< FEATURE, ALLOCATOR > & features, bool force_calculate_rate = false )
	{
		if( features.empty( ) )
		{
			return( 0.0 );
		}

		size_type positives  = 0;
		size_type detections = 0;

		typename FEATURE_LIST< FEATURE, ALLOCATOR >::const_iterator ite = features.begin( );
		for( ; ite != features.end( ) ; ++ite )
		{
			const FEATURE &f = *ite;
			if( force_calculate_rate || f.is_valid( ) )
			{
				if( f.is_positive( ) )
				{
					positives++;

					if( ( ( const derived_type * ) this )->evaluate( f ) >= 0 )
					{
						detections++;
					}
				}
			}
		}

		return( static_cast< double >( detections ) / static_cast< double >( positives > 0 ? positives : 1 ) );
	}

	/// @brief 特徴量のリストからFP率を計算する
	//! 
	//! @param[in]  features             … FP率の計算に使用する特徴量のリスト
	//! @param[in]  force_calculate_rate … 無効に設定されている特徴量も計算に使用するかどうか（デフォルトはfalse）
	//! 
	template < template < typename, typename > class FEATURE_LIST, class FEATURE, class ALLOCATOR >
	double false_positive_rate( const FEATURE_LIST< FEATURE, ALLOCATOR > & features, bool force_calculate_rate = false )
	{
		if( features.empty( ) )
		{
			return( 1.0 );
		}

		size_t negatives       = 0;
		size_t miss_detections = 0;

		typename FEATURE_LIST< FEATURE, ALLOCATOR >::const_iterator ite = features.begin( );
		for( ; ite != features.end( ) ; ++ite )
		{
			const FEATURE &f = *ite;
			if( force_calculate_rate || f.is_valid( ) )
			{
				if( f.is_negative( ) )
				{
					negatives++;

					if( ( ( const derived_type * ) this )->evaluate( f ) >= 0 )
					{
						miss_detections++;
					}
				}
			}
		}

		return( static_cast< double >( miss_detections ) / static_cast< double >( negatives > 0 ? negatives : 1 ) );
	}
};


/// @brief 強識別器をカスケード接続した強識別器の基底クラス
template < class DERIVED_CLASSIFIER, class BASE_STRONG_CLASSIFIER >
class cascaded_classifier_base : public classifier_base
{
private:
	typedef classifier_base						base;					///< @brief 基底クラス
	typedef DERIVED_CLASSIFIER                  derived_type;			///< @brief 派生クラス

public:
	typedef BASE_STRONG_CLASSIFIER              base_classifier_type;	///< @brief カスケードにする識別器
	typedef std::vector< base_classifier_type > classifier_list_type;	///< @brief カスケードにする識別器のリスト
	typedef feature::value_type                 value_type;				///< @brief MISTのコンテナ内に格納するデータ型．mist::array< data > の data と同じ
	typedef feature::size_type                  size_type;				///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef feature::difference_type            difference_type;		///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ

protected:
	classifier_list_type	classifiers_;				///< @brief AdaBoost識別器のリスト
	std::string				tag_;						///< @brief ユーザーが独自に設定可能な文字列

	double minimum_true_positive_rate_per_stage_;		///< @brief 各ステージで満たすべきTP率の下限
	double maximum_false_positive_rate_per_stage_;		///< @brief 各ステージで満たすべきFP率の上限
	double total_false_positive_rate_;					///< @brief カスケード型識別器の構築時の終了条件となる全体としてのFP率
	size_type maximum_number_of_classifiers_per_stage_;	///< @brief 構築するカスケードの最大段数
	double last_true_positive_rate_;					///< @brief カスケード型識別器の最後の学習を行った時点での全体のTP率
	double last_false_positive_rate_[ 4 ];				///< @brief カスケード型識別器の最後の学習を行った時点での全体のFP率

public:
	/// @brief 各ステージで満たすべきTP率の下限を取得
	double minimum_true_positive_rate_per_stage( )  const { return( minimum_true_positive_rate_per_stage_ ); }
	/// @brief 各ステージで満たすべきFP率の上限を取得
	double maximum_false_positive_rate_per_stage( ) const { return( maximum_false_positive_rate_per_stage_ ); };
	/// @brief 学習の終了条件となる全体としてのFP率を取得
	double total_false_positive_rate( )             const { return( total_false_positive_rate_ ); };
	/// @brief 構築するカスケードの最大段数を取得
	size_type maximum_number_of_classifiers_per_stage( )  const { return( maximum_number_of_classifiers_per_stage_ ); };
	/// @brief 最後の学習を行った時点での全体のFP率を取得
	double current_false_positive_rate( )              const { return( last_false_positive_rate_[ 3 ] ); };
	/// @brief 最後の学習を行った時点での全体のFP率を取得
	double last_false_positive_rate( )              const { return( last_false_positive_rate_[ 0 ] ); };
	/// @brief 最後の学習を行った時点での全体のTP率を取得
	double last_true_positive_rate( )               const { return( last_true_positive_rate_ ); };

	/// @brief カスケード接続されたすべてのAdaBoost識別器で使用する弱識別器の数を取得する
	size_type total_number_of_classifiers( ) const
	{
		size_type num = 0;
		for( size_type i = 0 ; i < classifiers_.size( ) ; i++ )
		{
			num += classifiers_[ i ].number_of_classifiers( );
		}
		return( num );
	}

	/// @brief 識別器に対してユーザーが設定可能な情報を表す文字列を取得する
	std::string tag( ) const { return( tag_ ); }
	/// @brief 識別器に対してユーザーが設定可能な情報を表す文字列を設定する
	void tag( const std::string &str ){ tag_ = str; }

	/// @brief 識別器を構築するAdaBoost識別器のリストを取得する
	size_type number_of_stages( ) const
	{
		return( classifiers_.size( ) );
	}

	/// @brief 識別器を構築するAdaBoost識別器のリストを取得する
	classifier_list_type & classifiers( )
	{
		return( classifiers_ );
	}

	/// @brief 識別器を構築するAdaBoost識別器のリストを取得する
	const classifier_list_type & classifiers( ) const
	{
		return( classifiers_ );
	}

	/// @brief 新しくAdaBoost識別器を追加する
	bool add_classifier( const base_classifier_type &c )
	{
		classifiers_.push_back( c );

		return( true );
	}

public:
	/// @brief デフォルトのコンストラクタ
	cascaded_classifier_base( ) : minimum_true_positive_rate_per_stage_( 0.99 ),
								  maximum_false_positive_rate_per_stage_( 0.50 ),
								  total_false_positive_rate_( 0.01 ),
								  maximum_number_of_classifiers_per_stage_( 200 )
	{
		last_true_positive_rate_ = 1.0;
		last_false_positive_rate_[ 0 ] = 1.0;
		last_false_positive_rate_[ 1 ] = 0.0;
		last_false_positive_rate_[ 2 ] = 0.0;
		last_false_positive_rate_[ 3 ] = 1.0;
	}

	/// @brief コピーコンストラクタ
	cascaded_classifier_base( const cascaded_classifier_base &c ) :
								base( c ),
								classifiers_( c.classifiers_ ),
								tag_( c.tag_ ),
								minimum_true_positive_rate_per_stage_( c.minimum_true_positive_rate_per_stage_ ),
								maximum_false_positive_rate_per_stage_( c.maximum_false_positive_rate_per_stage_ ),
								total_false_positive_rate_( c.total_false_positive_rate_ ),
								maximum_number_of_classifiers_per_stage_( c.maximum_number_of_classifiers_per_stage_ )
	{
		last_true_positive_rate_ = 1.0;
		last_false_positive_rate_[ 0 ] = 1.0;
		last_false_positive_rate_[ 1 ] = 0.0;
		last_false_positive_rate_[ 2 ] = 0.0;
		last_false_positive_rate_[ 3 ] = 1.0;
	}

	/// @brief デストラクタ
	~cascaded_classifier_base( ){ }

	/// @brief 他のカスケード型AdaBoost識別器の情報をコピーする
	cascaded_classifier_base& operator =( const cascaded_classifier_base& o )
	{
		if( this != &o )
		{
			base::operator =( o );

			classifiers_                             = o.classifiers_;
			tag_                                     = o.tag_;
			minimum_true_positive_rate_per_stage_    = o.minimum_true_positive_rate_per_stage_;
			maximum_false_positive_rate_per_stage_   = o.maximum_false_positive_rate_per_stage_;
			total_false_positive_rate_               = o.total_false_positive_rate_;
			maximum_number_of_classifiers_per_stage_ = o.maximum_number_of_classifiers_per_stage_;
			last_true_positive_rate_                 = o.last_true_positive_rate_;
			last_false_positive_rate_[ 0 ]           = o.last_false_positive_rate_[ 0 ];
			last_false_positive_rate_[ 1 ]           = o.last_false_positive_rate_[ 1 ];
			last_false_positive_rate_[ 2 ]           = o.last_false_positive_rate_[ 2 ];
			last_false_positive_rate_[ 3 ]           = o.last_false_positive_rate_[ 3 ];
		}

		return( *this );
	}

	/// @brief 各段の強識別器の初期化を行う関数（内部用）
	//! 
	//! @param[in,out] features   … 学習に用いる教師データ
	//! @param[in,out] classifier … 初期化する強識別器
	//! 
	template < template < typename, typename > class FEATURE_LIST, class FEATURE, class ALLOCATOR >
	void initialize__( FEATURE_LIST< FEATURE, ALLOCATOR > & features, base_classifier_type & classifier )
	{
		classifier.initialize( features );
	}

public:
	/// @brief 教師データを用いてカスケード型AdaBoost識別器を構築する
	//! 
	//! @param[in,out]  features                                … 学習に用いる教師データ
	//! @param[in]      minimum_true_positive_rate_per_stage    … 各ステージで満たすべきTP率の下限（デフォルトは0.99）
	//! @param[in]      maximum_false_positive_rate_per_stage   … 各ステージで満たすべきFP率の上限（デフォルトは0.50）
	//! @param[in]      total_false_positive_rate               … 学習の終了条件となる全体としてのFP率（デフォルトは0.01）
	//! @param[in]      maximum_number_of_classifiers_per_stage … 構築するAdaBoost識別器で使用する弱識別器の最大数（デフォルトは200）
	//! @param[in]      number_of_stages                        … 構築するカスケードの最大段数（デフォルトは100）
	//! @param[in]      number_of_classifiers_per_step          … 一度に追加する弱識別器の数（デフォルトは1）。多い方が識別性能は高くなる可能性が高い。
	//! 
	template < template < typename, typename > class FEATURE_LIST, class FEATURE, class ALLOCATOR >
	bool learn( FEATURE_LIST< FEATURE, ALLOCATOR > & features,
				double minimum_true_positive_rate_per_stage = 0.99,
				double maximum_false_positive_rate_per_stage = 0.50,
				double total_false_positive_rate = 0.01,
				size_type maximum_number_of_classifiers_per_stage = 200,
				size_type number_of_stages = 100,
				size_type number_of_classifiers_per_step = 1 )
	{
		if( features.empty( ) )
		{
			return( false );
		}

		initialize( minimum_true_positive_rate_per_stage, maximum_false_positive_rate_per_stage, total_false_positive_rate, maximum_number_of_classifiers_per_stage );

#if 1
		return( add_classifier( features, number_of_stages, number_of_classifiers_per_step ) );
#else
		size_type nstages = 0;
		while( nstages < number_of_stages )
		{
			if( add_classifier( features, 1 ) )
			{
				if( this->last_false_positive_rate( ) <= total_false_positive_rate_ )
				{
					break;
				}
			}
			else
			{
				break;
			}

			// ステージを 1 進める
			nstages++;
		}

		return( true );
#endif
	}

	/// @brief カスケード型AdaBoost識別器の設定を初期化する
	//! 
	//! @param[in]      minimum_true_positive_rate_per_stage    … 各ステージで満たすべきTP率の下限（デフォルトは0.99）
	//! @param[in]      maximum_false_positive_rate_per_stage   … 各ステージで満たすべきFP率の上限（デフォルトは0.50）
	//! @param[in]      total_false_positive_rate               … 学習の終了条件となる全体としてのFP率（デフォルトは0.01）
	//! @param[in]      maximum_number_of_classifiers_per_stage … 構築するAdaBoost識別器で使用する弱識別器の最大数（デフォルトは200）
	//! 
	bool initialize( double minimum_true_positive_rate_per_stage__ = 0.99,
					double maximum_false_positive_rate_per_stage__ = 0.50,
					double total_false_positive_rate__ = 0.01,
					size_type maximum_number_of_classifiers_per_stage__ = 200 )
	{
		classifiers_.clear( );
		minimum_true_positive_rate_per_stage_ = minimum_true_positive_rate_per_stage__;
		maximum_false_positive_rate_per_stage_ = maximum_false_positive_rate_per_stage__;
		total_false_positive_rate_ = total_false_positive_rate__;
		maximum_number_of_classifiers_per_stage_ = maximum_number_of_classifiers_per_stage__;

		last_true_positive_rate_ = 1.0;
		last_false_positive_rate_[ 0 ] = 1.0;
		last_false_positive_rate_[ 1 ] = 0.0;
		last_false_positive_rate_[ 2 ] = 0.0;
		last_false_positive_rate_[ 3 ] = 1.0;

		return( true );
	}

	/// @brief 現在のカスケード型AdaBoost識別器にAdaBoost識別器を新しく追加する
	//! 
	//! @param[in,out]  features                       … 学習に用いる教師データ
	//! @param[in]      number_of_stages               … 構築するカスケードの最大段数（デフォルトは100）
	//! @param[in]      number_of_classifiers_per_step … 一度に追加する弱識別器の数（デフォルトは1）。多い方が識別性能は高くなる可能性が高い。
	//! @param[in]      false_positive_rate_per_stage  … 弱識別器の追加を停止する最大のFP率。-1を指定すると識別器に設定した初期値を利用する。
	//! @param[in]      initialize_true_positive_rate  … 途中結果から学習する場合にTP率を最初期化する（デフォルトはfalse）。通常は指定する必要はない。
	//! 
	template < template < typename, typename > class FEATURE_LIST, class FEATURE, class ALLOCATOR >
	bool add_classifier( FEATURE_LIST< FEATURE, ALLOCATOR > & features, size_type number_of_stages = 100, size_type number_of_classifiers_per_step = 1, double false_positive_rate_per_stage = -1.0, bool initialize_true_positive_rate = false )
	{
		return( add_classifier( features, empty_info_functor( ), number_of_stages, number_of_classifiers_per_step, false_positive_rate_per_stage, initialize_true_positive_rate ) );
	}

	/// @brief 現在のカスケード型AdaBoost識別器にAdaBoost識別器を新しく追加する
	//! 
	//! @param[in,out]  features                       … 学習に用いる教師データ
	//! @param[in]      number_of_stages               … 構築するカスケードの最大段数（デフォルトは100）
	//! @param[in]      number_of_classifiers_per_step … 一度に追加する弱識別器の数（デフォルトは1）。多い方が識別性能は高くなる可能性が高い。
	//! @param[in]      false_positive_rate_per_stage  … 弱識別器の追加を停止する最大のFP率。-1を指定すると識別器に設定した初期値を利用する。
	//! @param[in]      initialize_true_positive_rate  … 途中結果から学習する場合にTP率を最初期化する（デフォルトはfalse）。通常は指定する必要はない。
	//! 
	template < template < typename, typename > class FEATURE_LIST, class FEATURE, class ALLOCATOR, class INFOFUNCTOR >
	bool add_classifier( FEATURE_LIST< FEATURE, ALLOCATOR > & features, INFOFUNCTOR finfo, size_type number_of_stages, size_type number_of_classifiers_per_step, double false_positive_rate_per_stage, bool initialize_true_positive_rate )
	{
		if( features.empty( ) )
		{
			std::cerr << "Training data is empty." << std::endl;
			return( false );
		}

		typedef FEATURE feature_type;

		// 有効なサンプルを追加する
		typedef std::list< feature_ref > feature_reference_list_type;
		feature_reference_list_type flist;
		size_type valid_count = 0, positive_count = 0, negative_count = 0;
		for( size_type i = 0 ; i < features.size( ) ; i++ )
		{
			feature_type &f = features[ i ];
			if( f.is_valid( ) )
			{
				valid_count++;

				if( f.is_positive( ) )
				{
					positive_count++;
					flist.push_back( feature_ref( f ) );
				}
				else if( ( ( const derived_type * ) this )->evaluate( f ) >= 0 && f.is_negative( ) )
				{
					negative_count++;
					flist.push_back( feature_ref( f ) );
				}
			}
		}

		// 分類すべきネガティブサンプルが見つからないので終了する
		if( positive_count == 0 || negative_count == 0 )
		{
			if( positive_count == 0 )
			{
				std::cerr << "There is no positive sample in training data." << std::endl;
			}
			if( negative_count == 0 )
			{
				std::cerr << "There is no negative sample in training data." << std::endl;
			}
			return( false );
		}

#if defined( __ADABOOST_OUTPUT_DEBUG_STRING__ ) && __ADABOOST_OUTPUT_DEBUG_STRING__ != 0
		std::cout << "# samples  : " << features.size( ) << " [" << valid_count << "]" << std::endl;
		std::cout << "# positives: " << positive_count << std::endl;
		std::cout << "# negatives: " << negative_count << std::endl;
#endif

		if( initialize_true_positive_rate )
		{
			last_true_positive_rate_ = true_positive_rate( flist );
		}

		double __minimum_true_positive_rate_per_stage__ = this->minimum_true_positive_rate_per_stage( );
		double __maximum_false_positive_rate_per_stage__ = false_positive_rate_per_stage < 0 ? this->maximum_false_positive_rate_per_stage( ) : false_positive_rate_per_stage;

		// 実際のサンプル数に合わせて満たすべきTP率およびFP率を再計算する
		{
			// TP率が厳しすぎる場合は調整する
			size_type tpmin = static_cast< size_type >( positive_count * __minimum_true_positive_rate_per_stage__ );
			size_type fpmax = static_cast< size_type >( std::ceil( negative_count * __maximum_false_positive_rate_per_stage__ ) );

			__minimum_true_positive_rate_per_stage__  = tpmin / static_cast< double >( positive_count );
			__maximum_false_positive_rate_per_stage__ = fpmax / static_cast< double >( negative_count );

			if( __minimum_true_positive_rate_per_stage__ != this->minimum_true_positive_rate_per_stage( ) )
			{
				std::cout << "Minimum true positive rate was changed to " << utility::ToString( __minimum_true_positive_rate_per_stage__ * 100.0, "%.1f%%" ) << std::endl;
			}

			if( __maximum_false_positive_rate_per_stage__ != this->maximum_false_positive_rate_per_stage( ) )
			{
				std::cout << "Maximum false positive rate was changed to " << utility::ToString( __maximum_false_positive_rate_per_stage__ * 100.0, "%.1f%%" ) << std::endl;
			}
		}

		size_type nstages = 0;

		while( this->last_false_positive_rate( ) > this->total_false_positive_rate( ) && nstages < number_of_stages )
		{
			double current_false_positive_rate = 1.0;
			double current_true_positive_rate  = 1.0;

			// 各ステージは強識別器で構成する
			{
				// 識別器の初期化
				base_classifier_type clsf;

				// 識別器を初期化する
				( ( derived_type * ) this )->initialize__( flist, clsf );

				// 識別器をリストに追加する
				classifiers_.push_back( clsf );
			}

			base_classifier_type &classifier = classifiers_.back( );

			size_t num_weaks = 0;
			size_t max_weaks = maximum_number_of_classifiers_per_stage_;
			while( current_false_positive_rate > __maximum_false_positive_rate_per_stage__ )
			{
				// カスケード識別器が目標の誤検出率を下回るまで
				// 現ステージの強識別器に使用する弱識別器数を増やす
				for( size_type n = 0 ; n < number_of_classifiers_per_step && num_weaks <= max_weaks ; n++ )
				{
					num_weaks++;

					// 弱識別機を1つ追加する
					classifier.add_classifiers( flist, finfo, 1 );

					// TP率を評価する
					current_true_positive_rate = true_positive_rate( flist );
					if( current_true_positive_rate > __minimum_true_positive_rate_per_stage__ )
					{
						break;
					}

#if defined( __ADABOOST_OUTPUT_DEBUG_STRING__ ) && __ADABOOST_OUTPUT_DEBUG_STRING__ != 0
					if( n < number_of_classifiers_per_step - 1 )
					{
						// カスケード識別器の誤検出率を計算する
						double tmp[ 5 ];
						false_positive_rate( flist, tmp );
						current_false_positive_rate = tmp[ 0 ];
						double total_FPs                 = last_false_positive_rate_[ 1 ] - ( tmp[ 2 ] - tmp[ 1 ] ) + tmp[ 3 ];
						double total_negatives           = last_false_positive_rate_[ 2 ] + tmp[ 4 ];
						double total_false_positive_rate = total_FPs / total_negatives;

						std::string info = finfo( classifier.classifiers( ).back( ).index( ) );

						if( info.size( ) > 0 )
						{
							printf( "DEBUG : Stage = %2ld, Weaks = %2ld, TH = %+0.3f, TP = %6.2f, FP = %6.2f, CFP = %6.2f [%s]\n", classifiers_.size( ), num_weaks, classifier.threshold( ), current_true_positive_rate * 100.0, total_false_positive_rate * 100.0, current_false_positive_rate * 100.0, info.c_str( ) );
						}
						else
						{
							printf( "DEBUG : Stage = %2ld, Weaks = %2ld, TH = %+0.3f, TP = %6.2f, FP = %6.2f, CFP = %6.2f\n", classifiers_.size( ), num_weaks, classifier.threshold( ), current_true_positive_rate * 100.0, total_false_positive_rate * 100.0, current_false_positive_rate * 100.0 );
						}

					}
#endif
				}

				// 弱識別器数を増加させても，目標の誤検出率を下回れない場合は，
				// 条件が厳しすぎるため学習を終了する
				if( num_weaks > max_weaks )
				{
#if defined( __ADABOOST_OUTPUT_DEBUG_STRING__ ) && __ADABOOST_OUTPUT_DEBUG_STRING__ != 0
					printf( "------------------------------------- REMOVED -------------------------------------\n" );
#endif

					// 学習に失敗したためリストから除外する
					classifiers_.pop_back( );

					return( false );
				}

				double th1 = classifier.threshold( );
				while( true )
				{
					// カスケード識別器の検出率を計算する
					current_true_positive_rate = true_positive_rate( flist );

					if( current_true_positive_rate > __minimum_true_positive_rate_per_stage__ )
					{
						break;
					}

					// 現ステージの強識別器のしきい値を調節しカスケード識別器の検出率を上げる
					th1 = classifier.threshold( );
					classifier.threshold( th1 - 0.5 );
				}

				double th2 = classifier.threshold( );

				if( th1 > th2 )
				{
					for( size_type l = 0 ; th2 < th1; l++ )
					{
						// 現ステージの強識別器のしきい値を調節しカスケード識別器の検出率を上げる
						double th = ( th1 + th2 ) * 0.5;
						classifier.threshold( th );

						// カスケード識別器の検出率を計算する
						current_true_positive_rate = true_positive_rate( flist );

						if( current_true_positive_rate > __minimum_true_positive_rate_per_stage__ )
						{
							if( l > 10 )
							{
								break;
							}

							th2 = th;
						}
						else
						{
							th1 = th;
						}
					}
				}

				// カスケード識別器の誤検出率を計算する
				double tmp[ 5 ];
				false_positive_rate( flist, tmp );
				current_false_positive_rate = tmp[ 0 ];
				double total_FPs                 = last_false_positive_rate_[ 1 ] - ( tmp[ 2 ] - tmp[ 1 ] ) + tmp[ 3 ];
				double total_negatives           = last_false_positive_rate_[ 2 ] + tmp[ 4 ];
				double total_false_positive_rate = total_FPs / total_negatives;

#if defined( __ADABOOST_OUTPUT_DEBUG_STRING__ ) && __ADABOOST_OUTPUT_DEBUG_STRING__ != 0
				printf( "DEBUG : Stage = %2ld, Weaks = %2ld, TH = %+0.3f, TP = %6.2f, FP = %6.2f, CFP = %6.2f\n", classifiers_.size( ), num_weaks, classifier.threshold( ), current_true_positive_rate * 100.0, total_false_positive_rate * 100.0, current_false_positive_rate * 100.0 );
				//printf( "%d / %d = %6.2f\n", ( int )total_FPs, ( int )total_negatives, total_false_positive_rate );
#endif
			}

			// 現在のステージのカスケード識別器の検出率と誤検出率を計算する
			{
				double tmp[ 5 ];
				false_positive_rate( flist, tmp );
				last_false_positive_rate_[ 1 ] = last_false_positive_rate_[ 1 ] - ( tmp[ 2 ] - tmp[ 1 ] ) + tmp[ 3 ];
				last_false_positive_rate_[ 2 ] = last_false_positive_rate_[ 2 ] + tmp[ 4 ];
				last_false_positive_rate_[ 0 ] = last_false_positive_rate_[ 1 ] / last_false_positive_rate_[ 2 ];
				last_false_positive_rate_[ 3 ] = tmp[ 0 ];

				last_true_positive_rate_  = current_true_positive_rate;

				double total_FP = this->last_false_positive_rate( );
				double total_TP = this->last_true_positive_rate( );
				double current_FP = tmp[ 0 ];

				// 学習結果を記録する
				std::string str = "TP(total): " + utility::ToString( total_TP, "%f" ) + ", FP(total): " + utility::ToString( total_FP, "%f" ) + ", FP(current): " + utility::ToString( current_FP, "%f" );
				classifier.info( str );

#if defined( __ADABOOST_OUTPUT_DEBUG_STRING__ ) && __ADABOOST_OUTPUT_DEBUG_STRING__ != 0
				printf( "-----------------------------------------------------------------------------------\n" );
#endif
			}

			// 最終的な誤検出率を下回れない場合は，現在のカスケード識別器で
			// 取り除ける Negative データを学習データから削除する
			if( this->last_false_positive_rate( ) > this->total_false_positive_rate( ) )
			{
				feature_reference_list_type::iterator ite = flist.begin( );
				if( nstages + 1 == number_of_stages )
				{
					for( ; ite != flist.end( ) ; ++ite )
					{
						ite->is_new( false );
						if( ( ( const derived_type * ) this )->evaluate( *ite ) < 0 )
						{
							ite->is_valid( false );
						}
					}
				}
				else
				{
					while( ite != flist.end( ) )
					{
						ite->is_new( false );
						if( ( ( const derived_type * ) this )->evaluate( *ite ) < 0 )
						{
							ite->is_valid( false );
							ite = flist.erase( ite );
						}
						else
						{
							++ite;
						}
					}
				}
			}

			if( current_false_positive_rate == 0 )
			{
				break;
			}

			// ステージを 1 進める
			nstages++;
		}

		return( true );
	}


public:
	/// @brief 学習済みの識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f … 分類する特徴量
	//! 
	//! @return 分類結果
	//! 
	template < class FEATURE >
	bool operator ()( const FEATURE &f ) const
	{
		return( ( ( const derived_type * ) this )->evaluate( f ) >= 0 );
	}

	/// @brief 学習済みの識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f … 特徴量を計算する関数オブジェクト
	//! @param[in]  p … 特徴量計算に使用するパラメータ
	//! 
	//! @return 分類結果
	//! 
	template < class FUNCTOR, class PARAMS >
	bool operator ()( const FUNCTOR &f, const PARAMS &p ) const
	{
		return( ( ( const derived_type * ) this )->evaluate( p, f ) >= 0 );
	}

	/// @brief 学習済みの弱識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2 >
	bool operator ()( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2 ) const
	{
		return( ( ( const derived_type * ) this )->evaluate( f, p1, p2 ) >= 0 );
	}

	/// @brief 学習済みの弱識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  p3 … 特徴量計算に使用するパラメータ3
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3 >
	bool operator ()( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3 ) const
	{
		return( ( ( const derived_type * ) this )->evaluate( f, p1, p2, p3 ) >= 0 );
	}

	/// @brief 学習済みの弱識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  p3 … 特徴量計算に使用するパラメータ3
	//! @param[in]  p4 … 特徴量計算に使用するパラメータ4
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4 >
	bool operator ()( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, const PARAM4 &p4 ) const
	{
		return( ( ( const derived_type * ) this )->evaluate( f, p1, p2, p3, p4 ) >= 0 );
	}

	/// @brief 学習済みの弱識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  p3 … 特徴量計算に使用するパラメータ3
	//! @param[in]  p4 … 特徴量計算に使用するパラメータ4
	//! @param[in]  p5 … 特徴量計算に使用するパラメータ5
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5 >
	bool operator ()( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, const PARAM4 &p4, const PARAM5 &p5 ) const
	{
		return( ( ( const derived_type * ) this )->evaluate( f, p1, p2, p3, p4, p5 ) >= 0 );
	}

	/// @brief 学習済みの識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f … 分類する特徴量
	//! 
	//! @return 分類結果
	//! 
	template < class FEATURE >
	double evaluate( const FEATURE &f ) const
	{
		for( size_type i = 0 ; i < classifiers_.size( ) ; i++ )
		{
			if( !classifiers_[ i ]( f ) )
			{
				return( -1.0 );
			}
		}

		return( 1.0 );
	}

	/// @brief 学習済みの識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f … 特徴量を計算する関数オブジェクト
	//! @param[in]  p … 特徴量計算に使用するパラメータ
	//! 
	//! @return 分類結果
	//! 
	template < class FUNCTOR, class PARAMS >
	double evaluate( const FUNCTOR &f, const PARAMS &p ) const
	{
		for( size_type i = 0 ; i < classifiers_.size( ) ; i++ )
		{
			if( !classifiers_[ i ]( f, p ) )
			{
				return( -1.0 );
			}
		}

		return( 1.0 );
	}

	/// @brief 学習済みの識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! 
	//! @return 分類結果
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2 >
	double evaluate( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2 ) const
	{
		for( size_type i = 0 ; i < classifiers_.size( ) ; i++ )
		{
			if( !classifiers_[ i ]( f, p1, p2 ) )
			{
				return( -1.0 );
			}
		}

		return( 1.0 );
	}

	/// @brief 学習済みの識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  p3 … 特徴量計算に使用するパラメータ3
	//! 
	//! @return 分類結果
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3 >
	double evaluate( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3 ) const
	{
		for( size_type i = 0 ; i < classifiers_.size( ) ; i++ )
		{
			if( !classifiers_[ i ]( f, p1, p2, p3 ) )
			{
				return( -1.0 );
			}
		}

		return( 1.0 );
	}

	/// @brief 学習済みの識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  p3 … 特徴量計算に使用するパラメータ3
	//! @param[in]  p4 … 特徴量計算に使用するパラメータ4
	//! 
	//! @return 分類結果
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4 >
	double evaluate( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, const PARAM4 &p4 ) const
	{
		for( size_type i = 0 ; i < classifiers_.size( ) ; i++ )
		{
			if( !classifiers_[ i ]( f, p1, p2, p3, p4 ) )
			{
				return( -1.0 );
			}
		}

		return( 1.0 );
	}

	/// @brief 学習済みの識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  p3 … 特徴量計算に使用するパラメータ3
	//! @param[in]  p4 … 特徴量計算に使用するパラメータ4
	//! @param[in]  p5 … 特徴量計算に使用するパラメータ4
	//! 
	//! @return 分類結果
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5 >
	double evaluate( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, const PARAM4 &p4, const PARAM5 &p5 ) const
	{
		for( size_type i = 0 ; i < classifiers_.size( ) ; i++ )
		{
			if( !classifiers_[ i ]( f, p1, p2, p3, p4, p5 ) )
			{
				return( -1.0 );
			}
		}

		return( 1.0 );
	}


public:
	/// @brief 追加の教師データを用いて弱識別器を更新する
	//! 
	//! @param[in]  f           … 分類する特徴量ベクトル
	//! @param[in]  is_positive … ポジティブサンプルかどうか
	//! 
	template < class FEATURE >
	void update( const FEATURE &f, bool is_positive )
	{
		double lambda = 1.0;
		for( size_type t = 0 ; t < classifiers_.size( ) ; t++ )
		{
			base_classifier_type &weak = classifiers_[ t ];

			lambda = weak.update( f, is_positive, lambda );

#if defined( __ADABOOST_CASCADE_TYPE__ ) && ( __ADABOOST_CASCADE_TYPE__ == 2 || __ADABOOST_CASCADE_TYPE__ == 3 )
			double val = 0.0;
			for( size_type i = 0 ; i <= t ; i++ )
			{
				val += classifiers_[ i ].evaluate( f );
			}
#else
			double val = weak.evaluate( f );
#endif

			if( val < 0 )
			{
				break;
			}
		}
	}

	/// @brief 追加の教師データを用いて弱識別器を更新する
	//! 
	//! @param[in]  f … 特徴量を計算する関数オブジェクト
	//! @param[in]  p … 特徴量計算に使用するパラメータ
	//! @param[in]  is_positive … ポジティブサンプルかどうか
	//! 
	template < class FUNCTOR, class PARAMS >
	void update( const FUNCTOR &f, const PARAMS &p, bool is_positive )
	{
		for( size_type t = 0 ; t < classifiers_.size( ) ; t++ )
		{
			base_classifier_type &weak = classifiers_[ t ];

			weak.update( f, p, is_positive );

#if defined( __ADABOOST_CASCADE_TYPE__ ) && ( __ADABOOST_CASCADE_TYPE__ == 2 || __ADABOOST_CASCADE_TYPE__ == 3 )
			double val = 0.0;
			for( size_type i = 0 ; i <= t ; i++ )
			{
				val += classifiers_[ i ].evaluate( f, p );
			}
#else
			double val = weak.evaluate( f, p );
#endif

			if( val < 0 )
			{
				break;
			}
		}
	}

	/// @brief 追加の教師データを用いて弱識別器を更新する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  is_positive … ポジティブサンプルかどうか
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2 >
	void update( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, bool is_positive )
	{
		for( size_type t = 0 ; t < classifiers_.size( ) ; t++ )
		{
			base_classifier_type &weak = classifiers_[ t ];

			weak.update( f, p1, p2, is_positive );

#if defined( __ADABOOST_CASCADE_TYPE__ ) && ( __ADABOOST_CASCADE_TYPE__ == 2 || __ADABOOST_CASCADE_TYPE__ == 3 )
			double val = 0.0;
			for( size_type i = 0 ; i <= t ; i++ )
			{
				val += classifiers_[ i ].evaluate( f, p1, p2 );
			}
#else
			double val = weak.evaluate( f, p1, p2 );
#endif

			if( val < 0 )
			{
				break;
			}
		}
	}

	/// @brief 追加の教師データを用いて弱識別器を更新する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  p3 … 特徴量計算に使用するパラメータ3
	//! @param[in]  is_positive … ポジティブサンプルかどうか
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3 >
	void update( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, bool is_positive )
	{
		for( size_type t = 0 ; t < classifiers_.size( ) ; t++ )
		{
			base_classifier_type &weak = classifiers_[ t ];

			weak.update( f, p1, p2, p3, is_positive );

#if defined( __ADABOOST_CASCADE_TYPE__ ) && ( __ADABOOST_CASCADE_TYPE__ == 2 || __ADABOOST_CASCADE_TYPE__ == 3 )
			double val = 0.0;
			for( size_type i = 0 ; i <= t ; i++ )
			{
				val += classifiers_[ i ].evaluate( f, p1, p2, p3 );
			}
#else
			double val = weak.evaluate( f, p1, p2, p3 );
#endif

			if( val < 0 )
			{
				break;
			}
		}
	}

	/// @brief 追加の教師データを用いて弱識別器を更新する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  p3 … 特徴量計算に使用するパラメータ3
	//! @param[in]  p4 … 特徴量計算に使用するパラメータ4
	//! @param[in]  is_positive … ポジティブサンプルかどうか
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4 >
	void update( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, const PARAM4 &p4, bool is_positive )
	{
		for( size_type t = 0 ; t < classifiers_.size( ) ; t++ )
		{
			base_classifier_type &weak = classifiers_[ t ];

			weak.update( f, p1, p2, p3, p4, is_positive );

#if defined( __ADABOOST_CASCADE_TYPE__ ) && ( __ADABOOST_CASCADE_TYPE__ == 2 || __ADABOOST_CASCADE_TYPE__ == 3 )
			double val = 0.0;
			for( size_type i = 0 ; i <= t ; i++ )
			{
				val += classifiers_[ i ].evaluate( f, p1, p2, p3, p4 );
			}
#else
			double val = weak.evaluate( f, p1, p2, p3, p4 );
#endif

			if( val < 0 )
			{
				break;
			}
		}
	}

public:
	/// @brief 特徴量のリストからTP率を計算する
	//! 
	//! @param[in]  features             … TP率の計算に使用する特徴量のリスト
	//! @param[in]  force_calculate_rate … 無効に設定されている特徴量も計算に使用するかどうか（デフォルトはfalse）
	//! 
	template < template < typename, typename > class FEATURE_LIST, class FEATURE, class ALLOCATOR >
	double true_positive_rate( const FEATURE_LIST< FEATURE, ALLOCATOR > &features, bool force_calculate_rate = false )
	{
		if( features.empty( ) )
		{
			return( 0.0 );
		}
		else
		{
			double rates[ 5 ];
			true_positive_rate( features, rates, force_calculate_rate );
			return( rates[ 0 ] );
		}
	}

	/// @brief 特徴量のリストからFP率を計算する
	//! 
	//! @param[in]  features             … FP率の計算に使用する特徴量のリスト
	//! @param[in]  force_calculate_rate … 無効に設定されている特徴量も計算に使用するかどうか（デフォルトはfalse）
	//! 
	template < template < typename, typename > class FEATURE_LIST, class FEATURE, class ALLOCATOR >
	double false_positive_rate( const FEATURE_LIST< FEATURE, ALLOCATOR > &features, bool force_calculate_rate = false )
	{
		if( features.empty( ) )
		{
			return( 0.0 );
		}
		else
		{
			double rates[ 5 ];
			false_positive_rate( features, rates, force_calculate_rate );
			return( rates[ 0 ] );
		}
	}

	/// @brief 特徴量のリストからTP率を計算する
	//! 
	//! @param[in]  features             … TP率の計算に使用する特徴量のリスト
	//! @param[out] rates                … 旧データと新データに分けて計算したTP率
	//! @param[in]  force_calculate_rate … 無効に設定されている特徴量も計算に使用するかどうか（デフォルトはfalse）
	//! 
	template < template < typename, typename > class FEATURE_LIST, class FEATURE, class ALLOCATOR >
	void true_positive_rate( const FEATURE_LIST< FEATURE, ALLOCATOR > &features, double rates[ 5 ], bool force_calculate_rate = false )
	{
		rates[ 0 ] = rates[ 1 ] = rates[ 2 ] = rates[ 3 ] = rates[ 4 ] = 0.0;

		if( features.empty( ) )
		{
			return;
		}			

		typedef FEATURE feature_type;
		typedef typename FEATURE_LIST< FEATURE, ALLOCATOR >::const_iterator const_iterator;

		size_t positives[ 2 ]  = { 0, 0 };
		size_t detections[ 2 ] = { 0, 0 };
		for( const_iterator ite = features.begin( ) ; ite != features.end( ) ; ++ite )
		{
			const feature_type &f = *ite;
			if( force_calculate_rate || f.is_valid( ) )
			{
				if( f.is_positive( ) )
				{
					positives[ f.is_new( ) ? 1 : 0 ]++;

					if( ( ( const derived_type * ) this )->evaluate( f ) >= 0 )
					{
						detections[ f.is_new( ) ? 1 : 0 ]++;
					}
				}
			}
		}

		size_t num = positives[ 0 ] + positives[ 1 ];
		rates[ 0 ] = static_cast< double >( detections[ 0 ] + detections[ 1 ] ) / static_cast< double >( num > 0 ? num : 1 );
		rates[ 1 ] = static_cast< double >( detections[ 0 ] );	// 従来データ
		rates[ 2 ] = static_cast< double >( positives[ 0 ] );	// 従来データ
		rates[ 3 ] = static_cast< double >( detections[ 1 ] );	// 新規データ
		rates[ 4 ] = static_cast< double >( positives[ 1 ] );	// 新規データ
	}

	/// @brief 特徴量のリストからFP率を計算する
	//! 
	//! @param[in]  features             … TP率の計算に使用する特徴量のリスト
	//! @param[out] rates                … 旧データと新データに分けて計算したFP率
	//! @param[in]  force_calculate_rate … 無効に設定されている特徴量も計算に使用するかどうか（デフォルトはfalse）
	//! 
	template < template < typename, typename > class FEATURE_LIST, class FEATURE, class ALLOCATOR >
	void false_positive_rate( const FEATURE_LIST< FEATURE, ALLOCATOR > &features, double rates[ 5 ], bool force_calculate_rate = false )
	{
		rates[ 0 ] = rates[ 1 ] = rates[ 2 ] = rates[ 3 ] = rates[ 4 ] = 0.0;

		if( features.empty( ) )
		{
			return;
		}			

		typedef FEATURE feature_type;
		typedef typename FEATURE_LIST< FEATURE, ALLOCATOR >::const_iterator const_iterator;

		size_t negatives[ 2 ]       = { 0, 0 };
		size_t miss_detections[ 2 ] = { 0, 0 };
		for( const_iterator ite = features.begin( ) ; ite != features.end( ) ; ++ite )
		{
			const feature_type &f = *ite;
			if( force_calculate_rate || f.is_valid( ) )
			{
				if( f.is_negative( ) )
				{
					negatives[ f.is_new( ) ? 1 : 0 ]++;

					if( ( ( const derived_type * ) this )->evaluate( f ) >= 0 )
					{
						miss_detections[ f.is_new( ) ? 1 : 0 ]++;
					}
				}
			}
		}

		size_t num = negatives[ 0 ] + negatives[ 1 ];
		rates[ 0 ] = static_cast< double >( miss_detections[ 0 ] + miss_detections[ 1 ] ) / static_cast< double >( num > 0 ? num : 1 );
		rates[ 1 ] = static_cast< double >( miss_detections[ 0 ] );	// 従来データ
		rates[ 2 ] = static_cast< double >( negatives[ 0 ] );		// 従来データ
		rates[ 3 ] = static_cast< double >( miss_detections[ 1 ] );	// 新規データ
		rates[ 4 ] = static_cast< double >( negatives[ 1 ] );		// 新規データ
	}
};



/// @brief 弱識別器をMulti-exit接続した強識別器の基底クラス
template < class DERIVED_CLASSIFIER, class BASE_WEAK_CLASSIFIER >
class multi_exit_classifier_base : public classifier_base
{
private:
	typedef classifier_base						base;					///< @brief 基底クラス
	typedef DERIVED_CLASSIFIER                  derived_type;			///< @brief 派生クラス

public:
	typedef BASE_WEAK_CLASSIFIER                base_classifier_type;	///< @brief AdaBoostで利用する弱識別器
	typedef feature::value_type                 value_type;				///< @brief MISTのコンテナ内に格納するデータ型．mist::array< data > の data と同じ
	typedef feature::size_type                  size_type;				///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef feature::difference_type            difference_type;		///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef std::vector< base_classifier_type > classifier_list_type;	///< @brief Multi-exit型のカスケードにする識別器のリスト
	typedef std::vector< size_type >            exit_list_type;			///< @brief Multi-exit型のカスケードを抜ける識別器のインデックスを保持するリスト

protected:
	classifier_list_type	classifiers_;				///< @brief 弱識別器のリスト
	exit_list_type			exits_;						///< @brief Multi-exit型カスケードの出口のリスト
	std::string				info_;						///< @brief 識別器を学習した際の情報を保持する文字列
	std::string				tag_;						///< @brief ユーザーが独自に設定可能な文字列

	double minimum_true_positive_rate_per_stage_;		///< @brief 各ステージで満たすべきTP率の下限
	double maximum_false_positive_rate_per_stage_;		///< @brief 各ステージで満たすべきFP率の上限
	double total_false_positive_rate_;					///< @brief Multi-exit型カスケード識別器の構築時の終了条件となる全体としてのFP率
	size_type maximum_number_of_classifiers_per_stage_;	///< @brief 構築するカスケードの最大段数
	double last_true_positive_rate_;					///< @brief Multi-exit型カスケード識別器の最後の学習を行った時点での全体のTP率
	double last_false_positive_rate_[ 4 ];				///< @brief Multi-exit型カスケード識別器の最後の学習を行った時点での全体のFP率

public:
	/// @brief 各ステージで満たすべきTP率の下限を取得
	double minimum_true_positive_rate_per_stage( )  const { return( minimum_true_positive_rate_per_stage_ ); }
	/// @brief 各ステージで満たすべきFP率の上限を取得
	double maximum_false_positive_rate_per_stage( ) const { return( maximum_false_positive_rate_per_stage_ ); };
	/// @brief 学習の終了条件となる全体としてのFP率を取得
	double total_false_positive_rate( )             const { return( total_false_positive_rate_ ); };
	/// @brief 構築するカスケードの最大段数を取得
	size_type maximum_number_of_classifiers_per_stage( )  const { return( maximum_number_of_classifiers_per_stage_ ); };
	/// @brief 最後の学習を行った時点での全体のFP率を取得
	double current_false_positive_rate( )              const { return( last_false_positive_rate_[ 3 ] ); };
	/// @brief 最後の学習を行った時点での全体のFP率を取得
	double last_false_positive_rate( )              const { return( last_false_positive_rate_[ 0 ] ); };
	/// @brief 最後の学習を行った時点での全体のTP率を取得
	double last_true_positive_rate( )               const { return( last_true_positive_rate_ ); };

	/// @brief 識別器を学習した際の情報を保持する文字列を取得する
	std::string info( ) const { return( info_ ); }
	/// @brief 識別器を学習した際の情報を保持する文字列を設定する
	void info( const std::string &str ){ info_ = str; }

	/// @brief 識別器に対してユーザーが設定可能な情報を表す文字列を取得する
	std::string tag( ) const { return( tag_ ); }
	/// @brief 識別器に対してユーザーが設定可能な情報を表す文字列を設定する
	void tag( const std::string &str ){ tag_ = str; }

	/// @brief 識別器を構築するAdaBoost識別器のリストを取得する
	size_type number_of_stages( ) const
	{
		return( exits_.size( ) );
	}

	/// @brief AdaBoost識別器を構築する弱識別器の数を取得する
	size_type number_of_classifiers( ) const { return( classifiers_.size( ) ); }

	/// @brief AdaBoost識別器を構築する弱識別器の数を取得する
	size_type total_number_of_classifiers( ) const { return( classifiers_.size( ) ); }

	/// @brief 識別器を構築する弱識別器のリストを取得する
	classifier_list_type & classifiers( ){ return( classifiers_ ); }

	/// @brief 識別器を構築する弱識別器のリストを取得する
	const classifier_list_type & classifiers( ) const { return( classifiers_ ); }

	/// @brief Multi-exit型カスケードを抜ける識別器のインデックスを保持するリストを取得する
	exit_list_type & exits( ){ return( exits_ ); }

	/// @brief Multi-exit型カスケードを抜ける識別器のインデックスを保持するリストを取得する
	const exit_list_type & exits( ) const { return( exits_ ); }

	/// @brief 新しく弱識別器を追加する
	bool add_classifier( const base_classifier_type &c )
	{
		classifiers_.push_back( c );

		return( true );
	}

public:
	/// @brief デフォルトのコンストラクタ
	multi_exit_classifier_base( ) : minimum_true_positive_rate_per_stage_( 0.99 ),
								  maximum_false_positive_rate_per_stage_( 0.50 ),
								  total_false_positive_rate_( 0.01 ),
								  maximum_number_of_classifiers_per_stage_( 200 )
	{
		last_true_positive_rate_ = 1.0;
		last_false_positive_rate_[ 0 ] = 1.0;
		last_false_positive_rate_[ 1 ] = 0.0;
		last_false_positive_rate_[ 2 ] = 0.0;
		last_false_positive_rate_[ 3 ] = 1.0;
	}

	/// @brief コピーコンストラクタ
	multi_exit_classifier_base( const multi_exit_classifier_base &c ) :
								base( c ),
								classifiers_( c.classifiers_ ),
								exits_( c.exits_ ),
								tag_( c.tag_ ),
								minimum_true_positive_rate_per_stage_( c.minimum_true_positive_rate_per_stage_ ),
								maximum_false_positive_rate_per_stage_( c.maximum_false_positive_rate_per_stage_ ),
								total_false_positive_rate_( c.total_false_positive_rate_ ),
								maximum_number_of_classifiers_per_stage_( c.maximum_number_of_classifiers_per_stage_ )
	{
		last_true_positive_rate_ = 1.0;
		last_false_positive_rate_[ 0 ] = 1.0;
		last_false_positive_rate_[ 1 ] = 0.0;
		last_false_positive_rate_[ 2 ] = 0.0;
		last_false_positive_rate_[ 3 ] = 1.0;
	}

	/// @brief デストラクタ
	~multi_exit_classifier_base( ){ }

	/// @brief 他のMulti-exit型AdaBoost識別器の情報をコピーする
	multi_exit_classifier_base& operator =( const multi_exit_classifier_base& o )
	{
		if( this != &o )
		{
			base::operator =( o );

			classifiers_                             = o.classifiers_;
			exits_                                   = o.exits_;
			tag_                                     = o.tag_;
			info_                                    = o.info_;
			minimum_true_positive_rate_per_stage_    = o.minimum_true_positive_rate_per_stage_;
			maximum_false_positive_rate_per_stage_   = o.maximum_false_positive_rate_per_stage_;
			total_false_positive_rate_               = o.total_false_positive_rate_;
			maximum_number_of_classifiers_per_stage_ = o.maximum_number_of_classifiers_per_stage_;
			last_true_positive_rate_                 = o.last_true_positive_rate_;
			last_false_positive_rate_[ 0 ]           = o.last_false_positive_rate_[ 0 ];
			last_false_positive_rate_[ 1 ]           = o.last_false_positive_rate_[ 1 ];
			last_false_positive_rate_[ 2 ]           = o.last_false_positive_rate_[ 2 ];
			last_false_positive_rate_[ 3 ]           = o.last_false_positive_rate_[ 3 ];
		}

		return( *this );
	}

	/// @brief 各段の強識別器の初期化を行う関数（内部用）
	//! 
	//! @param[in,out] features   … 学習に用いる教師データ
	//! @param[in,out] classifier … 初期化する強識別器
	//! 
	template < template < typename, typename > class FEATURE_LIST, class FEATURE, class ALLOCATOR >
	void initialize__( FEATURE_LIST< FEATURE, ALLOCATOR > & /* features */, base_classifier_type & /* classifier */ )
	{
	}

public:
	/// @brief 教師データを用いてMulti-exit型AdaBoost識別器を構築する
	//! 
	//! @param[in,out]  features                                … 学習に用いる教師データ
	//! @param[in]      minimum_true_positive_rate_per_stage    … 各ステージで満たすべきTP率の下限（デフォルトは0.99）
	//! @param[in]      maximum_false_positive_rate_per_stage   … 各ステージで満たすべきFP率の上限（デフォルトは0.50）
	//! @param[in]      total_false_positive_rate               … 学習の終了条件となる全体としてのFP率（デフォルトは0.01）
	//! @param[in]      maximum_number_of_classifiers_per_stage … 構築するAdaBoost識別器で使用する弱識別器の最大数（デフォルトは200）
	//! @param[in]      number_of_stages                        … 構築するカスケードの最大段数（デフォルトは100）
	//! @param[in]      minimum_number_of_classifiers_per_step  … 各ステージを構成する弱識別器の最小数（デフォルトは1）。多い方が識別性能は高くなる可能性が高い。
	//! 
	template < template < typename, typename > class FEATURE_LIST, class FEATURE, class ALLOCATOR >
	bool learn( FEATURE_LIST< FEATURE, ALLOCATOR > & features,
				double minimum_true_positive_rate_per_stage = 0.99,
				double maximum_false_positive_rate_per_stage = 0.50,
				double total_false_positive_rate = 0.01,
				size_type maximum_number_of_classifiers_per_stage = 200,
				size_type number_of_stages = 100,
				size_type minimum_number_of_classifiers_per_step = 1 )
	{
		if( features.empty( ) )
		{
			return( false );
		}

		initialize( minimum_true_positive_rate_per_stage, maximum_false_positive_rate_per_stage, total_false_positive_rate, maximum_number_of_classifiers_per_stage );

#if 1
		return( add_classifier( features, number_of_stages, minimum_number_of_classifiers_per_step ) );
#else
		size_type nstages = 0;
		while( nstages < number_of_stages )
		{
			if( add_classifier( features, 1 ) )
			{
				if( this->last_false_positive_rate( ) <= total_false_positive_rate_ )
				{
					break;
				}
			}
			else
			{
				break;
			}

			// ステージを 1 進める
			nstages++;
		}

		return( true );
#endif
	}

	/// @brief Multi-exit型AdaBoost識別器の設定を初期化する
	//! 
	//! @param[in]      minimum_true_positive_rate_per_stage    … 各ステージで満たすべきTP率の下限（デフォルトは0.99）
	//! @param[in]      maximum_false_positive_rate_per_stage   … 各ステージで満たすべきFP率の上限（デフォルトは0.50）
	//! @param[in]      total_false_positive_rate               … 学習の終了条件となる全体としてのFP率（デフォルトは0.01）
	//! @param[in]      maximum_number_of_classifiers_per_stage … 構築するAdaBoost識別器で使用する弱識別器の最大数（デフォルトは200）
	//! 
	bool initialize( double minimum_true_positive_rate_per_stage__ = 0.99,
					double maximum_false_positive_rate_per_stage__ = 0.50,
					double total_false_positive_rate__ = 0.01,
					size_type maximum_number_of_classifiers_per_stage__ = 200 )
	{
		classifiers_.clear( );
		exits_.clear( );

		minimum_true_positive_rate_per_stage_ = minimum_true_positive_rate_per_stage__;
		maximum_false_positive_rate_per_stage_ = maximum_false_positive_rate_per_stage__;
		total_false_positive_rate_ = total_false_positive_rate__;
		maximum_number_of_classifiers_per_stage_ = maximum_number_of_classifiers_per_stage__;

		last_true_positive_rate_ = 1.0;
		last_false_positive_rate_[ 0 ] = 1.0;
		last_false_positive_rate_[ 1 ] = 0.0;
		last_false_positive_rate_[ 2 ] = 0.0;
		last_false_positive_rate_[ 3 ] = 1.0;

		return( true );
	}

	template < template < typename, typename > class FEATURE_LIST, class FEATURE, class ALLOCATOR >
	bool add_classifiers__( FEATURE_LIST< FEATURE, ALLOCATOR > & features, size_type num_max_classifier_classifiers = 100 )
	{
		return( false );
	}

	/// @brief 現在のMulti-exit型AdaBoost識別器にAdaBoost識別器を新しく追加する
	//! 
	//! @param[in,out]  features                               … 学習に用いる教師データ
	//! @param[in]      number_of_exits                        … 構築するMulti-exit型カスケードの最大段数（デフォルトは100）
	//! @param[in]      minimum_number_of_classifiers_per_step … 各ステージを構成する弱識別器の最小数（デフォルトは1）。多い方が識別性能は高くなる可能性が高い。
	//! @param[in]      false_positive_rate_per_stage          … 弱識別器の追加を停止する最大のFP率。-1を指定すると識別器に設定した初期値を利用する。
	//! @param[in]      initialize_true_positive_rate          … 途中結果から学習する場合にTP率を最初期化する（デフォルトはfalse）。通常は指定する必要はない。
	//! 
	template < template < typename, typename > class FEATURE_LIST, class FEATURE, class ALLOCATOR >
	bool add_classifier( FEATURE_LIST< FEATURE, ALLOCATOR > & features, size_type number_of_exits = 100, size_type minimum_number_of_classifiers_per_step = 1, double false_positive_rate_per_stage = -1.0, bool initialize_true_positive_rate = false )
	{
		return( add_classifier( features, empty_info_functor( ), number_of_exits, minimum_number_of_classifiers_per_step, false_positive_rate_per_stage, initialize_true_positive_rate ) );
	}

	/// @brief 現在のMulti-exit型AdaBoost識別器にAdaBoost識別器を新しく追加する
	//! 
	//! @param[in,out]  features                               … 学習に用いる教師データ
	//! @param[in]      number_of_exits                        … 構築するMulti-exit型カスケードの最大段数（デフォルトは100）
	//! @param[in]      minimum_number_of_classifiers_per_step … 各ステージを構成する弱識別器の最小数（デフォルトは1）。多い方が識別性能は高くなる可能性が高い。
	//! @param[in]      false_positive_rate_per_stage          … 弱識別器の追加を停止する最大のFP率。-1を指定すると識別器に設定した初期値を利用する。
	//! @param[in]      initialize_true_positive_rate          … 途中結果から学習する場合にTP率を最初期化する（デフォルトはfalse）。通常は指定する必要はない。
	//! 
	template < template < typename, typename > class FEATURE_LIST, class FEATURE, class ALLOCATOR, class INFOFUNCTOR >
	bool add_classifier( FEATURE_LIST< FEATURE, ALLOCATOR > & features, INFOFUNCTOR finfo, size_type number_of_exits, size_type minimum_number_of_classifiers_per_step, double false_positive_rate_per_stage, bool initialize_true_positive_rate )
	{
		if( features.empty( ) )
		{
			std::cerr << "Training data is empty." << std::endl;
			return( false );
		}

		if( minimum_number_of_classifiers_per_step > maximum_number_of_classifiers_per_stage_ )
		{
			minimum_number_of_classifiers_per_step = maximum_number_of_classifiers_per_stage_;
		}

		typedef FEATURE feature_type;

		// 有効なサンプルを追加する
		typedef std::list< feature_ref > feature_reference_list_type;
		feature_reference_list_type flist;
		size_type valid_count = 0, positive_count = 0, negative_count = 0;
		for( size_type i = 0 ; i < features.size( ) ; i++ )
		{
			feature_type &f = features[ i ];
			if( f.is_valid( ) )
			{
				valid_count++;

				if( f.is_positive( ) )
				{
					positive_count++;
					flist.push_back( feature_ref( f ) );
				}
				else if( ( ( const derived_type * ) this )->evaluate( f ) >= 0 && f.is_negative( ) )
				{
					negative_count++;
					flist.push_back( feature_ref( f ) );
				}
			}
		}

		// 分類すべきネガティブサンプルが見つからないので終了する
		if( positive_count == 0 || negative_count == 0 )
		{
			if( positive_count == 0 )
			{
				std::cerr << "There is no positive sample in training data." << std::endl;
			}
			if( negative_count == 0 )
			{
				std::cerr << "There is no negative sample in training data." << std::endl;
			}
			return( false );
		}

#if defined( __ADABOOST_OUTPUT_DEBUG_STRING__ ) && __ADABOOST_OUTPUT_DEBUG_STRING__ != 0
		std::cout << "# samples  : " << features.size( ) << " [" << valid_count << "]" << std::endl;
		std::cout << "# positives: " << positive_count << std::endl;
		std::cout << "# negatives: " << negative_count << std::endl;
#endif

		if( initialize_true_positive_rate )
		{
			last_true_positive_rate_ = true_positive_rate( flist );
		}

		double __minimum_true_positive_rate_per_stage__ = this->minimum_true_positive_rate_per_stage( );
		double __maximum_false_positive_rate_per_stage__ = false_positive_rate_per_stage < 0 ? this->maximum_false_positive_rate_per_stage( ) : false_positive_rate_per_stage;

		// 実際のサンプル数に合わせて満たすべきTP率およびFP率を再計算する
		{
			// TP率が厳しすぎる場合は調整する
			size_type tpmin = static_cast< size_type >( positive_count * __minimum_true_positive_rate_per_stage__ );
			size_type fpmax = static_cast< size_type >( std::ceil( negative_count * __maximum_false_positive_rate_per_stage__ ) );

			__minimum_true_positive_rate_per_stage__  = tpmin / static_cast< double >( positive_count );
			__maximum_false_positive_rate_per_stage__ = fpmax / static_cast< double >( negative_count );

			if( __minimum_true_positive_rate_per_stage__ != this->minimum_true_positive_rate_per_stage( ) )
			{
				std::cout << "Minimum true positive rate was changed to " << utility::ToString( __minimum_true_positive_rate_per_stage__ * 100.0, "%.1f%%" ) << std::endl;
			}

			if( __maximum_false_positive_rate_per_stage__ != this->maximum_false_positive_rate_per_stage( ) )
			{
				std::cout << "Maximum false positive rate was changed to " << utility::ToString( __maximum_false_positive_rate_per_stage__ * 100.0, "%.1f%%" ) << std::endl;
			}
		}

		double alpha0 = __maximum_false_positive_rate_per_stage__;
		double beta0  = 1.0 - __minimum_true_positive_rate_per_stage__;
		double lambda = alpha0 / beta0;
		double log_lambda = lambda > 1.0 ? std::log( lambda ) : 0.0;
		size_type nstages = 0;

		while( this->last_false_positive_rate( ) > this->total_false_positive_rate( ) && nstages < number_of_exits )
		{
			double current_false_positive_rate = 1.0;
			double current_true_positive_rate  = 1.0;

			// ポジティブとネガティブの個数を数え直す
			positive_count = negative_count = 0;
			for( feature_reference_list_type::iterator ite = flist.begin( ) ; ite != flist.end( ) ; ++ite )
			{
				feature_ref &f = *ite;
				if( f.is_valid( ) )
				{
					if( f.is_positive( ) )
					{
						positive_count++;
					}
					else
					{
						negative_count++;
					}
				}
			}

			// 分類すべきネガティブサンプルが見つからないので終了する
			if( positive_count == 0 || negative_count == 0 )
			{
				if( positive_count == 0 )
				{
					std::cerr << "There is no positive sample in training data." << std::endl;
				}
				if( negative_count == 0 )
				{
					std::cerr << "There is no negative sample in training data." << std::endl;
				}
				return( false );
			}


			// 各ステージは弱識別器で構成する
			{
				// 識別器の初期化
				base_classifier_type clsf;

				// 識別器を初期化する
				( ( derived_type * ) this )->initialize__( flist, clsf );
			}

			// 重みの初期化
			if( exits_.empty( ) )
			{
				double pw = std::exp( 0.5 * log_lambda );
				double nw = std::exp( -0.5 * log_lambda );
				double posw = pw / ( pw + nw ) / positive_count;
				double negw = nw / ( pw + nw ) / negative_count;
				//double posw = std::exp( 0.5 * log_lambda ) / positive_count;
				//double negw = std::exp( -0.5 * log_lambda ) / negative_count;

				base::positive_weight( posw );
				base::negative_weight( negw );

				for( feature_reference_list_type::iterator ite = flist.begin( ) ; ite != flist.end( ) ; ++ite )
				{
					feature_ref &f = *ite;
					if( f.is_valid( ) )
					{
						if( f.is_positive( ) )
						{
							f.weight( posw );
						}
						else
						{
							f.weight( negw );
						}
					}
				}
			}
			else
			{
				for( feature_reference_list_type::iterator ite = flist.begin( ) ; ite != flist.end( ) ; ++ite )
				{
					feature_ref &f = *ite;
					if( f.is_valid( ) )
					{
						size_type eid = exits_.back( );
						double val = 0.0;
						for( size_type i = 0 ; i < eid ; i++ )
						{
							val += classifiers_[ i ].evaluate( f );
						}

						if( f.is_positive( ) )
						{
							double w = std::exp( 0.5 * log_lambda - val ) / positive_count;
							f.weight( w );
						}
						else
						{
							double w = std::exp( val - 0.5 * log_lambda ) / negative_count;
							f.weight( w );
						}
					}
				}
			}

			// 識別器の出口を設定する
			exits_.push_back( classifiers_.size( ) );

			size_t num_weaks = 0;
			size_t max_weaks = maximum_number_of_classifiers_per_stage_;
			while( num_weaks <= max_weaks )
			{
				// Multi-exit型カスケード識別器が目標の誤検出率を下回るまで
				// 現ステージの強識別器に使用する弱識別器数を増やす
				num_weaks++;

				// 弱識別機を1つ追加する
				( ( derived_type * ) this )->add_classifiers__( flist, 1 );

				// 出口を最後に移動させる
				exits_.back( ) = classifiers_.size( );

#if defined( __ADABOOST_OUTPUT_DEBUG_STRING__ ) && __ADABOOST_OUTPUT_DEBUG_STRING__ != 0
				{
					// Multi-exit型カスケード識別器の誤検出率を計算する
					double tmp[ 5 ];
					false_positive_rate( flist, tmp );
					current_true_positive_rate = this->true_positive_rate( flist );
					current_false_positive_rate = tmp[ 0 ];
					double total_FPs                 = last_false_positive_rate_[ 1 ] - ( tmp[ 2 ] - tmp[ 1 ] ) + tmp[ 3 ];
					double total_negatives           = last_false_positive_rate_[ 2 ] + tmp[ 4 ];
					double total_false_positive_rate = total_FPs / total_negatives;

					std::string info = finfo( classifiers_.back( ).index( ) );

					if( info.size( ) > 0 )
					{
						printf( "DEBUG : Stage = %2ld, Weaks = %2ld, TP = %6.2f, FP = %6.2f, CFP = %6.2f [%s]\n", exits_.size( ), num_weaks, current_true_positive_rate * 100.0, total_false_positive_rate * 100.0, current_false_positive_rate * 100.0, info.c_str( ) );
					}
					else
					{
						printf( "DEBUG : Stage = %2ld, Weaks = %2ld, TP = %6.2f, FP = %6.2f, CFP = %6.2f\n", exits_.size( ), num_weaks, current_true_positive_rate * 100.0, total_false_positive_rate * 100.0, current_false_positive_rate * 100.0 );
					}
				}
#else
				current_true_positive_rate = this->true_positive_rate( flist );
				current_false_positive_rate = this->false_positive_rate( flist );
#endif

				// 性能を評価する
				double alpha = current_false_positive_rate;
				double beta  = 1.0 - current_true_positive_rate;

				if( num_weaks >= minimum_number_of_classifiers_per_step && alpha <= alpha0 && beta <= beta0 )
				{
					break;
				}
			}

			// 弱識別器数を増加させても，目標の誤検出率を下回れない場合は，
			// 条件が厳しすぎるため学習を終了する
			if( num_weaks > max_weaks )
			{
#if defined( __ADABOOST_OUTPUT_DEBUG_STRING__ ) && __ADABOOST_OUTPUT_DEBUG_STRING__ != 0
				printf( "------------------------------------- REMOVED -------------------------------------\n" );
#endif

				// 学習に失敗したためリストから除外する
				exits_.pop_back( );
				if( exits_.empty( ) )
				{
					classifiers_.clear( );
				}
				else
				{
					classifiers_.erase( classifiers_.begin( ) + exits_.back( ) + 1, classifiers_.end( ) );
				}

				return( false );
			}

			// 現在のステージのMulti-exit型カスケード識別器の検出率と誤検出率を計算する
			{
				double tmp[ 5 ];
				false_positive_rate( flist, tmp );
				last_false_positive_rate_[ 1 ] = last_false_positive_rate_[ 1 ] - ( tmp[ 2 ] - tmp[ 1 ] ) + tmp[ 3 ];
				last_false_positive_rate_[ 2 ] = last_false_positive_rate_[ 2 ] + tmp[ 4 ];
				last_false_positive_rate_[ 0 ] = last_false_positive_rate_[ 1 ] / last_false_positive_rate_[ 2 ];
				last_false_positive_rate_[ 3 ] = tmp[ 0 ];

				last_true_positive_rate_  = current_true_positive_rate;

				double total_FP = this->last_false_positive_rate( );
				double total_TP = this->last_true_positive_rate( );
				double current_FP = tmp[ 0 ];

				// 学習結果を記録する
				std::string str = "TP(total): " + utility::ToString( total_TP, "%f" ) + ", FP(total): " + utility::ToString( total_FP, "%f" ) + ", FP(current): " + utility::ToString( current_FP, "%f" );

#if defined( __ADABOOST_OUTPUT_DEBUG_STRING__ ) && __ADABOOST_OUTPUT_DEBUG_STRING__ != 0
				printf( "-----------------------------------------------------------------------------------\n" );
#endif
			}

			// 最終的な誤検出率を下回れない場合は，現在のMulti-exit型カスケード識別器で
			// 取り除ける Negative データを学習データから削除する
			if( this->last_false_positive_rate( ) > this->total_false_positive_rate( ) )
			{
				feature_reference_list_type::iterator ite = flist.begin( );
				if( nstages + 1 == number_of_exits )
				{
					for( ; ite != flist.end( ) ; ++ite )
					{
						ite->is_new( false );
						if( ( ( const derived_type * ) this )->evaluate( *ite ) < 0 )
						{
							ite->is_valid( false );
						}
					}
				}
				else
				{
					while( ite != flist.end( ) )
					{
						ite->is_new( false );
						if( ( ( const derived_type * ) this )->evaluate( *ite ) < 0 )
						{
							ite->is_valid( false );
							ite = flist.erase( ite );
						}
						else
						{
							++ite;
						}
					}
				}
			}

			if( current_false_positive_rate == 0 )
			{
				break;
			}

			// ステージを 1 進める
			nstages++;
		}

		return( true );
	}


public:
	/// @brief 学習済みの識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f … 分類する特徴量
	//! 
	//! @return 分類結果
	//! 
	template < class FEATURE >
	bool operator ()( const FEATURE &f ) const
	{
		return( ( ( const derived_type * ) this )->evaluate( f ) >= 0 );
	}

	/// @brief 学習済みの別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f … 特徴量を計算する関数オブジェクト
	//! @param[in]  p … 特徴量計算に使用するパラメータ
	//! 
	//! @return 分類結果
	//! 
	template < class FUNCTOR, class PARAMS >
	bool operator ()( const FUNCTOR &f, const PARAMS &p ) const
	{
		return( ( ( const derived_type * ) this )->evaluate( p, f ) >= 0 );
	}

	/// @brief 学習済みの識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2 >
	bool operator ()( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2 ) const
	{
		return( ( ( const derived_type * ) this )->evaluate( f, p1, p2 ) >= 0 );
	}

	/// @brief 学習済みの識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  p3 … 特徴量計算に使用するパラメータ3
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3 >
	bool operator ()( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3 ) const
	{
		return( ( ( const derived_type * ) this )->evaluate( f, p1, p2, p3 ) >= 0 );
	}

	/// @brief 学習済みの識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  p3 … 特徴量計算に使用するパラメータ3
	//! @param[in]  p4 … 特徴量計算に使用するパラメータ4
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4 >
	bool operator ()( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, const PARAM4 &p4 ) const
	{
		return( ( ( const derived_type * ) this )->evaluate( f, p1, p2, p3, p4 ) >= 0 );
	}

	/// @brief 学習済みの識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  p3 … 特徴量計算に使用するパラメータ3
	//! @param[in]  p4 … 特徴量計算に使用するパラメータ4
	//! @param[in]  p5 … 特徴量計算に使用するパラメータ5
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5 >
	bool operator ()( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, const PARAM4 &p4, const PARAM5 &p5 ) const
	{
		return( ( ( const derived_type * ) this )->evaluate( f, p1, p2, p3, p4, p5 ) >= 0 );
	}

	/// @brief 学習済みの識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f … 分類する特徴量
	//! 
	//! @return 分類結果
	//! 
	template < class FEATURE >
	double evaluate( const FEATURE &f ) const
	{
		return( confidence( f ) );
	}

	/// @brief 学習済みの識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f … 特徴量を計算する関数オブジェクト
	//! @param[in]  p … 特徴量計算に使用するパラメータ
	//! 
	//! @return 分類結果
	//! 
	template < class FUNCTOR, class PARAMS >
	double evaluate( const FUNCTOR &f, const PARAMS &p ) const
	{
		return( confidence( f, p ) );
	}

	/// @brief 学習済みの識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! 
	//! @return 分類結果
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2 >
	double evaluate( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2 ) const
	{
		return( confidence( f, p1, p2 ) );
	}

	/// @brief 学習済みの識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  p3 … 特徴量計算に使用するパラメータ3
	//! 
	//! @return 分類結果
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3 >
	double evaluate( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3 ) const
	{
		return( confidence( f, p1, p2, p3 ) );
	}

	/// @brief 学習済みの識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  p3 … 特徴量計算に使用するパラメータ3
	//! @param[in]  p4 … 特徴量計算に使用するパラメータ4
	//! 
	//! @return 分類結果
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4 >
	double evaluate( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, const PARAM4 &p4 ) const
	{
		return( confidence( f, p1, p2, p3, p4 ) );
	}

	/// @brief 学習済みの識別器を用いて特徴量を分類する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  p3 … 特徴量計算に使用するパラメータ3
	//! @param[in]  p4 … 特徴量計算に使用するパラメータ4
	//! @param[in]  p5 … 特徴量計算に使用するパラメータ5
	//! 
	//! @return 分類結果
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5 >
	double evaluate( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, const PARAM4 &p4, const PARAM5 &p5 ) const
	{
		return( confidence( f, p1, p2, p3, p4, p5 ) );
	}

public:
	/// @brief 追加の教師データを用いて弱識別器を更新する
	//! 
	//! @param[in]  f           … 分類する特徴量ベクトル
	//! @param[in]  is_positive … ポジティブサンプルかどうか
	//! 
	template < class FEATURE >
	void update( const FEATURE &f, bool is_positive )
	{
		size_type sid = 0;
		double lambda = 1.0;
		double weight = is_positive ? base::positive_weight( ) : base::negative_weight( );
		for( size_type i = 0 ; i < exits_.size( ) ; i++ )
		{
			size_type eid = exits_[ i ];
			for( size_type t = sid ; t < eid ; t++ )
			{
				base_classifier_type &weak = classifiers_[ t ];

				weak.update( f, lambda * weight, is_positive );

				double v = weak.evaluate( f );
				if( ( is_positive ? +1.0 : -1.0 ) * v >= 0.0 )
				{
					weak.lambda_c( weak.lambda_c( ) + lambda * std::abs( v ) );

					double err = weak.lambda_w( ) / ( weak.lambda_c( ) + weak.lambda_w( ) );
					lambda /= 2.0 * ( 1.0 - err );
				}
				else
				{
					weak.lambda_w( weak.lambda_w( ) + lambda * std::abs( v ) );

					double err = weak.lambda_w( ) / ( weak.lambda_c( ) + weak.lambda_w( ) );
					lambda /= 2.0 * err;
				}
			}

			double val = 0.0;
			for( size_type t = 0 ; t < eid ; t++ )
			{
				base_classifier_type &weak = classifiers_[ t ];
				val += weak.evaluate( f ) * weak.weight( );
			}

			if( val < 0 )
			{
				break;
			}

			sid = eid;
		}
	}


	/// @brief 追加の教師データを用いて弱識別器を更新する
	//! 
	//! @param[in]  f … 特徴量を計算する関数オブジェクト
	//! @param[in]  p … 特徴量計算に使用するパラメータ
	//! @param[in]  is_positive … ポジティブサンプルかどうか
	//! 
	template < class FUNCTOR, class PARAMS >
	void update( const FUNCTOR &f, const PARAMS &p, bool is_positive )
	{
		size_type sid = 0;
		double lambda = 1.0;
		double weight = is_positive ? base::positive_weight( ) : base::negative_weight( );
		for( size_type i = 0 ; i < exits_.size( ) ; i++ )
		{
			size_type eid = exits_[ i ];
			for( size_type t = sid ; t < eid ; t++ )
			{
				base_classifier_type &weak = classifiers_[ t ];

				weak.update( f, p, lambda * weight, is_positive );

				double v = weak.evaluate( f, p );
				if( ( is_positive ? +1.0 : -1.0 ) * v >= 0.0 )
				{
					weak.lambda_c( weak.lambda_c( ) + lambda * std::abs( v ) );

					double err = weak.lambda_w( ) / ( weak.lambda_c( ) + weak.lambda_w( ) );
					lambda /= 2.0 * ( 1.0 - err );
				}
				else
				{
					weak.lambda_w( weak.lambda_w( ) + lambda * std::abs( v ) );

					double err = weak.lambda_w( ) / ( weak.lambda_c( ) + weak.lambda_w( ) );
					lambda /= 2.0 * err;
				}
			}

			double val = 0.0;
			for( size_type t = 0 ; t < eid ; t++ )
			{
				base_classifier_type &weak = classifiers_[ t ];
				val += weak.evaluate( f, p ) * weak.weight( );
			}

			if( val < 0 )
			{
				break;
			}

			sid = eid;
		}
	}

	/// @brief 追加の教師データを用いて弱識別器を更新する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  is_positive … ポジティブサンプルかどうか
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2 >
	void update( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, bool is_positive )
	{
		size_type sid = 0;
		double lambda = 1.0;
		double weight = is_positive ? base::positive_weight( ) : base::negative_weight( );
		for( size_type i = 0 ; i < exits_.size( ) ; i++ )
		{
			size_type eid = exits_[ i ];
			for( size_type t = sid ; t < eid ; t++ )
			{
				base_classifier_type &weak = classifiers_[ t ];

				weak.update( f, p1, p2, lambda * weight, is_positive );

				double v = weak.evaluate( f, p1, p2 );
				if( ( is_positive ? +1.0 : -1.0 ) * v >= 0.0 )
				{
					weak.lambda_c( weak.lambda_c( ) + lambda * std::abs( v ) );

					double err = weak.lambda_w( ) / ( weak.lambda_c( ) + weak.lambda_w( ) );
					lambda /= 2.0 * ( 1.0 - err );
				}
				else
				{
					weak.lambda_w( weak.lambda_w( ) + lambda * std::abs( v ) );

					double err = weak.lambda_w( ) / ( weak.lambda_c( ) + weak.lambda_w( ) );
					lambda /= 2.0 * err;
				}
			}

			double val = 0.0;
			for( size_type t = 0 ; t < eid ; t++ )
			{
				base_classifier_type &weak = classifiers_[ t ];
				val += weak.evaluate( f, p1, p2 ) * weak.weight( );
			}

			if( val < 0 )
			{
				break;
			}

			sid = eid;
		}
	}

	/// @brief 追加の教師データを用いて弱識別器を更新する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  p3 … 特徴量計算に使用するパラメータ3
	//! @param[in]  is_positive … ポジティブサンプルかどうか
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3 >
	void update( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, bool is_positive )
	{
		size_type sid = 0;
		double lambda = 1.0;
		double weight = is_positive ? base::positive_weight( ) : base::negative_weight( );
		for( size_type i = 0 ; i < exits_.size( ) ; i++ )
		{
			size_type eid = exits_[ i ];
			for( size_type t = sid ; t < eid ; t++ )
			{
				base_classifier_type &weak = classifiers_[ t ];

				weak.update( f, p1, p2, p3, lambda * weight, is_positive );

				double v = weak.evaluate( f, p1, p2, p3 );
				if( ( is_positive ? +1.0 : -1.0 ) * v >= 0.0 )
				{
					weak.lambda_c( weak.lambda_c( ) + lambda * std::abs( v ) );

					double err = weak.lambda_w( ) / ( weak.lambda_c( ) + weak.lambda_w( ) );
					lambda /= 2.0 * ( 1.0 - err );
				}
				else
				{
					weak.lambda_w( weak.lambda_w( ) + lambda * std::abs( v ) );

					double err = weak.lambda_w( ) / ( weak.lambda_c( ) + weak.lambda_w( ) );
					lambda /= 2.0 * err;
				}
			}

			double val = 0.0;
			for( size_type t = 0 ; t < eid ; t++ )
			{
				base_classifier_type &weak = classifiers_[ t ];
				val += weak.evaluate( f, p1, p2, p3 ) * weak.weight( );
			}

			if( val < 0 )
			{
				break;
			}

			sid = eid;
		}
	}

	/// @brief 追加の教師データを用いて弱識別器を更新する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  p3 … 特徴量計算に使用するパラメータ3
	//! @param[in]  p4 … 特徴量計算に使用するパラメータ4
	//! @param[in]  is_positive … ポジティブサンプルかどうか
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4 >
	void update( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, const PARAM4 &p4, bool is_positive )
	{
		size_type sid = 0;
		double lambda = 1.0;
		double weight = is_positive ? base::positive_weight( ) : base::negative_weight( );
		for( size_type i = 0 ; i < exits_.size( ) ; i++ )
		{
			size_type eid = exits_[ i ];
			for( size_type t = sid ; t < eid ; t++ )
			{
				base_classifier_type &weak = classifiers_[ t ];

				weak.update( f, p1, p2, p3, p4, lambda * weight, is_positive );

				double v = weak.evaluate( f, p1, p2, p3, p4 );
				if( ( is_positive ? +1.0 : -1.0 ) * v >= 0.0 )
				{
					weak.lambda_c( weak.lambda_c( ) + lambda * std::abs( v ) );

					double err = weak.lambda_w( ) / ( weak.lambda_c( ) + weak.lambda_w( ) );
					lambda /= 2.0 * ( 1.0 - err );
				}
				else
				{
					weak.lambda_w( weak.lambda_w( ) + lambda * std::abs( v ) );

					double err = weak.lambda_w( ) / ( weak.lambda_c( ) + weak.lambda_w( ) );
					lambda /= 2.0 * err;
				}
			}

			double val = 0.0;
			for( size_type t = 0 ; t < eid ; t++ )
			{
				base_classifier_type &weak = classifiers_[ t ];
				val += weak.evaluate( f, p1, p2, p3, p4 ) * weak.weight( );
			}

			if( val < 0 )
			{
				break;
			}

			sid = eid;
		}
	}

	/// @brief 追加の教師データを用いて弱識別器を更新する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  p3 … 特徴量計算に使用するパラメータ3
	//! @param[in]  p4 … 特徴量計算に使用するパラメータ4
	//! @param[in]  is_positive … ポジティブサンプルかどうか
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5 >
	void update( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, const PARAM4 &p4, const PARAM5 &p5, bool is_positive )
	{
		size_type sid = 0;
		double lambda = 1.0;
		double weight = is_positive ? base::positive_weight( ) : base::negative_weight( );
		for( size_type i = 0 ; i < exits_.size( ) ; i++ )
		{
			size_type eid = exits_[ i ];
			for( size_type t = sid ; t < eid ; t++ )
			{
				base_classifier_type &weak = classifiers_[ t ];

				weak.update( f, p1, p2, p3, p4, p5, lambda * weight, is_positive );

				double v = weak.evaluate( f, p1, p2, p3, p4, p5 );
				if( ( is_positive ? +1.0 : -1.0 ) * v >= 0.0 )
				{
					weak.lambda_c( weak.lambda_c( ) + lambda * std::abs( v ) );

					double err = weak.lambda_w( ) / ( weak.lambda_c( ) + weak.lambda_w( ) );
					lambda /= 2.0 * ( 1.0 - err );
				}
				else
				{
					weak.lambda_w( weak.lambda_w( ) + lambda * std::abs( v ) );

					double err = weak.lambda_w( ) / ( weak.lambda_c( ) + weak.lambda_w( ) );
					lambda /= 2.0 * err;
				}
			}

			double val = 0.0;
			for( size_type t = 0 ; t < eid ; t++ )
			{
				base_classifier_type &weak = classifiers_[ t ];
				val += weak.evaluate( f, p1, p2, p3, p4, p5 ) * weak.weight( );
			}

			if( val < 0 )
			{
				break;
			}

			sid = eid;
		}
	}

public:
	/// @brief 学習済みの識別器を用いて特徴量の信頼度を計算する
	//! 
	//! @param[in]  f … 分類する特徴量
	//! 
	//! @return 分類結果
	//! 
	template < class FEATURE >
	double confidence( const FEATURE &f ) const
	{
		double val = 0.0;
		size_type sid = 0;
		for( size_type j = 0 ; j < exits_.size( ) ; j++ )
		{
			size_type eid = exits_[ j ];
			for( size_type i = sid ; i < eid ; i++ )
			{
				val += classifiers_[ i ].evaluate( f ) * classifiers_[ i ].weight( );
			}

			if( val < 0 )
			{
				return( val );
			}

			sid = eid;
		}

		return( val );
	}

	/// @brief 学習済みの識別器を用いて特徴量の信頼度を計算する
	//! 
	//! @param[in]  f … 特徴量を計算する関数オブジェクト
	//! @param[in]  p … 特徴量計算に使用するパラメータ
	//! 
	//! @return 分類結果
	//! 
	template < class FUNCTOR, class PARAMS >
	double confidence( const FUNCTOR &f, const PARAMS &p ) const
	{
		double val = 0.0;
		size_type sid = 0;
		for( size_type j = 0 ; j < exits_.size( ) ; j++ )
		{
			size_type eid = exits_[ j ];
			for( size_type i = sid ; i < eid ; i++ )
			{
				val += classifiers_[ i ].evaluate( f, p ) * classifiers_[ i ].weight( );
			}

			if( val < 0 )
			{
				return( val );
			}

			sid = eid;
		}

		return( val );
	}

	/// @brief 学習済みの識別器を用いて特徴量の信頼度を計算する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! 
	//! @return 分類結果
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2 >
	double confidence( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2 ) const
	{
		double val = 0.0;
		size_type sid = 0;
		for( size_type j = 0 ; j < exits_.size( ) ; j++ )
		{
			size_type eid = exits_[ j ];
			for( size_type i = sid ; i < eid ; i++ )
			{
				val += classifiers_[ i ].evaluate( f, p1, p2 ) * classifiers_[ i ].weight( );
			}

			if( val < 0 )
			{
				return( val );
			}

			sid = eid;
		}

		return( val );
	}

	/// @brief 学習済みの識別器を用いて特徴量の信頼度を計算する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  p3 … 特徴量計算に使用するパラメータ3
	//! 
	//! @return 分類結果
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3 >
	double confidence( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3 ) const
	{
		double val = 0.0;
		size_type sid = 0;
		for( size_type j = 0 ; j < exits_.size( ) ; j++ )
		{
			size_type eid = exits_[ j ];
			for( size_type i = sid ; i < eid ; i++ )
			{
				val += classifiers_[ i ].evaluate( f, p1, p2, p3 ) * classifiers_[ i ].weight( );
			}

			if( val < 0 )
			{
				return( val );
			}

			sid = eid;
		}

		return( val );
	}

	/// @brief 学習済みの識別器を用いて特徴量の信頼度を計算する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  p3 … 特徴量計算に使用するパラメータ3
	//! @param[in]  p4 … 特徴量計算に使用するパラメータ4
	//! 
	//! @return 分類結果
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4 >
	double confidence( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, const PARAM4 &p4 ) const
	{
		double val = 0.0;
		size_type sid = 0;
		for( size_type j = 0 ; j < exits_.size( ) ; j++ )
		{
			size_type eid = exits_[ j ];
			for( size_type i = sid ; i < eid ; i++ )
			{
				val += classifiers_[ i ].evaluate( f, p1, p2, p3, p4 ) * classifiers_[ i ].weight( );
			}

			if( val < 0 )
			{
				return( val );
			}

			sid = eid;
		}

		return( val );
	}

	/// @brief 学習済みの識別器を用いて特徴量の信頼度を計算する
	//! 
	//! @param[in]  f  … 特徴量を計算する関数オブジェクト
	//! @param[in]  p1 … 特徴量計算に使用するパラメータ1
	//! @param[in]  p2 … 特徴量計算に使用するパラメータ2
	//! @param[in]  p3 … 特徴量計算に使用するパラメータ3
	//! @param[in]  p4 … 特徴量計算に使用するパラメータ4
	//! 
	//! @return 分類結果
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5 >
	double confidence( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, const PARAM4 &p4, const PARAM5 &p5 ) const
	{
		double val = 0.0;
		size_type sid = 0;
		for( size_type j = 0 ; j < exits_.size( ) ; j++ )
		{
			size_type eid = exits_[ j ];
			for( size_type i = sid ; i < eid ; i++ )
			{
				val += classifiers_[ i ].evaluate( f, p1, p2, p3, p4, p5 ) * classifiers_[ i ].weight( );
			}

			if( val < 0 )
			{
				return( val );
			}

			sid = eid;
		}

		return( val );
	}

public:
	/// @brief 特徴量のリストからTP率を計算する
	//! 
	//! @param[in]  features             … TP率の計算に使用する特徴量のリスト
	//! @param[in]  force_calculate_rate … 無効に設定されている特徴量も計算に使用するかどうか（デフォルトはfalse）
	//! 
	template < template < typename, typename > class FEATURE_LIST, class FEATURE, class ALLOCATOR >
	double true_positive_rate( const FEATURE_LIST< FEATURE, ALLOCATOR > &features, bool force_calculate_rate = false )
	{
		if( features.empty( ) )
		{
			return( 0.0 );
		}
		else
		{
			double rates[ 5 ];
			true_positive_rate( features, rates, force_calculate_rate );
			return( rates[ 0 ] );
		}
	}

	/// @brief 特徴量のリストからFP率を計算する
	//! 
	//! @param[in]  features             … FP率の計算に使用する特徴量のリスト
	//! @param[in]  force_calculate_rate … 無効に設定されている特徴量も計算に使用するかどうか（デフォルトはfalse）
	//! 
	template < template < typename, typename > class FEATURE_LIST, class FEATURE, class ALLOCATOR >
	double false_positive_rate( const FEATURE_LIST< FEATURE, ALLOCATOR > &features, bool force_calculate_rate = false )
	{
		if( features.empty( ) )
		{
			return( 0.0 );
		}
		else
		{
			double rates[ 5 ];
			false_positive_rate( features, rates, force_calculate_rate );
			return( rates[ 0 ] );
		}
	}

	/// @brief 特徴量のリストからTP率を計算する
	//! 
	//! @param[in]  features             … TP率の計算に使用する特徴量のリスト
	//! @param[out] rates                … 旧データと新データに分けて計算したTP率
	//! @param[in]  force_calculate_rate … 無効に設定されている特徴量も計算に使用するかどうか（デフォルトはfalse）
	//! 
	template < template < typename, typename > class FEATURE_LIST, class FEATURE, class ALLOCATOR >
	void true_positive_rate( const FEATURE_LIST< FEATURE, ALLOCATOR > &features, double rates[ 5 ], bool force_calculate_rate = false )
	{
		rates[ 0 ] = rates[ 1 ] = rates[ 2 ] = rates[ 3 ] = rates[ 4 ] = 0.0;

		if( features.empty( ) )
		{
			return;
		}			

		typedef FEATURE feature_type;
		typedef typename FEATURE_LIST< FEATURE, ALLOCATOR >::const_iterator const_iterator;

		size_t positives[ 2 ]  = { 0, 0 };
		size_t detections[ 2 ] = { 0, 0 };
		for( const_iterator ite = features.begin( ) ; ite != features.end( ) ; ++ite )
		{
			const feature_type &f = *ite;
			if( force_calculate_rate || f.is_valid( ) )
			{
				if( f.is_positive( ) )
				{
					positives[ f.is_new( ) ? 1 : 0 ]++;

					if( ( ( const derived_type * ) this )->evaluate( f ) >= 0 )
					{
						detections[ f.is_new( ) ? 1 : 0 ]++;
					}
				}
			}
		}

		size_t num = positives[ 0 ] + positives[ 1 ];
		rates[ 0 ] = static_cast< double >( detections[ 0 ] + detections[ 1 ] ) / static_cast< double >( num > 0 ? num : 1 );
		rates[ 1 ] = static_cast< double >( detections[ 0 ] );	// 従来データ
		rates[ 2 ] = static_cast< double >( positives[ 0 ] );	// 従来データ
		rates[ 3 ] = static_cast< double >( detections[ 1 ] );	// 新規データ
		rates[ 4 ] = static_cast< double >( positives[ 1 ] );	// 新規データ
	}
	
	/// @brief 特徴量のリストからFP率を計算する
	//! 
	//! @param[in]  features             … TP率の計算に使用する特徴量のリスト
	//! @param[out] rates                … 旧データと新データに分けて計算したFP率
	//! @param[in]  force_calculate_rate … 無効に設定されている特徴量も計算に使用するかどうか（デフォルトはfalse）
	//! 
	template < template < typename, typename > class FEATURE_LIST, class FEATURE, class ALLOCATOR >
	void false_positive_rate( const FEATURE_LIST< FEATURE, ALLOCATOR > &features, double rates[ 5 ], bool force_calculate_rate = false )
	{
		rates[ 0 ] = rates[ 1 ] = rates[ 2 ] = rates[ 3 ] = rates[ 4 ] = 0.0;

		if( features.empty( ) )
		{
			return;
		}			

		typedef FEATURE feature_type;
		typedef typename FEATURE_LIST< FEATURE, ALLOCATOR >::const_iterator const_iterator;

		size_t negatives[ 2 ]       = { 0, 0 };
		size_t miss_detections[ 2 ] = { 0, 0 };
		for( const_iterator ite = features.begin( ) ; ite != features.end( ) ; ++ite )
		{
			const feature_type &f = *ite;
			if( force_calculate_rate || f.is_valid( ) )
			{
				if( f.is_negative( ) )
				{
					negatives[ f.is_new( ) ? 1 : 0 ]++;

					if( ( ( const derived_type * ) this )->evaluate( f ) >= 0 )
					{
						miss_detections[ f.is_new( ) ? 1 : 0 ]++;
					}
				}
			}
		}

		size_t num = negatives[ 0 ] + negatives[ 1 ];
		rates[ 0 ] = static_cast< double >( miss_detections[ 0 ] + miss_detections[ 1 ] ) / static_cast< double >( num > 0 ? num : 1 );
		rates[ 1 ] = static_cast< double >( miss_detections[ 0 ] );	// 従来データ
		rates[ 2 ] = static_cast< double >( negatives[ 0 ] );		// 従来データ
		rates[ 3 ] = static_cast< double >( miss_detections[ 1 ] );	// 新規データ
		rates[ 4 ] = static_cast< double >( negatives[ 1 ] );		// 新規データ
	}
};

// AdaBoost名前空間の終わり
_ADABOOST_END


#endif	// __INCLUDE_ADABOOST_CLASSIFIER_BASE__