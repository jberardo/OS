#ifndef _KSAMP_H
#define _KSAMP_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

// ERROR CODES
#define ERR_MEMINFO 1;
#define ERR_STAT 2;
#define ERR_CPUINFO 3;
#define ERR_DISKSTATS 4;
#define ERR_OSRELEASE 5;
#define ERR_UPTIME 6;

#define PROC_MEMINFO "/proc/meminfo"
#define PROC_STAT "/proc/stat"
#define PROC_CPUINFO "/proc/cpuinfo"
#define PROC_DISKSTATS "/proc/diskstats"
#define PROC_OSRELEASE "/proc/sys/kernel/osrelease"
#define PROC_UPTIME "/proc/uptime"

#define CLEARSCR system ( "clear" )

// contains some system information
// all info retrieved from /proc
struct information
{
	// processor type
	char cpu_type[80];
	// kernel version
	char kernel_version[80];
	// amount of time since last boot
	int time_since_boot;
	int time_last_boot;
	// amount of time spent in user mode
	double cpu_user;
	// amount of time spent in system mode
	double cpu_system;
	// amount of time spent idle
	double cpu_idle;
	// totam memory
	double mem_total;
	// available memory
	double mem_available;
	// number of disk read requests
	double disk_read;
	// number of disk write requests
	double disk_write;
	// number of context switches
	double ctx_switch;
	// number of processes that have been created since last boot
	double procs;
} info;

char error_flag;

// number of times we will loop and call update function
// used to calculate the averages at the end
int total_calls;
int current_call;
//
// FUNCTIONS
//
//
// general function to open a file
FILE *open_file(const char *file);

// update /proc/stats
int update_stats(void);

// output and formatting
void print_summary(void);
void print_date(char *desc, unsigned long time);
void print_time(char *desc, int time);

void usage(char prog[10]);

#endif
