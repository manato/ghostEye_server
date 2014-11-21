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

// AdaBoost���O��Ԃ̎n�܂�
_ADABOOST_BEGIN

#define __FEATURE_TYPE_MASK__		0xff00
#define __FEATURE_FLAG_MASK__		0x00ff
#define __GET_FEATURE_FLAG__( t ) ( t & __FEATURE_FLAG_MASK__ )
#define __GET_FEATURE_TYPE__( t ) ( ( t & __FEATURE_TYPE_MASK__ ) >> 8 )
#define __PUT_FEATURE_TYPE__( t ) ( ( t << 8 ) & __FEATURE_TYPE_MASK__ )


#define __FEATURE_FLAG_NEW__		0x30	///< @brief �V�K�f�[�^��\���t���O
#define __FEATURE_FLAG_VALID__		0xc0	///< @brief �w�K�ɗ��p����f�[�^��\���t���O
#define __FEATURE_FLAG_POSITIVE__	0x01	///< @brief �|�W�e�B�u�T���v����\���t���O
#define __FEATURE_FLAG_DEFAULT__	( __FEATURE_FLAG_NEW__ | __FEATURE_FLAG_VALID__ | __FEATURE_FLAG_POSITIVE__ )	///< @brief �V�K�T���v���̃f�t�H���g�t���O


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

/// @brief �@�B�w�K�ŗ��p����Q�J�e�S���̓����ʂ��Ǘ�����N���X�i�����ł̂ݎg�p�j
class feature_one
{
public:
	typedef size_t size_type;			///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef ptrdiff_t difference_type;	///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���

private:
	feature_value_type  value_;			///< @brief �����ʂ̒l
	feature_flag_type   flag_;			///< @brief �w�K�f�[�^�̃J�e�S�����itrue �� false�j�Ȃǂ�ۑ�����t���O
	feature_weight_type weight_;		///< @brief �w�K�f�[�^�ɑ΂���d��

public:
	/// @brief �f�t�H���g�̃R���X�g���N�^
	feature_one( ) : value_( 0 ), flag_( __FEATURE_FLAG_DEFAULT__ ), weight_( 0 )
	{
	}

	/// @brief ���������w�肵�ē����ʂ�������
	//! 
	//! @param[in]  val �c ������
	//! @param[in]  flg �c �t���O
	//! @param[in]  wei �c �d��
	//! 
	feature_one( feature_value_type val, feature_flag_type flg, feature_weight_type wei ) : value_( val ), flag_( flg ), weight_( wei )
	{
	}

	/// @brief �R�s�[�R���X�g���N�^
	feature_one( const feature_one &f ): value_( f.value_ ), flag_( f.flag_ ), weight_( f.weight_ )
	{
	}

	/// @brief ���̓����ʂ��R�s�[����
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

	/// @brief ���̓����ʂƒl�̑召���r����
	bool operator <( const feature_one &f ) const
	{
		return( value_ < f.value_ );
	}

	/// @brief �����ʂ��擾
	feature_value_type value( ) const
	{
		return( value_ );
	}

	/// @brief �����ʂ�ݒ�
	void value( feature_value_type v )
	{
		value_ = v;
	}

	/// @brief �����ʂ̏d�݂��擾
	double weight( ) const
	{
		return( weight_ );
	}

	/// @brief �����ʂ̏d�݂�ݒ�
	void weight( feature_weight_type w )
	{
		weight_ = w;
	}

	/// @brief �T���v���̏�Ԃ�\���t���O���擾
	feature_flag_type flag( ) const
	{
		return( flag_ );
	}

	/// @brief �T���v���̏�Ԃ�\���t���O��ݒ�
	void flag( feature_flag_type f )
	{
		flag_ = f;
	}

	/// @brief �L���ȃT���v���ł��邩�ǂ������擾
	bool is_valid( ) const
	{
		return( ( flag_ & __FEATURE_FLAG_VALID__ ) != 0 );
	}

