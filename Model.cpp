// Created by iperc on 13/07/2024.
//

#include "Model.h"

#include <algorithm>
#include <chrono>
#include <filesystem> // Include filesystem for path operations

#include "Checker.h"


Model::Model(int num_users, int num_checkers, double prob_susceptible, double prob_recover, double prob_spreader,int epochs)
    : numUsers(num_users), numCheckers(num_checkers), prob_susceptible(prob_susceptible),
      prob_recover(prob_recover), prob_spreader(prob_spreader),epochs(epochs) {
    this->NumberOfPosts = new int;
    initializeUsers();
    //this->timestamp=std::to_string(std::time(0));
    this->timestamp="";
}

Model::Model(): numUsers(0), numCheckers(0), prob_susceptible(0), prob_recover(0), prob_spreader(0), epochs(0)
{
}

void Model::initializeUsers(){

    *this->NumberOfPosts = 0;
    int currentUsers=0;
    // Initialize regular users
    for (int i = 0; i < numUsers - numCheckers; ++i) {
        auto u = std::make_shared<User>(currentUsers++, "User", prob_susceptible, prob_recover, prob_spreader, Susceptible,this->NumberOfPosts);
        users.push_back(u);
    }

    // Initialize checkers
    for (int i = 0; i < numCheckers; ++i) {
        auto u = std::make_shared<Checker>(currentUsers++,this->NumberOfPosts);
        users.push_back(u);
    }

    // Shuffle the users vector, so that the checkers are spread in the network
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(users.begin(), users.end(), g);
    setupFollowRelationships();
}

void Model::setupFollowRelationships() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution d(0.5);

    for (const auto& user : users) {
        for (auto& other_user : users) {
            if (user != other_user && d(gen)) {
                other_user->follow(user.get());
            }
        }
    }
}


Model::UserTrialStats Model::collectUserTrialStats() {
    UserTrialStats stats = UserTrialStats();

    std::vector<int> reSharedPostsCounts;
    std::vector<int> fakeReSharedPostsCounts;

    for (const auto& user : users) {
        auto [SharedPosts, fakeSharedPosts] = user->allSharedPostsStats();
        auto [reSharedPosts, fakeReSharedPosts] = user->allReSharedPostStats();

        stats.update(SharedPosts,reSharedPosts,fakeSharedPosts,SharedPosts-fakeSharedPosts,fakeReSharedPosts,reSharedPosts-fakeReSharedPosts, user->state == Recovered, user->state == Infectious, user->state== Susceptible);
    }
    stats.finalizeStats();
    return stats;
}


Model::PostStats Model::collectPostStats() {
    std::vector<Post*> sharedPosts;

    for (const auto& user : users) {
        auto userSharedPosts = user->allSharedPosts();
        sharedPosts.insert(sharedPosts.end(), userSharedPosts.begin(), userSharedPosts.end());
    }

    if (sharedPosts.empty()) {
        std::cerr << "No posts to collect statistics from." << std::endl;
    }

    PostStats postStats;

    for (const auto& post : sharedPosts) {
        postStats.update(*post);
    }
    postStats.finalizeStats();
    return postStats;
}

void Model::printPostTrialStats(const PostStats& stats)
{
    stats.print();
}

struct EpochStateStats {
    std::vector<std::unordered_map<State, int>> stateCountsPerEpoch;

    void updateEpochStats(const std::vector<State>& states) {
        std::unordered_map<State, int> stateCount;
        for (const auto& state : states) {
            stateCount[state]++;
        }
        stateCountsPerEpoch.push_back(stateCount);
    }

    static std::string stateToString(const State state) {
        switch (state) {
        case Recovered: return "Recovered";
        case Susceptible: return "Susceptible";
        case Infectious: return "Infectious";
        default: return "Unknown";
        }
    }

