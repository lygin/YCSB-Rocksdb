//
//  basic_db.cc
//  YCSB-C
//
//  Created by Jinglei Ren on 12/17/14.
//  Copyright (c) 2014 Jinglei Ren <jinglei@ren.systems>.
//

#include "db/db_factory.h"

#include <string>
#include "db/basic_db.h"
#include "db/lock_stl_db.h"
#include "db/tbb_rand_db.h"
#include "db/tbb_scan_db.h"
#include "db/rocksdb_db.h"
#include "db/faster_db.h"
#include <filesystem>
const std::string rocksdb_path{"/mnt/nvme1/rocksdb_data"};
const std::string faster_path{"/mnt/nvme1/faster_data"};

using namespace std;
using ycsbc::DB;
using ycsbc::DBFactory;

DB* DBFactory::CreateDB(utils::Properties &props) {
  if (props["dbname"] == "basic") {
    return new BasicDB;
  } else if (props["dbname"] == "lock_stl") {
    return new LockStlDB;
  } else if (props["dbname"] == "tbb_rand") {
    return new TbbRandDB;
  } else if (props["dbname"] == "tbb_scan") {
    return new TbbScanDB;
  } else if (props["dbname"] == "rocksdb") {
    std::filesystem::create_directories(rocksdb_path);
    return new RocksDB(rocksdb_path.c_str());
  } else if (props["dbname"] == "faster"){
    std::filesystem::create_directories(faster_path);
    return new FasterDB(faster_path.c_str());
  } else return NULL;
}

