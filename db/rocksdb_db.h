//
//  rocksdb_db.h
//  YCSB-C
//

#ifndef YCSB_C_ROCKSDB_DB_H_
#define YCSB_C_ROCKSDB_DB_H_

#include "core/db.h"
#include "core/properties.h"
#include "rocksdb/db.h"
#include "rocksdb/env.h"
#include <iostream>
#include <string>

namespace ycsbc {

class RocksDB : public DB {
public:
  RocksDB(const char *dbPath);

  int Read(const std::string &table, const std::string &key,
           const std::vector<std::string> *fields, std::vector<KVPair> &result);

  int Scan(const std::string &table, const std::string &key, int len,
           const std::vector<std::string> *fields,
           std::vector<std::vector<KVPair>> &result);

  int Update(const std::string &table, const std::string &key,
             std::vector<KVPair> &values);

  int Insert(const std::string &table, const std::string &key,
             std::vector<KVPair> &values);

  int Delete(const std::string &table, const std::string &key);


  ~RocksDB();

private:
  rocksdb::DB *db_;
  rocksdb::Iterator *it{nullptr};
};

} // namespace ycsbc

#endif