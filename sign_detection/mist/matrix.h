/****************************************************************************************************************************
**
**         MIST ( Media Integration Standard Toolkit )
**
**         matrix template class implementation using expression template technique.
**
**
**           All matrix operations are described in row order.
**           All matrix elements are allocated as one dimensional array on the memory space.
**
**             ex) The order of 3x3 matrix are
**
**                   1  4  7
**                   2  5  8
**                   3  6  9
**
**
**         Please use matrix( row, col ) accesses operation. 
**
**
**
**         We developed these programs since 2003/09/05.
**
**             $LastChangedDate:: 2011-04-26 13:10:24 #$
**             $LastChangedRevision: 1365 $
**             $LastChangedBy: ddeguchi $
**             $HeadURL: https://mist.murase.m.is.nagoya-u.ac.jp/svn/mist/trunk/mist/matrix.h $
**
**              Copyright ***********************.
**                    All Rights Reserved.
**
****************************************************************************************************************************/

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



/// @file mist/matrix.h
//!
//! @brief ��ʍs����������߂̃N���X
//!
#ifndef __INCLUDE_MIST_MATRIX__
#define __INCLUDE_MIST_MATRIX__

#include <complex>

#ifndef __INCLUDE_MIST_H__
#include "mist.h"
#endif

#ifdef _OPENMP
#include <omp.h>
#endif

#include <cmath>


// mist���O��Ԃ̎n�܂�
_MIST_BEGIN



namespace __numeric__
{
	template < class T >
	struct is_complex
	{
		_MIST_CONST( bool, value, false );
	};

#if defined(__MIST_MSVC__) && __MIST_MSVC__ < 7

	#define IS_COMPLEX( type ) \
		template < >\
	struct is_complex< std::complex< type > >\
		{\
		enum{ value = true };\
		};\

		// �e�^�ɑ΂�����ꉻ
		IS_COMPLEX(unsigned char)
		IS_COMPLEX(unsigned short)
		IS_COMPLEX(unsigned int)
		IS_COMPLEX(unsigned long)
		IS_COMPLEX(signed char)
		IS_COMPLEX(signed short)
		IS_COMPLEX(signed int)
		IS_COMPLEX(signed long)
		IS_COMPLEX(bool)
		IS_COMPLEX(char)
		IS_COMPLEX(float)
		IS_COMPLEX(double)
		IS_COMPLEX(long double)

	#undef IS_COLOR

#else

	template < class T >
	struct is_complex< std::complex< T > >
	{
		_MIST_CONST( bool, value, true );
	};

#endif

	template< bool b >
	struct value_compare
	{
		template < class T > static bool le( const T &v1, const T &v2 ){ return( v1 <= v2 ); }
		template < class T > static bool ge( const T &v1, const T &v2 ){ return( v1 >= v2 ); }
		template < class T > static bool lt( const T &v1, const T &v2 ){ return( v1 <  v2 ); }
		template < class T > static bool gt( const T &v1, const T &v2 ){ return( v1 >  v2 ); }
		template < class T > static bool eq( const T &v1, const T &v2 ){ return( v1 == v2 ); }
		template < class T > static bool eq( const T &v1, const T &v2, const double delta ){ return( std::abs( v1 - v2 ) < delta ); }
		template < class T > static bool is_zero( const T &v ){ return( v == T( ) ); }
	};

	template< >
	struct value_compare< true >
	{
		template < class T >
		static bool lt( const T &v1, const T &v2 )
		{
			if( v1.real( ) < v2.real( ) )
			{
				return( true );
			}
			else if( v1.real( ) > v2.real( ) )
			{
				return( false );
			}
			else if( v1.imag( ) < v2.imag( ) )
			{
				return( true );
			}
			else
			{
				return( false );
			}
		}
		template < class T > static bool ge( const T &v1, const T &v2 ){ return( !lt( v1, v2 ) ); }
		template < class T > static bool le( const T &v1, const T &v2 ){ return( !lt( v2, v1 ) ); }
		template < class T > static bool gt( const T &v1, const T &v2 ){ return( lt( v2, v1 ) ); }
		template < class T > static bool eq( const T &v1, const T &v2 )
		{
			return( v1.real( ) == v2.real( ) && v1.imag( ) == v2.imag( ) );
		}
		template < class T > static bool eq( const T &v1, const T &v2, const double delta )
		{
			return( std::abs( v1.real( ) - v2.real( ) ) < delta && std::abs( v1.imag( ) - v2.imag( ) ) < delta );
		}
		template < class T > static bool is_zero( const T &v ){ return( v.real( ) == 0 && v.imag( ) == 0 ); }
	};

	// ���𕡑f����Ԃ��֐�
	template< class T > std::complex< T > conjugate( const std::complex< T > &c ) { return std::conj( c ); }
	template< class T > const T &conjugate( const T &c ) { return c; }
}


/// @brief �s�Ɨ�̗v�f�����ς̍s��
//! 
//! - _CHECK_MATRIX_OPERATION_  �}�N����1�̏ꍇ�́C�s�񉉎Z�̍��ӂƉE�ӂōs��̃T�C�Y���K�؂ł��邩���`�F�b�N����
//! - _CHECK_ACCESS_VIOLATION_  �}�N����1�̏ꍇ�́C�s��̗v�f�փA�N�Z�X����ۂɁC�͈͊O���ǂ����𔻒肷��
//! 
//! @param T         �c �s����Ɋi�[����f�[�^�^
//! @param Allocator �c MIST�R���e�i�����p����A���P�[�^�^�D�ȗ������ꍇ�́CSTL�̃f�t�H���g�A���P�[�^���g�p����
//! 
template < class T, class Allocator = ::std::allocator< T > >
class matrix : public array< T, Allocator >
{
public:
	typedef Allocator allocator_type;								///< @brief MIST�R���e�i�����p����A���P�[�^�^
	typedef typename Allocator::reference reference;				///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�̎Q�ƁDmist::matrix< data > �̏ꍇ�Cdata & �ƂȂ�
	typedef typename Allocator::const_reference const_reference;	///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�� const �Q�ƁDmist::matrix< data > �̏ꍇ�Cconst data & �ƂȂ�
	typedef typename Allocator::value_type value_type;				///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�Dmist::matrix< data > �� data �Ɠ���
	typedef typename Allocator::size_type size_type;				///< @brief �����Ȃ��̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� size_t �^�Ɠ���
	typedef typename Allocator::difference_type difference_type;	///< @brief �����t���̐�����\���^�D�R���e�i���̗v�f����C�e�v�f���w�肷��Ƃ��Ȃǂɗ��p���C�����I�ɂ� ptrdiff_t �^�Ɠ���
	typedef typename Allocator::pointer pointer;					///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�̃|�C���^�[�^�Dmist::matrix< data > �̏ꍇ�Cdata * �ƂȂ�
	typedef typename Allocator::const_pointer const_pointer;		///< @brief MIST�̃R���e�i���Ɋi�[����f�[�^�^�� const �|�C���^�[�^�Dmist::matrix< data > �̏ꍇ�Cconst data * �ƂȂ�

