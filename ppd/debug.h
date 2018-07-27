/*******************************************************************************
 *
 *	PExe-VM : Portable Executable Virtual Machine
 *
 *	Copyright (c) 2018 Ammon Dodson
 *	You should have received a copy of the license terms with this software. If
 *	not, please visit the project homepage at:
 *	https://github.com/ammon0/pexe-vm
 *
 ******************************************************************************/


#ifndef _DEBUG_H
#define _DEBUG_H

#ifdef __cplusplus
	extern "C" {
#endif

#include <util/msg.h>
#include <stdlib.h>

// exists only once in debug.o
#ifdef _DEBUG_C
	log_descriptor logfile = NULL;
#else
	extern log_descriptor logfile;
#endif

// open and close the error file
#define closeLog() msg_log_close(logfile)
void openLog(void);

// report things
#define error(...) msg_print(logfile, V_ERROR, __VA_ARGS__)
#define warn(...)  msg_print(logfile, V_WARN , __VA_ARGS__)
#define debug(...) msg_print(logfile, V_DEBUG, __VA_ARGS__)
#define trace(...) msg_trace(logfile         , __VA_ARGS__)

// kill the process
#define FATAL(...) \
	error(__VA_ARGS__); \
	closeLog(); \
	exit(EXIT_FAILURE)

#define V_DEFAULT V_NOTE

#ifdef __cplusplus
	}
#endif

#endif // _DEBUG_H


