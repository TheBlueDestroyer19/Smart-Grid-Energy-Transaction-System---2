#include <stdio.h>
#include <stdlib.h>
#include "template.h"

void showRegularBuyers(SellerTree* sellerRoot, Pair* pairhead) {
  printf("[!] Warning: If you have added a transaction and have not updated the list of buyer-seller pairs, then the exepcted output may not be shown.\nIn case of which you may need to do so by pressing 9.\n");

  Pair* current=pairhead;
  if(!current) printf("The list of buyer-seller pairs is not created!\nDo so by pressing 9!");
  else {
    int found;
    while(current && current->count>=5) {
      SellerData* sellerdata=searchSellersTree(sellerRoot,current->sellerID);
      buyerNode* curr=sellerdata->head;
      found=0;

      while(curr && curr->next && !found) {
      if(curr->buyerID==current->buyerID) found=1;
      else curr=curr->next;
      }
      if(curr && curr->buyerID==current->buyerID) found=1;

      if(!found) {
      buyerNode* newnode=(buyerNode*)malloc(sizeof(buyerNode));
      newnode->buyerID=current->buyerID;
      newnode->next=NULL;
      if(curr) curr->next=newnode;
      else sellerdata->head=newnode;
      }
      current=current->next;
    }

    SellerTree* node=sellerRoot;
    while(!node->is_leaf) node=node->children[0];

    while(node) {
      for(int i=0;i<node->numKeys;i++) {
        printf("\nSeller ID: %u\n",node->sellerdata[i].sellerID);
        printf("Regular Buyers: ");
        if(node->sellerdata[i].head==NULL) printf("None\n");
        else {
          buyerNode* curr=node->sellerdata[i].head;
          while(curr) {
            printf("%u, ",curr->buyerID);
            curr=curr->next;
          }
          printf("\n");
        }
      }
      node=node->next;
    }
  }
}
