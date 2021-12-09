#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <cstdint>
#include <algorithm>
#include <map>

// key = the number displayed
// value = the number of segments the key uses to be displayed
std::map<int,int> digitSegmentsMap = {
    {0, 6},
    {1, 2}, // 2 unique
    {2, 5},
    {3, 5},
    {4, 4}, // 4 unique
    {5, 5},
    {6, 6},
    {7, 3}, // 3 unique
    {8, 7}, // 7 unique
    {9, 6}
};

// key = the number displayed
// value = which segments are turned on
//     0
//  1     2
//     3
//  4     5
//     6
std::map<int, std::vector<int>> digitSegLocationsMap = {
    {0, { 0, 1, 2, 4, 5, 6 }},
    {1, { 2, 5 }},
    {2, { 0, 2, 3, 4, 6 }},
    {3, { 0, 2, 3, 5, 6 }},
    {4, { 1, 2, 3, 5 }},
    {5, { 0, 1, 3, 5, 6 }},
    {6, { 0, 1, 3, 4, 5, 6 }},
    {7, { 0, 2, 5}},
    {8, { 0, 1, 2, 3, 4, 5, 6 }},
    {9, { 0, 1, 2, 3, 5, 6}}
};

static inline void addPossibleCharsToMap(
    std::vector<int>& segLocMap,
    std::map<char,bool>* possibleMappings,
    std::string& str
)
{
    /* This must be a combination of letter segment locations
     * for that number mapping */
    if ( str.size() == segLocMap.size() ) {
        for ( int mapIndex : segLocMap )
        {
            /* No possible locations have been added, so add them all */
            if ( possibleMappings[mapIndex].empty() )
            {
                for ( char c : str )
                {
                    possibleMappings[mapIndex].insert({c, true});
                }
            }
            /* If there are less possibilities from the given string,
             * we can remove the non-matching ones from the map */
            else if ( str.size() < possibleMappings[mapIndex].size() )
            {
                for ( auto keyValPair : possibleMappings[mapIndex] )
                {
                    auto strIter = std::find( str.begin(), str.end(), keyValPair.first );
                    if ( strIter == str.end() )
                    {
                        possibleMappings[mapIndex].erase( keyValPair.first );
                    }
                }
            }
        }
    }
}

static bool calcSegmentMap(std::vector<std::string>& sigPatterns,
        std::vector<std::string>& digOutputs,
        std::vector<int>& decodedOutResults)
{
    constexpr size_t NUM_SEGMENTS = 7;
    std::map<char,bool> possibleMappings[NUM_SEGMENTS];

    /* Fill in as much of the map as we can */
    for ( std::string& str : sigPatterns )
    {
        addPossibleCharsToMap( digitSegLocationsMap[1], possibleMappings, str );
        addPossibleCharsToMap( digitSegLocationsMap[4], possibleMappings, str );
        addPossibleCharsToMap( digitSegLocationsMap[7], possibleMappings, str );
        addPossibleCharsToMap( digitSegLocationsMap[8], possibleMappings, str );
    }

    /* Now try combinations until we find a valid one */
    for ( auto pos0pair : possibleMappings[0])
    for ( auto pos1pair : possibleMappings[1])
    for ( auto pos2pair : possibleMappings[2])
    for ( auto pos3pair : possibleMappings[3])
    for ( auto pos4pair : possibleMappings[4])
    for ( auto pos5pair : possibleMappings[5])
    for ( auto pos6pair : possibleMappings[6])
    {
        std::list<char> availChars = { 'a', 'b', 'c', 'd', 'e', 'f', 'g' };
        char pos0char = 'Z';
        char pos1char = 'Z';
        char pos2char = 'Z';
        char pos3char = 'Z';
        char pos4char = 'Z';
        char pos5char = 'Z';
        char pos6char = 'Z';

#define AttemptAssignChar( charIterName, posPairName, posCharName ) \
        auto charIterName = std::find( \
            availChars.begin(),\
            availChars.end(),\
            posPairName.first\
        ); \
        if ( charIterName == availChars.end() ) continue; \
        posCharName = *charIterName; \
        availChars.erase( charIterName );

        AttemptAssignChar( char0iter, pos0pair, pos0char )
        AttemptAssignChar( char1iter, pos1pair, pos1char )
        AttemptAssignChar( char2iter, pos2pair, pos2char )
        AttemptAssignChar( char3iter, pos3pair, pos3char )
        AttemptAssignChar( char4iter, pos4pair, pos4char )
        AttemptAssignChar( char5iter, pos5pair, pos5char )
        AttemptAssignChar( char6iter, pos6pair, pos6char )

#undef AttemptAssignChar
        
        /* Generate valid strings based on the pairings */
        std::map<int,char> posToCharMap = {
            {0, pos0char},
            {1, pos1char},
            {2, pos2char},
            {3, pos3char},
            {4, pos4char},
            {5, pos5char},
            {6, pos6char}
        };
        std::vector<std::string> validStrings;
        auto alphaSortFunc = [](char a, char b) { return a < b; };
        for ( auto digSegPair : digitSegLocationsMap)
        {
            std::string res = "";
            /* Add which characters form the number */
            for ( int pos : digSegPair.second )
            {
                char c = posToCharMap[pos];
                res += c;
            }
            /* Make the string alphabetical */
            std::sort( res.begin(), res.end(), alphaSortFunc );
            validStrings.push_back(res);
        }

        /* Check if all of the input strings are valid with our current mapping */
        bool allMatching = true;
        for ( std::string& str : sigPatterns )
        {
            std::string strSorted = str;
            std::sort( strSorted.begin(), strSorted.end(), alphaSortFunc );
            allMatching = false;
            for ( std::string& validStr : validStrings )
            {
                if ( validStr == strSorted )
                {
                    allMatching = true;
                    break;
                }
            }
            if ( !allMatching )
            {
                break;
            }
        }
        /* If not all valid, this mapping doesn't work */
        if ( !allMatching )
        {
            continue;
        }

        /* Otherwise, found the map, and we can decode the results */
        decodedOutResults.clear();
        for ( std::string& digStr : digOutputs )
        {
            std::string strSorted = digStr;
            std::sort( strSorted.begin(), strSorted.end(), alphaSortFunc );
            for ( int i = 0; i < validStrings.size(); ++i)
            {
                if ( strSorted == validStrings[i] )
                {
                    decodedOutResults.push_back(i);
                }
            }
        }
        return true;
    }

    return false;
}

static inline int intVecToInt( std::vector<int>& vec )
{
    int result = 0;
    int multiplier = 1;
    for ( auto iter = vec.rbegin(); iter != vec.rend(); ++iter )
    {
        result += (*iter) * multiplier;
        multiplier *= 10;
    }
    return result;
}

