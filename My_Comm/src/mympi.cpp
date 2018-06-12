#include "mympi.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include <pthread.h>
#include <iostream>

using namespace std;


#define WAITTIME 5 //等待其他节点调用mympi_init() 单位秒
#define CONNECTION_PORT 8086
#define MAX_NODE_NUM 10
struct neighbor
{
    char hostname[32];//主机名
    unsigned int ip;//ip地址
    unsigned int nodeID;//节点ID，取IP地址的最后一个字节
    int sockfd;//用以通信的套接字
};

void mympi_neighborEntryInit();//提取topology.dat中的信息初始化邻居表
char myhostname[32];
unsigned int myIP = -1;
extern unsigned int myNodeID = -1;

vector<neighbor> neighborEntry;


//这个函数初始化邻居表中nodeID对应的sockfd，成功返回0，失败返回-1
int mympi_addSockfd(unsigned int nodeID,int sockfd)
{
    for(unsigned int i=0;i<neighborEntry.size();i++)
    {
        if(neighborEntry[i].nodeID == nodeID)
        {
            cout<<nodeID<<" has connected"<<endl;
            neighborEntry[i].sockfd = sockfd;
            return 0;
        }
    }
    return -1;
}
// 这个线程打开TCP端口CONNECTION_PORT, 等待节点ID比自己大的所有邻居的进入连接,
// 在所有进入连接都建立后, 这个线程终止.
void* waitNbrs(void* arg)
{
    cout<<"waitNbrs:thread started"<<endl;
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in serverAddr;
	struct sockaddr_in clientAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = CONNECTION_PORT;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(sockfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
	listen(sockfd,MAX_NODE_NUM);
	unsigned int i=0;
	unsigned int cnt = 0;
	for(i=0;i<neighborEntry.size();i++)
    {
        if(neighborEntry[i].nodeID>myNodeID)
        {
            cnt++;
        }
    }
    for(i=0;i<cnt;i++)
    {
        socklen_t clientAddrLen = sizeof(clientAddr);
        int temp_sockfd = accept(sockfd,(struct sockaddr*)&clientAddr,&clientAddrLen);
        int temp_nodeID = clientAddr.sin_addr.s_addr >> 24;
        cout<<"waitNbrs thread:";
        mympi_addSockfd(temp_nodeID,temp_sockfd);
    }
    cout<<"waitNbrs:thread stopped"<<endl;
    close(sockfd);
    pthread_exit(NULL);
}
// 这个函数连接到节点ID比自己小的所有邻居.
// 在所有外出连接都建立后, 返回1, 否则返回-1.
int connectNbrs()
{
    unsigned int i = 0;
    for(i=0;i<neighborEntry.size();i++)
    {
        if(neighborEntry[i].nodeID<myNodeID)
        {
            int sockfd = socket(AF_INET,SOCK_STREAM,0);
            struct sockaddr_in serverAddr;
            serverAddr.sin_family = AF_INET;
            serverAddr.sin_port = CONNECTION_PORT;
            serverAddr.sin_addr.s_addr = neighborEntry[i].ip;
            if(connect(sockfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr))<0)
            {
                return -1;
            }
            cout<<"main:";
            mympi_addSockfd(neighborEntry[i].nodeID,sockfd);
        }
    }
    return 1;
}

int mympi_init()//这个函数用以初始化mpi
{
    gethostname(myhostname,32);
    hostent* temphost = gethostbyname(myhostname);
    myIP = *(in_addr_t*)temphost->h_addr_list[0];
    myNodeID = myIP >> 24;
    mympi_neighborEntryInit();
    pthread_t waitNbrThread;
    pthread_create(&waitNbrThread,NULL,waitNbrs,NULL);
    sleep(WAITTIME);
    if(connectNbrs() < 0)
        return -1;
    pthread_join(waitNbrThread,NULL);
    return 1;
}

int mympi_sendmsg(char* hostname,char* data,int n)//这个函数用以给hostname发送n字节数据，调用后立即返回，返回实际发送的数据长度,错误时返回-1
{
    for(unsigned int i=0;i<neighborEntry.size();i++)
    {
        if(strcmp(neighborEntry[i].hostname,hostname) == 0)
        {
            return (send(neighborEntry[i].sockfd,data,n,0));
        }
    }
    return -1;
}

int mympi_recvmsg(char* hostname,char* data,int n)//这个函数用以接收hostname的数据，调用后阻塞，直到接收到n字节数据或者接收缓冲区为空时返回，返回实际接收的数据长度，错误时返回-1
{
    for(unsigned int i=0;i<neighborEntry.size();i++)
    {
        if(strcmp(neighborEntry[i].hostname,hostname) == 0)
        {
            return (recv(neighborEntry[i].sockfd,data,n,0));
        }
    }
    return -1;
}


void mympi_neighborEntryInit()
{
    FILE *fp;
    fp = fopen("topology.dat","r");
    char buff[32];
    while(fgets(buff,32,fp)!=NULL)
    {
        char hostname[32];
        sscanf(buff,"%s",hostname);
        if(strcmp(myhostname,hostname) != 0)
        {
            neighbor temp_neighbor;
            memcpy(temp_neighbor.hostname,hostname,32);
            hostent* temphost = gethostbyname(hostname);
            temp_neighbor.ip = *(in_addr_t*)temphost->h_addr_list[0];
            temp_neighbor.nodeID = temp_neighbor.ip >> 24;
            temp_neighbor.sockfd = -1;
            neighborEntry.push_back(temp_neighbor);
        }
    }
    fclose(fp);
}
