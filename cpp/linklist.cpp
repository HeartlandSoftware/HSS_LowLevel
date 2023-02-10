/**
 * linklist.cpp
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


MinNode* MinNode::LN_SuccWrap() const {
	MinNode* node = ln_Succ;

#ifdef DEBUG
	weak_assert(node);
#endif							  

	if (!node->ln_Succ) {
		long** l = (long**)node;
		node = (MinNode*)(--l);
		node = node->ln_Succ;
	}
	return node;
}


MinNode* MinNode::LN_PredWrap() const {
	MinNode* node = ln_Pred;

#ifdef DEBUG
	weak_assert(node);
#endif							  

	if (!node->ln_Pred) {
		long** l = (long**)node;
		node = (MinNode*)(++l);
		node = node->ln_Pred;
	}
	return node;
}


MinList::MinList() {
	lh_Head = (MinNode *)&lh_Tail;
	lh_Tail = nullptr;
	lh_TailPred = (MinNode *)this;
	lh_cnt = 0;
}
    	 

MinList::MinList(MinList &&list) {
	lh_Head = (MinNode *)&lh_Tail;
	lh_Tail = nullptr;
	lh_TailPred = (MinNode *)this;
	lh_cnt = 0;

	AppendList(&list);
}
    	 

void MinList::Insert(MinNode *New, MinNode *Where) {
	Where->ln_Succ->ln_Pred = New;
	New->ln_Succ = Where->ln_Succ;
	New->ln_Pred = Where;
	Where->ln_Succ = New;
	lh_cnt++;
}


MinNode *MinList::RemHead() {
	if (IsEmpty()) 
		return nullptr;
	MinNode *node = lh_Head;
	Remove(node);
	return node;
}


MinNode *MinList::RemTail() {
	if (IsEmpty())
		return nullptr;
	MinNode *node = lh_TailPred;
	Remove(node);
	return node;
}


void MinList::Remove(MinNode *Node) {
	Node->ln_Pred->ln_Succ = Node->ln_Succ;
	Node->ln_Succ->ln_Pred = Node->ln_Pred;
	lh_cnt--;
}


void MinList::Replace(MinNode *Remove, MinNode *Insert) {
	Insert->ln_Succ = Remove->ln_Succ;
	Insert->ln_Succ->ln_Pred = Insert;
	Insert->ln_Pred = Remove->ln_Pred;
	Insert->ln_Pred->ln_Succ = Insert;
}


void MinList::Swap(MinNode *node1, MinNode *node2) {
	MinNode *n = node1->ln_Pred;
	n->ln_Succ = node2;
	node1->ln_Pred = node2->ln_Pred;
	node1->ln_Pred->ln_Succ = node1;
	node2->ln_Pred = n;
	
	n = node1->ln_Succ;
	n->ln_Pred = node2;
	node1->ln_Succ = node2->ln_Succ;
	node1->ln_Succ->ln_Pred = node1;
	node2->ln_Succ = n;
}


// ***** Basic List Commands *************************************************

bool MinList::NodeHasIndex(const MinNode* Node) const {
	std::uint32_t idx = NodeIndex(Node);
	return (idx != (std::uint32_t)-1) ? true : false;
}

//!Returns the index of *Node
std::uint32_t MinList::NodeIndex(const MinNode *Node) const {
	std::uint32_t cnt = 0;
	MinNode *node = lh_Head;		// Set *node to the head
	while (node->ln_Succ) {			// Run through successors of node
		if (node == Node)
			return cnt;
		node = node->ln_Succ;
		cnt++;						// Increment count until we reach the node we specified
	}
	return (std::uint32_t)-1;
}

//!Returns the MinNode at specified index
MinNode *MinList::IndexNode(std::uint32_t index) const {
	MinNode *node;
	if (!lh_cnt)						// If we have no nodes, abort
		return nullptr;
	if (!index)							// If we want index=0, return the head
		return lh_Head;
	if (index <= (lh_cnt >> 1)) {		// If index <= half of the number of nodes, we traverse forwards
		node = lh_Head;
		while (index) {					//   Decrement index until = 0 and then return that node
			index--;
			node = node->ln_Succ;
		}
		return node;					
	} else {							// Else index is in the "top half"
		if (index >= lh_cnt)			// Check for out of bounds
			return nullptr;
		index = (std::uint32_t)(lh_cnt - index - 1);
		node = lh_TailPred;
		while (index) {					// Run through the predecessors, decrementing index
			index--;					//  until we reach the desired node
			node = node->ln_Pred;
		}
		return node;
	}
}


//!Reverses the order of nodes in the structure
void MinList::ReverseOrder(reverser_callback fcn, APTR parm) {
	if (IsEmpty())
		return;

	MinNode *t1 = lh_Head, *t2;

	while (t2 = t1->ln_Succ) {			// Set t2 as the successor to Head
		t1->ln_Succ = t1->ln_Pred;		// Swap successor with predecessor
		t1->ln_Pred = t2;				// Set the predecessor as t2
		if (fcn)
			fcn(parm, t1);				// Run callback on head
		t1 = t2;						// Head now equals t2
	}

	t2 = lh_TailPred;					// t2 becomes pred to tail
	lh_TailPred = lh_Head;				// pred to tail becomes head
	lh_Head = t2;						// set the head to t2
	lh_Head->ln_Pred = (MinNode *)&lh_Head;		// set the pred of Head to head
	lh_TailPred->ln_Succ = (MinNode *)&lh_Tail;	// set the pred of tail to tail
}


//!Append a list of nodes onto the structure
void MinList::AppendList(MinList *list) {
	if (list->IsEmpty())
		return;
	lh_TailPred->ln_Succ = list->lh_Head;			// take our current last node and make it's 'next' point to the 1st node on the list to swipe from
	lh_TailPred->ln_Succ->ln_Pred = lh_TailPred;	// take that 1st node and make its 'pred' point to the last thing on our list
	lh_TailPred = list->lh_TailPred;				// update our pointer to the last element on our list
	lh_TailPred->ln_Succ = (MinNode *)&lh_Tail;		// make sure that last element knows that it's on the end of 'this' list, not the old list
	lh_cnt += list->lh_cnt;							// keep track of how many nodes

	list->lh_Head = (MinNode *)&list->lh_Tail;		// reset the list we swiped things from
	list->lh_TailPred = (MinNode *)list;
	list->lh_cnt = 0;
}


void MinList::MoveHead(MinNode *NewHead) {
	if (IsEmpty())
		return;
	if (lh_Head == NewHead)
		return;

    #ifdef _DEBUG
	weak_assert(NodeIndex(NewHead) != (std::uint32_t)-1);
    #endif

	lh_TailPred->ln_Succ = lh_Head;
	lh_Head->ln_Pred = lh_TailPred;				// this makes the list completely circular and kinda detached from the list header

	lh_Head = NewHead;					// this code re-inserts the list header and "breaks the circle"
	lh_TailPred = NewHead->ln_Pred;	
	lh_Head->ln_Pred = (MinNode *)&lh_Head;
	lh_TailPred->ln_Succ = (MinNode *)&lh_Tail;
}


MinList *MinList::FindList(const MinNode *node) {
	while (node->LN_Pred())
		node = node->LN_Pred();
	return (MinList *)node;
}


void MinList::ReverseOrder(MinNode *left, MinNode *right, bool could_wrap, reverser_callback fcn, APTR parm) {

    #ifdef _DEBUG
HSS_PRAGMA_WARNING_PUSH
HSS_PRAGMA_GCC(GCC diagnostic ignored "-Wunused-but-set-variable")
HSS_PRAGMA_CLANG(clang diagnostic ignored "-Wunused-but-set-variable")
	const MinNode *tail, *head_orig = lh_Head;
	std::uint32_t	l_idx1 = NodeIndex(left), r_idx1 = NodeIndex(right),
		l_idx2 = l_idx1, r_idx2 = r_idx1;
	weak_assert(l_idx1 != (std::uint32_t)-1);
	weak_assert(r_idx1 != (std::uint32_t)-1);
HSS_PRAGMA_WARNING_POP
    #endif

	if (!left)
		return;
	if (!right)
		return;
	if (left == right)
		return;
	MinNode *old_head = lh_Head;
	if ((could_wrap) && (left != lh_Head)) {
		MoveHead(left);

    #ifdef _DEBUG
		l_idx2 = NodeIndex(left);
		r_idx2 = NodeIndex(right);
    #endif
//								// move left to the head of the list so that we can easily reverse a set of nodes without
//		lh_TailPred->ln_Succ = lh_Head;			// concern about wrapping around the list
//		lh_Head->ln_Pred = lh_TailPred;
//
//		lh_Head = left;
//		lh_TailPred = left->ln_Pred;
//		lh_Head->ln_Pred = (MinNode *)&lh_Head;
//		lh_TailPred->ln_Succ = (MinNode *)&lh_Tail;

    #ifdef _DEBUG
		tail = lh_TailPred;
		weak_assert(lh_Head == left);
		weak_assert(!lh_Tail);
		weak_assert(tail == left->LN_PredWrap());
    #endif

	}
	MinNode *t1 = left, *t2;

	while ((t2 = t1->ln_Succ)) {
		t1->ln_Succ = t1->ln_Pred;
		t1->ln_Pred = t2;

    #ifdef _DEBUG
		if ((could_wrap) && (left != lh_Head)) {
//			weak_assert(lh_Head == left);
			weak_assert(tail == lh_TailPred);
		}
		weak_assert(!lh_Tail);
//			weak_assert(t2->ln_Succ);				// this can happen if you are reversing the entire list - t2 becomes the end marker
		weak_assert(t1->ln_Succ);
		weak_assert(t1->ln_Pred);
		weak_assert(t2->ln_Pred);
    #endif

		if (fcn)
			fcn(parm, t1);					// walk along each node, reversing its direction as we encounter it, and calling any
		if (t1 == right)				// callback we were provided
			break;
		t1 = t2;
	}

    #ifdef _DEBUG
	if ((could_wrap) && (left != lh_Head)) {
//		weak_assert(lh_Head == left);
		weak_assert(tail == lh_TailPred);
	}
	weak_assert(!lh_Tail);
    #endif

	t1 = left->ln_Succ;					// once this is done, we have to do a bit of clean-up with the neighbouring nodes
	left->ln_Succ = right->ln_Pred;
	right->ln_Pred = t1;
	right->ln_Pred->ln_Succ = right;
	left->ln_Succ->ln_Pred = left;
/*
    #ifdef _DEBUG
	if ((could_wrap) && (left != lh_Head)) {
		weak_assert(lh_Head == right);
		weak_assert(tail == lh_TailPred);
	}
	weak_assert(!lh_Tail);
    #endif
*/
	if ((could_wrap) && (lh_Head != old_head)) {
		MoveHead(old_head);
//								// move the head of the list back to where we were - of course, the next entry or the tail
//		lh_TailPred->ln_Succ = lh_Head;			// may have changed...
//		lh_Head->ln_Pred = lh_TailPred;
//
//		lh_Head = old_head;
//		lh_TailPred = old_head->ln_Pred;
//		lh_Head->ln_Pred = (MinNode *)&lh_Head;
//		lh_TailPred->ln_Succ = (MinNode *)&lh_Tail;
	}

    #ifdef _DEBUG
