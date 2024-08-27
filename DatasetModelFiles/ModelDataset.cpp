//
// Created by iperc on 18/07/2024.
//

#include "ModelDataset.h"

#include <utility>

#include "DatasetUser.h"
#include "DataSetChecker.h"



void ModelDataset::initializeUsers(std::string engines,int min_matches)
{
    *this->NumberOfPosts = 0;

    this->engine = engine;
    this->min_matches=min_matches;
    int currentUsers = 0;
    // Initialize regular users
    for (int i = 0; i < numUsers - numCheckers; i++) {
        std::vector<Article> trueNews, falseNews;

        for (const auto& article : this->Persons.at(i).articles) {
            if (article.news_status == "true-news") {
                trueNews.push_back(article);
            } else {
                falseNews.push_back(article);
            }
        }

        auto u = std::make_shared<DatasetUser>(
            currentUsers++, "User", prob_susceptible, prob_recover, prob_spreader, Susceptible,
            this->Persons.at(i).name, this->Persons.at(i).surname, this->Persons.at(i).gender,
            this->Persons.at(i).bio, trueNews, falseNews,this->NumberOfPosts
        );
        users.push_back(u);
    }


    // Initialize checkers
    for (int i = numUsers - numCheckers; i < numUsers; i++) {
        std::vector<Article> trueNews, falseNews;

        for (const auto& article : this->Persons.at(i).articles) {
            if (article.news_status == "true-news") {
                trueNews.push_back(article);
            } else {
                falseNews.push_back(article);
            }
        }

        auto u = std::make_shared<DataSetChecker>(
            currentUsers++,
            this->Persons.at(i).name, this->Persons.at(i).surname, this->Persons.at(i).gender,
            this->Persons.at(i).bio, engines, trueNews, falseNews, min_matches,this->NumberOfPosts
        );
        users.emplace_back(u);
    }


    // Shuffle the users vector, so that the checkers are spread in the network
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(users.begin(), users.end(), g);

    // Example output for the number of false news articles for the first user
    if (!users.empty()) {
        std::cout << "Number of false news articles for the first user: " << users.at(0)->false_news.size() << std::endl;
    }

    setupFollowRelationships();
}

void ModelDataset::setupFollowRelationships() {
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

ModelDataset::ModelDataset(int num_users, int num_checkers, double prob_susceptible, double prob_recover, double prob_spreader, int epochs,std::string engines, int min_matches)
{
    this->NumberOfPosts = new int;
    //this->timestamp=std::to_string(std::time(0));
    this->timestamp   = "";
    this->numUsers = num_users;
    this->numCheckers = num_checkers;
    this->prob_susceptible = prob_susceptible;
    this->prob_recover = prob_recover;
    this->prob_spreader = prob_spreader;
    this->epochs = epochs;

    this->Persons = readPersonsFromFile("./file.json");
    initializeUsers(std::move(engines),min_matches);
}

void ModelDataset::printAggregatedPostTrialStats(const PostStatsAggregator stats)
{
    stats.print();

}

void ModelDataset::printAggregatedUserTrialStats(const UserTrialStatsAggregator stats)
{

}

void ModelDataset::printAggregatedEpochStateStats(const EpochStateStatsAggregator stats)
{
    stats.print();

}

void ModelDataset::csvAggregatedPostTrialStats(const PostStatsAggregator stats) const
{
    // Get the current directory and create the absolute path
    std::string currentDir = std::filesystem::current_path().string();
    std::string filePath = currentDir + "/" + this->timestamp + "Post.csv";
    stats.toCSV(filePath);

}

void ModelDataset::csvAggregatedUserTrialStats(const UserTrialStatsAggregator stats) const
{
    // Get the current directory and create the absolute path
    std::string currentDir = std::filesystem::current_path().string();
    std::string filePath = currentDir + "/" + this->timestamp + "UserTrial.csv";
    std::cout << filePath <<std::endl;
    stats.toCSV(filePath);

}

void ModelDataset::csvAggregatedEpochStateStats(const EpochStateStatsAggregator stats,int n_trials) const
{
    // Get the current directory and create the absolute path
    std::string currentDir = std::filesystem::current_path().string();
    std::string filePath = currentDir + "/" + this->timestamp + "Epochs.csv";
    std::cout << filePath <<std::endl;
    stats.toCSV(filePath,n_trials);

}


// Register the structs with nlohmann::json
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Skill, skillname)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Article, title, content, news_status, url)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Person, name, surname, gender, bio, articles)


std::tuple<int, int> ModelDataset::allReSharedPostStats()
{
    int countShared = re_sharedPosts.size();
    int countFake = 0;

    for (DatasetPost* post : re_sharedPosts) {
        if (post->isFake) {
            countFake++;
        }
    }

    return std::make_tuple(countShared, countFake);

}

ModelDataset::PostStats ModelDataset::collectPostStats() const
{
    std::vector<DatasetPost*> sharedPosts;

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




ModelDataset::UserTrialStats ModelDataset::collectUserTrialStats()
{
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

ModelDataset::EpochStateStats ModelDataset::collectMostPopularStatePerEpoch()
{
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

std::vector<Person> ModelDataset::readPersonsFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    nlohmann::json j;
    file >> j;

    file.close();
    std::vector<Person> persons;
    for(int i = 0; i< j.size();i++)
    {
        std::cout<<j[i].at("name")<< " " << j[i].at("surname") <<std::endl;
        std::cout<<i<<std::endl;

        Person x;
        ModelDataset::from_json(j[i],x);
        persons.push_back(x);

    }
    return persons;
}
void ModelDataset::execute()
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
    printUserTrialStats(this->collectUserTrialStats());
}
void ModelDataset::printUserTrialStats(const UserTrialStats& stats)
{
}
void ModelDataset::printPostTrialStats(const PostStats& stats)
{
    stats.print();
}
void ModelDataset::printEpochStateStats(const EpochStateStats& stats)
{
    stats.print();
}


// Execute multiple trials and print aggregated statistics
// Execute multiple trials and print aggregated statistics
void ModelDataset::execute_trials(int n_trials) {
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

        std::string fileName = "AnalysisAtTrial" + std::to_string(trial) + "over" + std::to_string(n_trials) + ".csv";

        // Collect statistics for the current trial
        post_stats_aggregator.update(collectPostStats());
        user_trial_stats_aggregator.update(collectUserTrialStats());
        epoch_state_stats_aggregator.update(collectMostPopularStatePerEpoch());


        std::cout << "Trial n:"<<trial<<" out of " << n_trials<<"ended"<< std::endl;
        users.clear(); //remove all users to generate another OSN
        initializeUsers((this->engine),min_matches); //create another OSN

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


