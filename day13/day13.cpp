#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <cstring>
#include <cstdint>

static void fillPaper(
    const std::vector<std::pair<uint64_t,uint64_t>>& input,
    uint64_t& numRows,
    uint64_t& numCols,
    std::vector<std::vector<bool>>& result )
{
    /* Find max rows, cols */
    uint64_t maxRow = 0;
    uint64_t maxCol = 0;
    for ( auto& pair : input )
    {
        if ( pair.second > maxRow ) {
            maxRow = pair.second;
        }
        if ( pair.first > maxCol ) {
            maxCol = pair.first;
        }
    }
    numRows = maxRow+1;
    numCols = maxCol+1;
    result.resize( numRows );
    for ( auto& row : result )
    {
        row.assign( numCols, false );
    }

    /* Now set points to true */
    for ( auto& pair : input )
    {
        result[pair.second][pair.first] = true;
    }
}

/* Updates paper, numRows, and numCols */
static void foldAlongY(
    const uint64_t foldRow,
    std::vector<std::vector<bool>>& paper,
    uint64_t& numRows,
    uint64_t& numCols )
{
    const int64_t startRow = foldRow + 1;
    const int64_t startResultRow = foldRow - 1;
    for ( int64_t copyRow = startRow, resultRow = startResultRow;
            copyRow < numRows && resultRow >= 0;
            ++copyRow, --resultRow )
    {
        for ( int64_t col = 0; col < numCols; ++col )
        {
            const bool copyVal = paper[copyRow][col];
            if ( copyVal ) {
                paper[resultRow][col] = true;
            }
        }
    }
    numRows = foldRow;
}

/* Updates paper, numRows, and numCols */
static void foldAlongX(
    const uint64_t foldCol,
    std::vector<std::vector<bool>>& paper,
    uint64_t& numRows,
    uint64_t& numCols )
{
    const int64_t startCol = foldCol + 1;
    const int64_t startResultCol = foldCol - 1;
    for ( int64_t copyCol = startCol, resultCol = startResultCol;
            copyCol < numCols && resultCol >= 0;
            ++copyCol, --resultCol )
    {
        for ( int64_t row = 0; row < numRows; ++row )
        {
            const bool copyVal = paper[row][copyCol];
            if ( copyVal ) {
                paper[row][resultCol] = true;
            }
        }
    }
    numCols = foldCol;
}

static inline void parseCommand(
    const std::string& command,
    char& resultXY,
    uint64_t& resultVal )
{
    sscanf( command.c_str(),
            "fold along %c=%llu",
            &resultXY,
            &resultVal
    );
}

static inline uint64_t calcNumDots(
    std::vector<std::vector<bool>>& paper,
    const uint64_t numRows,
    const uint64_t numCols )
{
    uint64_t numDots = 0;
    for ( uint64_t row = 0; row < numRows; ++row )
    {
        for ( uint64_t col = 0; col < numCols; ++col )
        {
            const bool isMarked = paper[row][col];
            numDots += (uint64_t)isMarked;
        }
    }
    return numDots;
}

