#include "User.h"

#include <utility>

// Constructor definition
User::User(int id, std::string  category, double prob_susceptible, double prob_recover, double prob_spreader, State initial_state,int* NumberOfPosts)
    : id(id), category(std::move(category)), prob_susceptible(prob_susceptible), prob_recover(prob_recover), prob_spreader(prob_spreader), state(initial_state),NumberOfPosts(NumberOfPosts) {}

// Method definitions

bool User::becomeInfectious() {
    return randomEvent(prob_spreader);
}

bool User::becomeSusceptible() {
    return randomEvent(prob_susceptible);
}

bool User::becomeRecovered() const
{
    return randomEvent(prob_recover);
}

bool User::spreadPost() {
    bool isFake = (this->state == Infectious);
    Post* post = new Post(this, isFake,this->NumberOfPosts);
    this->sharedPosts.push_back(post);
    addToFollowerFeeds(post);
    return true;
}

bool User::resharePost() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);

    if (this->feed.empty()) {
        return false; // nothing to reshare
    }

    //whenever status is not infected and there was a false news before
    //read from the feed starting from the end
    for (auto it = feed.rbegin(); it != feed.rend(); ++it) {
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


    return false; // no post selected for resharing
}

bool User::randomEvent(double probability) {
    static std::mt19937 gen{ std::random_device{}() };
    std::uniform_real_distribution<> dis(0.0, 1.0);
    return dis(gen) < probability;
}

void User::addToFollowerFeeds(Post* post) {
    for (auto follower : this->followers) {
        follower->feed.emplace_back(this, post);
    }
}

void User::Act() {
    changeState();
    performAction();
    perEpochState.push_back(this->state);
}


std::vector<State> User::getPerEpochState() const
{
    return this->perEpochState;;
}

std::tuple<int, int> User::allSharedPostsStats() {
    int countShared = sharedPosts.size();
    int countFake = 0;

    for (Post* post : sharedPosts) {
        if (post->isFake) {
            countFake++;
        }
    }

    return std::make_tuple(countShared, countFake);
}

std::tuple<int, int> User::allReSharedPostStats() {
    int countShared = re_sharedPosts.size();
    int countFake = 0;

    for (Post* post : re_sharedPosts) {
        if (post->isFake) {
            countFake++;
        }
    }

    return std::make_tuple(countShared, countFake);
}

std::vector<Post*> User::allSharedPosts()
{
    return this->sharedPosts;
}


void User::changeState() {
    if (this->state == Susceptible && this->becomeInfectious()) {
        this->state = Infectious;
    } else if (this->state == Recovered && this->becomeSusceptible()) {
        this->state = Susceptible;
    } else if (this->state == Infectious && this->becomeRecovered()) {
        this->state = Recovered;
    }
}

void User::performAction() {
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


void User::follow(User* other)
{
    this->followers.push_back(other);
}