	/// @brief MIST�̃R���e�i���𑀍삷��CSTL�Ō����������̃����_���A�N�Z�X�C�e���[�^
	typedef mist_iterator1< T, ptrdiff_t, pointer, reference > iterator;

	/// @brief MIST�̃R���e�i���𑀍삷��C�������̃����_���A�N�Z�X�C�e���[�^�̃R���X�g��
	typedef mist_iterator1< T, ptrdiff_t, const_pointer, const_reference > const_iterator;

	/// @brief MIST�̃R���e�i���𑀍삷��C�t�����̃����_���A�N�Z�X�C�e���[�^
	typedef mist_reverse_iterator< mist_iterator1< T, ptrdiff_t, pointer, reference > > reverse_iterator;

	/// @brief MIST�̃R���e�i���𑀍삷��C�t�����̃����_���A�N�Z�X�C�e���[�^�̃R���X�g��
	typedef mist_reverse_iterator< mist_iterator1< T, ptrdiff_t, const_pointer, const_reference > > const_reverse_iterator;

	/// @brief �f�[�^�^�̕ϊ����s��
	template < class TT, class AAllocator = std::allocator< TT > > 
	struct rebind
	{
		typedef matrix< TT, AAllocator > other;
	};

private:
	typedef array< T, Allocator > base;
	size_type size2_;
	size_type size1_;

private:
	// rows �� cols �Ƃ̍���������邽�߁Cmatrix �ł͎g���Ȃ��悤�ɂ���
	size_type width( ) const;	///< @brief X�������̃R���e�i�Ɋi�[����Ă���f�[�^����Ԃ�
	size_type height( ) const;	///< @brief Y�������̃R���e�i�Ɋi�[����Ă���f�[�^����Ԃ��i���1��Ԃ��j
	size_type depth( ) const;	///< @brief Z�������̃R���e�i�Ɋi�[����Ă���f�[�^����Ԃ��i���1��Ԃ��j

public:
	/// @brief �s��̃T�C�Y�� num �s 1 ��ɕύX����
	//! 
	//! ���̍s��ƈقȂ�ꍇ�́C�����I�ɃT�C�Y�𒲐�����D
	//! �S�Ă̗v�f�̒l�̓f�t�H���g�R���X�g���N�^�ŏ����������D
	//! �܂��C�g�ݍ��݌^�iint �� double �Ȃǁj�̏ꍇ�́C�S�Ă̗v�f��0�ŏ���������D
	//! 
	//! @param[in] num �c ���T�C�Y��̍s��̍s��
	//! 
	//! @retval true  �c ����Ƀ��T�C�Y���I��
	//! @retval false �c ���T�C�Y��̃��������m�ۂł��Ȃ������ꍇ
	//! 
	bool resize( size_type num )
	{
		if( base::resize( num ) )
		{
			size1_ = num;
			size2_ = 1;
			return( true );
		}
		else
		{
			size1_ = size2_ = 0;
			return( false );
		}
	}


	/// @brief �s��̃T�C�Y�� num1 �s num2 ��ɕύX����
	//! 
	//! ���̍s��T�C�Y�ƈقȂ�ꍇ�́C�����I�ɃT�C�Y�𒲐�����D
	//! �S�Ă̗v�f�̒l�̓f�t�H���g�R���X�g���N�^�ŏ����������D
	//! �܂��C�g�ݍ��݌^�iint �� double �Ȃǁj�̏ꍇ�́C�S�Ă̗v�f��0�ŏ���������D
	//! 
	//! @param[in] nrows �c ���T�C�Y��̍s��̍s��
	//! @param[in] ncols �c ���T�C�Y��̍s��̗�
	//! 
	//! @retval true  �c ����Ƀ��T�C�Y���I��
	//! @retval false �c ���T�C�Y��̃��������m�ۂł��Ȃ������ꍇ
	//! 
	bool resize( size_type nrows, size_type ncols )
	{
		if( base::resize( nrows * ncols ) )
		{
			size1_ = nrows;
			size2_ = ncols;
			return( true );
		}
		else
		{
			size1_ = size2_ = 0;
			return( false );
		}
	}

	/// @brief �s��̃T�C�Y�� num1 �s num2 ��ɕύX����
	//! 
	//! �s��T�C�Y�� num1 �s num2 ��ɕύX���C�S�Ă̗v�f��l val �ŏ���������D
	//! 
	//! @param[in] nrows �c ���T�C�Y��̍s��̍s��
	//! @param[in] ncols �c ���T�C�Y��̍s��̗�
	//! @param[in] val   �c ���T�C�Y��Ɋe�v�f������������l
	//! 
	//! @retval true  �c ����Ƀ��T�C�Y���I��
	//! @retval false �c ���T�C�Y��̃��������m�ۂł��Ȃ������ꍇ
	//! 
	bool resize( size_type nrows, size_type ncols, const T &val )
	{
		if( base::resize( nrows * ncols, val ) )
		{
			size1_ = nrows;
			size2_ = ncols;
			return( true );
		}
		else
		{
			size1_ = size2_ = 0;
			return( false );
		}
	}

	/// @brief �R���e�i���̗v�f���g���~���O����
	//! 
	//! @param[out] out   �c �g���~���O���ʂ̍s����i�[����I�u�W�F�N�g�i�s��̃T�C�Y�͎����ŕύX����܂��j
	//! @param[in]  row   �c �g���~���O�̍s�����̊J�n�ʒu�i�[������n�܂�C���f�b�N�X�j
	//! @param[in]  col   �c �g���~���O�̗�����̊J�n�ʒu�i�[������n�܂�C���f�b�N�X�j
	//! @param[in]  nrows �c �g���~���O��̍s���i-1�̏ꍇ�́C�Ō�܂ł��R�s�[����j
	//! @param[in]  ncols �c �g���~���O��̗񐔁i-1�̏ꍇ�́C�Ō�܂ł��R�s�[����j
	//! 
	//! @retval true  �c �g���~���O�ɐ��������ꍇ�i���ƃT�C�Y���ς��Ȃ��ꍇ���܂ށj
	//! @retval false �c �s���ȃg���~���O���s�����Ƃ����ꍇ
	//! 
	bool trim( matrix &out, size_type row, size_type col, difference_type nrows = -1, difference_type ncols = -1 ) const
	{
		difference_type nrows_ = rows( );
		difference_type ncols_ = cols( );
		if( nrows_ <= static_cast< difference_type >( row ) || nrows_ < static_cast< difference_type >( row + nrows ) )
		{
			return( false );
		}
		else if( ncols_ <= static_cast< difference_type >( col ) || ncols_ < static_cast< difference_type >( col + ncols ) )
		{
			return( false );
		}
		else if( nrows_ == nrows && ncols_ == ncols )
		{
			return( true );
		}

		if( nrows < 0 )
		{
			nrows = nrows_ - row;
		}
		if( ncols < 0 )
		{
			ncols = ncols_ - col;
		}

		if( out.resize( nrows, ncols ) )
		{
			const_pointer pi = paccess( row, col );
			pointer       po = out.paccess( 0, 0 );
			for( difference_type c = 0 ; c < ncols ; c++ )
			{
				po = base::allocator_.copy_objects( pi, nrows, po );
				pi += this->rows( );
			}

			return( true );
		}
		else
		{
			return( false );
		}
	}

