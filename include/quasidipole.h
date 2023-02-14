/*

    QuasiDipole: quasidipole.h

    Copyright (C) 2023  Johnathan K Burchill

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _QUASIDIPOLE_H
#define _QUASIDIPOLE_H

#define QD_YEAR_UPDATE_INTERVAL 0.1

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

enum quasiDipoleStatus
{
    QD_OK = 0,
    QD_MEM,
    QD_FILE_READ,
    QD_VOID_ARG,
    QD_TIME_CALC,
    QD_NO_COEFF_FILENAME
};

int initQuasiDipleCoefficients(char *filename, double year);
void freeQuasiDipoleCoefficients(void);

int geographicToQuasiDipole(char *coeffFilename, double unixTime, double geodeticLatitude, double longitude, double altitudeKm, double *qdLatitude, double *qdLongitude);

double solarTimeDifferenceHours(double dateAsYear);

int quasiDipoleToGeographic(char *coeffFilename, double unixTime, double qdLatitude, double qdLongitude, double altitudeKm, double *geodeticLatitude, double *longitude);
double quasiDipoleMagneticLocalTime(char *coeffFilename, double unixTime, double qdLatitude, double qdLongitude, double *mlt);

int quasiDipoleSphericalHarmonics(double theta, double phi);
int alfBasisInit(int32_t nmax0in, int32_t mmax0in);
int alfBasis(int32_t nmax, int32_t mmax, double theta);
double dotprod(double *a, double *b, size_t len);

#endif // _QUASIDIPOLE_H
