//
//  rocksdb_db.cc
//  YCSB-C
//

#include "rocksdb_db.h"
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>


using namespace std;

namespace ycsbc {
RocksDB::RocksDB(const char *dbPath) {
  rocksdb::Options options;
  options.create_if_missing = true;
  options.OptimizeLevelStyleCompaction();
  // 8 background threads for compaction
  options.IncreaseParallelism(8);
  rocksdb::Status s = rocksdb::DB::Open(options, dbPath, &db_);
  if (!s.ok()) {
    cerr << "Can't open rocksdb " << dbPath << s.ToString() << endl;
    exit(0);
  }
}

int RocksDB::Insert(const std::string &table, const std::string &key,
                    std::vector<KVPair> &values) {
  rocksdb::Status s;
  auto wo = rocksdb::WriteOptions();
  for (KVPair &p : values) {
    s = db_->Put(wo, key, p.second);
    if (!s.ok()) {
      cerr << "insert error" << s.ToString() << "\n" << endl;
      exit(0);
    }
  }
  return DB::kOK;
}

int RocksDB::Read(const std::string &table, const std::string &key,
                  const std::vector<std::string> *fields,
                  std::vector<KVPair> &result) {
  string value;
  rocksdb::Status s = db_->Get(rocksdb::ReadOptions(), key, &value);
  if (s.ok()) {
    result.emplace_back(key, value);
    return DB::kOK;
  } else if (s.IsNotFound()) {
    return DB::kOK;
  } else {
    cerr << "read error" << endl;
    exit(0);
  }
  return DB::kOK;
}

int RocksDB::Update(const std::string &table, const std::string &key,
                    std::vector<KVPair> &values) {
  return Insert(table, key, values);
}

int RocksDB::Delete(const std::string &table, const std::string &key) {
  rocksdb::Status s;
  auto wo = rocksdb::WriteOptions();
  s = db_->Delete(wo, key);
  if(s.ok()) {
    return DB::kOK;
  } else if(s.IsNotFound()) {
    return DB::kOK;
  }else {
    cerr << "Delete error" << endl;
    exit(0);
  }
}

int RocksDB::Scan(const std::string &table, const std::string &key, int len,
                  const std::vector<std::string> *fields,
                  std::vector<std::vector<KVPair>> &result) {
  auto it = db_->NewIterator(rocksdb::ReadOptions());
  it->Seek(key);
  std::string val;
  std::string k;
  int i;
  int cnt = 0;
  for (i = 0; i < len && it->Valid(); i++) {
    k = it->key().ToString();
    val = it->value().ToString();
    it->Next();
    if (val.empty())
      cnt++;
  }
  if (cnt > 0) {
    std::cout << cnt << "empty values" << std::endl;
  }
  delete it;
  return DB::kOK;
}

RocksDB::~RocksDB() { 
  delete db_; 
}


}