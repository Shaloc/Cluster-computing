#ifndef MPI_H
#define MPI_H


int mympi_init();//初始化mympi
int mympi_sendmsg(char* hostname,char* data,int n);//这个函数用以给hostname发送n字节数据，调用后立即返回，返回实际发送的数据长度，错误时返回-1
int mympi_recvmsg(char* hostname,char* data,int n);//这个函数用以接收hostname的数据，调用后阻塞，直到接收到n字节数据或者接收缓冲区为空时返回，返回实际接收的数据长度，错误时返回-1



extern int myNodeID;
#endif // MPI_H
