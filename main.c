/*

    QuasiDipole: main.c

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


#include "quasidipole.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

int main(int argc, char *argv[])
{
    int status = QD_OK;

    int nOptions = 0;

    for (int i = 0; i < argc; i++)
    {
        if (strcmp("--help", argv[i]) == 0)
        {
            fprintf(stdout, "You would like some help?\n");
            return 0;
        }
        else if (strcmp("--about", argv[i]) == 0)
        {
            fprintf(stdout, "You would like to know about this program?\n");
            return 0;
        }
    }

    int expectedArgC = 2;
    if (argc - 1 - nOptions != expectedArgC)
    {
        fprintf(stderr, "usage: %s <coeff-filename> <positions-filename>\n", argv[0]);
        return 1;
    }

    char *coeffFile = argv[1];
    char *positionsFile = argv[2];

    fprintf(stdout, "%20s : %s\n", "coefficient file", coeffFile);
    fprintf(stdout, "%20s : %s\n", "positions file", positionsFile);

    // Read in positions
    FILE *posFile = fopen(positionsFile, "r");
    if (posFile == NULL)
    {
        fprintf(stderr, "error opening positions-file %s\n", positionsFile);
        return QD_FILE_READ;        
    }

    char line[80] = {0};
    char *readline = NULL;
    long count = 0;
    do
    {
        readline = fgets(line, 80, posFile);
        count++;
    } while (readline != NULL);

    if (count == 0)
    {
        status = QD_FILE_READ;
        goto cleanup;
    }    

    double *times = malloc(count * sizeof *times);
    double *glat = malloc(count * sizeof *glat);
    double *glon = malloc(count * sizeof *glon);
    double *galt = malloc(count * sizeof *galt);
    double *qdlat = malloc(count * sizeof *qdlat);
    double *qdlon = malloc(count * sizeof *qdlon);
    double *qdmlt = malloc(count * sizeof *qdmlt);

    if (times == NULL || glat == NULL || glon == NULL || galt == NULL || qdlat == NULL || qdlon == NULL || qdmlt == NULL)
    {
        status = QD_MEM;
        goto cleanup;
    }

    status = fseek(posFile, 0, SEEK_SET);
    if (status != 0)
    {
        status = QD_FILE_READ;
        goto cleanup;
    }

    int nRead = 0;
    count = 0;
    while ((readline = fgets(line, 80, posFile)) != NULL)
    {
        nRead = sscanf(line, "%lf %lf %lf %lf", times + count, glat + count, glon + count, galt + count);
        if (nRead != 4)
        {
            status = QD_FILE_READ;
            goto cleanup;
        }
        count++;
    }

    for (long i = 0; i < count; i++)
    {
        status = geographicToQuasiDipole(coeffFile, times[i], glat[i], glon[i], galt[i], qdlat + i, qdlon + i);
        status = quasiDipoleMagneticLocalTime(coeffFile, times[i], qdlat[i], qdlon[i], qdmlt + i);
    }

    fprintf(stdout, "%9s%9s%9s%9s%9s%9s%9s\n", "Time (S)", "LAT","LON","ALT","QDLAT","QDLON","QDMLT");
    for (long i = 0; i < count; i++)
    {
        fprintf(stdout, "%9.0lf%9.1lf%9.1lf%9.1lf%9.2lf%9.2lf%9.2lf\n", times[i], glat[i], glon[i], galt[i], qdlat[i], qdlon[i], qdmlt[i]);
    }

    for (long i = 0; i < count; i++)
    {
        status = quasiDipoleToGeographic(coeffFile, times[i], qdlat[i], qdlon[i], galt[i], glat + i, glon + i);
    }
    fprintf(stdout, "%9s%9s%9s%9s%9s%9s\n", "Time (S)", "QDLAT","QDLON","ALT","LAT","LON");
    for (long i = 0; i < count; i++)
    {
        fprintf(stdout, "%9.0lf%9.1lf%9.1lf%9.1lf%9.2lf%9.2lf\n", times[i], qdlat[i], qdlon[i], galt[i], glat[i], glon[i]);
    }
    

cleanup:
    if (posFile != NULL)
        fclose(posFile);

    free(times);
    free(glat);
    free(glon);
    free(galt);
    free(qdlat);
    free(qdlon);
    free(qdmlt);

    freeQuasiDipoleCoefficients();

    return 0;

}
