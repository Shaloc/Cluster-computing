#include "mympi.h"
#include <iostream>

using namespace std;
int main()
{
    mympi_init();
    char* hostname = "csnetlab_1";
    char* str = "hello";
    mympi_sendmsg(hostname,str,6);
    char buff[6];
    mympi_recvmsg(hostname,buff,6);
    cout<<"recieved:"<<buff<<endl;
}
