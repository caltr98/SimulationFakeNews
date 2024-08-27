#ifndef CHECKER_H
#define CHECKER_H

#include "User.h"

class Checker : public User {
public:
    Checker(int id, int* NumberOfPosts);
    bool becomeInfectious() override;
    bool becomeSusceptible() override;
    bool resharePost() override;
};

#endif // CHECKER_H
