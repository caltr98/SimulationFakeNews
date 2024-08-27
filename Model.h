//
// Created by iperc on 13/07/2024.
//

#ifndef MODEL_H
#define MODEL_H

#include <algorithm>
#include <fstream>
#include <map>

#include "Post.h"
#include "User.h"
#include <vector>
#include <memory>

class Model
{
protected:
    int numUsers;
    int numCheckers; //subset of numUsers;
    double prob_susceptible;
    double prob_recover;
    double prob_spreader;
    std::vector<std::shared_ptr<User>> users;
    int epochs;
    std::string timestamp;//to be used for file naming

public:
    virtual ~Model() = default;

    int *NumberOfPosts;


    struct UserTrialStats
{
    double totalSharedPosts;
    double totalReSharedPosts;
    double totalFakeSharedPosts;
    double totalTrueSharedPosts;
    double totalFakeReSharedPosts;
    double totalTrueReSharedPosts;

    double totalSquaredSharedPosts;
    double totalSquaredReSharedPosts;
    double totalSquaredFakeSharedPosts;
    double totalSquaredTrueSharedPosts;
    double totalSquaredFakeReSharedPosts;
    double totalSquaredTrueReSharedPosts;

    double averageSharedPosts;
    double averageReSharedPosts;
    double averageFakeSharedPosts;
    double averageTrueSharedPosts;
    double averageFakeReSharedPosts;
    double averageTrueReSharedPosts;


    int countRecovered;
    int countInfectious;
    int countSusceptible;

    int numberOfUsers;

    UserTrialStats()
        : totalSharedPosts(0.0),
          totalReSharedPosts(0.0),
          totalFakeSharedPosts(0.0),
          totalTrueSharedPosts(0.0),
          totalFakeReSharedPosts(0.0),
          totalTrueReSharedPosts(0.0),
          totalSquaredSharedPosts(0.0),
          totalSquaredReSharedPosts(0.0),
          totalSquaredFakeSharedPosts(0.0),
          totalSquaredTrueSharedPosts(0.0),
          totalSquaredFakeReSharedPosts(0.0),
          totalSquaredTrueReSharedPosts(0.0),
          averageSharedPosts(0.0),
          averageReSharedPosts(0.0),
          averageFakeSharedPosts(0.0),
          averageTrueSharedPosts(0.0),
          averageFakeReSharedPosts(0.0),
          averageTrueReSharedPosts(0.0),
          countRecovered(0),
          countInfectious(0),
          countSusceptible(0),
          numberOfUsers(0) {}

    void update(int sharedPosts, int reSharedPosts, int fakeSharedPosts, int trueSharedPosts, int fakeReSharedPosts, int trueReSharedPosts, int
                recovered, int infectious, int susceptible) {
        totalSharedPosts += sharedPosts;
        totalSquaredSharedPosts += sharedPosts * sharedPosts;

        totalReSharedPosts += reSharedPosts;
        totalSquaredReSharedPosts += reSharedPosts * reSharedPosts;

        totalFakeSharedPosts += fakeSharedPosts;
        totalSquaredFakeSharedPosts += fakeSharedPosts * fakeSharedPosts;

        totalTrueSharedPosts += trueSharedPosts;
        totalSquaredTrueSharedPosts += trueSharedPosts * trueSharedPosts;

        totalFakeReSharedPosts += fakeReSharedPosts;
        totalSquaredFakeReSharedPosts += fakeReSharedPosts * fakeReSharedPosts;

        totalTrueReSharedPosts += trueReSharedPosts;
        totalSquaredTrueReSharedPosts += trueReSharedPosts * trueReSharedPosts;


        countRecovered += recovered;
        countInfectious += infectious;
        countSusceptible += susceptible;

    }

