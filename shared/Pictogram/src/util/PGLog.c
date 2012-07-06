//
//  PGLog.c
//  MobilePoly
//
//  Created by David Wagner on 06/07/2012.
//  Copyright (c) 2012 Noise & Heat. All rights reserved.
//

#include <stdio.h>
#include <stdarg.h>
#include "Pictogram.h"

PGResult _pgLogv(PGLogLevel level, const char *message, ...)
{
    va_list args;
    va_start(args, message);
	
	vprintf(message, args);
	
	va_end(args);
	
	return PGR_OK;
}
