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

#ifndef __INCLUDE_REAL_ADABOOST_CLASSIFIER__
#define __INCLUDE_REAL_ADABOOST_CLASSIFIER__

#include <vector>
#include <list>
#include <algorithm>

#include <libxml/xmlreader.h>
#include <libxml/xmlwriter.h>


#ifndef __INCLUDE_ADABOOST_COMMON__
#include "common.h"
#endif

#ifndef __INCLUDE_ADABOOST_UTILITY__
#include "utility.h"
#endif

#ifndef __INCLUDE_ADABOOST_FEATURE__
#include "feature.h"
#endif

#ifndef __INCLUDE_ADABOOST_CLASSIFIER_BASE__
#include "classifier_base.h"
#endif

#if !defined( __NO_ADABOOST_NAMESPACE__ ) || __NO_ADABOOST_NAMESPACE__ == 0
// AdaBoost名前空間の始まり
_ADABOOST_BEGIN
#endif


/// @brief AdaBoost で利用するしきい値処理を用いた弱識別器
class weak_classifier : public weak_classifier_base< weak_classifier >
{
private:
	typedef weak_classifier_base< weak_classifier > base;	///< @brief 基底クラス

public:
	typedef feature::value_type			value_type;			///< @brief MISTのコンテナ内に格納するデータ型．mist::array< data > の data と同じ
	typedef feature::size_type			size_type;			///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef feature::difference_type	difference_type;	///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ

private:
	size_type num_bins_;
	std::vector< double > hist_;							///< @brief 識別用の値を保持する配列
	std::vector< double > pos_;								///< @brief 識別用のポジティブサンプル側の値を保持する配列
	std::vector< double > neg_;								///< @brief 識別用のネガティブサンプル側の値を保持する配列

public:
	size_type size( ) const { return( hist_.size( ) ); }

	/// @brief 識別器で使用する確率密度関数の各値へアクセスするオペレータ
	//! 
	//! @param[in]  indx … 確率密度関数の値を取得するインデックス
	//! 
	const double & operator []( size_type indx ) const
	{
		return( hist_[ indx ] );
	}

	/// @brief 識別器で使用する確率密度関数の各値へアクセスするオペレータ
	//! 
	//! @param[in]  indx … 確率密度関数の値を取得するインデックス
	//! 
	double & operator []( size_type indx )
	{
		return( hist_[ indx ] );
	}

	/// @brief 識別器で使用するポジティブサンプル側の確率密度関数の各値へアクセスする関数
	//! 
	//! @param[in]  indx  … 確率密度関数の値を設定するインデックス
	//! @param[in]  value … 確率密度関数へ設定する値
	//! 
	void pos( size_type indx, double value )
	{
		pos_[ indx ] = value;
	}

	/// @brief 識別器で使用するポジティブサンプル側の確率密度関数の各値へアクセスする関数
	//! 
	//! @param[in]  indx … 確率密度関数の値を取得するインデックス
	//! 
	double pos( size_type indx ) const
	{
		return( pos_[ indx ] );
	}

	/// @brief 識別器で使用するネガティブサンプル側の確率密度関数の各値へアクセスする関数
	//! 
	//! @param[in]  indx  … 確率密度関数の値を設定するインデックス
	//! @param[in]  value … 確率密度関数へ設定する値
	//! 
	void neg( size_type indx, double value )
	{
		neg_[ indx ] = value;
	}

	/// @brief 識別器で使用するネガティブサンプル側の確率密度関数の各値へアクセスする関数
	//! 
	//! @param[in]  indx … 確率密度関数の値を取得するインデックス
	//! 
	double neg( size_type indx ) const
	{
		return( neg_[ indx ] );
	}

	/// @brief 弱識別器で使用するヒストグラムのビン数を変更する関数
	//! 
	//! @param[in]  nbins … 確率密度関数へ設定する値
	//! 
	void resize( size_type nbins )
	{
		if( nbins != hist_.size( ) )
		{
			std::vector< double > nh( nbins );
			std::vector< double > np( nbins );
			std::vector< double > nn( nbins );

			for( size_type i = 0 ; i < nbins ; i++ )
			{
				nh[ i ] = hist_[ i ];
				np[ i ] = pos_[ i ];
				nn[ i ] = neg_[ i ];
			}

			hist_.swap( nh );
			pos_.swap( np );
			neg_.swap( nn );
		}
	}

public:
	/// @brief デフォルトのコンストラクタ
	//! 
	//! @param[in]  indx … 弱識別器で使用する特徴量のインデックス（ID）
	//! @param[in]  s    … 弱識別器の符号
	//! @param[in]  th   … 弱識別器のしきい値
	//! 
	weak_classifier( size_type indx = 0, size_type nbins = __HISTOGRAM_CLASSIFIER_NUM_BINS__ ) : base( indx ), hist_( nbins ), pos_( nbins ), neg_( nbins )
	{
	}

	/// @brief コピーコンストラクタ
	weak_classifier( const weak_classifier& w ) : base( w ), hist_( w.hist_ ), pos_( w.pos_ ), neg_( w.neg_ )
	{
	}

