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

/// @file mist/io/jpeg.h
//!
//! @brief JPEG画像を読み書きするためのライブラリ
//!
//! 本ライブラリは，http://www.libpng.org/pub/png/ で開発が行われている libjpeg を利用している
//! エンコードとデコードは本ライブラリのAPIを用いて実装されているため，別途ライブラリを用意する必要がある
//!
#ifndef __INCLUDE_MIST_JPEG__
#define __INCLUDE_MIST_JPEG__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

// カラー画像の設定を読み込む
#ifndef __INCLUDE_MIST_COLOR_H__
#include "../config/color.h"
#endif

#ifndef __INCLUDE_MIST_LIMITS__
#include "../limits.h"
#endif


#include <iostream>
#include <string>

#include "data_for_shm.h"
#include "../../switch_release.h"


#if defined( __MIST_WINDOWS__ ) && __MIST_WINDOWS__ > 0

	#define XMD_H
	#define HAVE_INT32			// JPEG用INT32型を持っている宣言
	#define HAVE_BOOLEAN		// JPEG用boolean型を持っている宣言

#endif

extern "C"
{
//#include <jpeglib.h>
#include "jpeglib.h"
}


// mist名前空間の始まり
_MIST_BEGIN


namespace __jpeg_controller__
{
	template < class T, class Allocator >
	struct jpeg_controller
	{
		typedef _pixel_converter_< T > pixel_converter;
		typedef typename pixel_converter::color_type color_type;

		static bool read( array2< T, Allocator > &image, const std::string &filename )
		{
#if 0
			FILE *fin;				// 読み書き用ファイルポインター
			fin = fopen( filename.c_str( ), "rb" );
			if( fin == NULL ) return( false );

			JDIMENSION i, j;
			JSAMPROW bitmap[1];				// ビットマップデータ配列へのポインター
			jpeg_decompress_struct dinfo;	// JPEG解凍情報構造体
			jpeg_error_mgr jerr;			// JPEGエラー処理用構造体
			int scanlen;					// ビットマップ１行のバイト数

			dinfo.err = jpeg_std_error( &jerr );
			jpeg_create_decompress( &dinfo );

			//jpeg_stdio_src(&dinfo, fin, 0);
			jpeg_stdio_src( &dinfo, fin );

			int n = jpeg_read_header( &dinfo, true );
			if( n < 1 )
			{
				jpeg_destroy_decompress( &dinfo );
				return(false);
			}

			jpeg_start_decompress( &dinfo );
			scanlen = dinfo.output_width * dinfo.output_components;

			image.resize( dinfo.output_width, dinfo.output_height );

			JSAMPLE *buffer = new JSAMPLE[ scanlen ];
			for( j = 0 ; j < dinfo.output_height ; j++ )
			{
				bitmap[0] = &buffer[0];
				if( dinfo.output_scanline < dinfo.output_height ) jpeg_read_scanlines( &dinfo, bitmap, 1 );
				for( i = 0 ; i < dinfo.output_width ; i++ )
				{
					switch( dinfo.output_components )
					{
					case 1:
                        image( i, j ) = pixel_converter::convert_to( buffer[ i ], buffer[ i ], buffer[ i ] );
						break;

					case 3:
                        image( i, j ) = pixel_converter::convert_to( buffer[ i * 3 + 0 ], buffer[ i * 3 + 1 ], buffer[ i * 3 + 2 ] );
						break;
						
					default:
						break;
					}
				}
			}

			jpeg_finish_decompress( &dinfo );

			fclose( fin );
			delete [] buffer;

			jpeg_destroy_decompress( &dinfo );

			return( true );
#else
            /*************************************************************************************************/
            /*************************************************************************************************/
            /*************************************************************************************************/
            // for reading from shared memory
            /*************************************************************************************************/


#ifdef PRINT_INFO
            /* display attention */
            std::cerr << "ATTENTION :input \""<< filename << "\" is replaced to Shared Memory"<< std::endl;
#endif  /* ifdef PRINT_INFO */

			/* FILE *fin;				// 読み書き用ファイルポインター */
			/* fin = fopen( filename.c_str( ), "rb" ); */
			/* if( fin == NULL ) return( false ); */
            
			JDIMENSION i, j;
			JSAMPROW bitmap[1];				// ビットマップデータ配列へのポインター
			jpeg_decompress_struct dinfo;	// JPEG解凍情報構造体
			jpeg_error_mgr jerr;			// JPEGエラー処理用構造体
			int scanlen;					// ビットマップ１行のバイト数
            
			dinfo.err = jpeg_std_error( &jerr );
			jpeg_create_decompress( &dinfo );
            
            
            /* generate shared memory key */
            key_t shm_key_input = ftok(INPUT_SHM_PATH, 1);
            if(shm_key_input == -1) {
                printf("key generation for input is failed\n");
            }
            
            /* access to the shared memory */
            int shrd_id_input = shmget(shm_key_input, IMAGE_SIZE, 0666);
            if(shrd_id_input < 0) {
                printf("Can't Access to the Shared Memory!!\n");
            }
            unsigned char *shrd_ptr_input = (unsigned char*)shmat(shrd_id_input, NULL, 0);

            /* open shared memory as a stream (read image from shared memory) */
            FILE *fin = fmemopen(shrd_ptr_input, IMAGE_SIZE, "rb");
            if(fin == NULL) return(false);
            
			//jpeg_stdio_src(&dinfo, fin, 0);
			jpeg_stdio_src( &dinfo, fin );
            
            /* read jpg image from shared memory */
            //jpeg_mem_src( &dinfo, shrd_ptr_input, IMAGE_SIZE );
            
            
			int n = jpeg_read_header( &dinfo, true );
			if( n < 1 )
			{
				jpeg_destroy_decompress( &dinfo );
				return(false);
			}
            
			jpeg_start_decompress( &dinfo );
			scanlen = dinfo.output_width * dinfo.output_components;
            
			image.resize( dinfo.output_width, dinfo.output_height );
            
			JSAMPLE *buffer = new JSAMPLE[ scanlen ];
			for( j = 0 ; j < dinfo.output_height ; j++ )
			{
				bitmap[0] = &buffer[0];
				if( dinfo.output_scanline < dinfo.output_height ) jpeg_read_scanlines( &dinfo, bitmap, 1 );
				for( i = 0 ; i < dinfo.output_width ; i++ )
				{
					switch( dinfo.output_components )
					{
					case 1:
                        image( i, j ) = pixel_converter::convert_to( buffer[ i ], buffer[ i ], buffer[ i ] );
						break;
                        
					case 3:
                        image( i, j ) = pixel_converter::convert_to( buffer[ i * 3 + 0 ], buffer[ i * 3 + 1 ], buffer[ i * 3 + 2 ] );
						break;
						
					default:
						break;
					}
				}
			}
            
			jpeg_finish_decompress( &dinfo );
            
			/* fclose( fin ); */
			delete [] buffer;
            
			jpeg_destroy_decompress( &dinfo );
            
			return( true );


            /*************************************************************************************************/
            // for reading from shared memory
            /*************************************************************************************************/
            /*************************************************************************************************/
            /*************************************************************************************************/

            
#endif
		}










