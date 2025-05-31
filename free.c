#include <stdlib.h>
#include <stdio.h>
#include "template.h"

void deleteTransTree(TransactionTree* root) {
  if (!root->is_leaf) 
    for (int i = 0; i <= root->numKeys; i++) 
      deleteTransTree(root->children[i]);

  free(root);
}

void deleteBuyersList(buyerNode* head) {
  while(head) {
    buyerNode* temp=head;
    head=head->next;
    free(temp);
  }
}

void deleteSellersTree(SellerTree* sellerRoot) {
  if(!sellerRoot->is_leaf)
    for(int i=0;i<=sellerRoot->numKeys;i++)
      deleteSellersTree(sellerRoot->children[i]);

  if(sellerRoot->is_leaf) {
    for(int i=0;i<sellerRoot->numKeys;i++) {
      deleteTransTree(sellerRoot->sellerdata[i].subtree);
      deleteBuyersList(sellerRoot->sellerdata[i].head);
    }
  }
  free(sellerRoot);
}

void deleteBuyersTree(BuyerTree* buyerRoot) {
  if(!buyerRoot->is_leaf)
    for(int i=0;i<=buyerRoot->numKeys;i++)
      deleteBuyersTree(buyerRoot->children[i]);

  if(buyerRoot->is_leaf) 
    for(int i=0;i<buyerRoot->numKeys;i++)
      deleteTransTree(buyerRoot->buyerdata[i].subtree);

  free(buyerRoot);
}

void deletePairs(Pair* pairhead) {
  Pair* current;
  while(pairhead) {
    current=pairhead;
    pairhead=pairhead->next;
    free(current);
  }
}