    void finalizeStats() {
        if (numberOfUsers == 0) return;

        // Calculate averages
        averageSharedPosts = totalSharedPosts / numberOfUsers;
        averageReSharedPosts = totalReSharedPosts / numberOfUsers;
        averageFakeSharedPosts = totalFakeSharedPosts / numberOfUsers;
        averageTrueSharedPosts = totalTrueSharedPosts / numberOfUsers;
        averageFakeReSharedPosts = totalFakeReSharedPosts / numberOfUsers;
        averageTrueReSharedPosts = totalTrueReSharedPosts / numberOfUsers;

    }
};


    struct PostStats {
    int numberOfPosts;
    int maxDepth;
    int maxBreadth;
    int size;
    int minSize;
    int maxSize;
    double avgDepth;
    double avgBreadth;
    double avgSize;
    double stdDevSize; // Standard deviation of size

    int numberOfFakePosts;
    int maxDepthFake;
    int maxBreadthFake;
    double totalSizeFake; // Total size of fake posts
    double totalSizeSquaredFake; // Total size squared of fake posts
    double avgSizeFake;
    double stdDevSizeFake; // Standard deviation of size for fake posts

    int numberOfTruePosts;
    int maxDepthTrue;
    int maxBreadthTrue;
    double totalSizeTrue; // Total size of true posts
    double totalSizeSquaredTrue; // Total size squared of true posts
    double avgSizeTrue;
    double stdDevSizeTrue; // Standard deviation of size for true posts

    PostStats()
        : numberOfPosts(0),
          maxDepth(std::numeric_limits<int>::min()),
          maxBreadth(std::numeric_limits<int>::min()),
          size(0),
          minSize(std::numeric_limits<int>::max()),
          maxSize(std::numeric_limits<int>::min()),
          avgDepth(0.0),
          avgBreadth(0.0),
          avgSize(0.0),
          stdDevSize(0.0),
          numberOfFakePosts(0),
          maxDepthFake(std::numeric_limits<int>::min()),
          maxBreadthFake(std::numeric_limits<int>::min()),
          totalSizeFake(0.0),
          totalSizeSquaredFake(0.0),
          avgSizeFake(0.0),
          stdDevSizeFake(0.0),
          numberOfTruePosts(0),
          maxDepthTrue(std::numeric_limits<int>::min()),
          maxBreadthTrue(std::numeric_limits<int>::min()),
          totalSizeTrue(0.0),
          totalSizeSquaredTrue(0.0),
          avgSizeTrue(0.0),
          stdDevSizeTrue(0.0) {}

    void update(const Post& post) {
        auto [currentMaxDepth, currentMaxBreadth, currentSize] = post.findMaxDepthBreadthAndSize();
        maxDepth = std::max(maxDepth, currentMaxDepth);
        maxBreadth = std::max(maxBreadth, currentMaxBreadth);
        size = currentSize;
        minSize = std::min(minSize, currentSize);
        maxSize = std::max(maxSize, currentSize);
        numberOfPosts++;

        avgDepth = (avgDepth * (numberOfPosts - 1) + currentMaxDepth) / numberOfPosts;
        avgBreadth = (avgBreadth * (numberOfPosts - 1) + currentMaxBreadth) / numberOfPosts;
        avgSize = (avgSize * (numberOfPosts - 1) + currentSize) / numberOfPosts;

        double delta = currentSize - avgSize;
        stdDevSize = std::sqrt(((stdDevSize * stdDevSize * (numberOfPosts - 1)) + delta * delta) / numberOfPosts);

        if (post.isFake) {
            numberOfFakePosts++;
            maxDepthFake = std::max(maxDepthFake, currentMaxDepth);
            maxBreadthFake = std::max(maxBreadthFake, currentMaxBreadth);

            totalSizeFake += currentSize;
            totalSizeSquaredFake += currentSize * currentSize;
            avgSizeFake = (numberOfFakePosts > 0) ? totalSizeFake / numberOfFakePosts : 0.0;
        } else {
            numberOfTruePosts++;
            maxDepthTrue = std::max(maxDepthTrue, currentMaxDepth);
            maxBreadthTrue = std::max(maxBreadthTrue, currentMaxBreadth);

            totalSizeTrue += currentSize;
            totalSizeSquaredTrue += currentSize * currentSize;
        }
    }

