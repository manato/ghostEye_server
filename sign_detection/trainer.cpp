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
#include <ctime>
#include <string>
#include <algorithm>
#include <vector>
#include <deque>

#include <stdio.h>

#ifdef _OPENMP
#include <omp.h>
#endif

#include <mist/mist.h>
#include <mist/io/image.h>
#include <mist/io/lines.h>
#include <mist/utility/options.h>

#include <mist/timer.h>
#include <mist/random.h>
#include <mist/drawing.h>
#include <mist/interpolate.h>

#include "utility.h"
#include "classifier.h"


// ネガティブサンプルをランダムに取得する際に使用する乱数ジェネレータ
mist::uniform::random rnd( static_cast< int >( std::time( NULL ) ) );

struct weak_classifier_info_functor
{
	std::vector< adaboost::feature_info > &feature_list_;
	std::vector< size_t > &feature_idmap_;

	weak_classifier_info_functor( std::vector< adaboost::feature_info > &fl, std::vector< size_t > &fm ) : feature_list_( fl ), feature_idmap_( fm )
	{
	}

	std::string operator ()( size_t indx ) const
	{
		std::string str = "";
		size_t mode = feature_list_[ feature_idmap_[ indx ] ].mode;

		str += adaboost::get_feature_string( mode );
		str += ", ";
		str += adaboost::get_color_string( mode );

		return( str );
	}
};

struct feature_computation_info
{
	unsigned int x;
	unsigned int y;
	double       scale;
	bool         mirror;

	feature_computation_info( unsigned int xx = 0, unsigned int yy = 0, double s = 1.0, bool mrr = false ) : x( xx ), y( yy ), scale( s ), mirror( mrr )
	{
	}
};

struct weight_index_pair
{
	size_t index;
	double weight;

	weight_index_pair( size_t indx = 0, double w = 0.0 ) : index( indx ), weight( w )
	{
	}

	weight_index_pair( const weight_index_pair &w ) : index( w.index ), weight( w.weight )
	{
	}

	bool operator <( const weight_index_pair &w ) const
	{
		return( weight > w.weight );
	}
};

template < class Array >
inline size_t compute_number_of_samples( const Array &img, size_t image_width, size_t image_height, size_t number_of_scales )
{
	double scale = 1.0;
	size_t count = 0;
	for( size_t s = 0 ; s < number_of_scales ; s++ )
	{
		size_t w = static_cast< size_t >( image_width  * scale );
		size_t h = static_cast< size_t >( image_height * scale );
		size_t www = img.width( )  < w ? 0 : img.width( )  - w + 1;
		size_t hhh = img.height( ) < h ? 0 : img.height( ) - h + 1;
		count += www * hhh;

		scale *= __FEATURE_SCALING_FACTOR__;
	}

	return( count );
}