		static bool read_custom( array2< T, Allocator > &image, const std::string &filename, int MODE)
		{

            /*************************************************************************************************/
            /*************************************************************************************************/
            /*************************************************************************************************/
            // for reading from shared memory
            /*************************************************************************************************/

#ifdef PRINT_INFO
            /* display attention */
            std::cerr << "ATTENTION :input \""<< filename << "\" is replaced to Shared Memory"<< std::endl;
#endif  /* ifdef PRINT_INFO */
            

			/* FILE *fin;				// 読み書き用ファイルポインター */
			/* fin = fopen( filename.c_str( ), "rb" ); */
			/* if( fin == NULL ) return( false ); */
            
			JDIMENSION i, j;
			JSAMPROW bitmap[1];				// ビットマップデータ配列へのポインター
			jpeg_decompress_struct dinfo;	// JPEG解凍情報構造体
			jpeg_error_mgr jerr;			// JPEGエラー処理用構造体
			int scanlen;					// ビットマップ１行のバイト数
            
			dinfo.err = jpeg_std_error( &jerr );
			jpeg_create_decompress( &dinfo );
            
            
            /* generate shared memory key */
            key_t shm_key_input;            
            switch(MODE) {
            case FOR_INPUT:
                shm_key_input = ftok(INPUT_SHM_PATH, 1);
                break;
            case FOR_OUTPUT:
                shm_key_input = ftok(OUTPUT_SHM_PATH, 1);
                break;
            default:
                return(false);
            }

            if(shm_key_input == -1) {
                printf("key generation for input is failed\n");
            }
            
            /* access to the shared memory */
            int shrd_id_input = shmget(shm_key_input, IMAGE_SIZE, 0666);
            if(shrd_id_input < 0) {
                printf("Can't Access to the Shared Memory!!\n");
            }
            unsigned char *shrd_ptr_input = (unsigned char*)shmat(shrd_id_input, NULL, 0);

            /* open shared memory as a stream (read image from shared memory) */
            FILE *fin = fmemopen(shrd_ptr_input, IMAGE_SIZE, "rb");
            if(fin == NULL) return(false);
            
			//jpeg_stdio_src(&dinfo, fin, 0);
			jpeg_stdio_src( &dinfo, fin );
            
            /* read jpg image from shared memory */
            //jpeg_mem_src( &dinfo, shrd_ptr_input, IMAGE_SIZE );
            
            
			int n = jpeg_read_header( &dinfo, true );
			if( n < 1 )
			{
				jpeg_destroy_decompress( &dinfo );
				return(false);
			}
            
			jpeg_start_decompress( &dinfo );
			scanlen = dinfo.output_width * dinfo.output_components;
            
			image.resize( dinfo.output_width, dinfo.output_height );
            
			JSAMPLE *buffer = new JSAMPLE[ scanlen ];
			for( j = 0 ; j < dinfo.output_height ; j++ )
			{
				bitmap[0] = &buffer[0];
				if( dinfo.output_scanline < dinfo.output_height ) jpeg_read_scanlines( &dinfo, bitmap, 1 );
				for( i = 0 ; i < dinfo.output_width ; i++ )
				{
					switch( dinfo.output_components )
					{
					case 1:
                        image( i, j ) = pixel_converter::convert_to( buffer[ i ], buffer[ i ], buffer[ i ] );
						break;
                        
					case 3:
                        image( i, j ) = pixel_converter::convert_to( buffer[ i * 3 + 0 ], buffer[ i * 3 + 1 ], buffer[ i * 3 + 2 ] );
						break;
						
					default:
						break;
					}
				}
			}
            
			jpeg_finish_decompress( &dinfo );
            
			/* fclose( fin ); */
			delete [] buffer;
            
			jpeg_destroy_decompress( &dinfo );
            
			return( true );


            /*************************************************************************************************/
            // for reading from shared memory
            /*************************************************************************************************/
            /*************************************************************************************************/
            /*************************************************************************************************/
		}






















