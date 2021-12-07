#include <vector>
#include <list>
#include <iostream>
#include <cstdint>
#include <thread>
#include <future>

struct Fish
{
    int daySpawned;
    int reproduceCounter;

    Fish() :
        daySpawned( 1 ),
        reproduceCounter( 0 )
    {}

    Fish( const Fish& other ) :
        daySpawned( other.daySpawned ),
        reproduceCounter( other.reproduceCounter )
    {
    }

    Fish( Fish& other ) :
        daySpawned( other.daySpawned ),
        reproduceCounter( other.reproduceCounter )
    {
    }

    Fish( int daySpawned, int reproduceCounter ) :
        daySpawned( daySpawned ),
        reproduceCounter( reproduceCounter )
    {
    }

    void operator=( const Fish& rhs )
    {
        daySpawned = rhs.daySpawned;
        reproduceCounter = rhs.reproduceCounter;
    }

    void operator=( Fish& rhs )
    {
        daySpawned = rhs.daySpawned;
        reproduceCounter = rhs.reproduceCounter;
    }
};

/* Returns the number of new fish it spawns */
uint64_t simSingleFish( Fish fish, const int numDays )
{
    uint64_t numSpawned = 0;
    for ( int day = fish.daySpawned; day <= numDays; ++day )
    {
        --fish.reproduceCounter;
        if ( fish.reproduceCounter < 0 )
        {
            ++numSpawned;
            static const int reproduceDays = 7-1;
            static const int newReproduceDays = 9-1;
            fish.reproduceCounter = reproduceDays;
            Fish newFish( day+1, newReproduceDays );
            numSpawned += simSingleFish( newFish, numDays );
        }
    }
    return numSpawned;
}

uint64_t simNumFish(
    std::vector<Fish>& initial,
    size_t startIndex,
    size_t stopIndex, // 1 greater than end
    const int numDays )
{
    uint64_t numFish = 0;
    for ( size_t i = startIndex; i < stopIndex; ++i )
    {
        ++numFish;
        Fish f = initial[i];
        numFish += simSingleFish( f, numDays );
    }
    return numFish;
}

static uint64_t runAlgThreaded( std::vector<Fish>& input, const int numDays, const size_t numThreads )
{
    uint64_t numFish = 0;
    std::vector<std::future<uint64_t>> results;
    const size_t fishPerThread = input.size() / numThreads;
    for ( size_t i = 0; i < numThreads-1; ++i )
    {
        size_t startIndex = i*fishPerThread;
        size_t stopIndex = startIndex+fishPerThread;
        results.push_back( std::async( simNumFish,
            input,
            startIndex,
            stopIndex,
            numDays
        ));
    }
    results.push_back( std::async( simNumFish,
        input,
        (numThreads-1)*fishPerThread,
        input.size(),
        numDays
    ));

    for ( size_t i = 0; i < numThreads; ++i )
    {
        numFish += results[i].get();
    }

    return numFish;
}

bool testAlg()
{
    std::vector<int> initInput = { 3,4,3,1,2 };
    std::vector<Fish> initFishInput( initInput.size() );
    for ( size_t i = 0; i < initFishInput.size(); ++i )
    {
        initFishInput[i] = Fish( 1, initInput[i] );
    }
    uint64_t numFish = runAlgThreaded( initFishInput, 18, 4 );
    if ( numFish != 26 ) {
        std::cout << "First num fish test failed: " << numFish << std::endl;
        return false;
    }
    numFish = runAlgThreaded( initFishInput, 80, 4 );
    if ( numFish != 5934 ) {
        std::cout << "Second num fish test failed: " << numFish << std::endl;
        return false;
    }
    numFish = runAlgThreaded( initFishInput, 256, 4 );
    if ( numFish != 26984457539 ) {
        std::cout << "Third num fish test failed: " << numFish << std::endl;
        return false;
    }
    std::cout << "Test passed" << std::endl;
    return true;
}

std::vector<int> dayInput = {
    1,1,1,1,1,1,1,4,1,2,1,1,4,1,1,1,5,1,1,1,1,1,1,1,1,1,1,1,1,5,1,1,1,1,3,1,1,2,1,2,1,3,3,4,1,4,1,1,3,1,1,5,1,1,1,1,4,1,1,5,1,1,1,4,1,5,1,1,1,3,1,1,5,3,1,1,1,1,1,4,1,1,1,1,1,2,4,1,1,1,1,4,1,2,2,1,1,1,3,1,2,5,1,4,1,1,1,3,1,1,4,1,1,1,1,1,1,1,4,1,1,4,1,1,1,1,1,1,1,2,1,1,5,1,1,1,4,1,1,5,1,1,5,3,3,5,3,1,1,1,4,1,1,1,1,1,1,5,3,1,2,1,1,1,4,1,3,1,5,1,1,2,1,1,1,1,1,5,1,1,1,1,1,2,1,1,1,1,4,3,2,1,2,4,1,3,1,5,1,2,1,4,1,1,1,1,1,3,1,4,1,1,1,1,3,1,3,3,1,4,3,4,1,1,1,1,5,1,3,3,2,5,3,1,1,3,1,3,1,1,1,1,4,1,1,1,1,3,1,5,1,1,1,4,4,1,1,5,5,2,4,5,1,1,1,1,5,1,1,2,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,5,1,1,1,1,1,1,3,1,1,2,1,1
};
std::vector<Fish> dayFishInput( dayInput.size() );

int main()
{
    if ( !testAlg() ) {
        std::cout << "Test alg failed, returning" << std::endl;
        return 1;
    }
    for ( size_t i = 0; i < dayFishInput.size(); ++i )
    {
        dayFishInput[i] = Fish( 1, dayInput[i] );
    }
    uint64_t numFish = runAlgThreaded( dayFishInput, 256, 10 );
    std::cout << "Num fish: " << numFish << std::endl;
    return 0;
}

