//
//  rocksdb_db.cc
//  YCSB-C
//

#include "faster_db.h"
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <cassert>

thread_local Guid session;
using namespace std;
namespace ycsbc {
FasterDB::FasterDB(const char *dbPath): db_(new FasterKv<Key, Value, disk_t>(16*_MB, 10*_GB, dbPath)) {
  
}
void FasterDB::Init() {
  session = db_->StartSession();
  live_sessions_++;
}
void FasterDB::Close() {
  db_->Refresh();
  db_->StopSession();
  live_sessions_--;
}

int FasterDB::Insert(const std::string &table, const std::string &key,
                    std::vector<KVPair> &values) {
  UpsertContext context{Key(key.c_str()), Value(values[0].second.c_str())};
  auto callback = [](IAsyncContext* ctxt, Status result) {
    assert(result == Status::Ok);
  };
  Status result = db_->Upsert(context, callback, 1);
  return (result == Status::Ok)? 0 : 1  ;
}

int FasterDB::Read(const std::string &table, const std::string &key,
                  const std::vector<std::string> *fields,
                  std::vector<KVPair> &result) {
  ReadContext context{Key(key.c_str())};
  auto callback = [](IAsyncContext* ctxt, Status result) {
    assert(result == Status::Ok);
  };
  Status res = db_->Read(context, callback, 1);
  return 0;
}

int FasterDB::Update(const std::string &table, const std::string &key,
                    std::vector<KVPair> &values) {
  return Insert(table, key, values);
}

int FasterDB::Delete(const std::string &table, const std::string &key) {
  return 0;
}

int FasterDB::Scan(const std::string &table, const std::string &key, int len,
                  const std::vector<std::string> *fields,
                  std::vector<std::vector<KVPair>> &result) {
  return 0;
}

FasterDB::~FasterDB() { 
}

}