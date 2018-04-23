
#when using 
#mpirun -np 2 python mpi_pi.py means 2 processors
import mpi4py.MPI as MPI
import time

def f(x):
	return 4.0/(1.0 + x*x)

comm = MPI.COMM_WORLD
comm_rank = comm.Get_rank()
comm_size = comm.Get_size()

ndone = True
n = 0
NUM = 0.0000001

while ndone:
	if comm_rank == 0:
		if n == 0:
			n = 10000000
		else:
			n = 0
	#TIME
	time_start = time.time()
	n = comm.bcast(n if comm_rank == 0 else None, root=0)
	if n == 0:
		ndone = False
	else:
		h = NUM
		fsum = 0
		for i in range(comm_rank+1,n+1,comm_size):
			x = h * (i - 0.5)
			fsum = fsum + f(x)
		comm_pi = h * fsum
		print "processor %d/%d, comm_pi is %.17f" % (comm_rank+1,comm_size,comm_pi)

		cpi = comm.reduce(comm_pi,root = 0,op = MPI.SUM)
		if comm_rank == 0:
			#TIME
			time_end = time.time()
			print "This is processor%d, the PI I calulate is %.17f" % (comm_rank,cpi)
			print "total time:"
			print str(time_end-time_start)+'s'