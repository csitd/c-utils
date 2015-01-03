#include <stdio.h> 
#include <sys/sysinfo.h>


int main()
{ 
	struct sysinfo si;
	sysinfo(&si); 
	printf("free mem      %10lu\n", si.freeram /= 1024);
	float d = si.uptime; 
	printf("uptime hours  %10f\n", (d / 60 ) / 60);
	printf("buffer mem    %10lu\n", si.bufferram /= 1024);
	printf("total mem     %10lu\n", si.totalram /= 1024);
	printf("shared mem    %10lu\n", si.sharedram /= 1024);
	printf("mem unit      %10u\n", si.mem_unit);
	printf("used mem      %10lu\n", (si.totalram - si.freeram - si.bufferram)); 
	return 0;

}
