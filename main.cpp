//
// Created by iperc on 21/07/2024.
//
#include "Model.h"
#include "./DatasetModelFiles/ModelDataset.h"

namespace fs = std::filesystem;

void printDirectoryElements(const std::string& path) {
    for (const auto& entry : fs::directory_iterator(path)) {
        std::cout << entry.path() << std::endl;
    }
}

int main() {

    /*
    std::vector<std::string> str;
    str.push_back("Java");
    str.push_back("Javascript");
    std::tuple<int,int,int> x;

    // Parameters for Model initialization
    int num_users = 100;  // Number of users
    int num_checkers = 10;  // Number of checkers
    double prob_susceptible = 0.7;  // Probability of being susceptible
    double prob_recover = 0.2;  // Probability of recovery
    double prob_spreader = 0.3;  // Probability of being a spreader
    int epochs = 1000;  // Number of epochs

    // Create an instance of Model
    Model model(num_users, num_checkers, prob_susceptible, prob_recover, prob_spreader, epochs);

    // Execute the simulation
    //model.execute();
    model.execute_trials(15);

    */
    std::string path = "./"; // specify the directory path
    printDirectoryElements(path);


    // Parameters for Model initialization
    int num_users = 100;  // Number of users
    int num_checkers = 70;  // Number of checkers
    double prob_susceptible = 0.7   ;  // Probability of being susceptible
    double prob_recover = 0.2;  // Probability of recovery
    double prob_spreader = 0.3;  // Probability of being a spreader
    int epochs = 1000;  // Number of epochs

    // Create an instance of Model
    //ModelDataset model(num_users, num_checkers, prob_susceptible, prob_recover, prob_spreader, epochs,"BERT+OJDAPP",    1);

    Model model(num_users, num_checkers, prob_susceptible, prob_recover, prob_spreader, epochs);

    //Model model(num_users, num_checkers, prob_susceptible, prob_recover, prob_spreader, epochs);
    // Execute the simulation
    //model.execute();
    model.execute_trials(10);



    return 0;
}
