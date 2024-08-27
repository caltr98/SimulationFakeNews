//
// Created by iperc on 17/07/2024.
//

#include "DatasetUser.h"
#include <utility>
#include "DatasetPost.h"

DatasetUser::DatasetUser(int id, std::string category,
                         double prob_susceptible,
                         double prob_recover, double prob_spreader,
                         State initial_state, std::string name, std::string surname,
                         std::string gender, std::string bio,
                         std::vector<Article> true_news, std::vector<Article> false_news, int* NumberOfPosts)
    : state(Susceptible), id(id), category(std::move(category)), prob_susceptible(prob_susceptible),
      prob_recover(prob_recover), prob_spreader(prob_spreader), initial_state(initial_state),
      name(std::move(name)), surname(std::move(surname)), gender(std::move(gender)),
      bio(std::move(bio)), true_news(true_news), false_news(false_news), NumberOfPosts(NumberOfPosts)
{

}

void DatasetUser::addToFollowerFeeds(DatasetPost* post)
{
    for (auto follower : this->followers) {
        follower->feed.emplace_back(this, post);
    }
}

bool DatasetUser::spreadPost()
{
    bool isFake = (this->state == Infectious);
    DatasetPost* post = getRandomPost(isFake);
    this->sharedPosts.push_back(post);
    addToFollowerFeeds(post);
    return true;
}

bool DatasetUser::resharePost()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);

    if (this->feed.empty()) {
        return false; // nothing to reshare
    }

    //whenever status is not infected and there was a false news before
    //read from the feed starting from the end
    for (auto it = feed.rbegin(); it != feed.rend(); ++it)
    {
        auto& [sharer, post] = *it;
        if (dis(gen) == 1) {
            //trial start from top of the feed post (last one)
            if (post->isFake && this->state != Infectious) {
                if(this->state == Susceptible) //IF SUSCEPTIBLE, being exposed to fake news may yield to switch to Infecious
                {
                    this->state = Infectious;
                }
                else
                {
                    continue; // do not share a fake post if in Recovered state
                }
            }
            post->addResharer(sharer, this);
            this->re_sharedPosts.push_back(post);
            addToFollowerFeeds(post);
            return true;
        }
    }
    return false;
}

std::vector<DatasetPost*> DatasetUser::allSharedPosts() const
{
    return this->sharedPosts;
}

std::tuple<int, int> DatasetUser::allSharedPostsStats()
{
    int countShared = sharedPosts.size();
    int countFake = 0;

    for (DatasetPost* post : sharedPosts) {
        if (post->isFake) {
            countFake++;
        }
    }

    return std::make_tuple(countShared, countFake);
}

DatasetPost* DatasetUser::getRandomPost(bool isFake)
{
    if (isFake) {
        // Check if the vector is not empty
        if (!false_news.empty()) {
            // Seed with a real random value, if available
            std::random_device rd;
            // Use Mersenne Twister engine to generate pseudo-random numbers
            std::mt19937 gen(rd());
            // Generate random index within the range of the vector size
            std::uniform_int_distribution<> dis(0, false_news.size() - 1);
            int randomIndex = dis(gen);

            // Access the element at the random index
            Article article = false_news.at(randomIndex) ;

            auto* randomPost = new DatasetPost(this, article, true, this->NumberOfPosts);
            return randomPost;
        }
    } else {
        // Check if the vector is not empty
        if (!true_news.empty()) {
            // Seed with a real random value, if available
            std::random_device rd;
            // Use Mersenne Twister engine to generate pseudo-random numbers
            std::mt19937 gen(rd());
            // Generate random index within the range of the vector size
            std::uniform_int_distribution<> dis(0, true_news.size() - 1);
            int randomIndex = dis(gen);

            // Access the element at the random index
            Article article = true_news.at(randomIndex);

            auto* randomPost = new DatasetPost(this, article, false,this->NumberOfPosts);
            return randomPost;
        }
    }
    return nullptr;
}

void DatasetUser::performAction()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<> dis({0.3, 0.7}); // 30% for posting, 70% for resharing
    //random choice between posting or not
    if (dis(gen) == 0) {
        this->spreadPost();
    } else {
        this->resharePost();
    }
}

void DatasetUser::changeState()
{
    if (this->state == Susceptible && this->becomeInfectious()) {
        this->state = Infectious;
    } else if (this->state == Recovered && this->becomeSusceptible()) {
        this->state = Susceptible;
    } else if (this->state == Infectious && this->becomeRecovered()) {
        this->state = Recovered;
    }
}

void DatasetUser::Act()
{
    changeState();
    performAction();
    perEpochState.push_back(this->state);
}

bool DatasetUser::becomeInfectious()
{
    return randomEvent(prob_spreader);
}

bool DatasetUser::becomeSusceptible()
{
    return randomEvent(prob_susceptible);
}

bool DatasetUser::becomeRecovered() const
{
    return randomEvent(prob_recover);
}
bool DatasetUser::randomEvent(double probability) {
    static std::mt19937 gen{ std::random_device{}() };
    std::uniform_real_distribution<> dis(0.0, 1.0);
    return dis(gen) < probability;
}


void DatasetUser::follow(DatasetUser* other)
{
    this->followers.push_back(other);
}

std::tuple<int, int> DatasetUser::allReSharedPostStats() {
    int countShared = re_sharedPosts.size();
    int countFake = 0;

    for (DatasetPost* post : re_sharedPosts) {
        if (post->isFake) {
            countFake++;
        }
    }

    return std::make_tuple(countShared, countFake);
}

std::tuple<int, int> DatasetUser::allSharedPostStats() {
    int countShared = re_sharedPosts.size();
    int countFake = 0;

    for (DatasetPost* post : re_sharedPosts) {
        if (post->isFake) {
            countFake++;
        }
    }

    return std::make_tuple(countShared, countFake);
}