	/// @brief �R���e�i���̗v�f���g���~���O����
	//! 
	//! @param[in] row   �c �g���~���O�̍s�����̊J�n�ʒu�i�[������n�܂�C���f�b�N�X�j
	//! @param[in] col   �c �g���~���O�̗�����̊J�n�ʒu�i�[������n�܂�C���f�b�N�X�j
	//! @param[in] nrows �c �g���~���O��̍s���i-1�̏ꍇ�́C�Ō�܂ł��R�s�[����j
	//! @param[in] ncols �c �g���~���O��̗񐔁i-1�̏ꍇ�́C�Ō�܂ł��R�s�[����j
	//! 
	//! @retval true  �c �g���~���O�ɐ��������ꍇ�i���ƃT�C�Y���ς��Ȃ��ꍇ���܂ށj
	//! @retval false �c �s���ȃg���~���O���s�����Ƃ����ꍇ
	//! 
	bool trim( size_type row, size_type col, difference_type nrows = -1, difference_type ncols = -1 )
	{
		if( base::is_memory_shared( ) )
		{
			// �O���������𗘗p���Ă���ꍇ
			matrix o( *this );
			return( o.trim( *this, row, col, nrows, ncols ) );
		}
		else
		{
			matrix o;

			if( this->trim( o, row, col, nrows, ncols ) )
			{
				swap( o );

				return( true );
			}
			else
			{
				return( false );
			}
		}
	}

	/// @brief �s����̑S�Ă̓��e�����ւ���D
	//! 
	//! ����ւ����̍s�� m �̒��g�ƑS�ē���ւ���
	//! 
	//! @param[in] m �c ���e�����ւ���Ώ�
	//! 
	//! @retval true  �c �f�[�^�̃X���b�v�ɐ���
	//! @retval false �c �f�[�^�̃X���b�v�Ɏ��s
	//! 
	bool swap( matrix &m )
	{
		if( base::swap( m ) )
		{
			size_type _dmy_size1 = size1_;
			size_type _dmy_size2 = size2_;
			size1_ = m.size1_;
			size2_ = m.size2_;
			m.size1_ = _dmy_size1;
			m.size2_ = _dmy_size2;
			return( true );
		}
		else
		{
			return( false );
		}
	}

	/// @brief �s��̗v�f����ɂ���
	//! 
	//! �s��Ɋi�[����Ă���f�[�^��S�č폜���C�s�����i0�s0��j�ɂ���
	//! 
	void clear( )
	{
		base::clear( );
		size1_ = size2_ = 0;
	}


	size_type size1( ) const { return( size1_ ); }	///< @brief �s����Ԃ�
	size_type size2( ) const { return( size2_ ); }	///< @brief �񐔂�Ԃ�
	size_type rows( ) const { return( size1_ ); }	///< @brief �s����Ԃ�
	size_type cols( ) const { return( size2_ ); }	///< @brief �񐔂�Ԃ�



/************************************************************************************************************
**
**      �s�ɑ΂��鏇�����E�t�����̔����q
**
************************************************************************************************************/

	/// @brief �s�����𑀍삷�鏇���������_���A�N�Z�X�C�e���[�^��Ԃ�
	//!
	//! @code �s�����𑀍삷�鏇���������_���A�N�Z�X�C�e���[�^�̎g�p��
	//! std::cout << "�����������_���A�N�Z�X�C�e���[�^" << std::endl;
	//! mist::array< int >::iterator ite1 = a.row_begin( 1 );
	//! for( ; ite1 != a.row_end( 1 ) ; ite1++ )
	//! {
	//! 	std::cout << *ite1 << " ";
	//! }
	//! std::cout << std::endl << std::endl;
	//! @endcode
	//!
	iterator row_begin( size_type r ){ return( iterator( paccess( r, 0 ), rows( ) ) ); }

	/// @brief �s�����𑀍삷��R���X�g�^�̏����������_���A�N�Z�X�C�e���[�^��Ԃ�
	const_iterator row_begin( size_type r ) const { return( const_iterator( paccess( r, 0 ), rows( ) ) ); }

	/// @brief �s�����𑀍삷�鏇���������_���A�N�Z�X�C�e���[�^��Ԃ�
	iterator row_end( size_type r ){ return( iterator( paccess( r, cols( ) ), rows( ) ) ); }

	/// @brief �s�����𑀍삷��R���X�g�^�̏����������_���A�N�Z�X�C�e���[�^��Ԃ�
	const_iterator row_end( size_type r ) const { return( const_iterator( paccess( r, cols( ) ), rows( ) ) ); }


	/// @brief �s�����𑀍삷��t���������_���A�N�Z�X�C�e���[�^��Ԃ�
	//!
	//! @code �s�����𑀍삷��t���������_���A�N�Z�X�C�e���[�^�̎g�p��
	//! std::cout << "�t���������_���A�N�Z�X�C�e���[�^" << std::endl;
	//! mist::array< int >::iterator ite1 = a.row_rbegin( 1 );
	//! for( ; ite1 != a.row_rend( 1 ) ; ite1++ )
	//! {
	//! 	std::cout << *ite1 << " ";
	//! }
	//! std::cout << std::endl << std::endl;
	//! @endcode
	//!
	reverse_iterator row_rbegin( size_type r ){ return( reverse_iterator( row_end( r )  ) ); }

	/// @brief �s�����𑀍삷��R���X�g�^�̋t���������_���A�N�Z�X�C�e���[�^��Ԃ�
	const_reverse_iterator row_rbegin( size_type r ) const { return( const_reverse_iterator( row_end( r ) ) ); }

	/// @brief �s�����𑀍삷��t���������_���A�N�Z�X�C�e���[�^��Ԃ�
	reverse_iterator row_rend( size_type r ){ return( reverse_iterator( row_begin( r ) ) ); }

	/// @brief �s�����𑀍삷��R���X�g�^�̋t���������_���A�N�Z�X�C�e���[�^��Ԃ�
	const_reverse_iterator row_rend( size_type r ) const { return( const_reverse_iterator( row_begin( r ) ) ); }


/************************************************************************************************************
**
**      ��ɑ΂��鏇�����E�t�����̔����q
**
************************************************************************************************************/

	/// @brief ������𑀍삷�鏇���������_���A�N�Z�X�C�e���[�^��Ԃ�
	//!
	//! @code ������𑀍삷�鏇���������_���A�N�Z�X�C�e���[�^�̎g�p��
	//! std::cout << "�����������_���A�N�Z�X�C�e���[�^" << std::endl;
	//! mist::array< int >::iterator ite1 = a.row_begin( 1 );
	//! for( ; ite1 != a.row_end( 1 ) ; ite1++ )
	//! {
	//! 	std::cout << *ite1 << " ";
	//! }
	//! std::cout << std::endl << std::endl;
	//! @endcode
	//!
	iterator col_begin( size_type c ){ return( iterator( paccess( 0, c ), 1 ) ); }

