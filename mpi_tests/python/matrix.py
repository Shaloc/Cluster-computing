# -- coding: utf-8 --
import numpy as np
import time 
from mpi4py import MPI
from sys import argv

#计算 N*N 的两个矩阵相乘，N由入口参数传入

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

name,N = argv
N = int(N)
#产生A并按行分解发送，产生B并广播发送
A = None
B = np.zeros((N,N),dtype=int)
if rank == 0:
    A = np.random.randint(100,size=(N,N))  
#print('A = '+str(A))
    B = np.random.randint(100,size=(N,N))

comm.barrier()
tic = time.time()
rA =  np.zeros((int(N/size),N), dtype=int)
comm.Scatter(A, rA, root=0)
comm.Bcast(B,root=0)
#print('P'+str(rank)+' has A='+ str(rA))
#print('P'+str(rank)+' has B='+ str(B))
#每个节点计算数组，用循环不用numpy

C = np.zeros((int(N/size),N),dtype=int)
for i in range(int(N/size)):
    for j in range(N):
        #计算C[i][j]
        for k in range(N):
            C[i][j] = C[i][j] + rA[i][k]*B[k][j]

toc = time.time()
plife = 1000*(toc-tic)
print("Process"+str(rank)+" lives: "+str(plife)+ "ms")
#print('P'+str(rank)+' has C='+ str(C))
#发送数据
#Gather 按进程号合并列表
res = None
if rank == 0:
    res = np.zeros((N,N),dtype = int)
comm.Gather(C,res,root=0)
life = comm.gather(plife,root=0)
if rank == 0:
    #print('res is '+str(res))
    print("Use "+ str(max(life)) + "ms totally")
