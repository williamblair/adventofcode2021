#include <iostream>
#include <cctype>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

using Path = std::vector<std::string>;
using PathMap = std::map<std::string, std::vector<std::string>>;

static inline bool caveInPath( const Path& p, const std::string& cave )
{
    if ( std::find( p.begin(), p.end(), cave ) != p.end() ) {
        return true;
    }
    return false;
}

static PathMap genPathMap(
    std::vector<std::pair<std::string, std::string>>& input
)
{
    PathMap result;
    for ( auto pair : input )
    {
        /* Entry does not exist in the result map */
        if ( result.find( pair.first ) == result.end() )
        {
            /* Initialize std::vector<> */
            result[pair.first] = { pair.second };
        }
        /* Add to existing entries */
        else
        {
            result[pair.first].push_back( pair.second );
        }

        /* Entry does not exist in the result map */
        if ( result.find( pair.second ) == result.end() )
        {
            /* Initialize std::vector<> */
            result[pair.second] = { pair.first };
        }
        /* Add to existing entries */
        else
        {
            result[pair.second].push_back( pair.first );
        }
    }

    return result;
}

static void processCave(
    PathMap& pathMap,
    std::vector<Path>& result,
    Path& currentPath,
    std::string currentCave )
{
    for ( std::string& nextCave : pathMap[currentCave] )
    {
        Path nextPath = currentPath;

        /* Only allowed to visit lowercase caves once */
        if ( islower( nextCave[0] ) )
        {
            if ( caveInPath( currentPath, nextCave ) ) {
                continue;
            }
        }

        nextPath.push_back( nextCave );

        /* If we reached the end cave, don't process this path
         * any further */
        if ( nextCave == "end" ) {
            result.push_back( nextPath );
            continue;
        }

        /* Otherwise move on to the next path */
        processCave( pathMap,
            result,
            nextPath,
            nextCave );
    }
}

static void genAllPaths(
    PathMap& pathMap,
    std::vector<Path>& result
)
{
    Path currentPath = { "start" };
    std::string currentCave = "start";
    processCave( pathMap,
        result,
        currentPath,
        currentCave );
}

static bool testAlg()
{
    std::vector<std::pair<std::string, std::string>> paths = {
        {"start","A"},
        {"start","b"},
        {"A","c"},
        {"A","b"},
        {"b","d"},
        {"A","end"},
        {"b","end"}
    };
    PathMap pathMap = genPathMap( paths );

    std::vector<Path> allPaths;
    genAllPaths( pathMap, allPaths );

    if ( allPaths.size() != 10 ) {
        std::cout << "Path size failed (expected 10): " << allPaths.size() << std::endl;
        return false;
    }

    allPaths.clear();
    paths = {
        {"dc","end"},
        {"HN","start"},
        {"start","kj"},
        {"dc","start"},
        {"dc","HN"},
        {"LN","dc"},
        {"HN","end"},
        {"kj","sa"},
        {"kj","HN"},
        {"kj","dc"}
    };
    pathMap = genPathMap( paths );
    genAllPaths( pathMap, allPaths );

    if ( allPaths.size() != 19 ) {
        std::cout << "Path size failed (expected 19): " << allPaths.size() << std::endl;
        return false;
    }

    allPaths.clear();
    paths = {
        {"fs","end"},
        {"he","DX"},
        {"fs","he"},
        {"start","DX"},
        {"pj","DX"},
        {"end","zg"},
        {"zg","sl"},
        {"zg","pj"},
        {"pj","he"},
        {"RW","he"},
        {"fs","DX"},
        {"pj","RW"},
        {"zg","RW"},
        {"start","pj"},
        {"he","WI"},
        {"zg","he"},
        {"pj","fs"},
        {"start","RW"}
    };
    pathMap = genPathMap( paths );
    genAllPaths( pathMap, allPaths );
    if ( allPaths.size() != 226 ) {
        std::cout << "Path size failed (expected 226): " << allPaths.size() << std::endl;
        return false;
    }

    return true;
}

std::vector<std::pair<std::string, std::string>> input = {
{"EO","jc"},
{"end","tm"},
{"jy","FI"},
{"ek","EO"},
{"mg","ek"},
{"jc","jy"},
{"FI","start"},
{"jy","mg"},
{"mg","FI"},
{"jc","tm"},
{"end","EO"},
{"ds","EO"},
{"jy","start"},
{"tm","EO"},
{"mg","jc"},
{"ek","jc"},
{"tm","ek"},
{"FI","jc"},
{"jy","EO"},
{"ek","jy"},
{"ek","LT"},
{"start","mg"}
};

int main()
{
    if ( !testAlg() ) {
        return 1;
    }
    PathMap pathMap = genPathMap( input );
    std::vector<Path> allPaths;
    genAllPaths( pathMap, allPaths );
    std::cout << "Paths size: " << allPaths.size() << std::endl;
    return 0;
}

