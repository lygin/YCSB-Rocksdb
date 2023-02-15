#/bin/bash

repeat_num=1
db_names=(
  "rocksdb"
)

trap 'kill $(jobs -p)' SIGINT


workload_dir=workloads

tn=8
workload=$workload_dir/workload*.spec
output_file=output.result
rocksdb_data=rocksdb_data

rm $output_file

for file_name in ${workload[*]}; do
    rm -rf $rocksdb_data
    for db_name in ${db_names[@]}; do
      for ((i=1; i<=repeat_num; ++i)); do
        echo "Running $db_name with $tn threads for $file_name" 
        ./ycsbc -db $db_name -threads $tn -P $file_name 2>> $output_file &
        wait
      done
    done
done

