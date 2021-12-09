#include <iostream>
#include <cstdint>
#include <vector>

using HeightMap = std::vector<std::vector<int>>;

struct MapPoint
{
  int row;
  int col;
  int val;

  MapPoint() :
    row( 0 ),
    col( 0 ),
    val( 0 )
  {}

  MapPoint( int row, int col, int val ) :
    row( row ),
    col( col ),
    val( val )
  {}
};
static void findLowPoints( HeightMap& map, std::vector<MapPoint>& res )
{
  res.clear();
  for ( int64_t row = 0; row < (int64_t)map.size(); ++row )
  {
    for ( int64_t col = 0; col < (int64_t)map[0].size(); ++col )
    {
      bool allAdjacentsHigher = true;
      for ( int64_t rowOffset = -1; rowOffset <= 1 && allAdjacentsHigher; ++rowOffset )
      {
        for ( int64_t colOffset = -1; colOffset <= 1 && allAdjacentsHigher; ++colOffset )
        {
          /* Don't test the value against itself */
          if ( (rowOffset == 0 && colOffset == 0) )
          {
            continue;
          }
          /* Only test up,down,left,and right
           * (either row offset or col offset should be 0) */
          if ( rowOffset != 0 && colOffset != 0 )
          {
            continue;
          }
          
          int64_t testRow = row + rowOffset;
          int64_t testCol = col + colOffset;
          if ( testRow >= 0 && testRow < (int64_t)map.size() &&
               testCol >= 0 && testCol < (int64_t)map[0].size() )
          {
            allAdjacentsHigher &= ( map[testRow][testCol] > map[row][col] );
          }
        }
      }

      if ( allAdjacentsHigher ) {
        res.push_back(MapPoint( row, col, map[row][col] ));
      }
    }
  }
}

uint64_t calcDangerLevelsSum( std::vector<MapPoint>& lowPoints )
{
  uint64_t sum = 0;
  for ( MapPoint& lp : lowPoints )
  {
    sum += lp.val + 1;
  }
  return sum;
}

static bool testAlg()
{
  HeightMap testMap = {
    {2,1,9,9,9,4,3,2,1,0},
    {3,9,8,7,8,9,4,9,2,1},
    {9,8,5,6,7,8,9,8,9,2},
    {8,7,6,7,8,9,6,7,8,9},
    {9,8,9,9,9,6,5,6,7,8}
  };
  uint64_t numRows = testMap.size();
  uint64_t numCols = testMap[0].size();
  std::vector<MapPoint> lowPoints;
  findLowPoints( testMap, lowPoints );
  uint64_t dangerLevel = calcDangerLevelsSum( lowPoints );
  if ( dangerLevel != 15 ) {
    std::cout << "Test failed: " << dangerLevel << std::endl;
    return false;
  }
  return true;
}

