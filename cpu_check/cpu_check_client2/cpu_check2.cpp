#include <stdlib.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>


using namespace std;

#define SERV_PORT 8087

struct out_frame
{
	char name[10];
	int data;
};

struct cpu_stat_t
{
    char cpu[5];
    long int user;
    long int nice;
    long int system;
    long int idle;
    long int iowait;
    long int irq;
    long int softirq;
    long int stealstolen;
    long int gust;
};

cpu_stat_t cpu_stat[2];

int main(int argc,char** argv)
{
	int clientfd = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERV_PORT);
	serverAddr.sin_addr.s_addr = inet_addr(argv[2]);
        char name[10];
        memcpy(name,argv[1],strlen(argv[1])+1);
	int flag = connect(clientfd, (sockaddr *)&serverAddr, sizeof(serverAddr));
	if (flag < 0)
	{
		cout << "connect error" << endl;
		close(clientfd);
		return -1;
	}
	else
	{
		while (1)
		{
            FILE *fin;
            fin = fopen("/proc/stat","r");
            if(fin == NULL)
            {
               cout<<"open file error"<<endl;
               return -1;
            }
		    char buff[128];
		    fgets(buff,128,fin);
		    sscanf(buff,"%s%d%d%d%d%d%d%d%d%d",cpu_stat[0].cpu,&cpu_stat[0].user,&cpu_stat[0].nice,&cpu_stat[0].system,
             &cpu_stat[0].idle,&cpu_stat[0].iowait,&cpu_stat[0].irq,&cpu_stat[0].softirq,&cpu_stat[0].stealstolen,
             &cpu_stat[0].gust);
            usleep(200000);
            fclose(fin);
            fin = fopen("/proc/stat","r");
            if(fin == NULL)
            {
               cout<<"open file error"<<endl;
               return -1;
            }
            fgets(buff,128,fin);
            sscanf(buff,"%s%d%d%d%d%d%d%d%d%d",cpu_stat[1].cpu,&cpu_stat[1].user,&cpu_stat[1].nice,&cpu_stat[1].system,
            &cpu_stat[1].idle,&cpu_stat[1].iowait,&cpu_stat[1].irq,&cpu_stat[1].softirq,&cpu_stat[1].stealstolen,
            &cpu_stat[1].gust);
            int allTime1 = cpu_stat[0].user+cpu_stat[0].nice+cpu_stat[0].system+cpu_stat[0].idle+cpu_stat[0].iowait
            +cpu_stat[0].irq+cpu_stat[0].softirq+cpu_stat[0].stealstolen+cpu_stat[0].gust;
            int allTime2 = cpu_stat[1].user+cpu_stat[1].nice+cpu_stat[1].system+cpu_stat[1].idle+cpu_stat[1].iowait
            +cpu_stat[1].irq+cpu_stat[1].softirq+cpu_stat[1].stealstolen+cpu_stat[1].gust;
            int usage = 100-(cpu_stat[1].idle - cpu_stat[0].idle)*100/(allTime2-allTime1);
            fclose(fin);
			out_frame temp_frame;
			memcpy(temp_frame.name,name,strlen(name)+1);
			temp_frame.data = htonl(usage);
			send(clientfd,(char *)&temp_frame,sizeof(temp_frame),0);
		}
	}
}
