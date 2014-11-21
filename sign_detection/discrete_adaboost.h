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

#ifndef __INCLUDE_DISCRETE_ADABOOST_CLASSIFIER__
#define __INCLUDE_DISCRETE_ADABOOST_CLASSIFIER__

#include <vector>
#include <list>
#include <algorithm>


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
	double sign_;		///< @brief �������l�̕���
	double threshold_;	///< @brief ���ނɗp���邵�����l

public:
	/// @brief �㎯�ʊ�̎��ʋ��E�����肷��ۂɎg�p���镄�����擾����
	double sign( ) const { return( sign_ ); }
	/// @brief �㎯�ʊ�̎��ʋ��E�����肷��ۂɎg�p���镄����ݒ肷��
	void sign( double s ){ sign_ = s; }

	/// @brief �㎯�ʊ�̎��ʋ��E�����肷��ۂɎg�p���邵�����l���擾����
	double threshold( ) const { return( threshold_ ); }
	/// @brief �㎯�ʊ�̎��ʋ��E�����肷��ۂɎg�p���邵�����l��ݒ肷��
	void threshold( double t ){ threshold_ = t; }

	/// @brief �㎯�ʊ�Ŏg�p����q�X�g�O�����̃r������ύX����֐�
	//! 
	//! @param[in]  nbins �c �m�����x�֐��֐ݒ肷��l
	//! 
	void resize( size_type /* nbins */ ){}