//	weak_assert(left_orig == left);
//	weak_assert(right_orig == right);
	weak_assert(head_orig == lh_Head);
	weak_assert(old_head == head_orig);
	weak_assert(lh_Head == old_head);
	weak_assert(!lh_Tail);
    #endif

}


#ifndef __CUDACC__

void List::InsertAlphabetical(Node *node, bool case_sensitive) {
	Node *from = (Node *)lh_Head;
	if (node->ln_Name)
		while (from->LN_Succ()) {
			if (from->ln_Name) {
				if (case_sensitive) {
					if (_tcscmp(from->ln_Name, node->ln_Name) > 0)
						break;
				} else {
					if (_tcsicmp(from->ln_Name, node->ln_Name) > 0)
						break;
				}
			}
			from = from->LN_Succ();
		}
	Insert(node, from->LN_Pred());
}


Node *List::FindName(const TCHAR *Name, bool case_sensitive) const {
	if (Name) {
		Node *node = (Node *)lh_Head;
		while (node->LN_Succ()) {
			if (node->ln_Name) {
				if (case_sensitive) {
					if (!_tcscmp(Name, node->ln_Name))
						return node;
				} else {
					if (!_tcsicmp(Name, node->ln_Name))
						return node;
				}
			}
			node = node->LN_Succ();
		}
	}
	return nullptr;
}