	/// @brief ������𑀍삷��R���X�g�^�̏����������_���A�N�Z�X�C�e���[�^��Ԃ�
	const_iterator col_begin( size_type c ) const { return( const_iterator( paccess( 0, c ), 1 ) ); }

	/// @brief ������𑀍삷�鏇���������_���A�N�Z�X�C�e���[�^��Ԃ�
	iterator col_end( size_type c ){ return( iterator( paccess( rows( ), c ), 1 ) ); }

	/// @brief ������𑀍삷��R���X�g�^�̏����������_���A�N�Z�X�C�e���[�^��Ԃ�
	const_iterator col_end( size_type c ) const { return( const_iterator( paccess( rows( ), c ), 1 ) ); }


	/// @brief ������𑀍삷��t���������_���A�N�Z�X�C�e���[�^��Ԃ�
	//!
	//! @code ������𑀍삷��t���������_���A�N�Z�X�C�e���[�^�̎g�p��
	//! std::cout << "�t���������_���A�N�Z�X�C�e���[�^" << std::endl;
	//! mist::array< int >::iterator ite1 = a.row_rbegin( 1 );
	//! for( ; ite1 != a.row_rend( 1 ) ; ite1++ )
	//! {
	//! 	std::cout << *ite1 << " ";
	//! }
	//! std::cout << std::endl << std::endl;
	//! @endcode
	//!
	reverse_iterator col_rbegin( size_type c ){ return( reverse_iterator( col_end( c ) ) ); }

	/// @brief ������𑀍삷��R���X�g�^�̋t���������_���A�N�Z�X�C�e���[�^��Ԃ�
	const_reverse_iterator col_rbegin( size_type c ) const { return( const_reverse_iterator(  col_end( c ) ) ); }

	/// @brief ������𑀍삷��t���������_���A�N�Z�X�C�e���[�^��Ԃ�
	reverse_iterator col_rend( size_type c ){ return( reverse_iterator( col_begin( c ) ) ); }

	/// @brief ������𑀍삷��R���X�g�^�̋t���������_���A�N�Z�X�C�e���[�^��Ԃ�
	const_reverse_iterator col_rend( size_type c ) const { return( const_reverse_iterator( col_begin( c ) ) ); }





public:
	/**  @brief 3�~3 �̑Ίp�s����v�Z����
	 *  
	 *  3�̓��͐����̑傫�����̂��珇�Ԃɕ��בւ��āC�Ίp�s����쐬����
	 *  
	 *  \f[
	 *      \left(
	 *           \begin{array}{ccc}
	 *             \sigma_1 &    0     &    0     \\
	 *                0     & \sigma_2 &    0     \\
	 *                0     &    0     & \sigma_3
	 *           \end{array}
	 *      \right)
	 *  \f]
	 *  �������C\f$ \sigma_1 \ge \sigma_2 \ge \sigma_3 \f$
	 *  
	 *  @param[in] s1 �c ����1
	 *  @param[in] s2 �c ����2
	 *  @param[in] s3 �c ����3
	 * 
	 *  @return �Ίp�s��
	 */
	static const matrix diag( const value_type &s1, const value_type &s2, const value_type &s3 )
	{
		typedef __numeric__::value_compare< __numeric__::is_complex< value_type >::value > value_compare;
		matrix d( 3, 3 );

		if( value_compare::lt( s1, s2 ) )
		{
			// s1 < s2
			if( value_compare::lt( s1, s3 ) )
			{
				if( value_compare::lt( s2, s3 ) )
				{
					d( 0, 0 ) = s3;
					d( 1, 1 ) = s2;
					d( 2, 2 ) = s1;
				}
				else
				{
					d( 0, 0 ) = s2;
					d( 1, 1 ) = s3;
					d( 2, 2 ) = s1;
				}
			}
			else
			{
				d( 0, 0 ) = s2;
				d( 1, 1 ) = s1;
				d( 2, 2 ) = s3;
			}
		}
		else
		{
			// s2 < s1
			if( value_compare::lt( s1, s3 ) )
			{
				d( 0, 0 ) = s3;
				d( 1, 1 ) = s1;
				d( 2, 2 ) = s2;
			}
			else
			{
				if( value_compare::lt( s2, s3 ) )
				{
					d( 0, 0 ) = s1;
					d( 1, 1 ) = s3;
					d( 2, 2 ) = s2;
				}
				else
				{
					d( 0, 0 ) = s1;
					d( 1, 1 ) = s2;
					d( 2, 2 ) = s3;
				}
			}
		}
		return( d );
	}



	/// @brief ���̓f�[�^��p����4�~4�̍s���Ԃ�
	static const matrix _44(
								const value_type &a00, const value_type &a01, const value_type &a02, const value_type &a03,
								const value_type &a10, const value_type &a11, const value_type &a12, const value_type &a13,
								const value_type &a20, const value_type &a21, const value_type &a22, const value_type &a23,
								const value_type &a30, const value_type &a31, const value_type &a32, const value_type &a33
							)
	{
		matrix o( 4, 4 );

		o( 0, 0 ) = a00; o( 0, 1 ) = a01; o( 0, 2 ) = a02; o( 0, 3 ) = a03;
		o( 1, 0 ) = a10; o( 1, 1 ) = a11; o( 1, 2 ) = a12; o( 1, 3 ) = a13;
		o( 2, 0 ) = a20; o( 2, 1 ) = a21; o( 2, 2 ) = a22; o( 2, 3 ) = a23;
		o( 3, 0 ) = a30; o( 3, 1 ) = a31; o( 3, 2 ) = a32; o( 3, 3 ) = a33;

		return( o );
	}

	/// @brief ���̓f�[�^��p����3�~4�̍s���Ԃ�
	static const matrix _34(
								const value_type &a00, const value_type &a01, const value_type &a02, const value_type &a03,
								const value_type &a10, const value_type &a11, const value_type &a12, const value_type &a13,
								const value_type &a20, const value_type &a21, const value_type &a22, const value_type &a23
							)
	{
		matrix o( 3, 4 );

		o( 0, 0 ) = a00; o( 0, 1 ) = a01; o( 0, 2 ) = a02; o( 0, 3 ) = a03;
		o( 1, 0 ) = a10; o( 1, 1 ) = a11; o( 1, 2 ) = a12; o( 1, 3 ) = a13;
		o( 2, 0 ) = a20; o( 2, 1 ) = a21; o( 2, 2 ) = a22; o( 2, 3 ) = a23;

		return( o );
	}

	/// @brief ���̓f�[�^��p����4�~3�̍s���Ԃ�
	static const matrix _43(
								const value_type &a00, const value_type &a01, const value_type &a02,
								const value_type &a10, const value_type &a11, const value_type &a12,
								const value_type &a20, const value_type &a21, const value_type &a22,
								const value_type &a30, const value_type &a31, const value_type &a32
							)
	{
		matrix o( 4, 3 );

		o( 0, 0 ) = a00; o( 0, 1 ) = a01; o( 0, 2 ) = a02;
		o( 1, 0 ) = a10; o( 1, 1 ) = a11; o( 1, 2 ) = a12;
		o( 2, 0 ) = a20; o( 2, 1 ) = a21; o( 2, 2 ) = a22;
		o( 3, 0 ) = a30; o( 3, 1 ) = a31; o( 3, 2 ) = a32;

		return( o );
	}

