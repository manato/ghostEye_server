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

#ifndef __INCLUDE_ADABOOST_CLASSIFIER__
#define __INCLUDE_ADABOOST_CLASSIFIER__

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


// AdaBoost���O��Ԃ̎n�܂�
_ADABOOST_BEGIN


// AdaBoost ���O��Ԃ��������ɂ���
#define __NO_ADABOOST_NAMESPACE__	1


// Discrete AdaBoost �� discrete ���O��ԂŃC���|�[�g
namespace discrete
{
	#include "discrete_adaboost.h"
}

// Real AdaBoost �� real ���O��ԂŃC���|�[�g
namespace real
{
	#include "real_adaboost.h"
}

// Gentle AdaBoost �� gentle ���O��ԂŃC���|�[�g
namespace gentle
{
	#include "gentle_adaboost.h"
}

#undef __NO_ADABOOST_NAMESPACE__


// AdaBoost���O��Ԃ̏I���
_ADABOOST_END


#endif	// __INCLUDE_ADABOOST_CLASSIFIER__