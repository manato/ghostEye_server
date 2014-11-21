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

// �{�t�@�C���͎��ʊ�t�@�C������ʁX�ɃC���N���[�h�������̂ł���C2�d�C���N���[�h��OK

/// @brief AdaBoost���ʊ�ƃJ�X�P�[�h�^AdaBoost���ʊ�̏���XML�`���œ��o�͂���֐��Q�i�����p�j
namespace io
{
	///// @brief libxml2��API�𗘗p���ă^�O�̑������擾����֐�
	//inline std::string get_value( xmlTextReaderPtr reader, const std::string &default_value = "" )
	//{
	//	const xmlChar * value = xmlTextReaderConstValue( reader );
	//	if( value == NULL )
	//	{
	//		return( default_value );
	//	}
	//	else
	//	{
	//		return( std::string( ( const char * )value ) );
	//	}
	//}

	///// @brief libxml2��API�𗘗p���ă^�O�̑������擾����֐�
	//inline std::string get_attribute( xmlTextReaderPtr reader, const std::string &attr, const std::string &default_value = "" )
	//{
	//	// �m�[�h�̃A�g���r���[�g�����m�[�h���擾����
	//	xmlChar *value = xmlTextReaderGetAttribute( reader, ( const xmlChar * )attr.c_str( ) );

	//	if( value == NULL )
	//	{
	//		return( default_value );
	//	}
	//	else
	//	{
	//		return( std::string( ( const char * )value ) );
	//	}
	//}