	/// @brief ���̓f�[�^��p����4�~1�̍s���Ԃ�
	static const matrix _41( const value_type &a0, const value_type &a1, const value_type &a2, const value_type &a3 )
	{
		matrix o( 4, 1 );

		o[ 0 ] = a0;
		o[ 1 ] = a1;
		o[ 2 ] = a2;
		o[ 3 ] = a3;

		return( o );
	}

	/// @brief ���̓f�[�^��p����3�~3�̍s���Ԃ�
	static const matrix _33(
								const value_type &a00, const value_type &a01, const value_type &a02,
								const value_type &a10, const value_type &a11, const value_type &a12,
								const value_type &a20, const value_type &a21, const value_type &a22
							)
	{
		matrix o( 3, 3 );

		o( 0, 0 ) = a00; o( 0, 1 ) = a01; o( 0, 2 ) = a02;
		o( 1, 0 ) = a10; o( 1, 1 ) = a11; o( 1, 2 ) = a12;
		o( 2, 0 ) = a20; o( 2, 1 ) = a21; o( 2, 2 ) = a22;

		return( o );
	}

	/// @brief ���̓f�[�^��p����3�~1�̍s���Ԃ�
	static const matrix _31( const value_type &a0, const value_type &a1, const value_type &a2 )
	{
		matrix o( 3, 1 );

		o[ 0 ] = a0;
		o[ 1 ] = a1;
		o[ 2 ] = a2;

		return( o );
	}

	/// @brief ���̓f�[�^��p����2�~2�̍s���Ԃ�
	static const matrix _22(
								const value_type &a00, const value_type &a01,
								const value_type &a10, const value_type &a11
							)
	{
		matrix o( 2, 2 );

		o( 0, 0 ) = a00; o( 0, 1 ) = a01;
		o( 1, 0 ) = a10; o( 1, 1 ) = a11;

		return( o );
	}

	/// @brief ���̓f�[�^��p����3�~1�̍s���Ԃ�
	static const matrix _21( const value_type &a0, const value_type &a1 )
	{
		matrix o( 2, 1 );

		o[ 0 ] = a0;
		o[ 1 ] = a1;

		return( o );
	}

	/// @brief �C�ӃT�C�Y�̒P�ʍs���Ԃ�
	static const matrix identity( size_type rows, size_type cols )
	{
		size_type size = rows < cols ? rows : cols;
		matrix o( rows, cols );
		for( size_type i = 0 ; i < size ; i++ )
		{
			o( i, i ) = 1;
		}
		return( o );
	}

	/// @brief �C�ӃT�C�Y�̃[���s���Ԃ�
	static const matrix zero( size_type rows, size_type cols )
	{
		return( matrix( rows, cols ) );
	}

	/// @brief �������]�����s���Ԃ�
	matrix operator -( ) const
	{
		const matrix &m = *this;
		matrix o( size1_, size2_ );
		for( size_type i = 0 ; i < o.size( ) ; i++ )
		{
			o[ i ] = -m[ i ];
		}
		return( o );
	}

	/// @brief �]�u�s���Ԃ�
	matrix t( ) const
	{
		const matrix &m = *this;
		matrix o( size2_, size1_ );
		for( size_type c = 0 ; c < o.cols( ) ; c++ )
		{
			for( size_type r = 0 ; r < o.rows( ) ; r++ )
			{
				o( r, c ) = m( c, r );
			}
		}
		return( o );
	}

	/// @brief ����]�u�s���Ԃ�
	matrix dagger( ) const
	{
		const matrix &m = *this;
		matrix o( size2_, size1_ );
		for( size_type c = 0 ; c < o.cols( ) ; c++ )
		{
			for( size_type r = 0 ; r < o.rows( ) ; r++ )
			{
				o( r, c ) = __numeric__::conjugate( m( c, r ) );
			}
		}
		return( o );
	}


	// �s��ɑ΂��鉉�Z�q
	//   += �s��
	//   += �萔
	//   -= �s��
	//   -= �萔
	//   *= �s��
	//   *= �萔
	//   /= �萔

	/// @brief �s��̑����Z
	//! 
	//! \f[ {\bf A} = {\bf A} + {\bf B} \f]
	//! 
	//! @param[in] m2 �c �����Z���s���s��
	//! 
	//! @return �������g
	//! 
	template < class TT, class AAlocator >
	const matrix& operator +=( const matrix< TT, AAlocator > &m2 )
	{
		matrix &m1 = *this;
#if _CHECK_ARRAY_OPERATION_ != 0
		if( m1.size( ) != m2.size( ) )
		{
			// �����Z�ł��܂����O
			::std::cerr << "can't add arrays." << ::std::endl;
			return( *this );
		}
#endif
		for( size_type i = 0 ; i < m1.size( ) ; i++ ) m1[i] += static_cast< T >( m2[i] );
		return( m1 );
	}


	/// @brief �s��̈����Z
	//! 
	//! \f[ {\bf A} = {\bf A} - {\bf B} \f]
	//! 
	//! @param[in] m2 �c �����Z���s���s��
	//! 
	//! @return �������g
	//! 
	template < class TT, class AAlocator >
	const matrix& operator -=( const matrix< TT, AAlocator > &m2 )
	{
		matrix &m1 = *this;
#ifdef _CHECK_ARRAY_OPERATION_
		if( m1.size( ) != m2.size( ) )
		{
			// �����Z�ł��܂����O
			::std::cerr << "can't subtract matrixs." << ::std::endl;
			return( m1 );
		}
#endif
		for( size_type i = 0 ; i < m1.size( ) ; i++ ) m1[i] -= static_cast< T >( m2[i] );
		return( m1 );
	}


