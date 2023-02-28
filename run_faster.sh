#!/bin/bash

db_names=("faster")

trap 'kill $(jobs -p)' SIGINT


workload_dir=workloads

tns=(1 2 4 8 16)
workload=($workload_dir/workloada.spec $workload_dir/workloadb.spec)
output_file=faster_output2.result
rocksdb_data=/mnt/nvme1/faster_data

rm $output_file

for file_name in ${workload[*]}; do
    for db_name in ${db_names[@]}; do
      for tn in ${tns[*]}; do
        rm -rf $rocksdb_data
        echo "Running $db_name with $tn threads for $file_name" 
        ./build/ycsb -db $db_name -threads $tn -P $file_name 2>> $output_file &
        wait
      done
    done
done

