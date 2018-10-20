#!/usr/bin/python3
from mpi4py import MPI
import time
import sys

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

if rank == 0:
    iter = int(sys.argv[1])
    piter = int(int(iter)/size)   
else:
    iter = None
    piter = None

comm.barrier()
tic = time.time()
piter = comm.bcast(piter,root=0)


PI = 0.0
for i in range(rank*piter,rank*piter+piter):
    if i%2 == 0:
        PI = PI + 1.0/(2.0*i+1.0)
    else:
        PI = PI - 1.0/(2.0*i+1.0)

toc = time.time()
plife = 1000*(toc-tic)

print("Process"+str(rank)+" lives: "+str(plife)+ "ms")
recv_PI = comm.gather(PI, root=0)   
life = comm.gather(plife,root=0)

if rank == 0:
    PI = sum(recv_PI)*4.0
    print("Result is " + str(PI))
    print("=======================================")
    print("Use "+ str(max(life)) + "ms")
    print("=======================================")
