#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "skipList.h"
#include <time.h>

/* ************************************************************************
Main Function
 ************************************************************************ */
/* Test function:
 param: no parameters
 pre:	no parameres
 post: prints out the contents of the skip list */

void test(){
	int i,j;
	int M;
	struct skipList *slst;
	test();
	
	 srand ( time(NULL) );
	
	/*  FIX ME */
	slst=(struct skipList *)malloc(sizeof(struct skipList));  /*2*/
	assert(slst);
	
	/*  Initialize the skip list */
	initSkipList(slst);  /*2*/

	/*  Add to the skip list  M = 20 random integers in [0,100] */
	M=20;/*5*/
	for(i=0;i<M;i++)
	{
		addSkipList(slst, rand()%101);
	}
	
	/*  Print out the contents of the skip list in the breadth-first order, starting from top. 
	    While printing the elements, move to a new line every time you reach the end of one level, 
	    and move down to the lower level of the skip list. 
	    For example, the print out of a skip list with 5 elements should look like
	 
	    7
	    7 14 29
	    3 7 9 14 20
	 
	 *//*10*/

	printf("\nPrinting The Skiplis started ... \n");
	printSkipList(slst);
	printf("Printing The Skiplis Finished. \n");

	
	/* Develop test cases for evaluating your functions:  
	         int containsSkipList(struct skipList *slst, TYPE e) 
	         int removeSkipList(struct skipList *slst, TYPE e)
	 */

	printf("Adding 3 numbers that are not in the list now: e.g. 200, 202, 205 \n");

	addSkipList(slst, 200);
	addSkipList(slst, 202);
	addSkipList(slst, 205);

	printf("\nPrinting The Skiplis started ... \n");
	printSkipList(slst);
	printf("Printing The Skiplis Finished. \n");

	for(i=200; i<=205; i++)
		printf("\n%d %s in the list!.\n",i,containsSkipList(slst, i)==1?"IS":"IS NOT");


	printf("\nRemoving 200,202 and 205 from the Skiplis started ... \n");
	printf("\Before Removing:\n");
	printSkipList(slst);

	for(i=200; i<=205; i++)
		removeSkipList(slst, i);

	printf("\After Removing:\n");
	printSkipList(slst);
	printf("Removing 200,202 and 205 from the Skiplis Finished ... \n");


	for(i=200; i<205; i++)
		printf("\n%d %s in the list!.\n",i,containsSkipList(slst, i)==1?"IS":"IS NOT");
	


}


/* ************************************************************************
Internal Functions
 ************************************************************************ */

/* Coin toss function:
 param: 	no parameters
 pre:	no parameres
 post: output is a random intiger number in {0,1} */
int flipSkipLink(void)
{
	return rand() % 2; 	
}

/* Move to the right as long as the next element is smaller than the input value:
 param: 	current -- pointer to a place in the list from where we need to slide to the right
 param:	e --  input value
 pre:	current is not NULL
 post: returns one link before the link that contains the input value e */
struct skipLink * slideRightSkipList(struct skipLink *current, TYPE e){
	while ((current->next != 0) && LT(current->next->value, e))
		current = current->next;
	return current;
}


/* Create a new skip link for a value
	param: e	 -- the value to create a link for
	param: nextLnk	 -- the new link's next should point to nextLnk
	param: downLnk -- the new link's down should point to downLnk
	pre:	none
	post:	a link to store the value */
struct skipLink* newSkipLink(TYPE e, struct skipLink * nextLnk, struct skipLink* downLnk) {
	struct skipLink * tmp = (struct skipLink *) malloc(sizeof(struct skipLink));
	assert(tmp != 0);
	tmp->value = e;
	tmp->next = nextLnk;
	tmp->down = downLnk;
	return tmp;
}


/* Add a new skip link recursively
 param: current -- pointer to a place in the list where the new element should be inserted
 param: e	 -- the value to create a link for
 pre:	current is not NULL
 post: a link to store the value */
struct skipLink* skipLinkAdd(struct skipLink * current, TYPE e) {
	struct skipLink *newLink, *downLink;
	newLink = 0;
	if (current->down == 0) {
		newLink = newSkipLink(e, current->next, 0);
		current->next = newLink;
	}
	else {
		downLink = skipLinkAdd(slideRightSkipList(current->down, e), e);
		if (downLink != 0 && flipSkipLink()) {
			newLink = newSkipLink(e, current->next, downLink);
			current->next = newLink;
		}
	}
	return newLink;
}


/* ************************************************************************
Public Functions
 ************************************************************************ */

/* Initialize skip list:
 param:  slst -- pointer to the skip list
 pre:	slst is not null
 post: the sentinels are allocated, the pointers are set, and the list size equals zero */
void initSkipList (struct skipList *slst) 
{

	slst->size=0;
	slst->topSentinel=(struct skipLink * )malloc(sizeof(struct skipLink));
	assert(slst->topSentinel);
	slst->topSentinel->next=NULL;
	slst->topSentinel->down=NULL;
}

