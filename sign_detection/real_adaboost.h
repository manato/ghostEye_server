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
// AdaBoost���O��Ԃ̎n�܂�
_ADABOOST_BEGIN
#endif


/// @brief AdaBoost �ŗ��p���邵�����l������p�����㎯�ʊ�
class weak_classifier : public weak_classifier_base< weak_classifier >
{
private:
	typedef weak_classifier_base< weak_classifier > base;	///< @brief ���N���X

public:
	typedef feature::value_type			value_type;			///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�Dmist::array< data > �� data �Ɠ���
	typedef feature::size_type			size_type;			///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef feature::difference_type	difference_type;	///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���

private:
	size_type num_bins_;
	std::vector< double > hist_;							///< @brief ���ʗp�̒l��ێ�����z��
	std::vector< double > pos_;								///< @brief ���ʗp�̃|�W�e�B�u�T���v�����̒l��ێ�����z��
	std::vector< double > neg_;								///< @brief ���ʗp�̃l�K�e�B�u�T���v�����̒l��ێ�����z��

public:
	size_type size( ) const { return( hist_.size( ) ); }

	/// @brief ���ʊ�Ŏg�p����m�����x�֐��̊e�l�փA�N�Z�X����I�y���[�^
	//! 
	//! @param[in]  indx �c �m�����x�֐��̒l���擾����C���f�b�N�X
	//! 
	const double & operator []( size_type indx ) const
	{
		return( hist_[ indx ] );
	}

	/// @brief ���ʊ�Ŏg�p����m�����x�֐��̊e�l�փA�N�Z�X����I�y���[�^
	//! 
	//! @param[in]  indx �c �m�����x�֐��̒l���擾����C���f�b�N�X
	//! 
	double & operator []( size_type indx )
	{
		return( hist_[ indx ] );
	}

	/// @brief ���ʊ�Ŏg�p����|�W�e�B�u�T���v�����̊m�����x�֐��̊e�l�փA�N�Z�X����֐�
	//! 
	//! @param[in]  indx  �c �m�����x�֐��̒l��ݒ肷��C���f�b�N�X
	//! @param[in]  value �c �m�����x�֐��֐ݒ肷��l
	//! 
	void pos( size_type indx, double value )
	{
		pos_[ indx ] = value;
	}

	/// @brief ���ʊ�Ŏg�p����|�W�e�B�u�T���v�����̊m�����x�֐��̊e�l�փA�N�Z�X����֐�
	//! 
	//! @param[in]  indx �c �m�����x�֐��̒l���擾����C���f�b�N�X
	//! 
	double pos( size_type indx ) const
	{
		return( pos_[ indx ] );
	}

	/// @brief ���ʊ�Ŏg�p����l�K�e�B�u�T���v�����̊m�����x�֐��̊e�l�փA�N�Z�X����֐�
	//! 
	//! @param[in]  indx  �c �m�����x�֐��̒l��ݒ肷��C���f�b�N�X
	//! @param[in]  value �c �m�����x�֐��֐ݒ肷��l
	//! 
	void neg( size_type indx, double value )
	{
		neg_[ indx ] = value;
	}

	/// @brief ���ʊ�Ŏg�p����l�K�e�B�u�T���v�����̊m�����x�֐��̊e�l�փA�N�Z�X����֐�
	//! 
	//! @param[in]  indx �c �m�����x�֐��̒l���擾����C���f�b�N�X
	//! 
	double neg( size_type indx ) const
	{
		return( neg_[ indx ] );
	}

	/// @brief �㎯�ʊ�Ŏg�p����q�X�g�O�����̃r������ύX����֐�
	//! 
	//! @param[in]  nbins �c �m�����x�֐��֐ݒ肷��l
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
	/// @brief �f�t�H���g�̃R���X�g���N�^
	//! 
	//! @param[in]  indx �c �㎯�ʊ�Ŏg�p��������ʂ̃C���f�b�N�X�iID�j
	//! @param[in]  s    �c �㎯�ʊ�̕���
	//! @param[in]  th   �c �㎯�ʊ�̂������l
	//! 
	weak_classifier( size_type indx = 0, size_type nbins = __HISTOGRAM_CLASSIFIER_NUM_BINS__ ) : base( indx ), hist_( nbins ), pos_( nbins ), neg_( nbins )
	{
	}

