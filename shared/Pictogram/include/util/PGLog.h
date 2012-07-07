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


#define pgLog(level, message, ...) _pgLogv((level), ("[%s:%d] " message) , __FILE__, __LINE__, ## __VA_ARGS__)
//#define pgLog(...) ;
	
void _pgLogv(PGLogLevel level, const char *fmt, ...);
void pgSetLoggingLevel(PGLogLevel level);
	
#ifdef __cplusplus
}
#endif


#endif
