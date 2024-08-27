//
// Created by iperc on 17/07/2024.
//

#ifndef DATASETPOST_H
#define DATASETPOST_H

#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../Post.h"

#include "datasetstats.h"


// Forward declare DatasetUser to avoid circular dependency
class DatasetUser;

void printCountsToCSV(const std::unordered_map<int, int>& trueNewsMatches, const std::unordered_map<int, int>& fakeNewsMatches) ;


class DatasetPost {
public:
    int* NumberOfPosts;

    DatasetPost(DatasetUser* original_poster, Article article, bool isFake, int* NumberOfPosts);
    void addResharer(DatasetUser* parent, DatasetUser* child);
    bool hasReshared(DatasetUser* u);
    void printReshareTree(DatasetUser* user, int depth = 0) const;
    std::tuple<int, int, int> findMaxDepthBreadthAndSize() const;
    std::tuple<int, int, int> recursToFindStats(DatasetUser* u) const;
    void addAnalysis(int matches, int id_user, bool isFake, const std::string& post_or_reshare, const std::string& engine);

public:
    std::string title;
    Article article;  // Changed from Text to text to avoid name conflict
    bool isFake;
    std::unordered_map<DatasetUser*, std::vector<DatasetUser*>> reshare_tree;
    DatasetUser* original_poster;
    int id;
};


#endif // DATASETPOST_H