	/// @brief �R�s�[�R���X�g���N�^
	weak_classifier( const weak_classifier& w ) : base( w ), hist_( w.hist_ ), pos_( w.pos_ ), neg_( w.neg_ )
	{
	}

	/// @brief ���̎��ʊ�Ɠ����p�����[�^�̎��ʊ�ƂȂ�悤�Ƀf�[�^���R�s�[����
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
	/// @brief ���t�f�[�^��p���čœK�Ȏ㎯�ʊ���\�z����
	//! 
	//! @param[in]  features   �c �w�K�ɗp���鋳�t�f�[�^
	//! @param[in]  categories �c �w�K�f�[�^�̃J�e�S���itrue �������� false�j
	//! 
	template < template < typename, typename > class FEATURE_LIST, class FEATURE, class ALLOCATOR >
	double learn( const FEATURE_LIST< FEATURE, ALLOCATOR > & features )
	{
		typedef FEATURE feature_type;
		typedef typename FEATURE_LIST< FEATURE, ALLOCATOR >::const_iterator const_iterator;

		double _minimum_classification_error_ = 1.0e100;

		// Positive �� Negative ���ꂼ��̏d�݂̍��v���v�Z����
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

			// �e�������l�ł̏d�ݕt���덷���v�Z���C�덷�ŏ��̂������l�����߂�
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
	/// @brief �ǉ��̋��t�f�[�^��p���Ď㎯�ʊ���X�V����
	//! 
	//! @param[in]  f      �c �ǉ��̋��t�f�[�^
	//! @param[in]  weight �c �w�K����d��
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
	/// @brief �w�K�ς݂̎㎯�ʊ��p���ē����ʂ𕪗ނ���
	double evaluate_( double f ) const
	{
		return( hist_[ static_cast< size_type >( f ) ] );
	}
};


/// @brief AdaBoost�A���S���Y����p���ċ����ʊ���\�z����N���X
class boosted_classifier : public boosted_classifier_base< boosted_classifier, weak_classifier >
{
private:
	typedef boosted_classifier_base< boosted_classifier, weak_classifier > base;		///< @brief ���N���X

public:
	typedef feature::value_type value_type;												///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�Dmist::array< data > �� data �Ɠ���
	typedef feature::size_type size_type;												///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef feature::difference_type difference_type;									///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���
	typedef weak_classifier base_classifier_type;										///< @brief AdaBoost�ŗ��p����㎯�ʊ�
	typedef std::vector< base_classifier_type > classifier_list_type;					///< @brief AdaBoost�ŗ��p����㎯�ʊ�̃��X�g

public:
	/// @brief �f�t�H���g�̃R���X�g���N�^
	boosted_classifier( ) : base( )
	{
	}

	/// @brief �R�s�[�R���X�g���N�^
	boosted_classifier( const boosted_classifier &b ) : base( b )
	{
	}

	/// @brief �f�X�g���N�^
	~boosted_classifier( ){}

	/// @brief ����AdaBoost���ʊ�̏����R�s�[����
	boosted_classifier& operator =( const boosted_classifier &o )
	{
		if( this != &o )
		{
			base::operator =( o );
		}

		return( *this );
	}

public:
	/// @brief ���ʊ�̏���ۑ�����XML�t�@�C������ݒ�𕜌�����
	//! 
	//! @param[in] filename �c ���ʊ�̏���ۑ�����XML�t�@�C����
	//! 
	virtual bool load( const std::string &filename );

	/// @brief ���ʊ�̏���XML�`���Ńt�@�C���ɕۑ�����
	//! 
	//! @param[in] filename �c ���ʊ�̏���ۑ�����XML�t�@�C����
	//! 
	virtual bool save( const std::string &filename, const std::string &comment = "" ) const;

public:
	/// @brief ���݂̋����ʊ�Ɏ㎯�ʊ��ǉ�����
	//! 
	//! @param[in,out]  features                   �c �w�K�ɗp���鋳�t�f�[�^
	//! @param[in]      num_max_base_classifiers   �c �����ʊ�֒ǉ�����㎯�ʊ�̐��i�f�t�H���g��100�j
	//! @param[in]      min_true_positive_rate     �c �����ʊ�̍\�z���ɖ������ׂ�TP�̍ŏ��l�i�f�t�H���g��-1�j�D���̒l���w�肵���ꍇ�� num_max_base_classifiers �܂Ŏ㎯�ʊ��ǉ�����D
	//! @param[in]      max_false_positive_rate    �c �����ʊ�̍\�z���ɖ������ׂ�FP�̍ő�l�i�f�t�H���g��-1�j�D���̒l���w�肵���ꍇ�� num_max_base_classifiers �܂Ŏ㎯�ʊ��ǉ�����D
	//! @param[in]      _output_debug_string_      �c �w�K�̐i���󋵂��o�͂��邩�ǂ����i�f�t�H���g��false�j
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

