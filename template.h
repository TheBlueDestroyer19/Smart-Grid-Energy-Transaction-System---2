#define MAX 4
#define MIN 2
#define MAX_LINE_LENGTH 256

typedef struct Transaction{
  unsigned int transID;
  unsigned int sellerID;
  unsigned int buyerID;
  float amtOfEnergy;
  float pricePerKWh;
  float total_price;
  char date[11];
  char time[6];
  struct Transaction* next;
  int flag;
}Transaction;

typedef struct TransactionTree{
  int numKeys;
  int is_leaf;
  struct TransactionTree* children[MAX+1];
  struct TransactionTree* next;
  union {
    unsigned int keys[MAX];
    Transaction trans[MAX];
  }data;
}TransactionTree;

typedef struct buyerNode{
  unsigned int buyerID;
  float total_energy;
  struct buyerNode* next;
}buyerNode;

typedef struct SellerData{
  unsigned int sellerID;
  float a300;
  float b300;
  float revenue;
  buyerNode* head;
  buyerNode* tail;
  TransactionTree* subtree;
}SellerData;

typedef struct SellerTree{
  int numKeys;
  int is_leaf;
  struct SellerTree* children[MAX+1];
  struct SellerTree* next;
  SellerData sellerdata[MAX];
}SellerTree;

typedef struct BuyerData{
  unsigned int buyerID;
  float total_energy;
  TransactionTree* subtree;
}BuyerData;

typedef struct BuyerTree{
  int numKeys;
  int is_leaf;
  struct BuyerTree* children[MAX+1];
  struct BuyerTree* next;
  unsigned int keys[MAX];
  BuyerData buyerdata[MAX];
}BuyerTree;

typedef struct pair{
  unsigned int buyerID;
  unsigned int sellerID;
  int count;
  int flag;
  struct pair* next;
}Pair;

TransactionTree* addTransaction(TransactionTree* , SellerTree*, BuyerTree**);
TransactionTree* addTransactionsFromFile(TransactionTree*, BuyerTree**);
TransactionTree* createTransNode(int);
int searchTransTree(TransactionTree*, unsigned int);
void displayTransactions(TransactionTree*);
void deleteTransTree(TransactionTree*);
void saveTransactionsToCSV(TransactionTree* );
TransactionTree* insertTransTree(TransactionTree* ,Transaction* );
SellerTree* extractSellersFromFile(SellerTree*);
void createSellersList(TransactionTree* , SellerTree* );
void deleteSellersTree(SellerTree* );
void listTransinTime(TransactionTree *);
void displayEnergyInRange(TransactionTree*);
int Buyerlookup(BuyerTree*, unsigned int);
BuyerTree* insertBuyerTree(BuyerTree*, BuyerData*);
BuyerTree* createBuyerNode(int);
void createBuyersList(TransactionTree*, BuyerTree*);
void deleteBuyersTree(BuyerTree*);
void levelOrderTraversalBuyers(BuyerTree*);
buyerNode* sortListBuyers(BuyerTree* , buyerNode*);
void deleteBuyersList(buyerNode*);
void calculateRevenue(SellerTree* );
Pair* sort_pairs(TransactionTree*, Pair*);
void deletePairs(Pair*);
SellerData* searchSellersTree(SellerTree* , unsigned int );
void showRegularBuyers(SellerTree* , Pair* );