	/// @brief libxml2��API�𗘗p����AdaBoost���ʊ�̐ݒ���擾����֐�
	inline int read( xmlTextReaderPtr reader, boosted_classifier &classifier, const std::string &type = "", const std::string &structure = "" )
	{
		if( reader == NULL )
		{
			return( -1 );
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
			case 1: // Element
				if( sdepth == depth )
				{
					if( xmlStrcasecmp( name, BAD_CAST "classifier" ) != 0 )
					{
						std::cerr << "Root element of XML should be started with <classifier> tag." << std::endl;
						return( -1 );
					}
					else
					{
						double thredhold = utility::ToDouble( get_attribute( reader, "threshold", "0.0" ) );
						std::string tag  = get_attribute( reader, "tag", "" );
						classifier.threshold( thredhold );
						classifier.tag( tag );

						double posw = utility::ToDouble( get_attribute( reader, "posw", "0.5" ) );
						double negw = utility::ToDouble( get_attribute( reader, "negw", "0.5" ) );
						classifier.positive_weight( posw );
						classifier.negative_weight( negw );

						if( type != get_attribute( reader, "type", type ) || structure != get_attribute( reader, "structure", structure ) )
						{
							return( -1 );
						}
					}
				}
				else if( sdepth + 1 == depth && xmlStrcasecmp( name, BAD_CAST "weak" ) == 0 )
				{
					// �㎯�ʊ�̏��𒊏o����
					weak_classifier weak;
					ret = read( weak, reader );

					if( ret == 1 )
					{
						classifier.add_classifier( weak );
					}
				}
				else if( depth > sdepth )
				{
					std::cerr << "Failed to parse the XML file." << std::endl;
					return( -1 );
				}
				break;

			case 15: // EndElement
				if( sdepth == depth && xmlStrcasecmp( name, BAD_CAST "classifier" ) == 0 )
				{
					isEnd = true;
				}
				break;

			case 3: // Text
				{
					const xmlChar *text = xmlTextReaderConstValue( reader );
					std::cout << ( const char * )text << std::endl;
				}
				break;

			default:
			case 0: // None
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

		return( 1 );
	}

	/// @brief libxml2��API�𗘗p���ăJ�X�P�[�h�^AdaBoost���ʊ�̐ݒ���擾����֐�
	inline int read( xmlTextReaderPtr reader, cascaded_classifier &classifier, const std::string &type = "", const std::string &structure = "" )
	{
		if( reader == NULL )
		{
			return( -1 );
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
			case 1: // Element
				if( sdepth == depth )
				{
					if( xmlStrcasecmp( name, BAD_CAST "cascade" ) != 0 )
					{
						std::cerr << "Root element of XML should be started with <cascade> tag." << std::endl;
						return( -1 );
					}
					else
					{
						std::string tag  = get_attribute( reader, "tag", "" );
						classifier.tag( tag );

						double posw = utility::ToDouble( get_attribute( reader, "posw", "0.5" ) );
						double negw = utility::ToDouble( get_attribute( reader, "negw", "0.5" ) );
						classifier.positive_weight( posw );
						classifier.negative_weight( negw );

						if( type != get_attribute( reader, "type", type ) || structure != get_attribute( reader, "structure", structure ) )
						{
							return( -1 );
						}
					}
				}
				else if( sdepth + 1 == depth && xmlStrcasecmp( name, BAD_CAST "classifier" ) == 0 )
				{
					// �㎯�ʊ�̏��𒊏o����
					boosted_classifier b;
					ret = read( reader, b );

					if( ret == 1 )
					{
						classifier.add_classifier( b );
					}
				}
				else if( depth > sdepth )
				{
					std::cerr << "Failed to parse the XML file." << std::endl;
					return( -1 );
				}
				break;

			case 15: // EndElement
				if( sdepth == depth && xmlStrcasecmp( name, BAD_CAST "cascade" ) == 0 )
				{
					isEnd = true;
				}
				break;

			default:
			case 0: // None
			case 2: // Attribute
			case 3: // Text
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

		return( 1 );
	}

	/// @brief libxml2��API�𗘗p����Multi-exit�^AdaBoost���ʊ�̐ݒ���擾����֐�
	inline int read( xmlTextReaderPtr reader, multi_exit_classifier &classifier, const std::string &type = "", const std::string &structure = "" )
	{
		if( reader == NULL )
		{
			return( -1 );
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
			case 1: // Element
				if( sdepth == depth )
				{
					if( xmlStrcasecmp( name, BAD_CAST "cascade" ) != 0 )
					{
						std::cerr << "Root element of XML should be started with <cascade> tag." << std::endl;
						return( -1 );
					}
					else
					{
						std::string tag  = get_attribute( reader, "tag", "" );
						classifier.tag( tag );

						double posw = utility::ToDouble( get_attribute( reader, "posw", "0.5" ) );
						double negw = utility::ToDouble( get_attribute( reader, "negw", "0.5" ) );
						classifier.positive_weight( posw );
						classifier.negative_weight( negw );

						if( type != get_attribute( reader, "type", type ) || structure != get_attribute( reader, "structure", structure ) )
						{
							return( -1 );
						}
					}
				}
				else if( sdepth + 1 == depth && xmlStrcasecmp( name, BAD_CAST "classifier" ) == 0 )
				{
					// �㎯�ʊ�̏��𒊏o����
					boosted_classifier b;
					ret = read( reader, b );

					if( ret == 1 )
					{
						for( size_t i = 0 ; i < b.classifiers( ).size( ) ; i++ )
						{
							classifier.add_classifier( b.classifiers( )[ i ] );
						}

						classifier.exits( ).push_back( classifier.classifiers( ).size( ) );
					}
				}
				else if( depth > sdepth )
				{
					std::cerr << "Failed to parse the XML file." << std::endl;
					return( -1 );
				}
				break;

			case 15: // EndElement
				if( sdepth == depth && xmlStrcasecmp( name, BAD_CAST "cascade" ) == 0 )
				{
					isEnd = true;
				}
				break;

			default:
			case 0: // None
			case 2: // Attribute
			case 3: // Text
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

		return( 1 );
	}

	/// @brief libxml2��API�𗘗p����AdaBoost���ʊ�̐ݒ��ۑ�����֐�
	inline int write( xmlTextWriterPtr writer, const boosted_classifier &classifier, cascaded_classifier::difference_type id = -1, const std::string &type = "", const std::string &version = "", size_t tag_depth = 0, const std::string &comment = "" )
	{
		if( writer == NULL )
		{
			return( -1 );
		}

		// AdaBoost���ʊ�̊J�n�^�O
		if( xmlTextWriterStartElement( writer, BAD_CAST "classifier" ) < 0 )
		{
			std::cerr << "Failed to write <classifier> tag." << std::endl;
			return( -1 );
		}

		// ���ʊ��ID���o�͂���
		if( id >= 0 )
		{
			if( xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "id", "%ld", id ) < 0 )
			{
				std::cerr << "Failed to add a property (classifier id) to XML tag." << std::endl;
				return( -1 );
			}
		}

		// ���ʊ�Ɏ��ʊ�̎�ނ��o�͂���
		if( type != "" )
		{
			if( xmlTextWriterWriteAttribute( writer, BAD_CAST "type", BAD_CAST type.c_str( ) ) < 0 )
			{
				std::cerr << "Failed to add a property (classifier type) to XML tag." << std::endl;
				return( -1 );
			}
		}

		// ���ʊ�̃o�[�W�������o�͂���
		if( version != "" )
		{
			if( xmlTextWriterWriteAttribute( writer, BAD_CAST "version", BAD_CAST version.c_str( ) ) < 0 )
			{
				std::cerr << "Failed to add a property to XML tag." << std::endl;
				return( -1 );
			}
		}

		// AdaBoost���ʊ�̔��ʋ��E�����肷�邵�����l
		if( xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "threshold", "%f", classifier.threshold( ) ) < 0 )
		{
			std::cerr << "Failed to add a property to XML tag." << std::endl;
			return( -1 );
		}

		// ���ʊ�ɕt������t�������o�͂���
		std::string tag = classifier.tag( );
		if( tag != "" )
		{
			if( xmlTextWriterWriteAttribute( writer, BAD_CAST "tag", BAD_CAST tag.c_str( ) ) < 0 )
			{
				std::cerr << "Failed to write information about training." << std::endl;
				return( -1 );
			}
		}

		// ���ʊ�̍\�z�Ɏg�p�����T���v���̏����d�݂��o�͂���
		if( xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "posw", "%.12lf", classifier.positive_weight( ) ) < 0 )
		{
			std::cerr << "Failed to write weight information used for training." << std::endl;
			return( -1 );
		}
		if( xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "negw", "%.12lf", classifier.negative_weight( ) ) < 0 )
		{
			std::cerr << "Failed to write weight information used for training." << std::endl;
			return( -1 );
		}

