#include <stdio.h>
#include "template.h"

int main() {
  int choice=1;
  TransactionTree *transRoot=NULL;
  SellerTree *sellerRoot=NULL;
  BuyerTree *buyerRoot=createBuyerNode(1);
  buyerNode* bhead=NULL;
  Pair* pairhead=NULL;

  transRoot=addTransactionsFromFile(transRoot,&buyerRoot);
  sellerRoot=extractSellersFromFile(sellerRoot);

  while(choice) {
    printf("\n============================================================\n");
    printf("                SMART GRID TRANSACTION SYSTEM      \n");
    printf("============================================================\n");
    printf("  1. Add New Transaction\n");
    printf("  2. Display All Transactions\n");
    printf("  3. Create a set of Transactions for Every Seller\n");
    printf("  4. Create a set of Transactions for Every Buyer\n");
    printf("  5. Find all transactions in a Given Time Period\n");
    printf("  6. Calculate Total Revenue by Seller\n");
    printf("  7. Find and Display Transactions in Energy Range\n");
    printf("  8. Sort Buyers Based on Energy Bought\n");
    printf("  9. Sort Seller/Buyer Pairs by Number of Transactions\n");
    printf("  10. Display the list of regular buyers for every seller\n");
    printf("  0. Exit\n");
    printf("============================================================\n");
    printf("\nEnter your choice: ");
    scanf("%d", &choice);

    switch(choice) {
      case 1: transRoot=addTransaction(transRoot,sellerRoot,&buyerRoot); break;
      case 2: displayTransactions(transRoot); break;
      case 3: createSellersList(transRoot,sellerRoot); break;
      case 4: createBuyersList(transRoot,buyerRoot); break;
      case 5: listTransinTime(transRoot); break;
      case 6: calculateRevenue(sellerRoot); break;
      case 7: displayEnergyInRange(transRoot); break;
      case 8: bhead=sortListBuyers(buyerRoot,bhead); break;
      case 9: pairhead=sort_pairs(transRoot,pairhead); break;
      case 10: showRegularBuyers(sellerRoot,pairhead); break;
      case 0: printf("\nExiting program...\n"); break;
      default: printf("\n[!] Invalid choice! Please try again.\n");
    }
  }

  saveTransactionsToCSV(transRoot);
  printf("Clearing the transaction tree....");
  deleteTransTree(transRoot);
  printf("Done\n");
  printf("Clearing the sellers tree....");
  deleteSellersTree(sellerRoot);
  printf("Done\n");
  printf("Clearing the buyers tree.....");
  deleteBuyersTree(buyerRoot);
  printf("Done\n");
  printf("Clearing the buyers' list....");
  deleteBuyersList(bhead);
  printf("Done\n");
  printf("Clearing the pairs' list.....");
  deletePairs(pairhead);
  printf("Done\n");
  return 0;
}
