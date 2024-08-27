#ifndef USER_H
#define USER_H

#include "Post.h"
#include <string>
#include <vector>
#include <tuple>
#include <random>

enum State { Susceptible, Infectious, Recovered, CheckerFail, CheckerSuccess};

class User {
public:
    User(int id, std::string  category, double prob_susceptible, double prob_recover, double prob_spreader, State initial_state,int* NumberOfPosts);
    int id;
    int *NumberOfPosts;
    virtual bool becomeInfectious();
    virtual bool becomeSusceptible();
    bool becomeRecovered() const;
    virtual bool spreadPost();
    virtual bool resharePost();
    void follow(User *other);
    State state;

    std::vector<State> perEpochState;

    virtual std::tuple<int,int> allSharedPostsStats();
    std::tuple<int,int> allReSharedPostStats();

    std::vector<Post*> allSharedPosts();
    void Act();

    void addToFollowerFeeds(Post* post);

protected:
    static bool randomEvent(double probability);
    std::vector<State> getPerEpochState() const;
    void changeState();
    void performAction();

    std::string category;
    double prob_susceptible;
    double prob_recover;
    double prob_spreader;
    std::vector<User*> followers;
    std::vector<std::tuple<User*, Post*>> feed;
    std::vector<Post*> sharedPosts;
    std::vector<Post*> re_sharedPosts;
};

#endif // USER_H