		// �R�����g����ݒ肷��
		if( comment != "" )
		{
			if( xmlTextWriterWriteFormatComment( writer, " %s ", comment.c_str( ) ) < 0 )
			{
				std::cerr << "Failed to write comments on a tag." << std::endl;
				return( -1 );
			}
		}

		// �R�����g����ݒ肷��
		if( classifier.comment( ) != "" )
		{
			if( xmlTextWriterWriteFormatComment( writer, " %s ", classifier.comment( ).c_str( ) ) < 0 )
			{
				std::cerr << "Failed to write comments on a tag." << std::endl;
				return( -1 );
			}
		}

		// �w�K���ʂ̏����o�͂���
		std::string info = classifier.info( );
		if( info != "" )
		{
			if( xmlTextWriterWriteFormatComment( writer, " %s ", info.c_str( ) ) < 0 )
			{
				std::cerr << "Failed to write information about training." << std::endl;
				return( -1 );
			}
		}

		// �g�p���Ă���㎯�ʊ�̐����o�͂���
		if( xmlTextWriterWriteFormatComment( writer, " The number of weak classifiers used in this classifier: %ld ", classifier.number_of_classifiers( ) ) < 0 )
		{
			std::cerr << "Failed to write information about training." << std::endl;
			return( -1 );
		}


		// AdaBoost���ʊ���̎㎯�ʊ�̐ݒ��ۑ�����
		const boosted_classifier::classifier_list_type &classifiers = classifier.classifiers( );
		for( size_t i = 0 ; i < classifiers.size( ) ; i++ )
		{
			int ret = write( writer, classifiers[ i ], i + 1, tag_depth + 1 );
			if( ret < 0 )
			{
				std::cerr << "Failed to write parameters of weak classifiers." << std::endl;
				return( -1 );
			}
		}

