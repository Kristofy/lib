#include <vector>
#include "math_types.hpp"

struct TEST{
    TEST operator+(const TEST& t) {return TEST{};}
    TEST operator-(const TEST& t) {return TEST{};}
    TEST operator/(const TEST& t) {return TEST{};}
    TEST operator*(const TEST& t) {return TEST{};}

    static const bool CommutativeOverAddition = true;
    static const bool CommutativeOverMultiplication = true;
    static const bool AssociativeOverAddition = true;
    static const bool AssociativeOverMultiplication = true;
    static const bool Distributive = true;


};

template<>
TEST Types::Zero(){
    return TEST{};
}

template<>
TEST Types::One(){
    return TEST{};
}


int main()
{
    Assert::AdditiveIdenityExists<TEST>();
    Assert::AdditiveIdenityExists<int>();
    Assert::CommutativeRing<TEST>();
    std::vector<int> k;
}