#ifndef __HTREE_H
#define __HTREE_H

#include <stddef.h>
#include <string.h>
#if defined(_MSC_VER) && !defined(_NO_MFC)
#include <Windows.h>
#include <commctrl.h> // ACM 010110
#else
#define HTREEITEM void*
#endif
#include "types.h"
#include "hssconfig/config.h"

#ifdef _MSC_VER

#if (!defined(__INTEL_COMPILER)) && (!defined(__INTEL_LLVM_COMPILER))
#pragma managed(push, off)
#endif
#endif //_MSC_VER

#ifdef HSS_SHOULD_PRAGMA_PACK
#pragma pack(push, 4)			// force to go to 4-byte packing rules,
#endif							// to make this class as small as possible

// Created: June 8, 1999, out of TIPSY.  I changed the class name to HTree
//	(for hierarchical tree).  Some of the comments may be out of date (as in
//	referencing funny things like 'initial density', whatever that is.)


// ACM 010109
// added the ExpandNode structure for use in Brite.
// When performing operations on a tree, needed to go through the tree and record 
// the HTREEITEM values for each node and whether or not the node is expanded.
typedef struct {
    HTREEITEM m_hTreeItem;
    bool is_expanded;
} ExpandNode;

// this struct is to be used as the key for the nodes in the tree.  It has
// enough room for a 4 char (e.g. species code) or a long.  Note
// that there is a NULL char at the end of the species code; that 5th byte
// is _always_ 0.

typedef struct {
    union {
	char s[5];
	long l;
	float f;
	ExpandNode eNode; // ACM 010109
    };
} HTreeID;

// A HTree is a node in a tree.  It has a pointer to a child node.  It's
// siblings (and that sibling's sibling and so on) form a singly linked list of
// children to this node.

// this is an example of a tree.  note that even though it looks funny, i am
// trying to make a regular tree in which each node has multiple (possibly
// sorted)  children as easily as possible.
// 0
// |
// 0-----------0---------------------------0
// |           |                           |
// 0--0-----0  0--0--0-----0-----0--0--0   0-----0
// |  |     |  |  |  |     |     |  |  |   |     |

class HTree {
    public:
//  	HTree();
		// constructors that initialize the key to whatever is passed in.
		// if a char is passed, the key.s[0] = c and key.s[1 - 3] = nullptr;
	HTree( long l );
	HTree( char* s );
	HTree( char c );
	HTree( float f );
	HTree( ExpandNode e); // ACM 010109
	virtual ~HTree()			{ };

    protected:
	HTreeID m_key;
	HTree* m_child;
	HTree* m_sibling;
	short int m_num_children;

    public:
	__INLINE long  KeyLong()   const		{ return m_key.l;    };
	__INLINE float KeyFloat()  const		{ return m_key.f;    };
	__INLINE char  KeyChar()   const		{ return m_key.s[0]; };
	__INLINE const char* KeyString() const	{ return m_key.s;    };

	__INLINE HTree* Sibling() const		{ return m_sibling; };
	void Sibling(HTree* newsibling);
	void Child(HTree* newfirstchild);
	__INLINE HTree* FirstChild() const 	{ return m_child; };// get the first child in the list
	HTree* LastChild() const ;
	HTree* ChildAfter(HTree*) const;	// get the node after this child
	HTree* ChildBefore(HTree*) const;	// get the node before this child
	bool   IsChild(HTree*) const;
	HTree* ChildAtKey(char*) const;		// get the child with this key, if any.
	HTree* ChildAtKey(char) const;
	HTree* ChildAtKey(long) const;
	HTree* ChildAtKey(HTREEITEM toFind) const; // ACM 010110
	HTree* SiblingAtKey(HTree* toStart, long toFind) const; // ACM 010116
	void   SetLongKey(long NewVal); // ACM 0101116
	void   SetExpandValue(bool NewVal); // ACM 010109
	bool   GetExpandValue(); // ACM 010109
	void   SetHTreeItem(HTREEITEM NewVal);   // ACM 010109
	HTREEITEM GetHTreeItem(); // ACM 010109

		// insert a child into the tree in some position.
	void InsertAfter(HTree* addtn, HTree* aftertn);
	void InsertFirst(HTree* addtn);
	void InsertLast(HTree* addtn);
	void InsertInOrder(HTree* addtn);
	void InsertInOrderF(HTree* addtn);
	void InsertInOrderF_FileAcc(HTree* addtn);
};

#ifdef HSS_SHOULD_PRAGMA_PACK
#pragma pack(pop)			// go back to whatever the project
#endif

#ifdef _MSC_VER
#if (!defined(__INTEL_COMPILER)) && (!defined(__INTEL_LLVM_COMPILER))
#pragma managed(pop)
#endif

#endif					// settings are...

#endif // __HTREE_H
