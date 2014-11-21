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

/// @file mist/io/image.h
//!
//! @brief BMP，PNG，PNM，TIFF，JPEG，GIF の画像を読み書きするためのライブラリ
//!
#ifndef __INCLUDE_MIST_IMAGE__
#define __INCLUDE_MIST_IMAGE__


#ifndef __INCLUDE_MIST_H__
#include "../mist.h"
#endif

#ifndef __INCLUDE_MIST_BMP__
#include "bmp.h"
#endif

#ifndef __INCLUDE_MIST_PNG__
#include "png.h"
#endif

#ifndef __INCLUDE_MIST_GIF__
#include "gif.h"
#endif

#ifndef __INCLUDE_MIST_TIFF__
#include "tiff.h"
#endif

#ifndef __INCLUDE_MIST_JPEG__
#include "jpeg.h"
#endif

#ifndef __INCLUDE_MIST_PNM__
#include "pnm.h"
#endif

#ifndef __INCLUDE_MIST_TGA__
#include "tga.h"
#endif

#include <iostream>
#include <string>


/* for use shared memory */
#include "data_for_shm.h"

// mist名前空間の始まり
_MIST_BEGIN


namespace __image_controller__
{
	inline std::string to_lower_case( const std::string &str )
	{
		std::string s = "";
		for( std::string::size_type i = 0 ; i < str.size( ) ; i++ )
		{
			s += static_cast< char >( tolower( str[ i ] ) );
		}
		return( s );
	}

	inline std::string get_ext( const std::string &str )
	{
		std::string::size_type index = str.rfind( '.' );
		if( index == str.npos )
		{
			return( "" );
		}
		return( str.substr( index ) );
	}
}


//! @addtogroup image_group 画像入出力
//!
//! @code 次のヘッダをインクルードする
//! #include <mist/io/image.h>
//! @endcode
//!
//!  @{



/// @brief JPEG，TIFF，PNG，BMP，PNM，GIF 画像を拡張子に基づいてMISTコンテナに読み込む
//! 
//! @attention 拡張子に基づいて画像形式を判別するため，正しいファイル拡張子が付いている必要あり
//! 
//! @param[out] image    … 画像を読み込む先のMISTコンテナ
//! @param[in]  filename … 入力ファイル名
//!
//! @retval true  … 画像の読み込みに成功
//! @retval false … 画像の読み込みに失敗
//! 

