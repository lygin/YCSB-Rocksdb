# YCSB-C

Fork from YCSB-C
- separate load and run throughput
- add latency testing(use leveldb/util histogram)

## Quick Start

To build YCSB-C on Ubuntu, for example:

```
$ sudo apt-get install libtbb-dev librocksdb-dev
$ make
$ run.sh
```

Run Workload A with a Rocksdb-based
implementation of the database, for example:
```
./ycsbc -db rocksdb -threads 8 -P workloads/workloada.spec
```

Output like this:
```
rocksdb	workloads/workloada.spec	8
# Loading records:	100000
# Load throughput (KOPS): 486.449
# Run operations:	100000
# Run throughput (KOPS): 855.341
```
Also reference run.sh and run\_redis.sh for the command line. See help by
invoking `./ycsbc` without any arguments.

Note that we do not have load and run commands as the original YCSB. Specify
how many records to load by the recordcount property. Reference properties
files in the workloads dir.