    void finalizeStats() {
        if (numberOfPosts > 1) {
            stdDevSize = std::sqrt((totalSizeSquaredTrue - numberOfPosts * avgSize * avgSize) / (numberOfPosts - 1));
        } else {
            stdDevSize = 0.0; // No variance if only one post
        }

        if (numberOfFakePosts > 0) {
            avgSizeFake = totalSizeFake / numberOfFakePosts;

            if (numberOfFakePosts > 1) {
                stdDevSizeFake = std::sqrt((totalSizeSquaredFake - numberOfFakePosts * avgSizeFake * avgSizeFake) / (numberOfFakePosts - 1));
            } else {
                stdDevSizeFake = 0.0; // No variance if only one fake post
            }
        } else {
            avgSizeFake = 0.0;
            stdDevSizeFake = 0.0;
        }

        if (numberOfTruePosts > 0) {
            avgSizeTrue = totalSizeTrue / numberOfTruePosts;

            if (numberOfTruePosts > 1) {
                stdDevSizeTrue = std::sqrt((totalSizeSquaredTrue - numberOfTruePosts * avgSizeTrue * avgSizeTrue) / (numberOfTruePosts - 1));
            } else {
                stdDevSizeTrue = 0.0; // No variance if only one true post
            }
        } else {
            avgSizeTrue = 0.0;
            stdDevSizeTrue = 0.0;
        }
    }

    void print() const {
        std::cout << "Number of Posts: " << numberOfPosts << std::endl;
        std::cout << "Max Depth: " << maxDepth << ", Max Breadth: " << maxBreadth << ", Size: " << size << std::endl;
        std::cout << "Average Max Depth: " << avgDepth << ", Average Max Breadth: " << avgBreadth << ", Average Size: " << avgSize << std::endl;
        std::cout << "Standard Deviation Size: " << stdDevSize << std::endl;
        std::cout << "Max Size: " << maxSize << ", Min Size: " << minSize << std::endl;

        std::cout << "Number of Fake Posts: " << numberOfFakePosts << std::endl;
        std::cout << "Max Depth (Fake): " << maxDepthFake << ", Max Breadth (Fake): " << maxBreadthFake << ", Average Size (Fake): " << avgSizeFake << std::endl;
        std::cout << "Standard Deviation Size (Fake): " << stdDevSizeFake << std::endl;

        std::cout << "Number of True Posts: " << numberOfTruePosts << std::endl;
        std::cout << "Max Depth (True): " << maxDepthTrue << ", Max Breadth (True): " << maxBreadthTrue << ", Average Size (True): " << avgSizeTrue << std::endl;
        std::cout << "Standard Deviation Size (True): " << stdDevSizeTrue << std::endl;
    }
};


    struct EpochStateStats {
        std::vector<std::unordered_map<State, int>> stateCountsPerEpoch;

        void updateEpochStats(const std::vector<State>& states) {
            std::unordered_map<State, int> stateCount;
            //initialization of state
            stateCount[Susceptible] = 0;
            stateCount[Infectious] = 0;
            stateCount[Recovered] = 0;
            for (const auto& state : states) {
                stateCount[state]++;
            }
            stateCountsPerEpoch.push_back(stateCount);
        }