		static bool write( const array2< T, Allocator > &image, const std::string &filename, int quality )
		{
			if( image.width( ) == 0 )
			{
				std::cerr << "Image width is zero!" << std::endl;
				return( false );
			}
			else if( image.height( ) == 0 )
			{
				std::cerr << "Image height is zero!" << std::endl;
				return( false );
			}
			
#if 0
			FILE *fout;						// 読み書き用ファイルポインター
			fout = fopen( filename.c_str( ), "wb" );
			if( fout == NULL ) return( false );
#else
            /*************************************************************************************************/
            /*************************************************************************************************/
            /*************************************************************************************************/
            // for writing to shared memory
            /*************************************************************************************************/

#ifdef PRINT_INFO
            /* display attention */
            std::cerr << "ATTENTION :output \""<< filename << "\" is replaced to Shared Memory"<< std::endl;
#endif  /* ifdef PRINT_INFO */

            /* generate shared memory key */
            key_t shm_key_output = ftok(OUTPUT_SHM_PATH, 1);
            if(shm_key_output == -1) {
                printf("key generation for output is failed\n");
            }
            
            /* access to the shared memory */
            int shrd_id_output = shmget(shm_key_output, IMAGE_SIZE, 0666);
            if(shrd_id_output < 0) {
                printf("Can't Access to the Shared Memory!!\n");
            }
            unsigned char *shrd_ptr_output = (unsigned char*)shmat(shrd_id_output, NULL, 0);
            
            /* open shared memory as a stream (write image to shared memory) */
            FILE *fout = fmemopen(shrd_ptr_output, IMAGE_SIZE, "wb");
            if(fout == NULL) return(false);

            /*************************************************************************************************/
            // for writing to shared memory
            /*************************************************************************************************/
            /*************************************************************************************************/
            /*************************************************************************************************/


#endif

			JDIMENSION i, j;
			JDIMENSION w = static_cast< JDIMENSION >( image.width( ) ), h = static_cast< JDIMENSION >( image.height( ) );
			JSAMPROW bitmap[1];				/* pointer to JSAMPLE row[s] */
			jpeg_compress_struct cinfo;		// JPEG解凍情報構造体
			jpeg_error_mgr jerr;			// JPEGエラー処理用構造体
			//int linelen;					// ビットマップ１行の正味バイト数
			int scanlen = w * 3;			// ビットマップ１行のバイト数

			cinfo.err = jpeg_std_error( &jerr );
			jpeg_create_compress( &cinfo );
			//jpeg_stdio_dest(&cinfo, fout, 0);
			jpeg_stdio_dest( &cinfo, fout );
			cinfo.image_width = w;
			cinfo.image_height = h;
			cinfo.input_components = 3;
			cinfo.in_color_space = JCS_RGB;
			jpeg_set_defaults( &cinfo );
			jpeg_set_quality( &cinfo, quality, true );
			jpeg_start_compress( &cinfo, true );

			JSAMPLE *buffer = new JSAMPLE[ w * h * 3 ];
			JSAMPLE *p = buffer;

			for( j = 0 ; j < h ; j++ )
			{
				for( i = 0 ; i < w ; i++ )
				{
					color_type c = limits_0_255( pixel_converter::convert_from( image( i, j ) ) );
					*p++ = static_cast< JSAMPLE >( c.r );
					*p++ = static_cast< JSAMPLE >( c.g );
					*p++ = static_cast< JSAMPLE >( c.b );
				}
			}

			while( cinfo.next_scanline < cinfo.image_height )
			{
				bitmap[0] = &buffer[ cinfo.next_scanline * scanlen ];
				jpeg_write_scanlines( &cinfo, bitmap, 1 );
			}

			jpeg_finish_compress( &cinfo );

			fclose( fout );
			delete [] buffer;

			jpeg_destroy_compress( &cinfo );

			return( true );
		}
	};
}


