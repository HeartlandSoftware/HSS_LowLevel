/* File: rrlea.h
   Class: RRLEA
   Originally coded by: Erik Cutts
   Description:  The RRLEA class implements a run length limited encoding array
        (RRLEA).  The RRLEA in this class creates and stores a two-dimensional
        array using two one dimensional arrays.
   NOTE:  The x,y coordinates must be greater than zero.

   RWB: 20/01/05: this class was improperly modified to use ints rather than shorts,
   so I've kept it around simply and only to handle loading legacy files and put
   the original back into use.
*/

#ifndef __RRLEA_INT_H_
#define __RRLEA_INT_H_

#include <cstdlib>
#include <cstdio>
#include <cassert>
//#include <alloc.h>
//#include <iostream.h>
#include "types.h"

#if defined(_MSC_VER) && !defined(_NO_MFC)
#include <afx.h>
#endif

#define DEFAULTXSIZ 100
#define DEFAULTYSIZ 100
#define ARRAYINC    100			// must be greater than 2
#define SHORTVAL    65535

template<class T> class RLLEA;

template<class T> class RLLEAINT
{
    friend class RLLEA<T>;
public:
        std::uint32_t m_maxXSiz;		// max size of the X axis
        std::uint32_t m_maxYSiz;		// max size of the Y axis
        std::uint32_t m_siz;			// number of indexes in the array
        std::uint32_t m_inUse;			// number of indexes of the array in use
        std::uint32_t m_numRecords;		// number of x, y coordinates to store
        std::uint32_t *m_range;			// pointer to the range array
        T *m_val;			// pointer to the value array
        RLLEAINT(std::uint32_t x, std::uint32_t y, T initVal);
        ~RLLEAINT();

#if defined(_MSC_VER) && !defined(_NO_MFC)
	void Serialize(CArchive &ar);
#endif

};

// RLLEAINT methods


//Method: RLLEAINT constuctor
//Behaviour: Initializes the private members of the RLLEA class.
template<class T> RLLEAINT<T>::RLLEAINT(std::uint32_t x, std::uint32_t y, T initVal) 
{
    m_maxXSiz = x;
    m_maxYSiz = y;
    std::uint32_t i = 0;

    //calculates the number of records
    m_numRecords = m_maxXSiz * m_maxYSiz;

    //determine the min size of the arrays needed at init
    m_inUse = (m_numRecords / SHORTVAL) + 1;
    //determine the size of the arrays to be created
    m_siz = ((m_inUse / ARRAYINC) + 1) * ARRAYINC;

    //allocates space for the arrays
    m_range = (std::uint32_t *)malloc(sizeof(std::uint32_t) * (size_t)m_siz);
    m_val = (T*)malloc(sizeof(T) * (size_t)m_siz);

	if (m_range && m_val) {
		//init the arrays
		std::uint32_t *r = m_range;
		T *v = m_val;
		while (i < m_inUse)
		{
			*(r++) = SHORTVAL;
			*(v++) = initVal;
			i++;
		}
		while (i++ < m_siz) {
			*(r++) = 0;
			*(v++) = initVal;
		}
	}
}


//Method: RLLEA destructor
//Behaviour: Frees all memory not already de-allocated.
template<class T> RLLEAINT<T>::~RLLEAINT() {
    if (m_range) free(m_range);
    if (m_val) free(m_val);
}


#if defined(_MSC_VER) && !defined(_NO_MFC)
template<class T> void RLLEAINT<T>::Serialize(CArchive &ar) 
{
	if (ar.IsLoading()) 
	{
		int version;
		ar >> version;
		if (version != 1)
			AfxThrowArchiveException(CArchiveException::badIndex, "RLLEA<>");
		ar >> m_maxXSiz >> m_maxYSiz >> m_siz >> m_inUse >> m_numRecords;
		std::uint32_t *range;
		T *val;
		range = (std::uint32_t *)realloc(m_range, sizeof(std::uint32_t) * m_siz);
		val = (T *)realloc(m_val, sizeof(T) * m_siz);
		if (range && val) {
			m_range = range;
			m_val = val;
			for (std::uint32_t i = 0; i < m_siz; i++)
				ar >> m_range[i] >> m_val[i];
		} else
			AfxThrowMemoryException();
	} 
	else {
		int version = 1;
		ar << version;
		ar << m_maxXSiz << m_maxYSiz << m_siz << m_inUse << m_numRecords;
		for (std::uint32_t i = 0; i < m_siz; i++)
			ar << m_range[i] << m_val[i];
	}
}
#endif

#endif /* __RLLEA_INT_H_ */