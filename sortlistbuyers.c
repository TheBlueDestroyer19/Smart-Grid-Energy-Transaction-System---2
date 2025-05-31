#include <stdio.h>
#include <stdlib.h>
#include "template.h"

buyerNode* divideBuyersList(buyerNode *buyerlisthead) {
  buyerNode *fast,*slow,*temp=NULL;
  fast=slow=buyerlisthead;
  if(buyerlisthead==NULL || buyerlisthead->next==NULL) temp=NULL;
  else {
    fast=slow->next;
    while(fast!=NULL && fast->next!=NULL) {
      fast=fast->next->next;
      if(slow!=NULL) slow=slow->next;
    }
    temp=slow->next;
    slow->next=NULL;
  }
  return temp;
}
buyerNode* mergeBuyersList(buyerNode* first, buyerNode* second) {
  buyerNode *retval=NULL, *ptr1=first, *ptr2=second, *tail=NULL;
  if(first==NULL) retval=second;
  else if(second==NULL) retval=first;
  else {
    if(ptr1->total_energy<ptr2->total_energy) {
      retval=tail=ptr1;
      ptr1=ptr1->next;
    }
    else {
      retval=tail=ptr2;
      ptr2=ptr2->next;
    }
    while(ptr1!=NULL && ptr2!=NULL) {
      if(ptr1->total_energy<ptr2->total_energy) {
        tail->next=ptr1;
        ptr1=ptr1->next;
      }
      else {
        tail->next=ptr2;
        ptr2=ptr2->next;
      }
      tail=tail->next;
    }
    if(ptr1!=NULL) tail->next=ptr1;
    else tail->next=ptr2;
  }
  return retval;
}
buyerNode* mergeSortBuyersList(buyerNode *buyerlisthead) {
  buyerNode* retval,*second;
  if(buyerlisthead==NULL || buyerlisthead->next==NULL) retval=buyerlisthead;
  else {
    second=divideBuyersList(buyerlisthead);
    buyerlisthead=mergeSortBuyersList(buyerlisthead);
    second=mergeSortBuyersList(second);
    retval=mergeBuyersList(buyerlisthead,second);
  }
  return retval;
}

void showListBuyers(buyerNode* bhead) {
  printf("The list of buyers in increasing order of energy bought:\n\n");
  while(bhead) {
    printf("Buyer ID: %u\t\tAmount of Energy Bought: %f\n",bhead->buyerID, bhead->total_energy);
    bhead=bhead->next;
  }
}

buyerNode* sortListBuyers(BuyerTree* buyerRoot, buyerNode* bhead) {
  printf("[!] Warning: If you have added any entry in the database or not made the set of transactions for buyers, the expected results may not be shown!\nIn case of which you might need to consider creating a the set by pressing 4.\n\n");
  BuyerTree* current=buyerRoot;
  while(!current->is_leaf) current=current->children[0];
  int found;
  while(current) {
    for(int i=0;i<current->numKeys;i++) {
      unsigned int buyerID=current->buyerdata[i].buyerID;
      buyerNode* curr=bhead;
      found=0;
      while(curr && !found) {
        if(buyerID==curr->buyerID) found=1;
        else curr=curr->next;
      }

      if(found || !curr) {
        buyerNode* newnode=(buyerNode*)malloc(sizeof(buyerNode));
        newnode->buyerID=current->buyerdata[i].buyerID;
        newnode->total_energy=current->buyerdata[i].total_energy;
        newnode->next=NULL;

        if(curr) {
          newnode->next=curr->next;
          curr->next=newnode;
        }
        else {
          newnode->next=bhead;
          bhead=newnode;
        }
      }
      else curr->total_energy=current->buyerdata[i].total_energy;
    }
    current=current->next;
  }
  
  bhead=mergeSortBuyersList(bhead);
  showListBuyers(bhead);
  return bhead;
}
