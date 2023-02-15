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
# Load throughput (KOPS): 511.329
Count: 100000  Average: 15.1654  StdDev: 6.33
Percentiles(us): P50: 13.73 P90: 19.80 P99: 35.32 P99.9: 57.75 P99.99: 281.25
------------------------------------------------------
[       5,       6 )      26   0.026%   0.026% 
[       6,       7 )      97   0.097%   0.123% 
[       7,       8 )       2   0.002%   0.125% 
[       8,       9 )       4   0.004%   0.129% 
[       9,      10 )     322   0.322%   0.451% 
[      10,      12 )   20824  20.824%  21.275% ####
[      12,      14 )   33181  33.181%  54.456% #######
[      14,      16 )   14292  14.292%  68.748% ###
[      16,      18 )   14738  14.738%  83.486% ###
[      18,      20 )    7247   7.247%  90.733% #
[      20,      25 )    5481   5.481%  96.214% #
[      25,      30 )    2064   2.064%  98.278% 
[      30,      35 )     679   0.679%  98.957% 
[      35,      40 )     680   0.680%  99.637% 
[      40,      45 )     106   0.106%  99.743% 
[      45,      50 )      57   0.057%  99.800% 
[      50,      60 )     129   0.129%  99.929% 
[      60,      70 )      22   0.022%  99.951% 
[      70,      80 )       8   0.008%  99.959% 
[      80,      90 )       1   0.001%  99.960% 
[      90,     100 )      17   0.017%  99.977% 
[     100,     120 )       6   0.006%  99.983% 
[     120,     140 )       1   0.001%  99.984% 
[     200,     250 )       1   0.001%  99.985% 
[     250,     300 )       8   0.008%  99.993% 
[     300,     350 )       2   0.002%  99.995% 
[     350,     400 )       2   0.002%  99.997% 
[     400,     450 )       1   0.001%  99.998% 
[     450,     500 )       2   0.002% 100.000% 

# Run operations:	100000
# Run throughput (KOPS): 784.025
Count: 12500  Average: 9.9924  StdDev: 7.39
Percentiles(us): P50: 9.40 P90: 18.83 P99: 33.75 P99.9: 57.00 P99.99: 75.83
------------------------------------------------------
[       1,       2 )     269   2.152%   2.152% 
[       2,       3 )    2364  18.912%  21.064% ####
[       3,       4 )    1415  11.320%  32.384% ##
[       4,       5 )     607   4.856%  37.240% #
[       5,       6 )     364   2.912%  40.152% #
[       6,       7 )     434   3.472%  43.624% #
[       7,       8 )     269   2.152%  45.776% 
[       8,       9 )     371   2.968%  48.744% #
[       9,      10 )     390   3.120%  51.864% #
[      10,      12 )     988   7.904%  59.768% ##
[      12,      14 )    1637  13.096%  72.864% ###
[      14,      16 )    1177   9.416%  82.280% ##
[      16,      18 )     737   5.896%  88.176% #
[      18,      20 )     552   4.416%  92.592% #
[      20,      25 )     562   4.496%  97.088% #
[      25,      30 )     176   1.408%  98.496% 
[      30,      35 )      84   0.672%  99.168% 
[      35,      40 )      37   0.296%  99.464% 
[      40,      45 )      29   0.232%  99.696% 
[      45,      50 )      15   0.120%  99.816% 
[      50,      60 )      15   0.120%  99.936% 
[      60,      70 )       5   0.040%  99.976% 
[      70,      80 )       3   0.024% 100.000% 
```
Also reference run.sh and run\_redis.sh for the command line. See help by
invoking `./ycsbc` without any arguments.

Note that we do not have load and run commands as the original YCSB. Specify
how many records to load by the recordcount property. Reference properties
files in the workloads dir.

