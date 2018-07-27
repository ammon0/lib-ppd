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

#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>





void openLog(void){
	time_t    now;
	struct tm local_time;
	char      digits[16];
	char      logFileName[128] = "log/";
	
	
	now = time(NULL);
	local_time = *(localtime(&now));
	
	sprintf(digits,"%u",(local_time.tm_year+1900));
	strcat(logFileName,digits);
	strcat(logFileName,"_");
	
	sprintf(digits,"%u",(local_time.tm_mon+1));
	strcat(logFileName,digits);
	strcat(logFileName,"_");
	
	sprintf(digits,"%u",local_time.tm_mday);
	strcat(logFileName,digits);
	strcat(logFileName,"_");
	
	sprintf(digits,"%u",local_time.tm_hour);
	strcat(logFileName,digits);
	strcat(logFileName,"_");

	sprintf(digits,"%u",local_time.tm_min);
	strcat(logFileName,digits);
	strcat(logFileName,"_");

	sprintf(digits,"%u",local_time.tm_sec);
	strcat(logFileName,digits);

	strcat(logFileName,".log");
	
	
	if(msg_log_open(logfile, lm_replace, logFileName) == r_failure){
		error("openLog(): error opening %s\n", logFileName);
		return;
	}
	
	//msg_set_flag(logfile, MF_LOG_SYNC);
	msg_set_flag(logfile, MF_LOG_USEC);
	
	msg_print(logfile, V_INFO, "openLog(): log file opened\n");
	
}






