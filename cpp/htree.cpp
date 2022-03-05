#include "intel_check.h"
#include "htree.h"

HTree::HTree(char* s) {
	m_child = nullptr;
	m_sibling = nullptr;
	m_num_children = 0;
	for (short int i = 0; i < 4; i++)
		m_key.s[i] = s[i];
	m_key.s[4] = 0;
}


HTree::HTree(char c) {
	m_child = nullptr;
	m_sibling = nullptr;
	m_num_children = 0;
	m_key.s[0] = c;
	for (short int i = 1; i < 4; i++)
		m_key.s[i] = '\0';
}


HTree::HTree(long l) {
	m_child = nullptr;
	m_sibling = nullptr;
	m_num_children = 0;
	m_key.l = l;
}


HTree::HTree(float f) {
	m_child = nullptr;
	m_sibling = nullptr;
	m_num_children = 0;
	m_key.f = f;
}

// ACM 010109
HTree::HTree(ExpandNode e) {
	m_child = nullptr;
	m_sibling = nullptr;
	m_num_children = 0;
	m_key.eNode.m_hTreeItem = e.m_hTreeItem;
	m_key.eNode.is_expanded = e.is_expanded;
}

/*
HTree* HTree::FirstChild( ) const {
	return child;
}


HTree* HTree::Sibling( ) const {
	return sibling;
}
*/

void HTree::Sibling(HTree* newsibling) {
	m_sibling = newsibling;
}


void HTree::Child(HTree* newfirstchild) {
	m_child = newfirstchild;
}


HTree* HTree::LastChild() const {
	HTree* t = FirstChild();
	while (t) {
		if (!t->Sibling())
			return t;
		t = t->Sibling();
	}
	return nullptr;
}


HTree* HTree::ChildBefore(HTree* tn) const {
	HTree* t = FirstChild();
	while (t) {
		if (t->Sibling() == tn)
			return t;
		t = t->Sibling();
	}
	return nullptr;
}


HTree* HTree::ChildAfter(HTree* tn) const {
	HTree* t = FirstChild();
	while (t) {
		if (t->Sibling() == tn)
			return t->Sibling()->Sibling();
		t = t->Sibling();
	}
	return nullptr;
}


bool HTree::IsChild(HTree* tn) const {
	HTree* t = FirstChild();
	while (t) {
		if (t == tn)
			return true;
		t = t->Sibling();
	}
	return false;
}


HTree* HTree::ChildAtKey(char* str) const {
	HTree* t = FirstChild();
	while (t) {
		if (!strncmp(str, t->m_key.s, 4))
			return t;
		t = ChildAfter(t);
	}
	return nullptr;
}


HTree* HTree::ChildAtKey(long i) const {
	HTree* t = FirstChild();
	while (t) {
		if (i == t->m_key.l)
			return t;
		t = ChildAfter(t);
	}
	return nullptr;
}

// ACM 010116
HTree* HTree::SiblingAtKey(HTree* toStart, long toFind) const {
	HTree* t = toStart;
	while (t) {
	    if (toFind == t->m_key.l)
		return t;
	    else 
		t = t->Sibling();
	}
	return nullptr;
}

HTree* HTree::ChildAtKey(char c) const {
	HTree* t = FirstChild();
	while (t) {
		if (c == t->m_key.s[0])
			return t;
		t = ChildAfter(t);
	}
	return nullptr;
}

// ACM 010109
HTree* HTree::ChildAtKey(HTREEITEM toFind) const {
	HTree* t = FirstChild();
	while (t) {
		if (t->m_key.eNode.m_hTreeItem == toFind)
			return t;
		t = ChildAfter(t);
	}
	return nullptr;
}

void HTree::InsertInOrder(HTree* addtn) {
	HTree* t = FirstChild();
	if (t) {
		if (addtn->m_key.l > t->m_key.l) {
			InsertFirst(addtn);
			return;
		}
		while (t->Sibling()) {
			if (addtn->m_key.l > t->Sibling()->m_key.l)
				break;
			t = t->Sibling();
		}
		InsertAfter(addtn, t);
	} else {
		m_num_children++;
		m_child = addtn;
	}
}


void HTree::InsertInOrderF(HTree* addtn) {
	HTree* t = FirstChild();
	if (t) {
		if (addtn->m_key.f > t->m_key.f) {
			InsertFirst(addtn);
			return;
		}
		while (t->Sibling()) {
			if (addtn->m_key.f > t->Sibling()->m_key.f)
				break;
			t = t->Sibling();
		}
		InsertAfter(addtn, t);
	} else {
		m_num_children++;
		m_child = addtn;
	}
}


void HTree::InsertInOrderF_FileAcc(HTree* addtn) {
	HTree* t = FirstChild();
	if (t) {
		while (t->Sibling()) {
			if (addtn->m_key.l > t->Sibling()->m_key.l)
				break;
			t = t->Sibling();
		}
		InsertAfter(addtn, t);
	} else {
		m_num_children++;
		m_child = addtn;
	}
}


void HTree::InsertFirst(HTree *addtn) {
	m_num_children++;
	addtn->m_sibling = m_child;
	m_child = addtn;
}


void HTree::InsertLast(HTree* addtn) {
	HTree* t = LastChild();
	if (t)
		InsertAfter(addtn, t);
	else {
		m_num_children++;
		m_child = addtn;
	}
}


void HTree::InsertAfter(HTree* addtn, HTree* aftertn) {
	m_num_children++;
	addtn->Sibling(aftertn->Sibling());
	aftertn->Sibling(addtn);
}
// ACM 010116
void   HTree::SetLongKey(long NewVal) {
    m_key.l = NewVal;
}

// ACM 010109
void   HTree::SetExpandValue(bool newVal) {
	m_key.eNode.is_expanded = newVal;
}
	
// ACM 010109
bool   HTree::GetExpandValue() {
    return m_key.eNode.is_expanded;
}

// ACM 010109
void   HTree::SetHTreeItem(HTREEITEM NewVal) {
    m_key.eNode.m_hTreeItem = NewVal;   
}

// ACM 010109
HTREEITEM HTree::GetHTreeItem() {
    return m_key.eNode.m_hTreeItem;
}
