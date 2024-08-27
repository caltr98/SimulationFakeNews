#include "Checker.h"

Checker::Checker(int id,int*NumberOfPosts)
    : User(id, "Checker", 0.0, 1.0, 0.0, Recovered,(NumberOfPosts)) {}

bool Checker::becomeInfectious() {
    return false; // Checker never becomes infectious
}

bool Checker::becomeSusceptible() {
    return false; // Checker never becomes susceptible
}

//define resharePost
bool Checker::resharePost() {
    if (this->feed.empty()) {
        return false; // nothing to reshare
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);

    for (auto it = feed.rbegin(); it != feed.rend(); ++it) {
        auto& [sharer, post] = *it;
        if (dis(gen) == 1) {
            if (post->isFake) {
                continue; // Checker never shares fake news
            }
            post->addResharer(sharer, this);
            this->re_sharedPosts.push_back(post);
            addToFollowerFeeds(post);
            return true;
        }
    }

    return false; // no valid post selected for resharing
}
