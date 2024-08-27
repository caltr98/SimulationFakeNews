#include "post.h"
#include "User.h" // Include User.h for User class definition

static int n_posts = 0;

Post::Post(User* original_poster,bool isFake, int* NumberOfPosts)
    : original_poster(original_poster),isFake(isFake) {
    *NumberOfPosts = *NumberOfPosts+1 ;
    this->NumbersOfPosts =NumberOfPosts;
    id = ++n_posts;
    reshare_tree[original_poster] = std::vector<User*>();
}

void Post::addResharer(User* parent, User* child) {
    reshare_tree[parent].push_back(child);
    reshare_tree[child] = std::vector<User*>();
}

bool Post::hasReshared(User* u)
{
    return reshare_tree.find(u) != reshare_tree.end();
}
void Post::printReshareTree(User* user, int depth) const {
    if (reshare_tree.find(user) == reshare_tree.end()) return;
    for (User* child : reshare_tree.at(user)) {
        for (int i = 0; i < depth; ++i) std::cout << "  ";
        std::cout << "User " << child->id << "\n";
        printReshareTree(child, depth + 1);
    }
}

std::tuple<int, int, int> Post::findMaxDepthBreadthAndSize() const {
    return recursToFindStats(original_poster);
}

std::tuple<int, int, int> Post::recursToFindStats(User* u) const {
    if (!u) return {0, 0, 0}; // Handle the case where the user is null

    int maxDepth = 1;
    int maxBreadth = 0;
    int size = 1;

    if (reshare_tree.find(u) != reshare_tree.end()) {
        std::vector<User*> currentList = reshare_tree.at(u);
        maxBreadth = currentList.size();
        for (User* child : currentList) {
            auto [childMaxDepth, childMaxBreadth, childSize] = recursToFindStats(child);
            size += childSize;
            maxDepth = std::max(maxDepth, childMaxDepth + 1); // +1 to include the current level
            maxBreadth = std::max(maxBreadth, childMaxBreadth);
        }
    }
    return {maxDepth, maxBreadth, size};
}