		// AdaBoost���ʊ�̏I���^�O
		if( xmlTextWriterEndElement( writer ) < 0 )
		{
			std::cerr << "Failed to write </classifier> tag." << std::endl;
			return( -1 );
		}

		return( 0 );
	}

	/// @brief libxml2��API�𗘗p���ăJ�X�P�[�h�^AdaBoost���ʊ�̐ݒ��ۑ�����֐�
	inline int write( xmlTextWriterPtr writer, const cascaded_classifier &cascade, cascaded_classifier::difference_type id = -1, const std::string &type = "", const std::string &version = "", size_t tag_depth = 0, const std::string &comment = "" )
	{
		if( writer == NULL )
		{
			return( -1 );
		}

		// �J�X�P�[�h�^���ʊ�̊J�n�^�O
		if( xmlTextWriterStartElement( writer, BAD_CAST "cascade" ) < 0 )
		{
			std::cerr << "Failed to write <cascade> tag." << std::endl;
			return( -1 );
		}

		// ���ʊ��ID���o�͂���
		if( id >= 0 )
		{
			if( xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "id", "%ld", id ) < 0 )
			{
				std::cerr << "Failed to add a property (classifier id) to XML tag." << std::endl;
				return( -1 );
			}
		}

		// ���ʊ�Ɏ��ʊ�̎�ނ��o�͂���
		if( type != "" )
		{
			if( xmlTextWriterWriteAttribute( writer, BAD_CAST "type", BAD_CAST type.c_str( ) ) < 0 )
			{
				std::cerr << "Failed to add a property (classifier type) to XML tag." << std::endl;
				return( -1 );
			}
		}

#if defined( __ADABOOST_CASCADE_TYPE__ ) && __ADABOOST_CASCADE_TYPE__ == 2
		if( xmlTextWriterWriteAttribute( writer, BAD_CAST "structure", BAD_CAST "nested" ) < 0 )
#else
		if( xmlTextWriterWriteAttribute( writer, BAD_CAST "structure", BAD_CAST "viola" ) < 0 )
