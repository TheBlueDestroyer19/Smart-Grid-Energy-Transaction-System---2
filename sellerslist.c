#include <stdio.h>
#include <stdlib.h>
#include "template.h"

void displaySellers(SellerTree*);

SellerTree* createSellerNode(int is_leaf) {
  SellerTree* newnode=(SellerTree*)malloc(sizeof(SellerTree));
  newnode->is_leaf=is_leaf;
  newnode->numKeys=0;
  for(int i=0;i<=MAX;i++) newnode->children[i]=NULL;
  newnode->next=NULL;
  return newnode;
}

void splitSellers(SellerTree* parent, int i, SellerTree* child) {
  SellerTree* newchild=createSellerNode(child->is_leaf);
  int mid=MAX/2;
  if(child->is_leaf) {
    for(int j=mid;j<MAX;j++) 
      newchild->sellerdata[j-mid]=child->sellerdata[j];
    newchild->numKeys=MAX-mid;
  }
  else {
    for(int j=mid+1;j<MAX;j++)
      newchild->sellerdata[j-mid-1].sellerID=child->sellerdata[j].sellerID;
    for(int j=mid+1;j<=MAX;j++)
      newchild->children[j-mid-1]=child->children[j];
    newchild->numKeys=MAX-mid-1;
  }
  child->numKeys=mid;

  for(int j=parent->numKeys; j>i; j--) {
    parent->children[j+1]=parent->children[j];
    parent->sellerdata[j].sellerID=parent->sellerdata[j-1].sellerID;
  }

  parent->children[i+1]=newchild;
  parent->sellerdata[i].sellerID=child->sellerdata[mid].sellerID;
  parent->numKeys++;

  if(child->is_leaf) {
    newchild->next=child->next;
    child->next=newchild;
  }
}

void insertNonFullSellers(SellerTree* sellerNode, SellerData* data) {
  int i=sellerNode->numKeys-1;
  if(sellerNode->is_leaf) {
    while(i>=0 && sellerNode->sellerdata[i].sellerID > data->sellerID) {
      sellerNode->sellerdata[i+1]=sellerNode->sellerdata[i];
      i--;
    }
    i++;
    sellerNode->sellerdata[i]=*data;
    sellerNode->numKeys++;
  }
  else {
    while(i>=0 && sellerNode->sellerdata[i].sellerID > data->sellerID) i--;
    i++;
    if(sellerNode->children[i]->numKeys==MAX) {
      splitSellers(sellerNode,i,sellerNode->children[i]);
      if(data->sellerID>sellerNode->sellerdata[i].sellerID) i++;
    }
    insertNonFullSellers(sellerNode->children[i],data);
  }
}

SellerTree* insertSellerTree(SellerTree* sellerRoot,SellerData* data) {
  SellerTree* retval;
  if(sellerRoot->numKeys<MAX) {
    insertNonFullSellers(sellerRoot,data);
    retval=sellerRoot;
  }
  else {
    SellerTree* newroot=createSellerNode(0);
    newroot->children[0]=sellerRoot;
    splitSellers(newroot,0,sellerRoot);
    int i=(data->sellerID>newroot->sellerdata[0].sellerID)? 1:0;
    insertNonFullSellers(newroot->children[i],data);
    retval=newroot;
  }
  return retval;
}

void SearchSellersTree(SellerTree* sellerRoot, unsigned int sellerID,Transaction* trans) {
  if(sellerRoot->is_leaf) {
    int i=0;
    while(i<sellerRoot->numKeys && sellerID>sellerRoot->sellerdata[i].sellerID) i++;
    if(i<sellerRoot->numKeys) {
      if(!searchTransTree(sellerRoot->sellerdata[i].subtree, trans->transID))
        sellerRoot->sellerdata[i].subtree=insertTransTree(sellerRoot->sellerdata[i].subtree,trans);
    }
  }
  else {
    int i=0;
    while(i<sellerRoot->numKeys && sellerRoot->sellerdata[i].sellerID<=sellerID) i++;
    SearchSellersTree(sellerRoot->children[i],sellerID,trans);
  }
}

void createSellersList(TransactionTree* transRoot, SellerTree* sellerRoot) {
  TransactionTree* current=transRoot;
  while(!current->is_leaf) current=current->children[0];

  while(current) {
    for(int i=0;i<current->numKeys;i++) {
      Transaction trans=current->data.trans[i];
      SearchSellersTree(sellerRoot,current->data.trans[i].sellerID,&trans);
    }
    current=current->next;
  }
  displaySellers(sellerRoot);
}

SellerTree* extractSellersFromFile(SellerTree* sellerRoot) {
  sellerRoot=createSellerNode(1);
  FILE *file=fopen("sellers.csv","r");
  char line[MAX_LINE_LENGTH];
  if(!file) printf("Error in opening the file!\n");
  else if(fgets(line,sizeof(line),file)==NULL) {
    printf("Nothing in the file!\n");
    fclose(file);
  }
  else {
    while(fgets(line, sizeof(line), file)) {
      SellerData sellerdata;
      if(sscanf(line, "%u,%f,%f", &sellerdata.sellerID, &sellerdata.a300, &sellerdata.b300)==3) {
        sellerdata.revenue=0.0;
        sellerdata.head=NULL;
        sellerdata.subtree=createTransNode(1);
        sellerRoot=insertSellerTree(sellerRoot,&sellerdata);
      }
      else {
        printf("Line format error!\n");
      }
    }
    fclose(file);
  }
  return sellerRoot;
}

void displaySellers(SellerTree* sellerRoot) {
  SellerTree* current=sellerRoot;
  while(!current->is_leaf) current=current->children[0];

  while(current) {
    for(int i=0;i<current->numKeys;i++) {
      printf("\nSeller ID: %u\tPrice above 300: %f\tPrice below 300: %f\tRevenue: %f\n",current->sellerdata[i].sellerID,current->sellerdata[i].a300,current->sellerdata[i].b300, current->sellerdata[i].revenue);

      TransactionTree* transnode=current->sellerdata[i].subtree;
      while(!transnode->is_leaf) transnode=transnode->children[0];
      while(transnode) {
        for(int j=0;j<transnode->numKeys;j++) {
          printf("Transaction ID: %u\n",transnode->data.trans[j].transID);
          printf("Buyer ID: %u\n",transnode->data.trans[j].buyerID);
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