static bool testAlg()
{
    std::vector<std::vector<std::string>> testInputSigPatterns = {
    {"be", "cfbegad", "cbdgef", "fgaecd", "cgeb", "fdcge", "agebfd", "fecdb", "fabcd", "edb"},
    {"edbfga", "begcd", "cbg", "gc", "gcadebf", "fbgde", "acbgfd", "abcde", "gfcbed", "gfec"},
    {"fgaebd", "cg", "bdaec", "gdafb", "agbcfd", "gdcbef", "bgcad", "gfac", "gcb", "cdgabef"},
    {"fbegcd", "cbd", "adcefb", "dageb", "afcb", "bc", "aefdc", "ecdab", "fgdeca", "fcdbega"},
    {"aecbfdg", "fbg", "gf", "bafeg", "dbefa", "fcge", "gcbea", "fcaegb", "dgceab", "fcbdga"},
    {"fgeab", "ca", "afcebg", "bdacfeg", "cfaedg", "gcfdb", "baec", "bfadeg", "bafgc", "acf"},
    {"dbcfg", "fgd", "bdegcaf", "fgec", "aegbdf", "ecdfab", "fbedc", "dacgb", "gdcebf", "gf"},
    {"bdfegc", "cbegaf", "gecbf", "dfcage", "bdacg", "ed", "bedf", "ced", "adcbefg", "gebcd"},
    {"egadfb", "cdbfeg", "cegd", "fecab", "cgb", "gbdefca", "cg", "fgcdab", "egfdb", "bfceg"},
    {"gcafb", "gcf", "dcaebfg", "ecagb", "gf", "abcdeg", "gaef", "cafbge", "fdbac", "fegbdc"}
    };

    std::vector<std::vector<std::string>> testInputDigValues = {
    {"fdgacbe", "cefdb", "cefbgd", "gcbe"},
    {"fcgedb", "cgb", "dgebacf", "gc"},
    {"cg", "cg", "fdcagb", "cbg"},
    {"efabcd", "cedba", "gadfec", "cb"},
    {"gecf", "egdcabf", "bgf", "bfgea"},
    {"gebdcfa", "ecba", "ca", "fadegcb"},
    {"cefg", "dcbef", "fcge", "gbcadfe"},
    {"ed", "bcgafe", "cdgba", "cbgef"},
    {"gbdfcae", "bgc", "cg", "cgb"},
    {"fgae", "cfgab", "fg", "bagce"}
    };

    std::vector<int> expectedResults = {
        8394,
        9781,
        1197,
        9361,
        4873,
        8418,
        4548,
        1625,
        8717,
        4315
    };

    std::vector<int> decodedResults;
    size_t sum = 0;
    for ( size_t i = 0; i < testInputSigPatterns.size(); ++i )
    {
        if ( calcSegmentMap( testInputSigPatterns[i],
                testInputDigValues[i],
                decodedResults ))
        {
            std::cout << "Result: ";
            for ( int j : decodedResults )
            {
                std::cout << j << ", ";
            }
            int combined = intVecToInt( decodedResults );
            if ( combined != expectedResults[i] ) {
                std::cout << "Combined result failed: "
                    << combined << ", " 
                    << expectedResults[i]
                    << std::endl;
                return false;
            }
            std::cout << "Int result: " << combined << std::endl;
            sum += combined;
        }
    }
    std::cout << sum << std::endl;
    if ( sum != 61229 ) {
        return false;
    }
    return true;
}