static inline void printPaper(
    std::vector<std::vector<bool>>& paper,
    const uint64_t numRows,
    const uint64_t numCols )
{
    for ( uint64_t row = 0; row < numRows; ++row )
    {
        for ( uint64_t col = 0; col < numCols; ++col )
        {
            const bool isMarked = paper[row][col];
            std::cout << (isMarked ? '#' : '.');
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

static bool testAlg()
{
    uint64_t numRows;
    uint64_t numCols;
    std::vector<std::vector<bool>> paper;

    std::vector<std::pair<uint64_t,uint64_t>> testPoints = {
        {6,10},
        {0,14},
        {9,10},
        {0,3},
        {10,4},
        {4,11},
        {6,0},
        {6,12},
        {4,1},
        {0,13},
        {10,12},
        {3,4},
        {3,0},
        {8,4},
        {1,10},
        {2,14},
        {8,10},
        {9,0}
    };
    std::vector<std::string> testCommands = {
        "fold along y=7",
        "fold along x=5"
    };

    fillPaper( testPoints, numRows, numCols, paper );
    
    {
        char cmdXY;
        uint64_t rowCol; // row or col depending on cmd X or Y
        parseCommand( testCommands[0], cmdXY, rowCol );
        if ( cmdXY == 'y' ) {
            foldAlongY( rowCol, paper, numRows, numCols );
        }
        else {
            foldAlongX( rowCol, paper, numRows, numCols );
        }
        const uint64_t numDots = calcNumDots( paper, numRows, numCols );
        if ( numDots != 17 ) {
            std::cout << "Num Dots calc failed (expected 17): "
                << numDots
                << std::endl;
            printPaper( paper, numRows, numCols );
            return false;
        }

        parseCommand( testCommands[1], cmdXY, rowCol );
        if ( cmdXY == 'y' ) {
            foldAlongY( rowCol, paper, numRows, numCols );
        }
        else {
            foldAlongX( rowCol, paper, numRows, numCols );
        }
        printPaper( paper, numRows, numCols );
    }

    return true;
}

static std::vector<std::pair<uint64_t,uint64_t>> inputPoints = {
{218,593},
{1176,617},
{57,515},
{806,674},
{517,740},
{402,553},
{1118,53},
{596,577},
{473,147},
{166,817},
{149,154},
{209,228},
{170,159},
{124,376},
{960,481},
{107,73},
{646,313},
{708,393},
{1215,696},
{706,780},
{969,149},
{686,189},
{552,771},
{523,322},
{1058,789},
{155,205},
{616,782},
{113,154},
{1017,280},
{15,352},
{774,623},
{649,633},
{435,591},
{353,638},
{273,327},
{1139,180},
{763,821},
{360,506},
{219,432},
{656,770},
{499,124},
{1081,691},
{1079,68},
{1012,204},
{1146,665},
{669,421},
{192,558},
{92,873},
{626,477},
{1250,435},
{65,99},
{790,36},
{1208,795},
{1290,634},
{344,229},
{1171,43},
{179,154},
{75,316},
{398,788},
{725,80},
{793,694},
{1146,217},
{179,740},
{848,841},
{972,100},
{1010,313},
{495,399},
{410,4},
{442,315},
{1222,75},
{458,768},
{10,100},
{683,537},
{994,570},
{771,91},
{1240,330},
{417,201},
{1280,828},
{462,725},
{795,204},
{833,697},
{1206,357},
{164,217},
{438,312},
{161,621},
{1154,497},
{910,49},
{236,704},
{428,350},
{338,100},
{1215,870},
{589,219},
{956,245},
{1299,752},
{1139,409},
{736,750},
{70,564},
{1266,847},
{20,219},
{1056,78},
{688,488},
{326,376},
{445,666},
{1086,829},
{1155,205},
{715,138},
{484,455},
{1010,649},
{226,544},
{460,710},
{1032,616},
{811,385},
{756,126},
{644,655},
{518,693},
{97,451},
{671,800},
{624,705},
{336,303},
{468,294},
{356,652},
{651,495},
{869,546},
{174,642},
{1068,705},
{708,365},
{566,733},
{950,164},
{266,843},
{435,562},
{391,873},
{391,294},
{75,764},
{38,112},
{388,771},
{149,201},
{375,635},
{972,794},
{343,542},
{1299,142},
{1103,525},
{192,753},
{1063,441},
{989,709},
{8,329},
{316,794},
{524,96},
{666,794},
{822,705},
{256,442},
{708,281},
{338,207},
{31,273},
{994,239},
{1203,73},
{170,567},
{231,68},
{1028,259},
{1138,102},
{1079,480},
{944,289},
{714,129},
{935,635},
{977,261},
{443,54},
{723,508},
{376,651},
{1032,894},
{845,807},
{237,621},
{775,530},
{393,123},
{1006,413},
{465,87},
{579,578},
{22,600},
{580,341},
{284,23},
{328,128},
{244,627},
{10,794},
{433,597},
{684,240},
{875,751},
{994,155},
{137,304},
{984,742},
{441,702},
{1079,414},
{776,142},
{656,670},
{1163,819},
{321,633},
{524,315},
{65,219},
{15,94},
{1196,782},
{1250,883},
{136,649},
{977,709},
{709,887},
{338,879},
{1144,77},
{801,210},
{1196,392},
{1057,241},
{391,425},
{602,393},
{643,590},
{669,429},
{1039,544},
{433,297},
{1198,281},
{408,638},
{1220,190},
{910,221},
{254,845},
{430,829},
{790,277},
{758,381},
{758,323},
{323,467},
{500,291},
{520,858},
{951,808},
{114,782},
{186,600},
{391,469},
{1176,57},
{894,628},
{1176,277},
{179,787},
{8,113},
{795,690},
{290,707},
{10,465},
{1170,93},
{301,591},
{982,766},
{1120,649},
{813,633},
{1174,649},
{887,869},
{806,884},
{361,301},
{383,350},
{336,79},
{236,515},
{473,528},
{333,521},
{1258,689},
{1203,521},
{875,487},
{498,371},
{300,313},
{244,267},
{972,330},
{770,782},
{644,476},
{976,887},
{641,17},
{149,714},
{208,355},
{902,638},
{887,826},
{1302,815},
{518,439},
{803,292},
{932,429},
{124,638},
{997,730},
{459,98},
{1243,609},
{505,180},
{623,796},
{539,257},
{694,179},
{1056,721},
{641,473},
{416,628},
{1284,537},
{1280,66},
{172,792},
{402,789},
{1252,544},
{1174,439},
{11,752},
{1158,291},
{1240,620},
{723,386},
{244,4},
{892,621},
{58,765},
{818,236},
{1017,301},
{753,651},
{1148,0},
{1235,764},
{527,816},
{115,637},
{999,787},
{1006,481},
{500,739},
{137,416},
{1000,229},
{997,25},
{313,25},
{508,229},
{1156,326},
{639,516},
{672,411},
{328,376},
{930,134},
{508,341},
{226,350},
{1161,180},
{458,763},
{900,420},
{825,37},
{728,704},
{1084,544},
{792,649},
{808,575},
{872,312},
{113,516},
{48,488},
{831,640},
{298,92},
{23,379},
{381,663},
{674,315},
{402,105},
{417,649},
{663,462},
{500,313},
{304,824},
{514,861},
{810,774},
{669,249},
{107,704},
{1092,593},
{969,297},
{492,336},
{147,819},
{792,693},
{1278,276},
{107,821},
{997,205},
{1285,396},
{728,67},
{704,894},
{1140,735},
{1121,292},
{423,205},
{102,430},
{408,644},
{194,184},
{294,612},
{810,333},
{582,827},
{1158,603},
{1262,488},
{874,719},
{728,827},
{318,143},
{492,457},
{336,43},
{940,871},
{515,466},
{293,525},
{1115,86},
{611,364},
{509,646},
{418,379},
{462,753},
{987,287},
{639,110},
{982,152},
{464,693},
{1086,571},
{917,123},
{1089,297},
{1121,87},
{238,628},
{370,870},
{1180,350},
{169,814},
{694,715},
{411,621},
{826,775},
{134,617},
{641,249},
{887,733},
{874,271},
{410,474},
{796,481},
{1300,465},
{21,399},
{606,198},
{837,147},
{16,411},
{975,14},
{694,826},
{638,483},
{475,107},
{1000,236},
{622,406},
{1031,707},
{279,707},
{666,655},
{837,528},
{763,143},
{418,256},
{957,254},
{864,427},
{668,296},
{1141,485},
{318,781},
{1124,801},
{207,369},
{1103,369},
{805,714},
{333,709},
{514,353},
{1289,399},
{820,889},
{1294,859},
{504,674},
{974,751},
{509,658},
{1196,868},
{1193,676},
{893,693},
{323,159},
{508,105},
{1196,726},
{962,186},
{957,638},
{1176,53},
{1037,215},
{1074,190},
{908,553},
{818,620},
{806,436},
{1297,197},
{671,107},
{152,603},
{147,75},
{930,760},
{917,347},
{682,357},
{13,136},
{956,649},
{1094,666},
{927,544},
{1203,780},
{1049,323},
{589,99},
{90,190},
{279,315},
{80,108},
{278,224},
{343,339},
{872,393},
{812,371},
{304,413},
{423,752},
{536,623},
{632,152},
{1277,299},
{1156,120},
{808,689},
{1047,152},
{380,312},
{1102,243},
{1280,466},
{786,756},
{606,616},
{1121,864},
{1171,690},
{1176,858},
{102,571},
{463,705},
{805,421},
{88,523},
{507,75},
{808,700},
{1208,717},
{1260,653},
{966,859},
{649,261},
{408,698},
{1208,8},
{276,465},
{623,124},
{48,539},
{186,93},
{852,518},
{1066,267},
{1039,246},
{688,618},
{341,205},
{731,578},
{624,369},
{716,11},
{509,864},
{925,735},
{313,369},
{1123,242},
{1154,103},
{237,273},
{793,154},
{458,824},
{1156,581},
{835,107},
{1280,156},
{703,539},
{310,236},
{1210,495},
{966,229},
{192,165},
{738,26},
{192,865},
{801,236},
{694,292},
{776,590},
{1115,98},
{134,841},
{328,152},
{1020,707},
{306,708},
{1300,239},
{1302,49},
{402,509},
{400,49},
{154,781},
{1294,411},
{202,627},
{801,478},
{137,694},
{840,555},
{801,248},
{252,105},
{354,581},
{666,239},
{48,691},
{1091,407},
{894,852},
{1170,801},
{624,189},
{770,840},
{666,877},
{934,24},
{504,884},
{194,812},
{502,351},
{1186,289},
{22,152},
{117,666},
{1210,47},
{669,17},
{917,508},
{428,768},
{540,782},
{48,0},
{812,343},
{1118,841},
{300,649},
{509,248},
{492,218},
{1020,187},
{869,254},
{20,260},
{1010,581},
{802,105},
{1051,485},
{1282,169},
{140,413},
{1146,453},
{238,42},
{540,502},
{842,801},
{1002,126},
{107,521},
{3,847},
{316,564},
{557,451},
{1002,19},
{783,78},
{929,663},
{462,296},
{1170,481},
{892,414},
{1146,441},
{622,40},
{736,556},
{1156,113},
{944,605},
{418,862},
{351,291},
{540,616},
{1154,819},
{149,869},
{1180,586},
{17,80},
{997,369},
{509,684},
{795,466},
{972,739},
{48,355},
{418,173},
{452,70},
{848,764},
{669,473},
{596,544},
{1148,894},
{32,58},
{567,81},
{169,528},
{274,582},
{156,551},
{1144,817},
{848,729},
{114,726},
{360,282},
{366,605},
{1054,452},
{117,676},
{970,212},
{554,126},
{758,464},
{190,439},
{316,15},
{294,724},
{760,357},
{1231,49},
{107,780},
{465,200},
{370,471},
{393,508},
{1056,49},
{934,870},
{336,781},
{1081,891},
{13,249},
{1238,221},
{682,693},
{114,294},
{1133,19},
{602,582},
{31,546},
{616,68},
{624,817},
{478,10},
{735,49},
{490,889},
{1295,110},
{637,366},
{1295,784},
{162,218},
{790,150},
{520,501},
{425,651},
{416,852},
{1279,796},
{867,840},
{162,889},
{803,75},
{477,19},
{443,502},
{55,544},
{410,627},
{410,715},
{880,795},
{186,189},
{79,397},
{1066,362},
{549,297},
{1124,413},
{815,831},
{391,742},
{616,715},
{247,441},
{935,259},
{974,113},
{1097,546},
{518,201},
{335,14},
{266,11},
{418,36},
{269,603},
{846,455},
{867,54},
{863,138},
{52,666},
{490,376},
{899,441},
{1034,465},
{1108,376},
{840,107},
{468,600},
{186,801},
{949,301},
{256,620},
{267,84},
{818,218},
{507,448},
{328,770},
{1081,443},
{1151,831},
{458,394},
{266,65},
{130,579},
{557,203},
{190,201},
{934,651},
{1196,698},
{316,155},
{171,572},
{535,530},
{753,443},
{801,684},
{932,465},
{1255,544},
{632,742},
{294,164},
{820,5},
{28,110},
{788,514},
{320,833},
{1017,614},
{1044,65},
{1310,222},
{1293,80},
{656,124},
{316,17},
{293,593},
{212,894},
{892,379},
{641,429},
{566,371},
{957,256},
{687,124},
{344,658},
{304,600},
{300,245},
{328,681},
{994,291},
{721,219},
{276,632},
{908,789},
{714,544},
{1191,532},
{929,231},
{676,675},
{852,70},
{927,350},
{882,544},
{244,420},
{1136,709},
{1009,143},
{964,126},
{912,106},
{1180,544},
{1210,847},
{1176,281},
{1102,355},
{1200,228},
{552,878},
{788,626},
{8,565},
{967,339},
{44,399},
{927,96},
{792,245},
{22,294},
{477,697},
{1118,878},
{974,815},
{316,330},
{136,21},
{468,93},
{1297,136},
{835,292},
{507,292},
{679,515},
{208,651},
{398,106},
{1252,129},
{960,152},
{587,386},
{502,82},
{445,228},
{1072,42},
{1051,857},
{647,462},
{919,21},
{114,698},
{8,49},
{698,442},
{242,705},
{694,68},
{994,330},
{0,661},
{1174,887},
{498,523},
{815,63},
{530,518},
{1161,197},
{1192,411},
{192,617},
{972,463},
{1196,294},
{55,256},
{134,822},
{146,665},
{426,514},
{639,94},
{547,73},
{1144,189},
{1146,336},
{196,192},
{636,875},
{758,8},
{1215,472},
{408,704},
{520,764},
{833,427},
{972,291},
{1200,540},
{629,807},
{900,250},
{190,649},
{802,341},
{954,802},
{622,488},
{1223,81},
{1215,422},
{231,689},
{177,19},
{1251,756},
{90,173},
{311,784},
{0,670},
{974,781},
{1049,508},
{149,473},
{923,362},
{273,231},
{646,329},
{505,421},
{1064,480},
{293,369},
{239,701},
{952,290},
{642,598},
{48,616},
{1154,551},
{736,338},
{589,795},
{115,803},
{1238,133},
{994,794},
{981,565},
{987,875},
{1268,136},
{418,858},
{1044,323},
{808,319},
{152,767},
{793,620},
{919,425},
{790,501},
{1043,362},
{110,540},
{616,292},
{1054,620},
{520,277},
{599,798},
{1052,836},
{836,217},
{659,47},
{887,434},
{417,693},
{1108,32},
{393,771},
{42,136},
{1091,880},
{146,546},
{48,406},
{666,17},
{331,399},
{338,291},
{336,403},
{1064,644},
{164,441},
{359,546},
{628,357},
{616,112},
{154,774},
{171,322},
{385,735},
{843,652},
{28,784},
{1173,304},
{930,312},
{1073,273},
{899,273},
{0,672},
{636,539},
{266,571},
{992,591},
{709,551},
{192,29},
{775,362},
{485,37},
{313,730},
{994,15},
{651,47},
{970,682},
{254,78},
{1064,414},
{552,886},
{313,205},
{858,267},
{130,138},
{687,796},
{112,281},
{1310,770},
{107,114},
{1300,794},
{644,346},
{1208,571},
{793,416},
{673,366},
{832,884},
{1074,484},
{962,708},
{1206,537},
{1136,642},
{552,323},
{721,99},
{359,348},
{676,708},
{154,313},
{1148,218},
{499,385},
{30,66},
{678,600},
{1028,635},
{502,689},
{932,17},
{1305,709},
{1076,508},
{435,143},
{1191,362},
{708,529},
{1041,603}
};

static std::vector<std::string> inputInstructions = {
"fold along x=655",
"fold along y=447",
"fold along x=327",
"fold along y=223",
"fold along x=163",
"fold along y=111",
"fold along x=81",
"fold along y=55",
"fold along x=40",
"fold along y=27",
"fold along y=13",
"fold along y=6"
};

int main()
{
    if ( !testAlg() ) {
        return 1;
    }
    uint64_t numRows;
    uint64_t numCols;
    std::vector<std::vector<bool>> paper;
    fillPaper( inputPoints, numRows, numCols, paper );
    {
        char cmdXY;
        uint64_t rowCol; // row or col depending on cmd X or Y
        parseCommand( inputInstructions[0], cmdXY, rowCol );
        if ( cmdXY == 'y' ) {
            foldAlongY( rowCol, paper, numRows, numCols );
        }
        else {
            foldAlongX( rowCol, paper, numRows, numCols );
        }
        const uint64_t numDots = calcNumDots( paper, numRows, numCols );
        std::cout << "Num Dots after first instr: " << numDots << std::endl;
    }
    return 0;
}

