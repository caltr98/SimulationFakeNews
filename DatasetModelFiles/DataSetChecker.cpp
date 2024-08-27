//
// Created by iperc on 21/07/2024.
//
#include "DataSetChecker.h"
#include "HTTPRequester.h"
DataSetChecker::DataSetChecker(int id, std::string name, std::string surname, std::string gender,
                               std::string bio, std::string engines,
                               std::vector<Article> true_news, std::vector<Article> false_news, int min_matches,int*NumberOfPosts)
    : DatasetUser(id, "Checker", 0.0, 0.0, 0.0, CheckerSuccess, std::move(name), std::move(surname),
                  std::move(gender), bio, std::move(true_news), std::move(false_news),NumberOfPosts),
      engines(std::move(engines)),min_matches(min_matches) {}

bool DataSetChecker::becomeInfectious() {
    return false; // Checker never becomes infectious
}

bool DataSetChecker::becomeSusceptible() {
    return false; // Checker never becomes susceptible
}



DatasetPost* DataSetChecker::getRandomPost() {
    // Concatenate the two vectors
    std::vector<Article> combined_news = true_news;
    combined_news.insert(combined_news.end(), false_news.begin(), false_news.end());

    // Check if the combined vector is not empty
    if (!combined_news.empty()) {
        // Seed with a real random value, if available
        std::random_device rd;
        // Use Mersenne Twister engine to generate pseudo-random numbers
        std::mt19937 gen(rd());
        // Shuffle the combined vector
        std::shuffle(combined_news.begin(), combined_news.end(), gen);

        // Generate random index within the range of the vector size
        std::uniform_int_distribution<> dis(0, combined_news.size() - 1);
        int randomIndex = dis(gen);

        // Access the element at the random index
        Article article = combined_news.at(randomIndex);

        // Return a new DatasetPost only if the article matches skills
        if (article.matches_skills >= min_matches) {
            bool isArticleFake = (article.news_status == "false-news");
            return new DatasetPost(this, article, isArticleFake,this->NumberOfPosts);
        }
    }

    // Return nullptr if no suitable post found
    return nullptr;
}

bool DataSetChecker::spreadPost() {
    DatasetPost* randomPost = getRandomPost();

    if (randomPost == nullptr) {
        return false;
    }

    if(randomPost->isFake)
    {
        this->state = Infectious;
    }
    else if(!randomPost->isFake)
    {
        this->state = Recovered;
    }
    sharedPosts.push_back(randomPost);
    addToFollowerFeeds(randomPost);
    return true;
}



bool DataSetChecker::resharePost() {
    if (this->feed.empty()) {
        return false; // nothing to reshare
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);

    for (auto it = feed.rbegin(); it != feed.rend(); ++it) {
        auto& [sharer, post] = *it;
        if (dis(gen) == 1) {
            int matches = 0;
            matches = post->article.matches_skills;
            if (matches > min_matches) {

                if(post->isFake)
                {
                    this->state = Infectious;
                }
                else if (!post->isFake)
                {
                    this->state= Recovered;
                }
                post->addResharer(sharer, this);
                this->re_sharedPosts.push_back(post);
                addToFollowerFeeds(post);
                return true;
            }
            //if no matches, checker will try to find another post
        }
    }

    return false; // no valid post selected for resharing
}