Node *List::FindNextName(Node *from, const TCHAR *Name, bool case_sensitive) const {
	Node *node = from->LN_Succ();
	if (!node)
		return nullptr;
	while (node->LN_Succ()) {
		if (node->ln_Name) {
			if (case_sensitive) {
				if (!_tcscmp(Name, node->ln_Name))
					return node;
			} else {
				if (!_tcsicmp(Name, node->ln_Name))
					return node;
			}
		}
		node = node->LN_Succ();
	}
	return nullptr;
}


Node *List::FindPredName(Node *from, const TCHAR *Name, bool case_sensitive) const {
	Node *node = from->LN_Pred();
	if (!node)
		return nullptr;
	while (node->LN_Pred()) {
		if (node->ln_Name) {
			if (case_sensitive) {
				if (!_tcscmp(Name, node->ln_Name))
					return node;
			} else {
				if (!_tcsicmp(Name, node->ln_Name))
					return node;
			}
		}
		node = node->LN_Pred();
	}
	return nullptr;
}


Node *List::FindLastName(const TCHAR *Name, bool case_sensitive) const {
	if (Name) {
		Node *node = (Node *)lh_TailPred;
		while (node->LN_Pred()) {
			if (node->ln_Name) {
				if (case_sensitive) {
					if (!_tcscmp(Name, node->ln_Name))
						return node;
				} else {
					if (!_tcsicmp(Name, node->ln_Name))
						return node;
				}
			}
			node = node->LN_Pred();
		}
	}
	return nullptr;
}


