# Fake News Spread Simulation using Skill-Based Verifiable Credentials (CAVS) - C++ Implementation

This repository contains a C++ simulation to evaluate the impact of a skill-based verifiable credentials ([CAVS](https://github.com/caltr98/CAVS)) system on the spread of fake news in Online Social Networks (OSNs). The simulation compares two different scenarios: 

1. **Ground-Truth (GT)**: where checkers can always determine whether a post is true or false.
2. **Skill-Based (SK)**: where checkers use a skill match between the author and the post to decide whether to share it or not.

The simulation employs a Monte Carlo method with a **Susceptible-Infected-Recovered (SIR)** model to simulate user behavior and information diffusion in a social network.

## How to Run the Simulation

### Prerequisites

- A C++ compiler (e.g., `g++`).

### Steps

1. **Clone the repository**:
   ```bash
   git clone https://github.com/caltr98/CAVS.git
   cd CAVS

## Modify Parameters

To change the simulation parameters, you can modify the variables in the `main()` function of `main.cpp`. This includes:

- `num_users`: Number of users in the simulation.
- `num_checkers`: Number of checkers in the simulation.
- `epochs`: Number of simulation epochs.
- `monte_carlo_runs`: Number of Monte Carlo runs to perform.

## Build the Project

Compile the code with:

```bash
g++ -o simulation main.cpp -fopenmp
```

## Run the Simulation

The simulation will execute the **Skill-Based (SK)** simulation by default. You can switch to the **Ground-Truth (GT)** simulation by uncommenting the corresponding function call in `main.cpp`.
```bash
./simulation
```

## Simulation Parameters

The simulation uses the following parameters for the Monte Carlo simulation:

- `num_users`: Total number of users in the network (default: 100).
- `num_checkers`: Number of checkers in the network (default: 60).
- `epochs`: Number of simulation epochs (default: 100).
- `monte_carlo_runs`: Number of Monte Carlo simulations (default: 1000).
- `seed`: Seed for random number generation (default: 42).

## Simulation Logic

### Ground-Truth Simulation (GT):

In this scenario, checkers know the truth about the posts and share only verified, true posts.

### Skill-Based Simulation (SK):

In this scenario, checkers evaluate the author’s skills and match them to the content of the post. If the match is below a predefined threshold, they do not share the post.

The **SIR model** simulates the following user states:

- **Susceptible (S)**: Users who might create or reshare fake posts.
- **Infected (I)**: Users who only reshare fake posts.
- **Recovered (R)**: Users who only share true posts.


## Dataset

The synthetic dataset used in these simulations includes user profiles, skill labels, and true/false news posts. The dataset is available on [Kaggle](https://www.kaggle.com/datasets/calogeroturco/synthetic-articles-with-authors-having-esco-skills).
