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


// AdaBoost���O��Ԃ̎n�܂�
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

	/// @brief ���̎㎯�ʊ�̃f�[�^���R�s�[����
	empty_classifier& operator =( const empty_classifier& o )
	{
		if( this != &o )
		{
			is_positive_ = o.is_positive_;
		}

		return( *this );
	}

public:
	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f �c ���ނ��������
	//! 
	//! @return ���ތ���
	//! 
	template < class FEATURE >
	bool operator ()( const FEATURE &f ) const
	{
		return( evaluate( f ) >= 0 );
	}

	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p �c �����ʌv�Z�Ɏg�p����p�����[�^
	//! 
	//! @return ���ތ���
	//! 
	template < class FUNCTOR, class PARAMS >
	bool operator ()( const FUNCTOR &f, const PARAMS &p ) const
	{
		return( evaluate( p, f ) >= 0 );
	}

	/// @brief �w�K�ς݂̎㎯�ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2 >
	bool operator ()( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2 ) const
	{
		return( evaluate( f, p1, p2 ) >= 0 );
	}

	/// @brief �w�K�ς݂̎㎯�ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  p3 �c �����ʌv�Z�Ɏg�p����p�����[�^3
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3 >
	bool operator ()( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3 ) const
	{
		return( evaluate( f, p1, p2, p3 ) >= 0 );
	}

	/// @brief �w�K�ς݂̎㎯�ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  p3 �c �����ʌv�Z�Ɏg�p����p�����[�^3
	//! @param[in]  p4 �c �����ʌv�Z�Ɏg�p����p�����[�^4
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4 >
	bool operator ()( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, const PARAM4 &p4 ) const
	{
		return( evaluate( f, p1, p2, p3, p4 ) >= 0 );
	}

	/// @brief �w�K�ς݂̎㎯�ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  p3 �c �����ʌv�Z�Ɏg�p����p�����[�^3
	//! @param[in]  p4 �c �����ʌv�Z�Ɏg�p����p�����[�^4
	//! @param[in]  p5 �c �����ʌv�Z�Ɏg�p����p�����[�^5
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5 >
	bool operator ()( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, const PARAM4 &p4, const PARAM5 &p5 ) const
	{
		return( evaluate( f, p1, p2, p3, p4, p5 ) >= 0 );
	}

	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f �c ���ނ��������
	//! 
	//! @return ���ތ���
	//! 
	template < class FEATURE >
	double evaluate( const FEATURE &f ) const
	{
		return( is_positive_ ? 1.0 : -1.0 );
	}

	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p �c �����ʌv�Z�Ɏg�p����p�����[�^
	//! 
	//! @return ���ތ���
	//! 
	template < class FUNCTOR, class PARAMS >
	double evaluate( const FUNCTOR &f, const PARAMS &p ) const
	{
		return( is_positive_ ? 1.0 : -1.0 );
	}

	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! 
	//! @return ���ތ���
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2 >
	double evaluate( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2 ) const
	{
		return( is_positive_ ? 1.0 : -1.0 );
	}

	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  p3 �c �����ʌv�Z�Ɏg�p����p�����[�^3
	//! 
	//! @return ���ތ���
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3 >
	double evaluate( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3 ) const
	{
		return( is_positive_ ? 1.0 : -1.0 );
	}

	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  p3 �c �����ʌv�Z�Ɏg�p����p�����[�^3
	//! @param[in]  p4 �c �����ʌv�Z�Ɏg�p����p�����[�^4
	//! 
	//! @return ���ތ���
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4 >
	double evaluate( const FUNCTOR & /* f */, const PARAM1 & /* p1 */, const PARAM2 & /* p2 */, const PARAM3 & /* p3 */, const PARAM4 & /* p4 */ ) const
	{
		return( is_positive_ ? 1.0 : -1.0 );
	}

	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  p3 �c �����ʌv�Z�Ɏg�p����p�����[�^3
	//! @param[in]  p4 �c �����ʌv�Z�Ɏg�p����p�����[�^4
	//! @param[in]  p5 �c �����ʌv�Z�Ɏg�p����p�����[�^5
	//! 
	//! @return ���ތ���
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5 >
	double evaluate( const FUNCTOR & /* f */, const PARAM1 & /* p1 */, const PARAM2 & /* p2 */, const PARAM3 & /* p3 */, const PARAM4 & /* p4 */, const PARAM5 & /* p4 */ ) const
	{
		return( is_positive_ ? 1.0 : -1.0 );
	}
};


/// @brief AdaBoost �ŗ��p���邵�����l������p�����㎯�ʊ�
class classifier_base
{
public:
	typedef feature::value_type      value_type;		///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�Dmist::array< data > �� data �Ɠ���
	typedef feature::size_type       size_type;			///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef feature::difference_type difference_type;	///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���

private:
	std::string comment_;
	double positive_weight_;
	double negative_weight_;

public:
	/// @brief �f�t�H���g�̃R���X�g���N�^
	classifier_base( ) : comment_( ), positive_weight_( 0.5 ), negative_weight_( 0.5 )
	{
	}

	/// @brief �R�s�[�R���X�g���N�^
	classifier_base( const classifier_base & w ) : comment_( w.comment_ ), positive_weight_( w.positive_weight_ ), negative_weight_( w.negative_weight_ )
	{
	}

	/// @brief ���̎㎯�ʊ�̃f�[�^���R�s�[����
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
	/// @brief ���ʊ�ɑ΂��Đݒ肳�ꂽ�R�����g���擾����
	std::string comment( ) const { return( comment_ ); }

	/// @brief ���ʊ�ɑ΂��ăR�����g��ݒ肷��
	//! 
	//! @param[in] c �c �R�����g
	//! 
	void comment( const std::string &c ){ comment_ = c; }

	/// @brief ���ʊ�̍\�z�Ɏg�p����|�W�e�B�u�T���v���̏d�݂��擾����
	double positive_weight( ) const { return( positive_weight_ ); }

	/// @brief ���ʊ�̍\�z�Ɏg�p����|�W�e�B�u�T���v���̏d�݂�ݒ肷��
	//! 
	//! @param[in] w �c �d��
	//! 
	void positive_weight( const double w ){ positive_weight_ = w; }

	/// @brief ���ʊ�̍\�z�Ɏg�p����l�K�e�B�u�T���v���̏d�݂��擾����
	double negative_weight( ) const { return( negative_weight_ ); }

	/// @brief ���ʊ�̍\�z�Ɏg�p����l�K�e�B�u�T���v���̏d�݂�ݒ肷��
	//! 
	//! @param[in] w �c �d��
	//! 
	void negative_weight( const double w ){ negative_weight_ = w; }

	/// @brief ���ʊ�̏���ۑ�����XML�t�@�C������ݒ�𕜌�����
	//! 
	//! @param[in] filename �c ���ʊ�̏���ۑ�����XML�t�@�C����
	//! 
	virtual bool load( const std::string & /* filename */ ){ return( false ); }

	/// @brief ���ʊ�̏���XML�`���Ńt�@�C���ɕۑ�����
	//! 
	//! @param[in] filename �c ���ʊ�̏���ۑ�����XML�t�@�C����
	//! 
	virtual bool save( const std::string & /* filename */, const std::string & /* comment */ = "" ) const { return( false ); }
};


/// @brief AdaBoost �ŗ��p���邵�����l������p�����㎯�ʊ�
template < class DERIVED_CLASSIFIER >
class weak_classifier_base : public classifier_base
{
private:
	typedef classifier_base				base;				///< @brief ���N���X
	typedef DERIVED_CLASSIFIER			derived_type;		///< @brief �h���N���X

public:
	typedef feature::value_type			value_type;			///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�Dmist::array< data > �� data �Ɠ���
	typedef feature::size_type			size_type;			///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef feature::difference_type	difference_type;	///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���

	size_type index_;										///< @brief �g�p��������ʂ̔ԍ�
	double weight_;											///< @brief �㎯�ʊ�̏d��
	double lambda_c_;										///< @brief �I�����C���w�K�Ŏg�p����d�݁i���������ނł������́j
	double lambda_w_;										///< @brief �I�����C���w�K�Ŏg�p����d�݁i����ĕ��ނ��ꂽ���́j

public:
	/// @brief �f�t�H���g�̃R���X�g���N�^
	weak_classifier_base( size_type indx = 0 ) : index_( indx ), weight_( 0.0 ), lambda_c_( 0.0 ), lambda_w_( 0.0 )
	{
	}

	/// @brief �R�s�[�R���X�g���N�^
	weak_classifier_base( const weak_classifier_base &w ) : base( w ), index_( w.index_ ), weight_( w.weight_ ), lambda_c_( w.lambda_c_ ), lambda_w_( w.lambda_w_ )
	{
	}

	/// @brief �㎯�ʊ킪�g�p��������ʂ̃C���f�b�N�X���擾����
	size_type index( ) const { return( index_ ); }

	/// @brief �㎯�ʊ킪�g�p��������ʂ̃C���f�b�N�X��ݒ肷��
	void index( size_type i ){ index_ = i; }

	/// @brief �㎯�ʊ�̏d�݂��擾
	double weight( ) const { return( weight_ ); }

	/// @brief �㎯�ʊ�̏d�݂�ݒ�
	void weight( double w ){ weight_ = w; }


	/// @brief �I�����C���w�K�Ŏg�p����d�݁i���������ނł������́j��ݒ肷��֐�
	//! 
	//! @param[in]  value �c �m�����x�֐��֐ݒ肷��l
	//! 
	void lambda_c( double value )
	{
		lambda_c_ = value;
	}

	/// @brief �I�����C���w�K�Ŏg�p����d�݁i���������ނł������́j���擾����֐�
	//! 
	double lambda_c( ) const
	{
		return( lambda_c_ );
	}

	/// @brief �I�����C���w�K�Ŏg�p����d�݁i����ĕ��ނ��ꂽ���́j��ݒ肷��֐�
	//! 
	//! @param[in]  value �c �m�����x�֐��֐ݒ肷��l
	//! 
	void lambda_w( double value )
	{
		lambda_w_ = value;
	}

	/// @brief �I�����C���w�K�Ŏg�p����d�݁i����ĕ��ނ��ꂽ���́j���擾����֐�
	//! 
	double lambda_w( ) const
	{
		return( lambda_w_ );
	}

	/// @brief ���̎㎯�ʊ�̃f�[�^���R�s�[����
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
	/// @brief �w�K�ς݂̎㎯�ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f �c ���ނ�������ʃx�N�g��
	//! 
	template < class FEATURE >
	double operator ()( const FEATURE &f ) const
	{
		return( evaluate( f ) );
	}

	/// @brief �w�K�ς݂̎㎯�ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f �c ���ނ��������
	//! 
	double operator ()( double f ) const
	{
		return( evaluate( f ) );
	}

	/// @brief �w�K�ς݂̎㎯�ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p �c �����ʌv�Z�Ɏg�p����p�����[�^
	//! 
	template < class FUNCTOR, class PARAMS >
	double operator ()( const FUNCTOR &f, const PARAMS &p ) const
	{
		return( evaluate( f, p ) );
	}

	/// @brief �w�K�ς݂̎㎯�ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2 >
	double operator ()( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2 ) const
	{
		return( evaluate( f, p1, p2 ) );
	}

	/// @brief �w�K�ς݂̎㎯�ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  p3 �c �����ʌv�Z�Ɏg�p����p�����[�^3
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3 >
	double operator ()( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3 ) const
	{
		return( evaluate( f, p1, p2, p3 ) );
	}

	/// @brief �w�K�ς݂̎㎯�ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  p3 �c �����ʌv�Z�Ɏg�p����p�����[�^3
	//! @param[in]  p4 �c �����ʌv�Z�Ɏg�p����p�����[�^4
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4 >
	double operator ()( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, const PARAM4 &p4 ) const
	{
		return( evaluate( f, p1, p2, p3, p4 ) );
	}

	/// @brief �w�K�ς݂̎㎯�ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  p3 �c �����ʌv�Z�Ɏg�p����p�����[�^3
	//! @param[in]  p4 �c �����ʌv�Z�Ɏg�p����p�����[�^4
	//! @param[in]  p5 �c �����ʌv�Z�Ɏg�p����p�����[�^5
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5 >
	double operator ()( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, const PARAM4 &p4, const PARAM5 &p5 ) const
	{
		return( evaluate( f, p1, p2, p3, p4, p5 ) );
	}

	/// @brief �w�K�ς݂̎㎯�ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f �c ���ނ�������ʃx�N�g��
	//! 
	template < class FEATURE >
	double evaluate( const FEATURE &f ) const
	{
		return( ( ( const derived_type * ) this )->evaluate_( f[ index_ ] ) );
	}

	/// @brief �w�K�ς݂̎㎯�ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f �c ���ނ��������
	//! 
	double evaluate( double f ) const
	{
		return( ( ( const derived_type * ) this )->evaluate_( f ) );
	}

	/// @brief �w�K�ς݂̎㎯�ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p �c �����ʌv�Z�Ɏg�p����p�����[�^
	//! 
	template < class FUNCTOR, class PARAMS >
	double evaluate( const FUNCTOR &f, const PARAMS &p ) const
	{
		return( ( ( const derived_type * ) this )->evaluate_( f( index_, p ) ) );
	}

	/// @brief �w�K�ς݂̎㎯�ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2 >
	double evaluate( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2 ) const
	{
		return( ( ( const derived_type * ) this )->evaluate_( f( index_, p1, p2 ) ) );
	}

	/// @brief �w�K�ς݂̎㎯�ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  p3 �c �����ʌv�Z�Ɏg�p����p�����[�^3
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3 >
	double evaluate( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3 ) const
	{
		return( ( ( const derived_type * ) this )->evaluate_( f( index_, p1, p2, p3 ) ) );
	}

	/// @brief �w�K�ς݂̎㎯�ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  p3 �c �����ʌv�Z�Ɏg�p����p�����[�^3
	//! @param[in]  p4 �c �����ʌv�Z�Ɏg�p����p�����[�^4
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4 >
	double evaluate( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, const PARAM4 &p4 ) const
	{
		return( ( ( const derived_type * ) this )->evaluate_( f( index_, p1, p2, p3, p4 ) ) );
	}

	/// @brief �w�K�ς݂̎㎯�ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  p3 �c �����ʌv�Z�Ɏg�p����p�����[�^3
	//! @param[in]  p4 �c �����ʌv�Z�Ɏg�p����p�����[�^4
	//! @param[in]  p5 �c �����ʌv�Z�Ɏg�p����p�����[�^5
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5 >
	double evaluate( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, const PARAM4 &p4, const PARAM5 &p5 ) const
	{
		return( ( ( const derived_type * ) this )->evaluate_( f( index_, p1, p2, p3, p4, p5 ) ) );
	}

	/// @brief �w�K�ς݂̎㎯�ʊ��p���ē����ʂ𕪗ނ���
	double evaluate_( double f ) const
	{
		return( f >= 0 ? weight( ) : -weight( ) );
	}

	/// @brief �ǉ��̋��t�f�[�^��p���Ď㎯�ʊ���X�V����
	//! 
	//! @param[in]  f �c ���ނ�������ʃx�N�g��
	//! @param[in]  weight �c �w�K����d��
	//! 
	template < class FEATURE >
	void update( const FEATURE &f, const double weight, bool is_positive )
	{
		return( ( ( derived_type * ) this )->update_( f[ index_ ], weight, is_positive ) );
	}

	/// @brief �ǉ��̋��t�f�[�^��p���Ď㎯�ʊ���X�V����
	//! 
	//! @param[in]  f �c ���ނ��������
	//! @param[in]  weight �c �w�K����d��
	//! 
	void update( double f, const double weight, bool is_positive )
	{
		return( ( ( derived_type * ) this )->update_( f, weight, is_positive ) );
	}

	/// @brief �ǉ��̋��t�f�[�^��p���Ď㎯�ʊ���X�V����
	//! 
	//! @param[in]  f �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p �c �����ʌv�Z�Ɏg�p����p�����[�^
	//! @param[in]  weight �c �w�K����d��
	//! 
	template < class FUNCTOR, class PARAMS >
	void update( const FUNCTOR &f, const PARAMS &p, const double weight, bool is_positive )
	{
		return( ( ( derived_type * ) this )->update_( f( index_, p ), weight, is_positive ) );
	}

	/// @brief �ǉ��̋��t�f�[�^��p���Ď㎯�ʊ���X�V����
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  weight �c �w�K����d��
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2 >
	void update( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const double weight, bool is_positive )
	{
		return( ( ( derived_type * ) this )->update_( f( index_, p1, p2 ), weight, is_positive ) );
	}

	/// @brief �ǉ��̋��t�f�[�^��p���Ď㎯�ʊ���X�V����
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  p3 �c �����ʌv�Z�Ɏg�p����p�����[�^3
	//! @param[in]  weight �c �w�K����d��
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3 >
	void update( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, const double weight, bool is_positive )
	{
		return( ( ( derived_type * ) this )->update_( f( index_, p1, p2, p3 ), weight, is_positive ) );
	}

	/// @brief �ǉ��̋��t�f�[�^��p���Ď㎯�ʊ���X�V����
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  p3 �c �����ʌv�Z�Ɏg�p����p�����[�^3
	//! @param[in]  p4 �c �����ʌv�Z�Ɏg�p����p�����[�^4
	//! @param[in]  weight �c �w�K����d��
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4 >
	void update( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, const PARAM4 &p4, const double weight, bool is_positive )
	{
		return( ( ( derived_type * ) this )->update_( f( index_, p1, p2, p3, p4 ), weight, is_positive ) );
	}

	/// @brief �ǉ��̋��t�f�[�^��p���Ď㎯�ʊ���X�V����
	//! 
	//! @param[in]  f      �c �ǉ��̋��t�f�[�^
	//! @param[in]  weight �c �w�K����d��
	//! 
	void update_( double f, const double weight, bool is_positive )
	{
	}
};


/// @brief Boosting�ɂ��w�K���s�������ʊ�̊��N���X
template < class DERIVED_CLASSIFIER, class BASE_WEAK_CLASSIFIER >
class boosted_classifier_base : public classifier_base
{
private:
	typedef classifier_base						base;					///< @brief ���N���X
	typedef DERIVED_CLASSIFIER                  derived_type;			///< @brief �h���N���X

public:	
	typedef feature::value_type                 value_type;				///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�Dmist::array< data > �� data �Ɠ���
	typedef feature::size_type                  size_type;				///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef feature::difference_type            difference_type;		///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���
	typedef BASE_WEAK_CLASSIFIER                base_classifier_type;	///< @brief AdaBoost�ŗ��p����㎯�ʊ�
	typedef std::vector< base_classifier_type > classifier_list_type;	///< @brief AdaBoost�ŗ��p����㎯�ʊ�̃��X�g

protected:
	classifier_list_type	classifiers_;	///< @brief �㎯�ʊ�̃��X�g
	double					threshold_;		///< @brief ���ʂɎg�p���邵�����l
	std::string				info_;			///< @brief ���ʊ���w�K�����ۂ̏���ێ����镶����
	std::string				tag_;			///< @brief ���[�U�[���Ǝ��ɐݒ�\�ȕ�����

public:
	/// @brief �f�t�H���g�̃R���X�g���N�^
	boosted_classifier_base( ) : threshold_( 0.0 ), info_( "" ){}

	/// @brief �R�s�[�R���X�g���N�^
	boosted_classifier_base( const boosted_classifier_base &b ) : base( b ), classifiers_( b.classifiers_ ), threshold_( b.threshold_ ), info_( b.info_ ), tag_( b.tag_ ){}

	/// @brief �f�X�g���N�^
	~boosted_classifier_base( ){}

	/// @brief ����AdaBoost���ʊ�̏����R�s�[����
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
	/// @brief �㎯�ʊ�̎��ʋ��E�����肷��ۂɎg�p���邵�����l���擾����
	double threshold( ) const { return( threshold_ ); }
	/// @brief �㎯�ʊ�̎��ʋ��E�����肷��ۂɎg�p���邵�����l��ݒ肷��
	void threshold( double threshold ){ threshold_ = threshold; }

	/// @brief ���ʊ�ɑ΂��ă��[�U�[���ݒ�\�ȏ���\����������擾����
	std::string tag( ) const { return( tag_ ); }
	/// @brief ���ʊ�ɑ΂��ă��[�U�[���ݒ�\�ȏ���\���������ݒ肷��
	void tag( const std::string &str ){ tag_ = str; }

	/// @brief ���ʊ���w�K�����ۂ̏���ێ����镶������擾����
	std::string info( ) const { return( info_ ); }
	/// @brief ���ʊ���w�K�����ۂ̏���ێ����镶�����ݒ肷��
	void info( const std::string &str ){ info_ = str; }

	/// @brief AdaBoost���ʊ���\�z����㎯�ʊ�̐����擾����
	size_type number_of_classifiers( ) const { return( classifiers_.size( ) ); }

	/// @brief AdaBoost���ʊ���\�z����㎯�ʊ�̐����擾����
	size_type total_number_of_classifiers( ) const { return( classifiers_.size( ) ); }

	/// @brief ���ʊ���\�z����㎯�ʊ�̃��X�g���擾����
	classifier_list_type & classifiers( ){ return( classifiers_ ); }

	/// @brief ���ʊ���\�z����㎯�ʊ�̃��X�g���擾����
	const classifier_list_type & classifiers( ) const { return( classifiers_ ); }

	/// @brief �V�����㎯�ʊ��ǉ�����
	bool add_classifier( const base_classifier_type &weak )
	{
		classifiers_.push_back( weak );

		return( true );
	}

public:
	/// @brief ���t�f�[�^��p����AdaBoost�A���S���Y���ɂ�苭���ʊ���\�z����
	//! 
	//! @param[in,out]  features                   �c �w�K�ɗp���鋳�t�f�[�^
	//! @param[in]      num_max_base_classifiers   �c �����ʊ�̍\�z�Ɏg�p����㎯�ʊ�̍ő吔�i�f�t�H���g��100�j
	//! @param[in]      min_true_positive_rate     �c �����ʊ�̍\�z���ɖ������ׂ�TP�̍ŏ��l�i�f�t�H���g��-1�j�D���̒l���w�肵���ꍇ�� num_max_weak_classifiers �܂Ŏ㎯�ʊ��ǉ�����D
	//! @param[in]      max_false_positive_rate    �c �����ʊ�̍\�z���ɖ������ׂ�FP�̍ő�l�i�f�t�H���g��-1�j�D���̒l���w�肵���ꍇ�� num_max_weak_classifiers �܂Ŏ㎯�ʊ��ǉ�����D
	//! @param[in]      _output_debug_string_      �c �w�K�̐i���󋵂��o�͂��邩�ǂ����i�f�t�H���g��false�j
	//! 
	template < template < typename, typename > class FEATURE_LIST, class FEATURE, class ALLOCATOR >
	bool learn( FEATURE_LIST< FEATURE, ALLOCATOR > & features, size_type num_max_base_classifiers = 100, double min_true_positive_rate = -1.0, double max_false_positive_rate = -1.0, bool _output_debug_string_ = false )
	{
		if( !initialize( features ) )
		{
			return( false );
		}

		// ���ʊ���\�z����
		add_classifiers( features, num_max_base_classifiers, min_true_positive_rate, max_false_positive_rate, _output_debug_string_ );

		return( true );
	}

	/// @brief AdaBoost���ʊ�̐ݒ������������
	bool initialize( )
	{
		// �����ʊ������������
		classifiers_.clear( );

		// �������l������������
		threshold_ = 0.0;

		return( true );
	}

	/// @brief ���ʊ�̐ݒ�Ƌ��t�f�[�^������������
	//! 
	//! @param[in,out]  features �c �w�K�ɗp���鋳�t�f�[�^
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

		// �����ʊ������������
		initialize( );

		// �d�݂�����������
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

	/// @brief ���݂̋����ʊ�Ɏ㎯�ʊ��ǉ�����
	//! 
	//! @param[in,out]  features                   �c �w�K�ɗp���鋳�t�f�[�^
	//! @param[in]      num_max_base_classifiers   �c �����ʊ�֒ǉ�����㎯�ʊ�̐��i�f�t�H���g��100�j
	//! @param[in]      min_true_positive_rate     �c �����ʊ�̍\�z���ɖ������ׂ�TP�̍ŏ��l�i�f�t�H���g��-1�j�D���̒l���w�肵���ꍇ�� num_max_base_classifiers �܂Ŏ㎯�ʊ��ǉ�����D
	//! @param[in]      max_false_positive_rate    �c �����ʊ�̍\�z���ɖ������ׂ�FP�̍ő�l�i�f�t�H���g��-1�j�D���̒l���w�肵���ꍇ�� num_max_base_classifiers �܂Ŏ㎯�ʊ��ǉ�����D
	//! @param[in]      _output_debug_string_      �c �w�K�̐i���󋵂��o�͂��邩�ǂ����i�f�t�H���g��false�j
	//! 
	template < template < typename, typename > class FEATURE_LIST, class FEATURE, class ALLOCATOR >
	bool add_classifiers( FEATURE_LIST< FEATURE, ALLOCATOR > & features, size_type num_max_base_classifiers = 100, double min_true_positive_rate = -1.0, double max_false_positive_rate = -1.0, bool _output_debug_string_ = false )
	{
		return( add_classifiers( features, empty_info_functor( ), num_max_base_classifiers, min_true_positive_rate, max_false_positive_rate, _output_debug_string_ ) );
	}

	/// @brief ���݂̋����ʊ�Ɏ㎯�ʊ��ǉ�����
	//! 
	//! @param[in,out]  features                   �c �w�K�ɗp���鋳�t�f�[�^
	//! @param[in]      num_max_base_classifiers   �c �����ʊ�֒ǉ�����㎯�ʊ�̐��i�f�t�H���g��100�j
	//! @param[in]      min_true_positive_rate     �c �����ʊ�̍\�z���ɖ������ׂ�TP�̍ŏ��l�i�f�t�H���g��-1�j�D���̒l���w�肵���ꍇ�� num_max_base_classifiers �܂Ŏ㎯�ʊ��ǉ�����D
	//! @param[in]      max_false_positive_rate    �c �����ʊ�̍\�z���ɖ������ׂ�FP�̍ő�l�i�f�t�H���g��-1�j�D���̒l���w�肵���ꍇ�� num_max_base_classifiers �܂Ŏ㎯�ʊ��ǉ�����D
	//! @param[in]      _output_debug_string_      �c �w�K�̐i���󋵂��o�͂��邩�ǂ����i�f�t�H���g��false�j
	//! 
	template < template < typename, typename > class FEATURE_LIST, class FEATURE, class ALLOCATOR, class INFOFUNCTOR >
	bool add_classifiers( FEATURE_LIST< FEATURE, ALLOCATOR > & features, INFOFUNCTOR finfo, size_type num_max_base_classifiers, double min_true_positive_rate, double max_false_positive_rate, bool _output_debug_string_ )
	{
		// �h���N���X���ŏ���������
		return( ( ( derived_type * ) this )->add_classifiers( features, finfo, num_max_base_classifiers, min_true_positive_rate, max_false_positive_rate, _output_debug_string_ ) );
	}

public:
	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f �c ���ނ��������
	//! 
	//! @return ���ތ���
	//! 
	template < class FEATURE >
	bool operator ()( const FEATURE &f ) const
	{
		return( ( ( const derived_type * ) this )->evaluate( f ) >= 0.0 );
	}

	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p �c �����ʌv�Z�Ɏg�p����p�����[�^
	//! 
	//! @return ���ތ���
	//! 
	template < class FUNCTOR, class PARAMS >
	bool operator ()( const FUNCTOR &f, const PARAMS &p ) const
	{
		return( ( ( const derived_type * ) this )->evaluate( f, p ) >= 0.0 );
	}

	/// @brief �w�K�ς݂̎㎯�ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2 >
	bool operator ()( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2 ) const
	{
		return( ( ( const derived_type * ) this )->evaluate( f, p1, p2 ) >= 0.0 );
	}

	/// @brief �w�K�ς݂̎㎯�ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  p3 �c �����ʌv�Z�Ɏg�p����p�����[�^3
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3 >
	bool operator ()( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3 ) const
	{
		return( ( ( const derived_type * ) this )->evaluate( f, p1, p2, p3 ) >= 0.0 );
	}

	/// @brief �w�K�ς݂̎㎯�ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  p3 �c �����ʌv�Z�Ɏg�p����p�����[�^3
	//! @param[in]  p4 �c �����ʌv�Z�Ɏg�p����p�����[�^4
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4 >
	bool operator ()( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, const PARAM4 &p4 ) const
	{
		return( ( ( const derived_type * ) this )->evaluate( f, p1, p2, p3, p4 ) >= 0.0 );
	}

	/// @brief �w�K�ς݂̎㎯�ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  p3 �c �����ʌv�Z�Ɏg�p����p�����[�^3
	//! @param[in]  p4 �c �����ʌv�Z�Ɏg�p����p�����[�^4
	//! @param[in]  p5 �c �����ʌv�Z�Ɏg�p����p�����[�^5
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5 >
	bool operator ()( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, const PARAM4 &p4, const PARAM5 &p5 ) const
	{
		return( ( ( const derived_type * ) this )->evaluate( f, p1, p2, p3, p4, p5 ) >= 0.0 );
	}

	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f �c ���ނ��������
	//! 
	//! @return ���ތ���
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

	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p �c �����ʌv�Z�Ɏg�p����p�����[�^
	//! 
	//! @return ���ތ���
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

	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! 
	//! @return ���ތ���
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

	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  p3 �c �����ʌv�Z�Ɏg�p����p�����[�^3
	//! 
	//! @return ���ތ���
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

	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  p3 �c �����ʌv�Z�Ɏg�p����p�����[�^3
	//! @param[in]  p4 �c �����ʌv�Z�Ɏg�p����p�����[�^4
	//! 
	//! @return ���ތ���
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

	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  p3 �c �����ʌv�Z�Ɏg�p����p�����[�^3
	//! @param[in]  p4 �c �����ʌv�Z�Ɏg�p����p�����[�^4
	//! @param[in]  p5 �c �����ʌv�Z�Ɏg�p����p�����[�^5
	//! 
	//! @return ���ތ���
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
	/// @brief �ǉ��̋��t�f�[�^��p���Ď㎯�ʊ���X�V����
	//! 
	//! @param[in]  f           �c ���ނ�������ʃx�N�g��
	//! @param[in]  is_positive �c �|�W�e�B�u�T���v�����ǂ���
	//! @param[in]  lambda      �c �X�V�ɗp����d�݂̏����l
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

	/// @brief �ǉ��̋��t�f�[�^��p���Ď㎯�ʊ���X�V����
	//! 
	//! @param[in]  f �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p �c �����ʌv�Z�Ɏg�p����p�����[�^
	//! @param[in]  is_positive �c �|�W�e�B�u�T���v�����ǂ���
	//! @param[in]  lambda      �c �X�V�ɗp����d�݂̏����l
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

	/// @brief �ǉ��̋��t�f�[�^��p���Ď㎯�ʊ���X�V����
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  is_positive �c �|�W�e�B�u�T���v�����ǂ���
	//! @param[in]  lambda      �c �X�V�ɗp����d�݂̏����l
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

	/// @brief �ǉ��̋��t�f�[�^��p���Ď㎯�ʊ���X�V����
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  p3 �c �����ʌv�Z�Ɏg�p����p�����[�^3
	//! @param[in]  is_positive �c �|�W�e�B�u�T���v�����ǂ���
	//! @param[in]  lambda      �c �X�V�ɗp����d�݂̏����l
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

	/// @brief �ǉ��̋��t�f�[�^��p���Ď㎯�ʊ���X�V����
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  p3 �c �����ʌv�Z�Ɏg�p����p�����[�^3
	//! @param[in]  p4 �c �����ʌv�Z�Ɏg�p����p�����[�^4
	//! @param[in]  is_positive �c �|�W�e�B�u�T���v�����ǂ���
	//! @param[in]  lambda      �c �X�V�ɗp����d�݂̏����l
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
	/// @brief �����ʂ̃��X�g����TP�����v�Z����
	//! 
	//! @param[in]  features             �c TP���̌v�Z�Ɏg�p��������ʂ̃��X�g
	//! @param[in]  force_calculate_rate �c �����ɐݒ肳��Ă�������ʂ��v�Z�Ɏg�p���邩�ǂ����i�f�t�H���g��false�j
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

	/// @brief �����ʂ̃��X�g����FP�����v�Z����
	//! 
	//! @param[in]  features             �c FP���̌v�Z�Ɏg�p��������ʂ̃��X�g
	//! @param[in]  force_calculate_rate �c �����ɐݒ肳��Ă�������ʂ��v�Z�Ɏg�p���邩�ǂ����i�f�t�H���g��false�j
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


/// @brief �����ʊ���J�X�P�[�h�ڑ����������ʊ�̊��N���X
template < class DERIVED_CLASSIFIER, class BASE_STRONG_CLASSIFIER >
class cascaded_classifier_base : public classifier_base
{
private:
	typedef classifier_base						base;					///< @brief ���N���X
	typedef DERIVED_CLASSIFIER                  derived_type;			///< @brief �h���N���X

public:
	typedef BASE_STRONG_CLASSIFIER              base_classifier_type;	///< @brief �J�X�P�[�h�ɂ��鎯�ʊ�
	typedef std::vector< base_classifier_type > classifier_list_type;	///< @brief �J�X�P�[�h�ɂ��鎯�ʊ�̃��X�g
	typedef feature::value_type                 value_type;				///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�Dmist::array< data > �� data �Ɠ���
	typedef feature::size_type                  size_type;				///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef feature::difference_type            difference_type;		///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���

protected:
	classifier_list_type	classifiers_;				///< @brief AdaBoost���ʊ�̃��X�g
	std::string				tag_;						///< @brief ���[�U�[���Ǝ��ɐݒ�\�ȕ�����

	double minimum_true_positive_rate_per_stage_;		///< @brief �e�X�e�[�W�Ŗ������ׂ�TP���̉���
	double maximum_false_positive_rate_per_stage_;		///< @brief �e�X�e�[�W�Ŗ������ׂ�FP���̏��
	double total_false_positive_rate_;					///< @brief �J�X�P�[�h�^���ʊ�̍\�z���̏I�������ƂȂ�S�̂Ƃ��Ă�FP��
	size_type maximum_number_of_classifiers_per_stage_;	///< @brief �\�z����J�X�P�[�h�̍ő�i��
	double last_true_positive_rate_;					///< @brief �J�X�P�[�h�^���ʊ�̍Ō�̊w�K���s�������_�ł̑S�̂�TP��
	double last_false_positive_rate_[ 4 ];				///< @brief �J�X�P�[�h�^���ʊ�̍Ō�̊w�K���s�������_�ł̑S�̂�FP��

public:
	/// @brief �e�X�e�[�W�Ŗ������ׂ�TP���̉������擾
	double minimum_true_positive_rate_per_stage( )  const { return( minimum_true_positive_rate_per_stage_ ); }
	/// @brief �e�X�e�[�W�Ŗ������ׂ�FP���̏�����擾
	double maximum_false_positive_rate_per_stage( ) const { return( maximum_false_positive_rate_per_stage_ ); };
	/// @brief �w�K�̏I�������ƂȂ�S�̂Ƃ��Ă�FP�����擾
	double total_false_positive_rate( )             const { return( total_false_positive_rate_ ); };
	/// @brief �\�z����J�X�P�[�h�̍ő�i�����擾
	size_type maximum_number_of_classifiers_per_stage( )  const { return( maximum_number_of_classifiers_per_stage_ ); };
	/// @brief �Ō�̊w�K���s�������_�ł̑S�̂�FP�����擾
	double current_false_positive_rate( )              const { return( last_false_positive_rate_[ 3 ] ); };
	/// @brief �Ō�̊w�K���s�������_�ł̑S�̂�FP�����擾
	double last_false_positive_rate( )              const { return( last_false_positive_rate_[ 0 ] ); };
	/// @brief �Ō�̊w�K���s�������_�ł̑S�̂�TP�����擾
	double last_true_positive_rate( )               const { return( last_true_positive_rate_ ); };

	/// @brief �J�X�P�[�h�ڑ����ꂽ���ׂĂ�AdaBoost���ʊ�Ŏg�p����㎯�ʊ�̐����擾����
	size_type total_number_of_classifiers( ) const
	{
		size_type num = 0;
		for( size_type i = 0 ; i < classifiers_.size( ) ; i++ )
		{
			num += classifiers_[ i ].number_of_classifiers( );
		}
		return( num );
	}

	/// @brief ���ʊ�ɑ΂��ă��[�U�[���ݒ�\�ȏ���\����������擾����
	std::string tag( ) const { return( tag_ ); }
	/// @brief ���ʊ�ɑ΂��ă��[�U�[���ݒ�\�ȏ���\���������ݒ肷��
	void tag( const std::string &str ){ tag_ = str; }

	/// @brief ���ʊ���\�z����AdaBoost���ʊ�̃��X�g���擾����
	size_type number_of_stages( ) const
	{
		return( classifiers_.size( ) );
	}

	/// @brief ���ʊ���\�z����AdaBoost���ʊ�̃��X�g���擾����
	classifier_list_type & classifiers( )
	{
		return( classifiers_ );
	}

	/// @brief ���ʊ���\�z����AdaBoost���ʊ�̃��X�g���擾����
	const classifier_list_type & classifiers( ) const
	{
		return( classifiers_ );
	}

	/// @brief �V����AdaBoost���ʊ��ǉ�����
	bool add_classifier( const base_classifier_type &c )
	{
		classifiers_.push_back( c );

		return( true );
	}

public:
	/// @brief �f�t�H���g�̃R���X�g���N�^
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

	/// @brief �R�s�[�R���X�g���N�^
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

	/// @brief �f�X�g���N�^
	~cascaded_classifier_base( ){ }

	/// @brief ���̃J�X�P�[�h�^AdaBoost���ʊ�̏����R�s�[����
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

	/// @brief �e�i�̋����ʊ�̏��������s���֐��i�����p�j
	//! 
	//! @param[in,out] features   �c �w�K�ɗp���鋳�t�f�[�^
	//! @param[in,out] classifier �c ���������鋭���ʊ�
	//! 
	template < template < typename, typename > class FEATURE_LIST, class FEATURE, class ALLOCATOR >
	void initialize__( FEATURE_LIST< FEATURE, ALLOCATOR > & features, base_classifier_type & classifier )
	{
		classifier.initialize( features );
	}

public:
	/// @brief ���t�f�[�^��p���ăJ�X�P�[�h�^AdaBoost���ʊ���\�z����
	//! 
	//! @param[in,out]  features                                �c �w�K�ɗp���鋳�t�f�[�^
	//! @param[in]      minimum_true_positive_rate_per_stage    �c �e�X�e�[�W�Ŗ������ׂ�TP���̉����i�f�t�H���g��0.99�j
	//! @param[in]      maximum_false_positive_rate_per_stage   �c �e�X�e�[�W�Ŗ������ׂ�FP���̏���i�f�t�H���g��0.50�j
	//! @param[in]      total_false_positive_rate               �c �w�K�̏I�������ƂȂ�S�̂Ƃ��Ă�FP���i�f�t�H���g��0.01�j
	//! @param[in]      maximum_number_of_classifiers_per_stage �c �\�z����AdaBoost���ʊ�Ŏg�p����㎯�ʊ�̍ő吔�i�f�t�H���g��200�j
	//! @param[in]      number_of_stages                        �c �\�z����J�X�P�[�h�̍ő�i���i�f�t�H���g��100�j
	//! @param[in]      number_of_classifiers_per_step          �c ��x�ɒǉ�����㎯�ʊ�̐��i�f�t�H���g��1�j�B�����������ʐ��\�͍����Ȃ�\���������B
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

			// �X�e�[�W�� 1 �i�߂�
			nstages++;
		}

		return( true );
#endif
	}

	/// @brief �J�X�P�[�h�^AdaBoost���ʊ�̐ݒ������������
	//! 
	//! @param[in]      minimum_true_positive_rate_per_stage    �c �e�X�e�[�W�Ŗ������ׂ�TP���̉����i�f�t�H���g��0.99�j
	//! @param[in]      maximum_false_positive_rate_per_stage   �c �e�X�e�[�W�Ŗ������ׂ�FP���̏���i�f�t�H���g��0.50�j
	//! @param[in]      total_false_positive_rate               �c �w�K�̏I�������ƂȂ�S�̂Ƃ��Ă�FP���i�f�t�H���g��0.01�j
	//! @param[in]      maximum_number_of_classifiers_per_stage �c �\�z����AdaBoost���ʊ�Ŏg�p����㎯�ʊ�̍ő吔�i�f�t�H���g��200�j
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

	/// @brief ���݂̃J�X�P�[�h�^AdaBoost���ʊ��AdaBoost���ʊ��V�����ǉ�����
	//! 
	//! @param[in,out]  features                       �c �w�K�ɗp���鋳�t�f�[�^
	//! @param[in]      number_of_stages               �c �\�z����J�X�P�[�h�̍ő�i���i�f�t�H���g��100�j
	//! @param[in]      number_of_classifiers_per_step �c ��x�ɒǉ�����㎯�ʊ�̐��i�f�t�H���g��1�j�B�����������ʐ��\�͍����Ȃ�\���������B
	//! @param[in]      false_positive_rate_per_stage  �c �㎯�ʊ�̒ǉ����~����ő��FP���B-1���w�肷��Ǝ��ʊ�ɐݒ肵�������l�𗘗p����B
	//! @param[in]      initialize_true_positive_rate  �c �r�����ʂ���w�K����ꍇ��TP�����ŏ���������i�f�t�H���g��false�j�B�ʏ�͎w�肷��K�v�͂Ȃ��B
	//! 
	template < template < typename, typename > class FEATURE_LIST, class FEATURE, class ALLOCATOR >
	bool add_classifier( FEATURE_LIST< FEATURE, ALLOCATOR > & features, size_type number_of_stages = 100, size_type number_of_classifiers_per_step = 1, double false_positive_rate_per_stage = -1.0, bool initialize_true_positive_rate = false )
	{
		return( add_classifier( features, empty_info_functor( ), number_of_stages, number_of_classifiers_per_step, false_positive_rate_per_stage, initialize_true_positive_rate ) );
	}

	/// @brief ���݂̃J�X�P�[�h�^AdaBoost���ʊ��AdaBoost���ʊ��V�����ǉ�����
	//! 
	//! @param[in,out]  features                       �c �w�K�ɗp���鋳�t�f�[�^
	//! @param[in]      number_of_stages               �c �\�z����J�X�P�[�h�̍ő�i���i�f�t�H���g��100�j
	//! @param[in]      number_of_classifiers_per_step �c ��x�ɒǉ�����㎯�ʊ�̐��i�f�t�H���g��1�j�B�����������ʐ��\�͍����Ȃ�\���������B
	//! @param[in]      false_positive_rate_per_stage  �c �㎯�ʊ�̒ǉ����~����ő��FP���B-1���w�肷��Ǝ��ʊ�ɐݒ肵�������l�𗘗p����B
	//! @param[in]      initialize_true_positive_rate  �c �r�����ʂ���w�K����ꍇ��TP�����ŏ���������i�f�t�H���g��false�j�B�ʏ�͎w�肷��K�v�͂Ȃ��B
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

		// �L���ȃT���v����ǉ�����
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

		// ���ނ��ׂ��l�K�e�B�u�T���v����������Ȃ��̂ŏI������
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

		// ���ۂ̃T���v�����ɍ��킹�Ė������ׂ�TP�������FP�����Čv�Z����
		{
			// TP��������������ꍇ�͒�������
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

			// �e�X�e�[�W�͋����ʊ�ō\������
			{
				// ���ʊ�̏�����
				base_classifier_type clsf;

				// ���ʊ������������
				( ( derived_type * ) this )->initialize__( flist, clsf );

				// ���ʊ�����X�g�ɒǉ�����
				classifiers_.push_back( clsf );
			}

			base_classifier_type &classifier = classifiers_.back( );

			size_t num_weaks = 0;
			size_t max_weaks = maximum_number_of_classifiers_per_stage_;
			while( current_false_positive_rate > __maximum_false_positive_rate_per_stage__ )
			{
				// �J�X�P�[�h���ʊ킪�ڕW�̌댟�o���������܂�
				// ���X�e�[�W�̋����ʊ�Ɏg�p����㎯�ʊ퐔�𑝂₷
				for( size_type n = 0 ; n < number_of_classifiers_per_step && num_weaks <= max_weaks ; n++ )
				{
					num_weaks++;

					// �㎯�ʋ@��1�ǉ�����
					classifier.add_classifiers( flist, finfo, 1 );

					// TP����]������
					current_true_positive_rate = true_positive_rate( flist );
					if( current_true_positive_rate > __minimum_true_positive_rate_per_stage__ )
					{
						break;
					}

#if defined( __ADABOOST_OUTPUT_DEBUG_STRING__ ) && __ADABOOST_OUTPUT_DEBUG_STRING__ != 0
					if( n < number_of_classifiers_per_step - 1 )
					{
						// �J�X�P�[�h���ʊ�̌댟�o�����v�Z����
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

				// �㎯�ʊ퐔�𑝉������Ă��C�ڕW�̌댟�o���������Ȃ��ꍇ�́C
				// ���������������邽�ߊw�K���I������
				if( num_weaks > max_weaks )
				{
#if defined( __ADABOOST_OUTPUT_DEBUG_STRING__ ) && __ADABOOST_OUTPUT_DEBUG_STRING__ != 0
					printf( "------------------------------------- REMOVED -------------------------------------\n" );
#endif

					// �w�K�Ɏ��s�������߃��X�g���珜�O����
					classifiers_.pop_back( );

					return( false );
				}

				double th1 = classifier.threshold( );
				while( true )
				{
					// �J�X�P�[�h���ʊ�̌��o�����v�Z����
					current_true_positive_rate = true_positive_rate( flist );

					if( current_true_positive_rate > __minimum_true_positive_rate_per_stage__ )
					{
						break;
					}

					// ���X�e�[�W�̋����ʊ�̂������l�𒲐߂��J�X�P�[�h���ʊ�̌��o�����グ��
					th1 = classifier.threshold( );
					classifier.threshold( th1 - 0.5 );
				}

				double th2 = classifier.threshold( );

				if( th1 > th2 )
				{
					for( size_type l = 0 ; th2 < th1; l++ )
					{
						// ���X�e�[�W�̋����ʊ�̂������l�𒲐߂��J�X�P�[�h���ʊ�̌��o�����グ��
						double th = ( th1 + th2 ) * 0.5;
						classifier.threshold( th );

						// �J�X�P�[�h���ʊ�̌��o�����v�Z����
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

				// �J�X�P�[�h���ʊ�̌댟�o�����v�Z����
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

			// ���݂̃X�e�[�W�̃J�X�P�[�h���ʊ�̌��o���ƌ댟�o�����v�Z����
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

				// �w�K���ʂ��L�^����
				std::string str = "TP(total): " + utility::ToString( total_TP, "%f" ) + ", FP(total): " + utility::ToString( total_FP, "%f" ) + ", FP(current): " + utility::ToString( current_FP, "%f" );
				classifier.info( str );

#if defined( __ADABOOST_OUTPUT_DEBUG_STRING__ ) && __ADABOOST_OUTPUT_DEBUG_STRING__ != 0
				printf( "-----------------------------------------------------------------------------------\n" );
#endif
			}

			// �ŏI�I�Ȍ댟�o���������Ȃ��ꍇ�́C���݂̃J�X�P�[�h���ʊ��
			// ��菜���� Negative �f�[�^���w�K�f�[�^����폜����
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

			// �X�e�[�W�� 1 �i�߂�
			nstages++;
		}

		return( true );
	}


public:
	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f �c ���ނ��������
	//! 
	//! @return ���ތ���
	//! 
	template < class FEATURE >
	bool operator ()( const FEATURE &f ) const
	{
		return( ( ( const derived_type * ) this )->evaluate( f ) >= 0 );
	}

	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p �c �����ʌv�Z�Ɏg�p����p�����[�^
	//! 
	//! @return ���ތ���
	//! 
	template < class FUNCTOR, class PARAMS >
	bool operator ()( const FUNCTOR &f, const PARAMS &p ) const
	{
		return( ( ( const derived_type * ) this )->evaluate( p, f ) >= 0 );
	}

	/// @brief �w�K�ς݂̎㎯�ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2 >
	bool operator ()( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2 ) const
	{
		return( ( ( const derived_type * ) this )->evaluate( f, p1, p2 ) >= 0 );
	}

	/// @brief �w�K�ς݂̎㎯�ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  p3 �c �����ʌv�Z�Ɏg�p����p�����[�^3
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3 >
	bool operator ()( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3 ) const
	{
		return( ( ( const derived_type * ) this )->evaluate( f, p1, p2, p3 ) >= 0 );
	}

	/// @brief �w�K�ς݂̎㎯�ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  p3 �c �����ʌv�Z�Ɏg�p����p�����[�^3
	//! @param[in]  p4 �c �����ʌv�Z�Ɏg�p����p�����[�^4
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4 >
	bool operator ()( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, const PARAM4 &p4 ) const
	{
		return( ( ( const derived_type * ) this )->evaluate( f, p1, p2, p3, p4 ) >= 0 );
	}

	/// @brief �w�K�ς݂̎㎯�ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  p3 �c �����ʌv�Z�Ɏg�p����p�����[�^3
	//! @param[in]  p4 �c �����ʌv�Z�Ɏg�p����p�����[�^4
	//! @param[in]  p5 �c �����ʌv�Z�Ɏg�p����p�����[�^5
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5 >
	bool operator ()( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, const PARAM4 &p4, const PARAM5 &p5 ) const
	{
		return( ( ( const derived_type * ) this )->evaluate( f, p1, p2, p3, p4, p5 ) >= 0 );
	}

	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f �c ���ނ��������
	//! 
	//! @return ���ތ���
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

	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p �c �����ʌv�Z�Ɏg�p����p�����[�^
	//! 
	//! @return ���ތ���
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

	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! 
	//! @return ���ތ���
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

	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  p3 �c �����ʌv�Z�Ɏg�p����p�����[�^3
	//! 
	//! @return ���ތ���
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

	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  p3 �c �����ʌv�Z�Ɏg�p����p�����[�^3
	//! @param[in]  p4 �c �����ʌv�Z�Ɏg�p����p�����[�^4
	//! 
	//! @return ���ތ���
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

	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  p3 �c �����ʌv�Z�Ɏg�p����p�����[�^3
	//! @param[in]  p4 �c �����ʌv�Z�Ɏg�p����p�����[�^4
	//! @param[in]  p5 �c �����ʌv�Z�Ɏg�p����p�����[�^4
	//! 
	//! @return ���ތ���
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
	/// @brief �ǉ��̋��t�f�[�^��p���Ď㎯�ʊ���X�V����
	//! 
	//! @param[in]  f           �c ���ނ�������ʃx�N�g��
	//! @param[in]  is_positive �c �|�W�e�B�u�T���v�����ǂ���
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

	/// @brief �ǉ��̋��t�f�[�^��p���Ď㎯�ʊ���X�V����
	//! 
	//! @param[in]  f �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p �c �����ʌv�Z�Ɏg�p����p�����[�^
	//! @param[in]  is_positive �c �|�W�e�B�u�T���v�����ǂ���
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

	/// @brief �ǉ��̋��t�f�[�^��p���Ď㎯�ʊ���X�V����
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  is_positive �c �|�W�e�B�u�T���v�����ǂ���
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

	/// @brief �ǉ��̋��t�f�[�^��p���Ď㎯�ʊ���X�V����
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  p3 �c �����ʌv�Z�Ɏg�p����p�����[�^3
	//! @param[in]  is_positive �c �|�W�e�B�u�T���v�����ǂ���
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

	/// @brief �ǉ��̋��t�f�[�^��p���Ď㎯�ʊ���X�V����
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  p3 �c �����ʌv�Z�Ɏg�p����p�����[�^3
	//! @param[in]  p4 �c �����ʌv�Z�Ɏg�p����p�����[�^4
	//! @param[in]  is_positive �c �|�W�e�B�u�T���v�����ǂ���
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
	/// @brief �����ʂ̃��X�g����TP�����v�Z����
	//! 
	//! @param[in]  features             �c TP���̌v�Z�Ɏg�p��������ʂ̃��X�g
	//! @param[in]  force_calculate_rate �c �����ɐݒ肳��Ă�������ʂ��v�Z�Ɏg�p���邩�ǂ����i�f�t�H���g��false�j
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

	/// @brief �����ʂ̃��X�g����FP�����v�Z����
	//! 
	//! @param[in]  features             �c FP���̌v�Z�Ɏg�p��������ʂ̃��X�g
	//! @param[in]  force_calculate_rate �c �����ɐݒ肳��Ă�������ʂ��v�Z�Ɏg�p���邩�ǂ����i�f�t�H���g��false�j
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

	/// @brief �����ʂ̃��X�g����TP�����v�Z����
	//! 
	//! @param[in]  features             �c TP���̌v�Z�Ɏg�p��������ʂ̃��X�g
	//! @param[out] rates                �c ���f�[�^�ƐV�f�[�^�ɕ����Čv�Z����TP��
	//! @param[in]  force_calculate_rate �c �����ɐݒ肳��Ă�������ʂ��v�Z�Ɏg�p���邩�ǂ����i�f�t�H���g��false�j
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
		rates[ 1 ] = static_cast< double >( detections[ 0 ] );	// �]���f�[�^
		rates[ 2 ] = static_cast< double >( positives[ 0 ] );	// �]���f�[�^
		rates[ 3 ] = static_cast< double >( detections[ 1 ] );	// �V�K�f�[�^
		rates[ 4 ] = static_cast< double >( positives[ 1 ] );	// �V�K�f�[�^
	}

	/// @brief �����ʂ̃��X�g����FP�����v�Z����
	//! 
	//! @param[in]  features             �c TP���̌v�Z�Ɏg�p��������ʂ̃��X�g
	//! @param[out] rates                �c ���f�[�^�ƐV�f�[�^�ɕ����Čv�Z����FP��
	//! @param[in]  force_calculate_rate �c �����ɐݒ肳��Ă�������ʂ��v�Z�Ɏg�p���邩�ǂ����i�f�t�H���g��false�j
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
		rates[ 1 ] = static_cast< double >( miss_detections[ 0 ] );	// �]���f�[�^
		rates[ 2 ] = static_cast< double >( negatives[ 0 ] );		// �]���f�[�^
		rates[ 3 ] = static_cast< double >( miss_detections[ 1 ] );	// �V�K�f�[�^
		rates[ 4 ] = static_cast< double >( negatives[ 1 ] );		// �V�K�f�[�^
	}
};



/// @brief �㎯�ʊ��Multi-exit�ڑ����������ʊ�̊��N���X
template < class DERIVED_CLASSIFIER, class BASE_WEAK_CLASSIFIER >
class multi_exit_classifier_base : public classifier_base
{
private:
	typedef classifier_base						base;					///< @brief ���N���X
	typedef DERIVED_CLASSIFIER                  derived_type;			///< @brief �h���N���X

public:
	typedef BASE_WEAK_CLASSIFIER                base_classifier_type;	///< @brief AdaBoost�ŗ��p����㎯�ʊ�
	typedef feature::value_type                 value_type;				///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�Dmist::array< data > �� data �Ɠ���
	typedef feature::size_type                  size_type;				///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef feature::difference_type            difference_type;		///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���
	typedef std::vector< base_classifier_type > classifier_list_type;	///< @brief Multi-exit�^�̃J�X�P�[�h�ɂ��鎯�ʊ�̃��X�g
	typedef std::vector< size_type >            exit_list_type;			///< @brief Multi-exit�^�̃J�X�P�[�h�𔲂��鎯�ʊ�̃C���f�b�N�X��ێ����郊�X�g

protected:
	classifier_list_type	classifiers_;				///< @brief �㎯�ʊ�̃��X�g
	exit_list_type			exits_;						///< @brief Multi-exit�^�J�X�P�[�h�̏o���̃��X�g
	std::string				info_;						///< @brief ���ʊ���w�K�����ۂ̏���ێ����镶����
	std::string				tag_;						///< @brief ���[�U�[���Ǝ��ɐݒ�\�ȕ�����

	double minimum_true_positive_rate_per_stage_;		///< @brief �e�X�e�[�W�Ŗ������ׂ�TP���̉���
	double maximum_false_positive_rate_per_stage_;		///< @brief �e�X�e�[�W�Ŗ������ׂ�FP���̏��
	double total_false_positive_rate_;					///< @brief Multi-exit�^�J�X�P�[�h���ʊ�̍\�z���̏I�������ƂȂ�S�̂Ƃ��Ă�FP��
	size_type maximum_number_of_classifiers_per_stage_;	///< @brief �\�z����J�X�P�[�h�̍ő�i��
	double last_true_positive_rate_;					///< @brief Multi-exit�^�J�X�P�[�h���ʊ�̍Ō�̊w�K���s�������_�ł̑S�̂�TP��
	double last_false_positive_rate_[ 4 ];				///< @brief Multi-exit�^�J�X�P�[�h���ʊ�̍Ō�̊w�K���s�������_�ł̑S�̂�FP��

public:
	/// @brief �e�X�e�[�W�Ŗ������ׂ�TP���̉������擾
	double minimum_true_positive_rate_per_stage( )  const { return( minimum_true_positive_rate_per_stage_ ); }
	/// @brief �e�X�e�[�W�Ŗ������ׂ�FP���̏�����擾
	double maximum_false_positive_rate_per_stage( ) const { return( maximum_false_positive_rate_per_stage_ ); };
	/// @brief �w�K�̏I�������ƂȂ�S�̂Ƃ��Ă�FP�����擾
	double total_false_positive_rate( )             const { return( total_false_positive_rate_ ); };
	/// @brief �\�z����J�X�P�[�h�̍ő�i�����擾
	size_type maximum_number_of_classifiers_per_stage( )  const { return( maximum_number_of_classifiers_per_stage_ ); };
	/// @brief �Ō�̊w�K���s�������_�ł̑S�̂�FP�����擾
	double current_false_positive_rate( )              const { return( last_false_positive_rate_[ 3 ] ); };
	/// @brief �Ō�̊w�K���s�������_�ł̑S�̂�FP�����擾
	double last_false_positive_rate( )              const { return( last_false_positive_rate_[ 0 ] ); };
	/// @brief �Ō�̊w�K���s�������_�ł̑S�̂�TP�����擾
	double last_true_positive_rate( )               const { return( last_true_positive_rate_ ); };

	/// @brief ���ʊ���w�K�����ۂ̏���ێ����镶������擾����
	std::string info( ) const { return( info_ ); }
	/// @brief ���ʊ���w�K�����ۂ̏���ێ����镶�����ݒ肷��
	void info( const std::string &str ){ info_ = str; }

	/// @brief ���ʊ�ɑ΂��ă��[�U�[���ݒ�\�ȏ���\����������擾����
	std::string tag( ) const { return( tag_ ); }
	/// @brief ���ʊ�ɑ΂��ă��[�U�[���ݒ�\�ȏ���\���������ݒ肷��
	void tag( const std::string &str ){ tag_ = str; }

	/// @brief ���ʊ���\�z����AdaBoost���ʊ�̃��X�g���擾����
	size_type number_of_stages( ) const
	{
		return( exits_.size( ) );
	}

	/// @brief AdaBoost���ʊ���\�z����㎯�ʊ�̐����擾����
	size_type number_of_classifiers( ) const { return( classifiers_.size( ) ); }

	/// @brief AdaBoost���ʊ���\�z����㎯�ʊ�̐����擾����
	size_type total_number_of_classifiers( ) const { return( classifiers_.size( ) ); }

	/// @brief ���ʊ���\�z����㎯�ʊ�̃��X�g���擾����
	classifier_list_type & classifiers( ){ return( classifiers_ ); }

	/// @brief ���ʊ���\�z����㎯�ʊ�̃��X�g���擾����
	const classifier_list_type & classifiers( ) const { return( classifiers_ ); }

	/// @brief Multi-exit�^�J�X�P�[�h�𔲂��鎯�ʊ�̃C���f�b�N�X��ێ����郊�X�g���擾����
	exit_list_type & exits( ){ return( exits_ ); }

	/// @brief Multi-exit�^�J�X�P�[�h�𔲂��鎯�ʊ�̃C���f�b�N�X��ێ����郊�X�g���擾����
	const exit_list_type & exits( ) const { return( exits_ ); }

	/// @brief �V�����㎯�ʊ��ǉ�����
	bool add_classifier( const base_classifier_type &c )
	{
		classifiers_.push_back( c );

		return( true );
	}

public:
	/// @brief �f�t�H���g�̃R���X�g���N�^
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

	/// @brief �R�s�[�R���X�g���N�^
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

	/// @brief �f�X�g���N�^
	~multi_exit_classifier_base( ){ }

	/// @brief ����Multi-exit�^AdaBoost���ʊ�̏����R�s�[����
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

	/// @brief �e�i�̋����ʊ�̏��������s���֐��i�����p�j
	//! 
	//! @param[in,out] features   �c �w�K�ɗp���鋳�t�f�[�^
	//! @param[in,out] classifier �c ���������鋭���ʊ�
	//! 
	template < template < typename, typename > class FEATURE_LIST, class FEATURE, class ALLOCATOR >
	void initialize__( FEATURE_LIST< FEATURE, ALLOCATOR > & /* features */, base_classifier_type & /* classifier */ )
	{
	}

public:
	/// @brief ���t�f�[�^��p����Multi-exit�^AdaBoost���ʊ���\�z����
	//! 
	//! @param[in,out]  features                                �c �w�K�ɗp���鋳�t�f�[�^
	//! @param[in]      minimum_true_positive_rate_per_stage    �c �e�X�e�[�W�Ŗ������ׂ�TP���̉����i�f�t�H���g��0.99�j
	//! @param[in]      maximum_false_positive_rate_per_stage   �c �e�X�e�[�W�Ŗ������ׂ�FP���̏���i�f�t�H���g��0.50�j
	//! @param[in]      total_false_positive_rate               �c �w�K�̏I�������ƂȂ�S�̂Ƃ��Ă�FP���i�f�t�H���g��0.01�j
	//! @param[in]      maximum_number_of_classifiers_per_stage �c �\�z����AdaBoost���ʊ�Ŏg�p����㎯�ʊ�̍ő吔�i�f�t�H���g��200�j
	//! @param[in]      number_of_stages                        �c �\�z����J�X�P�[�h�̍ő�i���i�f�t�H���g��100�j
	//! @param[in]      minimum_number_of_classifiers_per_step  �c �e�X�e�[�W���\������㎯�ʊ�̍ŏ����i�f�t�H���g��1�j�B�����������ʐ��\�͍����Ȃ�\���������B
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

			// �X�e�[�W�� 1 �i�߂�
			nstages++;
		}

		return( true );
#endif
	}

	/// @brief Multi-exit�^AdaBoost���ʊ�̐ݒ������������
	//! 
	//! @param[in]      minimum_true_positive_rate_per_stage    �c �e�X�e�[�W�Ŗ������ׂ�TP���̉����i�f�t�H���g��0.99�j
	//! @param[in]      maximum_false_positive_rate_per_stage   �c �e�X�e�[�W�Ŗ������ׂ�FP���̏���i�f�t�H���g��0.50�j
	//! @param[in]      total_false_positive_rate               �c �w�K�̏I�������ƂȂ�S�̂Ƃ��Ă�FP���i�f�t�H���g��0.01�j
	//! @param[in]      maximum_number_of_classifiers_per_stage �c �\�z����AdaBoost���ʊ�Ŏg�p����㎯�ʊ�̍ő吔�i�f�t�H���g��200�j
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

	/// @brief ���݂�Multi-exit�^AdaBoost���ʊ��AdaBoost���ʊ��V�����ǉ�����
	//! 
	//! @param[in,out]  features                               �c �w�K�ɗp���鋳�t�f�[�^
	//! @param[in]      number_of_exits                        �c �\�z����Multi-exit�^�J�X�P�[�h�̍ő�i���i�f�t�H���g��100�j
	//! @param[in]      minimum_number_of_classifiers_per_step �c �e�X�e�[�W���\������㎯�ʊ�̍ŏ����i�f�t�H���g��1�j�B�����������ʐ��\�͍����Ȃ�\���������B
	//! @param[in]      false_positive_rate_per_stage          �c �㎯�ʊ�̒ǉ����~����ő��FP���B-1���w�肷��Ǝ��ʊ�ɐݒ肵�������l�𗘗p����B
	//! @param[in]      initialize_true_positive_rate          �c �r�����ʂ���w�K����ꍇ��TP�����ŏ���������i�f�t�H���g��false�j�B�ʏ�͎w�肷��K�v�͂Ȃ��B
	//! 
	template < template < typename, typename > class FEATURE_LIST, class FEATURE, class ALLOCATOR >
	bool add_classifier( FEATURE_LIST< FEATURE, ALLOCATOR > & features, size_type number_of_exits = 100, size_type minimum_number_of_classifiers_per_step = 1, double false_positive_rate_per_stage = -1.0, bool initialize_true_positive_rate = false )
	{
		return( add_classifier( features, empty_info_functor( ), number_of_exits, minimum_number_of_classifiers_per_step, false_positive_rate_per_stage, initialize_true_positive_rate ) );
	}

	/// @brief ���݂�Multi-exit�^AdaBoost���ʊ��AdaBoost���ʊ��V�����ǉ�����
	//! 
	//! @param[in,out]  features                               �c �w�K�ɗp���鋳�t�f�[�^
	//! @param[in]      number_of_exits                        �c �\�z����Multi-exit�^�J�X�P�[�h�̍ő�i���i�f�t�H���g��100�j
	//! @param[in]      minimum_number_of_classifiers_per_step �c �e�X�e�[�W���\������㎯�ʊ�̍ŏ����i�f�t�H���g��1�j�B�����������ʐ��\�͍����Ȃ�\���������B
	//! @param[in]      false_positive_rate_per_stage          �c �㎯�ʊ�̒ǉ����~����ő��FP���B-1���w�肷��Ǝ��ʊ�ɐݒ肵�������l�𗘗p����B
	//! @param[in]      initialize_true_positive_rate          �c �r�����ʂ���w�K����ꍇ��TP�����ŏ���������i�f�t�H���g��false�j�B�ʏ�͎w�肷��K�v�͂Ȃ��B
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

		// �L���ȃT���v����ǉ�����
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

		// ���ނ��ׂ��l�K�e�B�u�T���v����������Ȃ��̂ŏI������
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

		// ���ۂ̃T���v�����ɍ��킹�Ė������ׂ�TP�������FP�����Čv�Z����
		{
			// TP��������������ꍇ�͒�������
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

			// �|�W�e�B�u�ƃl�K�e�B�u�̌��𐔂�����
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

			// ���ނ��ׂ��l�K�e�B�u�T���v����������Ȃ��̂ŏI������
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


			// �e�X�e�[�W�͎㎯�ʊ�ō\������
			{
				// ���ʊ�̏�����
				base_classifier_type clsf;

				// ���ʊ������������
				( ( derived_type * ) this )->initialize__( flist, clsf );
			}

			// �d�݂̏�����
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

			// ���ʊ�̏o����ݒ肷��
			exits_.push_back( classifiers_.size( ) );

			size_t num_weaks = 0;
			size_t max_weaks = maximum_number_of_classifiers_per_stage_;
			while( num_weaks <= max_weaks )
			{
				// Multi-exit�^�J�X�P�[�h���ʊ킪�ڕW�̌댟�o���������܂�
				// ���X�e�[�W�̋����ʊ�Ɏg�p����㎯�ʊ퐔�𑝂₷
				num_weaks++;

				// �㎯�ʋ@��1�ǉ�����
				( ( derived_type * ) this )->add_classifiers__( flist, 1 );

				// �o�����Ō�Ɉړ�������
				exits_.back( ) = classifiers_.size( );

#if defined( __ADABOOST_OUTPUT_DEBUG_STRING__ ) && __ADABOOST_OUTPUT_DEBUG_STRING__ != 0
				{
					// Multi-exit�^�J�X�P�[�h���ʊ�̌댟�o�����v�Z����
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

				// ���\��]������
				double alpha = current_false_positive_rate;
				double beta  = 1.0 - current_true_positive_rate;

				if( num_weaks >= minimum_number_of_classifiers_per_step && alpha <= alpha0 && beta <= beta0 )
				{
					break;
				}
			}

			// �㎯�ʊ퐔�𑝉������Ă��C�ڕW�̌댟�o���������Ȃ��ꍇ�́C
			// ���������������邽�ߊw�K���I������
			if( num_weaks > max_weaks )
			{
#if defined( __ADABOOST_OUTPUT_DEBUG_STRING__ ) && __ADABOOST_OUTPUT_DEBUG_STRING__ != 0
				printf( "------------------------------------- REMOVED -------------------------------------\n" );
#endif

				// �w�K�Ɏ��s�������߃��X�g���珜�O����
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

			// ���݂̃X�e�[�W��Multi-exit�^�J�X�P�[�h���ʊ�̌��o���ƌ댟�o�����v�Z����
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

				// �w�K���ʂ��L�^����
				std::string str = "TP(total): " + utility::ToString( total_TP, "%f" ) + ", FP(total): " + utility::ToString( total_FP, "%f" ) + ", FP(current): " + utility::ToString( current_FP, "%f" );

#if defined( __ADABOOST_OUTPUT_DEBUG_STRING__ ) && __ADABOOST_OUTPUT_DEBUG_STRING__ != 0
				printf( "-----------------------------------------------------------------------------------\n" );
#endif
			}

			// �ŏI�I�Ȍ댟�o���������Ȃ��ꍇ�́C���݂�Multi-exit�^�J�X�P�[�h���ʊ��
			// ��菜���� Negative �f�[�^���w�K�f�[�^����폜����
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

			// �X�e�[�W�� 1 �i�߂�
			nstages++;
		}

		return( true );
	}


public:
	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f �c ���ނ��������
	//! 
	//! @return ���ތ���
	//! 
	template < class FEATURE >
	bool operator ()( const FEATURE &f ) const
	{
		return( ( ( const derived_type * ) this )->evaluate( f ) >= 0 );
	}

	/// @brief �w�K�ς݂̕ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p �c �����ʌv�Z�Ɏg�p����p�����[�^
	//! 
	//! @return ���ތ���
	//! 
	template < class FUNCTOR, class PARAMS >
	bool operator ()( const FUNCTOR &f, const PARAMS &p ) const
	{
		return( ( ( const derived_type * ) this )->evaluate( p, f ) >= 0 );
	}

	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2 >
	bool operator ()( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2 ) const
	{
		return( ( ( const derived_type * ) this )->evaluate( f, p1, p2 ) >= 0 );
	}

	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  p3 �c �����ʌv�Z�Ɏg�p����p�����[�^3
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3 >
	bool operator ()( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3 ) const
	{
		return( ( ( const derived_type * ) this )->evaluate( f, p1, p2, p3 ) >= 0 );
	}

	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  p3 �c �����ʌv�Z�Ɏg�p����p�����[�^3
	//! @param[in]  p4 �c �����ʌv�Z�Ɏg�p����p�����[�^4
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4 >
	bool operator ()( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, const PARAM4 &p4 ) const
	{
		return( ( ( const derived_type * ) this )->evaluate( f, p1, p2, p3, p4 ) >= 0 );
	}

	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  p3 �c �����ʌv�Z�Ɏg�p����p�����[�^3
	//! @param[in]  p4 �c �����ʌv�Z�Ɏg�p����p�����[�^4
	//! @param[in]  p5 �c �����ʌv�Z�Ɏg�p����p�����[�^5
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5 >
	bool operator ()( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, const PARAM4 &p4, const PARAM5 &p5 ) const
	{
		return( ( ( const derived_type * ) this )->evaluate( f, p1, p2, p3, p4, p5 ) >= 0 );
	}

	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f �c ���ނ��������
	//! 
	//! @return ���ތ���
	//! 
	template < class FEATURE >
	double evaluate( const FEATURE &f ) const
	{
		return( confidence( f ) );
	}

	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p �c �����ʌv�Z�Ɏg�p����p�����[�^
	//! 
	//! @return ���ތ���
	//! 
	template < class FUNCTOR, class PARAMS >
	double evaluate( const FUNCTOR &f, const PARAMS &p ) const
	{
		return( confidence( f, p ) );
	}

	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! 
	//! @return ���ތ���
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2 >
	double evaluate( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2 ) const
	{
		return( confidence( f, p1, p2 ) );
	}

	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  p3 �c �����ʌv�Z�Ɏg�p����p�����[�^3
	//! 
	//! @return ���ތ���
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3 >
	double evaluate( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3 ) const
	{
		return( confidence( f, p1, p2, p3 ) );
	}

	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  p3 �c �����ʌv�Z�Ɏg�p����p�����[�^3
	//! @param[in]  p4 �c �����ʌv�Z�Ɏg�p����p�����[�^4
	//! 
	//! @return ���ތ���
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4 >
	double evaluate( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, const PARAM4 &p4 ) const
	{
		return( confidence( f, p1, p2, p3, p4 ) );
	}

	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  p3 �c �����ʌv�Z�Ɏg�p����p�����[�^3
	//! @param[in]  p4 �c �����ʌv�Z�Ɏg�p����p�����[�^4
	//! @param[in]  p5 �c �����ʌv�Z�Ɏg�p����p�����[�^5
	//! 
	//! @return ���ތ���
	//! 
	template < class FUNCTOR, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5 >
	double evaluate( const FUNCTOR &f, const PARAM1 &p1, const PARAM2 &p2, const PARAM3 &p3, const PARAM4 &p4, const PARAM5 &p5 ) const
	{
		return( confidence( f, p1, p2, p3, p4, p5 ) );
	}

public:
	/// @brief �ǉ��̋��t�f�[�^��p���Ď㎯�ʊ���X�V����
	//! 
	//! @param[in]  f           �c ���ނ�������ʃx�N�g��
	//! @param[in]  is_positive �c �|�W�e�B�u�T���v�����ǂ���
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


	/// @brief �ǉ��̋��t�f�[�^��p���Ď㎯�ʊ���X�V����
	//! 
	//! @param[in]  f �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p �c �����ʌv�Z�Ɏg�p����p�����[�^
	//! @param[in]  is_positive �c �|�W�e�B�u�T���v�����ǂ���
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

	/// @brief �ǉ��̋��t�f�[�^��p���Ď㎯�ʊ���X�V����
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  is_positive �c �|�W�e�B�u�T���v�����ǂ���
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

	/// @brief �ǉ��̋��t�f�[�^��p���Ď㎯�ʊ���X�V����
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  p3 �c �����ʌv�Z�Ɏg�p����p�����[�^3
	//! @param[in]  is_positive �c �|�W�e�B�u�T���v�����ǂ���
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

	/// @brief �ǉ��̋��t�f�[�^��p���Ď㎯�ʊ���X�V����
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  p3 �c �����ʌv�Z�Ɏg�p����p�����[�^3
	//! @param[in]  p4 �c �����ʌv�Z�Ɏg�p����p�����[�^4
	//! @param[in]  is_positive �c �|�W�e�B�u�T���v�����ǂ���
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

	/// @brief �ǉ��̋��t�f�[�^��p���Ď㎯�ʊ���X�V����
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  p3 �c �����ʌv�Z�Ɏg�p����p�����[�^3
	//! @param[in]  p4 �c �����ʌv�Z�Ɏg�p����p�����[�^4
	//! @param[in]  is_positive �c �|�W�e�B�u�T���v�����ǂ���
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
	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ̐M���x���v�Z����
	//! 
	//! @param[in]  f �c ���ނ��������
	//! 
	//! @return ���ތ���
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

	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ̐M���x���v�Z����
	//! 
	//! @param[in]  f �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p �c �����ʌv�Z�Ɏg�p����p�����[�^
	//! 
	//! @return ���ތ���
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

	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ̐M���x���v�Z����
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! 
	//! @return ���ތ���
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

	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ̐M���x���v�Z����
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  p3 �c �����ʌv�Z�Ɏg�p����p�����[�^3
	//! 
	//! @return ���ތ���
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

	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ̐M���x���v�Z����
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  p3 �c �����ʌv�Z�Ɏg�p����p�����[�^3
	//! @param[in]  p4 �c �����ʌv�Z�Ɏg�p����p�����[�^4
	//! 
	//! @return ���ތ���
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

	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ̐M���x���v�Z����
	//! 
	//! @param[in]  f  �c �����ʂ��v�Z����֐��I�u�W�F�N�g
	//! @param[in]  p1 �c �����ʌv�Z�Ɏg�p����p�����[�^1
	//! @param[in]  p2 �c �����ʌv�Z�Ɏg�p����p�����[�^2
	//! @param[in]  p3 �c �����ʌv�Z�Ɏg�p����p�����[�^3
	//! @param[in]  p4 �c �����ʌv�Z�Ɏg�p����p�����[�^4
	//! 
	//! @return ���ތ���
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
	/// @brief �����ʂ̃��X�g����TP�����v�Z����
	//! 
	//! @param[in]  features             �c TP���̌v�Z�Ɏg�p��������ʂ̃��X�g
	//! @param[in]  force_calculate_rate �c �����ɐݒ肳��Ă�������ʂ��v�Z�Ɏg�p���邩�ǂ����i�f�t�H���g��false�j
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

	/// @brief �����ʂ̃��X�g����FP�����v�Z����
	//! 
	//! @param[in]  features             �c FP���̌v�Z�Ɏg�p��������ʂ̃��X�g
	//! @param[in]  force_calculate_rate �c �����ɐݒ肳��Ă�������ʂ��v�Z�Ɏg�p���邩�ǂ����i�f�t�H���g��false�j
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

	/// @brief �����ʂ̃��X�g����TP�����v�Z����
	//! 
	//! @param[in]  features             �c TP���̌v�Z�Ɏg�p��������ʂ̃��X�g
	//! @param[out] rates                �c ���f�[�^�ƐV�f�[�^�ɕ����Čv�Z����TP��
	//! @param[in]  force_calculate_rate �c �����ɐݒ肳��Ă�������ʂ��v�Z�Ɏg�p���邩�ǂ����i�f�t�H���g��false�j
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
		rates[ 1 ] = static_cast< double >( detections[ 0 ] );	// �]���f�[�^
		rates[ 2 ] = static_cast< double >( positives[ 0 ] );	// �]���f�[�^
		rates[ 3 ] = static_cast< double >( detections[ 1 ] );	// �V�K�f�[�^
		rates[ 4 ] = static_cast< double >( positives[ 1 ] );	// �V�K�f�[�^
	}
	
	/// @brief �����ʂ̃��X�g����FP�����v�Z����
	//! 
	//! @param[in]  features             �c TP���̌v�Z�Ɏg�p��������ʂ̃��X�g
	//! @param[out] rates                �c ���f�[�^�ƐV�f�[�^�ɕ����Čv�Z����FP��
	//! @param[in]  force_calculate_rate �c �����ɐݒ肳��Ă�������ʂ��v�Z�Ɏg�p���邩�ǂ����i�f�t�H���g��false�j
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
		rates[ 1 ] = static_cast< double >( miss_detections[ 0 ] );	// �]���f�[�^
		rates[ 2 ] = static_cast< double >( negatives[ 0 ] );		// �]���f�[�^
		rates[ 3 ] = static_cast< double >( miss_detections[ 1 ] );	// �V�K�f�[�^
		rates[ 4 ] = static_cast< double >( negatives[ 1 ] );		// �V�K�f�[�^
	}
};

// AdaBoost���O��Ԃ̏I���
_ADABOOST_END


#endif	// __INCLUDE_ADABOOST_CLASSIFIER_BASE__