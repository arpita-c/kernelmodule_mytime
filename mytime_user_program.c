#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define N 20

#define MAX_LENGTH 200

int main(int argc, char const *argv[])
{
	struct timeval gtodTimes[N];
	char *procClockTimes[N];
	int fd;
	ssize_t bytes_read;



	for(int i = 0; i < N; i++)
	{
		procClockTimes[i] = (char *)malloc(MAX_LENGTH*sizeof(char));
	}



	for(int i = 0; i< N; i++)
	{
		fd = open("/dev/mytime", O_RDONLY);
	        if (fd < 0)
        	{
                perror("mytime_user_program");
				return -1;
	        }

	        
		gettimeofday(&gtodTimes[i],0);


		bytes_read = read(fd,procClockTimes[i],MAX_LENGTH);



        fprintf(stdout,"%s\n",procClockTimes[i]);



		if(bytes_read < 0)
		{
			perror("mytime_user_program");
			return -1;
		}
		close(fd);
	}
	





	for(int i = 0; i < N; i++)
	{
		fprintf(stdout,"Gettimeofday: %ld\n%s\n",gtodTimes[i].tv_usec*1000, procClockTimes[i]);
	}




	for(int i = 0; i<N; i++)
	{
		free(procClockTimes[i]);
	}

	return 0;
}