	/// @brief �s��̊|���Z
	//! 
	//! \f[ {\bf A} = {\bf A} * {\bf B} \f]
	//! 
	//! @param[in] m2 �c �E����|����s��
	//! 
	//! @return �������g
	//! 
	template < class TT, class AAlocator >
	const matrix& operator *=( const matrix< TT, AAlocator > &m2 )
	{
		matrix &m1 = *this;

#if defined( _CHECK_MATRIX_OPERATION_ ) && _CHECK_MATRIX_OPERATION_ != 0
		if( m1.cols( ) != m2.rows( ) )
		{
			// �|���Z�ł��܂����O
			::std::cerr << "can't multiply matrices." << ::std::endl;
			return( m1 );
		}
#endif

		typedef __numeric__::value_compare< __numeric__::is_complex< value_type >::value > value_compare;

		matrix< T, Allocator > mat( m1.rows( ), m2.cols( ) );

#ifdef _OPENMP
		int nCols = static_cast< int >( mat.cols( ) );

		#pragma omp parallel for schedule( guided )
		for( int c = 0 ; c < nCols ; c++ )
		{
			for( size_type t = 0 ; t < m1.cols( ) ; t++ )
			{
				value_type val = static_cast< value_type >( m2( t, c ) );
				if( !value_compare::is_zero( val ) )
				{
					pointer pm0 = &mat( 0, c );
					pointer pm1 = &m1( 0, t );
					for( size_type r = 0 ; r < mat.rows( ) ; r++ )
					{
						pm0[ r ] += pm1[ r ] * val;
					}
				}
			}
		}
#else
		size_type r, c, t;

		for( c = 0 ; c < mat.cols( ) ; c++ )
		{
			for( t = 0 ; t < m1.cols( ) ; t++ )
			{
				value_type val = static_cast< value_type >( m2( t, c ) );
				if( !value_compare::is_zero( val ) )
				{
					pointer pm0 = &mat( 0, c );
					pointer pm1 = &m1( 0, t );
					for( r = 0 ; r < mat.rows( ) ; r++ )
					{
						pm0[ r ] += pm1[ r ] * val;
					}
				}
			}
		}
#endif

		m1.swap( mat );

		return( m1 );
	}


	/// @brief �s��ւ̒萔�i�P�ʍs��̒萔�{�j�̑����Z
	//! 
	//! \f[ {\bf A} = {\bf A} + val * {\bf I} \f]
	//! 
	//! @param[in] val �c �P�ʍs��̒萔�{��\����
	//! 
	//! @return �������g
	//! 
	const matrix& operator +=( typename type_trait< T >::value_type val )
	{
		matrix &m = *this;
		size_type i, size = m.rows( ) < m.cols( ) ? m.rows( ) : m.cols( );
		for( i = 0 ; i < size ; i++ ) m( i, i ) += val;
		return( m );
	}


	/// @brief �s��ւ̒萔�i�P�ʍs��̒萔�{�j�̈����Z
	//! 
	//! \f[ {\bf A} = {\bf A} - val * {\bf I} \f]
	//! 
	//! @param[in] val �c �P�ʍs��̒萔�{��\����
	//! 
	//! @return �������g
	//! 
	const matrix& operator -=( typename type_trait< T >::value_type val )
	{
		matrix &m = *this;
		size_type i, size = m.rows( ) < m.cols( ) ? m.rows( ) : m.cols( );
		for( i = 0 ; i < size ; i++ ) m( i, i ) -= val;
		return( m );
	}


	/// @brief �s��ւ̒萔�i�P�ʍs��̒萔�{�j�̊|���Z
	//! 
	//! \f[ {\bf A} = {\bf A} * val \f]
	//! 
	//! @param[in] val �c �s���萔�{���鐔
	//! 
	//! @return �������g
	//! 
	const matrix& operator *=( typename type_trait< T >::value_type val )
	{
		matrix &m = *this;
		for( size_type i = 0 ; i < m.size( ) ; i++ ) m[i] *= val;
		return( m );
	}


	/// @brief �s��ւ̒萔�i�P�ʍs��̒萔�{�j�̊���Z
	//! 
	//! \f[ {\bf A} = {\bf A} / val \f]
	//! 
	//! @param[in] val �c �s�������萔
	//! 
	//! @return �������g
	//! 
	const matrix& operator /=( typename type_trait< T >::value_type val )
	{
		matrix &m = *this;
#if defined( _CHECK_MATRIX_OPERATION_ ) && _CHECK_MATRIX_OPERATION_ != 0
		if( val == value_type( 0 ) )
		{
			// �[�����Z����
			::std::cerr << "zero division occured." << ::std::endl;
			return( m );
		}
#endif
		for( size_type i = 0 ; i < m.size( ) ; i++ ) m[i] /= val;
		return( m );
	}


	/// @brief 2�̍s�񂪓������i�S�v�f�̍��� delta �����j���ǂ����𔻒肷��
	//! 
	//! @param[in] a     �c ��r�Ώۂ̍s��
	//! @param[in] delta �c ���e�덷
	//! 
	//! @retval true  �c 2�̍s�񂪓������ꍇ
	//! @retval false �c 2�̍s�񂪈قȂ�ꍇ
	//! 
	bool is_equal( const matrix &a, const double delta )
	{
		typedef __numeric__::value_compare< __numeric__::is_complex< value_type >::value > value_compare;

		if( rows( ) != a.rows( ) || cols( ) != a.cols( ) )
		{
			return( false );
		}

		for( size_type i = 0 ; i < base::size( ) ; i++ )
		{
			if( !value_compare::eq( operator []( i ), a[ i ], delta ) )
			{
				return( false );
			}
		}

		return( true );
	}

	/// @brief 2�̍s�񂪓������i�S�v�f�������l�����j���ǂ����𔻒肷��
	//! 
	//! @param[in] a �c �E�Ӓl
	//! 
	//! @retval true  �c 2�̍s�񂪓������ꍇ
	//! @retval false �c 2�̍s�񂪈قȂ�ꍇ
	//! 
	bool operator ==( const matrix &a ) const
	{
		if( rows( ) != a.rows( ) || cols( ) != a.cols( ) )
		{
			return( false );
		}

		for( size_type i = 0 ; i < base::size( ) ; i++ )
		{
			if( operator []( i ) != a[ i ] )
			{
				return( false );
			}
		}

		return( true );
	}

	/// @brief 2�̍s�񂪓������Ȃ��i�ǂꂩ�P�ł��قȂ�v�f�����j���ǂ����𔻒肷��
	//! 
	//! @param[in] a �c �E�Ӓl
	//! 
	//! @retval true  �c 2�̍s�񂪈قȂ�ꍇ
	//! @retval false �c 2�̍s�񂪓������ꍇ
	//! 
	bool operator !=( const matrix &a ) const { return( !( *this == a ) ); }


public:
	/// @brief �v�f�̌^���قȂ�s���������
	//! 
	//! �R�s�[���ł���s�� o �ƑS�������s����쐬����D
	//! �R�s�[��i�����ł͎������g�j�̗v�f���� o �ƈقȂ�ꍇ�́C�����I�ɃT�C�Y�𒲐�����D
	//! �R�s�[���ƃR�s�[��Ńf�[�^�^�iarray< data > �� data�j���قȂ�ꍇ�̑�����s���D
	//! 
	//! @param[in] o  �c �R�s�[���̍s��
	//! 
	//! @return �������g
	//! 
	template < class TT, class AAlocator >
	const matrix& operator =( const matrix< TT, AAlocator > &o )
	{
		base::operator =( o );

		if( base::empty( ) )
		{
			size1_ = size2_ = 0;
		}
		else
		{
			size1_ = o.size1( );
			size2_ = o.size2( );
		}

		return( *this );
	}