	/// @brief �L���ȃT���v���ł��邩�ǂ�����ݒ�
	void is_valid( bool value )
	{
		if( !value )
		{
			flag_ &= ~__FEATURE_FLAG_VALID__;
		}
		else
		{
			// �ēx�L���ɐݒ肳�ꂽ�ꍇ�͐V�K�f�[�^�����Ƃ���
			if( !is_valid( ) )
			{
				is_new( true );
			}

			flag_ |= __FEATURE_FLAG_VALID__;
		}
	}

	/// @brief �V�K�T���v���ł��邩�ǂ������擾
	bool is_new( ) const
	{
		return( ( flag_ & __FEATURE_FLAG_NEW__ ) != 0 );
	}

	/// @brief �V�K�T���v���ł��邩�ǂ�����ݒ�
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

	/// @brief �|�W�e�B�u�T���v���ł��邩�ǂ������擾
	bool is_positive( ) const
	{
		return( ( flag_ & __FEATURE_FLAG_POSITIVE__ ) != 0 );
	}

	/// @brief �|�W�e�B�u�T���v���ł��邩�ǂ�����ݒ�
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

	/// @brief �l�K�e�B�u�T���v���ł��邩�ǂ������擾
	bool is_negative( ) const
	{
		return( !is_positive( ) );
	}

	/// @brief �l�K�e�B�u�T���v���ł��邩�ǂ�����ݒ�
	void is_negative( bool value )
	{
		is_positive( !value );
	}

	/// @brief �V�K�T���v���ł��邩�ǂ������擾
	int type( ) const
	{
		return( __GET_FEATURE_TYPE__( flag_ ) );
	}

	/// @brief �V�K�T���v���ł��邩�ǂ�����ݒ�
	void type( int value )
	{
		flag_ = __GET_FEATURE_FLAG__( flag_ ) | __PUT_FEATURE_TYPE__( value );
	}
};

/// @brief �@�B�w�K�ŗ��p���鑽�����̓����ʂ��Ǘ�����N���X
class feature : public std::vector< feature_value_type >
{
public:
	typedef std::vector< feature_value_type > base;	///< @brief �����ʂ�ێ�������N���X
	typedef base::value_type value_type;			///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�Dmist::array< data > �� data �Ɠ���
	typedef base::size_type size_type;				///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef base::difference_type difference_type;	///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���

private:
	feature_weight_type weight_;	///< @brief �w�K�f�[�^�ɑ΂���d��
	feature_flag_type   flag_;		///< @brief �w�K�f�[�^�̃J�e�S�����itrue �� false�j�Ȃǂ�ۑ�����t���O

public:
	/// @brief �f�t�H���g�̃R���X�g���N�^
	feature( ) : base( ), weight_( 0 ), flag_( __FEATURE_FLAG_DEFAULT__ )
	{
	}

	/// @brief ���������w�肵�ē����ʂ�������
	//! 
	//! @param[in]  dimension �c �����ʂ̎���
	//! 
	feature( size_type dimension ) : base( dimension ), weight_( 0 ), flag_( __FEATURE_FLAG_DEFAULT__ )
	{
	}

	/// @brief �J�e�S�������w�肵�ē����ʂ�������
	//! 
	//! @param[in]  cate      �c �J�e�S�������ʂ̎���
	//! 
	feature( bool cate ) : base( ), weight_( 0 ), flag_( __FEATURE_FLAG_DEFAULT__ )
	{
		is_positive( cate );
	}

	/// @brief �������ƃJ�e�S�������w�肵�ē����ʂ�������
	//! 
	//! @param[in]  dimension �c �����ʂ̎���
	//! @param[in]  cate      �c �J�e�S�������ʂ̎���
	//! 
	feature( size_type dimension, bool cate ) : base( dimension, 0 ), weight_( 0 ), flag_( __FEATURE_FLAG_DEFAULT__ )
	{
		is_positive( cate );
	}