template < class T, class Allocator >
inline bool read_image( mist::array2< T, Allocator > &image, const std::string &filename ) 
    // 仮引数に & : その引数が参照渡しされる
{
#if 0
    typedef typename mist::array2< T, Allocator >::size_type size_type;
    
    std::string ext = __image_controller__::to_lower_case( __image_controller__::get_ext( filename ) );

    /* ext = ".bmp"; */
    /* std::cerr << "ext\""<< ext << "\"" << std::endl; */

    if( ext == "" )
    {
        return( false );
    }
    
    bool ret = false;
    if( ext == ".jpeg" || ext == ".jpg" )
    {
        ret = mist::read_jpeg( image, filename );
    }
    else if( ext == ".bmp" )
    {
        ret = mist::read_bmp( image, filename );
    }
    else if( ext == ".tiff" || ext == ".tif" )
    {
        ret = mist::read_tiff( image, filename );
    }
    else if( ext == ".png" )
    {
        ret = mist::read_png( image, filename );
    }
    else if( ext == ".pbm" || ext == ".pgm" || ext == ".ppm" || ext == ".pnm" )
    {
        ret = mist::read_pnm( image, filename );
    }
    else if( ext == ".tga" )
    {
        ret = mist::read_tga( image, filename );
    }
    else if( ext == ".gif" )
    {
        ret = mist::read_gif( image, filename );
    }
    else
    {
        ret = false;
    }
    return( ret );
#else

    /*************************************************************************************************/
    /*************************************************************************************************/
    /*************************************************************************************************/
    // for reading from shared memory
    /*************************************************************************************************/
    

    typedef typename mist::array2< T, Allocator >::size_type size_type;
    
    std::string ext = __image_controller__::to_lower_case( __image_controller__::get_ext( filename ) );


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


    /* file format check */
    char form_check[8]="";
    memcpy(form_check, shrd_ptr_input, 8);  // read 8byte from shared memory


    if(strncmp(form_check, "\x89PNG\x0d\x0a\x1a\x0a", 8)==0) {
#ifdef PRINT_INFO
        printf("input file format is PNG\n");
#endif  /* ifdef PRINT_INFO */
        ext=  ".png";
    }else if(strncmp(form_check, "BM", 2)==0) {
#ifdef PRINT_INFO
        printf("input file format is BITMAP\n");
#endif  /* ifdef PRINT_INFO */
        ext=  ".bmp";
    }else if(strncmp(form_check, "GIF87a", 6)==0  || strncmp(form_check, "GIF89a", 6)==0) {
#ifdef PRINT_INFO
        printf("input file format is GIF\n");
#endif  /* ifdef PRINT_INFO */
        ext=  ".gif";
    }else if(strncmp(form_check, "\xff\xd8", 2)==0) {
#ifdef PRINT_INFO
        printf("input file format is JPEG\n");
#endif  /* ifdef PRINT_INFO */
        ext=  ".jpg";
    }else{  // file format  = unknown
#ifdef PRINT_INFO
        printf("Can't recognize file format.....\n");
#endif  /* ifdef PRINT_INFO */
        ext = "";
    }



                
    if( ext == "" )
    {
        return( false );
    }
    
    bool ret = false;
    if( ext == ".jpeg" || ext == ".jpg" )
    {
        ret = mist::read_jpeg( image, filename );
    }
    else if( ext == ".bmp" )
    {
        ret = mist::read_bmp( image, filename );
    }
    else if( ext == ".tiff" || ext == ".tif" )
    {
        ret = mist::read_tiff( image, filename );
    }
    else if( ext == ".png" )
    {
        ret = mist::read_png( image, filename );
    }
    else if( ext == ".pbm" || ext == ".pgm" || ext == ".ppm" || ext == ".pnm" )
    {
        ret = mist::read_pnm( image, filename );
    }
    else if( ext == ".tga" )
    {
        ret = mist::read_tga( image, filename );
    }
    else if( ext == ".gif" )
    {
        ret = mist::read_gif( image, filename );
    }
    else
    {
        ret = false;
    }
    return( ret );

    /*************************************************************************************************/
    // for reading from shared memory
    /*************************************************************************************************/
    /*************************************************************************************************/
    /*************************************************************************************************/
    

#endif

}








