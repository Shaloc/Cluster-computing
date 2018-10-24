import mpi4py.MPI as mpi
import matplotlib.pyplot as plt
import time
import random
import numpy as np

comm=mpi.COMM_WORLD
rank=comm.Get_rank()
data2send=[random.randint(1,100)]
for m in range(0,99):
    data2send.append(random.randint(1,100))
comm.send(data2send,1)
recv_data=comm.recv()
times=np.zeros(99)

if rank==0:
    for i in range(1,100):
        time_begin=time.time()
        for count in range(0,10):
            comm.send(data2send,1)
            recv_data=comm.recv()
        times[i-1]=(time.time()-time_begin)/10
        for m in range(0,100):
            data2send.append(random.randint(1,100))
    plt.plot(range(800,80000,800),times)
    plt.xlabel("Datasize/byte")
    plt.ylabel("Time/s")
    plt.show()

if rank==1:
    recv_data=comm.recv()
    comm.send(recv_data,0)
    for i in range(1,100):
        for count in range(0,10):
            recv_data=comm.recv()
            comm.send(recv_data,0)
