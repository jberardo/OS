#ifndef _KSAMP_C
#define _KSAMP_C

#include "ksamp.h"

int main (int argc, char *argv[])
{
	// check arguments
	if (argc != 3)
	{
		usage(argv[0]);
		
		return 1;
	}
	
	// amount of time in seconds for each system info retrieval
	int run_interval;
	// total amount of time in seconds the program should be running
	int run_total;
	error_flag = 0;
	
	// check user input
	// and assign the secunds for a loop and the total time
	if (!(run_interval = atoi(argv[1]) && (run_total = atoi(argv[2]))))
	{
		usage(argv[0]);
		
		return 1;
	}

	// total number that the loop should run
	// also use to calculae the averages
	total_calls = run_total / run_interval;
	current_call = 1;

	// run for a total of X times, X = (total time) / (loop time)
	for (int i = 0; i < total_calls; i++)
	{
		// update all information
		switch(update_stats())
		{
			// no errors
			case 0:
				// increment current loop index to average values
				current_call++;
				break;
			// no "break;" on all erros so we can print all errors that occurred
			// and break at the end (case default)
			case 1:
				printf("Error retrieving memory information.\n");
				error_flag = ERR_MEMINFO;
			case 2:
				printf("Error retrieving stat information.\n");
				error_flag = ERR_STAT;
			case 3:
				printf("Error retrieving cpu information.\n");
				error_flag = ERR_CPUINFO;
			case 4:
				printf("Error retrieving os release information.\n");
				error_flag = ERR_OSRELEASE;
			case 5:
				printf("Error retrieving uptime information.\n");
				error_flag = ERR_UPTIME;
			default:
				break;
		}
		
		// sleep for a amount of seconds
		sleep(run_interval);
				
		if (error_flag > 0)
		{
			return error_flag;
		}
	}
	 
	print_summary();
	
	return 0;
}

int update_stats()
{
	// temp array to parse unused strings in files with 'scanf'
	char unused[80];
	
	// file identifier
	FILE *fp;

	// buffer to read the file
	char buffer[256];

	//
	// CPU Info
	//
	// read /proc/cpuinfo file
	if (!(fp = open_file(PROC_CPUINFO)))
	{
		error_flag = ERR_CPUINFO;
	}
	else
	{
		while (!feof(fp))
		{
			if (fgets(buffer, 255, fp) == NULL)
			{
				break;
			}
	
			if (strncmp(buffer, "model name", 10) == 0)
			{
				sscanf(buffer, "%*s %*s %*s %[^\n]", info.cpu_type);
			}
		}
		
		// close file
		fclose(fp);
	}

	//
	// Kernel Version
	//
	// read /proc/sys/kernel/osrelease file
	if (!(fp = open_file(PROC_OSRELEASE)))
	{
		error_flag = ERR_OSRELEASE;
	}
	else
	{
		fscanf(fp, "%s", info.kernel_version);

		// close file
		fclose(fp);
	}
	
	//
	// Uptime
	//
	// read /proc/uptime
	if (!(fp = open_file(PROC_UPTIME)))
	{
		error_flag = ERR_UPTIME;
	}
	else
	{
		fscanf(fp, "%d", &info.time_since_boot);
		fclose(fp);
	}

	//	
	// Mem Info
	//
	if(!(fp = open_file(PROC_MEMINFO)))
	{
		error_flag = ERR_MEMINFO;
	}
	else
	{
		while (!feof(fp))
		{
			if (fgets(buffer, 255, fp) == NULL)
			{
				break;
			}

			if (strncmp(buffer, "MemTotal:", 9) == 0)
			{
				sscanf(buffer, "%*s %lf", &info.mem_total);
			}
			else if (strncmp(buffer, "MemFree:", 8) == 0)
			{
				double buff_mem_available = info.mem_available;
				sscanf(buffer, "%*s %lf", &info.mem_available);
				info.mem_available += buff_mem_available;
			}
		}
		
		fclose(fp);
	}

	//	
	// Disk I/O read/write requests
	//
	if(!(fp = open_file(PROC_DISKSTATS)))
	{
		error_flag = ERR_DISKSTATS;
	}
	else
	{
		while (!feof(fp))
		{
			if (fgets(buffer, 255, fp) == NULL)
			{
				break;
			}

			if (strstr(buffer, "sda "))
			{
				if (current_call > 1)
				{
					double buff_read = info.disk_read;
					double buff_write = info.disk_write;
					
					sscanf(buffer, "%*u %*u %*s %lf %*u %*u %*u %lf", &buff_read, &buff_write);
					
					info.disk_read += buff_read;
					info.disk_write += buff_write;
				}
				else
				{
					sscanf(buffer, "%*u %*u %*s %lf %*u %*u %*u %lf", &info.disk_read, &info.disk_write);
				}
			}
		}
	}
	
	// update stats for:
	// time cpu spent in
	// 		- user mode
	// 		- system mode
	// 		- idle time
	// Context switches
	// Time since last boot
	// Processes since last boot
	
	// read /proc/stat
	if(!(fp = open_file(PROC_STAT)))
	{
		error_flag = ERR_STAT;
	}
	
	{	
		while (!feof(fp))
		{
			if (fgets(buffer, 255, fp) == NULL)
			{
				break;
			}

			if ((strncmp(buffer, "cpu ", 4) == 0))
			{
				double buff_user  = info.cpu_user;
				double buff_system  = info.cpu_system;
				double buff_idle  = info.cpu_idle;
				
				sscanf(buffer, "%s %lf %s %lf %lf", unused, &info.cpu_user, unused, &info.cpu_system, &info.cpu_idle);
				info.cpu_user /= 1000;
				info.cpu_system /= 1000;
				info.cpu_idle /= 1000;
				info.cpu_user += buff_user;
				info.cpu_system += buff_system;
				info.cpu_idle += buff_idle;
				
			}
			else if ((strncmp(buffer, "ctxt", 4) == 0))
			{
				double buff_ctx = info.ctx_switch;
				
				sscanf(buffer, "%s %lf", unused, &info.ctx_switch);
				
				info.ctx_switch += buff_ctx;
			}
			else if ((strncmp(buffer, "btime", 5) == 0))
			{
				sscanf(buffer, "%s %d", unused, &info.time_last_boot);
			}
			else if ((strncmp(buffer, "processes", 9) == 0))
			{
				double buff_procs = info.procs;
				
				sscanf(buffer, "%s %lf", unused, &info.procs);
				info.procs += buff_procs;
			}
		}
	}
	
	fclose(fp);
	
	return error_flag;
}