template < class T, class Allocator >
inline bool read_image_custom( mist::array2< T, Allocator > &image, const std::string &filename , int MODE) 
    // 仮引数に & : その引数が参照渡しされる
{
    /*************************************************************************************************/
    /*************************************************************************************************/
    /*************************************************************************************************/
    // for reading from shared memory
    /*************************************************************************************************/
    

    typedef typename mist::array2< T, Allocator >::size_type size_type;
    
    std::string ext = __image_controller__::to_lower_case( __image_controller__::get_ext( filename ) );

    /***********************************************************/
    /* generate shared memory key */
    /***********************************************************/
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


    /* key generation for semaphore */
    key_t sem_key = ftok(SEM_PATH, 1);
    if(sem_key == -1) {
        printf("key generation for semaphore is failed\n");
    }


    /* generate key for reader-writer lock */
    key_t shm_key_rwlock = ftok(RWLOCK_SHM_PATH, 1);
    if(shm_key_rwlock == -1) {
      printf("key generation for reader-writer lock failed\n");
      return(1);
    }



    /* key generation for image update checker */
    key_t shm_key_imgupd = ftok(IMGUPD_SHM_PATH, 1);
    if(shm_key_imgupd == -1) {  // error semantics
      printf("generation key for image update checker failed\n");
    }



    /***********************************************************/
    /* access to the shared memory */
    /***********************************************************/
    int shrd_id_input = shmget(shm_key_input, IMAGE_SIZE, 0666);
    if(shrd_id_input < 0) {
        printf("Can't Access to the Shared Memory!!\n");
    }
    unsigned char *shrd_ptr_input = (unsigned char*)shmat(shrd_id_input, NULL, 0);

    /* open semaphore */
    int semid = semget(sem_key, 1, 0666);
    if(semid == -1) {
        printf("Can't Access to the semaphore\n");
    }


    /* open reader-writer lock */
    int shrd_id_rwlock = shmget(shm_key_rwlock, sizeof(pthread_rwlock_t), 0666);
    if(shrd_id_rwlock < 0) {
      printf("Can't Access to the Shared Memory for reader-writer lock\n");
      return(1);
    }

    pthread_rwlock_t *shrd_ptr_rwlock = (pthread_rwlock_t *)shmat(shrd_id_rwlock, NULL, 0);


    /* access shared image update checker */
    int shrd_id_imgupd = shmget(shm_key_imgupd, sizeof(char)*256, 0666);
    if(shrd_id_imgupd < 0) {  // error semantics
      printf("Can't Access Shared memory for image update checker...\n");
    }

    // attach image update checker
    char *shrd_ptr_imgupd = (char*)shmat(shrd_id_imgupd, NULL, 0);




    /***********************************************************/
    /* file format check */
    /***********************************************************/
    char form_check[8]="";

//    My_sem_operation(semid, LOCK); // lock semaphore
    pthread_rwlock_rdlock(shrd_ptr_rwlock);  // lock reader-writer lock as a reader
    memcpy(form_check, shrd_ptr_input, 8);  // read 8byte from shared memory
    pthread_rwlock_unlock(shrd_ptr_rwlock);  // unlock reader-writer lock
//    My_sem_operation(semid, UNLOCK); // unlock semaphore

    if(strncmp(form_check, "\x89PNG\x0d\x0a\x1a\x0a", 8)==0) {
#ifdef PRINT_INFO
        printf("input file format is PNG\n");
#endif  /* ifdef PRINT_INFO */
        ext=  ".png";
    }else if(strncmp(form_check, "BM", 2)==0) {
#ifdef PRINT_INFO
        printf("input file format is BITMAP\n");
#endif  /* ifdef PRINT_INFO */
        ext=  ".bmp";
    }else if(strncmp(form_check, "GIF87a", 6)==0  || strncmp(form_check, "GIF89a", 6)==0) {
#ifdef PRINT_INFO
        printf("input file format is GIF\n");
#endif  /* ifdef PRINT_INFO */
        ext=  ".gif";
    }else if(strncmp(form_check, "\xff\xd8", 2)==0) {
#ifdef PRINT_INFO
        printf("input file format is JPEG\n");
#endif  /* ifdef PRINT_INFO */
        ext=  ".jpg";
    }else{  // file format  = unknown
#ifdef PRINT_INFO
        printf("Can't recognize file format.....\n");
#endif  /* ifdef PRINT_INFO */
        ext = "";
    }

                
    if( ext == "" )
    {
        return( false );
    }


    // image update check
    static char imgupd_before[256] = {0};
    int upd_check = 0;
    while(1)  {
        My_sem_operation(semid, LOCK); // lock semaphore
        upd_check = strcmp(shrd_ptr_imgupd, imgupd_before);
        My_sem_operation(semid, UNLOCK); // unlock semaphore
        
        // if shrd_ptr_imgupd == imgupd_before, then continue loop
        if(upd_check != 0)
        {
            My_sem_operation(semid, LOCK); // lock semaphore
            strcpy(imgupd_before, shrd_ptr_imgupd);
            My_sem_operation(semid, UNLOCK); // unlock semaphore
            break;
        }
    }
    
    
//    My_sem_operation(semid, LOCK); // lock semaphore
    pthread_rwlock_rdlock(shrd_ptr_rwlock);  // lock reader-writer lock as a reader

    bool ret = false;
    if( ext == ".jpeg" || ext == ".jpg" )
    {
        ret = mist::read_jpeg_custom( image, filename , MODE);
    }
    else if( ext == ".bmp" )
    {
        ret = mist::read_bmp_custom( image, filename , MODE);
    }
    else if( ext == ".tiff" || ext == ".tif" )
    {
        ret = mist::read_tiff( image, filename );
    }
    else if( ext == ".png" )
    {
        ret = mist::read_png_custom( image, filename , MODE);
    }
    else if( ext == ".pbm" || ext == ".pgm" || ext == ".ppm" || ext == ".pnm" )
    {
        ret = mist::read_pnm( image, filename );
    }
    else if( ext == ".tga" )
    {
        ret = mist::read_tga( image, filename );
    }
    else if( ext == ".gif" )
    {
        ret = mist::read_gif_custom( image, filename , MODE);
    }
    else
    {
        ret = false;
    }
    pthread_rwlock_unlock(shrd_ptr_rwlock);  // unlock reader-writer lock 
//    My_sem_operation(semid, UNLOCK); // unlock semaphore

    return( ret );

    /*************************************************************************************************/
    // for reading from shared memory
    /*************************************************************************************************/
    /*************************************************************************************************/
    /*************************************************************************************************/
}





