	/// @brief 他の識別器と同じパラメータの識別器となるようにデータをコピーする
	weak_classifier& operator =( const weak_classifier& o )
	{
		if( this != &o )
		{
			base::operator =( o );

			hist_      = o.hist_;
			pos_       = o.pos_;
			neg_       = o.neg_;
		}

		return( *this );
	}

public:
	/// @brief 教師データを用いて最適な弱識別器を構築する
	//! 
	//! @param[in]  features   … 学習に用いる教師データ
	//! @param[in]  categories … 学習データのカテゴリ（true もしくは false）
	//! 
	template < template < typename, typename > class FEATURE_LIST, class FEATURE, class ALLOCATOR >
	double learn( const FEATURE_LIST< FEATURE, ALLOCATOR > & features )
	{
		typedef FEATURE feature_type;
		typedef typename FEATURE_LIST< FEATURE, ALLOCATOR >::const_iterator const_iterator;

		double _minimum_classification_error_ = 1.0e100;

		// Positive と Negative それぞれの重みの合計を計算する
		size_type nfeatures = 0;
		for( const_iterator ite = features.begin( ) ; ite != features.end( ) ; ++ite )
		{
			const feature_type &f = *ite;
			if( f.is_valid( ) )
			{
				if( nfeatures < f.number_of_dimentions( ) )
				{
					nfeatures = f.number_of_dimentions( );
					break;
				}
			}
		}

		std::vector< double > pos__, neg__;

		#pragma omp parallel for firstprivate( nfeatures, pos__, neg__ ) schedule( guided ) shared( _minimum_classification_error_ )
		for( int index = 0 ; index < ( int )nfeatures ; index++ )
		{
			pos__.resize( hist_.size( ) + 4 );
			neg__.resize( hist_.size( ) + 4 );

			double *_pos_ = &pos__[ 2 ];
			double *_neg_ = &neg__[ 2 ];
			for( size_type i = 0 ; i < pos__.size( ) ; i++ )
			{
				pos__[ i ] = 0;
				neg__[ i ] = 0;
			}

			for( const_iterator ite = features.begin( ) ; ite != features.end( ) ; ++ite )
			{
				const feature_type &f = *ite;
				if( f.is_valid( ) )
				{
					difference_type indx = static_cast< difference_type >( f[ index ] );

#if !defined( __FEATURE_NORMALIZED_TRAINING__ ) || __FEATURE_NORMALIZED_TRAINING__ == 0
					if( f.is_positive( ) )
					{
						_pos_[ indx ] += f.weight( );
					}
					else
					{
						_neg_[ indx ] += f.weight( );
					}
#else
					switch( f.type( ) )
					{
					case __ADABOOST_FEATURE_MLBP__:
						if( f.is_positive( ) )
						{
							_pos_[ ( ( indx << 2 ) & 0x00fc ) | ( ( indx >> 6 ) & 0x0003 ) ] += f.weight( ) * 0.011;
							_pos_[ ( ( indx << 1 ) & 0x00fe ) | ( ( indx >> 7 ) & 0x0001 ) ] += f.weight( ) * 0.207;
							_pos_[ indx ]                                                    += f.weight( ) * 0.564;
							_pos_[ ( ( indx >> 1 ) & 0x007f ) | ( ( indx << 7 ) & 0x0080 ) ] += f.weight( ) * 0.207;
							_pos_[ ( ( indx >> 2 ) & 0x003f ) | ( ( indx << 6 ) & 0x00c0 ) ] += f.weight( ) * 0.011;
						}
						else
						{
							_neg_[ ( ( indx << 2 ) & 0x00fc ) | ( ( indx >> 6 ) & 0x0003 ) ] += f.weight( ) * 0.011;
							_neg_[ ( ( indx << 1 ) & 0x00fe ) | ( ( indx >> 7 ) & 0x0001 ) ] += f.weight( ) * 0.207;
							_neg_[ indx ]                                                    += f.weight( ) * 0.564;
							_neg_[ ( ( indx >> 1 ) & 0x007f ) | ( ( indx << 7 ) & 0x0080 ) ] += f.weight( ) * 0.207;
							_neg_[ ( ( indx >> 2 ) & 0x003f ) | ( ( indx << 6 ) & 0x00c0 ) ] += f.weight( ) * 0.011;
						}
						break;

					case __ADABOOST_FEATURE_HAAR__:
						if( f.is_positive( ) )
						{
							_pos_[ indx - 2 ] += f.weight( ) * 0.011;
							_pos_[ indx - 1 ] += f.weight( ) * 0.207;
							_pos_[ indx + 0 ] += f.weight( ) * 0.564;
							_pos_[ indx + 1 ] += f.weight( ) * 0.207;
							_pos_[ indx + 2 ] += f.weight( ) * 0.011;
						}
						else
						{
							_neg_[ indx - 2 ] += f.weight( ) * 0.011;
							_neg_[ indx - 1 ] += f.weight( ) * 0.207;
							_neg_[ indx + 0 ] += f.weight( ) * 0.564;
							_neg_[ indx + 1 ] += f.weight( ) * 0.207;
							_neg_[ indx + 2 ] += f.weight( ) * 0.011;
						}
						break;

					default:
					case __ADABOOST_FEATURE_LRP__:
						if( f.is_positive( ) )
						{
							_pos_[ indx ] += f.weight( );
						}
						else
						{
							_neg_[ indx ] += f.weight( );
						}
						break;
					}
#endif
				}
			}

			// 各しきい値での重み付き誤差を計算し，誤差最小のしきい値を求める
			double error = 0.0;

#if defined( __ADABOOST_ASYMMETRIC_LOSS__ )
			const double c1 = __ADABOOST_ASYMMETRIC_LOSS__;
			const double c2 = 1.0 - c1;

			for( size_type i = 0 ; i < __HISTOGRAM_CLASSIFIER_NUM_BINS__ ; i++ )
			{
				error += std::pow( _pos_[ i ], c2 ) * std::pow( _neg_[ i ], c1 );
			}
#else
			for( size_type i = 0 ; i < __HISTOGRAM_CLASSIFIER_NUM_BINS__ ; i++ )
			{
				error += std::sqrt( _pos_[ i ] * _neg_[ i ] );
			}
#endif

			#pragma omp flush( _minimum_classification_error_ )
			if( _minimum_classification_error_ > error )
			{
				#pragma omp critical
				{
					#pragma omp flush( _minimum_classification_error_ )
					if( _minimum_classification_error_ > error )
					{
						_minimum_classification_error_ = error;
						index_ = index;

						for( size_type ii = 0 ; ii < pos_.size( ) ; ii++ )
						{
							pos_[ ii ] = _pos_[ ii ];
							neg_[ ii ] = _neg_[ ii ];
						}
					}
				}
			}
		}

#if defined( __ADABOOST_ASYMMETRIC_LOSS__ )
		const double c1 = __ADABOOST_ASYMMETRIC_LOSS__;
		const double c2 = 1.0 - c1;

		for( size_type i = 0 ; i < __HISTOGRAM_CLASSIFIER_NUM_BINS__ ; i++ )
		{
			hist_[ i ] = std::log( ( c1 * pos_[ i ] + 1.0e-8 ) / ( c2 * neg_[ i ] + 1.0e-8 ) );
		}
#else
		for( size_type i = 0 ; i < __HISTOGRAM_CLASSIFIER_NUM_BINS__ ; i++ )
		{
			hist_[ i ] = 0.5 * std::log( ( pos_[ i ] + 1.0e-8 ) / ( neg_[ i ] + 1.0e-8 ) );
		}
#endif

		return( _minimum_classification_error_ );
	}

public:
	/// @brief 追加の教師データを用いて弱識別器を更新する
	//! 
	//! @param[in]  f      … 追加の教師データ
	//! @param[in]  weight … 学習する重み
	//! 
	void update_( double f, const double weight, bool is_positive )
	{
		difference_type indx = static_cast< difference_type >( f );
		if( is_positive )
		{
			pos_[ indx ] += weight;
		}
		else
		{
			neg_[ indx ] += weight;
		}

#if defined( __ADABOOST_ASYMMETRIC_LOSS__ )
		const double c1 = __ADABOOST_ASYMMETRIC_LOSS__;
		const double c2 = 1.0 - c1;
		hist_[ indx ] = std::log( ( c1 * pos_[ indx ] + 1.0e-8 ) / ( c2 * neg_[ indx ] + 1.0e-8 ) );
#else
		hist_[ indx ] = 0.5 * std::log( ( pos_[ indx ] + 1.0e-8 ) / ( neg_[ indx ] + 1.0e-8 ) );
#endif
	}

public:
	/// @brief 学習済みの弱識別器を用いて特徴量を分類する
	double evaluate_( double f ) const
	{
		return( hist_[ static_cast< size_type >( f ) ] );
	}
};