	/// @brief �R�s�[�R���X�g���N�^
	feature( const feature &f ): base( f ), weight_( f.weight_ ), flag_( f.flag_ )
	{
	}

	/// @brief ���̓����ʂ��R�s�[����
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
	/// @brief �����ʂ̏d�݂��擾
	double weight( ) const
	{
		return( weight_ );
	}

	/// @brief �����ʂ̏d�݂�ݒ�
	void weight( feature_weight_type w )
	{
		weight_ = w;
	}

	/// @brief �T���v���̏�Ԃ�\���t���O���擾
	feature_flag_type flag( ) const
	{
		return( flag_ );
	}

	/// @brief �T���v���̏�Ԃ�\���t���O��ݒ�
	void flag( feature_flag_type f )
	{
		flag_ = f;
	}

	/// @brief �L���ȃT���v���ł��邩�ǂ������擾
	bool is_valid( ) const
	{
		return( ( flag_ & __FEATURE_FLAG_VALID__ ) != 0 );
	}

	/// @brief �L���ȃT���v���ł��邩�ǂ�����ݒ�
	void is_valid( bool value )
	{
		if( !value )
		{
			flag_ &= ~__FEATURE_FLAG_VALID__;
		}
		else
		{
			// �ēx�L���ɐݒ肳�ꂽ�ꍇ�͐V�K�f�[�^�����Ƃ���
			if( !is_valid( ) )
			{
				is_new( true );
			}

			flag_ |= __FEATURE_FLAG_VALID__;
		}
	}

	/// @brief �V�K�T���v���ł��邩�ǂ������擾
	bool is_new( ) const
	{
		return( ( flag_ & __FEATURE_FLAG_NEW__ ) != 0 );
	}

	/// @brief �V�K�T���v���ł��邩�ǂ�����ݒ�
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

	/// @brief �|�W�e�B�u�T���v���ł��邩�ǂ������擾
	bool is_positive( ) const
	{
		return( ( flag_ & __FEATURE_FLAG_POSITIVE__ ) != 0 );
	}

	/// @brief �|�W�e�B�u�T���v���ł��邩�ǂ�����ݒ�
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

	/// @brief �l�K�e�B�u�T���v���ł��邩�ǂ������擾
	bool is_negative( ) const
	{
		return( !is_positive( ) );
	}

	/// @brief �l�K�e�B�u�T���v���ł��邩�ǂ�����ݒ�
	void is_negative( bool value )
	{
		is_positive( !value );
	}

	/// @brief �V�K�T���v���ł��邩�ǂ������擾
	int type( ) const
	{
		return( __GET_FEATURE_TYPE__( flag_ ) );
	}

	/// @brief �V�K�T���v���ł��邩�ǂ�����ݒ�
	void type( int value )
	{
		flag_ = __GET_FEATURE_FLAG__( flag_ ) | __PUT_FEATURE_TYPE__( value );
	}

	/// @brief �����ʂ̎������擾
	size_type number_of_dimentions( ) const
	{
		return( base::size( ) );
	}
};

/// @brief �@�B�w�K�ŗ��p��������ʂ��Q�ƌo�R�ŃA�N�Z�X���邽�߂̃N���X
class feature_ref
{
public:
	typedef feature feature_type;
	typedef feature_type::value_type value_type;			///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�Dmist::array< data > �� data �Ɠ���
	typedef feature_type::size_type size_type;				///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef feature_type::difference_type difference_type;	///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���

private:
	feature_type *f_;	///< @brief �����ʂ̎Q�ƌ�

public:
	/// @brief �f�t�H���g�̃R���X�g���N�^
	feature_ref( ) : f_( NULL )
	{
	}