        static std::string stateToString(State state) {
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

    struct UserTrialStatsAggregator
    {
        int totalTrials;

        double sum_totalSharedPosts;
        double sum_totalFakeSharedPosts;
        double sum_totalReSharedPosts;
        double sum_totalFakeReSharedPosts;
        double sum_totalRecovered;
        double sum_totalInfectious;
        double sum_totalSusceptible;
        double sum_maxReSharedPosts;
        double sum_minReSharedPosts;
        double sum_avgReSharedPosts;
        double sum_stdDevReSharedPosts;
        double sum_maxFakeReSharedPosts;
        double sum_minFakeReSharedPosts;
        double sum_avgFakeReSharedPosts;
        double sum_stdDevFakeReSharedPosts;

        double sumSq_totalSharedPosts;
        double sumSq_totalFakeSharedPosts;
        double sumSq_totalReSharedPosts;
        double sumSq_totalFakeReSharedPosts;
        double sumSq_totalRecovered;
        double sumSq_totalInfectious;
        double sumSq_totalSusceptible;
        double sumSq_maxReSharedPosts;
        double sumSq_minReSharedPosts;
        double sumSq_avgReSharedPosts;
        double sumSq_stdDevReSharedPosts;
        double sumSq_maxFakeReSharedPosts;
        double sumSq_minFakeReSharedPosts;
        double sumSq_avgFakeReSharedPosts;
        double sumSq_stdDevFakeReSharedPosts;

        UserTrialStatsAggregator()
            : totalTrials(0),
              sum_totalSharedPosts(0.0), sum_totalFakeSharedPosts(0.0), sum_totalReSharedPosts(0.0), sum_totalFakeReSharedPosts(0.0),
              sum_totalRecovered(0.0), sum_totalInfectious(0.0), sum_totalSusceptible(0.0),
              sumSq_totalSharedPosts(0.0), sumSq_totalFakeSharedPosts(0.0), sumSq_totalReSharedPosts(0.0), sumSq_totalFakeReSharedPosts(0.0),
              sumSq_totalRecovered(0.0), sumSq_totalInfectious(0.0), sumSq_totalSusceptible(0.0) {}

        void update(const UserTrialStats& stats) {
            totalTrials++;
            sum_totalSharedPosts += stats.totalSharedPosts;
            sum_totalFakeSharedPosts += stats.totalFakeSharedPosts;
            sum_totalReSharedPosts += stats.totalReSharedPosts;
            sum_totalFakeReSharedPosts += stats.totalFakeReSharedPosts;
            sum_totalRecovered += stats.countRecovered;
            sum_totalInfectious += stats.countInfectious;
            sum_totalSusceptible += stats.countSusceptible;

            sumSq_totalSharedPosts += stats.totalSharedPosts * stats.totalSharedPosts;
            sumSq_totalFakeSharedPosts += stats.totalFakeSharedPosts * stats.totalFakeSharedPosts;
            sumSq_totalReSharedPosts += stats.totalReSharedPosts * stats.totalReSharedPosts;
            sumSq_totalFakeReSharedPosts += stats.totalFakeReSharedPosts * stats.totalFakeReSharedPosts;
            sumSq_totalRecovered += stats.countRecovered * stats.countRecovered;
            sumSq_totalInfectious += stats.countInfectious * stats.countInfectious;
            sumSq_totalSusceptible += stats.countSusceptible * stats.countSusceptible;
        }

        [[nodiscard]] double getAverage(double sum) const {
            return (totalTrials > 0) ? sum / totalTrials : 0.0;
        }

        [[nodiscard]] double getStdDev(double sum, double sumSq) const {
            return (totalTrials > 0) ? std::sqrt((sumSq / totalTrials) - (sum / totalTrials) * (sum / totalTrials)) : 0.0;
        }

        void toCSV(const std::string& filename) const {
            std::ofstream file(filename, std::ios::out);
            if (file.is_open()) {
                if (file.tellp() == 0) { // Check if the file is empty
                    file << "Total Trials,Average Total Shared Posts,Average Total Fake Shared Posts,Average Total ReShared Posts,Average Total Fake ReShared Posts,Average Total Recovered,Average Total Infectious,Average Total Susceptible,"
                         << "Std Dev Total Shared Posts,Std Dev Total Fake Shared Posts,Std Dev Total ReShared Posts,Std Dev Total Fake ReShared Posts,Std Dev Total Recovered,Std Dev Total Infectious,Std Dev Total Susceptible\n";
                }
                file << totalTrials << ","
                     << getAverage(sum_totalSharedPosts) << ","
                     << getAverage(sum_totalFakeSharedPosts) << ","
                     << getAverage(sum_totalReSharedPosts) << ","
                     << getAverage(sum_totalFakeReSharedPosts) << ","
                     << getAverage(sum_totalRecovered) << ","
                     << getAverage(sum_totalInfectious) << ","
                     << getAverage(sum_totalSusceptible) << ","
                     << getStdDev(sum_totalSharedPosts, sumSq_totalSharedPosts) << ","
                     << getStdDev(sum_totalFakeSharedPosts, sumSq_totalFakeSharedPosts) << ","
                     << getStdDev(sum_totalReSharedPosts, sumSq_totalReSharedPosts) << ","
                     << getStdDev(sum_totalFakeReSharedPosts, sumSq_totalFakeReSharedPosts) << ","
                     << getStdDev(sum_totalRecovered, sumSq_totalRecovered) << ","
                     << getStdDev(sum_totalInfectious, sumSq_totalInfectious) << ","
                     << getStdDev(sum_totalSusceptible, sumSq_totalSusceptible) << "\n";
                file.close();
            } else {
                std::cerr << "Unable to open file for writing: " << filename << std::endl;
            }
        }
    };


    struct PostStatsAggregator {
    int totalTrials;

    // Averages
    double avg_numberOfPosts;
    double avg_maxDepth;
    double avg_maxBreadth;
    double avg_size;
    double avg_avgDepth;
    double avg_avgBreadth;
    double avg_avgSize;
    double avg_stdDevSize;

    double avg_numberOfFakePosts;
    double avg_maxDepthFake;
    double avg_maxBreadthFake;
    double avg_sizeFake;

    double avg_numberOfTruePosts;
    double avg_maxDepthTrue;
    double avg_maxBreadthTrue;
    double avg_sizeTrue;

    // Sums of squares for standard deviation calculation
    double sumSq_numberOfPosts;
    double sumSq_maxDepth;
    double sumSq_maxBreadth;
    double sumSq_size;
    double sumSq_avgDepth;
    double sumSq_avgBreadth;
    double sumSq_avgSize;
    double sumSq_stdDevSize;

    double sumSq_numberOfFakePosts;
    double sumSq_maxDepthFake;
    double sumSq_maxBreadthFake;
    double sumSq_sizeFake;

    double sumSq_numberOfTruePosts;
    double sumSq_maxDepthTrue;
    double sumSq_maxBreadthTrue;
    double sumSq_sizeTrue;

    PostStatsAggregator()
        : totalTrials(0),
          avg_numberOfPosts(0.0),
          avg_maxDepth(0.0),
          avg_maxBreadth(0.0),
          avg_size(0.0),
          avg_avgDepth(0.0),
          avg_avgBreadth(0.0),
          avg_avgSize(0.0),
          avg_stdDevSize(0.0),
          avg_numberOfFakePosts(0.0),
          avg_maxDepthFake(0.0),
          avg_maxBreadthFake(0.0),
          avg_sizeFake(0.0),
          avg_numberOfTruePosts(0.0),
          avg_maxDepthTrue(0.0),
          avg_maxBreadthTrue(0.0),
          avg_sizeTrue(0.0),
          sumSq_numberOfPosts(0.0),
          sumSq_maxDepth(0.0),
          sumSq_maxBreadth(0.0),
          sumSq_size(0.0),
          sumSq_avgDepth(0.0),
          sumSq_avgBreadth(0.0),
          sumSq_avgSize(0.0),
          sumSq_stdDevSize(0.0),
          sumSq_numberOfFakePosts(0.0),
          sumSq_maxDepthFake(0.0),
          sumSq_maxBreadthFake(0.0),
          sumSq_sizeFake(0.0),
          sumSq_numberOfTruePosts(0.0),
          sumSq_maxDepthTrue(0.0),
          sumSq_maxBreadthTrue(0.0),
          sumSq_sizeTrue(0.0) {}

    void update(const PostStats& stats) {
        totalTrials++;

        avg_numberOfPosts += (stats.numberOfPosts - avg_numberOfPosts) / totalTrials;
        sumSq_numberOfPosts += stats.numberOfPosts * stats.numberOfPosts;

        avg_maxDepth += (stats.maxDepth - avg_maxDepth) / totalTrials;
        sumSq_maxDepth += stats.maxDepth * stats.maxDepth;

        avg_maxBreadth += (stats.maxBreadth - avg_maxBreadth) / totalTrials;
        sumSq_maxBreadth += stats.maxBreadth * stats.maxBreadth;

        avg_size += (stats.size - avg_size) / totalTrials;
        sumSq_size += stats.size * stats.size;

        avg_avgDepth += (stats.avgDepth - avg_avgDepth) / totalTrials;
        sumSq_avgDepth += stats.avgDepth * stats.avgDepth;

        avg_avgBreadth += (stats.avgBreadth - avg_avgBreadth) / totalTrials;
        sumSq_avgBreadth += stats.avgBreadth * stats.avgBreadth;

        avg_avgSize += (stats.avgSize - avg_avgSize) / totalTrials;
        sumSq_avgSize += stats.avgSize * stats.avgSize;

        avg_stdDevSize += (stats.stdDevSize - avg_stdDevSize) / totalTrials;
        sumSq_stdDevSize += stats.stdDevSize * stats.stdDevSize;

        avg_numberOfFakePosts += (stats.numberOfFakePosts - avg_numberOfFakePosts) / totalTrials;
        sumSq_numberOfFakePosts += stats.numberOfFakePosts * stats.numberOfFakePosts;

        avg_maxDepthFake += (stats.maxDepthFake - avg_maxDepthFake) / totalTrials;
        sumSq_maxDepthFake += stats.maxDepthFake * stats.maxDepthFake;

        avg_maxBreadthFake += (stats.maxBreadthFake - avg_maxBreadthFake) / totalTrials;
        sumSq_maxBreadthFake += stats.maxBreadthFake * stats.maxBreadthFake;

        avg_sizeFake += (stats.avgSizeFake - avg_sizeFake) / totalTrials;
        sumSq_sizeFake += stats.avgSizeFake * stats.avgSizeFake;

        avg_numberOfTruePosts += (stats.numberOfTruePosts - avg_numberOfTruePosts) / totalTrials;
        sumSq_numberOfTruePosts += stats.numberOfTruePosts * stats.numberOfTruePosts;

        avg_maxDepthTrue += (stats.maxDepthTrue - avg_maxDepthTrue) / totalTrials;
        sumSq_maxDepthTrue += stats.maxDepthTrue * stats.maxDepthTrue;

        avg_maxBreadthTrue += (stats.maxBreadthTrue - avg_maxBreadthTrue) / totalTrials;
        sumSq_maxBreadthTrue += stats.maxBreadthTrue * stats.maxBreadthTrue;

        avg_sizeTrue += (stats.avgSizeTrue - avg_sizeTrue) / totalTrials;
        sumSq_sizeTrue += stats.avgSizeTrue * stats.avgSizeTrue;
    }

    double calculate_std_dev(double sumSq, double avg, int count) const {
        if (count <= 1) return std::numeric_limits<double>::quiet_NaN();
        double variance = (sumSq - count * avg * avg) / (count - 1);
        return sqrt(variance);
    }

    void finalize(PostStats& result) const {
        result.numberOfPosts = avg_numberOfPosts;
        result.maxDepth = avg_maxDepth;
        result.maxBreadth = avg_maxBreadth;
        result.size = avg_size;
        result.avgDepth = avg_avgDepth;
        result.avgBreadth = avg_avgBreadth;
        result.avgSize = avg_avgSize;
        result.stdDevSize = calculate_std_dev(sumSq_stdDevSize, avg_stdDevSize, totalTrials);

        result.numberOfFakePosts = avg_numberOfFakePosts;
        result.maxDepthFake = avg_maxDepthFake;
        result.maxBreadthFake = avg_maxBreadthFake;
        result.avgSizeFake = avg_sizeFake;

        result.numberOfTruePosts = avg_numberOfTruePosts;
        result.maxDepthTrue = avg_maxDepthTrue;
        result.maxBreadthTrue = avg_maxBreadthTrue;
        result.avgSizeTrue = avg_sizeTrue;
    }

    void print() const {
        std::cout << "Post Statistics Aggregator:" << std::endl;
        std::cout << "Total Trials: " << totalTrials << std::endl;
        std::cout << "Average Number of Posts: " << avg_numberOfPosts << std::endl;
        std::cout << "Average Max Depth: " << avg_maxDepth << std::endl;
        std::cout << "Average Max Breadth: " << avg_maxBreadth << std::endl;
        std::cout << "Average Size: " << avg_size << std::endl;
        std::cout << "Average Average Depth: " << avg_avgDepth << std::endl;
        std::cout << "Average Average Breadth: " << avg_avgBreadth << std::endl;
        std::cout << "Average Average Size: " << avg_avgSize << std::endl;
        std::cout << "Standard Deviation of Size: " << calculate_std_dev(sumSq_size, avg_size, totalTrials) << std::endl;

        std::cout << "Average Number of Fake Posts: " << avg_numberOfFakePosts << std::endl;
        std::cout << "Average Max Depth of Fake Posts: " << avg_maxDepthFake << std::endl;
        std::cout << "Average Max Breadth of Fake Posts: " << avg_maxBreadthFake << std::endl;
        std::cout << "Average Size of Fake Posts: " << avg_sizeFake << std::endl;

        std::cout << "Average Number of True Posts: " << avg_numberOfTruePosts << std::endl;
        std::cout << "Average Max Depth of True Posts: " << avg_maxDepthTrue << std::endl;
        std::cout << "Average Max Breadth of True Posts: " << avg_maxBreadthTrue << std::endl;
        std::cout << "Average Size of True Posts: " << avg_sizeTrue << std::endl;
    }

    void toCSV(const std::string& filename) const {
        std::ofstream file(filename);
        if (file.is_open()) {
            file << "Metric,Average,StandardDeviation" << std::endl;
            file << "NumberOfPosts," << avg_numberOfPosts << "," << calculate_std_dev(sumSq_numberOfPosts, avg_numberOfPosts, totalTrials) << std::endl;
            file << "MaxDepth," << avg_maxDepth << "," << calculate_std_dev(sumSq_maxDepth, avg_maxDepth, totalTrials) << std::endl;
            file << "MaxBreadth," << avg_maxBreadth << "," << calculate_std_dev(sumSq_maxBreadth, avg_maxBreadth, totalTrials) << std::endl;
            file << "Size," << avg_size << "," << calculate_std_dev(sumSq_size, avg_size, totalTrials) << std::endl;
            file << "AverageDepth," << avg_avgDepth << "," << calculate_std_dev(sumSq_avgDepth, avg_avgDepth, totalTrials) << std::endl;
            file << "AverageBreadth," << avg_avgBreadth << "," << calculate_std_dev(sumSq_avgBreadth, avg_avgBreadth, totalTrials) << std::endl;
            file << "AverageSize," << avg_avgSize << "," << calculate_std_dev(sumSq_avgSize, avg_avgSize, totalTrials) << std::endl;
            file << "StdDevSize," << avg_stdDevSize << "," << calculate_std_dev(sumSq_stdDevSize, avg_stdDevSize, totalTrials) << std::endl;

            file << "NumberOfFakePosts," << avg_numberOfFakePosts << "," << calculate_std_dev(sumSq_numberOfFakePosts, avg_numberOfFakePosts, totalTrials) << std::endl;
            file << "MaxDepthFake," << avg_maxDepthFake << "," << calculate_std_dev(sumSq_maxDepthFake, avg_maxDepthFake, totalTrials) << std::endl;
            file << "MaxBreadthFake," << avg_maxBreadthFake << "," << calculate_std_dev(sumSq_maxBreadthFake, avg_maxBreadthFake, totalTrials) << std::endl;
            file << "SizeFake," << avg_sizeFake << "," << calculate_std_dev(sumSq_sizeFake, avg_sizeFake, totalTrials) << std::endl;

            file << "NumberOfTruePosts," << avg_numberOfTruePosts << "," << calculate_std_dev(sumSq_numberOfTruePosts, avg_numberOfTruePosts, totalTrials) << std::endl;
            file << "MaxDepthTrue," << avg_maxDepthTrue << "," << calculate_std_dev(sumSq_maxDepthTrue, avg_maxDepthTrue, totalTrials) << std::endl;
            file << "MaxBreadthTrue," << avg_maxBreadthTrue << "," << calculate_std_dev(sumSq_maxBreadthTrue, avg_maxBreadthTrue, totalTrials) << std::endl;
            file << "SizeTrue," << avg_sizeTrue << "," << calculate_std_dev(sumSq_sizeTrue, avg_sizeTrue, totalTrials) << std::endl;

            file.close();
        } else {
            std::cerr << "Unable to open file for writing: " << filename << std::endl;
        }
    }
};

    struct EpochStateStatsAggregator {
        std::vector<std::unordered_map<State, int>> stateCountsPerEpoch;

        static std::string stateToString(State state)
        {
            switch (state) {
            case Recovered: return "Recovered";
            case Susceptible: return "Susceptible";
            case Infectious: return "Infectious";
            default: return "Unknown";
            }
        }

        void update(const EpochStateStats& stats) {
            for (size_t epoch = 0; epoch < stats.stateCountsPerEpoch.size(); ++epoch) {
                if (epoch >= stateCountsPerEpoch.size()) {
                    stateCountsPerEpoch.push_back({});
                }
                for (const auto& pair : stats.stateCountsPerEpoch[epoch]) {
                    stateCountsPerEpoch[epoch][pair.first] += pair.second;
                }
            }
        }

        void print(int n_trials) const {
            std::cout << "Epoch State Statistics Aggregator:" << std::endl;
            for (size_t i = 0; i < stateCountsPerEpoch.size(); ++i) {
                std::cout << "Epoch " << i << ":";
                for (const auto& pair : stateCountsPerEpoch[i]) {
                    std::cout << " " << stateToString(pair.first) << "=" << pair.second/n_trials;
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }

        void toCSV(const std::string& filename,int n_trials) const {
            std::ofstream file(filename, std::ios::app); // Use append mode
            if (!file.is_open()) {
                file.open(filename, std::ios::out);
            }
            if (file.is_open()) {
                if (file.tellp() == 0) { // Check if the file is empty
                    file << "Epoch,State,Count\n";
                }
                for (size_t i = 0; i < stateCountsPerEpoch.size(); ++i) {
                    for (const auto& pair : stateCountsPerEpoch[i]) {
                        file << i << "," << stateToString(pair.first) << "," << pair.second/n_trials << "\n";
                    }
                }
                file.close();
            }
        }
    };

    Model(int num_users, int num_checkers, double prob_susceptible, double prob_recover, double prob_spreader,
          int epochs);
    Model();


    virtual void initializeUsers();
    virtual void setupFollowRelationships();
    virtual Model::UserTrialStats collectUserTrialStats();
    static void printUserTrialStats(const UserTrialStats& trial_stats);
    virtual Model::PostStats collectPostStats();
    static void printPostTrialStats(const PostStats& stats);
    EpochStateStats collectMostPopularStatePerEpoch() const;
    static void printEpochStateStats(const EpochStateStats& stats);
    void plotStats(const UserTrialStats& userTrialStats, const EpochStateStats& epochStateStats) const;


    void execute();
    static void printAggregatedPostTrialStats(const PostStatsAggregator stats);
    static void printAggregatedUserTrialStats(const UserTrialStatsAggregator stats);
    static void printAggregatedEpochStateStats(const EpochStateStatsAggregator stats, int n_trials);
    void csvAggregatedPostTrialStats(const PostStatsAggregator stats) const;
    void csvAggregatedUserTrialStats(const UserTrialStatsAggregator stats) const;
    void csvAggregatedEpochStateStats(const EpochStateStatsAggregator stats, int n_trials) const;


    void execute_trials(int n_trials);
};
#endif // MODEL_H
