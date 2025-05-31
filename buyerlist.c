#include <stdio.h>
#include <stdlib.h>
#include "template.h"

void displayBuyers(BuyerTree*);

BuyerTree* createBuyerNode(int is_leaf) {
  BuyerTree* newnode=(BuyerTree*)malloc(sizeof(BuyerTree));
  newnode->is_leaf=is_leaf;
  newnode->numKeys=0;
  for(int i=0;i<=MAX;i++) newnode->children[i]=NULL;
  newnode->next=NULL;
  return newnode;
}

void splitBuyers(BuyerTree* parent, int i, BuyerTree* child) {
  BuyerTree* newchild=createBuyerNode(child->is_leaf);
  int mid=MAX/2;
  if(child->is_leaf) {
    for(int j=mid;j<MAX;j++) 
      newchild->buyerdata[j-mid]=child->buyerdata[j];
    newchild->numKeys=MAX-mid;
  }
  else {
    for(int j=mid+1;j<MAX;j++)
      newchild->buyerdata[j-mid-1].buyerID=child->buyerdata[j].buyerID;
    for(int j=mid+1;j<=MAX;j++)
      newchild->children[j-mid-1]=child->children[j];
    newchild->numKeys=MAX-mid-1;
  }
  child->numKeys=mid;

  for(int j=parent->numKeys; j>i; j--) {
    parent->children[j+1]=parent->children[j];
    parent->buyerdata[j].buyerID=parent->buyerdata[j-1].buyerID;
  }

  parent->children[i+1]=newchild;
  parent->buyerdata[i].buyerID=child->buyerdata[mid].buyerID;
  parent->numKeys++;

  if(child->is_leaf) {
    newchild->next=child->next;
    child->next=newchild;
  }
}

void insertNonFullBuyers(BuyerTree* buyerNode, BuyerData* data) {
  int i=buyerNode->numKeys-1;
  if(buyerNode->is_leaf) {
    while(i>=0 && buyerNode->buyerdata[i].buyerID > data->buyerID) {
      buyerNode->buyerdata[i+1]=buyerNode->buyerdata[i];
      i--;
    }
    i++;
    buyerNode->buyerdata[i]=*data;
    buyerNode->numKeys++;
  }
  else {
    while(i>=0 && buyerNode->buyerdata[i].buyerID > data->buyerID) i--;
    i++;
    if(buyerNode->children[i]->numKeys==MAX) {
      splitBuyers(buyerNode,i,buyerNode->children[i]);
      if(data->buyerID>buyerNode->buyerdata[i].buyerID) i++;
    }
    insertNonFullBuyers(buyerNode->children[i],data);
  }
}

BuyerTree* insertBuyerTree(BuyerTree* buyerRoot,BuyerData* data) {
  BuyerTree* retval;
  if(buyerRoot->numKeys<MAX) {
    insertNonFullBuyers(buyerRoot,data);
    retval=buyerRoot;
  }
  else {
    BuyerTree* newroot=createBuyerNode(0);
    newroot->children[0]=buyerRoot;
    splitBuyers(newroot,0,buyerRoot);
    int i=(data->buyerID>newroot->buyerdata[0].buyerID)? 1:0;
    insertNonFullBuyers(newroot->children[i],data);
    retval=newroot;
  }
  return retval;
}

void SearchBuyersTree(BuyerTree* buyerRoot, unsigned int buyerID,Transaction* trans) {
  if(buyerRoot->is_leaf) {
    int i=0;
    while(i<buyerRoot->numKeys && buyerID>buyerRoot->buyerdata[i].buyerID) i++;
    if(i<buyerRoot->numKeys && buyerID==buyerRoot->buyerdata[i].buyerID) {
      if(!searchTransTree(buyerRoot->buyerdata[i].subtree, trans->transID)) {
        buyerRoot->buyerdata[i].total_energy+=trans->amtOfEnergy;
        buyerRoot->buyerdata[i].subtree=insertTransTree(buyerRoot->buyerdata[i].subtree,trans);
      }
    }
  }
  else {
    int i=0;
    while(i<buyerRoot->numKeys && buyerRoot->buyerdata[i].buyerID<=buyerID) i++;
    SearchBuyersTree(buyerRoot->children[i],buyerID,trans);
  }
}

void createBuyersList(TransactionTree* transRoot, BuyerTree* buyerRoot) {
  TransactionTree* current=transRoot;
  while(!current->is_leaf) current=current->children[0];

  while(current) {
    for(int i=0;i<current->numKeys;i++) {
      Transaction trans=current->data.trans[i];
      SearchBuyersTree(buyerRoot,current->data.trans[i].buyerID,&trans);
    }
    current=current->next;
  }
  displayBuyers(buyerRoot);
}

void displayBuyers(BuyerTree* buyerRoot) {
  BuyerTree* current=buyerRoot;
  printf("%d",buyerRoot->is_leaf);
  while(!current->is_leaf) current=current->children[0];

  while(current) {
    for(int i=0;i<current->numKeys;i++) {
      printf("\n\nBuyer ID: %u\tTotal Energy Bought: %f\n",current->buyerdata[i].buyerID,current->buyerdata[i].total_energy);

      TransactionTree* transnode=current->buyerdata[i].subtree;
      while(!transnode->is_leaf) transnode=transnode->children[0];
      while(transnode) {
        for(int j=0;j<transnode->numKeys;j++) {
          printf("\nTransaction ID: %u\n",transnode->data.trans[j].transID);
          printf("Seller ID: %u\n",transnode->data.trans[j].sellerID);
          printf("Amount of Energy: %f\n",transnode->data.trans[j].amtOfEnergy);
          printf("Price per KWh: %f\n",transnode->data.trans[j].pricePerKWh);
          printf("Total Price: %f\n",transnode->data.trans[j].total_price); 
          printf("Date: %s\n",transnode->data.trans[j].date);
          printf("Time: %s\n",transnode->data.trans[j].time); 
        }
        transnode=transnode->next;
      }
    }
    current=current->next;
  }
}

int Buyerlookup(BuyerTree* buyerRoot, unsigned int buyerID) {
  int retval;
  if(buyerRoot==NULL) retval=0;
  else if(buyerRoot->is_leaf) {
    int i=0;
    while(i<buyerRoot->numKeys && buyerID>buyerRoot->buyerdata[i].buyerID) i++;
    if(i<buyerRoot->numKeys && buyerID==buyerRoot->buyerdata[i].buyerID) retval=1;
    else retval=0;
  }
  else {
    int i=0;
    while(i<buyerRoot->numKeys && buyerRoot->buyerdata[i].buyerID<=buyerID) i++;
    retval=Buyerlookup(buyerRoot->children[i],buyerID);
  }
  return retval;
}