std::vector<std::vector<std::string>> inputSegments = {
{"ecfdbg", "decfba", "aegd", "fdcag", "fagecd", "gd", "gcafb", "efdac", "cbgeafd", "dfg"},
{"cfdabeg", "cda", "bcgad", "bedfac", "aefbgd", "bfgcad", "dfbga", "egabc", "dgfc", "cd"},
{"aedbfgc", "gcaf", "ebgfca", "bca", "edgcfb", "adbge", "gebac", "fbegc", "ac", "afdcbe"},
{"gaeb", "cafdbg", "aedfc", "ecfgdb", "ab", "cdaefgb", "cgbdae", "bdecg", "cba", "eacdb"},
{"fcbgea", "fbedg", "bc", "cbefg", "befgcda", "cfgdab", "gbc", "efdgca", "fecag", "aceb"},
{"fdebcg", "bag", "gdefa", "ceadgb", "fcab", "ebagfc", "ab", "fbegc", "fcaedbg", "fbeag"},
{"fg", "fcagb", "dfgceba", "edgacb", "fagd", "gfb", "dcgba", "agcfdb", "gecfbd", "ceafb"},
{"eabfd", "dfb", "gbcfad", "ebdag", "fagbce", "gefabdc", "edcf", "aecfb", "cbdafe", "df"},
{"fbaec", "bfegad", "bga", "deacbg", "gb", "fbdg", "acgdef", "deagf", "fgabe", "gabecdf"},
{"bfdea", "agfbc", "gbd", "faecdbg", "gd", "efgd", "cdgeab", "efadbg", "faebcd", "fabdg"},
{"gfedac", "cgaef", "gab", "ceba", "ba", "afcdgb", "gdefbac", "fegbd", "bafeg", "abcgfe"},
{"bgda", "agfcdbe", "agfde", "faegb", "baedgf", "fcdaeg", "fdbeac", "ba", "bcegf", "bfa"},
{"dfgbac", "fdcaeb", "cbgeda", "bfdea", "dcabf", "ebadcfg", "gfbea", "dbe", "fecd", "ed"},
{"efdba", "edb", "eagbcd", "bacef", "bd", "bcfd", "becfagd", "begcaf", "cbaefd", "aefdg"},
{"gfecbd", "gbadf", "fabged", "gbf", "cbdage", "fagcd", "feab", "edgcbaf", "fb", "aedbg"},
{"gfaedc", "caefb", "badcfge", "dgfe", "acdgbf", "eag", "eg", "fgacd", "gecfa", "ecbdag"},
{"gfebda", "fgacd", "dfbag", "bg", "geab", "gcbdeaf", "eacbdf", "cedbgf", "fbg", "abfed"},
{"ebadfc", "efdbga", "bega", "fdgcae", "dbgaefc", "edfab", "fag", "ag", "dagbf", "cfgbd"},
{"bf", "fbd", "cgbdaf", "dcfbae", "fgbcd", "gcbadef", "gafcde", "bfag", "cebgd", "dcafg"},
{"gfecab", "fageb", "cgb", "acgeb", "gdfeab", "ecbafdg", "gfca", "aecbd", "gc", "edcgbf"},
{"cdge", "abfdge", "cgfeadb", "bdc", "gbacd", "cd", "cdaebf", "bgeda", "cgdaeb", "acfgb"},
{"efabdgc", "ecafg", "dacgbf", "ecgfd", "dfe", "gbfdec", "ed", "bedc", "cfdgb", "gedafb"},
{"bfagdc", "dbagef", "gbedcf", "afdeb", "ab", "edgfb", "gdebfac", "fdeca", "baf", "eagb"},
{"cfdbae", "dgefc", "fcaeg", "geafdc", "dfag", "fd", "cegbd", "daefbgc", "cfd", "cefbag"},
{"cbged", "feabc", "fdgb", "egf", "ecfdbg", "dfgcea", "gabfdce", "ebcdag", "bgefc", "fg"},
{"ebcdfa", "ef", "cfe", "cadbfge", "becfd", "dgbeac", "dafe", "gbdfc", "cefgba", "ceabd"},
{"fbdage", "bf", "acdgb", "cabf", "gcfed", "gdcfbea", "fgbdc", "fbd", "fcbadg", "bgedca"},
{"gfacdbe", "fga", "gbdaf", "gadbc", "dfeagc", "fg", "bcgf", "eacgbd", "acdgfb", "debfa"},
{"gadbcf", "badfe", "bae", "cfaed", "abfdg", "adfecbg", "eb", "bedgaf", "ecfagb", "egdb"},
{"dbgecaf", "gacd", "dfbac", "dbaefg", "dba", "bfgced", "cafbdg", "cbfgd", "faecb", "ad"},
{"afgcd", "dabgf", "gedfba", "agc", "bdfgac", "gdefc", "bcaf", "cfeadgb", "ac", "ecdbga"},
{"acbegfd", "cgeb", "dagbef", "gbaed", "caebdg", "gc", "dafgec", "adcbg", "cdbfa", "gdc"},
{"gfab", "agfbde", "fa", "edbgf", "cbedfa", "decga", "cdagefb", "degbfc", "egfda", "afd"},
{"caegdbf", "daefg", "fbgac", "be", "fgaeb", "cbea", "bge", "cdfagb", "dgecfb", "abecfg"},
{"efcgda", "bd", "efgdcab", "afbec", "bgcd", "fdagc", "dgabef", "dab", "dafbc", "cfdagb"},
{"abdge", "fd", "fda", "fecd", "bfcage", "fecdba", "fdgbcea", "afbde", "acdbfg", "cfbea"},
{"acfdbe", "egcadb", "gfbacd", "acfbd", "bagefcd", "bgfa", "cefgd", "gac", "cdagf", "ag"},
{"eabdfc", "fcaeg", "fecad", "bcfda", "dcfbge", "ebda", "de", "ced", "fdacbg", "dfbcega"},
{"bfc", "gdfceb", "cf", "eafgbd", "edcab", "ebcfa", "gfeacb", "acgf", "adcbgef", "fabge"},
{"gfbc", "gaced", "fg", "bfcda", "fgd", "fcgad", "dgefbac", "bcafde", "dgcfab", "edgfba"},
{"acbdgf", "dfcgb", "dcfeg", "egbd", "aedfc", "fcebag", "ge", "gef", "cfegdba", "fgbdce"},
{"agcfd", "fga", "fecda", "gadebf", "dbgcfe", "fcbdga", "dcfgb", "ga", "gbac", "cbfgead"},
{"eba", "ebdc", "gbefda", "dcbegaf", "fbaced", "afgec", "be", "dcfba", "ebacf", "dfcagb"},
{"bfcge", "bfead", "edgfacb", "edcfag", "abegfd", "becdf", "dacb", "cd", "cde", "bfdcea"},
{"bacegd", "gafcbe", "begfa", "bcfad", "ed", "gedf", "ebgfda", "bdefa", "ead", "fegdbac"},
{"df", "fbgdc", "fcd", "bfgce", "abfd", "cdfgba", "cgefad", "cgbad", "ecadgb", "egbfacd"},
{"dfabge", "bcfdg", "egfbca", "bdaefcg", "decbfg", "ebfgd", "fgdca", "cb", "cbde", "cbg"},
{"fgda", "dafecgb", "egdbf", "gef", "dbgec", "agebcf", "gf", "edgafb", "fcaedb", "bafde"},
{"abfgecd", "eabgf", "fcdabe", "gbdafe", "acebg", "dfecgb", "gf", "gfb", "fgad", "dafeb"},
{"ceafbg", "dfcabe", "gcfea", "gbac", "fgead", "gc", "cbdgef", "egc", "aecfgbd", "cefba"},
{"cadfb", "caefbd", "cbfega", "fdbga", "cad", "dc", "edbc", "aefbc", "egbacfd", "dfgeca"},
{"gfcb", "cf", "ecafdg", "fcbed", "dfbagce", "fbgeda", "gdfbec", "cef", "eacbd", "ebdfg"},
{"gd", "cgd", "ebdafgc", "cfbega", "gcbfde", "bcfdg", "badfc", "bgaced", "bcefg", "gedf"},
{"bfga", "dabge", "befacd", "agbdce", "fb", "aegfbd", "fcged", "dbegf", "ebf", "ebgdcaf"},
{"beac", "ea", "ade", "gefcd", "ecgbad", "cbagd", "badgcfe", "ebfgad", "acgde", "gcfdab"},
{"cadfbg", "bcgafed", "cbedf", "afeg", "abegdc", "adf", "gedca", "efacd", "acdgfe", "fa"},
{"egd", "ed", "egacd", "cgabfed", "efcd", "degfca", "dagbfc", "dcagf", "bgfdea", "abecg"},
{"cgdeaf", "fcbdeg", "fgc", "dgcea", "cadgbe", "abdcfeg", "gf", "cafbe", "gadf", "cfega"},
{"caegd", "cbfgae", "dafg", "egd", "gd", "ecafg", "gfabdec", "cafdge", "bacde", "bdfecg"},
{"badgfe", "ed", "dcge", "fgdceb", "cebfd", "cbfedga", "dabcf", "efd", "ecgafb", "cgbef"},
{"dgabcef", "bcd", "cfeadb", "degfca", "bgace", "gcebd", "cegfd", "gbdf", "bd", "begfdc"},
{"edafgb", "bagf", "acedb", "dbg", "fcgdabe", "bcdgfe", "gb", "bgead", "dfaecg", "faged"},
{"bc", "abcegf", "bfcgd", "gdcbef", "dcbe", "gcb", "cafegbd", "aegdbf", "dfagc", "dbegf"},
{"be", "ecgbf", "cbafg", "ebg", "bagedc", "fbae", "degfabc", "fgcdab", "dgfce", "cabfeg"},
{"af", "dbacge", "efa", "abegfdc", "gfdce", "begcfa", "aecfg", "afbced", "fgba", "ecabg"},
{"acbedg", "edfacbg", "begd", "cdfabe", "dagcb", "eacdb", "gfeabc", "bg", "gba", "dfgac"},
{"dcbgaef", "bca", "abed", "cbgad", "gdaec", "fecbag", "edgbca", "geadfc", "ab", "cdgfb"},
{"cgabe", "gfcbe", "cfegba", "fdbgc", "fe", "fdgebca", "aedcbg", "gfea", "bfdcea", "fbe"},
{"afecdg", "dfbc", "cd", "dcg", "aegcb", "baedgf", "becgd", "becfdg", "eacgbdf", "ebdgf"},
{"gfdba", "adbefc", "ebcdfag", "bcgaf", "bfdaeg", "dg", "adbfe", "egbacd", "bgd", "dfge"},
{"dfaegb", "fe", "bcgde", "face", "afgbdc", "acgebdf", "agfcb", "cgbef", "fcgbea", "gfe"},
{"abdcef", "gdfea", "cbaef", "gaefb", "cagebf", "agfdbec", "agdbfc", "ecgb", "fbg", "bg"},
{"bcf", "gbadec", "bf", "bgaf", "fcdea", "cgbda", "gebdfc", "dgefcab", "fcabd", "bdagcf"},
{"cdafgb", "bgdf", "gacefd", "cdb", "bd", "ecfadbg", "cdafg", "cdebag", "bfadc", "ecafb"},
{"egb", "dgbf", "bgdeca", "febag", "fbadge", "ebadf", "aecfg", "gb", "geacfdb", "fbacde"},
{"ac", "eagfbdc", "bgacf", "cbdgae", "gabdf", "acdf", "gdfaeb", "acg", "dbgafc", "gbfce"},
{"ef", "eagf", "bfegcd", "gbaefd", "fbe", "eafbd", "dagfb", "abced", "agcfbd", "abcefdg"},
{"fdgbc", "efbgd", "aedbg", "dfce", "bcagdf", "fe", "fegcab", "cdgabef", "efg", "cgdfbe"},
{"fe", "dbefgac", "baegc", "eagfc", "ebgf", "acdgf", "aedbfc", "bgafce", "cef", "bcgdea"},
{"fbgcd", "bdf", "dfbega", "dfcga", "abfdcg", "db", "dabc", "cfebg", "feadgc", "ecgbdaf"},
{"bac", "cdbeagf", "abfd", "bcfea", "edcfba", "abdcge", "edacf", "gcbfe", "gcdefa", "ba"},
{"gcabfd", "acdg", "cg", "gfdbcae", "ecbgfa", "cbg", "bfegd", "gdcfb", "cedafb", "fadcb"},
{"fcdaegb", "gdefcb", "dgfacb", "dcgea", "eb", "dgcbf", "afcegb", "bgced", "bdef", "bec"},
{"defcb", "eabcf", "bca", "abfeg", "ac", "cafbge", "gfac", "dbaecg", "fcbegad", "bagedf"},
{"gdcfea", "adgcf", "cbged", "bgadfc", "gbf", "gfdbc", "cfegdba", "bcfa", "bf", "dgaefb"},
{"bdgcaef", "adg", "egbca", "cdabg", "eadbcg", "eadc", "cgdfb", "efagbd", "ad", "ebfagc"},
{"cade", "afgecd", "fgedc", "cbaegf", "efbgd", "gdfcabe", "ce", "fec", "cadgfb", "dcgaf"},
{"efgcba", "ged", "gcdbea", "acgeb", "decag", "cdgaf", "egbcafd", "bdae", "fgcbed", "de"},
{"agfce", "af", "febgc", "ebgadc", "cadeg", "dafg", "faegcd", "bceadf", "bfaecgd", "efa"},
{"cbgdae", "debcfa", "becadfg", "agecb", "gcde", "cfgba", "ce", "begda", "bec", "fdaebg"},
{"bagedf", "ecf", "ec", "dbefc", "cbedfa", "edca", "dfbcg", "edbfa", "eabgfc", "aefgcdb"},
{"aef", "bface", "dabefg", "ebfdgac", "caeg", "bgfac", "fcegba", "fcbed", "ae", "cfgbad"},
{"dfeca", "fdagcb", "feagc", "adcbef", "fg", "gefd", "cabge", "fgdbace", "cegadf", "cgf"},
{"gbaefd", "agedbcf", "cagbe", "fb", "cgbeaf", "baefc", "bfgc", "bgeacd", "efb", "dcfae"},
{"cegf", "bcedg", "bgacdef", "gbedf", "fg", "ebadf", "ebgdcf", "dgcfba", "debgac", "bfg"},
{"decafgb", "acgbd", "fegdab", "cefag", "dfcga", "gdf", "fd", "aecfgb", "dfecga", "cefd"},
{"faebc", "acb", "bdfgca", "afgedcb", "ab", "daeb", "fcbge", "afcebd", "feacd", "cafedg"},
{"gfeb", "ebgcdfa", "dfgca", "abfdg", "agebd", "bf", "abf", "bceafd", "dabgce", "dbfgae"},
{"ecbdfg", "gecdfa", "efcadgb", "ebdac", "deabf", "fbga", "bf", "dbf", "efagbd", "dfgae"},
{"fg", "cedbgaf", "feacbd", "dafgeb", "fag", "gfecba", "acgde", "febac", "cfega", "fcgb"},
{"cfbgde", "aegcdf", "daefbc", "dgceb", "fcged", "gafdecb", "cbfg", "bc", "ceb", "gebad"},
{"cdefga", "fbgcade", "fgdca", "cd", "bafdec", "adc", "dbgeaf", "cafgb", "efgda", "cegd"},
{"agfbc", "af", "deagfbc", "fecbg", "gfa", "fcegbd", "cfae", "bgafec", "edagbf", "bcadg"},
{"bdgf", "afceg", "adcfeb", "egbaf", "afgbde", "afb", "fb", "cegbfda", "daebg", "adgbec"},
{"efgb", "eg", "aecbd", "ecbdg", "daecgf", "dcgfb", "fbagdc", "egfadcb", "gce", "ecbgdf"},
{"acgbd", "eb", "dbace", "bdafgc", "fabdge", "bceg", "gecdba", "edcaf", "aeb", "bagcdef"},
{"cadfgb", "acefdg", "fcb", "fbea", "fb", "ebdfca", "ebcdf", "degcb", "dfaec", "cbdefga"},
{"fcdab", "edc", "cfeb", "gdfcae", "dcaeb", "ec", "gdcbaf", "fdcabe", "debga", "efbgadc"},
{"gb", "gefca", "acbfge", "bdgaecf", "bfcag", "bdfca", "abg", "aedfgb", "gebc", "agfecd"},
{"dgabcef", "fbdgea", "fb", "cdbf", "cebfg", "bdaegc", "dgceb", "egcdfb", "bfg", "acgef"},
{"cfaebdg", "fg", "fgcead", "agedb", "fge", "ebcdf", "gebdf", "dfeagb", "dbgace", "fgab"},
{"fbe", "egcba", "bgefdca", "bgdafe", "afdgb", "fdbgac", "dgcbfe", "fbega", "defa", "fe"},
{"gdc", "acfdeg", "geabc", "dbfg", "dabcfe", "dbeafcg", "dg", "bedcf", "edcgb", "efcbdg"},
{"bafgcd", "febdgca", "abgcf", "bcgea", "be", "edgac", "aeb", "cbef", "afebgd", "ecgbfa"},
{"cdae", "cbd", "gacbe", "fcbdge", "cefbag", "bagcde", "gcabd", "bdafg", "dc", "fdgebca"},
{"dac", "dfgae", "fcdabg", "agbc", "dcfbg", "gfedbc", "fadcg", "bfaegcd", "ca", "eadbcf"},
{"egbdcf", "bdeagfc", "aegcb", "bafec", "fdceb", "edgafc", "caebfd", "aef", "fbad", "af"},
{"ebadg", "fcaeb", "dc", "acdg", "becgfd", "adecgbf", "dbc", "dbeafg", "dbaec", "bedagc"},
{"cfaegbd", "agbdce", "edca", "badgf", "edgfbc", "ebagd", "ed", "gde", "aebfgc", "ebcag"},
{"fgb", "dfagceb", "cabgdf", "gf", "afge", "ecbaf", "deabcf", "cbdeg", "bgefc", "gfceba"},
{"abd", "fgcdaeb", "gfeabc", "gdfacb", "fcbea", "faebcd", "dcbeg", "badce", "ad", "dafe"},
{"fcgdeab", "eabgf", "feagd", "dge", "dg", "cdaefb", "acedbg", "cdfg", "gcaedf", "efadc"},
{"fegcb", "fcedag", "badgce", "fcbdgea", "afg", "gafeb", "af", "dbgefa", "degba", "fadb"},
{"abcdfg", "dcbfea", "bagd", "cegfb", "bd", "dbf", "cgafde", "edgcbfa", "fgdca", "bcdfg"},
{"afgbd", "fbgde", "dcfageb", "ag", "gba", "gfdcab", "eadgcb", "cfag", "daebfc", "cdbfa"},
{"egf", "ecabfd", "fg", "ecdfb", "cfgb", "cbegadf", "dgbfae", "begcfd", "agcde", "cgdfe"},
{"bafcdge", "dgbace", "cgbad", "cdafe", "bedcfg", "be", "abge", "cebad", "dcfabg", "bed"},
{"becagf", "fdec", "debafc", "afedb", "adbgc", "bgdfea", "dabcf", "cf", "cabegfd", "afc"},
{"dcagb", "gfcade", "cgdebf", "fdbeag", "gdfeabc", "efdcg", "ecfb", "deb", "egbcd", "eb"},
{"ebcdaf", "fgdc", "gbeac", "dgfae", "efadbg", "cd", "gcefda", "dgace", "fbgedac", "cda"},
{"fgdcb", "adceb", "fcgeabd", "acedbg", "af", "fba", "aebgdf", "efac", "dcabfe", "adbcf"},
{"fcb", "fc", "fcdgb", "dabfg", "eabcfdg", "cbged", "cefg", "begdca", "gcdbfe", "efdacb"},
{"gfdabc", "ad", "bdfgea", "fegbdac", "gbaecf", "bfega", "adg", "adbe", "afged", "egdcf"},
{"bgdcfea", "afb", "gabfcd", "adbfeg", "efdba", "bgdfe", "dfgbce", "eafdc", "bage", "ab"},
{"geabd", "adecfgb", "cg", "ebcdfa", "agfbdc", "fdcg", "dabcf", "dcbag", "faebcg", "cbg"},
{"bdfa", "dafbge", "fgedb", "bfceg", "bdg", "afdebgc", "bd", "bdaecg", "defcga", "gfdea"},
{"aedcgb", "fb", "gdbaf", "dgabe", "acdfg", "fabgde", "fadecgb", "gbf", "edfb", "facegb"},
{"dfbce", "gafed", "ba", "eagb", "gfdbca", "aefdb", "fecdag", "fagbedc", "abf", "egadfb"},
{"afdec", "fcegda", "edgabc", "bgdfc", "faebdc", "ba", "bac", "dcfebag", "fadbc", "fbae"},
{"gecd", "dgf", "eabdf", "gd", "gfdeb", "fgcadb", "gcbfae", "cefbg", "cedfbg", "cfeabdg"},
{"adfge", "bcgfde", "baecg", "dcfgaeb", "bafdge", "gdeac", "dagcef", "dc", "cdg", "acdf"},
{"efgac", "agbfedc", "aecbfd", "agbfde", "dcbg", "fdbag", "gacdfb", "bc", "fgbac", "fbc"},
{"be", "cbfgad", "acegdb", "agefd", "dabgc", "cebfgad", "gbce", "bdega", "dfbace", "bed"},
{"gf", "fag", "afgbe", "ecbaf", "geadb", "eabgdf", "egdf", "dagbfc", "ecagdb", "cbgdeaf"},
{"bag", "defba", "gcfdae", "cgbd", "gb", "eabgfc", "gcfabd", "dfagb", "gcdfa", "ecbgfda"},
{"adgfcbe", "egd", "fgadeb", "cgbdae", "acdgb", "cgedb", "deca", "de", "efcbg", "acbdgf"},
{"cagdebf", "dgebcf", "edc", "abecfd", "egfadb", "fabed", "ec", "gadbc", "efca", "dabce"},
{"fcedg", "dfgbae", "eg", "deg", "egcb", "fgcebd", "efacd", "gfdcb", "gbfecda", "gdafbc"},
{"dbag", "aefcbd", "gbc", "gcdbf", "bdcfa", "bg", "dfabcg", "ebadgcf", "cfgeba", "dgecf"},
{"cdgfbea", "gd", "bdag", "gebca", "bdcgfe", "dgc", "fcagbe", "gabedc", "afcde", "cdaeg"},
{"cedgbfa", "ecabgd", "cdafeb", "bafcg", "cegdbf", "cae", "gdae", "gabce", "ae", "edcgb"},
{"eacgbf", "afbdg", "gdecaf", "gafbcde", "gfc", "baefc", "bcge", "fcabg", "fdcbea", "cg"},
{"fgebd", "fed", "bfea", "cbdeg", "decfga", "fe", "cfdgba", "defbgac", "dfgba", "afdbeg"},
{"adbfcg", "fagdbe", "bagec", "dc", "fbaedcg", "bdfag", "gbdca", "fcgedb", "dcfa", "cgd"},
{"fgbaecd", "fgecd", "cafeg", "feacbg", "fdgcb", "fgaecd", "acdbge", "ed", "adfe", "dge"},
{"gedab", "afdcbeg", "decabg", "bfedcg", "dgefab", "ef", "aedf", "cfgab", "gbaef", "efb"},
{"fdb", "ebdgfc", "bd", "adefgcb", "efgad", "gbcd", "dfbge", "afcgeb", "fecgb", "dbeacf"},
{"abgde", "bdefa", "fagdbe", "fbadceg", "dgecab", "dfe", "ef", "edfgca", "bfdac", "fbge"},
{"febad", "bacged", "fge", "cdbeg", "cfebdag", "fgdc", "gf", "debcgf", "bdefg", "cfebga"},
{"agdcbf", "bdefcga", "cbgefd", "dcagb", "gbedac", "faedc", "gfc", "bfag", "gfcad", "gf"},
{"bdce", "gafbed", "gaecf", "bcefdag", "gbc", "dacbeg", "fcbgad", "egdab", "cb", "cegba"},
{"ac", "afgceb", "eagbf", "befcagd", "abcg", "degcf", "aefbdg", "ceagf", "edbacf", "acf"},
{"abcg", "fcaeb", "gfdcabe", "ab", "befdc", "cfage", "gdbfea", "gecdaf", "efcgba", "bfa"},
{"fbdeg", "gb", "fcdeb", "dfecbg", "eafgdbc", "edgfa", "eagbdc", "baedfc", "cbfg", "beg"},
{"afg", "afdbec", "cbdgaf", "afcgedb", "bedgf", "cdagef", "ga", "afcdb", "gbca", "gfbda"},
{"edgafcb", "fdeab", "dgabcf", "cbgf", "gdfeac", "dgfab", "gdeacb", "acgbd", "gfa", "gf"},
{"dcbfa", "gfbace", "ebfca", "efcg", "bgfeadc", "abgce", "gbafde", "ef", "beadcg", "fbe"},
{"gc", "cfdbe", "fgcebd", "bcgef", "ceg", "fdagceb", "ecadbg", "fgdc", "fageb", "bedcfa"},
{"fgead", "bcdafg", "ba", "bcae", "edafb", "bcefad", "dbcfe", "cafbgde", "gbcdef", "adb"},
{"gfeab", "abc", "bc", "ecbf", "gdebaf", "gebac", "gdcea", "gecdfba", "acdfbg", "fbgcae"},
{"aecgf", "acbf", "cgadef", "fecbg", "bc", "ebc", "dcegba", "befdg", "aebgcf", "afdebgc"},
{"fg", "facg", "efcagd", "eadfg", "cebfda", "feg", "daegb", "bdfcge", "afced", "cfeadgb"},
{"gefda", "fed", "edfbgc", "fgeab", "cfad", "cdeag", "fd", "cebgad", "dcefabg", "fagdce"},
{"ebfad", "cf", "befca", "gabedc", "fac", "cabeg", "dcaefg", "abfecg", "fbacged", "gcbf"},
{"aegbf", "cbdfega", "adbfg", "ef", "cebdgf", "dfcbga", "gebca", "fadgbe", "afde", "gfe"},
{"cdgab", "cg", "edgbac", "gbedfa", "gebcdaf", "dcebfg", "gcd", "degba", "aceg", "dacfb"},
{"daebfg", "fcebgad", "db", "gacbf", "feacdg", "gafdb", "eafdbc", "dgaef", "dfb", "bedg"},
{"ecfd", "fcdgba", "cebgf", "fge", "bgafed", "febdcg", "cbfegad", "cabeg", "fe", "gfdbc"},
{"fac", "gbeaf", "bcdga", "adgbcef", "febcda", "cfgba", "gefbda", "bagcfe", "cfge", "fc"},
{"afegdcb", "fdgba", "bfgacd", "fdcg", "fd", "cbfga", "adf", "ebcagf", "dcfabe", "gbdae"},
{"cdbgef", "bgef", "ge", "ged", "badec", "agfdcb", "adgecf", "cgdeb", "cbgdf", "fdbceag"},
{"dfbea", "gb", "cfgea", "bag", "fgbc", "debcga", "acdbgef", "afcgbe", "gfeba", "cfdaeg"},
{"fdgcab", "cd", "ebdaf", "baecdgf", "cda", "bcgaf", "cfebga", "acebgd", "cbdaf", "cfgd"},
{"fcbga", "cbeagf", "dcabg", "da", "dfabcg", "bdaf", "begcd", "dca", "fdagce", "bcgfdae"},
{"degacbf", "cf", "dcgfba", "adegbf", "efgab", "gfce", "acfeb", "dbeca", "caf", "gcebaf"},
{"bcaefd", "gbce", "dcegfb", "gcbadf", "dgfaecb", "fbdce", "dgcfe", "cg", "agfed", "fcg"},
{"cdfag", "dgebacf", "bdfagc", "fba", "acdgef", "fcdab", "bfdg", "baedc", "gbfaec", "fb"},
{"bcdgf", "eafcbgd", "bd", "adfb", "dcabeg", "gebfac", "gcfba", "gfdacb", "fdgce", "bgd"},
{"cdgafe", "deg", "bedfc", "dfgb", "cgdbe", "dg", "dcefba", "efdgbc", "cagbefd", "cegab"},
{"bg", "febdca", "edgcb", "cdeab", "gbc", "edgbca", "gdab", "beagcf", "cdegf", "debacfg"},
{"dbca", "cgaedb", "gcfbe", "cea", "fadbeg", "gabce", "ca", "edcafg", "degab", "abfedgc"},
{"gcdfe", "adbfgec", "cbafge", "dgbaf", "ebf", "gfbced", "eb", "adcgfe", "becd", "fgebd"},
{"dbgeafc", "dacbg", "badfge", "abfce", "ed", "eda", "dcfe", "decba", "dcfeab", "gfbeca"},
{"efcd", "bfcadge", "fcdba", "egabf", "bfdcag", "dcefba", "deagbc", "fabed", "ed", "edb"},
{"dcgf", "fegdab", "bcaeg", "dcegaf", "dga", "gd", "dcfbea", "fcdea", "aedcg", "bdefgca"},
{"bf", "edfbgc", "cegbf", "bcadefg", "ebf", "acbfde", "gcedf", "feadgc", "gbcae", "gfbd"},
{"efgba", "eacd", "efadcg", "dcgefb", "gacdf", "dbcfgae", "efd", "bgcdaf", "edfga", "ed"},
{"afgdc", "fbg", "cdgaef", "fagbdc", "bgcefa", "cdbf", "bgdaf", "bdaeg", "fb", "dgabcfe"},
{"cadgfe", "acbed", "cfabgde", "fdbcge", "cfga", "fad", "fdeabg", "fa", "dcaef", "dcfge"},
{"agdefc", "cfad", "bdgecfa", "gadfe", "aefgc", "ad", "ceadgb", "gbafce", "bfedg", "agd"}
};

