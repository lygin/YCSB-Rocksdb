//
//  faster_db.h
//  YCSB-C
//

#ifndef YCSB_C_FASTER_DB_H_
#define YCSB_C_FASTER_DB_H_

#include "core/db.h"
#include "core/properties.h"
#include "core/faster.h"
#include "device/file_system_disk.h"
#include <iostream>
#include <string>
#include <atomic>
using namespace FASTER::core;
typedef FASTER::environment::QueueIoHandler handler_t;
/// Disk's log uses 64 MB segments.
typedef FASTER::device::FileSystemDisk<handler_t, 67108864L> disk_t;
const uint64_t _MB = 1024 * 1024;
const uint64_t _GB = 1024 * 1024 * 1024;
class UpsertContext;
class ReadContext;
class Key;
class Value;

static thread_local std::mutex locker;

namespace ycsbc {

class FasterDB : public DB {
public:
  FasterDB(const char *dbPath);

  void Init();
  void Close();

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


  ~FasterDB();

private:
  FasterKv<Key, Value, disk_t> *db_;
  std::atomic<int> live_sessions_;
};

} // namespace ycsbc

class Key {
 public:
  Key(const char *key){
    memcpy(key_, key, 8);
    key_[8] = '\0';
  }
  Key(const Key& other) {
    memcpy(key_, other.key_, 9);
  }

  inline static constexpr uint32_t size() {
    return static_cast<uint32_t>(sizeof(Key));
  }

  inline KeyHash GetHash() const {
    std::hash<std::string> hash_fn;
    return KeyHash{ hash_fn(key_) };
  }

  inline bool operator==(const Key& other) const {
    return strncmp(key_, other.key_, 9) == 0;
  }
  inline bool operator!=(const Key& other) const {
    return strncmp(key_, other.key_, 9) != 0;
  }

  char key_[9];
};


class Value {
  public:
  Value(const char *value) {
    memcpy(value_, value, 8);
    value_[8] = '\0';
  }
  Value(const Value& other) {
    memcpy(value_, other.value_, 9);
  }

  inline static constexpr uint32_t size() {
    return static_cast<uint32_t>(sizeof(Value));
  }

  friend class UpsertContext;
  friend class ReadContext;

  char value_[9];
};

class UpsertContext : public IAsyncContext {
  public:
  typedef Key key_t;
  typedef Value value_t;
  UpsertContext(const Key& key, const Value& val)
    : key_{ key }
    , val_{ val } {
  }

  /// Copy (and deep-copy) constructor.
  UpsertContext(const UpsertContext& other)
    : key_{ other.key_ }
    , val_{ other.val_ } {
  }

  /// The implicit and explicit interfaces require a key() accessor.
  inline const Key& key() const {
    return key_;
  }
  inline static constexpr uint32_t value_size() {
    return sizeof(Value);
  }
  inline static constexpr uint32_t value_size(const Value& old_value) {
    return sizeof(Value);
  }
  /// Non-atomic and atomic Put() methods.
  inline void Put(Value& value) {
    value = val_;
    //printf("Put %s\n", value.value_);
  }
  inline bool PutAtomic(Value& value) {
    // Get the lock on the value.
    locker.lock();
    value = val_;
    //printf("PutATOMIC %s\n", value.value_);
    locker.unlock();
    return true;
  }

  protected:
  /// The explicit interface requires a DeepCopy_Internal() implementation.
  Status DeepCopy_Internal(IAsyncContext*& context_copy) {
    return IAsyncContext::DeepCopy_Internal(*this, context_copy);
  }

  private:
  Key key_;
  Value val_;
};

class ReadContext : public IAsyncContext {
  public:
  typedef Key key_t;
  typedef Value value_t;
  ReadContext(Key key): key_{ key }{}

  /// Copy (and deep-copy) constructor.
  ReadContext(const ReadContext& other): key_{ other.key_ }{}

  /// The implicit and explicit interfaces require a key() accessor.
  inline const Key& key() const {
    return key_;
  }

  inline void Get(const Value& value) {
    //printf("Get %s\n", value.value_);
  }
  inline void GetAtomic(const Value& value) {
    // Get the lock on the value.
    // locker.lock();
    //printf("GetATOMIC %s\n", value.value_);
    // locker.unlock();
  }

  protected:
  Status DeepCopy_Internal(IAsyncContext*& context_copy) {
    return IAsyncContext::DeepCopy_Internal(*this, context_copy);
  }

  private:
  Key key_;
};


#endif