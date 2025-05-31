#include <stdio.h>
#include "template.h"

void displayTransactions(TransactionTree* transRoot) {
  if (transRoot==NULL) {
    printf("No transactions available.\n");
  }
  else {
    TransactionTree* current = transRoot;
    while (!current->is_leaf) current = current->children[0];
    
    printf("\nAll Transactions:\n");
    printf("%-10s %-10s %-10s %-10s %-10s %-12s %-8s\n", "TransID", "SellerID", "BuyerID", "Energy", "Price/kWh", "Date", "Time");
    printf("--------------------------------------------------------------------------------\n");
    
    while (current) {
      for (int i = 0; i < current->numKeys; i++) {
        printf("%-10u %-10u %-10u %-10.2f %-10.2f %-12s %-8s\n", current->data.trans[i].transID, current->data.trans[i].sellerID,                                          current->data.trans[i].buyerID, current->data.trans[i].amtOfEnergy, current->data.trans[i].pricePerKWh, current->data.trans[i].date, current->data.trans[i].time);
      }
      current = current->next; // Move to next leaf node
    }
  }
}