HeightMap input = {
{5,7,9,6,7,9,8,6,2,1,2,3,7,9,9,5,4,9,8,7,6,5,4,3,4,5,6,7,9,8,7,5,4,2,9,9,9,7,6,5,6,7,9,9,8,7,5,4,5,6,7,9,1,0,9,8,7,8,9,9,9,8,7,7,8,9,9,7,8,9,8,7,6,5,3,2,1,2,3,4,5,6,9,9,8,9,9,9,8,7,6,8,8,7,8,9,9,9,2,1},
{4,6,4,5,9,7,6,4,3,4,4,5,6,7,8,9,3,4,9,6,5,4,3,2,1,2,9,8,9,9,7,6,7,9,8,9,8,6,5,4,6,9,8,9,8,7,6,3,5,6,7,8,9,9,8,7,6,7,8,9,7,6,5,6,7,8,9,6,9,8,7,6,5,4,3,2,0,1,2,3,4,7,8,9,7,8,9,9,8,6,5,6,7,6,7,9,8,7,9,9},
{3,2,3,4,9,8,7,5,4,5,9,7,8,9,9,3,2,9,8,7,9,5,4,1,0,9,8,9,9,9,8,9,8,9,7,8,9,5,4,3,2,5,6,8,9,7,6,4,6,7,8,9,4,9,8,7,5,6,7,8,9,5,4,6,8,9,6,5,7,9,8,7,7,8,4,2,1,2,3,4,5,6,9,7,6,7,8,9,9,5,4,3,4,5,9,8,5,6,7,8},
{4,3,5,6,7,9,8,6,7,9,9,8,9,9,9,9,0,1,9,9,8,7,3,2,9,8,7,8,9,9,9,7,9,8,6,7,9,7,6,5,3,4,5,6,8,9,8,5,6,9,9,1,2,9,6,5,4,5,8,9,0,1,3,4,7,8,9,4,6,7,9,9,8,9,5,6,8,9,9,9,6,7,8,9,5,6,9,8,6,4,3,2,3,9,8,7,4,5,7,9},
{6,4,6,7,9,9,9,7,8,9,9,9,9,8,9,8,9,3,4,9,7,6,5,9,8,9,6,7,9,8,7,6,7,6,5,6,8,9,9,6,4,5,7,9,9,9,9,6,7,9,9,9,9,8,7,6,3,6,7,8,9,3,4,5,6,9,1,3,4,6,7,8,9,7,6,9,9,8,7,8,9,8,9,3,4,9,8,7,6,5,1,0,9,8,7,6,3,4,5,6},
{7,5,9,8,9,9,7,9,9,9,9,8,7,7,9,6,8,9,9,9,8,9,9,8,7,9,5,9,8,7,6,5,4,3,4,5,6,7,8,9,5,6,9,8,9,8,9,9,8,9,9,8,7,6,5,1,2,5,6,7,8,9,5,6,9,1,0,2,9,8,8,9,9,8,9,8,7,7,6,7,9,9,1,2,3,9,9,8,8,6,2,9,8,7,6,5,2,3,4,5},
{8,7,8,9,8,7,6,7,8,9,8,6,5,6,8,5,5,7,8,9,9,9,8,7,6,6,4,5,9,8,7,6,6,5,5,6,7,9,9,8,6,8,9,6,5,6,7,8,9,8,7,6,5,5,4,0,3,4,8,9,9,9,9,9,8,9,1,9,8,9,9,9,8,9,8,7,5,6,4,6,7,9,0,9,9,7,8,9,8,7,9,8,7,6,5,4,3,4,5,6},
{9,8,9,8,7,6,5,4,9,8,9,7,4,3,2,4,4,5,6,7,9,9,8,6,5,5,3,4,5,9,8,7,8,6,7,8,8,9,9,8,7,9,8,6,4,5,8,8,9,9,8,5,4,3,2,1,2,6,9,5,4,9,8,7,7,8,9,8,7,8,9,8,7,6,5,4,3,2,3,5,6,7,9,8,8,6,9,8,9,8,9,9,8,7,6,6,4,6,7,8},
{9,9,8,9,9,9,7,9,8,7,5,4,3,2,1,2,3,6,7,9,8,7,6,5,4,3,2,3,4,5,9,8,9,7,8,9,9,5,4,9,8,9,5,4,3,4,5,6,9,8,7,6,5,5,6,4,3,7,8,9,9,8,7,6,5,9,9,9,6,7,9,9,6,5,4,3,2,1,2,4,5,9,8,7,6,5,8,7,8,9,8,9,9,8,7,9,8,7,8,9},
{9,8,7,7,9,8,9,9,9,7,6,6,2,1,0,1,4,5,6,7,9,8,7,6,5,4,3,4,5,6,7,9,9,8,9,6,7,9,3,2,9,8,7,6,2,4,7,7,8,9,8,9,7,8,7,5,4,8,9,9,9,9,8,9,6,7,9,4,5,9,9,8,7,6,8,7,3,0,1,3,4,9,8,7,5,4,5,6,9,6,7,8,9,9,8,9,9,8,9,3},
{8,7,6,5,6,7,8,9,9,8,9,8,3,2,3,2,3,6,7,8,9,9,8,9,6,7,6,5,7,8,8,9,3,9,3,4,6,8,9,1,2,9,5,4,3,4,9,9,9,9,9,9,8,9,9,6,5,9,8,7,9,6,9,9,8,9,4,3,4,9,8,9,9,7,9,6,5,3,2,6,5,6,9,5,4,3,4,4,7,5,6,9,1,0,9,2,3,9,6,4},
{9,8,7,4,7,8,9,0,1,9,8,9,4,3,4,3,4,5,6,7,8,9,9,4,9,8,7,8,9,9,9,3,2,1,2,3,5,7,9,9,9,8,7,5,4,5,6,7,8,9,4,5,9,9,8,7,7,8,9,6,4,5,7,8,9,4,2,1,3,9,7,6,9,8,9,8,5,4,3,7,6,9,8,4,3,2,1,2,3,4,5,8,9,1,9,4,9,7,6,5},
{7,9,6,5,6,7,8,9,9,8,7,6,5,7,6,4,5,6,7,8,9,3,4,3,3,9,8,9,8,7,6,5,4,2,3,4,6,9,8,7,8,9,8,6,5,6,7,8,9,2,3,6,7,9,9,8,8,9,6,5,3,9,8,9,9,4,3,2,9,8,6,5,2,9,9,9,6,7,8,9,8,9,7,5,4,5,2,3,4,5,6,7,8,9,8,9,8,9,7,6},
{6,5,9,8,7,8,9,1,0,9,9,7,6,9,7,5,6,9,8,9,2,1,0,1,2,3,9,8,9,9,9,7,6,3,4,5,7,8,9,6,5,5,9,8,6,7,9,9,7,6,4,7,9,8,9,9,9,8,7,9,9,8,9,9,8,7,4,5,6,9,5,4,1,0,9,8,7,9,9,7,9,8,7,6,5,6,7,6,5,7,9,8,9,6,6,8,7,8,9,7},
{5,4,3,9,8,9,8,9,2,9,9,8,9,9,8,9,7,8,9,9,6,2,1,2,3,4,5,7,8,9,9,8,9,4,5,6,8,9,8,9,4,3,5,9,9,8,9,9,8,7,9,9,8,7,8,9,9,9,9,8,9,7,9,8,7,6,5,7,9,8,4,3,2,1,4,9,8,9,8,6,4,9,9,8,7,7,8,7,8,9,9,9,6,5,4,5,6,9,8,9},
{5,2,1,2,9,8,7,8,9,8,9,9,9,9,9,9,8,9,9,8,5,4,3,3,6,5,6,8,9,9,9,9,8,9,8,7,9,9,7,9,3,2,6,5,6,9,1,0,9,9,8,9,5,6,7,8,9,8,8,7,9,6,8,9,8,7,8,9,8,7,5,4,4,2,3,4,9,9,5,4,3,4,5,9,9,8,9,8,9,9,9,8,7,4,3,4,5,6,7,9},
{4,3,0,9,8,7,5,9,8,7,9,9,9,8,7,6,9,9,8,7,6,5,6,8,7,6,7,9,9,8,9,8,7,8,9,9,8,7,6,5,4,1,2,4,5,8,9,2,9,8,7,5,4,6,7,9,8,7,8,5,4,5,9,9,9,8,9,6,9,8,9,5,5,6,7,9,9,8,9,3,2,3,4,5,9,9,7,9,8,9,9,9,9,3,2,1,4,5,8,9},
{5,9,9,6,9,5,4,6,7,6,7,9,9,9,8,4,3,2,9,8,7,7,7,8,8,9,8,9,8,7,8,9,6,9,8,9,9,8,7,4,3,2,4,5,6,7,8,9,9,7,6,5,3,5,9,8,7,6,7,4,3,4,8,9,9,9,6,5,4,9,8,7,7,7,8,9,8,7,8,9,1,2,3,9,8,7,6,7,7,8,9,9,8,4,3,2,3,4,5,6},
{9,8,7,5,4,3,2,5,4,5,7,8,9,7,4,3,2,1,2,9,8,9,8,9,9,9,9,8,9,6,7,6,5,9,7,8,9,9,8,5,5,9,5,6,7,8,9,9,8,8,6,8,2,4,9,9,6,5,3,1,2,6,7,9,8,6,5,4,3,5,9,9,8,8,9,8,7,6,7,8,9,9,5,9,8,9,5,4,6,9,8,7,6,5,4,6,7,5,6,7},
{2,9,9,4,3,2,1,0,3,6,8,9,7,6,5,4,1,0,1,9,9,6,9,3,6,9,8,7,6,5,4,5,4,3,6,7,9,9,9,9,9,8,9,7,8,9,9,9,7,8,5,4,1,9,8,7,6,4,3,0,1,5,6,9,9,7,9,4,2,3,4,5,9,9,9,9,9,5,4,6,7,8,9,9,7,6,4,3,7,8,9,9,7,8,5,7,8,9,7,8},
{0,9,8,7,4,3,2,1,2,8,9,9,8,7,6,3,2,1,9,8,7,5,4,2,4,6,9,8,7,6,3,2,1,2,4,7,8,9,9,8,9,7,8,9,9,9,8,7,6,5,4,3,2,9,8,7,7,3,2,1,2,4,5,8,9,9,8,9,0,1,2,6,7,8,9,8,7,6,7,8,8,9,5,9,8,7,3,2,3,4,5,9,8,8,7,8,9,9,8,9},
{9,8,7,6,5,4,4,3,4,6,7,8,9,9,7,9,3,2,9,8,7,6,5,1,2,3,4,9,9,9,8,4,0,1,5,6,9,8,9,7,5,6,9,2,3,4,9,8,7,9,9,4,9,8,7,6,5,4,3,4,3,4,5,7,8,9,7,8,9,2,4,7,8,9,9,9,8,7,8,9,9,3,3,4,9,8,9,1,2,3,4,8,9,9,9,9,8,6,9,5},
{9,9,8,9,8,7,5,6,7,9,8,9,9,8,9,8,9,3,9,9,8,4,7,0,1,4,5,9,8,8,6,2,1,2,3,9,8,7,8,6,4,5,6,9,4,6,7,9,9,8,8,9,9,9,8,7,9,5,4,6,4,5,6,7,8,9,6,7,8,9,9,8,9,8,9,6,9,9,9,3,2,1,2,3,4,9,8,9,3,4,5,6,7,8,9,8,7,5,4,4},
{9,9,9,7,9,8,7,8,8,9,9,9,8,7,9,7,9,9,8,7,4,3,2,1,2,5,9,8,7,6,5,3,2,3,9,8,7,6,5,4,3,9,7,8,9,9,9,8,9,7,7,8,9,9,9,9,8,9,9,7,8,6,7,9,9,4,5,6,9,8,9,9,9,7,6,5,6,7,8,9,3,0,3,4,9,8,7,8,9,5,6,9,9,9,8,9,8,4,3,3},
{8,9,8,6,7,9,8,9,9,9,9,8,7,6,7,6,7,8,9,6,5,5,3,2,9,6,7,9,8,7,6,4,3,9,8,7,6,5,4,1,2,8,9,9,9,8,8,7,8,6,5,6,8,9,9,8,6,7,8,9,8,7,9,1,2,3,9,9,5,6,7,9,9,9,5,4,3,4,5,9,5,9,4,9,9,8,6,7,8,9,7,8,9,9,7,6,5,3,2,1},
{7,8,4,5,6,7,9,2,1,9,8,7,8,5,6,5,6,7,8,9,7,6,5,9,8,9,9,9,9,8,7,5,5,6,9,9,8,7,3,2,3,7,8,9,9,7,6,6,7,5,4,5,7,9,8,6,5,6,7,8,9,8,9,2,3,9,8,7,4,5,9,9,9,7,6,5,2,3,7,8,9,8,9,8,7,6,5,6,7,8,9,9,9,8,6,5,4,3,1,0},
{6,4,3,4,7,9,8,9,9,8,7,6,5,4,5,4,5,6,7,8,9,7,9,8,7,8,9,8,7,9,8,6,7,7,8,9,9,5,4,3,4,6,7,8,9,6,5,5,3,2,3,4,9,8,7,5,4,5,8,7,8,9,8,9,4,9,9,6,3,6,8,7,8,9,9,2,1,5,6,9,8,7,9,7,5,3,4,5,8,9,9,2,1,9,9,6,4,3,2,4},
{0,1,2,3,9,8,7,9,9,9,8,7,4,3,4,3,4,5,6,7,8,9,8,7,6,7,9,9,5,4,9,7,8,9,9,8,7,6,5,4,5,9,8,9,9,8,4,2,1,0,5,6,9,8,9,4,3,4,7,6,7,8,7,9,9,7,6,5,2,1,7,6,7,9,8,9,2,4,9,8,7,6,5,4,3,2,3,4,9,9,8,9,0,9,8,7,8,4,3,4},
{4,3,3,9,8,7,6,7,9,8,9,5,5,1,0,2,3,6,7,8,9,8,9,8,5,6,7,8,9,3,9,8,9,1,0,9,9,9,6,7,8,9,9,8,7,6,5,4,2,1,5,9,8,7,5,3,2,3,8,5,4,5,6,8,9,6,5,4,1,0,1,5,6,9,7,8,9,5,6,9,9,7,6,8,4,4,5,6,7,9,7,8,9,9,9,8,9,5,4,5},
{5,4,9,8,7,6,5,9,8,6,5,4,3,2,3,4,4,8,8,9,8,7,8,9,4,7,9,9,0,1,2,9,9,9,1,2,9,8,7,8,9,9,9,9,8,7,6,5,3,2,4,9,9,8,6,2,1,0,1,2,3,4,6,7,8,9,4,3,2,1,2,4,9,7,6,9,7,6,7,8,9,8,7,9,5,8,6,7,9,5,6,7,8,9,1,9,9,6,5,6},
{6,5,9,9,7,5,4,3,9,7,6,5,4,5,4,5,5,8,9,9,9,6,7,8,9,8,9,9,9,9,3,9,9,8,9,4,9,9,9,9,9,9,8,7,9,9,7,6,7,9,9,8,9,7,5,3,5,1,2,3,7,9,9,8,9,9,5,4,3,2,3,9,8,7,5,8,9,9,8,9,8,9,9,8,6,7,8,8,9,4,6,9,9,9,0,9,8,7,8,9},
{7,9,8,8,7,3,2,1,9,8,7,6,5,6,5,7,6,7,8,9,7,5,6,9,8,9,9,9,8,7,9,8,7,6,8,9,8,9,4,6,9,9,9,5,4,4,9,7,9,8,7,7,9,8,9,8,7,6,4,5,6,7,8,9,9,8,7,6,4,3,4,5,9,6,4,7,8,9,9,8,7,6,0,9,7,8,9,9,5,3,4,8,7,8,9,9,9,8,9,9},
{9,8,7,6,5,4,6,0,1,9,8,7,8,7,6,7,8,8,9,9,8,4,5,6,7,8,9,9,9,6,8,7,6,5,6,6,7,9,3,9,8,9,6,5,3,2,9,8,9,9,6,6,5,9,2,9,8,7,8,8,7,8,9,3,2,9,8,7,5,4,5,9,8,7,5,6,7,8,9,9,6,5,1,9,8,9,9,7,3,2,3,4,5,6,7,9,9,9,8,9},
{0,9,9,8,7,5,7,8,9,8,9,8,9,8,9,9,9,9,2,0,9,9,7,8,9,9,8,8,7,5,4,5,4,3,4,5,7,8,9,8,7,8,9,5,4,0,1,9,9,8,5,4,3,0,1,2,9,8,9,9,8,9,8,4,1,2,9,8,6,6,7,8,9,9,9,8,9,9,9,9,5,4,2,4,9,9,8,6,4,1,2,3,4,5,8,9,9,8,7,8},
{1,2,9,9,8,9,8,9,8,7,6,9,9,9,5,4,9,5,4,2,9,9,9,9,9,8,7,6,5,4,3,2,1,2,3,4,8,9,8,7,6,9,8,9,2,1,5,9,8,7,6,3,2,1,2,3,4,9,9,9,9,8,7,3,2,3,4,9,7,7,9,9,5,6,7,9,4,3,9,7,6,5,3,5,6,8,9,7,6,0,1,2,3,4,9,8,8,7,6,7},
{2,9,8,9,9,9,9,8,7,6,5,6,9,9,9,9,8,9,5,9,8,9,9,8,6,9,8,7,6,5,4,1,0,3,4,6,7,8,9,6,5,6,7,8,9,2,3,4,9,8,5,4,3,3,4,5,6,8,9,8,9,9,6,4,3,4,5,6,9,8,9,3,4,6,7,8,9,2,1,9,7,6,8,9,7,9,9,6,5,3,2,3,4,9,8,7,5,7,5,6},
{9,8,7,9,8,9,9,9,8,8,6,9,8,9,8,8,7,8,9,8,7,8,9,9,5,4,9,8,9,7,6,2,1,5,9,8,9,9,9,5,4,5,6,7,8,9,4,9,8,9,9,9,9,6,5,6,8,9,8,7,9,8,9,9,4,5,6,7,8,9,3,2,3,5,6,7,8,9,0,1,9,7,9,9,8,9,8,7,6,5,6,7,5,6,9,9,4,3,4,5},
{8,7,6,5,6,7,7,8,9,9,9,8,7,6,7,7,6,9,7,6,6,7,8,9,6,3,2,9,8,6,5,4,2,3,4,9,9,9,8,9,9,9,7,9,9,5,9,8,7,8,7,8,8,9,7,7,9,8,7,6,8,7,7,8,9,6,8,8,9,5,4,3,8,6,7,8,9,9,1,2,9,8,9,6,9,3,9,8,9,6,7,8,9,9,8,7,3,2,0,1},
{8,6,5,4,5,4,6,7,8,9,7,6,6,5,6,3,4,5,9,5,4,8,9,9,4,2,1,9,8,7,6,5,3,4,5,7,8,9,7,7,7,8,9,1,0,4,9,8,6,5,6,7,7,8,9,9,4,9,6,5,4,5,6,7,8,9,9,9,7,6,6,5,7,8,9,9,7,8,9,3,4,9,5,4,3,2,1,9,9,8,8,9,3,2,9,6,5,4,1,2},
{8,6,4,3,4,3,7,8,9,9,8,5,4,3,1,2,9,9,8,4,3,5,6,8,9,1,0,1,9,8,7,6,4,5,6,8,9,6,6,5,6,7,9,3,2,3,9,7,5,3,4,5,6,7,8,9,5,9,8,6,8,7,7,8,9,5,7,9,8,7,7,9,8,9,3,4,6,7,9,9,5,9,6,5,4,3,0,1,2,9,9,5,4,9,8,7,6,7,2,3},
{6,5,3,2,1,2,6,7,7,8,9,6,9,4,0,9,8,7,6,4,2,4,5,7,8,9,1,2,4,9,8,7,5,6,7,9,6,5,5,3,5,6,8,9,3,9,8,6,3,2,0,3,4,5,6,8,9,9,9,7,9,9,8,9,6,4,6,7,9,9,8,9,9,0,1,2,5,6,7,8,9,8,9,7,6,4,3,2,3,5,8,9,5,6,9,8,7,8,9,4},
{8,5,4,4,4,3,4,5,6,7,9,9,8,9,1,2,9,9,8,5,3,5,6,8,9,8,4,3,5,6,9,8,6,7,9,8,9,4,3,2,4,5,9,9,4,9,9,5,4,2,1,2,5,6,8,9,9,9,9,8,9,5,9,4,5,3,5,7,9,8,9,8,9,1,2,3,4,6,7,9,8,7,9,7,6,5,6,3,5,6,7,9,6,9,7,9,9,9,8,5},
{9,6,5,5,5,6,6,7,8,9,8,9,7,8,9,3,4,7,9,6,9,7,8,9,7,6,5,4,5,6,7,9,7,9,8,7,9,4,2,1,2,6,7,8,9,8,7,6,7,3,4,3,4,6,7,8,9,8,7,9,5,4,4,3,1,2,3,4,9,7,6,7,9,9,3,5,6,7,9,8,7,6,8,9,9,6,7,4,7,8,9,9,9,7,6,5,9,8,7,6},
{9,7,7,6,7,8,9,9,9,8,6,5,6,7,9,9,5,6,7,9,8,9,9,9,8,7,6,5,6,7,8,9,9,8,9,6,7,9,9,2,3,4,9,9,2,9,8,7,6,5,5,6,5,7,8,9,8,7,6,5,4,3,2,1,0,1,2,9,8,9,5,6,7,8,9,6,7,9,8,7,6,5,6,7,8,9,8,5,9,9,9,8,7,6,6,4,2,9,8,9},
{9,9,8,8,8,9,9,8,7,5,4,3,4,6,7,8,9,7,9,8,7,8,9,9,9,8,7,9,8,8,9,9,8,7,6,5,7,8,8,9,4,5,8,9,1,9,9,9,7,9,6,7,6,8,9,7,9,8,8,6,5,4,5,2,1,9,9,8,6,7,4,5,6,9,8,7,9,8,7,6,5,4,5,6,7,8,9,6,7,9,8,7,6,5,4,3,1,0,9,6},
{8,7,9,9,9,4,3,9,8,9,5,4,5,7,8,9,9,9,9,8,6,8,7,9,9,9,8,9,9,9,9,8,7,6,5,4,5,6,7,8,9,6,7,8,9,8,7,8,9,8,9,8,7,9,7,6,5,9,9,7,9,8,7,4,9,8,7,6,5,4,3,4,7,8,9,9,9,9,9,7,5,3,4,5,9,9,8,7,9,7,9,8,7,6,5,3,2,1,4,5},
{7,6,7,9,9,6,9,9,9,8,6,5,6,7,8,9,9,8,7,6,5,7,6,7,8,9,9,4,3,2,3,9,6,5,4,3,2,7,8,9,9,7,9,9,8,7,6,7,9,7,8,9,8,9,6,5,3,5,9,8,9,9,9,5,9,9,8,3,2,1,2,3,4,5,6,8,9,3,9,8,9,4,5,6,7,8,9,9,7,6,4,9,8,7,6,8,7,2,3,6},
{7,5,6,7,8,9,8,9,5,9,7,6,7,8,9,3,9,7,6,5,4,3,4,6,7,9,9,5,9,0,9,8,7,6,6,4,5,6,7,8,9,8,9,9,9,6,5,4,5,6,7,8,9,6,5,4,2,3,5,9,9,9,8,9,8,9,9,7,5,4,3,5,9,6,7,9,4,2,1,9,8,6,7,8,9,9,6,8,9,7,3,1,9,9,8,7,5,3,4,5},
{3,4,6,7,9,8,7,6,4,4,9,7,8,9,1,2,9,8,7,7,8,4,9,8,9,8,7,9,8,9,9,9,8,7,8,5,6,7,8,9,9,9,9,9,8,7,4,3,4,5,6,9,6,5,3,2,1,3,6,7,9,8,7,6,7,8,9,8,6,6,4,6,8,7,8,9,4,3,9,8,9,8,9,9,3,4,5,9,7,9,2,0,1,2,9,8,9,4,6,6},
{6,5,6,9,9,9,9,5,3,3,9,8,9,9,9,9,7,9,8,8,9,6,7,9,7,8,6,7,7,8,9,8,9,8,9,8,7,8,9,7,9,9,9,8,7,6,4,2,4,6,7,8,9,6,2,1,0,2,3,9,8,7,6,5,8,9,8,9,7,7,5,7,8,9,9,6,5,9,8,7,6,9,3,2,2,3,7,5,6,7,9,9,3,3,4,9,6,5,7,8},
{8,6,7,8,9,9,8,9,2,1,2,9,6,7,8,9,6,7,9,9,8,7,8,9,6,5,4,5,6,9,8,7,6,9,3,9,9,9,8,6,7,8,9,7,6,4,3,1,2,3,9,8,9,9,3,9,2,3,4,9,9,6,5,4,5,6,7,8,9,7,6,8,9,3,9,8,9,9,9,9,5,4,2,0,1,2,3,4,5,6,7,8,9,4,9,8,7,6,8,9},
{9,7,9,9,8,6,7,8,9,0,1,4,5,6,7,9,5,4,2,9,9,8,9,8,7,8,9,6,7,8,9,9,5,4,2,3,4,9,9,5,6,9,8,7,6,5,1,0,1,2,5,6,7,8,9,8,9,4,9,8,7,5,4,3,4,5,8,9,9,9,7,9,1,2,4,9,8,9,9,8,9,5,3,1,2,3,4,7,8,9,9,9,9,5,7,9,8,7,8,9},
{2,9,8,8,6,5,6,8,9,1,2,3,7,7,8,9,8,6,9,8,7,9,9,9,8,9,9,8,9,9,6,8,9,5,9,9,9,8,7,6,7,8,9,8,6,4,2,1,2,3,4,5,9,9,6,7,8,9,9,8,7,6,5,2,3,7,7,8,9,9,9,1,0,2,9,9,7,5,6,7,8,9,4,3,6,5,5,6,7,9,9,9,8,9,9,9,9,8,9,0},
{3,9,7,7,5,4,5,7,8,9,3,4,7,8,9,9,9,9,8,7,6,7,8,9,9,9,8,9,4,3,4,9,9,9,8,7,9,9,8,9,8,9,8,9,8,7,4,2,3,4,6,7,8,9,5,6,7,8,9,9,3,2,1,0,4,6,6,7,8,9,3,2,1,9,8,7,6,4,5,6,7,9,9,8,7,8,6,7,9,8,9,7,6,7,8,9,9,9,2,1},
{9,8,6,5,4,3,7,8,9,6,5,5,6,7,8,9,9,9,9,9,4,6,7,9,9,8,7,6,5,4,5,9,9,8,7,6,7,9,9,9,9,8,7,9,7,6,5,3,4,7,8,9,9,5,4,5,8,9,9,9,7,6,2,1,2,4,5,6,7,9,4,5,9,8,7,6,4,3,7,8,9,8,7,9,8,9,7,9,8,7,6,5,4,5,9,7,8,9,4,3},
{9,9,7,6,5,4,5,7,8,9,6,9,7,8,9,9,9,8,7,8,3,4,6,8,9,9,8,9,6,9,9,7,6,7,6,5,6,8,9,9,8,7,6,9,8,9,6,7,5,6,7,8,9,3,2,6,7,9,8,7,6,4,3,2,5,5,6,7,8,9,5,9,8,9,4,3,2,1,8,9,9,7,6,5,9,9,8,9,7,6,5,4,3,4,5,6,7,8,9,4},
{9,9,8,9,8,6,7,8,9,9,9,8,9,9,9,9,9,8,6,5,4,5,6,7,8,9,9,5,9,8,7,6,5,4,5,4,5,9,9,8,9,6,5,4,9,8,7,9,7,8,8,9,3,2,1,9,9,6,9,9,7,5,4,3,4,5,7,8,9,8,7,9,7,6,5,4,3,2,3,7,8,9,9,4,3,5,9,6,9,8,6,5,4,6,7,8,9,9,6,5},
{8,8,9,6,9,7,8,9,9,9,8,7,9,8,7,8,9,9,7,6,6,8,7,8,9,2,1,3,9,8,6,5,4,3,2,3,4,9,8,7,8,9,4,3,4,9,8,9,8,9,9,6,5,4,9,8,7,5,6,9,8,6,6,5,6,7,8,9,0,9,9,9,8,9,6,5,5,3,4,5,6,7,8,9,2,3,4,5,6,9,7,6,5,6,7,9,8,9,8,7},
{7,6,4,5,9,8,9,8,9,8,7,6,5,6,5,5,6,8,9,7,7,9,8,9,8,9,0,1,9,8,7,6,6,2,1,2,9,8,7,6,7,8,9,2,3,4,9,8,9,9,9,8,6,9,8,9,5,4,5,3,9,9,8,7,8,9,9,9,1,9,8,7,9,8,7,7,6,4,5,6,8,9,9,3,1,2,5,7,8,9,8,7,6,7,9,6,7,8,9,9},
{9,8,6,9,9,9,8,7,8,9,9,6,4,3,3,4,5,7,8,9,9,5,9,3,7,8,9,2,3,9,7,6,5,4,3,9,8,9,4,5,6,8,9,3,4,5,6,7,8,9,7,9,9,8,7,6,4,3,0,1,2,3,9,8,9,6,7,8,9,8,7,6,7,9,9,8,7,7,6,7,9,8,8,9,0,2,3,4,9,9,9,8,9,8,9,5,7,8,9,8},
{6,9,9,8,7,8,9,6,7,9,8,9,2,1,2,3,4,5,7,8,9,4,3,2,5,9,4,3,4,9,8,7,6,5,9,8,7,8,3,4,5,6,8,9,5,6,8,8,9,4,6,7,8,9,8,9,5,2,1,3,4,5,7,9,6,5,6,8,9,9,6,5,6,5,4,9,8,8,7,8,9,7,7,8,9,6,5,6,7,9,9,9,9,9,5,4,6,4,5,7},
{5,9,8,7,6,5,4,5,9,7,6,5,1,0,1,2,3,4,9,9,7,5,4,3,4,7,9,9,5,7,9,9,8,9,8,6,5,4,2,3,5,6,7,8,9,7,9,9,2,3,7,8,9,9,9,7,6,3,2,4,5,6,7,8,9,4,7,9,9,9,7,4,3,4,3,5,9,9,8,9,9,6,5,4,9,7,6,7,9,8,7,9,8,9,4,3,2,3,4,7},
{4,3,9,8,7,5,3,2,9,9,8,4,3,1,2,3,4,5,7,8,9,6,5,4,5,6,7,8,9,9,2,0,9,9,6,5,4,5,1,2,4,5,5,7,8,9,2,0,9,9,8,9,3,9,8,7,5,4,3,5,6,7,8,9,4,3,9,8,7,9,8,4,2,1,2,4,9,9,9,9,8,7,6,3,8,9,8,9,8,7,6,6,7,8,9,5,4,4,5,6},
{1,2,3,9,8,5,4,9,8,7,6,5,3,2,3,8,7,6,8,9,9,8,9,5,6,9,9,9,9,8,9,2,9,8,7,4,3,1,0,1,2,3,4,6,7,8,9,9,8,9,9,4,2,9,8,7,6,5,7,6,8,8,9,9,5,9,8,7,6,5,4,3,1,0,2,9,8,9,2,1,9,6,5,4,7,8,9,8,9,6,5,4,6,6,7,9,5,5,6,7},
{0,1,2,3,9,9,5,7,9,9,7,6,8,5,4,5,6,7,8,9,6,9,8,6,9,8,7,8,9,7,9,3,4,9,5,4,3,2,1,3,4,5,5,7,8,9,9,9,7,8,9,9,1,0,9,9,7,6,7,7,9,9,9,8,7,8,9,9,8,7,5,3,2,1,3,9,7,8,9,9,8,7,6,5,6,9,8,7,6,5,4,3,4,5,7,8,9,9,7,9},
{3,2,3,5,9,8,6,9,9,9,8,7,9,6,5,6,7,8,9,7,5,6,9,9,9,8,6,7,6,6,8,9,5,6,9,7,5,4,3,4,6,9,6,8,9,9,9,8,6,9,8,7,9,1,2,9,8,7,8,9,9,9,9,9,8,9,4,6,9,7,6,4,3,2,9,8,6,7,8,9,9,9,9,8,7,8,9,8,5,4,3,2,3,4,5,6,7,8,9,8},
{9,9,4,5,7,9,9,8,7,6,9,9,8,7,9,8,8,9,7,6,4,5,9,8,7,6,5,4,4,5,6,8,9,9,8,7,6,5,9,5,9,8,9,9,8,9,7,6,5,9,7,6,8,9,9,9,9,8,9,9,9,8,8,6,9,8,9,9,8,7,6,5,9,9,8,7,5,6,7,9,9,8,9,9,8,9,9,9,8,6,4,3,6,5,7,8,8,9,7,7},
{8,7,9,9,8,9,5,9,9,5,4,3,9,8,9,9,9,8,6,5,3,4,5,9,8,7,3,2,3,6,7,9,6,8,9,8,8,9,8,9,8,7,8,9,7,9,9,5,4,6,9,5,6,7,8,9,1,9,9,9,8,7,8,5,4,6,8,9,9,8,7,9,8,7,6,6,4,5,6,9,9,7,8,9,9,8,9,8,7,6,5,4,7,9,8,9,9,7,6,6},
{5,6,7,8,9,2,3,9,9,4,3,2,3,9,9,9,8,7,6,5,4,5,9,8,7,5,4,3,4,7,8,9,5,7,9,9,9,9,7,6,5,5,6,7,6,7,8,9,3,2,5,4,5,8,9,1,0,1,9,8,7,6,7,4,3,7,8,7,8,9,8,9,9,8,5,4,3,4,9,8,7,6,7,9,9,6,4,9,8,7,6,6,7,8,9,8,7,6,4,5},
{4,5,6,7,9,1,9,8,7,6,2,1,4,8,9,9,9,8,7,6,8,9,9,9,8,9,5,4,9,8,9,5,4,6,8,9,8,7,6,5,4,3,2,3,5,6,7,8,9,1,2,3,6,7,8,9,1,9,9,8,6,5,4,3,2,4,5,6,7,9,9,9,8,7,6,5,4,6,7,9,6,5,6,7,8,9,9,9,9,8,7,7,8,9,2,9,8,7,5,6},
{3,4,6,7,8,9,8,7,6,5,4,3,6,7,8,9,9,9,8,9,9,8,9,9,9,8,7,9,9,9,5,4,3,4,9,9,9,8,9,6,5,2,1,2,3,7,8,9,5,0,1,9,7,8,9,9,9,8,9,9,7,5,3,2,1,3,5,9,9,8,8,9,9,9,7,7,5,6,9,8,7,6,7,8,9,9,8,8,7,9,8,9,9,2,1,0,9,8,6,7},
{2,3,6,9,9,2,9,8,7,6,5,4,5,6,9,9,9,9,9,9,8,7,8,9,9,9,9,9,8,7,6,5,4,5,6,8,9,9,4,3,2,1,0,2,3,5,9,9,4,2,9,8,9,9,9,9,9,7,9,8,9,9,4,9,2,3,9,8,7,6,7,8,9,9,8,9,7,7,8,9,8,9,8,9,9,9,7,6,5,6,9,9,9,3,2,9,8,9,8,9},
{3,4,5,8,9,1,0,9,9,7,6,7,6,7,8,9,8,9,9,8,7,6,9,8,9,2,1,2,9,8,7,6,7,6,8,9,9,6,5,6,4,3,2,3,4,6,7,8,9,9,8,7,8,8,9,8,8,6,9,7,9,8,9,8,9,4,9,9,6,5,6,7,9,9,9,9,8,9,9,9,9,8,9,9,8,6,6,5,4,7,8,9,8,9,9,8,7,8,9,7},
{4,5,6,7,8,9,9,9,9,8,7,8,7,8,9,5,7,9,8,7,6,5,6,7,8,9,2,3,5,9,8,7,8,9,9,9,8,7,8,9,5,4,5,4,9,7,8,9,9,9,9,6,7,7,9,7,6,5,7,6,5,7,8,7,8,9,8,7,6,4,5,6,7,8,7,8,9,2,1,9,8,7,9,9,9,5,4,3,2,8,9,5,6,7,9,7,6,5,4,6},
{6,8,7,8,9,7,8,9,9,9,8,9,8,9,3,4,9,8,9,8,9,4,5,6,9,6,5,4,5,8,9,8,9,0,2,4,9,8,9,9,8,7,6,5,6,9,9,9,9,8,8,4,5,6,8,9,5,4,6,5,4,6,5,6,9,8,9,8,4,3,4,5,8,9,6,7,8,9,9,8,7,6,9,9,8,9,9,2,1,2,3,4,5,8,9,9,7,9,9,7},
{7,9,8,9,5,6,9,8,9,9,9,1,9,1,2,9,8,7,8,9,8,9,6,7,8,9,7,9,6,7,8,9,3,1,2,3,9,9,6,4,9,9,8,9,7,8,9,9,8,7,6,3,4,7,7,9,4,3,2,1,2,3,4,9,8,7,9,9,3,2,1,2,3,4,5,8,9,9,8,7,6,5,8,7,6,7,8,9,0,1,2,5,6,7,9,1,9,9,8,9},
{8,9,9,1,3,5,6,7,7,9,9,0,9,0,1,9,7,6,7,6,7,8,9,8,9,9,8,9,8,9,9,8,4,2,4,9,8,6,4,3,2,3,9,9,8,9,9,8,7,5,4,2,3,5,6,7,9,9,3,0,2,3,4,5,9,6,5,4,3,1,0,1,4,5,6,8,9,1,9,9,5,4,3,4,5,6,9,2,1,3,9,8,9,8,9,2,9,8,7,9},
{9,4,1,0,1,2,4,5,6,7,8,9,8,9,9,8,6,5,4,5,6,7,8,9,1,2,9,4,9,9,8,7,5,3,6,7,9,6,5,4,1,3,4,6,9,9,8,7,6,5,2,1,2,4,5,6,7,8,9,1,3,4,5,9,8,7,6,4,3,2,3,2,3,5,8,9,1,0,9,7,6,5,4,5,7,7,8,9,2,5,6,7,9,9,5,9,8,7,6,7},
{4,3,2,1,2,6,7,8,9,8,9,8,7,5,4,9,8,7,6,8,9,9,9,1,0,1,2,3,9,8,7,6,5,4,5,9,8,7,6,3,2,4,5,6,9,8,7,7,6,4,1,0,2,3,4,5,6,9,8,6,5,5,6,7,9,9,7,9,4,3,4,3,4,5,7,8,9,9,9,9,8,8,7,6,7,8,9,9,7,6,7,8,9,5,3,4,9,6,5,4},
{6,5,3,2,3,5,6,7,8,9,8,7,6,4,3,4,9,8,7,8,9,8,9,4,3,2,3,5,6,9,8,7,6,5,6,7,9,8,5,4,6,5,9,9,8,7,6,5,4,3,2,1,3,4,5,6,7,8,9,7,6,7,7,8,9,9,9,8,9,4,5,6,5,6,8,9,9,8,9,9,9,9,8,7,8,9,4,9,8,7,8,9,4,3,2,9,8,7,6,5},
{7,6,5,6,4,6,7,8,9,3,9,8,7,5,4,5,9,9,8,9,6,7,8,9,6,5,4,5,6,9,9,8,8,7,9,9,8,7,6,7,9,9,8,9,9,8,7,6,5,4,3,2,4,5,6,8,8,9,9,8,8,9,9,9,9,9,8,7,8,9,6,8,9,7,8,9,8,7,8,9,8,9,9,9,9,4,3,2,9,8,9,7,5,2,1,2,9,9,7,6},
{8,7,6,7,5,8,9,9,5,4,8,9,9,7,5,6,8,9,9,4,5,9,9,8,7,7,5,6,9,8,9,9,9,8,9,1,9,8,7,9,8,9,7,8,9,9,8,7,6,5,4,8,6,6,8,9,9,4,5,9,9,9,8,9,9,8,7,6,7,8,9,9,9,8,9,8,7,6,9,8,7,8,9,9,7,6,4,3,4,9,6,5,4,3,2,3,4,9,8,8},
{9,8,7,9,6,7,8,9,7,5,7,9,8,9,6,7,9,8,7,5,9,8,9,9,9,8,9,9,8,7,8,9,5,9,1,0,2,9,9,8,7,6,6,9,9,9,9,8,8,7,6,7,8,9,9,1,2,3,6,7,8,9,7,8,9,8,6,5,6,7,8,9,4,9,8,7,6,5,3,9,6,7,8,9,8,7,6,5,7,8,9,7,5,9,4,4,9,9,9,9},
{2,9,8,9,7,8,9,9,9,6,9,8,7,8,9,8,9,9,8,9,8,7,8,9,9,9,5,4,9,6,9,6,4,3,2,1,3,4,5,9,9,4,5,7,8,9,7,9,9,8,7,8,9,4,3,2,3,9,9,8,9,8,6,5,4,5,4,4,7,8,9,2,3,9,8,6,5,4,2,6,5,6,9,4,9,8,9,6,8,9,9,8,9,8,9,9,8,9,3,1},
{1,2,9,9,8,9,9,9,8,9,7,7,6,9,9,9,1,2,9,9,7,6,8,9,9,7,6,9,8,7,8,9,6,5,4,2,5,6,7,8,9,9,6,9,9,9,6,5,6,9,8,9,9,6,5,3,9,8,7,9,8,7,5,4,3,4,2,3,4,9,4,3,5,9,6,5,4,2,1,2,4,8,9,5,6,9,9,7,8,9,9,9,8,7,6,8,7,8,9,9},
{0,4,6,7,9,9,9,8,7,6,5,4,5,7,8,9,0,9,8,8,6,5,6,8,9,8,7,9,9,8,9,8,7,6,5,6,6,7,8,9,9,8,7,8,9,8,7,6,8,9,9,7,8,9,7,9,8,7,6,5,9,9,8,3,2,1,0,4,5,6,9,4,9,8,7,8,5,4,3,3,6,7,8,9,9,8,9,8,9,9,9,8,7,6,5,4,6,7,9,7},
{2,3,7,8,9,8,8,9,6,5,4,3,4,6,7,8,9,8,7,6,7,4,7,7,8,9,9,8,9,9,9,9,9,8,9,8,7,8,9,3,5,9,8,9,9,9,8,7,9,8,7,6,7,9,8,9,6,5,4,3,4,9,7,5,6,7,2,5,6,9,8,9,8,9,8,7,6,6,5,4,5,6,7,8,9,7,8,9,9,9,8,6,9,5,4,3,5,8,9,6},
{4,4,9,9,8,7,7,9,9,4,3,2,3,5,6,9,8,7,6,5,2,3,4,6,7,8,9,7,9,8,8,9,9,9,9,9,9,9,1,2,4,5,9,9,9,9,9,8,9,9,6,5,6,8,9,8,9,6,3,2,3,4,9,7,8,8,3,4,7,9,7,6,7,9,9,9,7,8,7,5,7,8,9,9,5,6,7,9,8,7,6,5,6,4,3,2,6,3,4,5},
{9,9,8,7,6,5,6,7,8,9,2,1,2,4,8,9,9,8,6,3,1,3,5,6,8,9,6,6,7,6,7,8,9,7,8,9,9,1,0,6,5,9,8,9,8,9,9,9,9,9,8,4,8,7,8,7,8,9,2,1,2,5,9,8,9,6,5,9,9,8,9,5,6,7,9,9,8,9,8,9,8,9,6,6,4,8,6,8,9,7,5,4,3,2,1,0,1,2,3,4},
{8,7,9,8,8,7,8,9,9,4,3,2,3,4,7,8,9,6,5,4,2,3,4,5,9,9,5,4,4,5,6,9,8,6,7,9,8,9,1,9,9,8,7,6,7,8,9,9,9,8,7,3,1,6,5,6,7,8,9,4,3,6,7,9,8,7,9,8,7,6,5,4,5,6,8,9,9,3,9,9,9,6,5,4,3,4,5,9,8,7,6,8,7,5,2,1,2,3,4,5},
{9,6,5,9,9,8,9,7,6,5,4,3,4,5,6,8,9,7,6,7,9,6,5,6,7,8,9,3,2,9,9,7,6,5,9,8,7,8,9,8,7,6,5,5,6,9,8,9,8,7,6,2,0,5,4,5,6,7,8,9,4,5,7,8,9,9,8,7,6,5,4,3,4,6,9,6,5,4,9,9,8,7,6,5,4,6,6,7,9,9,8,9,9,4,3,2,3,4,7,6},
{8,9,3,4,5,9,9,8,7,9,8,7,6,8,9,9,9,9,7,8,9,8,8,7,9,9,5,4,9,8,7,6,5,4,4,9,6,9,8,7,6,5,4,3,4,6,7,8,9,9,4,3,1,2,3,4,8,9,9,6,5,6,7,9,5,3,9,8,5,4,3,2,3,5,8,9,9,9,8,9,9,8,7,6,7,8,9,8,9,8,9,8,8,9,4,5,6,5,6,7},
{7,8,9,5,6,7,8,9,9,8,9,8,7,8,9,9,9,9,8,9,9,9,9,9,8,7,6,9,8,7,6,5,4,3,3,4,5,7,9,8,7,6,5,7,6,7,8,9,8,8,9,4,3,4,4,5,6,7,8,9,6,7,8,9,3,2,9,8,7,5,4,3,4,5,6,7,8,9,7,6,8,9,9,9,8,9,9,9,8,7,8,7,7,8,9,6,7,6,7,8},
{6,7,9,6,9,8,9,9,8,7,6,9,8,9,6,9,7,9,9,9,9,8,7,8,9,8,7,8,9,9,8,6,3,2,1,3,4,8,9,9,8,7,8,9,9,8,9,4,6,7,9,9,4,6,5,6,7,9,9,8,7,8,9,8,9,1,9,9,8,7,6,4,5,6,7,9,9,7,6,5,7,8,9,9,9,0,9,8,7,6,7,6,6,9,8,9,8,7,9,9},
{5,7,8,9,9,9,5,7,9,9,5,4,9,3,4,7,6,8,9,9,8,7,6,7,8,9,9,9,9,8,6,7,2,1,0,2,3,9,9,9,9,9,9,7,6,9,4,3,4,8,7,8,9,8,7,9,8,9,9,9,8,9,6,7,8,9,8,9,9,8,7,6,7,9,8,9,7,8,7,4,5,9,9,8,9,2,9,8,6,5,3,4,5,6,7,8,9,9,1,0},
{4,5,6,7,8,9,4,6,9,8,4,3,2,1,3,4,5,6,7,9,8,8,5,6,6,7,8,9,8,7,5,4,3,2,3,3,5,6,9,8,7,9,8,6,5,3,2,1,2,9,6,7,9,9,9,9,9,7,8,5,9,4,5,6,9,8,7,6,7,9,8,7,8,9,9,8,6,4,3,2,1,2,5,7,8,9,8,7,8,3,2,3,4,5,6,7,9,8,9,1},
{3,7,7,9,9,9,2,9,7,6,5,5,3,2,3,5,6,7,9,9,7,7,4,5,5,6,9,8,9,9,8,7,6,5,4,8,7,9,8,7,6,8,9,8,6,4,3,4,3,4,5,8,9,9,9,8,7,6,5,4,3,2,3,7,8,9,7,5,6,9,9,8,9,7,9,7,6,5,2,1,0,1,4,5,9,8,7,6,5,4,3,4,5,6,7,8,9,7,8,9},
{9,8,8,9,6,8,9,9,8,7,6,6,4,3,4,6,7,9,8,7,6,5,3,2,3,4,5,6,7,8,9,8,8,7,5,6,9,8,7,6,5,9,8,7,6,5,4,5,4,5,6,9,9,8,6,9,8,7,6,5,2,1,4,5,7,8,9,6,9,8,7,9,5,6,9,8,7,6,8,8,9,2,3,6,8,9,8,7,9,5,6,5,6,7,8,9,5,6,7,8},
{4,9,9,4,5,6,7,8,9,9,8,6,5,4,5,7,9,9,9,9,7,4,3,1,3,3,4,5,6,9,9,9,9,8,7,9,8,7,6,5,4,3,9,8,7,9,8,7,5,6,7,8,9,5,5,6,9,8,7,6,7,2,3,4,9,9,9,9,8,7,6,5,4,2,2,9,8,7,8,7,6,5,4,5,6,7,9,9,7,6,7,8,7,8,9,3,4,5,8,9},
{2,1,2,3,4,5,6,9,9,9,8,7,6,5,6,7,8,9,9,8,7,5,1,0,1,2,5,6,7,8,9,1,0,9,9,9,9,9,8,4,3,2,1,9,8,9,9,8,9,8,9,9,5,4,3,2,4,9,8,7,8,9,4,5,6,7,8,9,9,6,5,4,2,1,0,1,9,8,9,8,7,7,6,6,7,8,9,9,8,7,8,9,8,9,1,2,3,6,9,9}
};

int main()
{
  if ( !testAlg() ) {
    return 1;
  }
  uint64_t numRows = input.size();
  uint64_t numCols = input[0].size();
  std::vector<MapPoint> lowPoints;
  findLowPoints( input, lowPoints );
  uint64_t dangerLevel = calcDangerLevelsSum( lowPoints );
  std::cout << "Danger level sum: " << dangerLevel << std::endl;
  return 0;
}

