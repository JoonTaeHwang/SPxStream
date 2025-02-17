/*********************************************************************
*
* (c) Copyright 2012, 2015, 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for base64 encode and decode from 
*	http://www.adp-gmbh.ch/cpp/common/base64.html
*
* Revision Control:
*   02/05/18 v1.4    AGC	Remove priv from name.
*
* Previous Changes:
*   05/04/18 1.3    AGC	Move to SPxLibUtils.
*   22/10/15 1.2    AGC	Rename private headers.
*   02/08/12 1.1    AGC	Initial Version.
**********************************************************************/
/* 
   base64.cpp and base64.h

   Copyright (C) 2004-2008 Ren� Nyffenegger

   This source code is provided 'as-is', without any express or implied
   warranty. In no event will the author be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this source code must not be misrepresented; you must not
      claim that you wrote the original source code. If you use this source code
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original source code.

   3. This notice may not be removed or altered from any source distribution.

   Ren� Nyffenegger rene.nyffenegger@adp-gmbh.ch

*/

#include <string>

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len);
std::string base64_decode(std::string const& encoded_string);

/*********************************************************************
*
*   End of File
*
**********************************************************************/
