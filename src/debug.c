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

#define _DEBUG_C


#include <ppd/debug.h>

#include <util/string.h>
#include <util/io.h>

#include <time.h>
#include <stdlib.h>


#define FILE_NAME_SZ 128
#define DIGITS_SZ 16


void openLog(void){
	time_t    now;
	struct tm local_time;
	char      digits[DIGITS_SZ];
	char      logFileName[FILE_NAME_SZ] = "log/";
	
	
	now = time(NULL);
	local_time = *(localtime(&now));
	
	snprintf(digits, DIGITS_SZ,"%u",(local_time.tm_year+1900));
	strlcat(logFileName,digits, FILE_NAME_SZ);
	strlcat(logFileName,"_", FILE_NAME_SZ);
	
	snprintf(digits, DIGITS_SZ,"%u",(local_time.tm_mon+1));
	strlcat(logFileName,digits, FILE_NAME_SZ);
	strlcat(logFileName,"_", FILE_NAME_SZ);
	
	snprintf(digits, DIGITS_SZ,"%u",local_time.tm_mday);
	strlcat(logFileName,digits, FILE_NAME_SZ);
	strlcat(logFileName,"_", FILE_NAME_SZ);
	
	snprintf(digits, DIGITS_SZ,"%u",local_time.tm_hour);
	strlcat(logFileName,digits, FILE_NAME_SZ);
	strlcat(logFileName,"_", FILE_NAME_SZ);

	snprintf(digits, DIGITS_SZ,"%u",local_time.tm_min);
	strlcat(logFileName,digits, FILE_NAME_SZ);
	strlcat(logFileName,"_", FILE_NAME_SZ);

	snprintf(digits, DIGITS_SZ,"%u",local_time.tm_sec);
	strlcat(logFileName,digits, FILE_NAME_SZ);

	strlcat(logFileName,".log", FILE_NAME_SZ);
	
	
	if(msg_log_open(logfile, lm_replace, logFileName) == r_failure){
		error("openLog(): error opening %s\n", logFileName);
		return;
	}
	
	//msg_set_flag(logfile, MF_LOG_SYNC);
	msg_set_flag(logfile, MF_LOG_USEC);
	
	msg_print(logfile, V_INFO, "openLog(): log file opened\n");
	
}






