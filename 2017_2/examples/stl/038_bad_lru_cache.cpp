#ifndef LIBUTIL_LRU_CACHE_HPP__
#define LIBUTIL_LRU_CACHE_HPP__

#include <unordered_map>

namespace libutil {

template <class Tkey, class TItem> class LastUsedCache {
  struct record_t {
    TItem data;
    uint64_t gen;
  };

  typedef std::unordered_map<Tkey, record_t> cache_t;
  cache_t cache_;
  size_t max_size_;
  uint64_t generation_;

public:
  LastUsedCache(size_t nelems) : max_size_(nelems), generation_(0) {}

  bool find(Tkey key, TItem *item = nullptr) const {
    typename cache_t::const_iterator it = cache_.find(key);

    if (it == cache_.end())
      return false;
    else {
      if (item)
        *item = it->second.data;
      return true;
    }
  }

  void insert(Tkey key, const TItem &data) {
    if (max_size_ != 0U) {
      record_t rec{data, ++generation_};

      std::pair<typename cache_t::iterator, bool> result =
          cache_.insert(typename cache_t::value_type(key, rec));

      if (result.second) {
        if (cache_.size() > max_size_) {
          uint64_t min_gen = -1;
          for (typename cache_t::value_type &val : cache_)
            min_gen = std::min(min_gen, val.second.gen);

          for (typename cache_t::iterator val_it = cache_.begin();
               val_it != cache_.end(); ++val_it)
            if (val_it->second.gen == min_gen) {
              cache_.erase(val_it);
              break;
            }
        }
      } else {
        result.first->second.gen = ++generation_;
      }
    }
  }

  size_t size() const { return cache_.size(); }

  bool empty() const { return (size() == 0); }

  void clear() { cache_.clear(); }
};

} // namespace libutil
