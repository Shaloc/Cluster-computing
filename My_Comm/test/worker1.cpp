/*
 * @Author: Bowen GAO [shalocn@outlook.com] 
 * @Date: 2018-06-12 17:36:42 
 * @Last Modified by: Bowen GAO [shalocn@outlook.com]
 * @Last Modified time: 2018-06-12 18:18:14
 */

#include "../src/mympi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * main: entry
 * argv[1]: worker's number(1, 2...)
 */
int main(int argc, char** argv){
    bool nDone = true;
    int n = 10000000;
    double num = 0.0000000001;
    int worker = int(argv[1][0]);
    int totalWorker = 2;
    double local, sum, width, mypi, pi;
    
    if(mympi_init() != 1 || argv[1] == NULL){
        printf("STDERR: No other nodes or no proper argument input\n");
        return -1;
    }

    width = 1.0 / n;
    sum = 0.0;
    
    for(int i = worker; i < n; i+= totalWorker){
        local = width*((double)i + 0.5);
        sum += 4.0/(1.0 + local*local);
    } 
    mypi = width*sum;

    char result[100];
    sprintf(result, "%.17f", mypi);
    
    if(worker != 1){
        mympi_sendmsg("g0dev1", result, strlen(result));
    }
    else
    {
        char workerResult[10][100];
        double workerPi[10];

        strcpy(workerResult[1], result);
        /*
         * 此处接收是一个阻塞函数所以发送和接收需要同步
         *这里应该是一个可以优化的地方，即设置一个消息队列机制，保证发送的数据不会因为阻塞在上一个接收而无法接收
         */
        mympi_recvmsg("g0dev2", workerResult[2], 100);
        for(int j = 0; j <= totalWorker; j++)
        {
            printf("worker %d's pi is %s\n", j, workerResult[j]);
            workerPi[j] = atof(workerResult[j]);
            pi += workerPi[j];
        }
        printf("Worker[0]: pi is %.17f\n", pi);
    }
    return 0;
}