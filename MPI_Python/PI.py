#!/usr/bin/python3
from mpi4py import MPI
import time

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

if rank == 0:
    iter = input("Iteration:")   
    piter = int(int(iter)/size)   
else:
    iter = None
    piter = None

comm.barrier()#等所有进程运行到这步开始计时
tic = time.time()
piter = comm.bcast(piter,root=0)


PI = 0.0
for i in range(rank*piter,rank*piter+piter):
    if i%2 == 0:
        PI = PI + 1/(2*i+1)
    else:
        PI = PI - 1/(2*i+1)

toc = time.time()
plife = 1000*(toc-tic)

print("p"+str(rank)+" lives: "+str(plife))
recv_PI = comm.gather(PI, root=0)   
life = comm.gather(plife,root=0)

if rank == 0:
    PI = sum(recv_PI)*4
    print(str(PI))
    print(life)

    print("Use "+ str(max(life)) + "ms")