/**
 * linklist.noclang.cpp
 *
 * Copyright 2004-2023 Heartland Software Solutions Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the license at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the LIcense is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// this code wouldn't compile correctly with the clang compiler.  It could
// be re-tested to see if things are better, but leaving it here for now.

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
