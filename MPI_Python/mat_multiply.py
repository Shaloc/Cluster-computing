from mpi4py import MPI
import numpy as np

# 使用方矩阵计算
numberRows = 500
numberColumns = 500
Master = 0

print ("Initialising variables.\n")
a = np.zeros(shape=(numberRows, numberColumns))
b = np.zeros(shape=(numberRows, numberColumns))
c = np.zeros(shape=(numberRows, numberColumns))

def populateMatrix( p ):
    for i in range(0, numberRows):
        for j in range(0, numberColumns):
            p[i][j] = i+j

comm = MPI.COMM_WORLD
commSize = comm.Get_size()
commRank = comm.Get_rank()
processorName = MPI.Get_processor_name()



# 计算每个节点的工作
if (commSize == 1):
    slice = numberRows
else:
    slice = numberRows / (commSize-1)

if slice <= 1:
	print 'E/Slice < 1'
	exit()

populateMatrix(b)

comm.Barrier()
    
if commRank == Master:
    print ("Start")
    populateMatrix(a)
        
    for i in range(1, commSize):
        offset = (i-1)*slice #0, 10, 20
        row = a[offset,:]
        comm.send(offset, dest=i, tag=i)
        comm.send(row, dest=i, tag=i)
        for j in range(0, slice):
            comm.send(a[j+offset,:], dest=i, tag=j+offset)
    print ("All sent to workers.\n")

comm.Barrier()

if commRank != Master:

    print ("Data Received from process %d.\n" % (commRank))
    offset = comm.recv(source=0, tag=commRank)
    recv_data = comm.recv(source=0, tag=commRank)
    for j in range(1, slice):
        c = comm.recv(source=0, tag=j+offset)
        recv_data = np.vstack((recv_data, c))

    print ("Start Calculation from process %d.\n" % (commRank))

    #Loop through rows
    t_start = MPI.Wtime()
    for i in range(0, slice):
        res = np.zeros(shape=(numberColumns))
        if (slice == 1):
            r = recv_data
        else:
            r = recv_data[i,:]
        ai = 0
        for j in range(0, numberColumns):
            q = b[:,j] #get the column we want
            for x in range(0, numberColumns):
                res[j] = res[j] + (r[x]*q[x])
            ai = ai + 1
        if (i > 0):
           send = np.vstack((send, res))
        else:
            send = res
    t_diff = MPI.Wtime() - t_start
    
    print("Process %d finished in %5.4fs.\n" %(commRank, t_diff))
    comm.Send([send, MPI.FLOAT], dest=0, tag=commRank) #1, 12, 23

comm.Barrier()

if commRank == Master:  
    res1 = np.zeros(shape=(slice, numberColumns))
    comm.Recv([res1, MPI.FLOAT], source=1, tag=1)
    print ("Received response from 1.\n")
    kl = np.vstack((res1))
    for i in range(2, commSize):
        resx= np.zeros(shape=(slice, numberColumns))
        comm.Recv([resx, MPI.FLOAT], source=i, tag=i)
        print ("Received response from %d.\n" % (i))
        kl = np.vstack((kl, resx))
    print ("End")
  	print ("Result AxB.\n")
    print (kl)   

comm.Barrier()