#endif
		{
			std::cerr << "Failed to add a property (cascade type) to XML tag." << std::endl;
			return( -1 );
		}

		// ���ʊ�̃o�[�W�������o�͂���
		if( version != "" )
		{
			if( xmlTextWriterWriteAttribute( writer, BAD_CAST "version", BAD_CAST version.c_str( ) ) < 0 )
			{
				std::cerr << "Failed to add a property to XML tag." << std::endl;
				return( -1 );
			}
		}

		// ���ʊ�ɕt������t�������o�͂���
		std::string tag = cascade.tag( );
		if( tag != "" )
		{
			if( xmlTextWriterWriteAttribute( writer, BAD_CAST "tag", BAD_CAST tag.c_str( ) ) < 0 )
			{
				std::cerr << "Failed to write information about training." << std::endl;
				return( -1 );
			}
		}

		// ���ʊ�̍\�z�Ɏg�p�����T���v���̏����d�݂��o�͂���
		if( xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "posw", "%.12lf", cascade.positive_weight( ) ) < 0 )
		{
			std::cerr << "Failed to write weight information used for training." << std::endl;
			return( -1 );
		}
		if( xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "negw", "%.12lf", cascade.negative_weight( ) ) < 0 )
		{
			std::cerr << "Failed to write weight information used for training." << std::endl;
			return( -1 );
		}

		// �R�����g����ݒ肷��
		if( comment != "" )
		{
			if( xmlTextWriterWriteFormatComment( writer, " %s ", comment.c_str( ) ) < 0 )
			{
				std::cerr << "Failed to write comments on a tag." << std::endl;
				return( -1 );
			}
		}

		// �R�����g����ݒ肷��
		if( cascade.comment( ) != "" )
		{
			if( xmlTextWriterWriteFormatComment( writer, " %s ", cascade.comment( ).c_str( ) ) < 0 )
			{
				std::cerr << "Failed to write comments on a tag." << std::endl;
				return( -1 );
			}
		}

		// �w�K���ʂ̏����o�͂���
		if( xmlTextWriterWriteFormatComment( writer, " Lower bound of TP in each stage: %f ", cascade.minimum_true_positive_rate_per_stage( ) ) < 0 )
		{
			std::cerr << "Failed to write information about training." << std::endl;
			return( -1 );
		}
		if( xmlTextWriterWriteFormatComment( writer, " Upper bound of FP in each stage: %f ", cascade.maximum_false_positive_rate_per_stage( ) ) < 0 )
		{
			std::cerr << "Failed to write information about training." << std::endl;
			return( -1 );
		}
		if( xmlTextWriterWriteFormatComment( writer, " Upper bound of FP in a cascaded classifier: %f ", cascade.total_false_positive_rate( ) ) < 0 )
		{
			std::cerr << "Failed to write information about training." << std::endl;
			return( -1 );
		}
		if( xmlTextWriterWriteFormatComment( writer, " Maximum number of stages in a cascaded classifier: %ld ", cascade.maximum_number_of_classifiers_per_stage( ) ) < 0 )
		{
			std::cerr << "Failed to write information about training." << std::endl;
			return( -1 );
		}
		if( xmlTextWriterWriteFormatComment( writer, " Total number of weak classifiers used in AdaBoost classifiers: %ld ", cascade.total_number_of_classifiers( ) ) < 0 )
		{
			std::cerr << "Failed to write information about training." << std::endl;
			return( -1 );
		}

		// AdaBoost���ʊ�̃p�����[�^��ۑ�����
		const cascaded_classifier::classifier_list_type &classifiers = cascade.classifiers( );
		for( size_t i = 0 ; i < classifiers.size( ) ; i++ )
		{
			int ret = write( writer, classifiers[ i ], i + 1, "", "", tag_depth + 1, "" );
			if( ret < 0 )
			{
				std::cerr << "Failed to write parameters of AdaBoost classifier." << std::endl;
				return( -1 );
			}
		}

		// �J�X�P�[�h�^���ʊ�̏I���^�O
		if( xmlTextWriterEndElement( writer ) < 0 )
		{
			std::cerr << "Failed to write </cascade> tag." << std::endl;
			return( -1 );
		}

		return( 0 );
	}

	/// @brief libxml2��API�𗘗p����Multi-exit�^AdaBoost���ʊ�̐ݒ��ۑ�����֐�
	inline int write( xmlTextWriterPtr writer, const multi_exit_classifier &cascade, multi_exit_classifier::difference_type id = -1, const std::string &type = "", const std::string &version = "", size_t tag_depth = 0, const std::string &comment = "" )
	{
		if( writer == NULL )
		{
			return( -1 );
		}

		// �J�X�P�[�h�^���ʊ�̊J�n�^�O
		if( xmlTextWriterStartElement( writer, BAD_CAST "cascade" ) < 0 )
		{
			std::cerr << "Failed to write <cascade> tag." << std::endl;
			return( -1 );
		}

		// ���ʊ��ID���o�͂���
		if( id >= 0 )
		{
			if( xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "id", "%ld", id ) < 0 )
			{
				std::cerr << "Failed to add a property (classifier id) to XML tag." << std::endl;
				return( -1 );
			}
		}

		// ���ʊ�Ɏ��ʊ�̎�ނ��o�͂���
		if( type != "" )
		{
			if( xmlTextWriterWriteAttribute( writer, BAD_CAST "type", BAD_CAST type.c_str( ) ) < 0 )
			{
				std::cerr << "Failed to add a property (classifier type) to XML tag." << std::endl;
				return( -1 );
			}
		}

		if( xmlTextWriterWriteAttribute( writer, BAD_CAST "structure", BAD_CAST "multi-exit" ) < 0 )
		{
			std::cerr << "Failed to add a property (cascade type) to XML tag." << std::endl;
			return( -1 );
		}

		// ���ʊ�̃o�[�W�������o�͂���
		if( version != "" )
		{
			if( xmlTextWriterWriteAttribute( writer, BAD_CAST "version", BAD_CAST version.c_str( ) ) < 0 )
			{
				std::cerr << "Failed to add a property to XML tag." << std::endl;
				return( -1 );
			}
		}

		// ���ʊ�ɕt������t�������o�͂���
		std::string tag = cascade.tag( );
		if( tag != "" )
		{
			if( xmlTextWriterWriteAttribute( writer, BAD_CAST "tag", BAD_CAST tag.c_str( ) ) < 0 )
			{
				std::cerr << "Failed to write information about training." << std::endl;
				return( -1 );
			}
		}

		// ���ʊ�ɕt������t�������o�͂���
		std::string info = cascade.info( );
		if( info != "" )
		{
			if( xmlTextWriterWriteAttribute( writer, BAD_CAST "info", BAD_CAST info.c_str( ) ) < 0 )
			{
				std::cerr << "Failed to write information about training." << std::endl;
				return( -1 );
			}
		}

		// ���ʊ�̍\�z�Ɏg�p�����T���v���̏����d�݂��o�͂���
		if( xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "posw", "%.12lf", cascade.positive_weight( ) ) < 0 )
		{
			std::cerr << "Failed to write weight information used for training." << std::endl;
			return( -1 );
		}
		if( xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "negw", "%.12lf", cascade.negative_weight( ) ) < 0 )
		{
			std::cerr << "Failed to write weight information used for training." << std::endl;
			return( -1 );
		}

		// �R�����g����ݒ肷��
		if( comment != "" )
		{
			if( xmlTextWriterWriteFormatComment( writer, " %s ", comment.c_str( ) ) < 0 )
			{
				std::cerr << "Failed to write comments on a tag." << std::endl;
				return( -1 );
			}
		}

		// �R�����g����ݒ肷��
		if( cascade.comment( ) != "" )
		{
			if( xmlTextWriterWriteFormatComment( writer, " %s ", cascade.comment( ).c_str( ) ) < 0 )
			{
				std::cerr << "Failed to write comments on a tag." << std::endl;
				return( -1 );
			}
		}

		// �w�K���ʂ̏����o�͂���
		if( xmlTextWriterWriteFormatComment( writer, " Lower bound of TP in each stage: %f ", cascade.minimum_true_positive_rate_per_stage( ) ) < 0 )
		{
			std::cerr << "Failed to write information about training." << std::endl;
			return( -1 );
		}
		if( xmlTextWriterWriteFormatComment( writer, " Upper bound of FP in each stage: %f ", cascade.maximum_false_positive_rate_per_stage( ) ) < 0 )
		{
			std::cerr << "Failed to write information about training." << std::endl;
			return( -1 );
		}
		if( xmlTextWriterWriteFormatComment( writer, " Upper bound of FP in a cascaded classifier: %f ", cascade.total_false_positive_rate( ) ) < 0 )
		{
			std::cerr << "Failed to write information about training." << std::endl;
			return( -1 );
		}
		if( xmlTextWriterWriteFormatComment( writer, " Maximum number of stages in a cascaded classifier: %ld ", cascade.maximum_number_of_classifiers_per_stage( ) ) < 0 )
		{
			std::cerr << "Failed to write information about training." << std::endl;
			return( -1 );
		}
		if( xmlTextWriterWriteFormatComment( writer, " Total number of weak classifiers used in AdaBoost classifiers: %ld ", cascade.number_of_classifiers( ) ) < 0 )
		{
			std::cerr << "Failed to write information about training." << std::endl;
			return( -1 );
		}

		// AdaBoost���ʊ�̃p�����[�^��ۑ�����
		const multi_exit_classifier::classifier_list_type &classifiers = cascade.classifiers( );
		const multi_exit_classifier::exit_list_type &exits = cascade.exits( );
		size_t sid = 0;
		for( size_t j = 0 ; j < exits.size( ) ; j++ )
		{
			size_t eid = exits[ j ];

			// AdaBoost���ʊ�̊J�n�^�O
			if( xmlTextWriterStartElement( writer, BAD_CAST "classifier" ) < 0 )
			{
				std::cerr << "Failed to write <classifier> tag." << std::endl;
				return( -1 );
			}

			// ���ʊ��ID���o�͂���
			if( xmlTextWriterWriteFormatAttribute( writer, BAD_CAST "id", "%ld", j + 1 ) < 0 )
			{
				std::cerr << "Failed to add a property (classifier id) to XML tag." << std::endl;
				return( -1 );
			}

			for( size_t i = sid ; i < eid && i < classifiers.size( ) ; i++ )
			{
				int ret = write( writer, classifiers[ i ], i - sid + 1, tag_depth + 2 );
				if( ret < 0 )
				{
					std::cerr << "Failed to write parameters of weak classifiers." << std::endl;
					return( -1 );
				}
			}

			// AdaBoost���ʊ�̏I���^�O
			if( xmlTextWriterEndElement( writer ) < 0 )
			{
				std::cerr << "Failed to write </classifier> tag." << std::endl;
				return( -1 );
			}

			sid = eid;
		}

		// �J�X�P�[�h�^���ʊ�̏I���^�O
		if( xmlTextWriterEndElement( writer ) < 0 )
		{
			std::cerr << "Failed to write </cascade> tag." << std::endl;
			return( -1 );
		}

		return( 0 );
	}
}


