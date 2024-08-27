Simulation of spread of fake news inside Online Social Network, developed for my master thesis.

There are two possibile simulations:
1) Base Simulation in the root folder, where there are fact checkers that know the ground truth of true/false news and stop the spread if meet a false news
2) DataSet Model: in this case, the fact checkers work with a synthetic dataset generated with GPT-4o containing **bio** and **articles*** of an author. The generating code is in CAVS Repository at section [ProfilesAndNewsGenerator](https://github.com/caltr98/CAVS/tree/master/ProfilesAndNewsGenerator)
3)  Furthermore being processed by [CAVS](https://github.com/caltr98/CAVS) to extract skills from both and match them. 


# Base Simulation run
1) Setup main.c parameters:
```cpp
  // Parameters for Model initialization
  int num_users = 100;          // Number of users in the simulation
  int num_checkers = 70;        // Number of checkers in the simulation
  double prob_susceptible = 0.7; // Probability of a user being susceptible to influence
  double prob_recover = 0.2;     // Probability of a user recovering or becoming immune to influence
  double prob_spreader = 0.3;    // Probability of a user spreading influence to others
  int epochs = 1000;             // Number of epochs (iterations) for the simulation
  // Create an instance of Model with the specified parameters
  Model model(num_users, num_checkers, prob_susceptible, prob_recover, prob_spreader, epochs);
  model.execute_trials(10); // Select the number of trial on which to average the results
  ```
1) Compile the C++ code
2) Execute the main

# Model Simulation run
1) Setup main.c parameters:
```cpp
  // Parameters for Model initialization
  
  int num_users = 100;          // Number of users in the simulation
  int num_checkers = 70;        // Number of checkers in the simulation
  double prob_susceptible = 0.7; // Probability of a user being susceptible to influence
  double prob_recover = 0.2;     // Probability of a user recovering or becoming immune to influence
  double prob_spreader = 0.3;    // Probability of a user spreading influence to others
  int epochs = 1000;             // Number of epochs (iterations) for the simulation
  // Create an instance of Model with the specified parameters, including the AI tools used and the min number of skill mathces
  // to propagate a news (below is 1)
  ModelDataset model(num_users, num_checkers, prob_susceptible, prob_recover, prob_spreader, epochs,"BERT+OJDAPP",    1);
  model.execute_trials(10); // Select the number of trial on which to average the results
  ```
1) Compile the C++ code
2) Execute the main

# Dataset
The used dataset is cmake-build-debug/file.json. It is a JSON array with entry of type
```json
{
  "type": "object",
  "properties": {
    "name": {
      "type": "string"
    },
    "surname": {
      "type": "string"
    },
    "gender": {
      "type": "string"
    },
    "bio": {
      "type": "string"
    },
    "articles": {
      "type": "array",
      "items": {
        "type": "object",
        "properties": {
          "title": {
            "type": "string"
          },
          "content": {
            "type": "string"
          },
          "news-status": {
            "type": "string",
            "enum": ["true-news", "false-news"]
          },
          "url": {
            "type": "string",
            "format": "uri"
          },
          "article_id": {
            "type": "integer"
          },
          "skills": {
            "type": "array",
            "items": {
              "type": "array",
              "items": [
                {
                  "type": "string"
                },
                {
                  "type": "string",
                  "format": "uri"
                }
              ]
            }
          },
          "matches_skills": {
            "type": "integer"
          }
        },
        "required": [
          "title",
          "content",
          "news-status",
          "url",
          "article_id",
          "skills",
          "matches_skills"
        ]
      }
    },
    "skills": {
      "type": "array",
      "items": {
        "type": "array",
        "items": [
          {
            "type": "string"
          },
          {
            "type": "string",
            "format": "uri"
          }
        ]
      }
    },
    "author_id": {
      "type": "integer"
    }
  },
  "required": [
    "name",
    "surname",
    "gender",
    "bio",
    "articles",
    "skills",
    "author_id"
  ]
}
```
In our case, the articles array contains 3 true news article and 2 fake news articles.
The skills array along with matches_skill (the count) is provided by [CAVS](https://github.com/caltr98/CAVS).

