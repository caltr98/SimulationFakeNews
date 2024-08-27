//
// Created by iperc on 17/07/2024.
//

#ifndef DATASETUSER_H
#define DATASETUSER_H


#include "DatasetPost.h"
#include "../User.h"
#include "datasetstats.h"

class DatasetUser {

public:
    virtual ~DatasetUser() = default;
    int* NumberOfPosts;

    DatasetUser(int id, std::string category, double prob_susceptible, double prob_recover, double prob_spreader,
                State initial_state, std::string name, std::string surname, std::string gender, std::string bio,
                std::vector<Article> true_news, std::vector<Article> false_news, int*NumberOfPosts);
    void addToFollowerFeeds(DatasetPost* post);
    bool spreadPost();
    virtual bool resharePost();

    [[nodiscard]] std::vector<DatasetPost*> allSharedPosts() const;

    virtual std::tuple<int,int> allSharedPostsStats();
    virtual std::tuple<int,int> allReSharedPostStats();
    std::tuple<int, int> allSharedPostStats();


    virtual bool becomeInfectious();
    virtual bool becomeSusceptible();

    bool becomeRecovered() const;
    void follow(User *other);
    State state;
    std::string bio;

    DatasetPost* getRandomPost(bool isFake);
    void Act();
    int id;

    std::vector<State> perEpochState;

    void follow(DatasetUser *other);

    void addToFollowerFeeds(Post* post);

    static bool randomEvent(double probability);
    [[nodiscard]] std::vector<State> getPerEpochState() const;
    void changeState();
    void performAction();

    std::string category;
    double prob_susceptible;
    double prob_recover;
    double prob_spreader;


    std::string name;
    std::string surname;
    std::string gender;
    std::vector<std::string> skills;
    std::vector<Article> true_news;
    std::vector<Article> false_news;
    std::vector<std::tuple<DatasetUser*, DatasetPost*>> feed;
    std::vector<DatasetUser*> followers;
    std::vector<DatasetPost*> sharedPosts;

    std::vector<DatasetPost*> re_sharedPosts;
    State initial_state;

};



#endif //DATASETUSER_H