std::uint32_t List::GetNameCount(const TCHAR *Name, bool case_sensitive) const {
	std::uint32_t cnt = 0;
	if (Name) {
		Node *node = (Node *)lh_Head;
		while (node->LN_Succ()) {
			if (node->ln_Name) {
				if (case_sensitive) {
					if (!_tcscmp(Name, node->ln_Name))
						cnt++;
				} else {
					if (!_tcsicmp(Name, node->ln_Name))
						cnt++;
				}
            }
			node = node->LN_Succ();
		}
	}
	return cnt;
}


void SList::InsertAlphabetical(SNode *node, bool case_sensitive) {
	SNode *from = (SNode *)lh_Head;
	while (from->LN_Succ()) {
		if (case_sensitive) {
			if (from->ln_Name.compare(node->ln_Name) > 0)
				break;
		} else {
			if (boost::iequals(from->ln_Name, node->ln_Name))
				break;
		}
		from = from->LN_Succ();
	}
	Insert(node, from->LN_Pred());
}


SNode *SList::FindName(const tstring &Name, bool case_sensitive) const {
	SNode *node = (SNode *)lh_Head;
	while (node->LN_Succ()) {
		if (case_sensitive) {
			if (!Name.compare(node->ln_Name))
				return node;
		} else {
			if (boost::iequals(Name, node->ln_Name))
				return node;
		}
		node = node->LN_Succ();
	}
	return nullptr;
}


SNode *SList::FindNextName(SNode *from, const tstring &Name, bool case_sensitive) const {
	SNode *node = from->LN_Succ();
	if (!node)
		return nullptr;
	while (node->LN_Succ()) {
		if (case_sensitive) {
			if (!Name.compare(node->ln_Name))
				return node;
		} else {
			if (boost::iequals(Name, node->ln_Name))
				return node;
		}
		node = node->LN_Succ();
	}
	return nullptr;
}


SNode *SList::FindPredName(SNode *from, const tstring &Name, bool case_sensitive) const {
	SNode *node = from->LN_Pred();
	if (!node)
		return nullptr;
	while (node->LN_Pred()) {
		if (case_sensitive) {
			if (!Name.compare(node->ln_Name))
				return node;
		}
		else {
			if (boost::iequals(Name, node->ln_Name))
				return node;
		}
		node = node->LN_Pred();
	}
	return nullptr;
}


SNode *SList::FindLastName(const tstring &Name, bool case_sensitive) const {
	SNode *node = (SNode *)lh_TailPred;
	while (node->LN_Pred()) {
		if (case_sensitive) {
			if (!Name.compare(node->ln_Name))
				return node;
		}
		else {
			if (boost::iequals(Name, node->ln_Name))
				return node;
		}
		node = node->LN_Pred();
	}
	return nullptr;
}


