#!/bin/sh

ntpd
ntpd
ntpd
ntpd
ntpd
ntpd
ntpd

date
echo "========== Initializing test environment ==========="
cd /root
/root/net_test
/root/scp.sh PI.py
/root/scp.sh matrix.py

use_mpi="-f mpi_config_file"
blank=" "
line="===================================="
work_dir=/root/results

if [ -d /root/results ]; then
	rm -rf /root/results/*
	echo -e "Working directory clean."
else
	echo -e "Making work directory."
	mkdir -p /root/results
fi

echo $line
echo "Test sequence 1 start"
echo $line
# Test sequence 1  -- Floating Point, Calculate PI
for devices in $(seq 16 -2 2); do
echo "[info][$(date)] using $devices nodes"
echo "*********node: $devices **************" >> $work_dir/result_multi_fl.dat
	for iter in $(seq 20000 20000 1040000); do # Will generate (1040000-20000)/2 + 1 = 52 results
	    echo "********** iter: $iter *************" >> $work_dir/result_multi_fl.dat
	    echo "[info][$(date)] iter $iter start"
	    mpiexec -np $devices $use_mpi python PI.py $iter >> $work_dir/result_multi_fl.dat
	    echo "[info][$(date)] iter ${iter} done"
	done
echo "[info][$(date)] device ${devices} done"
done

echo $line
echo "Test sequence 2 start"
echo $line
# Test sequence 2 -- Do these tests on a single device
for devices in $(seq 16 -2 2); do
echo "[info][$(date)] using $devices threads"
echo "*********node: $devices **************" >> result_single_fl.dat
	for iter in $(seq 20000 20000 1040000); do
	    echo "********** iter: $iter *************" >> result_single_fl.dat
	    echo "[info][$(date)] iter $iter start"
	    mpiexec -np $devices python PI.py $iter >> result_single_fl.dat
	    echo "[info][$(date)]iter ${iter} done"
	done
echo "[info][$(date)] threads ${devices} done"
done


echo $line
echo "Test sequence 3 start"
echo $line

# Test sequence 3 -- Fixed point using matrix multiply
# device = 2, 10 20 30 40 50 50 70 80 90 100 110 120 --12 results
devices=2
echo "[info][$(date)] using $devices nodes"
for iter in $(echo 10 20 30 40 50 50 70 80 90 100 110 120); do
	echo "********** iter: $iter *************" >> result_multi_fi.dat
	echo "[info][$(date)] iter $iter start"
	mpiexec -np $devices $use_mpi python matrix.py $iter >> result_multi_fi.dat
	echo "[info][$(date)]iter ${iter} done"
done
# device = 4, 20 32 44 56 68 80 92 104 116 128 140 152 --12 results
devices=4
echo "[info][$(date)] using $devices nodes"
for iter in $(echo 20 32 44 56 68 80 92 104 116 128 140 152); do
	echo "********** iter: $iter *************" >> result_multi_fi.dat
	echo "[info][$(date)] iter $iter start"
	mpiexec -np $devices $use_mpi python matrix.py $iter >> result_multi_fi.dat
	echo "[info][$(date)]iter ${iter} done"
done
# device = 6 , 18 30 42 54 66 78 90 102 114 126 138 150--12 results
devices=6
echo "[info][$(date)] using $devices nodes"
for iter in $(echo 18 30 42 54 66 78 90 102 114 126 138 150); do
	echo "********** iter: $iter *************" >> result_multi_fi.dat
	echo "[info][$(date)] iter $iter start"
	mpiexec -np $devices $use_mpi python matrix.py $iter >> result_multi_fi.dat
	echo "[info][$(date)]iter ${iter} done"
done
# device = 8 , 32 40 48 56 64 72 80 96 104 112 120 144--12 results
devices=8
echo "[info][$(date)] using $devices nodes"
for iter in $(echo 32 40 48 56 64 72 80 96 104 112 120 144); do
	echo "********** iter: $iter *************" >> result_multi_fi.dat
	echo "[info][$(date)] iter $iter start"
	mpiexec -np $devices $use_mpi python matrix.py $iter >> result_multi_fi.dat
	echo "[info][$(date)]iter ${iter} done"
done
# device = 10 , 20 30 50 60 70 80 90 100 120 130 140 150--12 results
devices = 10
echo "[info][$(date)] using $devices nodes"
for iter in $(echo 20 30 50 60 70 80 90 100 120 130 140 150); do
	echo "********** iter: $iter *************" >> result_multi_fi.dat
	echo "[info][$(date)] iter $iter start"
	mpiexec -np $devices $use_mpi python matrix.py $iter >> result_multi_fi.dat
	echo "[info][$(date)]iter ${iter} done"
done
# device = 12 , 12 24 36 48 60 72 84 96 108 120 132 144--12 results
devices = 12
echo "[info][$(date)] using $devices nodes"
for iter in $(echo 12 24 36 48 60 72 84 96 108 120 132 144); do
	echo "********** iter: $iter *************" >> result_multi_fi.dat
	echo "[info][$(date)] iter $iter start"
	mpiexec -np $devices $use_mpi python matrix.py $iter >> result_multi_fi.dat
	echo "[info][$(date)]iter ${iter} done"
done
# device = 14 , 14 28 42 56 70 84 98 112 126 140 154 168--12 results
devices = 14
echo "[info][$(date)] using $devices nodes"
for iter in $(echo 14 28 42 56 70 84 98 112 126 140 154 168); do
	echo "********** iter: $iter *************" >> result_multi_fi.dat
	echo "[info][$(date)] iter $iter start"
	mpiexec -np $devices $use_mpi python matrix.py $iter >> result_multi_fi.dat
	echo "[info][$(date)]iter ${iter} done"
done
# device = 16 , 16 32 48 64 80 96 112 128 144 160 176 192--12 results
devices = 16
echo "[info][$(date)] using $devices nodes"
for iter in $(echo 16 32 48 64 80 96 112 128 144 160 176 192); do
	echo "********** iter: $iter *************" >> result_multi_fi.dat
	echo "[info][$(date)] iter $iter start"
	mpiexec -np $devices $use_mpi python matrix.py $iter >> result_multi_fi.dat
	echo "[info][$(date)]iter ${iter} done"
done

echo $line
echo "Test sequence 4 start"
echo $line

# Test sequence 4 -- Fixed point using matrix multiply -- single device
# device = 2, 10 20 30 40 50 50 70 80 90 100 110 120 --12 results
devices=2
echo "[info][$(date)] using $devices nodes"
for iter in $(echo 10 20 30 40 50 50 70 80 90 100 110 120); do
	echo "********** iter: $iter *************" >> result_single_fi.dat
	echo "[info][$(date)] iter $iter start"
	mpiexec -np $devices  python matrix.py $iter >> result_single_fi.dat
	echo "[info][$(date)] iter ${iter} done"
done
# device = 4, 20 32 44 56 68 80 92 104 116 128 140 152 --12 results
devices=4
echo "[info][$(date)] using $devices nodes"
for iter in $(echo 20 32 44 56 68 80 92 104 116 128 140 152); do
	echo "********** iter: $iter *************" >> result_single_fi.dat
	echo "[info][$(date)] iter $iter start"
	mpiexec -np $devices  python matrix.py $iter >> result_single_fi.dat
	echo "[info][$(date)]iter ${iter} done"
done
# device = 6 , 18 30 42 54 66 78 90 102 114 126 138 150--12 results
devices=6
echo "[info][$(date)] using $devices nodes"
for iter in $(echo 18 30 42 54 66 78 90 102 114 126 138 150); do
	echo "********** iter: $iter *************" >> result_single_fi.dat
	echo "[info][$(date)] iter $iter start"
	mpiexec -np $devices  python matrix.py $iter >> result_single_fi.dat
	echo "[info][$(date)]iter ${iter} done"
done
# device = 8 , 32 40 48 56 64 72 80 96 104 112 120 144--12 results
devices=8
echo "[info][$(date)] using $devices nodes"
for iter in $(echo 32 40 48 56 64 72 80 96 104 112 120 144); do
	echo "********** iter: $iter *************" >> result_single_fi.dat
	echo "[info][$(date)] iter $iter start"
	mpiexec -np $devices  python matrix.py $iter >> result_single_fi.dat
	echo "[info][$(date)]iter ${iter} done"
done
# device = 10 , 20 30 50 60 70 80 90 100 120 130 140 150--12 results
devices = 10
echo "[info][$(date)] using $devices nodes"
for iter in $(echo 20 30 50 60 70 80 90 100 120 130 140 150); do
	echo "********** iter: $iter *************" >> result_single_fi.dat
	echo "[info][$(date)] iter $iter start"
	mpiexec -np $devices  python matrix.py $iter >> result_single_fi.dat
	echo "[info][$(date)]iter ${iter} done"
done
# device = 12 , 12 24 36 48 60 72 84 96 108 120 132 144--12 results
devices = 12
echo "[info][$(date)] using $devices nodes"
for iter in $(echo 12 24 36 48 60 72 84 96 108 120 132 144); do
	echo "********** iter: $iter *************" >> result_single_fi.dat
	echo "[info][$(date)] iter $iter start"
	mpiexec -np $devices  python matrix.py $iter >> result_single_fi.dat
	echo "[info][$(date)]iter ${iter} done"
done
# device = 14 , 14 28 42 56 70 84 98 112 126 140 154 168--12 results
devices = 14
echo "[info][$(date)] using $devices nodes"
for iter in $(echo 14 28 42 56 70 84 98 112 126 140 154 168); do
	echo "********** iter: $iter *************" >> result_single_fi.dat
	echo "[info][$(date)] iter $iter start"
	mpiexec -np $devices  python matrix.py $iter >> result_single_fi.dat
	echo "[info][$(date)]iter ${iter} done"
done
# device = 16 , 16 32 48 64 80 96 112 128 144 160 176 192--12 results
devices = 16
echo "[info][$(date)] using $devices nodes"
for iter in $(echo 16 32 48 64 80 96 112 128 144 160 176 192); do
	echo "********** iter: $iter *************" >> result_single_fi.dat
	echo "[info][$(date)] iter $iter start"
	mpiexec -np $devices  python matrix.py $iter >> result_single_fi.dat
	echo "[info][$(date)]iter ${iter} done"
done