    void print() const {
        for (size_t i = 0; i < stateCountsPerEpoch.size(); ++i) {
            std::cout << "Epoch " << i << ":";
            for (const auto& stateCount : stateCountsPerEpoch[i]) {
                std::cout << " " << stateToString(stateCount.first) << "=" << stateCount.second;
            }
            std::cout << std::endl;
        }
    }
};
Model::EpochStateStats Model::collectMostPopularStatePerEpoch() const {
    EpochStateStats epochStateStats;

    if (users.empty()) return epochStateStats;

    for (size_t epoch = 0; epoch < this->epochs; ++epoch) {
        std::vector<State> statesForEpoch;
        for (const auto& user : users) {
            statesForEpoch.push_back(user->perEpochState[epoch]);
        }
        epochStateStats.updateEpochStats(statesForEpoch);
    }

    return epochStateStats;
}

void Model::printEpochStateStats(const EpochStateStats& stats)
{
    stats.print();
}

void Model::execute()
{
    int i =0;

    for(i = 0;i<this->epochs;i++)
    {
        for (auto user : this->users)
        {
         user->Act();
        }
    }
    printEpochStateStats(this->collectMostPopularStatePerEpoch());
    printPostTrialStats(this->collectPostStats());
    //printUserTrialStats(this->collectUserTrialStats());
}

void Model::printAggregatedPostTrialStats(const PostStatsAggregator stats)
{
    stats.print();

}

void Model::printAggregatedUserTrialStats(const UserTrialStatsAggregator stats)
{
}

void Model::printAggregatedEpochStateStats(const EpochStateStatsAggregator stats, int n_trials)

{
    stats.print(n_trials);
}

void Model::csvAggregatedPostTrialStats(const PostStatsAggregator stats) const
{
    // Get the current directory and create the absolute path
    std::string currentDir = std::filesystem::current_path().string();
    std::string filePath = currentDir + "/" + this->timestamp + "Post.csv";
    stats.toCSV(filePath);
}

void Model::csvAggregatedUserTrialStats(const UserTrialStatsAggregator stats) const
{
    // Get the current directory and create the absolute path
    std::string currentDir = std::filesystem::current_path().string();
    std::string filePath = currentDir + "/" + this->timestamp + "UserTrial.csv";
    std::cout << filePath <<std::endl;
    stats.toCSV(filePath);

}

void Model::csvAggregatedEpochStateStats(const EpochStateStatsAggregator stats, int n_trials) const
{
    // Get the current directory and create the absolute path
    std::string currentDir = std::filesystem::current_path().string();
    std::string filePath = currentDir + "/" + this->timestamp + "Epochs.csv";
    std::cout << filePath <<std::endl;
    stats.toCSV(filePath, n_trials);
}


// Execute multiple trials and print aggregated statistics
// Execute multiple trials and print aggregated statistics
void Model::execute_trials(int n_trials) {
    std::vector<PostStatsAggregator> postStatsAggregators(n_trials); // Vector to aggregate post statistics for each trial
    std::vector<UserTrialStatsAggregator> userStatsAggregators(n_trials); // Vector to aggregate user trial statistics for each trial
    std::vector<EpochStateStatsAggregator> epochStatsAggregators(n_trials); // Vector to aggregate epoch state statistics for each trial


    PostStatsAggregator post_stats_aggregator;
    UserTrialStatsAggregator user_trial_stats_aggregator;
    EpochStateStatsAggregator epoch_state_stats_aggregator;
    for (int trial = 0; trial < n_trials; ++trial) {
        // Run the simulation for specified number of epochs
        for (int i = 0; i < epochs; ++i) {
            for (auto user : users) {
                user->Act(); // Perform actions for each user in each epoch
            }
        }

        // Collect statistics for the current trial
        post_stats_aggregator.update(collectPostStats());
        user_trial_stats_aggregator.update(collectUserTrialStats());
        epoch_state_stats_aggregator.update(collectMostPopularStatePerEpoch());

        std::cout << "Trial n:"<<trial<<" out of " << n_trials<<"ended"<< std::endl;
        users.clear();
        initializeUsers();//generate another OSN
    }

    // Print aggregated statistics
    printAggregatedPostTrialStats(post_stats_aggregator);
    printAggregatedUserTrialStats(user_trial_stats_aggregator);
    //printAggregatedEpochStateStats(epoch_state_stats_aggregator);

    //store in csv
    csvAggregatedPostTrialStats(post_stats_aggregator);
    csvAggregatedUserTrialStats(user_trial_stats_aggregator);
    csvAggregatedEpochStateStats(epoch_state_stats_aggregator, n_trials);

}