static std::vector<std::vector<std::string>> inputResults = {
{"bgacf", "afdebc", "fceda", "cabfg"},
{"gdbefac", "fgabcd", "dcbefag", "aedgfb"},
{"ceafbg", "acb", "egbad", "gfaebc"},
{"gfacdeb", "gabdec", "begcd", "adbce"},
{"faecg", "cb", "cb", "fecbga"},
{"cfegb", "bga", "edfga", "fgbec"},
{"facgb", "fbdcge", "gf", "gfabc"},
{"egbcdfa", "afdebcg", "aecfbd", "acefb"},
{"fgdb", "gefba", "gcdafe", "bg"},
{"cdbgea", "fged", "cfabed", "bgd"},
{"gcbadf", "egafb", "ebafg", "ab"},
{"bcefg", "eagdf", "ab", "bacefd"},
{"ebd", "cdfe", "bgeaf", "cdbfea"},
{"fdage", "edbgac", "cgaebd", "bed"},
{"agdcf", "bgfda", "afdgc", "afdgb"},
{"acdbgf", "cbegad", "cgdaf", "eag"},
{"aedfb", "gb", "eafdb", "dbafe"},
{"efacgd", "dbefag", "fadebg", "bgfad"},
{"afbedc", "gcdeb", "fbgadc", "dbf"},
{"afcg", "cedba", "aecdb", "cgeba"},
{"baecdg", "fcbag", "dfeagb", "gcde"},
{"egcaf", "cfgae", "efcgd", "de"},
{"bfa", "bgadfc", "adcef", "ab"},
{"efgcd", "cgdfe", "cefdg", "cdf"},
{"fbgec", "bgdf", "efgcbda", "cbedg"},
{"bfcaeg", "gfedcab", "acbfge", "fe"},
{"bcadeg", "bf", "facbdeg", "gdabef"},
{"gdcab", "agcbd", "dafbecg", "gfa"},
{"bdaef", "egfdcab", "eb", "be"},
{"degbcf", "baegdf", "gaedbf", "dfegbc"},
{"gfadbec", "fgcda", "edgbac", "fbagd"},
{"bdaeg", "bdfac", "bdfac", "bcgad"},
{"daf", "fa", "afd", "efacbd"},
{"gebaf", "aceb", "gfdecb", "befga"},
{"dba", "dcgb", "ecbdfga", "abfcd"},
{"fedc", "fced", "adebf", "fbacde"},
{"ag", "bcadeg", "dgcafb", "cefdg"},
{"gcebfda", "gdecfba", "dbgfec", "defca"},
{"bafgecd", "fgabe", "aegbcf", "badec"},
{"bcgf", "bdceaf", "dbfaec", "bdacf"},
{"gcbfea", "fceda", "dcfgb", "dfgbc"},
{"fdeca", "gbcdf", "cfdgb", "afg"},
{"eb", "eab", "bea", "be"},
{"adefgc", "cdab", "fbead", "dcba"},
{"geafb", "fedba", "dbcage", "egdf"},
{"bcdgaf", "cedfga", "dbgafc", "dgfbc"},
{"beadcgf", "dgacf", "adfgc", "bc"},
{"fg", "fecdab", "bdefg", "bgfde"},
{"efdacb", "efadcgb", "dagf", "dfabe"},
{"badecf", "badefgc", "bfaec", "cbeaf"},
{"cefdab", "cdfage", "bacdf", "dbafc"},
{"gbdaef", "caedfg", "cf", "gbdefc"},
{"gfed", "fcbgd", "cdgfbe", "cfbeg"},
{"bagf", "ecfdg", "fbe", "bedagc"},
{"ea", "gceadb", "gcbad", "cbdga"},
{"daecg", "fdecga", "edafc", "adbgce"},
{"dge", "egcad", "ged", "cgdfa"},
{"aecgfd", "bfaec", "gf", "dceabg"},
{"gedfcb", "edcagfb", "cdega", "becgfd"},
{"dgec", "dfe", "ecbgfa", "cebgaf"},
{"acbeg", "dbc", "gdfb", "gfdb"},
{"gafb", "dgafeb", "gbaf", "fadegb"},
{"cbg", "cbg", "cfedgba", "dgcfabe"},
{"gfcde", "dgfbca", "dcabgf", "bacegd"},
{"ecadfb", "gfeca", "efa", "gefdc"},
{"cegbaf", "ecfdba", "cgbda", "dfebca"},
{"beagcd", "dgbca", "eabd", "ceabgd"},
{"feb", "gaef", "badgec", "ecgabf"},
{"gbedf", "edgbf", "bcdeg", "gfadbe"},
{"gbd", "bgd", "bcgfa", "fbgad"},
{"acef", "afedgb", "bgdfcea", "fgedab"},
{"aebfc", "egcb", "acfgbe", "gebc"},
{"edacf", "bf", "cfb", "gadcb"},
{"bcefa", "cfdba", "agdfc", "fabdc"},
{"fdbg", "bg", "caegf", "egfab"},
{"gdbeaf", "agbcf", "dcbagf", "fgadeb"},
{"ef", "ef", "ecdgfab", "deafb"},
{"efgcdb", "efg", "agcfeb", "dgfabc"},
{"agbdce", "aecbfg", "cdgaf", "ef"},
{"fcbeg", "gcdfb", "cgbdf", "acfdg"},
{"cegfb", "cafde", "ba", "fegdcba"},
{"cgfbd", "cfgdb", "fdegb", "cg"},
{"ecdag", "eb", "befd", "dabcfge"},
{"bfgae", "cfga", "ecabf", "aegbcf"},
{"fgadc", "bfg", "fb", "bf"},
{"da", "dace", "dga", "gedfab"},
{"ce", "adefgc", "cdea", "fce"},
{"adeb", "edg", "cgadf", "agdec"},
{"egbcf", "fbgec", "acedg", "af"},
{"bec", "eadgb", "eadbg", "badefgc"},
{"fec", "dafebc", "cfe", "aefdb"},
{"efcba", "baefc", "afbcg", "gbcfa"},
{"fg", "decfab", "cfdbga", "baecg"},
{"gcbaed", "fb", "bgfcae", "bcfg"},
{"bdfgca", "bgf", "fgdbce", "becfadg"},
{"gcebfa", "fgacbe", "fgbdae", "fcde"},
{"fbgdca", "ab", "afedgc", "ceafbd"},
{"fgeb", "gadbf", "gcafd", "bf"},
{"adfbe", "cfdage", "efdcga", "gfab"},
{"decbgfa", "fg", "ecgad", "afdbgce"},
{"dafecbg", "eagdb", "bec", "begdc"},
{"afcgb", "ecdg", "bcfag", "caedfg"},
{"agf", "ecfa", "becfg", "af"},
{"fba", "ceadgbf", "bf", "fb"},
{"afbegcd", "fegdac", "cebad", "bgafdc"},
{"fdgabc", "fegadb", "dgbcfea", "egabfd"},
{"aefcdbg", "cdegaf", "gdbec", "dcfae"},
{"fcbe", "gbaed", "fcaedb", "acedgf"},
{"cgafb", "ebcg", "aecfg", "efgbac"},
{"cbgde", "faegc", "befgdca", "cbdf"},
{"gfba", "eagdfcb", "dcabeg", "cegafd"},
{"ebf", "egafb", "fbgdac", "egafb"},
{"agcefd", "cbdfe", "dg", "bcedf"},
{"bea", "eb", "ecfbag", "abfcg"},
{"cegdfb", "gbeacd", "cd", "cabdge"},
{"cfabged", "ac", "cbag", "dfceab"},
{"efa", "adfcbge", "dfba", "ebgac"},
{"cdb", "bdc", "dc", "aecbd"},
{"cbeag", "edca", "gdbea", "fbedgc"},
{"bgecd", "gf", "bacef", "fg"},
{"adecb", "gfaebc", "fdcbga", "aecdb"},
{"cfade", "cbeadg", "bgfea", "dg"},
{"fbegc", "fabge", "af", "dcgabe"},
{"fdabce", "gbcfd", "cbefda", "ecabdf"},
{"ga", "ag", "bdaefc", "ga"},
{"gbdefa", "fedgc", "cafedb", "cfdge"},
{"dbe", "gabfced", "cgbedf", "cbadg"},
{"bagdc", "decf", "fca", "dfbae"},
{"eb", "edagbf", "ebfc", "eagdcf"},
{"aegdc", "dfabge", "gcade", "daceg"},
{"cdbaf", "cgeadb", "bdcae", "bfgaed"},
{"ecfg", "fabgcde", "gdecbaf", "bacged"},
{"dcfgab", "bgcefa", "gad", "gafcbd"},
{"ab", "dgefb", "baf", "bgaedcf"},
{"dagbe", "fgdbeca", "cbagdfe", "fcbeda"},
{"bagfdce", "dcgafe", "fegbc", "dbegca"},
{"agdcf", "badeg", "fb", "fabcge"},
{"eafdb", "edfcbag", "dcfbe", "ba"},
{"deafc", "acb", "fbgaecd", "cfdea"},
{"gd", "edfba", "fgbce", "fbgec"},
{"cgd", "dgcae", "gcead", "cd"},
{"bc", "afdegb", "bgdc", "edbafg"},
{"bagde", "eb", "egcb", "edfbac"},
{"cbefa", "fg", "bdega", "gfacbed"},
{"begacf", "agb", "cgfade", "fcdga"},
{"cfbdag", "dcgba", "geabdf", "eadc"},
{"dec", "fbadec", "debac", "edc"},
{"efacd", "ge", "cfgabde", "fagecdb"},
{"bfaced", "gedcf", "dbag", "cgbdaf"},
{"abgcde", "ebcga", "dagb", "bgcefa"},
{"aec", "cageb", "ecdgfb", "egda"},
{"agfdb", "cgfba", "bgafc", "eacgdf"},
{"fgdba", "abegdfc", "bcadgf", "fbegd"},
{"fcad", "bfdgae", "dgc", "eagbc"},
{"cfgdb", "gcedf", "deaf", "cfdbg"},
{"abcgf", "cabgf", "feagdcb", "cdbaefg"},
{"bfd", "cgfdeba", "cafdeb", "gedfa"},
{"fegb", "cagdeb", "ecdgab", "bdagefc"},
{"fg", "egf", "beafd", "efg"},
{"bgfa", "dbagc", "bfgdca", "bcgad"},
{"eadbgc", "cb", "ebadgf", "debc"},
{"abecgf", "bgac", "gacb", "ceadbf"},
{"bgac", "bcga", "cagb", "efgbda"},
{"adbgce", "dfcaeb", "bgfc", "abcged"},
{"cfagdb", "edcafg", "cbfgda", "bgfed"},
{"gaf", "bcgf", "ecbagd", "agf"},
{"egcf", "fabdc", "adebgf", "bdcage"},
{"adgcbe", "aefgdcb", "gabfe", "gc"},
{"dcfeb", "gecdfb", "caeb", "bacdfe"},
{"cbdagf", "gbcae", "bc", "edgbaf"},
{"ecb", "bcgfe", "bgedf", "abcf"},
{"acdfe", "fgeda", "fg", "cdbegf"},
{"faegdc", "eafdg", "fd", "cgbfdea"},
{"bfade", "fcbg", "fc", "cgbf"},
{"fdae", "afcbgd", "bgcedaf", "gef"},
{"gdc", "cgdba", "cdg", "gbadc"},
{"cefdbag", "gedfca", "gafdeb", "befcda"},
{"ecgdfb", "edfc", "gebdaf", "fecd"},
{"gcfe", "gbcaf", "acfbeg", "acdgb"},
{"df", "bcaefd", "cfagb", "df"},
{"dcfgb", "gcebd", "egbf", "dcegb"},
{"beafd", "bag", "cbgf", "gba"},
{"aegcfb", "cdgf", "cdgf", "afegbdc"},
{"acgbd", "dbceg", "acd", "cfbga"},
{"bacfe", "abfecg", "ebafc", "debac"},
{"dbgfec", "gc", "acdgfb", "efbcd"},
{"cedbafg", "fb", "dgfeac", "cadbf"},
{"cfbga", "fgcab", "db", "fcegd"},
{"bgdf", "abfdec", "egd", "gedbfc"},
{"bcaedf", "dgcef", "badg", "fecadbg"},
{"fdebag", "beacg", "abcd", "adcegb"},
{"ecgfba", "gcbfde", "dbec", "cdafge"},
{"febcda", "defc", "beacf", "fdgabe"},
{"gbeacd", "fdeab", "ebcfad", "fageb"},
{"cbegdfa", "eabcfd", "adfce", "cbefda"},
{"feb", "dfgb", "egbcdfa", "dfegc"},
{"cfbgda", "dgcfa", "gcfead", "eadfg"},
{"dagfc", "cefagb", "egcabf", "fcbd"},
{"fad", "bafgecd", "cfegad", "adbfcge"},
{"dag", "ad", "fcage", "cgbafe"}
};

int main()
{
    if ( !testAlg() ) {
        return 1;
    }
    std::vector<int> decodedResults;
    size_t sum = 0;
    for ( size_t i = 0; i < inputSegments.size(); ++i )
    {
        if ( calcSegmentMap(inputSegments[i],
                inputResults[i],
                decodedResults) )
        {
            std::cout << "Result: ";
            for ( int j : decodedResults )
            {
                std::cout << j << ", ";
            }
            int combined = intVecToInt( decodedResults );
            std::cout << "Int result: " << combined << std::endl;
            sum += combined;
        }
    }
    std::cout << sum << std::endl;
    return 0;
}

