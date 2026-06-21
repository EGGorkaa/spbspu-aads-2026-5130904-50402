#ifndef CUCKOOHASH_HPP
#define CUCKOOHASH_HPP

#include <boost/container_hash/hash.hpp>
#include <functional>
#include <utility>
#include <algorithm>

namespace novikov
{
  template < class Key > struct Hash1
  {
    size_t operator()(const Key &key) const
    {
      return std::hash< Key >{}(key);
    }
  };

  template < class Key > struct Hash2
  {
    size_t operator()(const Key &key) const
    {
      return boost::hash< Key >{}(key);
    }
  };

  template < class Key, class Value, class Hash1 = Hash1< Key >, class Hash2 = Hash2< Key >,
             class Equal = std::equal_to< Key > >
  class CuckooHash
  {
  private:
    struct Item
    {
      std::pair< Key, Value > data;
      bool occupied;

      Item():
        data(),
        occupied(false)
      {}
    };

    Item *table1_;
    Item *table2_;
    size_t capacity_;
    size_t size_;

    Hash1 hash1_;
    Hash2 hash2_;
    Equal equal_;

    size_t hashPos1(const Key &key) const;
    size_t hashPos2(const Key &key) const;

    void swap(CuckooHash &other) noexcept;

    void insertWithoutCheck(const Key &key, const Value &value);

  public:
    CuckooHash(size_t capacity = 17);

    CuckooHash(const CuckooHash &other);
    CuckooHash(CuckooHash &&other) noexcept;

    ~CuckooHash();

    CuckooHash &operator=(const CuckooHash &other);
    CuckooHash &operator=(CuckooHash &&other) noexcept;

    void insert(const Key &key, const Value &value);
    Value get(const Key &key) const;
    bool has(const Key &key) const;
    bool remove(const Key &key);

    void clear();
    void rehash(size_t newCapacity);

    size_t size() const;
    bool empty() const;
  };

  template < class Key, class Value, class Hash1, class Hash2, class Equal >
  size_t CuckooHash< Key, Value, Hash1, Hash2, Equal >::hashPos1(const Key &key) const
  {
    return hash1_(key) % capacity_;
  }

  template < class Key, class Value, class Hash1, class Hash2, class Equal >
  size_t CuckooHash< Key, Value, Hash1, Hash2, Equal >::hashPos2(const Key &key) const
  {
    return hash2_(key) % capacity_;
  }

  template < class Key, class Value, class Hash1, class Hash2, class Equal >
  void CuckooHash< Key, Value, Hash1, Hash2, Equal >::swap(CuckooHash &other) noexcept
  {
    std::swap(table1_, other.table1_);
    std::swap(table2_, other.table2_);
    std::swap(capacity_, other.capacity_);
    std::swap(size_, other.size_);
    std::swap(hash1_, other.hash1_);
    std::swap(hash2_, other.hash2_);
    std::swap(equal_, other.equal_);
  }

  template < class Key, class Value, class Hash1, class Hash2, class Equal >
  void CuckooHash< Key, Value, Hash1, Hash2, Equal >::insertWithoutCheck(const Key &key, const Value &value)
  {
    std::pair< Key, Value > current(key, value);
    bool firstTable = true;
    size_t kickCount = 0;

    while (kickCount < capacity_) {
      if (firstTable) {
        size_t pos = hashPos1(current.first);
        if (!table1_[pos].occupied) {
          table1_[pos].data = current;
          table1_[pos].occupied = true;
          ++size_;
          return;
        }
        std::swap(current, table1_[pos].data);
        firstTable = false;
      } else {
        size_t pos = hashPos2(current.first);
        if (!table2_[pos].occupied) {
          table2_[pos].data = current;
          table2_[pos].occupied = true;
          ++size_;
          return;
        }
        std::swap(current, table2_[pos].data);
        firstTable = true;
      }
      ++kickCount;
    }

    rehash(capacity_ * 2 + 1);
    insertWithoutCheck(current.first, current.second);
  }

}

#endif