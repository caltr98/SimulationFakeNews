//
// Created by iperc on 21/07/2024.
//
#include "Model.h"
#include "./DatasetModelFiles/ModelDataset.h"
#include <omp.h>

namespace fs = std::filesystem;

void printDirectoryElements(const std::string& path) {
    for (const auto& entry : fs::directory_iterator(path)) {
        std::cout << entry.path() << std::endl;
    }
}


void run_monte_carlo_simulation_ground_truth(int num_runs, int num_users, int num_checkers, int epochs, int seed) {
    std::mt19937 gen(seed);  // Use seed passed from main
    std::uniform_real_distribution<> prob_dist(0.1, 0.9);

    // Ground Truth simulation
    Model ground_truth_model(num_users, num_checkers, 0, 0, 0, epochs);

    // Parallelize the simulation runs
    for (int i = 0; i < num_runs; i++) {
        // Randomize probabilities for each run
        double prob_susceptible = prob_dist(gen);
        double prob_recover = prob_dist(gen);
        double prob_spreader = prob_dist(gen);

        ground_truth_model.execute_trial_with_parameters(prob_susceptible, prob_recover, prob_spreader, i);

        // Check if the current epoch is a multiple of 20 and print
        if (i % 20 == 0) {
            std::cout << "index " << i << " completed " << num_runs << " runs." << std::endl;
            std::cout << "Probabilities: Susceptible = " << prob_susceptible
                      << ", Recover = " << prob_recover
                      << ", Spreader = " << prob_spreader << std::endl;
        }
    }

    ground_truth_model.finalize(num_runs);
}

void run_monte_carlo_simulation_skill_based(int num_runs, int num_users, int num_checkers, int epochs, int seed) {
    std::mt19937 gen(seed);  // Use seed passed from main
    std::uniform_real_distribution<> prob_dist(0.1, 0.9);

    // Skill Based simulation
    ModelDataset skill_based_model(num_users, num_checkers, 0, 0, 0, epochs, "", 1);

    // Parallelize the simulation runs
    for (int i = 0; i < num_runs; i++) {
        // Randomize probabilities for each run
        double prob_susceptible = prob_dist(gen);
        double prob_recover = prob_dist(gen);
        double prob_spreader = prob_dist(gen);


        skill_based_model.execute_trial_with_parameters(prob_susceptible, prob_recover, prob_spreader, i);


        // Check if the current epoch is a multiple of 20 and print
        if (i % 20 == 0) {
            std::cout << "index " << i << " completed " << num_runs << " runs." << std::endl;
            std::cout << "Probabilities: Susceptible = " << prob_susceptible
                      << ", Recover = " << prob_recover
                      << ", Spreader = " << prob_spreader << std::endl;
        }
    }
    skill_based_model.finalize(num_runs);
}

int main() {
    // Parameters for Model initialization
    int num_users = 100;
    int num_checkers = 60;
    int epochs = 100;
    int monte_carlo_runs = 1000;
    int seed = 42;  // Fixed seed

    //run_monte_carlo_simulation_ground_truth(monte_carlo_runs, num_users, num_checkers, epochs, seed);
    run_monte_carlo_simulation_skill_based(monte_carlo_runs, num_users, num_checkers, epochs, seed);


    return 0;
}