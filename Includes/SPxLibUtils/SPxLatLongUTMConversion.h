/*********************************************************************
*
* (c) Copyright 2009 - 2014, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxLatLongUTMConversion.h,v $
* ID: $Id: SPxLatLongUTMConversion.h,v 1.7 2014/07/04 14:47:16 rew Exp $
*
* Purpose:
*   LatLong <-> UTM conversion.
*
*   Open source code obtained from:
*   http://www.gpsy.com/gpsinfo/geotoutm
*
* Revision Control:
*   04/07/14 v1.7    AGC	Remove unused string header.
*
* Previous Changes:
*   17/02/14 1.6    AGC	Use const where appropriate.
*   18/11/13 1.5    AGC	Fix clang warning.
*   16/12/10 1.4    REW	Fix previous change.
*   16/12/10 1.3    REW	Add doNotNormaliseLong to LLtoUTMgivenZone().
*   10/03/10 1.2    REW	Add LLtoUTMgivenZone().
*   02/10/09 1.1    SP 	Initial version
**********************************************************************/

//definitions for lat/long to UTM and UTM to lat/lng conversions
#ifndef SPX_LATLONGCONV
#define SPX_LATLONGCONV

void LLtoUTM(int ReferenceEllipsoid,
		const double Lat, const double Long, 
		double &UTMNorthing, double &UTMEasting, char* UTMZone);
void LLtoUTMgivenZone(int ReferenceEllipsoid,
		const double Lat, const double Long, 
		double &UTMNorthing, double &UTMEasting, char* UTMZone,
		int doNotNormaliseLong=0);
void UTMtoLL(int ReferenceEllipsoid,
		const double UTMNorthing, const double UTMEasting,
		const char* UTMZone,
		double& Lat,  double& Long );
char UTMLetterDesignator(double Lat);
void LLtoSwissGrid(const double Lat, const double Long, 
			double &SwissNorthing, double &SwissEasting);
void SwissGridtoLL(const double SwissNorthing, const double SwissEasting, 
			double& Lat, double& Long);


class Ellipsoid
{
public:
	Ellipsoid(){}
	Ellipsoid(int Id, const char* name, double radius, double ecc)
	{
		id = Id; ellipsoidName = name; 
		EquatorialRadius = radius; eccentricitySquared = ecc;
	}

	int id;
	const char* ellipsoidName;
	double EquatorialRadius; 
	double eccentricitySquared;  

};



#endif

/*********************************************************************
*
* End of file
*
**********************************************************************/
