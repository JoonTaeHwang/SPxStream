/*********************************************************************
*
* (c) Copyright 2017, 2018, 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	SHA1 header file.
*
* Revision Control:
*   26/08/20 v1.5    AGC	Support legacy mode for final().
*
* Previous Changes:
*   02/05/18 1.4    AGC	Remove priv from name.
*   05/04/18 1.3    AGC	Move to SPxLibUtils.
*   14/11/17 1.2    AGC	Add this header.
*			Support Linux builds.
*   13/11/17 1.1    AGC	Initial version.
*
**********************************************************************/
/*
    sha1.hpp - header of

    ============
    SHA-1 in C++
    ============

    100% Public Domain.

    Original C Code
        -- Steve Reid <steve@edmweb.com>
    Small changes to fit into bglibs
        -- Bruce Guenter <bruce@untroubled.org>
    Translation to simpler C++ Code
        -- Volker Grabsch <vog@notjusthosting.com>
    Safety fixes
        -- Eugene Hopkinson <slowriot at voxelstorm dot com>
*/

#ifndef SHA1_HPP
#define SHA1_HPP


#include "SPxLibUtils/SPxTypes.h"
#include <iostream>
#include <string>


class SHA1
{
public:
    SHA1();
    void update(const std::string &s);
    void update(std::istream &is);
    std::string final(int useLegacy=FALSE);
    void final(unsigned char(&result)[20]);
    static std::string from_file(const std::string &filename);

private:
    UINT32 digest[5];
    std::string buffer;
    UINT64 transforms;
};


#endif /* SHA1_HPP */
