#ifndef SIMULATION_POST_H
#define SIMULATION_POST_H

#include <vector>
#include <unordered_map>
#include <tuple>
#include <string>
#include <iostream>

class User; // Forward declaration

class Post {
public:
    int id;
    bool isFake;
    User* original_poster;
    std::unordered_map<User*, std::vector<User*>> reshare_tree;
    int* NumbersOfPosts;

    explicit Post(User* original_poster,bool isFake,int* NumberOfPosts);
    Post();

    void addResharer(User* parent, User* child);
    bool hasReshared(User* u);
    void printReshareTree(User* user, int depth = 0) const;
    std::tuple<int, int, int> findMaxDepthBreadthAndSize() const;

private:
    std::tuple<int, int, int> recursToFindStats(User* u) const;
};

#endif // SIMULATION_POST_H