/// @brief XML�t�@�C�����环�ʊ�iAdaBoost���ʊ�j�̏���ǂݍ���
//! 
//! @param[in]  classifier �c XML�̏�����ɏ��������鎯�ʊ�iAdaBoost���ʊ��j
//! @param[in]  filename   �c �ݒ��ǂݍ���XML�̃t�@�C����
//! 
template < typename CLASSIFIER >
bool read( CLASSIFIER &classifier, const std::string &filename, const std::string &type, const std::string &structure )
{
	// ���C�u���������������A�R���p�C������DLL�̃o�[�W�����̈Ⴂ�ɂ��API�̔�݊������e�X�g����B
	LIBXML_TEST_VERSION

	// XMLReader ������������
	xmlTextReaderPtr reader = xmlReaderForFile( filename.c_str( ), NULL, 0 );
	if( reader == NULL )
	{
		std::cerr << "Failed to open a file [" << filename << "].";
		return( false );
	}

	// ���ʊ������������
	classifier.initialize( );

	// �ŏ��̃f�[�^��ǂݎ��
	int ret = xmlTextReaderRead( reader );

	if( ret >= 0 )
	{
		// XML�t�@�C������f�[�^��ǂݎ��
		ret = io::read( reader, classifier, type, structure );
	}

	// XMLReader���m�ۂ��Ă��郊�\�[�X���������
	xmlFreeTextReader( reader );

	// XML ���C�u�����̃N���[���A�b�v�����s����B
	xmlCleanupParser( );

	if( ret < 0 )
	{
		// �e�X�g�p�Ƀ��������_���v����
		xmlMemoryDump( );

		return( false );
	}
	else
	{
		return( true );
	}
}

