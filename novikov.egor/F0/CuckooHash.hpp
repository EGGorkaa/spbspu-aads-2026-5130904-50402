#ifndef CUCKOOHASH_HPP
#define CUCKOOHASH_HPP

#include <boost/container_hash/hash.hpp>
#include <functional>
#include <utility>

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
}

#endif