/// @brief AdaBoostアルゴリズムを用いて強識別器を構築するクラス
class boosted_classifier : public boosted_classifier_base< boosted_classifier, weak_classifier >
{
private:
	typedef boosted_classifier_base< boosted_classifier, weak_classifier > base;		///< @brief 基底クラス

public:
	typedef feature::value_type value_type;												///< @brief MISTのコンテナ内に格納するデータ型．mist::array< data > の data と同じ
	typedef feature::size_type size_type;												///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef feature::difference_type difference_type;									///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ
	typedef weak_classifier base_classifier_type;										///< @brief AdaBoostで利用する弱識別器
	typedef std::vector< base_classifier_type > classifier_list_type;					///< @brief AdaBoostで利用する弱識別器のリスト

public:
	/// @brief デフォルトのコンストラクタ
	boosted_classifier( ) : base( )
	{
	}

	/// @brief コピーコンストラクタ
	boosted_classifier( const boosted_classifier &b ) : base( b )
	{
	}

	/// @brief デストラクタ
	~boosted_classifier( ){}

	/// @brief 他のAdaBoost識別器の情報をコピーする
	boosted_classifier& operator =( const boosted_classifier &o )
	{
		if( this != &o )
		{
			base::operator =( o );
		}

		return( *this );
	}

public:
	/// @brief 識別器の情報を保存したXMLファイルから設定を復元する
	//! 
	//! @param[in] filename … 識別器の情報を保存したXMLファイル名
	//! 
	virtual bool load( const std::string &filename );

