#include <stdio.h>
#include <stdlib.h>
#include "template.h"

void saveTransactionsToCSV(TransactionTree* transRoot) {
  FILE* file = fopen("transactions.csv", "w");
  fprintf(file, "TransactionID,SellerID,BuyerID,Amount of Energy Traded,Price per KWh,Date of Transaction,Time of Transaction\n");

  while (!transRoot->is_leaf) 
        transRoot = transRoot->children[0];  // Go down to the first child until we reach a leaf

  TransactionTree* current = transRoot;
  while (current != NULL) {
    for (int i = 0; i < current->numKeys; i++) {
      Transaction* t = &current->data.trans[i];
      fprintf(file, "%u,%u,%u,%.2f,%.2f,%s,%s\n", t->transID, t->sellerID, t->buyerID, t->amtOfEnergy, t->pricePerKWh, t->date, t->time);
      }
      current = current->next;  // Move to the next leaf node
    }

  fclose(file);
  printf("Transactions saved successfully to 'transactions.csv'\n");
}