/// @brief XML�t�@�C�����环�ʊ�i�J�X�P�[�h�^AdaBoost���ʊ�AMulti-exit�^AdaBoost���ʊ�j�̏���ǂݍ���
//! 
//! @param[in]  classifier �c XML�̏�����ɏ��������鎯�ʊ�i�J�X�P�[�h�^AdaBoost���ʊ�AMulti-exit�^AdaBoost���ʊ�j
//! @param[in]  filename   �c �ݒ��ǂݍ���XML�̃t�@�C����
//! 
template < typename CLASSIFIER >
inline bool read_cascade( CLASSIFIER &classifier, const std::string &filename, const std::string &type, const std::string &structure )
{
	// ���C�u���������������A�R���p�C������DLL�̃o�[�W�����̈Ⴂ�ɂ��API�̔�݊������e�X�g����B
	LIBXML_TEST_VERSION

	// XMLReader ������������
	xmlTextReaderPtr reader = xmlReaderForFile( filename.c_str( ), NULL, 0 );
	if( reader == NULL )
	{
		std::cerr << "Failed to open a file [" << filename << "].";
		return( false );
	}

	// ���ʊ������������
	classifier.initialize( classifier.minimum_true_positive_rate_per_stage( ), classifier.maximum_false_positive_rate_per_stage( ),
											classifier.total_false_positive_rate( ), classifier.maximum_number_of_classifiers_per_stage( ) );

	// �ŏ��̃f�[�^��ǂݎ��
	int ret = xmlTextReaderRead( reader );

	if( ret >= 0 )
	{
		// XML�t�@�C������f�[�^��ǂݎ��
		ret = io::read( reader, classifier, type, structure );
	}

	// XMLReader���m�ۂ��Ă��郊�\�[�X���������
	xmlFreeTextReader( reader );

	// XML ���C�u�����̃N���[���A�b�v�����s����B
	xmlCleanupParser( );

	if( ret < 0 )
	{
		// �e�X�g�p�Ƀ��������_���v����
		xmlMemoryDump( );

		return( false );
	}
	else
	{
		return( true );
	}
}

