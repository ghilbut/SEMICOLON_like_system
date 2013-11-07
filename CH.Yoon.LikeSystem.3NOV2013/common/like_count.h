#ifndef LIKE_COUNT_H_
#define LIKE_COUNT_H_

#include <json/json.h>
#include <set>
#include <string>


class LikeCount {
public:
    explicit LikeCount(Json::Value& list);
    ~LikeCount(void);

    bool IsLike(const std::string& user) const;
    unsigned int Like(const std::string& user, bool like);
    unsigned int Size(void) const;


private:
    std::set<std::string> likes_;
    Json::Value& list_;
};

#endif  // LIKE_COUNT_H_