	/// @brief �v�f�̌^�������s���������
	//! 
	//! �R�s�[���ł���s�� o �ƑS�������s����쐬����D
	//! �R�s�[��i�����ł͎������g�j�̗v�f���� o �ƈقȂ�ꍇ�́C�����I�ɃT�C�Y�𒲐�����D
	//! 
	//! @param[in] o  �c �R�s�[���̍s��
	//! 
	//! @return �������g
	//! 
	const matrix< T, Allocator >& operator =( const matrix< T, Allocator > &o )
	{
		if( this == &o ) return( *this );

		base::operator =( o );

		if( base::empty( ) )
		{
			size1_ = size2_ = 0;
		}
		else
		{
			size1_ = o.size1( );
			size2_ = o.size2( );
		}

		return( *this );
	}


// �v�f�ւ̃A�N�Z�X
private:
	/// @brief index �Ŏ������ʒu�̗v�f�̃|�C���^��Ԃ�
	//! 
	//! @param[in] r �c �s�ʒu
	//! @param[in] c �c ��ʒu
	//! 
	//! @return �w�肳�ꂽ�v�f�������|�C���^
	//! 
	pointer paccess( size_type r, size_type c )
	{
		return( base::data_ + r + c * size1_ );
	}

	/// @brief index �Ŏ������ʒu�̗v�f�� const �|�C���^��Ԃ�
	//! 
	//! @param[in] r �c �s�ʒu
	//! @param[in] c �c ��ʒu
	//! 
	//! @return �w�肳�ꂽ�v�f������ const �|�C���^
	//! 
	const_pointer paccess( size_type r, size_type c ) const
	{
		return( base::data_ + r + c * size1_ );
	}


public:
	/// @brief r �s c �� �Ŏ������ʒu�̗v�f�̎Q�Ƃ�Ԃ�
	//!
	//! DEBUG �}�N����L���ɂ����iNDEBUG�}�N�����`���Ȃ��j�ꍇ�́C�w�肳�ꂽ r �s c �� ���L���Ȕ͈͓��ɂ��邩���`�F�b�N����
	//! 
	//! @param[in] r �c �s�ʒu
	//! @param[in] c �c ��ʒu
	//! 
	//! @return �w�肳�ꂽ�v�f�������Q��
	//! 
	reference at( size_type r, size_type c )
	{
		_CHECK_ACCESS_VIOLATION2U_( r, c )
		return( base::data_[ r + c * size1_ ] );
	}

	/// @brief r �s c �� �Ŏ������ʒu�̗v�f�� const �Q�Ƃ�Ԃ�
	//!
	//! DEBUG �}�N����L���ɂ����iNDEBUG�}�N�����`���Ȃ��j�ꍇ�́C�w�肳�ꂽ r �s c �� ���L���Ȕ͈͓��ɂ��邩���`�F�b�N����
	//! 
	//! @param[in] r �c �s�ʒu
	//! @param[in] c �c ��ʒu
	//! 
	//! @return �w�肳�ꂽ�v�f������ const �Q��
	//! 
	const_reference at( size_type r, size_type c ) const
	{
		_CHECK_ACCESS_VIOLATION2U_( r, c )
		return( base::data_[ r + c * size1_ ] );
	}

	/// @brief r �s c �� �Ŏ������ʒu�̗v�f�̎Q�Ƃ�Ԃ�
	//!
	//! DEBUG �}�N����L���ɂ����iNDEBUG�}�N�����`���Ȃ��j�ꍇ�́C�w�肳�ꂽ r �s c �� ���L���Ȕ͈͓��ɂ��邩���`�F�b�N����
	//! 
	//! @param[in] r �c �s�ʒu
	//! @param[in] c �c ��ʒu
	//! 
	//! @return �w�肳�ꂽ�v�f�������Q��
	//! 
	reference operator ()( size_type r, size_type c )
	{
		_CHECK_ACCESS_VIOLATION2U_( r, c )
		return( base::data_[ r + c * size1_ ] );
	}

	/// @brief r �s c �� �Ŏ������ʒu�̗v�f�� const �Q�Ƃ�Ԃ�
	//!
	//! DEBUG �}�N����L���ɂ����iNDEBUG�}�N�����`���Ȃ��j�ꍇ�́C�w�肳�ꂽ r �s c �� ���L���Ȕ͈͓��ɂ��邩���`�F�b�N����
	//! 
	//! @param[in] r �c �s�ʒu
	//! @param[in] c �c ��ʒu
	//! 
	//! @return �w�肳�ꂽ�v�f������ const �Q��
	//! 
	const_reference operator ()( size_type r, size_type c ) const
	{
		_CHECK_ACCESS_VIOLATION2U_( r, c )
		return( base::data_[ r + c * size1_ ] );
	}


public:
	/// @brief �f�B�t�H���g�R���X�g���N�^�D0 �s 0 �� �̍s����쐬����
	matrix( ) : base( ), size2_( 0 ), size1_( 0 ) {}

	/// @brief �A���P�[�^ a �̃R�s�[�𗘗p���C0 �s 0 �� �̍s����쐬����
	explicit matrix( const Allocator &a ) : base( a ), size2_( 0 ), size1_( 0 ) {}


	/// @brief �v�f�� rnum �s cnum �� �̍s����쐬���C�f�t�H���g�l�ŗv�f������������
	matrix( size_type rnum, size_type cnum ) : base( rnum * cnum ), size2_( cnum ), size1_( rnum )
	{
		if( base::empty( ) ) size1_ = size2_ = 0;
	}

	/// @brief �v�f�� rnum �s cnum �� �ŃA���P�[�^ a �̃R�s�[�𗘗p�����s����쐬���C�f�t�H���g�l�ŗv�f������������
	matrix( size_type rnum, size_type cnum, const Allocator &a ) : base( rnum * cnum, a ), size2_( cnum ), size1_( rnum )
	{
		if( base::empty( ) ) size1_ = size2_ = 0;
	}


	/// @brief �v�f�� rnum �s cnum �� �̍s����쐬���C�S�v�f��l val �ŗv�f������������
	matrix( size_type rnum, size_type cnum, const T &val ) : base( rnum * cnum, val ), size2_( cnum ), size1_( rnum )
	{
		if( base::empty( ) ) size1_ = size2_ = 0;
	}

	/// @brief �v�f�� rnum �s cnum �� �ŃA���P�[�^ a �̃R�s�[�𗘗p�����s����쐬���C�S�v�f��l val �ŗv�f������������
	matrix( size_type rnum, size_type cnum, const T &val, const Allocator &a ) : base( rnum * cnum, val, a ), size2_( cnum ), size1_( rnum )
	{
		if( base::empty( ) ) size1_ = size2_ = 0;
	}




	/// @brief ptr ���w���������̈�ɁC�v�f�� rnum �s cnum �� �̍s����쐬����iptr ���w����̗��p�\�ȃ������ʂ� mem_available �j
	matrix( size_type rnum, size_type cnum, pointer ptr, size_type mem_available ) : base( rnum * cnum, ptr, mem_available ), size2_( cnum ), size1_( rnum )
	{
		if( base::empty( ) ) size1_ = size2_ = 0;
	}

	/// @brief ptr ���w���������̈�ɁC�v�f�� rnum �s cnum �� �̍s����쐬���C�S�v�f��l val �ŗv�f������������iptr ���w����̗��p�\�ȃ������ʂ� mem_available �j
	matrix( size_type rnum, size_type cnum, const T &val, pointer ptr, size_type mem_available ) : base( rnum * cnum, val, ptr, mem_available ), size2_( cnum ), size1_( rnum )
	{
		if( base::empty( ) ) size1_ = size2_ = 0;
	}