		// �S�̂̂������l���Čv�Z����
		threshold_ = 0.0;

		// AdaBoost �ɂ�苭���ʊ���w�K����
		for( size_type t = 0 ; t < num_max_base_classifiers ; t++ )
		{
			// �d�݂𐳋K������
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

			// �㎯�ʊ���w�K����
			classifiers_.push_back( base_classifier_type( ) );
			base_classifier_type &weak = classifiers_.back( );
			weak.learn( features );

			//�㎯�ʊ�̏d�݂�1�ŌŒ�;
			weak.weight( 1.0 );

			// �d�݂��X�V����
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

				// �R���\�[���Ɋw�K�󋵂��o�͂���
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

	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ𕪗ނ���
	//! 
	//! @param[in]  f �c ���ނ��������
	//! 
	//! @return ���ތ���
	//! 
	template < class FEATURE >
	double evaluate( const FEATURE &f ) const
	{
		return( this->confidence( f ) );
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
		return( this->confidence( f, p ) );
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
		return( this->confidence( f, p1, p2 ) );
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
		return( this->confidence( f, p1, p2, p3 ) );
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
		return( this->confidence( f, p1, p2, p3, p4 ) );
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
		return( this->confidence( f, p1, p2, p3, p4, p5 ) );
	}

	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ̐M���x���v�Z����
	//! 
	//! @param[in]  f �c ���ނ��������
	//! 
	//! @return ���ތ���
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
		double value = 0.0;

		for( size_type t = 0 ; t < classifiers_.size( ) ; t++ )
		{
			const base_classifier_type &weak = classifiers_[ t ];
			value += weak.evaluate( f, p ) * weak.weight( );
		}

		return( value - threshold_ );
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
		double value = 0.0;

		for( size_type t = 0 ; t < classifiers_.size( ) ; t++ )
		{
			const base_classifier_type &weak = classifiers_[ t ];
			value += weak.evaluate( f, p1, p2 ) * weak.weight( );
		}

		return( value - threshold_ );
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
		double value = 0.0;

		for( size_type t = 0 ; t < classifiers_.size( ) ; t++ )
		{
			const base_classifier_type &weak = classifiers_[ t ];
			value += weak.evaluate( f, p1, p2, p3 ) * weak.weight( );
		}

		return( value - threshold_ );
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
		double value = 0.0;

		for( size_type t = 0 ; t < classifiers_.size( ) ; t++ )
		{
			const base_classifier_type &weak = classifiers_[ t ];
			value += weak.evaluate( f, p1, p2, p3, p4 ) * weak.weight( );
		}

		return( value - threshold_ );
	}

	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ̐M���x���v�Z����
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


/// @brief �J�X�P�[�h�^AdaBoost���ʊ���\�z����N���X
class cascaded_classifier : public cascaded_classifier_base< cascaded_classifier, boosted_classifier >
{
private:
	typedef cascaded_classifier_base< cascaded_classifier, boosted_classifier > base;		///< @brief ���N���X

public:
	typedef boosted_classifier base_classifier_type;										///< @brief �J�X�P�[�h�ɂ��鎯�ʊ�
	typedef std::vector< base_classifier_type > classifier_list_type;						///< @brief �J�X�P�[�h�ɂ��鎯�ʊ�̃��X�g
	typedef feature::value_type value_type;													///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�Dmist::array< data > �� data �Ɠ���
	typedef feature::size_type size_type;													///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef feature::difference_type difference_type;										///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���

public:
	/// @brief �f�t�H���g�̃R���X�g���N�^
	cascaded_classifier( ) : base( )
	{
	}

