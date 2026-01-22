


/*
	C Project
	Address Book

	Team A: Fenil, Flaby, Shridhar

	26/Nov/2012

	Implementation C File

	All Error, Warning and Information Logging

*/


#include <stdio.h>
#include "abk.h"

#define ABK_LOG_FILE "ABK.LOG"

return_t abk_log (int result, char *msgtype, char *msg)
{
	FILE *fp = fopen(ABK_LOG_FILE, "a");
	if (fp == NULL)
	{
		perror(ABK_LOG_FILE);
		return result;
	}

	fprintf(fp, "%s!: %s %s %s\n", msgtype, __TIME__, __DATE__, msg);
	fclose(fp);
	printf("%s!: %s %s %s\n", msgtype, __TIME__, __DATE__, msg);
	WAIT_FOR_ENTER_KEY;
	return result;
}

return_t log_info(return_t result, char *msg)
{
	return abk_log(result, "INFO", msg);
}

return_t log_warn(return_t result, char *msg)
{
	return abk_log(result, "WARN", msg);
}

return_t log_error(return_t result, char *msg)
{
	return abk_log(result, "ERROR", msg);
}