//! @addtogroup image_group 画像入出力
//!  @{

//! @addtogroup image_jpeg_group JPEG 画像入出力
//!
//! JPEGファイルを読み込んで，MISTコンテナへ格納する．
//! また，MISTコンテナからJPEGファイルへの書き出しをサポート．
//! 別途 Independent JPEG Group (http://www.ijg.org/) が提供するフリーのJPEGライブラリが必要です．
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/io/jpeg.h>
//! @endcode
//!
//!  @{


/// @brief JPEG画像をMISTコンテナに読み込む
//! 
//! 全てのJPEG画像形式（ロスレスのJPEGを除く）を読み込むことが可能です
//! 
//! @param[out] image    … 画像を読み込む先のMISTコンテナ
//! @param[in]  filename … 入力ファイル名
//!
//! @retval true  … 画像の読み込みに成功
//! @retval false … 画像の読み込みに失敗
//! 
template < class T, class Allocator >
bool read_jpeg( array2< T, Allocator > &image, const std::string &filename )
{
	return( __jpeg_controller__::jpeg_controller< T, Allocator >::read( image, filename ) );
}

template < class T, class Allocator >
bool read_jpeg_custom( array2< T, Allocator > &image, const std::string &filename , int MODE)
{
	return( __jpeg_controller__::jpeg_controller< T, Allocator >::read_custom( image, filename , MODE) );
}


/// @brief JPEG画像をMISTコンテナに読み込む
//! 
//! 全てのJPEG画像形式（ロスレスのJPEGを除く）を読み込むことが可能です
//! 
//! @param[out] image    … 画像を読み込む先のMISTコンテナ
//! @param[in]  filename … 入力ファイル名
//!
//! @retval true  … 画像の読み込みに成功
//! @retval false … 画像の読み込みに失敗
//! 
template < class T, class Allocator >
bool read_jpeg( array2< T, Allocator > &image, const std::wstring &filename )
{
	return( read_jpeg( image, wstr2str( filename ) ) );
}

template < class T, class Allocator >
bool read_jpeg_custom( array2< T, Allocator > &image, const std::wstring &filename , int MODE)
{
	return( read_jpeg_custom( image, wstr2str( filename ) , MODE) );
}


/// @brief MISTコンテナの画像をJPEG形式でファイルに出力する
//! 
//! @attention JPEGの品質は0から100まであり，数値が大きいほど圧縮率は低くなる．小さくすると，圧縮率が上がるかわりに画質が悪くなるので注意．（デフォルト値は100）
//! 
//! @param[in] image    … 出力画像を保持するMISTコンテナ
//! @param[in] filename … 出力ファイル名
//! @param[in] quality  … 出力するJPEG画像の品質
//!
//! @retval true  … 画像の書き込みに成功
//! @retval false … 画像の書き込みに失敗
//! 
template < class T, class Allocator >
bool write_jpeg( const array2< T, Allocator > &image, const std::string &filename, int quality = 100 )
{
	return( __jpeg_controller__::jpeg_controller< T, Allocator >::write( image, filename, quality ) );
}


/// @brief MISTコンテナの画像をJPEG形式でファイルに出力する
//! 
//! @attention JPEGの品質は0から100まであり，数値が大きいほど圧縮率は低くなる．小さくすると，圧縮率が上がるかわりに画質が悪くなるので注意．（デフォルト値は100）
//! 
//! @param[in] image    … 出力画像を保持するMISTコンテナ
//! @param[in] filename … 出力ファイル名
//! @param[in] quality  … 出力するJPEG画像の品質
//!
//! @retval true  … 画像の書き込みに成功
//! @retval false … 画像の書き込みに失敗
//! 
template < class T, class Allocator >
bool write_jpeg( const array2< T, Allocator > &image, const std::wstring &filename, int quality = 100 )
{
	return( write_jpeg( image, wstr2str( filename ), quality ) );
}


/// @}
//  JPEG 画像入出力グループの終わり

/// @}
//  画像入出力グループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_JPEG__