std::uint32_t SList::GetNameCount(const tstring &Name, bool case_sensitive) const {
	std::uint32_t cnt = 0;
	SNode *node = (SNode *)lh_Head;
	while (node->LN_Succ()) {
		if (case_sensitive) {
			if (!Name.compare(node->ln_Name))
				cnt++;
		} else {
			if (boost::iequals(Name, node->ln_Name))
				cnt++;
		}
		node = node->LN_Succ();
	}
	return cnt;
}

#endif /* __CUDACC__ */


void PtrList::InsertAscending(PtrNode *node) {
	PtrNode *from = (PtrNode *)lh_Head;
	while (from->LN_Succ()) {
		if (from->ln_Ptr < node->ln_Ptr)
			break;
		from = from->LN_Succ();
	}
	Insert(node, from->LN_Pred());
}


PtrNode *PtrList::FindPtr(const APTR ptr) const {
	if ((ptr) && GetCount()) {
		PtrNode *node = (PtrNode *)lh_Head;
		while (node->LN_Succ()) {
			if (ptr == node->ln_Ptr)
				return node;
			node = node->LN_Succ();
		}
	}
	return nullptr;
}


void PtrList::FindPtr(const APTR ptr, PtrList &targetList) {
	if ((ptr) && GetCount()) {
		PtrNode *node = (PtrNode *)lh_Head;
		while (node->LN_Succ()) {
			if (ptr == node->ln_Ptr) {
				PtrNode *succ_node = node->LN_Succ();
				Remove(node);
				targetList.AddTail(node);
				node = succ_node;
			} else
				node = node->LN_Succ();
		}
	}
}


PtrNode *PtrList::FindNextPtr(const PtrNode *continue_from, const APTR ptr) const {
	if ((ptr) && GetCount()) {
		const PtrNode *node;
		if (continue_from)
			node = continue_from;
		else	node = (PtrNode *)lh_Head;
		while (node->LN_Succ()) {
			if (ptr == node->ln_Ptr)
				return (PtrNode *)node;
			node = node->LN_Succ();
		}
	}
	return nullptr;
}


std::uint32_t PtrList::FindPtrIndex(const APTR ptr) const {
	if (ptr) {
		std::uint32_t index = 0;
		PtrNode *node = (PtrNode *)lh_Head;
		while (node->LN_Succ()) {
			if (ptr == node->ln_Ptr)
				return index;
			index++;
			node = node->LN_Succ();
		}
	}
	return (std::uint32_t)-1;
}


std::uint32_t PtrList::FindNextPtrIndex(const std::uint32_t continue_from, const APTR ptr) const {
	if (ptr) {
		std::uint32_t index = continue_from;
		PtrNode *node = IndexNode(continue_from);
		if (node)
			while (node->LN_Succ()) {
				if (ptr == node->ln_Ptr)
					return index;
				index++;
				node = node->LN_Succ();
			}
	}
	return (std::uint32_t)-1;
}


std::uint32_t PtrList::GetPtrCount(const APTR ptr) const {
	std::uint32_t cnt = 0;
	PtrNode *node = (PtrNode *)lh_Head;
	while (node->LN_Succ()) {
		if (ptr == node->ln_Ptr)
			cnt++;
		node = node->LN_Succ();
	}
	return cnt;
}

//#include <afx.h>

bool MinList::VerifyListCount() const {
	std::uint32_t cnt = 0;
	MinNode *node = lh_Head;
	while (node->LN_Succ()) {
		cnt++;
		weak_assert(cnt);
/*		if (!cnt) {
			MessageBox(0, "Looped around!", "hi", MB_OK);
		}
		if (node->LN_Succ()->LN_Pred() != node) {
			MessageBox(0, "Bad fwd pointer", "hi", MB_OK);
		}
		if (node->LN_Pred()->LN_Succ() != node) {
			MessageBox(0, "Bad reverse pointer", "hi", MB_OK);
		}
*/
//		((!cnt) || !::AfxAssertFailedLine("linklist.cpp", __LINE__) || (__debugbreak(), 0));
		node = node->LN_Succ();
	}
//	((cnt == lh_cnt) || !::AfxAssertFailedLine("linklist.cpp", __LINE__) || (__debugbreak(), 0));
	weak_assert(cnt == lh_cnt);
//	if (cnt != lh_cnt) {
//		MessageBox(0, "Badd!", "hi", MB_OK);
//	}
	return (cnt == lh_cnt);
}
//#endif