/// @brief JPEG，TIFF，PNG，BMP，PNM，GIF 画像を拡張子に基づいてMISTコンテナに読み込む
//! 
//! @attention 拡張子に基づいて画像形式を判別するため，正しいファイル拡張子が付いている必要あり
//! 
//! @param[out] image    … 画像を読み込む先のMISTコンテナ
//! @param[in]  filename … 入力ファイル名
//!
//! @retval true  … 画像の読み込みに成功
//! @retval false … 画像の読み込みに失敗
//! 
template < class T, class Allocator >
inline bool read_image( mist::array2< T, Allocator > &image, const std::wstring &filename )
{
	return( read_image( image, wstr2str( filename ) ) );
    
}

template < class T, class Allocator >
inline bool read_image_custom( mist::array2< T, Allocator > &image, const std::wstring &filename , int MODE)
{
	return( read_image_custom( image, wstr2str( filename ) , MODE) );
    
}

/// @brief MISTコンテナの画像をTIFF形式でJPEG，TIFF，PNG，BMP，PNM，GIF 画像形式でファイルに出力する
//! 
//! @attention 拡張子に基づいて画像形式を判別するため，正しいファイル拡張子が付いている必要あり
//! @attention 出力されるファイルの圧縮率は，各 read_jpeg 等のデフォルト値が用いられる
//! 
//! @param[out] image    … 画像を読み込む先のMISTコンテナ
//! @param[in]  filename … 入力ファイル名
//!
//! @retval true  … 画像の読み込みに成功
//! @retval false … 画像の読み込みに失敗
//! 
template < class T, class Allocator >
inline bool write_image( const mist::array2< T, Allocator > &image, const std::string &filename )
{
	typedef typename mist::array2< T, Allocator >::size_type size_type;

	std::string ext = __image_controller__::to_lower_case( __image_controller__::get_ext( filename ) );

    /*************************************************************************************************/
    /*************************************************************************************************/
    /*************************************************************************************************/
    // for writing to shared memory
    /*************************************************************************************************/

    /* generate shared memory key */
    key_t shm_key_output = ftok(OUTPUT_SHM_PATH, 1);
    if(shm_key_output == -1) {
        printf("key generation for output is failed\n");
    }


    /* generate semaphore key */
    key_t sem_key = ftok(SEM_PATH, 1);  // key for semaphore
    if(sem_key == -1) {
        printf("key generation for semaphore is failed\n");
    }


    /* access to the shared memory */
    int shrd_id_output = shmget(shm_key_output, IMAGE_SIZE, 0666);
    if(shrd_id_output < 0) {
        printf("Can't Access to the Shared Memory!!\n");
    }
    unsigned char *shrd_ptr_output = (unsigned char*)shmat(shrd_id_output, NULL, 0);


    /* open semaphore */
    int semid = semget(sem_key, 1, 0666);
    if(semid == -1) {
        printf("Can't Access to the semaphore\n");
    }


    /* file format check */
    char form_check[8]="";

    My_sem_operation(semid, LOCK); // lock semaphore
    memcpy(form_check, shrd_ptr_output, 8);  // read 8byte from shared memory
    My_sem_operation(semid, UNLOCK); // unlock semaphore


    if(strncmp(form_check, "\x89PNG\x0d\x0a\x1a\x0a", 8)==0) {
#ifdef PRINT_INFO
        printf("output file format is PNG\n");
#endif  /* ifdef PRINT_INFO */
        ext=  ".png";
    }else if(strncmp(form_check, "BM", 2)==0) {
#ifdef PRINT_INFO
        printf("output file format is BITMAP\n");
#endif  /* ifdef PRINT_INFO */
        ext=  ".bmp";
    }else if(strncmp(form_check, "GIF87a", 6)==0  || strncmp(form_check, "GIF89a", 6)==0) {
#ifdef PRINT_INFO
        printf("output file format is GIF\n");
#endif  /* ifdef PRINT_INFO */
        ext=  ".gif";
    }else if(strncmp(form_check, "\xff\xd8", 2)==0) {
#ifdef PRINT_INFO
        printf("output file format is JPEG\n");
#endif  /* ifdef PRINT_INFO */
        ext=  ".jpg";
    }else{  // file format  = unknown
#ifdef PRINT_INFO
        printf("Can't recognize file format.....\n");
#endif  /* ifdef PRINT_INFO */
        ext = "";
    }

    /*************************************************************************************************/
    // for writing to shared memory
    /*************************************************************************************************/
    /*************************************************************************************************/
    /*************************************************************************************************/
    

	if( ext == "" )
	{
#ifdef PRINT_INFO
		std::cerr << "Unknown file type!" << std::endl;
#endif  /* ifdef PRINT_INFO */
		return( false );
	}

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

    My_sem_operation(semid, LOCK); // lock semaphore
	bool ret = false;
	if( ext == ".jpeg" || ext == ".jpg" )
	{
		ret = mist::write_jpeg( image, filename );
	}
	else if( ext == ".bmp" )
	{
		ret = mist::write_bmp( image, filename );
	}
	else if( ext == ".tiff" || ext == ".tif" )
	{
		ret = mist::write_tiff( image, filename );
	}
	else if( ext == ".png" )
	{
		ret = mist::write_png( image, filename );
	}
	else if( ext == ".pbm" )
	{
		ret = mist::write_pnm( image, filename, 1 );
	}
	else if( ext == ".pgm" )
	{
		ret = mist::write_pnm( image, filename, 2 );
	}
	else if( ext == ".ppm" || ext == ".pnm" )
	{
		ret = mist::write_pnm( image, filename, 3 );
	}
	else if( ext == ".tga" )
	{
		ret = mist::write_tga( image, filename );
	}
	else
	{
		ret = false;
	}
    My_sem_operation(semid, UNLOCK); // unlock semaphore

	return( ret );
}

/// @brief MISTコンテナの画像をTIFF形式でJPEG，TIFF，PNG，BMP，PNM，GIF 画像形式でファイルに出力する
//! 
//! @attention 拡張子に基づいて画像形式を判別するため，正しいファイル拡張子が付いている必要あり
//! @attention 出力されるファイルの圧縮率は，各 read_jpeg 等のデフォルト値が用いられる
//! 
//! @param[out] image    … 画像を読み込む先のMISTコンテナ
//! @param[in]  filename … 入力ファイル名
//!
//! @retval true  … 画像の読み込みに成功
//! @retval false … 画像の読み込みに失敗
//! 
template < class T, class Allocator >
inline bool write_image( const mist::array2< T, Allocator > &image, const std::wstring &filename )
{
	return( write_image( image, wstr2str( filename ) ) );
}

/// @}
//  画像入出力グループの終わり


// mist名前空間の終わり
_MIST_END


#endif // __INCLUDE_MIST_IMAGE__
