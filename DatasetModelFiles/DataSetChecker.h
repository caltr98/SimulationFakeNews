//
// Created by iperc on 21/07/2024.
//

#ifndef DATASETCHECKER_H
#define DATASETCHECKER_H
#include "DatasetUser.h"

class DataSetChecker : public DatasetUser
{
public:
    DataSetChecker(int id, std::string name, std::string surname, std::string gender, std::string bio,
                   std::string engines,
                   std::vector<Article> true_news, std::vector<Article> false_news, int min_matches,int*NumberOfPosts);
    bool becomeInfectious() override;
    bool becomeSusceptible() override;
    bool spreadPost();
    DatasetPost* getRandomPost();
    bool resharePost() override;
    std::string engines;
    int min_matches;
};


#endif //DATASETCHECKER_H
