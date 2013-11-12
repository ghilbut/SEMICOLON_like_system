#ifndef LIKE_COUNT_H_
#define LIKE_COUNT_H_

#include <json/json.h>
#include <set>
#include <string>


class LikeCount {
public:
    LikeCount(void);
    ~LikeCount(void);

    bool IsLike(const std::string& user) const;
    size_t Like(const std::string& user, bool like);
    size_t Size(void) const;


private:
    std::set<std::string> likes_;
};

#endif  // LIKE_COUNT_H_