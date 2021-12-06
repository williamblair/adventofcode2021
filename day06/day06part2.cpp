#include <vector>
#include <cstdint>
#include <iostream>

size_t simNumFish( std::vector<int8_t>& initial, const int numDays )
{
    const int reproduceDays = 7-1;
    int newReproduceDays = 9-1;
    std::vector<int8_t> fishVec( initial );
    for ( int day = 1; day <= numDays; ++day )
    {
        size_t beginFishSize = fishVec.size();
        for ( size_t i = 0; i < beginFishSize; ++i )
        {
            fishVec[i]--;
            if ( fishVec[i] < 0 ) {
                fishVec[i] = reproduceDays;
                fishVec.push_back( newReproduceDays );
            }
        }
    }

    return fishVec.size();
}

bool testAlg()
{
    std::vector<int8_t> initFishInput = { 3,4,3,1,2 };
    size_t numFish = simNumFish( initFishInput, 18 );
    if ( numFish != 26 ) {
        std::cout << "First num fish test failed: " << numFish << std::endl;
        return false;
    }
    numFish = simNumFish( initFishInput, 256 );
    if ( numFish != 26984457539 ) {
        std::cout << "Second num fish test failed: " << numFish << std::endl;
        return false;
    }
    std::cout << "Test passed" << std::endl;
    return true;
}

std::vector<int8_t> dayInput = {
    1,1,1,1,1,1,1,4,1,2,1,1,4,1,1,1,5,1,1,1,1,1,1,1,1,1,1,1,1,5,1,1,1,1,3,1,1,2,1,2,1,3,3,4,1,4,1,1,3,1,1,5,1,1,1,1,4,1,1,5,1,1,1,4,1,5,1,1,1,3,1,1,5,3,1,1,1,1,1,4,1,1,1,1,1,2,4,1,1,1,1,4,1,2,2,1,1,1,3,1,2,5,1,4,1,1,1,3,1,1,4,1,1,1,1,1,1,1,4,1,1,4,1,1,1,1,1,1,1,2,1,1,5,1,1,1,4,1,1,5,1,1,5,3,3,5,3,1,1,1,4,1,1,1,1,1,1,5,3,1,2,1,1,1,4,1,3,1,5,1,1,2,1,1,1,1,1,5,1,1,1,1,1,2,1,1,1,1,4,3,2,1,2,4,1,3,1,5,1,2,1,4,1,1,1,1,1,3,1,4,1,1,1,1,3,1,3,3,1,4,3,4,1,1,1,1,5,1,3,3,2,5,3,1,1,3,1,3,1,1,1,1,4,1,1,1,1,3,1,5,1,1,1,4,4,1,1,5,5,2,4,5,1,1,1,1,5,1,1,2,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,5,1,1,1,1,1,1,3,1,1,2,1,1
};

int main()
{
    if ( !testAlg() ) {
        return 1;
    }
    size_t numFish = simNumFish( dayInput, 80 );
    std::cout << "Num fish: " << numFish << std::endl;
    return 0;
}