FILE *open_file(const char *file)
{
	FILE *fp;

	fp = fopen(file, "r");

	if (!fp) {
		return NULL;
	}

	return fp;
}

void print_time(char *desc, int time)
{
	int days = time / 60 / 60 / 24;
	int hours = (time / 60 / 60) % 24;
	int minutes = (time / 60) % 60;
	//int seconds = time % 60;
	
	printf("%s\t%02d days, %02d hours and %02d minutes\n", desc, days, hours, minutes);	
}

void print_date(char *desc, unsigned long time)
{
	char timestamp[64] = "";
	time_t temp;
	memset(timestamp, '\0', sizeof(timestamp));
	temp = time;
	strftime(timestamp, sizeof(timestamp), "%c", localtime(&temp));
	
	printf("%s\t%s\n", desc, timestamp);
}

void print_summary(void)
{
	
	CLEARSCR;
	
	printf("---------------------------------------\n");		
	printf("System Info\n");
	printf("\tCPU Type\t%s\n", info.cpu_type);
	printf("\tKernel Version\t%s\n", info.kernel_version);
	printf("---------------------------------------\n");	
	printf("Boot\n");
	print_time("\tUp Since", info.time_since_boot);
	print_date("\tBoot Time", info.time_last_boot);
	printf("---------------------------------------\n");	
	printf("Processes\n");
	printf("\tTotal Procs\t%.0lf\n", (info.procs / total_calls));
	printf("\tCtx Switches\t%.0lf times\n", (info.ctx_switch / total_calls));
	printf("---------------------------------------\n");
	printf("Memory\n");
	printf("\tTotal\t\t%.2lf Mb\n", (info.mem_total/ 1000));
	printf("\tAvailable\t%.2lf Mb\n", ((info.mem_available / 1000) / total_calls));
	printf("---------------------------------------\n");
	printf("Disk I/O requests\n");
	printf("\tDisk Read\t%.0lf\n", (info.disk_read / total_calls));
	printf("\tDisk Write\t%.0lf\n", (info.disk_write / total_calls));
	printf("---------------------------------------\n");
	printf("Amount CPU time\n");
	print_time("\tUser mode", (info.cpu_user / total_calls));
	print_time("\tSystem mode", (info.cpu_system / total_calls));
	print_time("\tIdle time", (info.cpu_idle / total_calls));
}

void usage(char prog[10])
{
	printf("Wrong number of arguments. Usage: \n");
	printf("%s x y\n", prog);
	printf("x: run every x secnods\n");
	printf("y: run for a total of y seconds\n");
}

#endif // end define _KSAMP_C
