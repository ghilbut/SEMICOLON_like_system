#include "like_count.h"
#include <boost/assert.hpp>


LikeCount::LikeCount(Json::Value& list)
    : list_(list) {
    BOOST_ASSERT_MSG(list.isArray(), "json list must be array.");

    const unsigned int size = list_.size();
    for (unsigned int index = 0; index < size; ++index) {
        likes_.insert(list_[index].asString());
    }
}

LikeCount::~LikeCount(void) {
    // nothing
}

bool LikeCount::IsLike(const std::string& user) const {
    return (likes_.find(user) != likes_.end());
}

unsigned int LikeCount::Like(const std::string& user, bool like) {
    std::set<std::string>::iterator itr = likes_.find(user);
    std::set<std::string>::iterator end = likes_.end();
    if (like) {
        if (itr == end) {
            likes_.insert(user);
            list_.append(user);
        }
    } else {
        if (itr != end) {
            likes_.erase(itr);
            list_.clear();

            itr = likes_.begin();
            end = likes_.end();
            for (; itr != end; ++itr) {
                list_.append(*itr);
            }
        }
    }
    return list_.size();
}

unsigned int LikeCount::Size(void) const {
    return list_.size();
}