template < class FEATURE_LIST, class FEATURE_IDMAP, class TRAINING_LIST, class CLASSIFIER >
size_t compute_training_data( const FEATURE_LIST &flist, const FEATURE_IDMAP &fidmap, TRAINING_LIST &tlist, const CLASSIFIER &classifier, const std::string &fname,
								size_t image_width, size_t image_height, bool is_positive, size_t sindex, size_t number_of_samples, size_t number_of_scales, bool mirror, size_t use_weight_sampling )
{
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	static size_type _old_image_width_ = 0;
	static size_type _old_image_height_ = 0;
	static adaboost::color_image_type ori, ori_inv;

	if( number_of_samples == 0 )
	{
		return( sindex );
	}
	else if( !mist::read_image( ori, fname ) )
	{
		// ファイルが読めないみたい・・・
		std::cerr << "ファイル [" << fname << "] の読み込みに失敗しました．" << std::endl;
		return( sindex );
	}

	if( mirror )
	{
		ori_inv.resize( ori.width( ), ori.height( ) );
		for( size_type j = 0 ; j < ori_inv.height( ) ; j++ )
		{
			for( size_type i = 0 ; i < ori_inv.width( ) ; i++ )
			{
				ori_inv( i, j ) = ori( ori.width( ) - i - 1, ori.height( ) - j - 1 );
			}
		}
	}

	std::vector< adaboost::integral_image_type > iimgs, iimgs_inv;
	adaboost::convert_image( ori, iimgs );

	if( mirror )
	{
		adaboost::convert_image( ori_inv, iimgs_inv );
	}

	// 計算範囲をランダムに設定する
	static std::vector< feature_computation_info > feature_computation_list;
	static std::vector< size_type > indices;

	if( ori.width( ) != _old_image_width_ || ori.height( ) != _old_image_height_ )
	{
		_old_image_width_ = ori.width( );
		_old_image_height_ = ori.height( );

		feature_computation_list.resize( 0 );
		indices.resize( 0 );

		double scale = 1.0;

		size_t count = 0;
		size_t number_of_elements_in_feature_list = compute_number_of_samples( ori, image_width, image_height, number_of_scales ) * ( mirror ? 2 : 1 );

		indices.reserve( number_of_elements_in_feature_list );
		feature_computation_list.reserve( number_of_elements_in_feature_list ); 
		for( size_type s = 0 ; s < number_of_scales ; s++ )
		{
			size_type w = static_cast< size_t >( image_width  * scale + 0.5 );
			size_type h = static_cast< size_t >( image_height * scale + 0.5 );
			difference_type www = ori.width( )  < w ? -1 : ori.width( )  - w;
			difference_type hhh = ori.height( ) < h ? -1 : ori.height( ) - h;

			for( difference_type j = 0 ; j <= hhh ; j++ )
			{
				for( difference_type i = 0 ; i <= www ; i++ )
				{
					indices.push_back( count++ );
					feature_computation_list.push_back( feature_computation_info( static_cast< unsigned int >( i ), static_cast< unsigned int >( j ), scale, false ) );
					if( mirror )
					{
						indices.push_back( count++ );
						feature_computation_list.push_back( feature_computation_info( static_cast< unsigned int >( i ), static_cast< unsigned int >( j ), scale, true ) );
					}
				}
			}

			scale *= __FEATURE_SCALING_FACTOR__;
		}

		// インデックスをランダムシャッフルする
		std::random_shuffle( indices.begin( ), indices.end( ), rnd ); // ランダムシャッフル
	}
	else
	{
		for( size_type i = 0 ; i < indices.size( ) ; i++ )
		{
			indices[ i ] = i;
		}

		// インデックスをランダムシャッフルする
		std::random_shuffle( indices.begin( ), indices.end( ), rnd ); // ランダムシャッフル
	}

	adaboost::feature_calculator< adaboost::integral_image_type > fcalculator( iimgs, flist ), fcalculator_inv( iimgs_inv, flist );

	size_t nsamples = 0;
	std::vector< weight_index_pair > weight_index_list;
	weight_index_list.reserve( number_of_samples * ( use_weight_sampling > 1 ? use_weight_sampling : 1 ) );

	for( size_type i = 0 ; i < feature_computation_list.size( ) && weight_index_list.size( ) < weight_index_list.capacity( ) ; i++ )
	{
		const feature_computation_info &info = feature_computation_list[ indices[ i ] ];

		if( info.mirror )
		{
			fcalculator_inv.initialize( );
			double v = classifier.evaluate( fcalculator_inv, flist, fidmap, info.x, info.y, info.scale );
			if( ( v >= 0.0 ) != is_positive )
			{
				weight_index_list.push_back( weight_index_pair( indices[ i ], v ) );
			}
		}
		else
		{
			fcalculator.initialize( );
			double v = classifier.evaluate( fcalculator, flist, fidmap, info.x, info.y, info.scale );
			if( ( v >= 0.0 ) != is_positive )
			{
				weight_index_list.push_back( weight_index_pair( indices[ i ], v ) );
			}
		}
	}

	if( use_weight_sampling > 1 )
	{
		std::sort( weight_index_list.begin( ), weight_index_list.end( ) );
	}

	for( size_type i = 0 ; i < weight_index_list.size( ) && nsamples < number_of_samples && sindex + nsamples < tlist.size( ) ; i++ )
	{
		const feature_computation_info &info = feature_computation_list[ weight_index_list[ i ].index ];

		if( info.mirror )
		{
			adaboost::feature &f = tlist[ sindex + nsamples ];

			compute_feature( iimgs_inv, flist, fidmap, f, info.x, info.y, info.scale );

			f.is_valid( true );
			f.is_new( true );
			f.is_positive( is_positive );

			nsamples++;
		}
		else
		{
			adaboost::feature &f = tlist[ sindex + nsamples ];

			compute_feature( iimgs, flist, fidmap, f, info.x, info.y, info.scale );

			f.is_valid( true );
			f.is_new( true );
			f.is_positive( is_positive );

			nsamples++;
		}
	}

	return( sindex + nsamples );
}

