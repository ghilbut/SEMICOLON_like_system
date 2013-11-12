#include "like_count.h"
#include <boost/assert.hpp>


LikeCount::LikeCount(void) {
    // nothing
}

LikeCount::~LikeCount(void) {
    // nothing
}

bool LikeCount::IsLike(const std::string& user) const {
    return (likes_.find(user) != likes_.end());
}

size_t LikeCount::Like(const std::string& user, bool like) {
    std::set<std::string>::iterator itr = likes_.find(user);
    std::set<std::string>::iterator end = likes_.end();
    if (like) {
        if (itr == end) {
            likes_.insert(user);
        }
    } else {
        if (itr != end) {
            likes_.erase(itr);
        }
    }
    return likes_.size();
}

size_t LikeCount::Size(void) const {
    return likes_.size();
}