	/// @brief �R�s�[�R���X�g���N�^
	cascaded_classifier( const cascaded_classifier &c ) : base( c )
	{
	}

	/// @brief �f�X�g���N�^
	~cascaded_classifier( ){ }

	/// @brief ���̃J�X�P�[�h�^AdaBoost���ʊ�̏����R�s�[����
	cascaded_classifier& operator =( const cascaded_classifier& o )
	{
		if( this != &o )
		{
			base::operator =( o );
		}

		return( *this );
	}

public:
	/// @brief ���ʊ�̏���ۑ�����XML�t�@�C������ݒ�𕜌�����
	//! 
	//! @param[in] filename �c ���ʊ�̏���ۑ�����XML�t�@�C����
	//! 
	virtual bool load( const std::string &filename );

	/// @brief ���ʊ�̏���XML�`���Ńt�@�C���ɕۑ�����
	//! 
	//! @param[in] filename �c ���ʊ�̏���ۑ�����XML�t�@�C����
	//! 
	virtual bool save( const std::string &filename, const std::string &comment = "" ) const;

#if defined( __ADABOOST_CASCADE_TYPE__ ) && ( __ADABOOST_CASCADE_TYPE__ == 2 || __ADABOOST_CASCADE_TYPE__ == 3 )
public:
	/// @brief �e�i�̋����ʊ�̏��������s���֐��i�����p�j
	//! 
	//! @param[in,out] features   �c �w�K�ɗp���鋳�t�f�[�^
	//! @param[in,out] classifier �c ���������鋭���ʊ�
	//! 
	template < template < typename, typename > class FEATURE_LIST, class FEATURE, class ALLOCATOR >
	void initialize__( FEATURE_LIST< FEATURE, ALLOCATOR > & features, base_classifier_type & classifier )
	{
		classifier.initialize( features );

		if( base::number_of_stages( ) > 0 )
		{
			typedef typename FEATURE_LIST< FEATURE, ALLOCATOR >::iterator iterator;
			typedef FEATURE feature_type;

			// �d�݂��X�V����
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
#endif

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

	/// @brief �w�K�ς݂̎��ʊ��p���ē����ʂ̐M���x���v�Z����
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


/// @brief Multi-exit�^AdaBoost���ʊ���\�z����N���X
class multi_exit_classifier : public multi_exit_classifier_base< multi_exit_classifier, weak_classifier >
{
private:
	typedef multi_exit_classifier_base< multi_exit_classifier, weak_classifier > base;			///< @brief ���N���X

public:
	typedef weak_classifier base_classifier_type;												///< @brief Multi-exit�^�J�X�P�[�h�ɂ��鎯�ʊ�
	typedef base::classifier_list_type classifier_list_type;									///< @brief Multi-exit�^�J�X�P�[�h�ɂ��鎯�ʊ�̃��X�g
	typedef base::exit_list_type exit_list_type;												///< @brief Multi-exit�^�J�X�P�[�h�𔲂��鎯�ʊ�̃C���f�b�N�X��ێ����郊�X�g
	typedef feature::value_type value_type;														///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�Dmist::array< data > �� data �Ɠ���
	typedef feature::size_type size_type;														///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef feature::difference_type difference_type;											///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���

public:
	/// @brief �f�t�H���g�̃R���X�g���N�^
	multi_exit_classifier( ) : base( )
	{
	}

	/// @brief �R�s�[�R���X�g���N�^
	multi_exit_classifier( const multi_exit_classifier &c ) : base( c )
	{
	}

	/// @brief �f�X�g���N�^
	~multi_exit_classifier( ){ }

	/// @brief ����Multi-exit�^AdaBoost���ʊ�̏����R�s�[����
	multi_exit_classifier& operator =( const multi_exit_classifier& o )
	{
		if( this != &o )
		{
			base::operator =( o );
		}

		return( *this );
	}

public:
	/// @brief ���ʊ�̏���ۑ�����XML�t�@�C������ݒ�𕜌�����
	//! 
	//! @param[in] filename �c ���ʊ�̏���ۑ�����XML�t�@�C����
	//! 
	virtual bool load( const std::string &filename );

	/// @brief ���ʊ�̏���XML�`���Ńt�@�C���ɕۑ�����
	//! 
	//! @param[in] filename �c ���ʊ�̏���ۑ�����XML�t�@�C����
	//! 
	virtual bool save( const std::string &filename, const std::string &comment = "" ) const;

public:
	/// @brief ���݂̋����ʊ�Ɏ㎯�ʊ��ǉ�����
	//! 
	//! @param[in,out]  features                 �c �w�K�ɗp���鋳�t�f�[�^
	//! @param[in]      num_max_base_classifiers �c �����ʊ�֒ǉ�����㎯�ʊ�̐��i�f�t�H���g��100�j
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

		// AdaBoost �ɂ�苭���ʊ���w�K����
		for( size_type t = 0 ; t < num_max_base_classifiers ; t++ )
		{
			// �d�݂𐳋K������
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

			// �㎯�ʊ���w�K����
			base::classifiers( ).push_back( base_classifier_type( ) );
			base_classifier_type &weak = base::classifiers( ).back( );
			weak.learn( features );

			//�㎯�ʊ�̏d�݂�1�ŌŒ�;
			weak.weight( 1.0 );

			// �d�݂��X�V����
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


/// @brief AdaBoost���ʊ�ƃJ�X�P�[�h�^AdaBoost���ʊ�̏���XML�`���œ��o�͂���֐��Q�i�����p�j
namespace io
{
	/// @brief libxml2��API�𗘗p���ă^�O�̑������擾����֐�
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

	/// @brief libxml2��API�𗘗p���ă^�O�̑������擾����֐�
	inline std::string get_attribute( xmlTextReaderPtr reader, const std::string &attr, const std::string &default_value = "" )
	{
		// �m�[�h�̃A�g���r���[�g�����m�[�h���擾����
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

	/// @brief libxml2��API�𗘗p���Ď㎯�ʊ�̐ݒ���擾����֐�
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

	/// @brief libxml2��API�𗘗p���Ď㎯�ʊ�̐ݒ��ۑ�����֐�
	inline int write( xmlTextWriterPtr writer, const weak_classifier &weak, weak_classifier::difference_type id, size_t tag_depth )
	{
		// �㎯�ʊ�̊J�n�^�O
		if( xmlTextWriterStartElement( writer, BAD_CAST "weak" ) < 0 )
		{
			std::cerr << "Failed to write <weak> tag." << std::endl;
			return( -1 );
		}

		// ���ʊ��ID���o�͂���
		if( id >= 0 )
		{
			if( xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "id", "%ld", id ) < 0 )
			{
				std::cerr << "Failed to add a property to XML tag." << std::endl;
				return( -1 );
			}
		}

		// �㎯�ʊ�Ŏg�p��������ʂ�ID
		if( xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "index", "%ld", weak.index( ) ) < 0 )
		{
			std::cerr << "Failed to add a property to XML tag." << std::endl;
			return( -1 );
		}

		// �㎯�ʊ�Ŏg�p��������ʂ̃r����
		if( xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "nbins", "%ld", weak.size( ) ) < 0 )
		{
			std::cerr << "Failed to add a property to XML tag." << std::endl;
			return( -1 );
		}

		// �����ʊ���\�z����ۂɎg�p����㎯�ʊ�̏d��
		if( xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "weight", "%f", weak.weight( ) ) < 0 )
		{
			std::cerr << "Failed to add a property to XML tag." << std::endl;
			return( -1 );
		}

		// �㎯�ʊ���I�����C���X�V����ۂ̏d�݁i���������ނ��ꂽ���́j
		if( xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "lambda_c", "%f", weak.lambda_c( ) ) < 0 )
		{
			std::cerr << "Failed to add a property to XML tag." << std::endl;
			return( -1 );
		}

		// �㎯�ʊ���I�����C���X�V����ۂ̏d�݁i����ĕ��ނ��ꂽ���́j
		if( xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "lambda_w", "%f", weak.lambda_w( ) ) < 0 )
		{
			std::cerr << "Failed to add a property to XML tag." << std::endl;
			return( -1 );
		}

		// �R�����g����ݒ肷��
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

		// �㎯�ʊ�̊m�����x�֐��̒l��ۑ�����
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

		// �㎯�ʊ�̏I���^�O
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
// AdaBoost���O��Ԃ̏I���
_ADABOOST_END
#endif

#endif	// __INCLUDE_REAL_ADABOOST_CLASSIFIER__