template < class FEATURE_LIST, class FEATURE_IDMAP, class TRAINING_LIST, class CLASSIFIER, class STRING_LIST >
size_t compute_negative_data_from_list( const FEATURE_LIST &flist, const FEATURE_IDMAP &fidmap, TRAINING_LIST &tlist, const CLASSIFIER &classifier, STRING_LIST &image_list,
											size_t image_width, size_t image_height, bool is_positive, size_t sindex, size_t number_of_total_samples, size_t number_of_scales, bool &finished, bool mirror = false, size_t use_weight_sampling = 1, size_t reduce_negative_image_per_stage = 1 )
{
	typedef STRING_LIST string_list_type;

	// サンプルを一時的に収集しつくした画像のリスト
	string_list_type tmp_list, tmp_list2;

	// ネガティブ画像のリストを削減
	if( reduce_negative_image_per_stage > 1 )
	{
		typename string_list_type::iterator ite = image_list.begin( );
		for( size_t i = 0 ; i < image_list.size( ) / reduce_negative_image_per_stage ; i++, ++ite );

		for( ; ite != image_list.end( ) ; )
		{
			tmp_list2.push_back( *ite );
			ite = image_list.erase( ite );
		}
	}

	// 足りないサンプル数分だけ補う
	while( sindex < number_of_total_samples && !image_list.empty( ) )
	{
		// ネガティブ画像から特徴量を計算する
		std::cout << std::endl;
		std::cout << "Load negatives [" << number_of_total_samples - sindex << "] " << std::flush;

		// 学習に使用するネガティブサンプルをすべて再初期化する
		// ただし，構築済みのカスケード型AdaBoost識別器で正しく分類可能なものは除外する
		size_t image_list_size = image_list.size( );
		typename string_list_type::iterator ite = image_list.begin( );
		for( size_t k = 0 ; ite != image_list.end( ) && sindex < number_of_total_samples ; k++ )
		{
			// ネガティブサンプルを抽出するネガティブ画像ファイル名
			std::string fname = *ite;

			// 入れ替えるサンプル数
			size_t number_of_samples = number_of_total_samples - sindex;

			// ネガティブ画像から抽出するサンプル数
			size_t number_of_samples_per_image = number_of_samples / ( image_list_size - k );

			// １枚の画像から取得するネガティブサンプルの上限をチェックする
			if( sindex + number_of_samples_per_image + 1 <= number_of_total_samples )
			{
				number_of_samples_per_image++;
			}

			// ネガティブサンプルをどの程度抽出できたかどうかを判定するための一時変数
			size_t oindx = sindex;

			// nsamplesだけネガティブサンプルを画像から抽出する
			sindex = compute_training_data( flist, fidmap, tlist, classifier, fname, image_width, image_height, is_positive, sindex, number_of_samples_per_image, number_of_scales, mirror, use_weight_sampling );

			// 画像内のすべてのパターンを分類できた場合は以降の処理から除外する
			if( oindx + number_of_samples_per_image > sindex )
			{
				if( oindx == sindex )
				{
					std::cout << "x" << std::flush;
				}
				else
				{
					std::cout << "o" << std::flush;
					tmp_list.push_back( *ite );
				}

				ite = image_list.erase( ite );
			}
			else
			{
				std::cout << "." << std::flush;
				++ite;
			}
		}
	}

	finished = image_list.empty( );

	// 分類できていないサンプルが含まれる可能性のあるものを復元する
	image_list.insert( image_list.end( ), tmp_list.begin( ), tmp_list.end( ) );

	if( reduce_negative_image_per_stage > 1 )
	{
		if( sindex < number_of_total_samples )
		{
			sindex = compute_negative_data_from_list( flist, fidmap, tlist, classifier, tmp_list2, image_width, image_height, is_positive, sindex, number_of_total_samples, number_of_scales, finished, mirror, use_weight_sampling, 1 );
		}
		else
		{
			for( size_t i = 0 ; i < tmp_list2.size( ) ; i++ )
			{
				std::cout << "-";
			}
			std::cout << std::flush;
		}

		image_list.insert( image_list.end( ), tmp_list2.begin( ), tmp_list2.end( ) );
	}

	return( sindex );
}

