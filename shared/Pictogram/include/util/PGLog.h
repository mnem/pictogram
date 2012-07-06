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

#define pgLog(level, message, ...) _pgLogv((level), ("[%s:%s in %s] " message) , __FILE__, __LINE__, __func__, ## __VA_ARGS__)
//#define pgLog(...) ;
	
PGResult _pgLogv(PGLogLevel level, const char *message, ...);
	
#ifdef __cplusplus
}
#endif


#endif
