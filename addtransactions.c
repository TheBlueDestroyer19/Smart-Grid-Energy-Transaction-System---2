#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "template.h"

TransactionTree* createTransNode(int is_leaf) {
  TransactionTree* node=(TransactionTree*)malloc(sizeof(TransactionTree));
  node->is_leaf=is_leaf;
  node->numKeys=0;
  node->next=NULL;
  for(int i=0;i<=MAX;i++) node->children[i]=NULL;
  return node;
}

int isValidDate(char *date) {
  int valid=1;
  int day, month, year;
  int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  if(strlen(date)!=10 || date[2]!=':' || date[5]!=':') valid=0;
  if(sscanf(date, "%2d:%2d:%4d", &day, &month, &year) != 3) valid=0;
  if(month<1 || month>12) valid=0;
  if(day<1 || day>31) valid=0;
  if((year%4==0 && year%100!=0) || (year%400==0)) daysInMonth[1]=29;
  if(day>daysInMonth[month-1]) valid=0;
    
  return valid;
}
int isValidTime(char *time) {
  int valid=1;
  int hours, minutes;

  if(strlen(time)!=5 || time[2]!=':') valid=0;
  if(sscanf(time,"%2d:%2d",&hours,&minutes)!=2) valid=0;
  if(hours<0 || hours>23) valid=0;
  if(minutes<0 || minutes>59) valid=0;
    
  return valid;
}
int isValidID(TransactionTree* transNode, unsigned int transID) {
  int retval;
  if(transNode->is_leaf) {
    retval=1;
    for(int i=0;i<transNode->numKeys && retval;i++) if(transNode->data.trans[i].transID==transID) retval=0;
  }
  else {
    int i=transNode->numKeys-1;
    while(i>=0 && transID<transNode->data.keys[i]) i--;
    i++;
    retval=isValidID(transNode->children[i],transID);
  }
  return retval;
}

void split(TransactionTree* parent, int i, TransactionTree* child) {
  TransactionTree* newchild=createTransNode(child->is_leaf);
  int mid=MAX/2;
  if(child->is_leaf) {
    for(int j=mid;j<MAX;j++) 
      newchild->data.trans[j-mid]=child->data.trans[j];
    newchild->numKeys=MAX-mid;
  }
  else {
    for(int j=mid+1;j<MAX;j++)
      newchild->data.keys[j-mid-1]=child->data.keys[j];
    for(int j=mid+1;j<=MAX;j++)
      newchild->children[j-mid-1]=child->children[j];
    newchild->numKeys=MAX-mid-1;
  }
  child->numKeys=mid;

  for(int j=parent->numKeys;j>i;j--) {
    parent->children[j+1]=parent->children[j];
    parent->data.keys[j]=parent->data.keys[j-1];
  }

  parent->children[i+1]=newchild;
  if(child->is_leaf) parent->data.keys[i]=child->data.trans[mid].transID;
  else parent->data.keys[i]=child->data.keys[mid];
  parent->numKeys++;

  if(child->is_leaf) {
    newchild->next=child->next;
    child->next=newchild;
  }
}

void insertNonFull(TransactionTree* node,Transaction *trans) {
  int i=node->numKeys-1;
  if(node->is_leaf) {
    while(i>=0 && trans->transID<node->data.trans[i].transID) {
      node->data.trans[i+1]=node->data.trans[i];
      i--;
    }
    node->data.trans[i+1]=*trans;
    node->numKeys++;
  }
  else {
    while(i>=0 && trans->transID<node->data.keys[i]) i--;
    i++;
    if(node->children[i]->numKeys==MAX) {
    split(node,i,node->children[i]);
      if(trans->transID>node->data.keys[i]) i++;
    }
    insertNonFull(node->children[i],trans);
  }
}

TransactionTree* insertTransTree(TransactionTree* transRoot,Transaction* trans) {
  TransactionTree* retval;
  if(transRoot->numKeys==MAX) { 
    TransactionTree* newroot=createTransNode(0);
    newroot->children[0]=transRoot;
    split(newroot,0,transRoot);
    int i=(trans->transID>newroot->data.keys[0])? 1:0;
    insertNonFull(newroot->children[i],trans);
    retval=newroot;
  }
else {
    insertNonFull(transRoot,trans);
    retval=transRoot;
  }
  return retval;
}

