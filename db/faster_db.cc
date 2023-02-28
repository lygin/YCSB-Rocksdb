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
static const uint64_t table_size = 16*_MB;
static const uint64_t log_size = 10*_GB;
static const double log_inmem_ratio = 0.2;
namespace ycsbc {
FasterDB::FasterDB(const char *dbPath): 
  db_(new FasterKv<Key, Value, disk_t>(table_size,log_size, dbPath, log_inmem_ratio)) {}

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
  if(res == Status::NotFound) {
    const char *key = context.key().key_;
    printf("%d %s\n", strlen(key), key);
  }
  return (res == Status::Ok)? 0 : 1  ;
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
  char buf[100] = {0};
  sprintf(buf, "Hashtable Memory Used: %f MB\n", db_->HashtableMemoryBytes()*1.0/1024/1024);
  cerr << buf;
}

}