#include <iostream>
#include <cstdint>
#include <vector>
#include <algorithm>

using EnergyTable = std::vector<std::vector<int>>;
struct TableCoord
{
    uint64_t row;
    uint64_t col;
    
    TableCoord() :
        row( 0 ),
        col( 0 )
    {
    }
    
    TableCoord( uint64_t row, uint64_t col ) :
        row( row ),
        col( col )
    {
    }
    
    TableCoord( const TableCoord& other )
    {
        row = other.row;
        col = other.col;
    }
    
    void operator=( TableCoord& other )
    {
        row = other.row;
        col = other.col;
    }
    
    bool operator==( const TableCoord& other )
    {
        return ( row == other.row &&
            col == other.col );
    }
};

static inline void flash( EnergyTable& table, TableCoord& coord )
{
    for ( int64_t rowOffset = -1; rowOffset <= 1; ++rowOffset )
    {
        for ( int64_t colOffset = -1; colOffset <= 1; ++colOffset )
        {
            /* Don't flash the input coordinate */
            if ( rowOffset == 0 && colOffset == 0 ) continue;
            
            int64_t testRow = coord.row + rowOffset;
            int64_t testCol = coord.col + colOffset;
            if ( testRow < table.size() && testRow >= 0 &&
                 testCol < table[0].size() && testCol >= 0 )
            {
                table[testRow][testCol]++;
            }
        }
    }
}

static uint64_t flashCount = 0;

static void runIter( EnergyTable& table )
{
    /* Energy level of each octopus increases by 1 */
    for ( std::vector<int>& row : table )
    {
        for ( int& energy : row )
        {
            ++energy;
        }
    }
    
    std::vector<TableCoord> alreadyFlashed;
    
    bool didFlash;
    do
    {
        didFlash = false;
        
        /* Each octopus with energy > 9 flashes, causing
         * energy of all adjacent to increase by 1 */
        for ( uint64_t row = 0; row < table.size(); ++row )
        {
            for ( uint64_t col = 0; col < table[0].size(); ++col )
            {
                TableCoord coord( row, col );
                /* If energy high enough and not already flashed, flash */
                if ( (table[row][col] > 9) &&
                     (std::find( alreadyFlashed.begin(),
                        alreadyFlashed.end(),
                        coord ) == alreadyFlashed.end()) )
                {
                    /* Flash the other octopuses */
                    flash( table, coord );
                    
                    /* Add to already flashed list */
                    alreadyFlashed.push_back( coord );
                    
                    /* Indicate at least one octopush flashed,
                     * meaning we need to check again if an energy increased
                     * above flash threshold */
                    didFlash = true;
                    
                    /* Increase the flash count */
                    ++flashCount;
                }
            }
        }
    } while ( didFlash );
    
    /* Set any flashed energy levels back to zero */
    for ( std::vector<int>& row : table )
    {
        for ( int& energy : row )
        {
            if ( energy > 9 ) {
                energy = 0;
            }
        }
    }
}

static void printEnergyTable( EnergyTable& table )
{
    for ( std::vector<int>& row : table )
    {
        for ( int val : row )
        {
            std::cout << val;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

static bool testAlg()
{
    EnergyTable energyLevels = {
        {5,4,8,3,1,4,3,2,2,3},
        {2,7,4,5,8,5,4,7,1,1},
        {5,2,6,4,5,5,6,1,7,3},
        {6,1,4,1,3,3,6,1,4,6},
        {6,3,5,7,3,8,5,4,7,8},
        {4,1,6,7,5,2,4,6,4,5},
        {2,1,7,6,8,4,1,7,2,1},
        {6,8,8,2,8,8,1,1,3,4},
        {4,8,4,6,8,4,8,5,5,4},
        {5,2,8,3,7,5,1,5,2,6}
    };
    
    flashCount = 0;
    for ( size_t i = 0; i < 100; ++ i )
    {
        runIter( energyLevels );
    }
    if ( flashCount != 1656 ) {
        std::cout << "Flash count failed: " << flashCount << std::endl;
        return false;
    }
    
    return true;
}

EnergyTable input = {
{8,8,2,6,8,7,6,7,1,4},
{3,1,2,7,7,8,7,2,3,8},
{8,1,8,2,8,5,2,8,6,1},
{4,6,5,5,3,7,1,4,8,3},
{3,8,6,4,5,5,1,3,6,5},
{1,8,7,8,2,5,3,5,8,1},
{8,3,1,7,4,2,2,4,3,7},
{1,5,1,7,2,5,4,2,6,6},
{2,6,2,1,1,2,4,7,6,1},
{3,4,7,3,3,3,1,5,1,4}
};

int main()
{
    if ( !testAlg() ) {
        return 1;
    }
    flashCount = 0;
    for ( size_t i = 0; i < 100; ++ i )
    {
        runIter( input );
    }
    std::cout << "Flash Count: " << flashCount << std::endl;
    return 0;
}