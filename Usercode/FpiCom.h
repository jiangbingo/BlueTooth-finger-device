
#ifndef FCFINGER_H
#define FCFINGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wblib.h"
#include "Defines.h"

	BYTE FpiSearch(BYTE *cpFeature, WORD wSearchFlag, WORD wEndIndex, WORD *wIndex);
	BYTE FpiGetSensorType(void);
	BYTE FpiGetImage(BYTE *cpImage, WORD wTimeout);
#endif