int main( int argc, char *argv[] )
{
	typedef adaboost::feature                     feature_type;
	typedef adaboost::feature_value_type          feature_value_type;
	typedef adaboost::image_type::size_type       size_type;
	typedef adaboost::image_type::difference_type difference_type;
	typedef std::deque< std::string >             string_list_type;

	std::string header;

	header += "【カスケード型AdaBoost識別器の学習プログラム】\n\n";
	header += std::string( __ADABOOST_FEATURE_STRING__ ) + " + " + __ADABOOST_CLASSIFIER_STRING__ + "\n\n";
	header += "[使用例]\n";
	header += "AdaTrainer [オプション] positives.txt negatives.txt [result.xml(省略可)]\n";
	header += "※「positives.txt」と「negatives.txt」は画像へのパスを改行で区切ったテキストファイル\n\n";
	header += "※「result.xml」は学習結果の識別器を保存するXMLファイルであり、省略した場合は「result.xml」を使用する\n\n";
	header += "[オプション]";

	mist::options op( header );

	op.add( "minhitrate", "各段が満たすべき最低のTP率", 0.995 );
#if defined( __ADABOOST_CASCADE_TYPE__ ) && __ADABOOST_CASCADE_TYPE__ != 0
	op.add( "maxfalsealarm", "各段で許される最大のFP率", 0.5 );
#endif
	op.add( "totalfalsealarm", "作成する識別器が満たすべき最大のFP率", 0.02 );
#if defined( __ADABOOST_CASCADE_TYPE__ ) && __ADABOOST_CASCADE_TYPE__ != 0
	op.add( "nstages", "作成する識別器の最大段数", 14 );
	op.add( "nweaks", "各段で使用できる最大の弱識別器数", 100 );
	op.add( "nwstep", "各段で一度に追加する弱識別器の数", 1 );
#else
	op.add( "nweaks", "最大の弱識別器数", 100 );
#endif
	op.add( "nscales", "ネガティブ画像から特徴量を切り出す窓の大きさを変化させる回数", 6 );
	op.add( "hardsampling", "ネガティブサンプルを余分に集めて難しいサンプルのみで学習を行う（何倍のサンプルを集めるかを指定）", 1 );
	op.add( "rdim", "次元を削減して学習を行う際に指定するパラメータ（2で2分の1，3で3分の1，1以下は削減しない）", 1 );
	op.add( "npos", "学習に使用するポジティブサンプル数", -1 );
	op.add( "nneg", "学習に使用するネガティブサンプル数", 500 );
	op.add( "w", "学習画像の横幅（省略した場合は学習画像の値を使用）", -1 );
	op.add( "h", "学習画像の縦幅（省略した場合は学習画像の値を使用）", -1 );
	op.add( "rneg", "1段分の識別器を構成する際にネガティブ画像を何分の１用いるか（省略した場合は全てからサンプリング）", 1 );
	op.add( "mirror", "鏡像のデータを学習に使用するかどうか（デフォルトでは学習しない）" );
	op.add( "optimize", "最終的な識別器の速度が改善するよう弱識別器の順序を調整する" );
	op.add( "seed", "ネガティブサンプルをランダムにサンプリングする際に使用するシード（省略した場合は現在の時刻を使用して初期化）", -1 );
	op.add( "input", "検出器のXMLファイルを指定し，初期検出器として利用する（途中から学習を再開する場合に利用）", "" );

#ifdef _OPENMP
	op.add( "nthreads", "使用するスレッド数（-1を指定するとCPU数を自動設定）", 2 );
#endif

	if( !op.parse_args( argc, argv ) )
	{
		op.show_help( );
		return( 1 );
	}

	// ポジティブ画像のリストを取得する
	string_list_type pos_list, neg_list;

	double          min_hitrate                = op.get_double( "minhitrate" );
#if defined( __ADABOOST_CASCADE_TYPE__ ) && __ADABOOST_CASCADE_TYPE__ != 0
	double          max_false_alarm            = op.get_double( "maxfalsealarm" );
#endif
	double          total_false_alarm          = op.get_double( "totalfalsealarm" );
	difference_type number_of_weaks            = op.get_int( "nweaks" );
#if defined( __ADABOOST_CASCADE_TYPE__ ) && __ADABOOST_CASCADE_TYPE__ != 0
	difference_type number_of_weaks_step       = op.get_int( "nwstep" );
	difference_type number_of_stages           = op.get_int( "nstages" );
#endif
	difference_type number_of_scales           = op.get_int( "nscales" );
	difference_type hardsampling               = op.get_int( "hardsampling" );
	difference_type number_of_positive_samples = op.get_int( "npos" );
	difference_type number_of_negative_samples = op.get_int( "nneg" );
	difference_type sample_width               = op.get_int( "w" );
	difference_type sample_height              = op.get_int( "h" );
	difference_type reduce_neg                 = op.get_int( "rneg" );
	difference_type random_seed                = op.get_int( "seed" );
	difference_type reduced_dimension          = op.get_int( "rdim" );
	std::string     initial_classifier_xml     = op.get_string( "input" );
	bool optimize                              = op.isset( "optimize" );
	bool mirror_training                       = op.isset( "mirror" );
	bool initialize_true_positive_rate         = false;

#ifdef _OPENMP
	int __nthreads__ = op.get_int( "nthreads" );
	if( __nthreads__ > 0 )
	{
		omp_set_num_threads( __nthreads__ );
	}
#endif

	if( op.size( ) < 2 )
	{
		std::cerr << "ポジティブ画像のリストもしくはネガティブ画像のリストが指定されていません．" << std::endl << std::endl;
		op.show_help( );
		return( 1 );
	}

	// その他のオプションを取得する
	std::string posname = op[ 0 ];
	std::string negname = op[ 1 ];
	std::string xmlname = op.size( ) > 2 ? op[ 2 ] : "result.xml";

	// 出力用のフォルダを作成する
	adaboost::utility::make_dir( xmlname, false );

	// 乱数のシードを初期化
	if( random_seed < 0 )
	{
		random_seed = static_cast< difference_type >( std::time( NULL ) );
	}
	
	rnd.init( static_cast< unsigned long >( random_seed ) );

	// ポジティブ画像のファイル名のリストを読み込む
	if( posname != "" )
	{
		mist::read_lines( pos_list, posname );

		if( number_of_positive_samples <= 0 || number_of_positive_samples > static_cast< difference_type >( pos_list.size( ) ) )
		{
			number_of_positive_samples = pos_list.size( );
		}
	}

	// 実際に使用するポジティブサンプル数
	difference_type number_of_positive_samples2 = mirror_training ? number_of_positive_samples * 2 : number_of_positive_samples;

	if( pos_list.empty( ) )
	{
		std::cerr << "ポジティブ画像のリストが指定されていません．" << std::endl << std::endl;
		op.show_help( );
		return( 1 );
	}

	// ネガティブ画像のファイル名のリストを読み込む
	if( negname != "" )
	{
		mist::read_lines( neg_list, negname );
	}

	if( neg_list.empty( ) )
	{
		std::cerr << "ネガティブ画像のリストが指定されていません．" << std::endl << std::endl;
		op.show_help( );
		return( 1 );
	}


	// 特徴量の計算範囲をポジティブ画像の大きさを基に決定する
	if( !pos_list.empty( ) && ( sample_width < 0 || sample_height < 0 ) )
	{
		std::string fname = pos_list.front( );
		adaboost::image_type img;

		if( !mist::read_image( img, fname ) )
		{
			// ファイルが読めないみたい・・・
			std::cerr << "ファイル [" << fname << "] の読み込みに失敗しました．" << std::endl << std::endl;
			op.show_help( );
			return( 1 );
		}

		sample_width  = static_cast< int >( img.width( ) );
		sample_height = static_cast< int >( img.height( ) );
	}

	if( sample_width < 0 || sample_height < 0 )
	{
		std::cerr << "特徴量の計算範囲が指定されていません．" << std::endl << std::endl;
		op.show_help( );
		return( 1 );
	}

	// 特徴量のインデックスを作成する
	std::vector< adaboost::feature_info > feature_list;
	std::vector< size_type > feature_idmap;
	weak_classifier_info_functor finfo( feature_list, feature_idmap );
	{
		adaboost::image_type tmp( sample_width, sample_height );

		// 特徴量のリストを作成する
		adaboost::construct_feature_list( tmp, feature_list );

		// 学習に使用する特徴量のマップを作成する
		// 次元が大きすぎる場合はこれでメモリ削減を行う
		feature_idmap.resize( feature_list.size( ) );
		for( size_type i = 0 ; i < feature_idmap.size( ) ; i++ )
		{
			feature_idmap[ i ] = i;
		}

		// インデックスをランダムにシャッフルする
		if( reduced_dimension > 1 )
		{
			std::random_shuffle( feature_idmap.begin( ), feature_idmap.end( ), rnd );
			feature_idmap.erase( feature_idmap.begin( ) + feature_idmap.size( ) / reduced_dimension, feature_idmap.end( ) );
		}
	}


	header += __ADABOOST_FEATURE_STRING__;
	// 情報をコンソールに表示
	std::cout << "Feature                       : " << __ADABOOST_FEATURE_STRING__ << std::endl;
	std::cout << "Classifier                    : " << __ADABOOST_CLASSIFIER_STRING__ << std::endl;
	std::cout << "Positive                      : " << posname << " [" << pos_list.size( ) << "]" << std::endl;
	std::cout << "Negative                      : " << negname << " [" << neg_list.size( ) << "]" << std::endl;
	std::cout << "Output XML                    : " << xmlname << std::endl;
	std::cout << "Minimum hit rate              : " << min_hitrate << std::endl;
#if defined( __ADABOOST_CASCADE_TYPE__ ) && __ADABOOST_CASCADE_TYPE__ != 0
	std::cout << "Maximum false alarm           : " << max_false_alarm << std::endl;
#endif
	std::cout << "Total false alarm             : " << total_false_alarm << std::endl;
#if defined( __ADABOOST_CASCADE_TYPE__ ) && __ADABOOST_CASCADE_TYPE__ != 0
	std::cout << "Number of weaks in a stage    : " << number_of_weaks << std::endl;
	std::cout << "Number of total stages        : " << number_of_stages << std::endl;
	std::cout << "Number of weaks in a step     : " << number_of_weaks_step << std::endl;
#else
	std::cout << "Number of weaks               : " << number_of_weaks << std::endl;
#endif
	std::cout << "Number of hard sampling       : " << hardsampling << std::endl;
	if( mirror_training )
	{
		std::cout << "Number of positives in a stage: " << number_of_positive_samples << " (Mirroring)" << std::endl;
	}
	else
	{
		std::cout << "Number of positives in a stage: " << number_of_positive_samples << std::endl;
	}
	std::cout << "Number of negatives in a stage: " << number_of_negative_samples << std::endl;
	std::cout << "Number of scales              : " << number_of_scales << std::endl;
	std::cout << "Number of features            : " << feature_list.size( ) << std::endl;
	std::cout << "Number of features (Reduced)  : " << feature_idmap.size( ) << std::endl;
	std::cout << "Range to compute features     : " << sample_width << " x " << sample_height << std::endl;
	std::cout << "Speed optimization            : " << ( optimize ? "Yes" : "No" ) << std::endl;
	std::cout << "Seed for random generator     : " << random_seed << std::endl;
	std::cout << std::endl;

	// シードを変更する
	rnd.init( static_cast< unsigned long >( ++random_seed ) );

	// AdaBoost識別器を定義する
	__DEFINE_ADABOOST_CLASSIFIER_TYPE__;

	adaboost_classifier_type classifier;

#if defined( __ADABOOST_CASCADE_TYPE__ ) && __ADABOOST_CASCADE_TYPE__ != 0
	// カスケード型AdaBoost識別器を構築するためのパラメータを設定
	classifier.initialize( min_hitrate, max_false_alarm, total_false_alarm, number_of_weaks );
#endif

	// 識別に使用するウィンドウサイズを記憶する
	std::string tag = adaboost::utility::ToString( sample_width ) + "," + adaboost::utility::ToString( sample_height );

	classifier.tag( tag );

	if( initial_classifier_xml != "" )
	{
		std::cout << "Load initial classifier from " << initial_classifier_xml << std::endl << std::endl;
		if( !classifier.load( initial_classifier_xml ) )
		{
			std::cerr << "Failed to load classifier from " << initial_classifier_xml << std::endl << std::endl;
		}

		initialize_true_positive_rate = true;
	}

	// 学習に利用するデータ
	std::vector< feature_type > train;
	train.reserve( number_of_positive_samples2 + number_of_negative_samples );

	size_type numBytes = ( number_of_positive_samples2 + number_of_negative_samples ) * ( feature_idmap.size( ) * sizeof( adaboost::feature_value_type ) + sizeof( adaboost::feature ) );
	std::cout << "Allocating memory space for training data: " << numBytes / ( 1024 * 1024 ) << " MBytes" << std::endl << std::endl;

	// 学習用サンプルのメモリ領域を最初にを割り当てる
	for( difference_type i = 0 ; i < number_of_positive_samples2 + number_of_negative_samples ; i++ )
	{
		train.push_back( adaboost::feature( feature_idmap.size( ), true ) );
	}

	std::cout << "Load positives [" << number_of_positive_samples2 << "] " << std::flush;
	{
		// ポジティブ画像のリストをランダムにシャッフルする
		std::random_shuffle( pos_list.begin( ), pos_list.end( ), rnd );

		string_list_type::iterator ite = pos_list.begin( );
		for( difference_type k = 0 ; k < number_of_positive_samples ; k++ )
		{
			std::string fname = *ite++;
			compute_training_data( feature_list, feature_idmap, train, adaboost::empty_classifier( false ), fname, sample_width, sample_height, true, mirror_training ? k * 2 : k, mirror_training ? 2 : 1, 1, mirror_training, 1 );

			if( ( k % 100 ) == 0 )
			{
				std::cout << "." << std::flush;
			}
		}
	}

	// ネガティブ画像から特徴量を計算する
	std::cout << std::endl;

	// 分類できないネガティブサンプルが無くなったかどうかを示すフラグ
	bool bfinished = false;

	// ネガティブサンプルの収集が各段で一定になるように乱数を初期化１
	rnd.init( static_cast< unsigned long >( ++random_seed ) );

	if( initial_classifier_xml != "" )
	{
		compute_negative_data_from_list( feature_list, feature_idmap, train, classifier, neg_list, sample_width, sample_height, false, number_of_positive_samples2, train.size( ), number_of_scales, bfinished, mirror_training, 1, reduce_neg );
	}
	else
	{
		compute_negative_data_from_list( feature_list, feature_idmap, train, adaboost::empty_classifier( true ), neg_list, sample_width, sample_height, false, number_of_positive_samples2, train.size( ), number_of_scales, bfinished, mirror_training, 1, reduce_neg );
	}

	std::cout << std::endl;
	std::cout << std::endl;

	{
		mist::timer t;

#if defined( __ADABOOST_CASCADE_TYPE__ ) && __ADABOOST_CASCADE_TYPE__ != 0
		// 学習データを用いて識別器を構築します
		difference_type nstages = classifier.number_of_stages( );
		while( nstages < number_of_stages )
		{
#if defined( __ADABOOST_CASCADE_TYPE__ ) && __ADABOOST_CASCADE_TYPE__ == 3
			// カスケード型AdaBoost識別器の1段分のAdaBoost識別器を学習する
			if( classifier.add_classifier( train, finfo, 1, number_of_weaks_step, max_false_alarm, initialize_true_positive_rate ) )
			{
				size_type sid = 0, eid = classifier.exits( ).back( );
				if( classifier.exits( ).size( ) > 1 )
				{
					sid = classifier.exits( )[ classifier.exits( ).size( ) - 2 ];
				}

				// 弱識別器に特徴量の情報を設定する
				for( size_type i = sid ; i < eid ; i++ )
				{
					classifier.classifiers( )[ i ].comment( feature_list[ classifier.classifiers( )[ i ].index( ) ].to_string( ) );
					classifier.classifiers( )[ i ].resize( feature_list[ classifier.classifiers( )[ i ].index( ) ].number_of_bins( ) );
				}
#else
			// カスケード型AdaBoost識別器の1段分のAdaBoost識別器を学習する
			if( classifier.add_classifier( train, 1, number_of_weaks_step, max_false_alarm, initialize_true_positive_rate ) )
			{
				// 弱識別器に特徴量の情報を設定する
				for( size_type i = 0 ; i < classifier.classifiers( ).back( ).classifiers( ).size( ) ; i++ )
				{
					classifier.classifiers( ).back( ).classifiers( )[ i ].comment( feature_list[ classifier.classifiers( ).back( ).classifiers( )[ i ].index( ) ].to_string( ) );
					classifier.classifiers( ).back( ).classifiers( )[ i ].resize( feature_list[ classifier.classifiers( ).back( ).classifiers( )[ i ].index( ) ].number_of_bins( ) );
				}
#endif

				// 格段の学習結果を保存する
				classifier.save( xmlname + adaboost::utility::ToString( nstages + 1, ".%d" ), std::string( __ADABOOST_FEATURE_STRING__ ) + " + " + __ADABOOST_CLASSIFIER_STRING__ );

				// 構築したカスケード型AdaBoost識別器が終了条件を満たすかどうかを判定
				if( classifier.last_false_positive_rate( ) <= classifier.total_false_positive_rate( ) || ( bfinished && classifier.current_false_positive_rate( ) == 0 ) )
				{
					break;
				}
				else if( nstages + 1 < number_of_stages )
				{
					// データを切り詰める
					size_t cindx = number_of_positive_samples2;

					// 一度すべてのネガティブサンプルを無効にする
					for( size_t i = cindx ; i < train.size( ) ; i++ )
					{
						train[ i ].is_valid( false );
					}

					// ネガティブサンプルの収集が各段で一定になるように乱数を初期化２
					rnd.init( static_cast< unsigned long >( ++random_seed ) );

					// ネガティブ画像のリストをランダムにシャッフルする
					std::random_shuffle( neg_list.begin( ), neg_list.end( ), rnd );

					// 足りないサンプル数分だけ補う
					cindx = compute_negative_data_from_list( feature_list, feature_idmap, train, classifier, neg_list, sample_width, sample_height, false, cindx, train.size( ), number_of_scales, bfinished, mirror_training, hardsampling, reduce_neg );

					std::cout << std::endl;

					// サンプル数が足りない場合はFP率を変更する
					if( cindx < train.size( ) )
					{
						if( number_of_positive_samples2 < number_of_negative_samples * 2 && ( static_cast< difference_type >( cindx ) - number_of_positive_samples2 ) * 3 < number_of_positive_samples2 * 2 )
						{
							std::cout << "Stopping condition changed ..." << std::endl;
							max_false_alarm = 0;
						}
						else
						{
							double nfp = ( cindx - number_of_positive_samples2 ) / static_cast< double >( number_of_negative_samples );
							if( nfp < max_false_alarm )
							{
								std::cout << "Stopping condition changed ..." << std::endl;
								max_false_alarm = 0;
							}
						}
					}
				}
			}
			else
			{
				break;
			}

			// ステージを 1 進める
			nstages++;
		}
#else
		classifier.learn( train, number_of_weaks, min_hitrate, total_false_alarm, true );
#endif


		// 次元削減をした場合は弱識別器のインデックスを振りなおす
#if defined( __ADABOOST_CASCADE_TYPE__ ) && __ADABOOST_CASCADE_TYPE__ != 0

#if defined( __ADABOOST_CASCADE_TYPE__ ) && __ADABOOST_CASCADE_TYPE__ == 3
		// 弱識別器に特徴量の情報を設定する
		for( size_type i = 0 ; i < classifier.classifiers( ).size( ) ; i++ )
		{
			classifier.classifiers( )[ i ].index( feature_idmap[ classifier.classifiers( )[ i ].index( ) ] );
		}
#else
		// 弱識別器に特徴量の情報を設定する
		for( size_type j = 0 ; j < classifier.classifiers( ).size( ) ; j++ )
		{
			for( size_type i = 0 ; i < classifier.classifiers( )[ j ].classifiers( ).size( ) ; i++ )
			{
				classifier.classifiers( )[ j ].classifiers( )[ i ].index( feature_idmap[ classifier.classifiers( )[ j ].classifiers( )[ i ].index( ) ] );
			}
		}
#endif

#else
		for( size_type i = 0 ; i < classifier.classifiers( ).size( ) ; i++ )
		{
			classifier.classifiers( )[ i ].index( feature_idmap[ classifier.classifiers( )[ i ].index( ) ] );
		}
#endif

#if defined( __ADABOOST_CASCADE_TYPE__ ) && __ADABOOST_CASCADE_TYPE__ == 3
		if( optimize )
		{
			std::cout << "Speed optimization for the detection ..." << std::endl;

			adaboost::color_image_type img, ori;

			mist::read_image( ori, pos_list[ 0 ] );

			mist::nearest::interpolate( ori, img, ori.width( ) * 10, ori.height( ) * 10 );

			adaboost::color_image_type timg( img.width( ) * 10, img.height( ) * 10 );

			for( size_t j = 0 ; j < img.height( ) ; j++ )
			{
				for( size_t i = 0 ; i < img.width( ) ; i++ )
				{
					timg( i + img.width( ), j + img.height( ) ) = img( i, j ); 
				}
			}

			std::vector< adaboost::integral_image_type > iimgs;
			adaboost::convert_image( timg, iimgs );
			adaboost::feature_calculator< adaboost::integral_image_type > fcalculator( iimgs, feature_list );

			mist::timer t;
			double maxtime = 1.0e100;
			adaboost_classifier_type best( classifier );

			size_type sid = 0;
			for( size_type c = 0 ; c < classifier.exits( ).size( ) ; c++ )
			{
				size_type eid = classifier.exits( )[ c ];

				std::vector< size_t > ids( eid - sid );
				for( size_t i = 0 ; i < ids.size( ) ; i++ )
				{
					ids[ i ] = i;
				}

				adaboost_classifier_type cbest( best );

				do
				{
					for( size_t i = 0 ; i < ids.size( ) ; i++ )
					{
						classifier.classifiers( )[ sid + i ] = cbest.classifiers( )[ sid + ids[ i ] ];
					}

					t.reset( );

					size_t count = 0, maxloop = 10000;
					for( size_t i = 0 ; i < maxloop ; i++ )
					{
						fcalculator.initialize( );
						if( classifier( fcalculator, feature_list, img.width( ), img.height( ), img.width( ) / ( double )ori.width( ) ) )
						{
							count++;
						}
					}

					if( !( count == 0 || count == maxloop ) )
					{
						std::cerr << "Speed optimization failed." << std::endl;
					}

					double tm = t.elapse( );

					if( tm < maxtime )
					{
						maxtime = tm;
						best = classifier;
					}
				} while( std::next_permutation( ids.begin( ), ids.end( ) ) );

				sid = eid;
			}

			classifier = best;
		}
#endif

		double tm = t.elapse( );

		std::cout << "Total Num. of Weaks: " << classifier.total_number_of_classifiers( ) << std::endl;
		std::cout << "Computation Time: ";

		std::string tmstr = "";
		if( tm < 60 )
		{
			tmstr += adaboost::utility::ToString( tm ) +  " sec.";
		}
		else if( tm < 3600 )
		{
			int min = static_cast< int >( tm / 60 );
			double sec = tm - min * 60;
			tmstr += adaboost::utility::ToString( min ) +  " min. " + adaboost::utility::ToString( sec ) +  " sec.";
		}
		else
		{
			int hor = static_cast< int >( tm / 3600 );
			int min = static_cast< int >( ( tm - hor * 3600 ) / 60 );
			double sec = tm - hor * 3600 - min * 60;
			tmstr += adaboost::utility::ToString( hor ) +  " hour " +  adaboost::utility::ToString( min ) +  " min. " + adaboost::utility::ToString( sec ) +  " sec.";
		}

		//std::cout << tmstr << std::endl;

		// 使用した特徴に関する情報を識別器に設定する
		{
			int used_colors[ __ADABOOST_NUMBER_OF_COLORS__ ];
			int used_features[ __ADABOOST_NUMBER_OF_FEATURES__ ];

			for( size_t i = 0 ; i < __ADABOOST_NUMBER_OF_COLORS__ ; i++ )
			{
				used_colors[ i ] = 0;
			}

			for( size_t i = 0 ; i < __ADABOOST_NUMBER_OF_FEATURES__ ; i++ )
			{
				used_features[ i ] = 0;
			}

			// 次元削減をした場合は弱識別器のインデックスを振りなおす
#if defined( __ADABOOST_CASCADE_TYPE__ ) && __ADABOOST_CASCADE_TYPE__ != 0

#if defined( __ADABOOST_CASCADE_TYPE__ ) && __ADABOOST_CASCADE_TYPE__ == 3
			// 弱識別器に特徴量の情報を設定する
			for( size_t i = 0 ; i < classifier.classifiers( ).size( ) ; i++ )
			{
				used_colors[ __GET_ADABOOST_FEATURE_COLOR__( feature_list[ classifier.classifiers( )[ i ].index( ) ].mode ) ]++;
				used_features[ __GET_ADABOOST_FEATURE_TYPE__( feature_list[ classifier.classifiers( )[ i ].index( ) ].mode ) ]++;
			}
#else
			// 弱識別器に特徴量の情報を設定する
			for( size_type j = 0 ; j < classifier.classifiers( ).size( ) ; j++ )
			{
				for( size_type i = 0 ; i < classifier.classifiers( )[ j ].classifiers( ).size( ) ; i++ )
				{
					used_colors[ __GET_ADABOOST_FEATURE_COLOR__( feature_list[ classifier.classifiers( )[ j ].classifiers( )[ i ].index( ) ].mode ) ]++;
					used_features[ __GET_ADABOOST_FEATURE_TYPE__( feature_list[ classifier.classifiers( )[ j ].classifiers( )[ i ].index( ) ].mode ) ]++;
				}
			}
#endif

#else
			for( size_type i = 0 ; i < classifier.classifiers( ).size( ) ; i++ )
			{
				used_colors[ __GET_ADABOOST_FEATURE_COLOR__( feature_list[ classifier.classifiers( )[ i ].index( ) ].mode ) ]++;
				used_features[ __GET_ADABOOST_FEATURE_TYPE__( feature_list[ classifier.classifiers( )[ i ].index( ) ].mode ) ]++;
			}
#endif

			std::string str = "Computation Time: " + tmstr + ",\n";

			str += "\t     \n";
			str += "\t     [# of features]\n";
			for( size_t i = 0 ; i < __ADABOOST_NUMBER_OF_FEATURES__ ; i++ )
			{
				if( used_features[ i ] > 0 )
				{
					str += "\t     ";
					str += adaboost::get_feature_string( __ADABOOST_PATTERN__( i, 0, 0 ) );
					str += " : " + adaboost::utility::ToString( used_features[ i ] ) + ",\n";
				}
			}

			str += "\t     \n";
			str += "\t     [# of color images]\n";
			for( size_t i = 0 ; i < __ADABOOST_NUMBER_OF_COLORS__ ; i++ )
			{
				if( used_colors[ i ] > 0 )
				{
					str += "\t     ";
					str += adaboost::get_color_string( i );
					str += " : " + adaboost::utility::ToString( __ADABOOST_PATTERN__( 0, 0, used_colors[ i ] ) ) + ",\n";
				}
			}

			// 最後の改行を削除する
			str = str.substr( 0, str.size( ) - 1 );
			classifier.comment( str );

			adaboost::utility::replace_string_all( str, "\t     ", "" );
			std::cout << str << std::endl;
		}

		// 学習結果を保存する
		classifier.save( xmlname, std::string( __ADABOOST_FEATURE_STRING__ ) + " + " + __ADABOOST_CLASSIFIER_STRING__ );
	}

	return( 0 );
}
