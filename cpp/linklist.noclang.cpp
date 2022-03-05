//    LINKLIST.CPP
//
//    Started:       May 8, 1992
//    Last Modified: November 2, 2015
//    Version 2.0
//    By Robert W. Bryce

/* This module is to contain some of the lower-level routines for manipulating
   linked lists for the forestry project.  All efforts will be made to keep 
   this standard C code, and therefor machine-independent.  Linked lists here
   follow the same basic structure as that used on standard Amiga linked
   lists.  There is no particular reason for this other than it was there and
   it was something to work from.

   The linklist class acts as both the head and tail of the link list.
   lh_Tail always stays as NULL and is used as the marker for looping through
   the list.  The first element starts at (MinNode*)this, where 'this' is a
   MinList*.  The last element of the list is always at
   (MinNode*)this->lh_Tail.  So, these are like "fake" elements, and the
   elements that you actually add and remove from the list are the second
   through to the second-to-last elements, inclusive.  This means that the
   node pointers in the list class remain automatically updated for all time.

   January 9, 1998:     added MinList::NodeIndex(), MinList::IndexNode()
   January 28:          added List::FindNextName()
   February 3:          added List::InsertAlphabetical()
   April 8:             added List::FindPtr()
   May 21, 1998:        no functionality added or changed, just made the
			string search and placement methods a bit more robust
			by checking for NULL, etc.
   November 6, 1998:    moved List::FindPtr() into a new class called PtrList,
			added PtrNode class
   March 8, 1999:       added FindPredName(), FindLastName() methods to the
			List class
   March 27, 2000:	optimized IndexNode() to search either way on the linked
			list
   April 4, 2000:	added PtrList::FindPtrIndex()
   May 10, 2000:	added the Big* classes
   May 11, 2000:	added LN_SuccWrap() and LN_PredWrap() methods to MinNode
   June 20, 2001:	added Replace(), couple days earlier added MinList
   Nov 30, 2001:	inlined more routines
   Dec 28, 2006:	added EmptyList()
   Jan 3, 2007:		added more AppendFromList() routines
   Jan 8, 2007:		little bit faster Reverse() routines
   Feb 20, 2007:	added MoveHead(), InsertList() routines
   Feb 28, 2007:	finished the migration to the newer classes (depreciating the stuff with a std::uint16_t counter field)
   June 11, 2007:	added FindNextPtr(), FindNextPtrIndex()
*/

#include "intel_check.h"

#ifndef __CUDACC__
#include <cstdlib>
#include <cstring>
#include <boost/algorithm/string.hpp>
#endif

#include "linklist.h"

#ifdef __CUDACC__
//using namespace HSSLowlevel_CUDA;
#else
#ifdef _DEBUG
#include <cassert>
#endif
#endif


//! Append nodes from a list onto the structure, by creating new nodes from originating list
void MinList::AppendFromList(MinList *list, MinNode *left, MinNode *right, bool could_wrap) {
	if (list->IsEmpty())							// Catch empty lists
		return;
	if (left == right) {							// Just dealing with one node, so easy
		list->Remove(left);
		AddTail(left);
		return;
	}
	MinNode *old_head = list->lh_Head;				
	if ((could_wrap) && (left != list->lh_Head))	// Re-wrap if intended
		list->MoveHead(left);

    #ifdef _DEBUG
	std::uint32_t l_index = list->NodeIndex(left);
	std::uint32_t r_index = list->NodeIndex(right);
	weak_assert(l_index != (std::uint32_t)-1);
	weak_assert(r_index != (std::uint32_t)-1);
	weak_assert(l_index < r_index);
    #endif

	std::uint32_t cnt = 1;
	MinNode *mn = left;
	while (mn != right) {							// Start at the left node and count
		cnt++;
		if (mn == old_head)							// set oldhead to left if we encounter it
			old_head = left;
		mn = mn->LN_Succ();							
	}
	if (mn == old_head)
		old_head = left;

													// Now we reorganize after the append???
	left->ln_Pred->ln_Succ = right->ln_Succ;		// 
	right->ln_Succ->ln_Pred = left->ln_Pred;		// remove the nodes
	list->lh_cnt -= cnt;							// reduce the list counter by the number of nodes we processed above

	lh_TailPred->ln_Succ = left;					// Set the tail's predecessor to left
	left->ln_Pred = lh_TailPred;					// Set left's predecessor to the tail-predecessor
	lh_TailPred = right;							// Set the tail's precessor to right
	right->ln_Succ = (MinNode *)&lh_Tail;			// Set right's successor to the tail
	lh_cnt += cnt;									// Update list counter accordingly

	if ((could_wrap) && (list->lh_Head != old_head) && (old_head != left))
		list->MoveHead(old_head);
}


void MinList::InsertList(MinList *list, MinNode *Where) {
	if (list->IsEmpty())
		return;
	list->lh_TailPred->ln_Succ = Where->ln_Succ;
	Where->ln_Succ->ln_Pred = list->lh_TailPred;
	list->lh_Head->ln_Pred = Where;
	Where->ln_Succ = list->lh_Head;
	lh_cnt += list->lh_cnt;					// keep track of how many nodes

	list->lh_Head = (MinNode *)&list->lh_Tail;		// reset the list we swiped things from
	list->lh_TailPred = (MinNode *)list;
	list->lh_cnt = 0;
}


void MinList::InsertFromList(MinList *list, MinNode *left, MinNode *right, bool could_wrap, MinNode *Where) {
	if (list->IsEmpty())
		return;
	if (left == right) {
		list->Remove(left);
		AddTail(left);
		return;
	}
	MinNode *old_head = list->lh_Head;
	if ((could_wrap) && (left != list->lh_Head))
		list->MoveHead(left);

    #ifdef _DEBUG
	std::uint32_t l_index = list->NodeIndex(left);
	std::uint32_t r_index = list->NodeIndex(right);
	weak_assert(l_index != (std::uint32_t)-1);
	weak_assert(r_index != (std::uint32_t)-1);
	weak_assert(l_index < r_index);
    #endif

	std::uint32_t cnt = 1;
	MinNode *mn = left;
	while (mn != right) {
		cnt++;
		if (mn == old_head)
			old_head = left;
		mn = mn->LN_Succ();
	}
	if (mn == old_head)
		old_head = left;

	left->ln_Pred->ln_Succ = right->ln_Succ;
	right->ln_Succ->ln_Pred = left->ln_Pred;		// remove the nodes
	list->lh_cnt -= cnt;

	Where->ln_Succ->ln_Pred = right;
	right->ln_Succ = Where->ln_Succ;
	Where->ln_Succ = left;
	left->ln_Pred = Where;
	lh_cnt += cnt;

	if ((could_wrap) && (list->lh_Head != old_head) && (old_head != left))
		list->MoveHead(old_head);
}
