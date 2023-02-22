# YCSB-Rocksdb

Fork from YCSB-C
- separate load and run throughput
- add latency testing(use leveldb/util histogram)

## Quick Start

To build YCSB-C on Ubuntu, for example:

```
$ sudo apt-get install libtbb-dev librocksdb-dev
$ mkdir build
$ cd build
$ cmake ..
$ make
```

Run Workload A with a Rocksdb-based
implementation of the database, for example:
```
./ycsb -db faster -threads 8 -P ../workloads/workloada.spec
```
