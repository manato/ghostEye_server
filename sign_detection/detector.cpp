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
#include <map>

#include <stdio.h>

#ifdef _OPENMP
#include <omp.h>
#endif

#include <mist/mist.h>
#include <mist/integral_image.h>
#include <mist/io/image.h>
#include <mist/utility/options.h>

#include <mist/timer.h>
#include <mist/drawing.h>
#include <mist/converter.h>

#include "utility.h"
#include "classifier.h"
#include "validation.h"
#include "clustering.h"


#include "mist/io/data_for_shm.h"
#include "switch_release.h"

int main( int argc, char *argv[] )
{
	typedef adaboost::color_type color_type;
	typedef adaboost::image_type image_type;
	typedef adaboost::color_image_type color_image_type;
	typedef adaboost::integral_image_type integral_image_type;
	typedef adaboost::feature feature_type;
	typedef adaboost::feature_value_type feature_value_type;
	typedef std::multimap< std::string, adaboost::validation_data >	validation_map_type;
	typedef validation_map_type::value_type							validation_value_type;

	std::string header;

	header += "【カスケード型AdaBoost識別器の検出プログラム】\n\n";
	header += std::string( __ADABOOST_FEATURE_STRING__ ) + " + " + __ADABOOST_CLASSIFIER_STRING__ + "\n\n";
	header += "[使用例]\n";
	header += "AdaDetector [識別器のXMLファイル] [入力画像] [出力画像（out.png）]\n";
	header += "※「出力画像」を省略した場合は実行ファイルのディレクトリに「out.png」が生成される\n\n";
	header += "[オプション]";

	mist::options op( header );

	op.add( "nscales", "検出窓の大きさを変化させる回数", 6 );
	op.add( "output", "検出結果の枠を出力するファイル名（指定しない場合は出力しない）", "" );
	op.add( "merge", "検出結果の枠をマージするかどうか（指定しない場合はマージしない）" );
	op.add( "mindetections", "検出結果の枠をマージする際に対応する検出枠が少ない物を削除するしきい値", 1 );
	op.add( "overlap", "検出結果の枠をマージする際の重なり率に関するしきい値", 0.7 );
	op.add( "noweight", "検出結果の枠をマージする際に各枠の重みを考慮するかどうか" );
	op.add( "nooutput", "検出結果の枠を標準出力に出力し，それ以外の出力を行わない" );

#ifdef _OPENMP
	op.add( "nthreads", "使用するスレッド数（-1を指定するとCPU数を自動設定）", 2 );
#endif

	if( !op.parse_args( argc, argv ) )
	{
		op.show_help( );
		return( 1 );
	}
	else if( op.size( ) < 2 )
	{
		std::cerr << "識別器のデータもしくは検出対象の画像が指定されていません．" << std::endl << std::endl;
		op.show_help( );
		return( 1 );
	}

	bool   bnooutput               = op.isset( "nooutput" );
	std::string xml                = op[ 0 ];
	std::string ifname             = op[ 1 ];
	std::string ofname             = op.size( ) > 2 ? op[ 2 ] : "out.png";
	std::string oxml               = op.get_string( "output" );
	size_t number_of_scales        = op.get_int( "nscales" );
	bool   bmerge_results          = op.isset( "merge" );
	bool   noweight                = op.isset( "noweight" );
	double number_of_mindetections = op.get_double( "mindetections" );
	double overlap_threshold       = op.get_double( "overlap" );

#ifdef _OPENMP
	int __nthreads__ = op.get_int( "nthreads" );
	if( __nthreads__ > 0 )
	{
		omp_set_num_threads( __nthreads__ );
	}
#endif

	// 出力用のフォルダを作成する
	if( !bnooutput )
	{
		adaboost::utility::make_dir( ofname, false );
		adaboost::utility::make_dir( oxml, false );
	}


	// AdaBoost識別器を定義する
	__DEFINE_ADABOOST_CLASSIFIER_TYPE__;

	adaboost_classifier_type classifier;


	validation_map_type olist;

	if( !classifier.load( xml ) )
	{
		std::cerr << "検出器のXMLファイルを読み込むことができませんでした．" << std::endl;
		return( 1 );
	}

	std::vector< std::string > params;
	adaboost::utility::split_string( classifier.tag( ), params, "," );

	if( params.size( ) != 2 )
	{
		std::cerr << "本プログラムでは使用できない識別器です．" << std::endl;
	}

	std::vector< adaboost::feature_info > feature_list;
	int image_width  = atoi( params[ 0 ].c_str( ) );
	int image_height = atoi( params[ 1 ].c_str( ) );

	{
		image_type tmp( image_width, image_height );

		// 特徴量のリストを作成する
		adaboost::construct_feature_list( tmp, feature_list );
	}

	{
		// 識別器で使用する特徴量のみに特徴量のリストを削減する
#if defined( __ADABOOST_CASCADE_TYPE__ ) && ( __ADABOOST_CASCADE_TYPE__ == 1 || __ADABOOST_CASCADE_TYPE__ == 2 )
		std::map< size_t, size_t > fmap;
		for( size_t b = 0 ; b < classifier.classifiers( ).size( ) ; b++ )
		{
			adaboost_classifier_type::classifier_list_type::value_type &clsf = classifier.classifiers( )[ b ];
			for( size_t i = 0 ; i < clsf.classifiers( ).size( ) ; i++ )
			{
				fmap[ clsf.classifiers( )[ i ].index( ) ] = 1;
			}
		}

		std::map< size_t, size_t >::iterator ite = fmap.begin( );
		std::vector< adaboost::feature_info > feature_list_new;
		for( size_t i = 0 ; ite != fmap.end( ) ; ++ite, ++i )
		{
			feature_list_new.push_back( feature_list[ ite->first ] );
			ite->second = i;
		}

		for( size_t b = 0 ; b < classifier.classifiers( ).size( ) ; b++ )
		{
			adaboost_classifier_type::classifier_list_type::value_type &clsf = classifier.classifiers( )[ b ];
			for( size_t i = 0 ; i < clsf.classifiers( ).size( ) ; i++ )
			{
				clsf.classifiers( )[ i ].index( fmap[ clsf.classifiers( )[ i ].index( ) ] );
			}
		}
#else
		// 識別器で使用する特徴量のみに特徴量のリストを削減する
		std::map< size_t, size_t > fmap;
		for( size_t b = 0 ; b < classifier.classifiers( ).size( ) ; b++ )
		{
			adaboost_classifier_type::classifier_list_type::value_type &clsf = classifier.classifiers( )[ b ];
			fmap[ clsf.index( ) ] = 1;
		}
		
		std::map< size_t, size_t >::iterator ite = fmap.begin( );
		std::vector< adaboost::feature_info > feature_list_new;
		for( size_t i = 0 ; ite != fmap.end( ) ; ++ite, ++i )
		{
			feature_list_new.push_back( feature_list[ ite->first ] );
			ite->second = i;
		}

		for( size_t b = 0 ; b < classifier.classifiers( ).size( ) ; b++ )
		{
			adaboost_classifier_type::classifier_list_type::value_type &clsf = classifier.classifiers( )[ b ];
			clsf.index( fmap[ clsf.index( ) ] );
		}
#endif
		
		feature_list = feature_list_new;
	}


    // generate key for shared memory
    // key_t shm_key_rbuf_dst = ftok(RBUF_DST_PATH, 1);
    // if(shm_key_rbuf_dst == -1) { // error semantics
    //   printf("key generation for rbuf_dst_SHM is failed\n");
    //   return(1);
    // }
    
    key_t shm_key_rbuf = ftok(RBUF_PATH, 1);
    if(shm_key_rbuf == -1) { // error semantics
      printf("key generation for rbuf_SHM is failed\n");
      return(1);
    }
    
    key_t shm_key_rbuf_head = ftok(RBUF_HEAD_PATH, 1);
    if(shm_key_rbuf_head == -1) { // error semantics
      printf("key generation for rbuf_head_SHM is failed\n");
      return(1);
    }
    
    key_t shm_key_rbuf_tail = ftok(RBUF_TAIL_PATH, 1);
    if(shm_key_rbuf_tail == -1) { // error semantics
      printf("key generation for rbuf_tail_SHM is failed\n");
      return(1);
    }
    

    // generate key for semaphore
    key_t sem_key = ftok(SEM_PATH, 1);
    if(sem_key == -1) { // error semantics
      printf("key generation for semaphore is failed\n");
      return(1);
    }



    // key generation for image update checker 
    key_t shm_key_imgupd = ftok(IMGUPD_SHM_PATH, 1);
    if(shm_key_imgupd == -1) {  // error semantics
      printf("generation key for image update checker failed\n");
    }



    // access to the shared memory
    // int shrd_id_rbuf_dst = shmget(shm_key_rbuf_dst, RBUF_ELEMENT_NUM*sizeof(int), 0666);
    // if(shrd_id_rbuf_dst < 0) {  // error semantics
    //   printf("Can't Access to the Shared Memory for rbuf_dst!!\n");
    //   return(1);
    // }

    //    int shrd_id_rbuf = shmget(shm_key_rbuf, MAX_OBJECT_NUM*sizeof(int*), 0666);
    int shrd_id_rbuf = shmget(shm_key_rbuf, MAX_OBJECT_NUM*sizeof(obj_coordinate), 0666);
    if(shrd_id_rbuf < 0) {  // error semantics
      printf("Can't Access to the Shared Memory for rbuf!!\n");
      return(1);
    }

    int shrd_id_rbuf_head = shmget(shm_key_rbuf_head, sizeof(int), 0666);
    if(shrd_id_rbuf_head < 0) {  // error semantics
      printf("Can't Access to the Shared Memory for rbuf_head!!\n");
      return(1);
    }

    int shrd_id_rbuf_tail = shmget(shm_key_rbuf_tail, sizeof(int), 0666);
    if(shrd_id_rbuf_tail < 0) {  // error semantics
      printf("Can't Access to the Shared Memory for rbuf_tail!!\n");
      return(1);
    }


    // open semaphore
    int semid = semget(sem_key, 1, 0666);
    if(semid == -1) {
      printf("Can't Access to the semaphore\n");
      return(1);
    }

    

    // attach the shared memory
    //    int *shrd_ptr_rbuf_dst = (int *)shmat(shrd_id_rbuf_dst, NULL, 0);
    //    int **shrd_ptr_rbuf = (int **)shmat(shrd_id_rbuf, NULL, 0);
    obj_coordinate *shrd_ptr_rbuf = (obj_coordinate *)shmat(shrd_id_rbuf, NULL, 0);
    int *shrd_ptr_rbuf_head = (int *)shmat(shrd_id_rbuf_head, NULL, 0);
    int *shrd_ptr_rbuf_tail = (int *)shmat(shrd_id_rbuf_tail, NULL, 0);

    
    
    // int *tmpptr = shrd_ptr_rbuf_dst;
    // for(int i=0; i<MAX_OBJECT_NUM; i++) {
    //   shrd_ptr_rbuf[i] = tmpptr;
    //   tmpptr += CO_NUM;
    // }


    while(1){

	// 検出してみる
	{
		color_image_type out;
        //		color_image_type out2;
		image_type img;

        //        printf("%s\n", ifname.c_str());

        //		if( !mist::read_image( out, ifname) || !mist::read_image( out2, ifname))
        //		if( !mist::read_image_custom( out, ifname, FOR_INPUT) || !mist::read_image_custom( out2, ifname, FOR_OUTPUT))
        		if( !mist::read_image_custom( out, ifname, FOR_INPUT))
          {
			// ファイルが読めないみたい・・・
			std::cerr << "ファイル [" << ifname << "] の読み込みに失敗しました．" << std::endl;
			return( 1 );
		}

		mist::timer t;

		std::vector< adaboost::integral_image_type > iimgs;
		adaboost::convert_image( out, iimgs );
		adaboost::feature_calculator< adaboost::integral_image_type > fcalculator( iimgs, feature_list );

		std::vector< adaboost::validation_data > detected_list;
		double scale = 1.0;

		for( size_t s = 0 ; s < number_of_scales ; s++ )
		{
			size_t w = static_cast< size_t >( image_width  * scale + 0.5 );
			size_t h = static_cast< size_t >( image_height * scale + 0.5 );
			int www = static_cast< int >( out.width( )  - w );
			int hhh = static_cast< int >( out.height( ) - h );

#if defined( __SHRINK_IMAGE_WHEN_DETECTION__ ) && __SHRINK_IMAGE_WHEN_DETECTION__ == 0
			#pragma omp parallel for firstprivate( www, hhh, w, h, scale, fcalculator ) schedule( guided )
			for( int j = 0 ; j <= hhh ; j++ )
			{
				for( int i = 0 ; i <= www ; i++ )
				{
#else
			int nw  = static_cast< int >( www / scale );
			int nh  = static_cast< int >( hhh / scale );

			#pragma omp parallel for firstprivate( w, h, scale, fcalculator ) schedule( guided )
			for( int jj = 0 ; jj < nh ; jj++ )
			{
				int j = static_cast< int >( ( jj + 0.5 ) * scale + 0.5 );
				for( int ii = 0 ; ii < nw ; ii++ )
				{
					int i = static_cast< int >( ( ii + 0.5 ) * scale + 0.5 );
#endif

					// 特徴量計算器を初期化する
					fcalculator.initialize( );

#if defined( __ADABOOST_CLASSIFIER_TYPE__ ) && ( __ADABOOST_CLASSIFIER_TYPE__ == 1 || __ADABOOST_CLASSIFIER_TYPE__ == 2 )
					double conf = classifier.confidence( fcalculator, feature_list, i, j, scale );
					if( conf >= 0 )
					{
						#pragma omp critical
						{
							detected_list.push_back( adaboost::validation_data( i, j, w, h, 1, conf ) );
						}
					}
#else
					if( classifier( fcalculator, feature_list, i, j, scale ) )
					{
						#pragma omp critical
						{
							detected_list.push_back( adaboost::validation_data( i, j, w, h ) );
						}
					}
#endif
				}
			}

			scale *= __FEATURE_SCALING_FACTOR__;
		}

		// 結果を統合する
		if( bmerge_results )
		{
			adaboost::clustering_results( detected_list, out.width( ), out.height( ), number_of_mindetections, overlap_threshold, !noweight );
		}

		double tm = t.elapse( );

		if( !bnooutput )
		{
#ifdef PRINT_INFO
			std::cout << "Number of Detections: " << detected_list.size( ) << std::endl;
			std::cout << "Computation Time: " << tm << " sec." << std::endl;
#endif  // ifdef PRINT_INFO
		}

		for( size_t i = 0 ; i < detected_list.size( ) ; i++ )
		{
			const adaboost::validation_data &d = detected_list[ i ];
            //			mist::draw_rect( out, d.x, d.y, d.x + d.w - 1, d.y + d.h - 1, mist::colors< color_type >::orange( ) );
            //            			mist::draw_rect( out2, d.x, d.y, d.x + d.w - 1, d.y + d.h - 1, mist::colors< color_type >::orange( ) );
            mist::draw_rect_custom( out, 
                                    d.x, 
                                    d.y, 
                                    d.x + d.w - 1,
                                    d.y + d.h - 1,
                                    mist::colors< color_type >::orange( ),
                                    //shrd_ptr_rbuf,      // int **rbuf
                                    shrd_ptr_rbuf,      // obj_coordinate *rbuf
                                    shrd_ptr_rbuf_head, // int *head 
                                    shrd_ptr_rbuf_tail, // int *tail
                                    semid               // int semid
                                    );

			if( !oxml.empty( ) )
			{
				olist.insert( validation_map_type::value_type( ifname, d ) );
			}
		}

		if( !bnooutput )  
		{                       
          mist::write_image( out, ofname ); // output detection result
          //          			mist::write_image( out2, ofname ); // output detection result
		}

		if( bnooutput )
		{
			for( size_t i = 0 ; i < detected_list.size( ) ; i++ )
			{
				const adaboost::validation_data &d = detected_list[ i ];
#ifdef PRINT_INFO
				printf( "%ld,%ld,%ld,%ld,%f\n", d.x, d.y, d.w, d.h, d.weight );
#endif  // ifdef PRINT_INFO
			}
		}
		else if( !oxml.empty( ) )
		{
			// 検出枠をソートする
			validation_map_type tmap;
			validation_map_type::iterator lite = olist.begin( );
			for( ; lite != olist.end( ) ; )
			{
				const std::string fname = lite->first;
				validation_map_type::iterator uite = olist.upper_bound( lite->first );
				std::vector< adaboost::validation_data > vtmp;
				for( ; lite != uite ; ++lite )
				{
					vtmp.push_back( lite->second );
				}

				std::sort( vtmp.begin( ), vtmp.end( ) );

				for( size_t i = 0 ; i < vtmp.size( ) ; i++ )
				{
					tmap.insert( validation_map_type::value_type( fname, vtmp[ i ] ) );
				}
			}

			adaboost::write_validation_data( tmap, oxml );
		}
	}

        }  // while(1)

	return( 0 );
}