	/// @brief 識別器の情報をXML形式でファイルに保存する
	//! 
	//! @param[in] filename … 識別器の情報を保存するXMLファイル名
	//! 
	virtual bool save( const std::string &filename, const std::string &comment = "" ) const;

public:
	/// @brief 現在の強識別器に弱識別器を追加する
	//! 
	//! @param[in,out]  features                   … 学習に用いる教師データ
	//! @param[in]      num_max_base_classifiers   … 強識別器へ追加する弱識別器の数（デフォルトは100）
	//! @param[in]      min_true_positive_rate     … 強識別器の構築時に満たすべきTPの最小値（デフォルトは-1）．負の値を指定した場合は num_max_base_classifiers まで弱識別器を追加する．
	//! @param[in]      max_false_positive_rate    … 強識別器の構築時に満たすべきFPの最大値（デフォルトは-1）．負の値を指定した場合は num_max_base_classifiers まで弱識別器を追加する．
	//! @param[in]      _output_debug_string_      … 学習の進捗状況を出力するかどうか（デフォルトはfalse）
	//! 
	template < template < typename, typename > class FEATURE_LIST, class FEATURE, class ALLOCATOR, class INFOFUNCTOR >
	bool add_classifiers( FEATURE_LIST< FEATURE, ALLOCATOR > & features, INFOFUNCTOR finfo, size_type num_max_base_classifiers = 100, double min_true_positive_rate = -1.0, double max_false_positive_rate = -1.0, bool _output_debug_string_ = false )
	{
		typedef FEATURE feature_type;
		typedef typename FEATURE_LIST< FEATURE, ALLOCATOR >::iterator iterator;

		if( features.empty( ) )
		{
			return( false );
		}

		// 全体のしきい値を再計算する
		threshold_ = 0.0;

		// AdaBoost により強識別器を学習する
		for( size_type t = 0 ; t < num_max_base_classifiers ; t++ )
		{
			// 重みを正規化する
			double s = 0.0;
			for( iterator ite = features.begin( ) ; ite != features.end( ) ; ++ite )
			{
				const feature_type &f = *ite;
				if( f.is_valid( ) )
				{
					s += f.weight( );
				}
			}
			for( iterator ite = features.begin( ) ; ite != features.end( ) ; ++ite )
			{
				feature_type &f = *ite;
				if( f.is_valid( ) )
				{
					f.weight( f.weight( ) / s );
				}
			}				

			// 弱識別器を学習する
			classifiers_.push_back( base_classifier_type( ) );
			base_classifier_type &weak = classifiers_.back( );
			weak.learn( features );

			//弱識別器の重みは1で固定;
			weak.weight( 1.0 );

			// 重みを更新する
			for( iterator ite = features.begin( ) ; ite != features.end( ) ; ++ite )
			{
				feature_type &f = *ite;
				if( f.is_valid( ) )
				{
					double v = weak.evaluate( f );

#if defined( __ADABOOST_ASYMMETRIC_LOSS__ )
					const double c1 = __ADABOOST_ASYMMETRIC_LOSS__;
					const double c2 = 1.0 - c1;
					f.weight( f.weight( ) * std::exp( - ( f.is_positive( ) ? +c1 : -c2 ) * v ) );
#else
					f.weight( f.weight( ) * std::exp( - ( f.is_positive( ) ? +1.0 : -1.0 ) * v ) );
#endif

					if( ( f.is_positive( ) ? +1.0 : -1.0 ) * v >= 0.0 )
					{
						weak.lambda_c( weak.lambda_c( ) + std::abs( v ) );
					}
					else
					{
						weak.lambda_w( weak.lambda_w( ) + std::abs( v ) );
					}
				}
			}

			if( ( min_true_positive_rate >= 0.0 && max_false_positive_rate >= 0.0 ) || _output_debug_string_ )
			{
				double TP = base::true_positive_rate( features );
				double FP = base::false_positive_rate( features );

				// コンソールに学習状況を出力する
				if( _output_debug_string_ )
				{
					std::string info = finfo( weak.index( ) );
					if( info.size( ) > 0 )
					{
						printf( "DEBUG : Weaks = %2ld, TP = %6.2f, FP = %6.2f, Lc = %f, Lw = %f [%s]\n", classifiers_.size( ), TP * 100.0, FP * 100.0, weak.lambda_c( ), weak.lambda_w( ), info.c_str( ) );
					}
					else
					{
						printf( "DEBUG : Weaks = %2ld, TP = %6.2f, FP = %6.2f, Lc = %f, Lw = %f\n", classifiers_.size( ), TP * 100.0, FP * 100.0, weak.lambda_c( ), weak.lambda_w( ) );
					}
				}

				if( min_true_positive_rate >= 0.0 && max_false_positive_rate >= 0.0 && TP >= min_true_positive_rate && FP <= max_false_positive_rate )
				{
					break;
				}
			}
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
	double evaluate( const FEATURE &f ) const
	{
		return( this->confidence( f ) );
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
		return( this->confidence( f, p ) );
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
		return( this->confidence( f, p1, p2 ) );
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
		return( this->confidence( f, p1, p2, p3 ) );
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
		return( this->confidence( f, p1, p2, p3, p4 ) );
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
		return( this->confidence( f, p1, p2, p3, p4, p5 ) );
	}

	/// @brief 学習済みの識別器を用いて特徴量の信頼度を計算する
	//! 
	//! @param[in]  f … 分類する特徴量
	//! 
	//! @return 分類結果
	//! 
	template < class FEATURE >
	double confidence( const FEATURE &f ) const
	{
		double value = 0.0;

		for( size_type t = 0 ; t < classifiers_.size( ) ; t++ )
		{
			const base_classifier_type &weak = classifiers_[ t ];
			value += weak.evaluate( f ) * weak.weight( );
		}

		return( value - threshold_ );
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
		double value = 0.0;

		for( size_type t = 0 ; t < classifiers_.size( ) ; t++ )
		{
			const base_classifier_type &weak = classifiers_[ t ];
			value += weak.evaluate( f, p ) * weak.weight( );
		}

		return( value - threshold_ );
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
		double value = 0.0;

		for( size_type t = 0 ; t < classifiers_.size( ) ; t++ )
		{
			const base_classifier_type &weak = classifiers_[ t ];
			value += weak.evaluate( f, p1, p2 ) * weak.weight( );
		}

		return( value - threshold_ );
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
		double value = 0.0;

		for( size_type t = 0 ; t < classifiers_.size( ) ; t++ )
		{
			const base_classifier_type &weak = classifiers_[ t ];
			value += weak.evaluate( f, p1, p2, p3 ) * weak.weight( );
		}

		return( value - threshold_ );
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
		double value = 0.0;

		for( size_type t = 0 ; t < classifiers_.size( ) ; t++ )
		{
			const base_classifier_type &weak = classifiers_[ t ];
			value += weak.evaluate( f, p1, p2, p3, p4 ) * weak.weight( );
		}

		return( value - threshold_ );
	}

	/// @brief 学習済みの識別器を用いて特徴量の信頼度を計算する
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
	double confidence( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, const PARAM4 &p4, const PARAM5 &p5 ) const
	{
		double value = 0.0;

		for( size_type t = 0 ; t < classifiers_.size( ) ; t++ )
		{
			const base_classifier_type &weak = classifiers_[ t ];
			value += weak.evaluate( f, p1, p2, p3, p4, p5 ) * weak.weight( );
		}

		return( value - threshold_ );
	}
};


/// @brief カスケード型AdaBoost識別器を構築するクラス
class cascaded_classifier : public cascaded_classifier_base< cascaded_classifier, boosted_classifier >
{
private:
	typedef cascaded_classifier_base< cascaded_classifier, boosted_classifier > base;		///< @brief 基底クラス

public:
	typedef boosted_classifier base_classifier_type;										///< @brief カスケードにする識別器
	typedef std::vector< base_classifier_type > classifier_list_type;						///< @brief カスケードにする識別器のリスト
	typedef feature::value_type value_type;													///< @brief MISTのコンテナ内に格納するデータ型．mist::array< data > の data と同じ
	typedef feature::size_type size_type;													///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef feature::difference_type difference_type;										///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ

public:
	/// @brief デフォルトのコンストラクタ
	cascaded_classifier( ) : base( )
	{
	}

	/// @brief コピーコンストラクタ
	cascaded_classifier( const cascaded_classifier &c ) : base( c )
	{
	}

	/// @brief デストラクタ
	~cascaded_classifier( ){ }

	/// @brief 他のカスケード型AdaBoost識別器の情報をコピーする
	cascaded_classifier& operator =( const cascaded_classifier& o )
	{
		if( this != &o )
		{
			base::operator =( o );
		}

		return( *this );
	}

public:
	/// @brief 識別器の情報を保存したXMLファイルから設定を復元する
	//! 
	//! @param[in] filename … 識別器の情報を保存したXMLファイル名
	//! 
	virtual bool load( const std::string &filename );

	/// @brief 識別器の情報をXML形式でファイルに保存する
	//! 
	//! @param[in] filename … 識別器の情報を保存するXMLファイル名
	//! 
	virtual bool save( const std::string &filename, const std::string &comment = "" ) const;

#if defined( __ADABOOST_CASCADE_TYPE__ ) && ( __ADABOOST_CASCADE_TYPE__ == 2 || __ADABOOST_CASCADE_TYPE__ == 3 )
public:
	/// @brief 各段の強識別器の初期化を行う関数（内部用）
	//! 
	//! @param[in,out] features   … 学習に用いる教師データ
	//! @param[in,out] classifier … 初期化する強識別器
	//! 
	template < template < typename, typename > class FEATURE_LIST, class FEATURE, class ALLOCATOR >
	void initialize__( FEATURE_LIST< FEATURE, ALLOCATOR > & features, base_classifier_type & classifier )
	{
		classifier.initialize( features );

		if( base::number_of_stages( ) > 0 )
		{
			typedef typename FEATURE_LIST< FEATURE, ALLOCATOR >::iterator iterator;
			typedef FEATURE feature_type;

			// 重みを更新する
			for( iterator ite = features.begin( ) ; ite != features.end( ) ; ++ite )
			{
				feature_type &f = *ite;
				if( f.is_valid( ) )
				{
					f.weight( f.weight( ) * std::exp( - ( f.is_positive( ) ? +1 : -1 ) * this->confidence( f ) ) );
				}
			}
		}
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
#endif

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
		for( size_type i = 0 ; i < classifiers_.size( ) ; i++ )
		{
			val += classifiers_[ i ].confidence( f );
			if( val < 0 )
			{
				return( val );
			}
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
		for( size_type i = 0 ; i < classifiers_.size( ) ; i++ )
		{
			val += classifiers_[ i ].confidence( f, p );
			if( val < 0 )
			{
				return( val );
			}
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
		for( size_type i = 0 ; i < classifiers_.size( ) ; i++ )
		{
			val += classifiers_[ i ].confidence( f, p1, p2 );
			if( val < 0 )
			{
				return( val );
			}
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
		for( size_type i = 0 ; i < classifiers_.size( ) ; i++ )
		{
			val += classifiers_[ i ].confidence( f, p1, p2, p3 );
			if( val < 0 )
			{
				return( val );
			}
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
		for( size_type i = 0 ; i < classifiers_.size( ) ; i++ )
		{
			val += classifiers_[ i ].confidence( f, p1, p2, p3, p4 );
			if( val < 0 )
			{
				return( val );
			}
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
	//! @param[in]  p5 … 特徴量計算に使用するパラメータ5
	//! 
	//! @return 分類結果
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5 >
	double confidence( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, const PARAM4 &p4, const PARAM5 &p5 ) const
	{
		double val = 0.0;
		for( size_type i = 0 ; i < classifiers_.size( ) ; i++ )
		{
			val += classifiers_[ i ].confidence( f, p1, p2, p3, p4, p5 );
			if( val < 0 )
			{
				return( val );
			}
		}

		return( val );
	}
};


/// @brief Multi-exit型AdaBoost識別器を構築するクラス
class multi_exit_classifier : public multi_exit_classifier_base< multi_exit_classifier, weak_classifier >
{
private:
	typedef multi_exit_classifier_base< multi_exit_classifier, weak_classifier > base;			///< @brief 基底クラス

public:
	typedef weak_classifier base_classifier_type;												///< @brief Multi-exit型カスケードにする識別器
	typedef base::classifier_list_type classifier_list_type;									///< @brief Multi-exit型カスケードにする識別器のリスト
	typedef base::exit_list_type exit_list_type;												///< @brief Multi-exit型カスケードを抜ける識別器のインデックスを保持するリスト
	typedef feature::value_type value_type;														///< @brief MISTのコンテナ内に格納するデータ型．mist::array< data > の data と同じ
	typedef feature::size_type size_type;														///< @brief 符号なしの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には size_t 型と同じ
	typedef feature::difference_type difference_type;											///< @brief 符号付きの整数を表す型．コンテナ内の要素数や，各要素を指定するときなどに利用し，内部的には ptrdiff_t 型と同じ

public:
	/// @brief デフォルトのコンストラクタ
	multi_exit_classifier( ) : base( )
	{
	}

	/// @brief コピーコンストラクタ
	multi_exit_classifier( const multi_exit_classifier &c ) : base( c )
	{
	}

	/// @brief デストラクタ
	~multi_exit_classifier( ){ }

	/// @brief 他のMulti-exit型AdaBoost識別器の情報をコピーする
	multi_exit_classifier& operator =( const multi_exit_classifier& o )
	{
		if( this != &o )
		{
			base::operator =( o );
		}

		return( *this );
	}

public:
	/// @brief 識別器の情報を保存したXMLファイルから設定を復元する
	//! 
	//! @param[in] filename … 識別器の情報を保存したXMLファイル名
	//! 
	virtual bool load( const std::string &filename );

	/// @brief 識別器の情報をXML形式でファイルに保存する
	//! 
	//! @param[in] filename … 識別器の情報を保存するXMLファイル名
	//! 
	virtual bool save( const std::string &filename, const std::string &comment = "" ) const;

public:
	/// @brief 現在の強識別器に弱識別器を追加する
	//! 
	//! @param[in,out]  features                 … 学習に用いる教師データ
	//! @param[in]      num_max_base_classifiers … 強識別器へ追加する弱識別器の数（デフォルトは100）
	//! 
	template < template < typename, typename > class FEATURE_LIST, class FEATURE, class ALLOCATOR >
	bool add_classifiers__( FEATURE_LIST< FEATURE, ALLOCATOR > & features, size_type num_max_base_classifiers = 100 )
	{
		typedef FEATURE feature_type;
		typedef typename FEATURE_LIST< FEATURE, ALLOCATOR >::iterator iterator;

		if( features.empty( ) )
		{
			return( false );
		}

		// AdaBoost により強識別器を学習する
		for( size_type t = 0 ; t < num_max_base_classifiers ; t++ )
		{
			// 重みを正規化する
			double s = 0.0;
			for( iterator ite = features.begin( ) ; ite != features.end( ) ; ++ite )
			{
				const feature_type &f = *ite;
				if( f.is_valid( ) )
				{
					s += f.weight( );
				}
			}
			for( iterator ite = features.begin( ) ; ite != features.end( ) ; ++ite )
			{
				feature_type &f = *ite;
				if( f.is_valid( ) )
				{
					f.weight( f.weight( ) / s );
				}
			}				

			// 弱識別器を学習する
			base::classifiers( ).push_back( base_classifier_type( ) );
			base_classifier_type &weak = base::classifiers( ).back( );
			weak.learn( features );

			//弱識別器の重みは1で固定;
			weak.weight( 1.0 );

			// 重みを更新する
			for( iterator ite = features.begin( ) ; ite != features.end( ) ; ++ite )
			{
				feature_type &f = *ite;
				if( f.is_valid( ) )
				{
					double v = weak.evaluate( f );

#if defined( __ADABOOST_ASYMMETRIC_LOSS__ )
					const double c1 = __ADABOOST_ASYMMETRIC_LOSS__;
					const double c2 = 1.0 - c1;
					f.weight( f.weight( ) * std::exp( - ( f.is_positive( ) ? +c1 : -c2 ) * v ) );
#else
					f.weight( f.weight( ) * std::exp( - ( f.is_positive( ) ? +1.0 : -1.0 ) * v ) );
#endif

					if( ( f.is_positive( ) ? +1.0 : -1.0 ) * v >= 0.0 )
					{
						weak.lambda_c( weak.lambda_c( ) + std::abs( v ) );
					}
					else
					{
						weak.lambda_w( weak.lambda_w( ) + std::abs( v ) );
					}
				}
			}
		}

		return( true );
	}
};


/// @brief AdaBoost識別器とカスケード型AdaBoost識別器の情報をXML形式で入出力する関数群（内部用）
namespace io
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

	/// @brief libxml2のAPIを利用して弱識別器の設定を取得する関数
	inline int read( weak_classifier &weak, xmlTextReaderPtr reader )
	{
		if( reader == NULL )
		{
			return( -1 );
		}

		int    weakID    = utility::ToInt( get_attribute( reader, "id", "-1" ) );
		int    nbins     = utility::ToInt( get_attribute( reader, "nbins", "-1" ) );
		int    featureID = utility::ToInt( get_attribute( reader, "index", "-1" ) );
		double weight    = utility::ToDouble( get_attribute( reader, "weight", "0.0" ) );
		double lambda_c  = utility::ToDouble( get_attribute( reader, "lambda_c", "0.0" ) );
		double lambda_w  = utility::ToDouble( get_attribute( reader, "lambda_w", "0.0" ) );

		weak.index( featureID );
		weak.weight( weight );
		weak.lambda_c( lambda_c );
		weak.lambda_w( lambda_w );

		if( nbins > 0 )
		{
			weak.resize( nbins );
		}
		else
		{
			weak.resize( __HISTOGRAM_CLASSIFIER_NUM_BINS__ );
		}

		bool isEnd  = false;
		int  ret    = 1;
		int  sdepth = xmlTextReaderDepth( reader );

		while( ret == 1 && !isEnd )
		{
			const xmlChar *name = xmlTextReaderConstName( reader );
			const int  depth    = xmlTextReaderDepth( reader );
			//const bool isEmpty  = xmlTextReaderIsEmptyElement( reader ) != 0;
			//const bool hasValue = xmlTextReaderHasValue( reader ) != 0;

			switch( xmlTextReaderNodeType( reader ) )
			{
			case 15: // EndElement
				if( sdepth == depth && xmlStrcasecmp( name, BAD_CAST "weak" ) == 0 )
				{
					isEnd = true;
				}
				break;

			case 3: // Text
				{
					const xmlChar *text = xmlTextReaderConstValue( reader );

					std::vector< std::string > vals;
					utility::split_string( ( const char * )text, vals, "," );

					if( vals.size( ) == weak.size( ) )
					{
						for( size_t i = 0 ; i < vals.size( ) ; i++ )
						{
							weak[ i ] = utility::ToDouble( vals[ i ] );
						}
					}
					else if( vals.size( ) == weak.size( ) * 3 )
					{
						for( size_t i = 0 ; i < weak.size( ) ; i++ )
						{
							weak[ i ] = utility::ToDouble( vals[ i ] );
							weak.pos( i, utility::ToDouble( vals[ i + weak.size( ) ] ) );
							weak.neg( i, utility::ToDouble( vals[ i + weak.size( ) * 2 ] ) );
						}
					}
					else
					{
						return( -1 );
					}
				}
				break;

			default:
			case 0: // None
			case 1: // Element
			case 2: // Attribute
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

		return( weakID >= 0 && featureID >= 0 ? 1 : -1 );
	}

	/// @brief libxml2のAPIを利用して弱識別器の設定を保存する関数
	inline int write( xmlTextWriterPtr writer, const weak_classifier &weak, weak_classifier::difference_type id, size_t tag_depth )
	{
		// 弱識別器の開始タグ
		if( xmlTextWriterStartElement( writer, BAD_CAST "weak" ) < 0 )
		{
			std::cerr << "Failed to write <weak> tag." << std::endl;
			return( -1 );
		}

		// 識別器のIDを出力する
		if( id >= 0 )
		{
			if( xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "id", "%ld", id ) < 0 )
			{
				std::cerr << "Failed to add a property to XML tag." << std::endl;
				return( -1 );
			}
		}

		// 弱識別器で使用する特徴量のID
		if( xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "index", "%ld", weak.index( ) ) < 0 )
		{
			std::cerr << "Failed to add a property to XML tag." << std::endl;
			return( -1 );
		}

		// 弱識別器で使用する特徴量のビン数
		if( xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "nbins", "%ld", weak.size( ) ) < 0 )
		{
			std::cerr << "Failed to add a property to XML tag." << std::endl;
			return( -1 );
		}

		// 強識別器を構築する際に使用する弱識別器の重み
		if( xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "weight", "%f", weak.weight( ) ) < 0 )
		{
			std::cerr << "Failed to add a property to XML tag." << std::endl;
			return( -1 );
		}

		// 弱識別器をオンライン更新する際の重み（正しく分類されたもの）
		if( xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "lambda_c", "%f", weak.lambda_c( ) ) < 0 )
		{
			std::cerr << "Failed to add a property to XML tag." << std::endl;
			return( -1 );
		}

		// 弱識別器をオンライン更新する際の重み（誤って分類されたもの）
		if( xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "lambda_w", "%f", weak.lambda_w( ) ) < 0 )
		{
			std::cerr << "Failed to add a property to XML tag." << std::endl;
			return( -1 );
		}

		// コメント情報を設定する
		if( weak.comment( ) != "" )
		{
			if( xmlTextWriterWriteFormatComment( writer, " %s ", weak.comment( ).c_str( ) ) < 0 )
			{
				std::cerr << "Failed to write comments on a tag." << std::endl;
				return( -1 );
			}
		}

		std::string indent = "";
		for( size_t i = 0 ; i < tag_depth ; i++ )
		{
			indent += "\t";
		}

		// 弱識別器の確率密度関数の値を保存する
		std::string vals = std::string( weak.comment( ) != "" ? "" : "\n" ) + indent + "\t";
		for( size_t i = 0 ; i < weak.size( ) ; i++ )
		{
			if( i > 0 )
			{
				vals += ",";
			}

			vals += utility::ToString( weak[ i ], "%.12lf" );
		}
		for( size_t i = 0 ; i < weak.size( ) ; i++ )
		{
			vals += ",";
			vals += utility::ToString( weak.pos( i ), "%.12lf" );
		}
		for( size_t i = 0 ; i < weak.size( ) ; i++ )
		{
			vals += ",";
			vals += utility::ToString( weak.neg( i ), "%.12lf" );
		}
		vals += "\n";
		vals += indent;

		if( xmlTextWriterWriteString( writer, BAD_CAST vals.c_str( ) ) < 0 )
		{
			std::cerr << "Failed to add a value to XML tag." << std::endl;
			return( -1 );
		}

		// 弱識別器の終了タグ
		if( xmlTextWriterEndElement( writer ) < 0 )
		{
			std::cerr << "Failed to write </weak> tag." << std::endl;
			return( -1 );
		}

		return( 0 );
	}
}

#include "classifier_io.h"

bool boosted_classifier::load( const std::string &filename )
{
	return( read( *this, filename, "real", "" ) );
}

bool boosted_classifier::save( const std::string &filename, const std::string &comment ) const
{
	return( write( *this, filename, "real", comment ) );
}

bool cascaded_classifier::load( const std::string &filename )
{
#if defined( __ADABOOST_CASCADE_TYPE__ ) && __ADABOOST_CASCADE_TYPE__ == 2
	return( read_cascade( *this, filename, "real", "nested" ) );
#else
	return( read_cascade( *this, filename, "real", "viola" ) );
#endif
}

bool cascaded_classifier::save( const std::string &filename, const std::string &comment ) const
{
	return( write( *this, filename, "real", comment ) );
}

bool multi_exit_classifier::load( const std::string &filename )
{
	return( read_cascade( *this, filename, "real", "multi-exit" ) );
}

bool multi_exit_classifier::save( const std::string &filename, const std::string &comment ) const
{
	return( write( *this, filename, "real", comment ) );
}


#if !defined( __NO_ADABOOST_NAMESPACE__ ) || __NO_ADABOOST_NAMESPACE__ == 0
// AdaBoost名前空間の終わり
_ADABOOST_END
#endif

#endif	// __INCLUDE_REAL_ADABOOST_CLASSIFIER__