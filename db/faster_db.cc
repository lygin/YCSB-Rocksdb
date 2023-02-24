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
FasterDB::FasterDB(const char *dbPath): db_(new FasterKv<Key, Value, disk_t>(1024,10*_GB, dbPath, 0.2)) {
  
}
void FasterDB::Init() {
  session = db_->StartSession();
  live_sessions_++;
}
void FasterDB::Close() {
  auto result = db_->CompletePending(true);
  assert(result);
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
  db_->Refresh();
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
  return (res == Status::Ok || res == Status::NotFound)? 0 : 1  ;
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
  printf("HashtableMemoryBytes: %f MB\n", db_->HashtableMemoryBytes()*1.0/1024/1024);
}

}