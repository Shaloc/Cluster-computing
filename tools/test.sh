#!/bin/sh


#Test sequence 0 

#for devices in $(seq 16 -2 2); do
#echo "[info] using $devices nodes"
#echo "*********node: $devices **************" >> result.txt
#for interval in $(seq 80000 80000 1040000); do
#    echo "********** iter: $interval *************" >> result.txt
#    echo "[info] iter $interval start"
#    mpiexec -np $devices -f mpi_config_file python PI.py $interval >> result.txt
#    echo "[info] iter ${interval} done"
#done
#echo "[info] device ${devices} done"
#done

#Test sequence 1
for device in $(seq 10 -5 5); do
    echo "[info] using $device nodes"
    echo "########### Using $device nodes #############" >> result.py
    echo "devices = $device " >> result.py
    for iter in $(seq 200 -50 50); do
	echo "########## iter: $iter #############" >> result.py
	echo "result_$device.append(' ')" >> result.py
	echo "[info] iter $iter start"
	echo "'''" >> result.py
	mpiexec -np $device python matrix.py $iter >> result.py	
	echo "'''" >> result.py
	echo "[info] iter $iter done"
    done
    echo "device $device done"
done




