#ifndef LIBUTIL_LRU_CACHE_HPP__
#define LIBUTIL_LRU_CACHE_HPP__

#include <boost/unordered_map.hpp>
#include <list>

namespace libutil {

template <class Tkey, class TItem> class lru_cache {
  typedef std::list<Tkey> lru_t;
  lru_t lru_;

  struct lru_record_t {
    TItem data;
    typename lru_t::iterator lru_it;
    lru_record_t(const TItem &dat, const typename lru_t::iterator &it)
        : data(dat), lru_it(it) {}
  };

  typedef boost::unordered_map<Tkey, lru_record_t> cache_t;
  cache_t cache_;
  size_t max_size_;

public:
  lru_cache(size_t nelems) : max_size_(nelems) {}

  bool find(Tkey key, TItem *item = NULL) const {
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
      lru_.push_front(key);
      lru_record_t rec(data, lru_.begin());

      std::pair<typename cache_t::iterator, bool> result =
          cache_.insert(typename cache_t::value_type(key, rec));

      if (result.second) {
        if (cache_.size() > max_size_) {
          Tkey older_key = lru_.back();
          lru_.pop_back();
          cache_.erase(older_key);
        }
      } else {
        typename cache_t::iterator &oldit = result.first;
        lru_.erase(oldit->second.lru_it);
        oldit->second = rec;
      }
    }
  }

  size_t size() const { return cache_.size(); }

  bool empty() const { return (size() == 0); }

  void clear() {
    cache_.clear();
    lru_.clear();
  }
};

} // namespace libutil


#endif // !LIBUTIL_LRU_CACHE_HPP__
