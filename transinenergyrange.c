#include <stdio.h>
#include <stdlib.h>
#include "template.h"

Transaction* Merge(Transaction* list1, Transaction* list2) {
  Transaction* retval;
  if (list1 == NULL) retval=list2;
  else if (list2 == NULL) retval=list1;
  else {
    Transaction* result = NULL;
    Transaction* tail = NULL;
    Transaction* ptr1 = list1;
    Transaction* ptr2 = list2;

    if (ptr1->amtOfEnergy < ptr2->amtOfEnergy) {
      result = tail = ptr1;
      ptr1 = ptr1->next;
    }
    else {
      result = tail = ptr2;
      ptr2 = ptr2->next;
    }

    while (ptr1 != NULL && ptr2 != NULL) {
      if (ptr1->amtOfEnergy < ptr2->amtOfEnergy) {
        tail->next = ptr1;
        ptr1 = ptr1->next;
      }
      else {
        tail->next = ptr2;
        ptr2 = ptr2->next;
      }
      tail = tail->next;
    }

    if (ptr1 != NULL) 
      tail->next = ptr1;
    else 
      tail->next = ptr2;

    retval=result;
  }

  return retval;
}

Transaction* Divide(Transaction* lptr) {
  Transaction* retval;
  if (lptr == NULL || lptr->next == NULL) retval=NULL;
  else {
    Transaction* slow = lptr;
    Transaction* fast = lptr->next;

    while (fast != NULL && fast->next != NULL) {
      slow = slow->next;
      fast = fast->next->next;
    }
    Transaction* nptr = slow->next;
    slow->next = NULL;

    retval=nptr;
  }
  return retval;
}

Transaction* MergeSort(Transaction* lptr) {
  Transaction* retval;
  if (lptr == NULL || lptr->next == NULL) retval=lptr;
  else {
    Transaction* nptr = Divide(lptr);
    lptr = MergeSort(lptr);
    nptr = MergeSort(nptr);
    retval=Merge(lptr, nptr);
  }
  return retval;
}

void displayEnergyInRange(TransactionTree* root) {
  float low, high;
  printf("Enter the lowest amount\n");
  scanf("%f", &low);
  printf("Enter the highest amount\n");
  scanf("%f", &high);

  TransactionTree* temp = root;
  while(temp->children[0] != NULL) temp=temp->children[0];

  Transaction* head = (Transaction*)malloc(sizeof(Transaction));
  *head = temp->data.trans[0];
  temp = temp->next;
  Transaction* nptr = head;

  if (temp != NULL) {
    nptr = (Transaction*)malloc(sizeof(Transaction));
    head->next = nptr;
    while (temp != NULL) {
      for (int i = 0; i < temp->numKeys; i++) {
        *nptr = temp->data.trans[i];
        nptr->next = (Transaction*)malloc(sizeof(Transaction));
        nptr = nptr->next;
      }
      temp = temp->next;
    }
    nptr->next = NULL;
    head = MergeSort(head);
  } else {
    head->next = NULL;
  }

  while (nptr != NULL) {
    if (nptr->amtOfEnergy >= low && nptr->amtOfEnergy <= high) {
      printf("Transaction Id: %u\n", nptr->transID);
      printf("Seller Id: %d\n", nptr->sellerID);
      printf("Buyer Id: %d\n", nptr->buyerID);
      printf("Amount of energy in kWh: %f\n", nptr->amtOfEnergy);
      printf("Price per kWh: %f\n", nptr->pricePerKWh);
      printf("The date is %s\n", nptr->date);
      printf("The time of transaction is %s\n\n", nptr->time);
    }
    nptr = nptr->next;
  }

  printf("Cleaning up the trash..");
  while (head != NULL) {
    nptr = head;
    head = head->next;
    free(nptr);
  }
}
