//
// Created by iperc on 17/07/2024.
//

#include "DatasetPost.h"

#include <fstream>
#include <utility>

#include "DatasetUser.h"

static int n_posts = 0;

DatasetPost::DatasetPost(DatasetUser* original_poster, Article article, bool isFake,int*NumbersOfPosts):original_poster(original_poster),isFake(isFake),NumberOfPosts(NumberOfPosts)
{
    this->id = ++n_posts;

    this->original_poster  = original_poster;
    this->article = article;
    this->isFake = isFake;



}

void DatasetPost::addResharer(DatasetUser* parent, DatasetUser* child) {
    reshare_tree[parent].push_back(child);
    reshare_tree[child] = std::vector<DatasetUser*>();
}

bool DatasetPost::hasReshared(DatasetUser* u)
{
    return reshare_tree.find(u) != reshare_tree.end();
}


void DatasetPost::printReshareTree(DatasetUser* user, int depth) const {
    if (reshare_tree.find(user) == reshare_tree.end()) return;
    for (DatasetUser* child : reshare_tree.at(user)) {
        for (int i = 0; i < depth; ++i) std::cout << "  ";
        std::cout << "User " << child->id << "\n";
        printReshareTree(child, depth + 1);
    }
}

std::tuple<int, int, int> DatasetPost::findMaxDepthBreadthAndSize() const {
    return recursToFindStats(original_poster);
}

std::tuple<int, int, int> DatasetPost::recursToFindStats(DatasetUser* u) const {
    if (!u) return {0, 0, 0}; // Handle the case where the user is null

    int maxDepth = 1;
    int maxBreadth = 0;
    int size = 1;

    if (reshare_tree.find(u) != reshare_tree.end()) {
        std::vector<DatasetUser*> currentList = reshare_tree.at(u);
        maxBreadth = currentList.size();
        for (DatasetUser* child : currentList) {
            auto [childMaxDepth, childMaxBreadth, childSize] = recursToFindStats(child);
            size += childSize;
            maxDepth = std::max(maxDepth, childMaxDepth + 1); // +1 to include the current level
            maxBreadth = std::max(maxBreadth, childMaxBreadth);
        }
    }
    return {maxDepth, maxBreadth, size};
}



// Function to print counts to CSV file
void printCountsToCSV(const std::unordered_map<int, int>& matches, const std::string& filename, int n_trials) {
    std::ofstream file(filename);
    file << "matches,count\n";

    // Find the maximum match count
    int max_match_count = 0;
    for (const auto& pair : matches) {
        if (pair.first > max_match_count) {
            max_match_count = pair.first;
        }
    }

    // Print the counts in CSV format
    for (int i = 0; i <= max_match_count; ++i) {
        int count = matches.count(i) ? matches.at(i) / n_trials : 0;
        file << i << ',' << count << '\n';
    }

    file.close();
}