	/// @brief ���̍s��ŗv�f�̌^���قȂ���̂��瓯���T�C�Y�̍s����쐬����
	//!
	//! @attention �قȂ�v�f�^�ԂŃf�[�^�̕ϊ����\�łȂ��Ă͂Ȃ�Ȃ�
	//!
	template < class TT, class AAlocator >
	matrix( const matrix< TT, AAlocator > &o ) : base( o ), size2_( o.size2( ) ), size1_( o.size1( ) )
	{
		if( base::empty( ) ) size1_ = size2_ = 0;
	}


	/// @brief ���̍s��œ����v�f�^�̂��̂�p���ď���������
	matrix( const matrix< T, Allocator > &o ) : base( o ), size2_( o.size2_ ), size1_( o.size1_ )
	{
		if( base::empty( ) ) size1_ = size2_ = 0;
	}
};



/// @brief �w�肳�ꂽ�X�g���[���ɁC�s����̗v�f�𐮌`���ďo�͂���
//! 
//! @param[in,out] out �c ���͂Əo�͂��s���X�g���[��
//! @param[in]     m   �c �o�͂���s��
//! 
//! @return ���͂��ꂽ�X�g���[��
//! 
//! @code �o�͗�
//! 1, 4, 7
//! 2, 5, 8
//! 3, 6, 9
//! @endcode
//! 
template < class T, class Allocator >
inline ::std::ostream &operator <<( ::std::ostream &out, const matrix< T, Allocator > &m )
{
	typename matrix< T, Allocator >::size_type r, c;
	for( r = 0 ; r < m.rows( ) ; r++ )
	{
		if( r != 0 )
		{
			out << ::std::endl;
		}
		for( c = 0 ; c < m.cols( ) ; c++ )
		{
			out << m( r, c );
			if( c != m.cols( ) - 1 ) out << ", ";
		}
	}

	return( out );
}


/************************************************************************************************************
**
**      �s��ɑ΂��鉉�Z�q
**        �s�� + �s��
**        �s�� + �萔
**        �萔 + �s��
**
**        �s�� - �s��
**        �s�� - �萔
**        �萔 - �s��
**
**        �s�� * �s��
**        �s�� * �萔
**        �萔 * �s��
**
**        �s�� / �萔
**
************************************************************************************************************/


/// @brief �����Z
//! 
//! @param[in] m1 �c ���Ӓl
//! @param[in] m2 �c �E�Ӓl
//! 
//! @return �����Z����
//! 
template < class T, class Allocator >
inline matrix< T, Allocator > operator +( const matrix< T, Allocator > &m1, const matrix< T, Allocator > &m2 )
{
	return( matrix< T, Allocator >( m1 ) += m2 );
}


/// @brief �����Z
//! 
//! @param[in] m1 �c ���Ӓl
//! @param[in] m2 �c �E�Ӓl
//! 
//! @return �����Z����
//! 
template < class T, class Allocator >
inline matrix< T, Allocator > operator -( const matrix< T, Allocator > &m1, const matrix< T, Allocator > &m2 )
{
	return( matrix< T, Allocator >( m1 ) -= m2 );
}


/// @brief �|���Z
//! 
//! @param[in] m1 �c ���Ӓl
//! @param[in] m2 �c �E�Ӓl
//! 
//! @return �|���Z����
//! 
template < class T, class Allocator >
inline matrix< T, Allocator > operator *( const matrix< T, Allocator > &m1, const matrix< T, Allocator > &m2 )
{
	return( matrix< T, Allocator >( m1 ) *= m2 );
}


/// @brief �P�ʍs��̒萔�{�Ƃ̑����Z
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] m   �c �s��
//! @param[in] val �c �萔
//! 
//! @return ���Ӓl�̍s��ɒP�ʍs��̒萔�{�𑫂��Z��������
//! 
template < class T, class Allocator >
inline matrix< T, Allocator > operator +( const matrix< T, Allocator > &m, typename type_trait< T >::value_type val )
{
	return( matrix< T, Allocator >( m ) += val );
}

/// @brief �萔�Ƃ̑����Z
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] val �c �萔
//! @param[in] m   �c �s��
//! 
//! @return �E�Ӓl�̍s��ɒP�ʍs��̒萔�{�𑫂��Z��������
//! 
template < class T, class Allocator >
inline matrix< T, Allocator > operator +( typename matrix< T, Allocator >::value_type val, const matrix< T, Allocator > &m )
{
	return( matrix< T, Allocator >( m ) += val );
}



/// @brief �萔�Ƃ̈����Z
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] m   �c �s��
//! @param[in] val �c �萔
//! 
//! @return ���Ӓl�̍s�񂩂�C�P�ʍs���萔�{�������̂������Z��������
//! 
template < class T, class Allocator >
inline matrix< T, Allocator > operator -( const matrix< T, Allocator > &m, typename type_trait< T >::value_type val )
{
	return( matrix< T, Allocator >( m ) -= val );
}

/// @brief �萔�Ƃ̈����Z
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] val �c �萔
//! @param[in] m   �c �s��
//! 
//! @return �P�ʍs���萔�{�������̂���E�Ӓl�̍s��������Z��������
//! 
template < class T, class Allocator >
inline matrix< T, Allocator > operator -( typename matrix< T, Allocator >::value_type val, const matrix< T, Allocator > &m )
{
	return( matrix< T, Allocator >( m ) -= val );
}


/// @brief �萔�Ƃ̊|���Z
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] m   �c �s��
//! @param[in] val �c �萔
//! 
//! @return ���Ӓl�̍s���萔�{��������
//! 
template < class T, class Allocator >
inline matrix< T, Allocator > operator *( const matrix< T, Allocator > &m, typename type_trait< T >::value_type val )
{
	return( matrix< T, Allocator >( m ) *= val );
}

/// @brief �萔�Ƃ̊|���Z
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] val �c �萔
//! @param[in] m   �c �s��
//! 
//! @return �E�Ӓl�̍s���萔�{��������
//! 
template < class T, class Allocator >
inline matrix< T, Allocator > operator *( typename matrix< T, Allocator >::value_type val, const matrix< T, Allocator > &m )
{
	return( matrix< T, Allocator >( m ) *= val );
}


// @brief �萔�Ƃ̊���Z
//! 
//! �ڍׂȐ�����֐��̎g�p�������
//! 
//! @param[in] m   �c �s��
//! @param[in] val �c �萔
//! 
//! @return ���Ӓl�̍s���萔�Ŋ���Z��������
//! 
template < class T, class Allocator >
inline matrix< T, Allocator > operator /( const matrix< T, Allocator > &m, typename type_trait< T >::value_type val )
{
	return( matrix< T, Allocator >( m ) /= val );
}


// mist���O��Ԃ̏I���
_MIST_END

#endif // __INCLUDE_MIST_MATRIX__