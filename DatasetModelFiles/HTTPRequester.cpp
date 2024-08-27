#include <iostream>
#include <string>
#include "HTTPRequester.h"

#include <iostream>
#include <sstream>

#include "json.hpp"


int forward_analysis(const std::string& document, const std::string bio){
    try {
        http::Request request{"http://localhost:4200/simulation"}; // Updated URL

        // Use nlohmann::json to create the JSON body
        nlohmann::json body_json;
        body_json["document"] = document;
        body_json["bio"] = bio;


        std::cout<<body_json["skills"]<<std::endl;
        std::string body = body_json.dump();

        const auto response = request.send("POST", body, {
            {"Content-Type", "application/json"}
        });

        std::string responseInString =std::string{response.body.begin(), response.body.end()};


        std::cout <<responseInString <<std::endl;
        // Parse the response manually
        auto matches_pos = responseInString.find("\"matches\":");

        int matches = std::stoi(responseInString.substr(matches_pos + 10, responseInString.find(",", matches_pos) - (matches_pos + 10)));

        std::cout << "matches "<< matches<<std::endl;

        return matches;

    } catch (const std::exception& e) {
        std::cerr << "Request failed, error: " << e.what() << '\n';
    }

    return 0;
}
