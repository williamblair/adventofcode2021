#include <iostream>
#include <map>
#include <queue>
#include <cstdint>
#include <string>

struct PolyLinkedList
{
  char val;
  PolyLinkedList* next;

  PolyLinkedList() :
    val( '.' ),
    next( nullptr )
  {}

  PolyLinkedList( char val, PolyLinkedList* next ) :
    val( val ),
    next( next )
  {}
};

static void initList( const std::string& init, PolyLinkedList* head )
{
  PolyLinkedList* node = head;
  for ( const char c : init )
  {
    node->val = c;
    node->next = new PolyLinkedList();
    node = node->next;
    node->next = nullptr;
  }
}

static void freeList( PolyLinkedList* head )
{
  PolyLinkedList* node = head;
  while ( node != nullptr )
  {
    PolyLinkedList* tmp = node->next;
    delete node;
    node = tmp;
  }
}

static void printList( PolyLinkedList* head )
{
  PolyLinkedList* node = head;
  while ( node != nullptr && node->val != '.' )
  {
    std::cout << node->val;
    node = node->next;
  }
  std::cout << std::endl;
}

/* Pair entry 1 = the list entry to insert the next entry at 
 * Pair entry 2 = the new entry to put in the list */
using PolyInsertQueue = std::queue<std::pair<PolyLinkedList*,PolyLinkedList*>>;
static void genPairInsertQueue(
  std::map<std::string,char>& pairMap,
  PolyLinkedList* curListHead,
  PolyInsertQueue& result )
{
  for ( auto& mapEntry : pairMap )
  {
    /* Search for matching patterns */
    PolyLinkedList* node = curListHead;
    PolyLinkedList* nextNode = node->next;
    while ( node != nullptr && nextNode != nullptr )
    {
      std::string listPairStr = "";
      listPairStr += node->val;
      listPairStr += nextNode->val;

      /* If the pattern matches,
       * generate a new node and place it in the queue */
      if ( listPairStr == mapEntry.first ) {
        PolyLinkedList* newEntry = new PolyLinkedList();
        newEntry->val = mapEntry.second;
        newEntry->next = nextNode;
        result.emplace( node, newEntry );
      }

      node = node->next;
      nextNode = nextNode->next;
    }
  }
}

static inline void insertNewNode(
  std::pair<PolyLinkedList*,PolyLinkedList*>& pair,
  PolyLinkedList* head )
{
  //PolyLinkedList* node = head;
  //while ( node != nullptr )
  //{
    //if ( node == pair.first )
    //{
    //  node->next = pair.second;
    //  return;
    //}
    //node = node->next;
  //}
  //std::cout << "ERROR - failed to find matching node in insertNewNode"
  //          << std::endl;
  pair.first->next = pair.second;
}

static void applyPolyInsertQueue(
  PolyInsertQueue& queue,
  PolyLinkedList* head )
{
  while ( !queue.empty() )
  {
    auto& pair = queue.front();
    insertNewNode( pair, head );
    queue.pop();
  }
}

static uint64_t calcGreatestLeastQuantityDiff( PolyLinkedList* head )
{
  std::map<char,uint64_t> charCounts;
  PolyLinkedList* node = head;
  /* Make map of counts */
  while ( node != nullptr && node->val != '.' )
  {
    if ( charCounts.find( node->val ) != charCounts.end() ) {
      charCounts[node->val]++;
    }
    else {
      charCounts[node->val] = 0;
    }
    node = node->next;
  }

  /* Find the greatest and least */
  char maxChar = '.';
  uint64_t maxCount = 0;
  char minChar = '.';
  uint64_t minCount = 0xFFFFFFFFFFFFFFFF;
  for ( auto& pair : charCounts )
  {
    if ( pair.second > maxCount ) {
      maxCount = pair.second;
      maxChar = pair.first;
    }
    if ( pair.second < minCount ) {
      minCount = pair.second;
      minChar = pair.first;
    }
  }

  /* Calc the difference */
  return maxCount - minCount;
}