/// @brief XML�t�@�C���֎��ʊ�iAdaBoost���ʊ�A�J�X�P�[�h�^AdaBoost���ʊ�AMulti-exit�^AdaBoost���ʊ�j�̏���ۑ�����
//! 
//! @param[in]  classifier �c XML�֏���ۑ����鎯�ʊ�iAdaBoost���ʊ�A�J�X�P�[�h�^AdaBoost���ʊ�j
//! @param[in]  filename   �c �ݒ��ۑ�����XML�̃t�@�C����
//! 
template < typename CLASSIFIER >
bool write( const CLASSIFIER &classifier, const std::string &filename, const std::string &type, const std::string &comment = "" )
{
	// ���C�u���������������A�R���p�C������DLL�̃o�[�W�����̈Ⴂ�ɂ��API�̔�݊������e�X�g����B
	LIBXML_TEST_VERSION

	// XMLReader ������������
	xmlTextWriterPtr writer = xmlNewTextWriterFilename( filename.c_str( ), 0 );
	if( writer == NULL )
	{
		std::cerr << "Failed to create a file [" << filename << "].";
		return( false );
	}

	int ret = -1;

	// XML�t�@�C���̃C���f���g��ݒ肷��
	ret = xmlTextWriterSetIndent( writer, 1 );
	if( ret < 0 )
	{
		std::cerr << "Failed to enable XML indentatation." << std::endl;
	}

	// XML�t�@�C���̃C���f���g�������^�u�ɐݒ肷��
	ret = xmlTextWriterSetIndentString( writer, BAD_CAST "\t" );
	if( ret < 0 )
	{
		std::cerr << "Failed to change XML indentat character." << std::endl;
	}

	ret = xmlTextWriterStartDocument( writer, NULL, "utf-8", NULL );
	if( ret < 0 )
	{
		std::cerr << "Failed to write XML tag." << std::endl;
	}

	if( ret >= 0 )
	{
		ret = io::write( writer, classifier, -1, type, __ADABOOST_CLASSIFIER_VERSION__, 0, comment );
	}

	if( ret >= 0 && xmlTextWriterEndDocument( writer ) < 0 )
	{
		std::cerr << "Failed to write XML tag." << std::endl;
		ret = -1;
	}


	// XMLReader���m�ۂ��Ă��郊�\�[�X���������
	xmlFreeTextWriter( writer );

	// XMLReader���m�ۂ��Ă��郊�\�[�X���������
	xmlCleanupParser( );

	if( ret < 0 )
	{
		// �e�X�g�p�Ƀ��������_���v����
		xmlMemoryDump( );
	}

	return( true );
}