/* Checks if an element is in the skip list:
 param: slst -- pointer to the skip list
 param: e -- element to be checked
 pre:	slst is not null
 post: returns true or false  */
int containsSkipList(struct skipList *slst, TYPE e)
{
	struct skipLink *tmp = slst->topSentinel;
	while (tmp != 0) {  /* 2pts */
		tmp = slideRightSkipList(tmp, e); /* 1pt */
		if ((tmp->next != 0) && EQ(e, tmp->next->value)) /* 2pts */
			return 1;
		tmp = tmp->down;  /* 1pt */
	}
	return 0;
}


/* Remove an element from the skip list:
 param: slst -- pointer to the skip list
 param: e -- element to be removed
 pre:	slst is not null
 post: the new element is removed from all internal sorted lists */
void removeSkipList(struct skipList *slst, TYPE e)
{
	struct skipLink *current, *tmp;
	current = slst->topSentinel; /* 1pts */
	while (current != 0) {  /* 2pts */
		current = slideRightSkipList(current, e);
		if (current->next != 0 && EQ(e, current->next->value)) {  /* 2pts */
			tmp = current->next; /* 2pts */
			current->next = current->next->next;
			free(tmp);    /* 2pts */
			if (current->down == NULL)
				slst->size--;   /* 2pts */
		}
		current = current->down; /* 1pt */
	}
}




/* Add a new element to the skip list:
	param: slst -- pointer to the skip list
	param: e -- element to be added
	pre:	slst is not null
	post:	the new element is added to the lowest list and randomly to higher-level lists */
void addSkipList(struct skipList *slst, TYPE e)
{
	struct skipLink *downLink, *newLink;
	
	downLink = skipLinkAdd(slideRightSkipList(slst->topSentinel,e),e);  /* 6pts */

	if (downLink != 0 && flipSkipLink()) {   /* 2pts */
		newLink = newSkipLink(e, 0, downLink);    /* 4pts */
		slst->topSentinel = newSkipLink(0, newLink, slst->topSentinel);    /* 4pts */
	}
	slst->size++;   /* 2pts */
}

/* Find the number of elements in the skip list:
 param: slst -- pointer to the skip list
 pre:	slst is not null
 post: the number of elements */
int sizeSkipList(struct skipList *slst){
	
	return slst->size;
	
}


/* Print the links in the skip list:
	param: slst -- pointer to the skip list
	pre:	slst is not null and slst is not empty
	post: the links in the skip list are printed breadth-first, top-down */
void printSkipList(struct skipList *slst)
{
	/* FIX ME*/
        struct skipLink *sentinel = slst->topSentinel;
	struct skipLink *current;

	while (sentinel != NULL) {
		current = sentinel->next;
		while (current != NULL) {
			printf("%g ", current->value);
			current = current->next;
		}
		printf("\n");
		sentinel = sentinel->down;
	}
}


/* Merge two skip lists, by adding elements of skip list 2 to skip list 1 
 that are not already contained in skip list 1. 
 The function is also supposed to remove the entire skip list 2 from the memory.
 param: slst1 -- pointer to the skip list 1
 param: slst2 -- pointer to the skip list 2
 pre: slst1 and slst2 are not null, and skip list 1 and skip list 2 are not empty
 post: slst1 points to the merger skip list,  slst2 is null*/
void mergeSkipList(struct skipList *slst1, struct skipList *slst2)
{

	struct skipLink *sentinel = slst2->topSentinel;
	struct skipLink *current, *tmp;
		
	/* Iterate while there are lists in Skip List 2  (1pt) */ 
	while (sentinel != NULL) { /* the loop over lists */
			
		/* Iterate while there are elements in 
		the visited list of Skip List 2  (2pts) */ 
			
		current = sentinel->next; /* 1pt */
			
		while (current != NULL) { /* 1pt */ /* the loop over links in a list */
				
			/* Check if the visited element of Skip List 2 is in Skip List 1,
			    if not, add it to Skip List 1,
			    remove the element, and continue iteration  (4pts) */ 
				
			if (!containsSkipList(slst1, current->value))   /* 1pt */
				addSkipList(slst1, current->value);     /* 1pt */
				
			/* Remove the visited element of Skip List 2 
			    and move to the next element */    
				
			/* we first must find a link before the current one, 
			   because the current link will be removed */       
			tmp = slideRightSkipList(sentinel,current->value); 
			removeSkipList(slst2, current->value);   /* 1pt */
				
			/* we can now move to the next link because this inner loop
			   is over links in a particular list of the Skip List */
				current = tmp->next;    /* 1pt */
				
			/* note that it would be wrong to write current = current->next; 
			because current was freed by removeSkipList */       
		}	
		/* Since this loop is over the lists, 
		  we move to the lower list */ 
			
		sentinel = sentinel->down; /* 1pt */
			
	} /* end of merging */
		
	/* Remove the entire Skip List 2 from the memory (2pts) */
		
	free(slst2->topSentinel);   /* 1pt */
		
	free(slst2);   /* 1pt */
		
} /* end of the function */
	


