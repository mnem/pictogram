//
//  PGLog.h
//
//  Created by David Wagner on 06/07/2012.
//  Copyright (c) 2012 Noise & Heat. All rights reserved.
//

#ifndef PGLog_h
#define PGLog_h

#ifdef __cplusplus
extern "C" {
#endif

#include "../core/PGDataTypes.h"
	
#ifdef DISABLE_pgLog
#	define pgLog(...) ;
#	define _pgLogv(level, fmt, ...) ;
#	define pgLogSetLevel(level) ;
#else
#	define pgLog(level, message, ...) _pgLogv((level), ("[%s:%d] " message "\n"), __FILE__, __LINE__, ## __VA_ARGS__)
	void _pgLogv(PGLogLevel level, const char *fmt, ...);
	void pgLogSetLevel(PGLogLevel level);
#endif

#ifdef DISABLE_pgLogAnyGlErrors
#	define pgLogAnyGlErrors(...) ;
#else
#	define pgLogAnyGlErrors(message, ...) _pgLogAnyGlErrorsv(("[%s:%d] --GL ERROR--" message "\n"), __FILE__, __LINE__, ## __VA_ARGS__)
	void _pgLogAnyGlErrorsv(const char *fmt, ...);
#endif
	
#ifdef __cplusplus
}
#endif


#endif
