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

void _pgLogv(PGLogLevel level, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
}