TransactionTree* addTransaction(TransactionTree* transRoot, SellerTree* sellerRoot, BuyerTree** buyerRoot) {
  Transaction trans;
  unsigned int id;
  float amt;
  char timestamp[11];
  printf("Enter the transaction ID: "); scanf("%u",&id);
  while(!isValidID(transRoot,id)) {
    printf("The given ID already exists in the database!!\nPlease re-enter: ");
    scanf("%u",&id);
  }
  trans.transID=id;
  printf("Enter the seller ID: "); scanf("%u",&id);
  trans.sellerID=id;
  printf("Enter the buyer ID: "); scanf("%u",&id);
  trans.buyerID=id;
  printf("Enter the amount of energy traded: "); scanf("%f",&amt);
  trans.amtOfEnergy=amt;
  printf("Enter the date of transaction: "); scanf("%s",timestamp);
  while(!isValidDate(timestamp)) {
    printf("The date is incorrect!!\nPlease re-enter the date: ");
    scanf("%s",timestamp);
  }
  strcpy(trans.date,timestamp);
  printf("Enter the time of transaction: "); scanf("%s",timestamp);
  while(!isValidTime(timestamp)) {
    printf("The time is incorrect!!\nPlease re-enter the time: ");
    scanf("%s",timestamp);
  }
  strcpy(trans.time,timestamp);

  //Lookup for the value of price per KWh from the Sellers Tree if not found then say id not found
  SellerData* sellerdata=searchSellersTree(sellerRoot,trans.sellerID);
  if(sellerdata) {
    if(trans.amtOfEnergy>=300) trans.pricePerKWh=sellerdata->a300;
    else trans.pricePerKWh=sellerdata->b300;

    trans.total_price=trans.amtOfEnergy * trans.pricePerKWh;
    trans.flag=0;
    trans.next=NULL;
  
    transRoot=insertTransTree(transRoot,&trans);

    if(!Buyerlookup(*buyerRoot, trans.buyerID)) {
      BuyerData bdata;
      bdata.buyerID=trans.buyerID;
      bdata.total_energy=0.0;
      bdata.subtree=createTransNode(1);
      *buyerRoot=insertBuyerTree(*buyerRoot,&bdata);
    }
    printf("Successfully added the transaction!!\n");
  }
  else printf("The given seller is not a verified seller.\nFailed to add the transaction!!\n");
  return transRoot;
}

TransactionTree* addTransactionsFromFile(TransactionTree* transRoot, BuyerTree** buyerRoot) {
  transRoot=createTransNode(1);
  FILE *file = fopen("transactions.csv", "r");
  char line[MAX_LINE_LENGTH];
  if (!file) {
    printf("Error opening file\n");
  }
  else if (fgets(line, sizeof(line), file) == NULL) {
    fclose(file);
  }
  else {    
    while (fgets(line, sizeof(line), file)) {
      Transaction trans;
        
      if(sscanf(line, "%u,%u,%u,%f,%f,%10[^,],%5s", &trans.transID, &trans.sellerID, &trans.buyerID, &trans.amtOfEnergy, &trans.pricePerKWh, trans.date, trans.time) == 7) {

        trans.total_price = trans.amtOfEnergy * trans.pricePerKWh;
        trans.flag=0;
        trans.next=NULL;

        transRoot=insertTransTree(transRoot,&trans);
        if(!Buyerlookup(*buyerRoot, trans.buyerID)) {
          BuyerData bdata;
          bdata.buyerID=trans.buyerID;
          bdata.total_energy=0.0;
          bdata.subtree=createTransNode(1);
          *buyerRoot=insertBuyerTree(*buyerRoot,&bdata);
        }
      }
      else {
        fprintf(stderr, "Invalid line format: %s", line);
      }
    }
    fclose(file);
  }
  return transRoot;
}

int searchTransTree(TransactionTree* transNode, unsigned int transID) {
  int retval;
  if(transNode->is_leaf) {
    int i=0;
    while(i<transNode->numKeys && transID>transNode->data.trans[i].transID) i++;
    if(i<transNode->numKeys && transID==transNode->data.trans[i].transID) retval=1;
    else retval=0;
  }
  else {
    int i=0;
    while(i<transNode->numKeys && transID>=transNode->data.trans[i].transID) i++;
    retval=searchTransTree(transNode->children[i],transID);
  }
  return retval;
}