public:
	/// @brief �f�t�H���g�̃R���X�g���N�^
	//! 
	//! @param[in]  indx �c �㎯�ʊ�Ŏg�p��������ʂ̃C���f�b�N�X�iID�j
	//! @param[in]  s    �c �㎯�ʊ�̕���
	//! @param[in]  th   �c �㎯�ʊ�̂������l
	//! 
	weak_classifier( size_type indx = 0, double s = 1.0, double th = 0.0 ) : base( indx ), sign_( s ), threshold_( th )
	{
	}

	/// @brief �R�s�[�R���X�g���N�^
	weak_classifier( const weak_classifier& w ) : base( w ), sign_( w.sign_ ), threshold_( w.threshold_ )
	{
	}

	/// @brief ���̎��ʊ�Ɠ����p�����[�^�̎��ʊ�ƂȂ�悤�Ƀf�[�^���R�s�[����
	weak_classifier& operator =( const weak_classifier& o )
	{
		if( this != &o )
		{
			base::operator =( o );

			sign_      = o.sign_;
			threshold_ = o.threshold_;
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
		double overall_sum_of_positive_weights = 0.0;
		double overall_sum_of_negative_weights = 0.0;
		size_type nfeatures = 0;
		for( const_iterator ite = features.begin( ) ; ite != features.end( ) ; ++ite )
		{
			const feature_type &f = *ite;
			if( f.is_valid( ) )
			{
				if( f.is_positive( ) )
				{
					overall_sum_of_positive_weights += f.weight( );
				}
				else
				{
					overall_sum_of_negative_weights += f.weight( );
				}

				if( nfeatures < f.number_of_dimentions( ) )
				{
					nfeatures = f.number_of_dimentions( );
				}
			}
		}

#if defined( __USE_FAST_WEAK_LEARNER__ ) && __USE_FAST_WEAK_LEARNER__ != 0
		double max_sigma = -1.0;

		#pragma omp parallel for firstprivate( nfeatures ) schedule( guided ) shared( _minimum_classification_error_ )
		for( int index = 0 ; index < ( int )nfeatures ; index++ )
		{
			double pos[ __HISTOGRAM_CLASSIFIER_NUM_BINS__ ];
			double neg[ __HISTOGRAM_CLASSIFIER_NUM_BINS__ ];
			for( size_type i = 0 ; i < __HISTOGRAM_CLASSIFIER_NUM_BINS__ ; i++ )
			{
				pos[ i ] = 0;
				neg[ i ] = 0;
			}

			for( const_iterator ite = features.begin( ) ; ite != features.end( ) ; ++ite )
			{
				const feature_type &f = *ite;
				if( f.is_valid( ) )
				{
					if( f.is_positive( ) )
					{
						pos[ static_cast< size_type >( f[ index ] ) ] += f.weight( );
					}
					else
					{
						neg[ static_cast< size_type >( f[ index ] ) ] += f.weight( );
					}
				}
			}

			// �e�������l�ł̏d�ݕt���덷���v�Z���C�덷�ŏ��̂������l�����߂�
			double min_error = 1.0e100, sgn = 1.0, th = 0.0;
			double sum_of_positive_weights = 0.0;
			double sum_of_negative_weights = 0.0;

			for( size_type i = 0 ; i < __HISTOGRAM_CLASSIFIER_NUM_BINS__ ; i++ )
			{
				double e1 = sum_of_positive_weights + overall_sum_of_negative_weights - sum_of_negative_weights;
				double e2 = sum_of_negative_weights + neg[ i ] + overall_sum_of_positive_weights - sum_of_positive_weights - pos[ i ];
				double error = e1 < e2 ? e1 : e2;

				if( error <= min_error )
				{
					min_error = error;
					th        = static_cast< double >( i );
					sgn       = e1 < e2 ? -1.0 : 1.0;
				}

				sum_of_positive_weights += pos[ i ];
				sum_of_negative_weights += neg[ i ];
			}

			double sigma = 0.0;
			{
				double M1 = 0.0;
				double M2 = 0.0;
				double S1 = 0.0;
				double S2 = 0.0;

				for( size_type i = 0 ; i < __HISTOGRAM_CLASSIFIER_NUM_BINS__ ; i++ )
				{
					M1 += i * pos[ i ];
					M2 += i * neg[ i ];
				}

				M1 /= overall_sum_of_positive_weights;
				M2 /= overall_sum_of_negative_weights;

				for( size_type i = 0 ; i < __HISTOGRAM_CLASSIFIER_NUM_BINS__ ; i++ )
				{
					S1 += pos[ i ] * ( i - M1 ) * ( i - M1 );
					S2 += neg[ i ] * ( i - M2 ) * ( i - M2 );
				}

				S1 /= overall_sum_of_positive_weights;
				S2 /= overall_sum_of_negative_weights;
				double V1 = overall_sum_of_positive_weights * overall_sum_of_negative_weights * ( M1 - M2 ) * ( M1 - M2 );
				double V2 = ( overall_sum_of_positive_weights + overall_sum_of_negative_weights ) * ( overall_sum_of_positive_weights * S1 + overall_sum_of_negative_weights * S2 );
				sigma = V1 / ( 1.0 + V2 );
			}

			#pragma omp critical
			{
				#pragma omp flush( _minimum_classification_error_ )
				if( _minimum_classification_error_ > min_error )
				{
					_minimum_classification_error_ = min_error;
					index_ = index;
					sign_ = sgn;
					threshold_ = th;
					max_sigma = sigma;
				}
				else if( _minimum_classification_error_ == min_error && sigma > max_sigma )
				{
					_minimum_classification_error_ = min_error;
					index_ = index;
					sign_ = sgn;
					threshold_ = th;
					max_sigma = sigma;
				}
			}
		}
#else
		double max_sigma = -1.0;

		// �����ʂ̃��X�g���쐬����
		#pragma omp parallel for firstprivate( nfeatures ) schedule( guided )
		for( int index = 0 ; index < ( int )nfeatures ; index++ )
		{
			std::vector< feature_one > flist;
			flist.reserve( features.size( ) );
			for( const_iterator ite = features.begin( ) ; ite != features.end( ) ; ++ite )
			{
				const feature_type &f = *ite;
				if( f.is_valid( ) )
				{
					flist.push_back( feature_one( f[ index ], f.flag( ), f.weight( ) ) );
				}
			}

			// �����ʂ��\�[�g����
			std::sort( flist.begin( ), flist.end( ) );

			// �e�������l�ł̏d�ݕt���덷���v�Z���C�덷�ŏ��̂������l�����߂�
			double min_error = 1.0e100, sgn = 1.0, th = 0.0;
			double sum_of_positive_weights = 0.0;
			double sum_of_negative_weights = 0.0;

			for( size_type i = 0 ; i < flist.size( ) ; i++ )
			{
				const feature_one &f = flist[ i ];
				if( f.is_positive( ) )
				{
					sum_of_positive_weights += f.weight( );
				}
				else
				{
					sum_of_negative_weights += f.weight( );
				}

				double e1 = sum_of_positive_weights + overall_sum_of_negative_weights - sum_of_negative_weights;
				double e2 = sum_of_negative_weights + overall_sum_of_positive_weights - sum_of_positive_weights;
				double error = e1 < e2 ? e1 : e2;

				if( error <= min_error )
				{
					min_error = error;
					th        = f.value( );
					sgn       = e1 < e2 ? -1.0 : 1.0;

					if( 0 < i && i < flist.size( ) - 2 )
					{
						double v1 = std::abs( flist[ i - 1 ].value( ) - flist[ i + 0 ].value( ) );
						double v2 = std::abs( flist[ i + 1 ].value( ) - flist[ i + 2 ].value( ) );
						if( std::abs( v1 + v2 ) > 0 )
						{
							double t1 = flist[ i + 0 ].value( );
							double t2 = flist[ i + 1 ].value( );
							th = ( t1 * v1 + t2 * v2 ) / ( v1 + v2 );
						}
					}
					else if( i < flist.size( ) - 1 )
					{
						th = ( th + flist[ i + 1 ].value( ) ) * 0.5;
					}
				}
			}

			double e = 0.0;
			for( const_iterator ite = features.begin( ) ; ite != features.end( ) ; ++ite )
			{
				const feature_type &f = *ite;
				if( f.is_valid( ) )
				{
					if( evaluate_( f, index, sgn, th ) != f.is_positive( ) )
					{
						e += f.weight( );
					}
				}
			}

			double sigma = 0.0;
			{
				double M1 = 0.0;
				double M2 = 0.0;
				double S1 = 0.0;
				double S2 = 0.0;

				for( size_type i = 0 ; i < flist.size( ) ; i++ )
				{
					const feature_one &f = flist[ i ];
					if( f.is_positive( ) )
					{
						M1 += f.weight( ) * f.value( );
					}
					else
					{
						M2 += f.weight( ) * f.value( );
					}
				}

				M1 /= overall_sum_of_positive_weights;
				M2 /= overall_sum_of_negative_weights;

				for( size_type i = 0 ; i < flist.size( ) ; i++ )
				{
					const feature_one &f = flist[ i ];
					if( f.is_positive( ) )
					{
						S1 += f.weight( ) * ( f.value( ) - M1 ) * ( f.value( ) - M1 );
					}
					else
					{
						S2 += f.weight( ) * ( f.value( ) - M2 ) * ( f.value( ) - M2 );
					}
				}

				S1 /= overall_sum_of_positive_weights;
				S2 /= overall_sum_of_negative_weights;
				double V1 = overall_sum_of_positive_weights * overall_sum_of_negative_weights * ( M1 - M2 ) * ( M1 - M2 );
				double V2 = ( overall_sum_of_positive_weights + overall_sum_of_negative_weights ) * ( overall_sum_of_positive_weights * S1 + overall_sum_of_negative_weights * S2 );
				sigma = V1 / ( 1.0 + V2 );
			}

			#pragma omp critical
			{
				if( _minimum_classification_error_ > e )
				{
					_minimum_classification_error_ = e;
					index_ = index;
					sign_ = sgn;
					threshold_ = th;
					max_sigma = sigma;
				}
				else if( _minimum_classification_error_ == e && sigma > max_sigma )
				{
					_minimum_classification_error_ = e;
					index_ = index;
					sign_ = sgn;
					threshold_ = th;
					max_sigma = sigma;
				}
			}
		}
#endif

		return( _minimum_classification_error_ );
	}

public:
	/// @brief �w�K�ς݂̎㎯�ʊ��p���ē����ʂ𕪗ނ���
	double evaluate_( double f ) const
	{
		return( sign_ * f <= sign_ * threshold_ ? weight( ) : -weight( ) );
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
		for( size_type i = 0 ; i < classifiers_.size( ) ; i++ )
		{
			threshold_ += classifiers_[ i ].weight( ) * 0.5;
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
			classifiers_.push_back( base_classifier_type( ) );
			base_classifier_type &weak = classifiers_.back( );
			double error = weak.learn( features );

			const double eps = 10e-8;
			double beta = error / ( 1.0 - error );
			weak.weight( error < eps ? 1.0 : std::log( 1.0 / beta ) );

			// �S�̂̂������l���v�Z����
			threshold_ += weak.weight( ) * 0.5;

			// �덷���\���������ꍇ�͏I��
			if( error < eps )
			{
				break;
			}

			// �d�݂��X�V����
			for( iterator ite = features.begin( ) ; ite != features.end( ) ; ++ite )
			{
				feature_type &f = *ite;
				if( f.is_valid( ) )
				{
					if( f.is_positive( ) == ( weak.evaluate( f ) > 0 ) )
					{
						f.weight( f.weight( ) * beta );
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
						printf( "DEBUG : Weaks = %2ld, TP = %6.2f, FP = %6.2f [%s]\n", classifiers_.size( ), TP * 100.0, FP * 100.0, info.c_str( ) );
					}
					else
					{
						printf( "DEBUG : Weaks = %2ld, TP = %6.2f, FP = %6.2f\n", classifiers_.size( ), TP * 100.0, FP * 100.0 );
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
			classifiers_.push_back( base_classifier_type( ) );
			base_classifier_type &weak = classifiers_.back( );
			double error = weak.learn( features );

			const double eps = 10e-8;
			double cm = 0.5 * std::log( ( 1.0 - error ) / error );
			weak.weight( cm );

			// �덷���\���������ꍇ�͏I��
			if( error < eps )
			{
				break;
			}

			// �d�݂��X�V����
			for( iterator ite = features.begin( ) ; ite != features.end( ) ; ++ite )
			{
				feature_type &f = *ite;
				if( f.is_valid( ) )
				{
					f.weight( f.weight( ) * std::exp( - ( f.is_positive( ) ? +1 : -1 ) * weak.evaluate( f ) ) );
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
		int    weakID    = utility::ToInt( get_attribute( reader, "id", "-1" ) );
		int    featureID = utility::ToInt( get_attribute( reader, "index", "-1" ) );
		double sign      = utility::ToDouble( get_attribute( reader, "sign", "+1.0" ) );
		double thredhold = utility::ToDouble( get_attribute( reader, "threshold", "0.0" ) );
		double weight    = utility::ToDouble( get_attribute( reader, "weight", "0.0" ) );

		weak.index( featureID );
		weak.sign( sign );
		weak.threshold( thredhold );
		weak.weight( weight );

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

		// �㎯�ʊ�̔��ʋ��E�����肷�镄��
		if( xmlTextWriterWriteAttribute( writer, BAD_CAST "sign", BAD_CAST ( weak.sign( ) >= 0 ? "+1" : "-1" ) ) < 0 )
		{
			std::cerr << "Failed to add a property to XML tag." << std::endl;
			return( -1 );
		}

		// �㎯�ʊ�̔��ʋ��E�����肷�邵�����l
		if( xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "threshold", "%f", weak.threshold( ) ) < 0 )
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

		if( xmlTextWriterWriteString( writer, BAD_CAST indent.c_str( ) ) < 0 )
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
	return( read( *this, filename, "discrete", "" ) );
}

bool boosted_classifier::save( const std::string &filename, const std::string &comment ) const
{
	return( write( *this, filename, "discrete", comment ) );
}

bool cascaded_classifier::load( const std::string &filename )
{
#if defined( __ADABOOST_CASCADE_TYPE__ ) && __ADABOOST_CASCADE_TYPE__ == 2
	return( read_cascade( *this, filename, "discrete", "nested" ) );
#else
	return( read_cascade( *this, filename, "discrete", "viola" ) );
#endif
}

bool cascaded_classifier::save( const std::string &filename, const std::string &comment ) const
{
	return( write( *this, filename, "discrete", comment ) );
}

bool multi_exit_classifier::load( const std::string &filename )
{
	return( read_cascade( *this, filename, "discrete", "multi-exit" ) );
}

bool multi_exit_classifier::save( const std::string &filename, const std::string &comment ) const
{
	return( write( *this, filename, "discrete", comment ) );
}


#if !defined( __NO_ADABOOST_NAMESPACE__ ) || __NO_ADABOOST_NAMESPACE__ == 0
// AdaBoost���O��Ԃ̏I���
_ADABOOST_END
#endif

#endif	// __INCLUDE_DISCRETE_ADABOOST_CLASSIFIER__