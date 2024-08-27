//
// Created by iperc on 23/07/2024.
//

#ifndef DATASETSTATS_H
#define DATASETSTATS_H
// Define the structs
struct Skill {
    std::string skillname;
    std::string skillURI;
};

struct Article {
    std::string title;
    std::string content;
    std::string news_status;
    std::string url;
    int matches_skills;
    std::vector<Skill> articles_skills;
    int article_id;

};

struct Person {
    std::string name;
    std::string surname;
    std::string gender;
    std::string bio;
    std::string author_id;
    std::vector<Skill> author_skills;
    std::vector<Article> articles;
};


#endif //DATASETSTATS_H