	/// @brief �f�t�H���g�̃R���X�g���N�^
	feature_ref( feature_type &f ) : f_( &f )
	{
	}

public:
	/// @brief �w�肵���C���f�b�N�X�ɑΉ���������ʂ̒l���擾
	//! 
	//! @param[in]  index �c �����ʂ̃C���f�b�N�X
	//! 
	const value_type &operator []( size_type index ) const
	{
		return( ( *f_ )[ index ] );
	}

	/// @brief �w�肵���C���f�b�N�X�ɑΉ���������ʂ̒l���擾
	//! 
	//! @param[in]  index �c �����ʂ̃C���f�b�N�X
	//! 
	value_type &operator []( size_type index )
	{
		return( ( *f_ )[ index ] );
	}

public:
	/// @brief �����ʂ̏d�݂��擾
	double weight( ) const
	{
		return( f_->weight( ) );
	}

	/// @brief �����ʂ̏d�݂�ݒ�
	void weight( feature_weight_type w )
	{
		f_->weight( w );
	}

	/// @brief �T���v���̏�Ԃ�\���t���O���擾
	feature_flag_type flag( ) const
	{
		return( f_->flag( ) );
	}

	/// @brief �T���v���̏�Ԃ�\���t���O��ݒ�
	void flag( feature_flag_type f )
	{
		f_->flag( f );
	}

	/// @brief �L���ȃT���v���ł��邩�ǂ������擾
	bool is_valid( ) const
	{
		return( f_->is_valid( ) );
	}

	/// @brief �L���ȃT���v���ł��邩�ǂ�����ݒ�
	void is_valid( bool value )
	{
		f_->is_valid( value );
	}

	/// @brief �V�K�T���v���ł��邩�ǂ������擾
	bool is_new( ) const
	{
		return( f_->is_new( ) );
	}

	/// @brief �V�K�T���v���ł��邩�ǂ�����ݒ�
	void is_new( bool value )
	{
		f_->is_new( value );
	}

	/// @brief �|�W�e�B�u�T���v���ł��邩�ǂ������擾
	bool is_positive( ) const
	{
		return( f_->is_positive( ) );
	}

	/// @brief �|�W�e�B�u�T���v���ł��邩�ǂ�����ݒ�
	void is_positive( bool value )
	{
		f_->is_positive( value );
	}

	/// @brief �l�K�e�B�u�T���v���ł��邩�ǂ������擾
	bool is_negative( ) const
	{
		return( f_->is_negative( ) );
	}

	/// @brief �l�K�e�B�u�T���v���ł��邩�ǂ�����ݒ�
	void is_negative( bool value )
	{
		f_->is_negative( value );
	}

	/// @brief �V�K�T���v���ł��邩�ǂ������擾
	int type( ) const
	{
		return( f_->type( ) );
	}

	/// @brief �V�K�T���v���ł��邩�ǂ�����ݒ�
	void type( int value )
	{
		f_->type( value );
	}

	/// @brief �����ʂ̎������擾
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
	typedef size_t size_type;			///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef ptrdiff_t difference_type;	///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���

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
	typedef size_t size_type;						///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef ptrdiff_t difference_type;				///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���

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

	// �����팸�������ꍇ�͎㎯�ʊ�̃C���f�b�N�X��U��Ȃ���
#if defined( __ADABOOST_CASCADE_TYPE__ ) && __ADABOOST_CASCADE_TYPE__ != 0

#if defined( __ADABOOST_CASCADE_TYPE__ ) && __ADABOOST_CASCADE_TYPE__ == 3
	// �㎯�ʊ�ɓ����ʂ̏���ݒ肷��
	for( size_t i = 0 ; i < classifier.classifiers( ).size( ) ; i++ )
	{
		used_colors[ __GET_ADABOOST_FEATURE_COLOR__( flist[ classifier.classifiers( )[ i ].index( ) ].mode ) ] = true;
	}
#else
	// �㎯�ʊ�ɓ����ʂ̏���ݒ肷��
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

// AdaBoost���O��Ԃ̏I���
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