static bool testAlg()
{
  std::string testBase = "NNCB";

  std::map<std::string,char> testPolyMap = {
    {"CH",'B'},
    {"HH",'N'},
    {"CB",'H'},
    {"NH",'C'},
    {"HB",'C'},
    {"HC",'B'},
    {"HN",'C'},
    {"NN",'C'},
    {"BH",'H'},
    {"NC",'B'},
    {"NB",'B'},
    {"BN",'B'},
    {"BB",'N'},
    {"BC",'B'},
    {"CC",'N'},
    {"CN",'C'}
  };

  PolyLinkedList* listHead = new PolyLinkedList();
  initList( testBase, listHead );
  printList( listHead );

  for ( size_t i = 0; i < 40; ++i )
  {
    PolyInsertQueue insertQueue;
    genPairInsertQueue( testPolyMap, listHead, insertQueue );
    applyPolyInsertQueue( insertQueue, listHead );
    //printList( listHead );
  }

  bool retVal = true;
  uint64_t countDiff = calcGreatestLeastQuantityDiff( listHead );
  if ( countDiff != 2188189693529 ) {
    std::cout << "Max Min count diff failed (expected 2188189693529): "
              << countDiff
              << std::endl;
    retVal = false;
  }
  else {
    std::cout << "Test passed" << std::endl;
  }

  freeList( listHead );
  listHead = nullptr;

  return retVal;
}

static std::string inputBase = "PSVVKKCNBPNBBHNSFKBO";

static std::map<std::string,char> inputPolyMap = {
{"CF",'H'},
{"PP",'H'},
{"SP",'V'},
{"NO",'C'},
{"SF",'F'},
{"FS",'H'},
{"OF",'P'},
{"PN",'B'},
{"SH",'V'},
{"BO",'K'},
{"ON",'V'},
{"VP",'S'},
{"HN",'B'},
{"PS",'P'},
{"FV",'H'},
{"NC",'N'},
{"FN",'S'},
{"PF",'F'},
{"BF",'F'},
{"NB",'O'},
{"HS",'C'},
{"SC",'V'},
{"PC",'K'},
{"KF",'K'},
{"HC",'C'},
{"OK",'H'},
{"KS",'P'},
{"VF",'C'},
{"NV",'S'},
{"KK",'F'},
{"HV",'H'},
{"SV",'V'},
{"KC",'N'},
{"HF",'P'},
{"SN",'F'},
{"VS",'P'},
{"VN",'F'},
{"VH",'C'},
{"OB",'K'},
{"VV",'O'},
{"VC",'O'},
{"KP",'V'},
{"OP",'C'},
{"HO",'S'},
{"NP",'K'},
{"HB",'C'},
{"CS",'S'},
{"OO",'S'},
{"CV",'K'},
{"BS",'F'},
{"BH",'P'},
{"HP",'P'},
{"PK",'B'},
{"BB",'H'},
{"PV",'N'},
{"VO",'P'},
{"SS",'B'},
{"CC",'F'},
{"BC",'V'},
{"FF",'S'},
{"HK",'V'},
{"OH",'N'},
{"BV",'C'},
{"CP",'F'},
{"KN",'K'},
{"NN",'S'},
{"FB",'F'},
{"PH",'O'},
{"FH",'N'},
{"FK",'P'},
{"CK",'V'},
{"CN",'S'},
{"BP",'K'},
{"CH",'F'},
{"FP",'K'},
{"HH",'N'},
{"NF",'C'},
{"VB",'B'},
{"FO",'N'},
{"PB",'C'},
{"KH",'K'},
{"PO",'K'},
{"OV",'F'},
{"NH",'H'},
{"KV",'B'},
{"OS",'K'},
{"OC",'K'},
{"FC",'H'},
{"SO",'H'},
{"KO",'P'},
{"NS",'F'},
{"CB",'C'},
{"CO",'F'},
{"KB",'V'},
{"BK",'K'},
{"NK",'O'},
{"SK",'C'},
{"SB",'B'},
{"VK",'O'},
{"BN",'H'}
};

int main()
{
  if ( !testAlg() ) {
    return 1;
  }

  PolyLinkedList* listHead = new PolyLinkedList();
  initList( inputBase, listHead );
  printList( listHead );

  for ( size_t i = 0; i < 40; ++i )
  {
    PolyInsertQueue insertQueue;
    genPairInsertQueue( inputPolyMap, listHead, insertQueue );
    applyPolyInsertQueue( insertQueue, listHead );
    //printList( listHead );
  }

  uint64_t countDiff = calcGreatestLeastQuantityDiff( listHead );
  std::cout << "Count Diff: " << countDiff << std::endl;

  freeList( listHead );
  listHead = nullptr;
  return 0;
}

