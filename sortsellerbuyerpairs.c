#include <stdlib.h>
#include <stdio.h>
#include "template.h"

Pair* MergePairs(Pair* list1, Pair* list2) {
  Pair *retval=NULL,*ptr1=list1,*ptr2=list2,*tail=NULL; 

  if(list1==NULL) retval=list2;
  else if(list2==NULL) retval=list1;
  else {
    if(ptr1->count>ptr2->count) {
      retval=tail=ptr1;
      ptr1=ptr1->next;
    }
    else {
      retval=tail=ptr2;
      ptr2=ptr2->next;
    }

    while (ptr1!=NULL && ptr2!=NULL) {
      if (ptr1->count>ptr2->count) {
        tail->next=ptr1;
        ptr1=ptr1->next;
      }
      else {
        tail->next=ptr2;
        ptr2=ptr2->next;
      }
      tail=tail->next;
    }

    if(ptr1!=NULL) 
        tail->next=ptr1;
    else 
        tail->next=ptr2;
  }
  return retval;
}

Pair* DividePairs(Pair* lptr) {
  Pair *fast,*slow,*nptr=NULL;
  fast=slow=lptr;
  if(lptr==NULL||lptr->next==NULL) nptr=NULL;
  else {
    fast=slow->next;
    while (fast!=NULL && fast->next!=NULL) {
      if(slow!=NULL) slow=slow->next;
      fast=fast->next->next;
    }

    nptr=slow->next;
    slow->next = NULL;
  }
  return nptr;
}

Pair* MergeSortPairs(Pair* lptr) {
  Pair *retval,*nptr;
  if (lptr==NULL || lptr->next==NULL) retval=lptr;
  else {
    nptr=DividePairs(lptr);
    lptr=MergeSortPairs(lptr);
    nptr=MergeSortPairs(nptr);
    retval=MergePairs(lptr,nptr);
  }
  return retval;
}
Pair* sort_pairs(TransactionTree* root, Pair* pairhead) {
  int found;
  TransactionTree *temp=root;
    while(temp->children[0]!=NULL)
    temp=temp->children[0];
    Pair *pairs=NULL, *pptr;
    Pair *tail=NULL;
        while(temp!=NULL)
            {
            for(int i=0;i<temp->numKeys;i++)
            {
               
                    if(temp->data.trans[i].flag==0) {
                      temp->data.trans[i].flag=1;
                      pairs=pairhead; found=0;
                      while(pairs!=NULL && !found) {
                        if(temp->data.trans[i].sellerID==pairs->sellerID && temp->data.trans[i].buyerID==pairs->buyerID) found=1;
                        else pairs=pairs->next;
                      }  
                    
                      if(found) {
                        pairs->count++;
                      } 
                      else {
                        pptr = (Pair*)malloc(sizeof(Pair));
                        pptr->next=NULL;
                        pptr->count=1;
                        pptr->flag=0;
                        pptr->buyerID=temp->data.trans[i].buyerID;
                        pptr->sellerID=temp->data.trans[i].sellerID;

                        if (pairhead==NULL) {
                            pairhead=pptr;
                            tail=pptr;
                        } else {
                            tail->next=pptr;
                            tail=pptr;  // Updating tail pointer
                        }
                      }
                    }
                }
            temp=temp->next;
            }
    
    
    pairhead=MergeSortPairs(pairhead);
    printf("Seller/Buyer pairs in decreasing order of the number of transactions between them are \n\n");
    
    pairs=pairhead;
    while(pairs!=NULL) {   
      printf("Seller ID:%d\n",pairs->sellerID);
      printf("Buyer ID:%d\n",pairs->buyerID);
      printf("Count:%d\n\n",pairs->count);
      pairs=pairs->next;
    }
    return pairhead;
  }
  
