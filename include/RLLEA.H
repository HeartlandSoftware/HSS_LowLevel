/* File: rrlea.h
   Class: RRLEA
   Originally coded by: Erik Cutts
   Description:  The RRLEA class implements a run length limited encoding array
        (RRLEA).  The RRLEA in this class creates and stores a two-dimensional
        array using two one dimensional arrays.
   NOTE:  The x,y coordinates must be greater than zero.
*/

#ifndef __RRLEA_H_
#define __RRLEA_H_

#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cstdint>
//#include <alloc.h>
//#include <iostream.h>
#include "types.h"
#include "RLLEAINT.H"

#ifdef _MSC_VER

#if (!defined(__INTEL_COMPILER)) && (!defined(__INTEL_LLVM_COMPILER))
#pragma managed(push, off)
#endif

#endif

#define DEFAULTXSIZ 100
#define DEFAULTYSIZ 100
#define ARRAYINC    100              //must be greater than 2
#define SHORTVAL    65535

template<class T> class RLLEA
{
protected:
	std::uint16_t m_maxXSiz;         //max size of the X axis
	std::uint16_t m_maxYSiz;         //max size of the Y axis
	std::uint32_t m_siz;              //number of indexes in the array
	std::uint32_t m_inUse;            //number of indexes of the array in use
	std::uint32_t m_numRecords;       //number of x, y coordinates to store
	std::uint16_t *m_range;          //pointer to the range array
	T *m_val;                 //pointer to the value array
public:
	RLLEA(std::uint16_t x, std::uint16_t y, T initVal);
	void set(std::uint16_t x, std::uint16_t y, T value);
	__INLINE T get(std::uint16_t x, std::uint16_t y);
	__INLINE T get(std::uint16_t x, std::uint16_t y, struct get_faster *gf);
	__INLINE T getPrevX(struct get_faster *gf);
	__INLINE T getPrevY(struct get_faster *gf);
	__INLINE T getNextX(struct get_faster *gf);
	__INLINE T getNextY(struct get_faster *gf);
	~RLLEA();

#if defined(_MSC_VER) && !defined(_NO_MFC)
	void Serialize(CArchive &ar);
	void Capture(RLLEAINT<int> *r);
#endif

};

// RLLEA methods


//Method: RLLEA constuctor
//Behaviour: Initializes the private members of the RLLEA class.
template<class T> RLLEA<T>::RLLEA(std::uint16_t x, std::uint16_t y, T initVal) {
    m_maxXSiz = x;
    m_maxYSiz = y;
    std::uint32_t i = 0;

    //calculates the number of records
    m_numRecords = (std::uint32_t)m_maxXSiz * (std::uint32_t)m_maxYSiz;

    //determine the min size of the arrays needed at init
    m_inUse = (m_numRecords / SHORTVAL) + 1;
    //determine the size of the arrays to be created
    m_siz = ((m_inUse / ARRAYINC) + 1) * ARRAYINC;

    //allocates space for the arrays
    m_range = (std::uint16_t*)malloc(sizeof(std::uint16_t) * (size_t)m_siz);
    m_val = (T*)malloc(sizeof(T) * (size_t)m_siz);

	if (m_range && m_val) {
		//init the arrays
		std::uint16_t *r = m_range;
		T *v = m_val;
		while (i < m_inUse) {
			*(r++) = SHORTVAL;
			*(v++) = initVal;
			i++;
		}
		while (i++ < m_siz) {
			*(r++) = 0;
			*(v++) = initVal;
		}
	}

#if defined(_MSC_VER) && !defined(_NO_MFC)
	else
		AfxThrowMemoryException();
#endif

}


//Method: set
//Behaviour: Set the coordinate x, y to value.
//Exceptions: If x or y are out of the range then USER_EXCEPTION is thrown
template<class T> void RLLEA<T>::set(std::uint16_t x, std::uint16_t y, T value) {
    std::uint32_t tot = m_range[0];
    std::uint32_t prevTot = 0;
    std::uint32_t j, i = 0;
    std::uint16_t *r1, *r2;
    T *v1, *v2;

//    //determine validity of x, y coordinates
//    if( (m_maxXSiz <= x) /*|| (x < 1)*/ )
//  #ifdef _MSC_VER
//	    AfxThrowUserException();
////	return;
//  #else
//	    return; //cout<<"die, x"<<endl;
//  #endif
//
//    if( (m_maxYSiz <= y) /*|| (y < 1)*/ )
//  #ifdef _MSC_VER
//	    AfxThrowUserException();
////	return;
//  #else
//	    return; //cout<<"die, x"<<endl;
//  #endif

    std::uint32_t index = (std::uint32_t)x + (((std::uint32_t)y /*- 1*/) * (std::uint32_t)m_maxXSiz);      //calculate index of x, y

//    if( index >= m_numRecords )
//  #ifdef _MSC_VER
//	    AfxThrowUserException();
////	return;
//  #else
//	    return; //cout<<"die, x"<<endl;
//  #endif

    //determines location of index in the range array (stores in i)
        //Assumes: index is in the array
    r1 = m_range;
    while(index > tot) {
        prevTot = tot;
        i++;
        r1++;
        tot += *r1;
    }

    //if value is not already set in i, then set, else done
    if (m_val[i] != value) {

        //if insertion is in the middle of range of i
            //then insert new value in the range
        if( ((prevTot + 1) < index) && (index < tot) ) {

            m_inUse = m_inUse + 2;             //two new containers must be inserted

            //if more space is needed in the arrays, reallocate space
                //Assumes: ARRAYINC >= 2
            if(m_siz < m_inUse) {
                m_siz += ARRAYINC;
				std::uint16_t *range = (std::uint16_t*)realloc(m_range,sizeof(std::uint16_t) * m_siz);
                T *val = (T*)realloc(m_val,sizeof(T) * m_siz);
				if (range && val) {
					m_range = range;
					m_val = val;
				}

#if defined(_MSC_VER) && !defined(_NO_MFC)
				else
					AfxThrowMemoryException();
#endif

            }

            //two containers are inserted ahead of the container indexed by i
            j = m_siz - 1;
            r1 = &m_range[j];
            r2 = &m_range[j - 2];
            v1 = &m_val[j];
            v2 = &m_val[j - 2];

            while( j > (i + 1) ) {
				weak_assert(r2 >= m_range);
				weak_assert(v2 >= m_val);
                *(r1--) = *(r2--);
                *(v1--) = *(v2--);
				j--;
            }

            //update ranges and values
            m_val[i] = m_val[i+2];
            m_range[i] = (short)(index - prevTot - 1);

            i++;
            m_val[i] = value;           //INSERT VALUE
            m_range[i] = 1;

            i++;
            m_range[i] = (short)(tot - index);
        }

        //else, if insertion is at beginning of the range
        else if(index == prevTot + 1) {

            //if the range consists of only the index to be changed,
                //update it
            if(index == tot) {
                m_val[i] = value;       //INSERT VALUE
            }

            //else insert new value
            else {

                //if index is not the first range and then  compaction possible
                if( (i != 0) && (m_val[i-1] == value) ) {
                        m_range[i-1]++;
                        m_range[i]--;
                } else {    //no compaction possbile

                    m_inUse++;      //one new container must be inserted

                    //if more space is needed in the arrays, reallocate space
                    if (m_siz < m_inUse) {
                        m_siz += ARRAYINC;
                        std::uint16_t *range = (std::uint16_t*)realloc(m_range,sizeof(std::int16_t) * m_siz);
                        T *val = (T*)realloc(m_val,sizeof(T) * m_siz);
						if (range && val) {
							m_range = range;
							m_val = val;
						}

#if defined(_MSC_VER) && !defined(_NO_MFC)
						else
							AfxThrowMemoryException();
#endif

                    }

                    //insert new container ahead of old containers
                    j = m_siz - 1;
//                    r1 = &m_range[j];
//                    v1 = &m_val[j];

                    while(j > i) {
						weak_assert(j >= 1);
						m_range[j] = m_range[j - 1];
						m_val[j] = m_val[j - 1];
//                        *r1 = *(r1 - 1);
//                        *v1 = *(v1 - 1);
						j--;
                    }

                    m_val[i] = value;       //i indexes new container for value
                    m_range[i] = 1;

                    i++;
                    m_range[i]--;
                }
            }


        //else if insertion is at the end of the range,
            //insert the new value at the end of the range
        } else if(index == tot) {

            //if i is not the last range inUse and the range following i has the
                //same value then compact,
                //no new container needed
            if( (i != m_inUse - 1) && (m_val[i+1] == value) ) {
                        m_range[i+1]++;
                        m_range[i]--;
            } else {    //else no compaction possbile
                m_inUse++;              //one new container must be inserted

                //if more space is needed in the arrays, reallocate space
                    //Assumes: ARRAYINC >= 1
                if (m_siz < m_inUse) {
                    m_siz += ARRAYINC;
					std::uint16_t *range = (std::uint16_t*)realloc(m_range, sizeof(std::uint16_t) * m_siz);
                    T *val = (T*)realloc(m_val, sizeof(T) * m_siz);
					if (range && val) {
						m_range = range;
						m_val = val;
					}

#if defined(_MSC_VER) && !defined(_NO_MFC)
					else
						AfxThrowMemoryException();
#endif

                }

                //insert new container after the container indexed by i
                j = m_siz - 1;
//                r1 = &m_range[j];
//                v1 = &m_val[j];

                while( j > (i + 1) ) {
					weak_assert(j >= 1);
					m_range[j] = m_range[j - 1];
					m_val[j] = m_val[j - 1];
//					  *r1 = *(--r1);
//                    *v1 = *(--v1);
					j--;
                }

                --m_range[i];
                i++;

                //i indexes container for new value
                m_val[i] = value;
                m_range[i] = 1;
            }
        }
    }
}


struct get_faster {
	std::uint32_t	index, tot;
	void *v;
	std::uint16_t	x, y, *r;
};


template<class T> T RLLEA<T>::getPrevX(struct get_faster *gf) {
	gf->index--;
	gf->x--;
	while (gf->index <= gf->tot) {
		T* t = (T *)gf->v; t--; gf->v = t;
		gf->r--;
		gf->tot -= *(gf->r);
	}
	return *((T *)gf->v);
}


template<class T> T RLLEA<T>::getPrevY(struct get_faster *gf) {
	gf->index -= m_maxXSiz;
	gf->y--;
	while (gf->index <= gf->tot) {
		T* t = (T *)gf->v; t--; gf->v = t;
		gf->r--;
		gf->tot -= *(gf->r);
	}
	return *((T *)gf->v);
}


template<class T> T RLLEA<T>::getNextX(struct get_faster *gf) {
	gf->index++;
	gf->x++;
	while (gf->index > gf->tot) {
		T* t = (T *)gf->v; t++; gf->v = t;
		gf->r++;
		gf->tot += *(gf->r);
	}
	return *((T *)gf->v);
}


template<class T> T RLLEA<T>::getNextY(struct get_faster *gf) {
	gf->index += m_maxXSiz;
	gf->y++;
	while (gf->index > gf->tot) {
		T* t = (T *)gf->v; t++; gf->v = t;
		gf->r++;
		gf->tot += *(gf->r);
	}
	return *((T *)gf->v);
}


template<class T> T RLLEA<T>::get(std::uint16_t x, std::uint16_t y) {
	struct get_faster gf;
	gf.index = x + (y * m_maxXSiz);
	gf.tot = m_range[0];
	gf.v = m_val;
	gf.r = m_range;
	while (gf.index > gf.tot) {
		T* t = (T *)gf.v; t++; gf.v = t;
		gf.r++;
		gf.tot += *(gf.r);
	}
	return *((T *)gf.v);
}


//Method: get
//Behaviour: Returns the value at the coordinate x, y.
//Exceptions: If x or y are out of the arrays range then method exits program.
template<class T> T RLLEA<T>::get(std::uint16_t x, std::uint16_t y, struct get_faster *__gf) {
	bool dir;
	struct get_faster _gf, *gf;
	if (!__gf) {
		gf = &_gf;
		gf->index = (std::uint32_t)-1;
	} else
		gf = __gf;

	if (gf->index == (std::uint32_t)-1) {			// have to do basic initialization
		gf->index = x + (y * m_maxXSiz);
		gf->tot = m_range[0];
		//gf->i = 0;
		gf->v = m_val;
		gf->r = m_range;
		dir = true;				// need to search forward from the very start
	} else {
		if ((gf->x == (x - 1)) && (gf->y == y)) {
			gf->index++;
			gf->x++;
			dir = true;			// need to search forward to the "next" value
		} else {
			std::uint32_t new_index = x + (y * m_maxXSiz);
			if (new_index > gf->index)
				dir = true;		// the requested value is after this one so search forward
			else if (new_index < (gf->index >> 1)) {
				gf->tot = m_range[0];	// the requested value is in the first half of where we
				//gf->i = 0;		// are now, so start over from the start 'cause it's likely
				gf->v = m_val;
				gf->r = m_range;	// going to be faster
				dir = true;
			} else
				dir = false;		// the requested value is between where we are now, and half
							// of where we are, so search backwards from here
			gf->index = new_index;
		}
	}
	if (dir) {					// search forward...
		while (gf->index > gf->tot) {
			T* t = (T *)gf->v; t++; gf->v = t;
			gf->r++;
			gf->tot += *(gf->r);
		}
	} else {					// search backward
		while (gf->index <= gf->tot) {
			T* t = (T *)gf->v; t--; gf->v = t;
			gf->r--;
			gf->tot -= *(gf->r);
		}
	}
	if (__gf) {
		gf->x = x;
		gf->y = y;
	}
	return *((T *)gf->v);//m_val[gf->i];
}

//Method: RLLEA destructor
//Behaviour: Frees all memory not already de-allocated.
template<class T> RLLEA<T>::~RLLEA() {
    if (m_range) free(m_range);
    if (m_val) free(m_val);
}


#if defined(_MSC_VER) && !defined(_NO_MFC)
template<class T> void RLLEA<T>::Serialize(CArchive &ar) {
	if (ar.IsLoading()) {
		UCHAR version;
		ar >> version;
		if (version != 1)
			AfxThrowArchiveException(CArchiveException::badIndex, "RLLEA<>");
		ar >> m_maxXSiz >> m_maxYSiz >> m_siz >> m_inUse >> m_numRecords;
		std::uint16_t *range = (std::uint16_t *)realloc(m_range, sizeof(std::uint16_t) * m_siz);
		T *val = (T *)realloc(m_val, sizeof(T) * m_siz);
		if (range && val) {
			m_range = range;
			m_val = val;
			for (std::uint32_t i = 0; i < m_siz; i++)
				ar >> m_range[i] >> m_val[i];
		} else
			AfxThrowMemoryException();
	} else {
		UCHAR version = 1;
		ar << version;
		ar << m_maxXSiz << m_maxYSiz << m_siz << m_inUse << m_numRecords;
		for (std::uint32_t i = 0; i < m_siz; i++)
			ar << m_range[i] << m_val[i];
	}
}

template<class T> void RLLEA<T>::Capture(RLLEAINT<int> *r) {
	m_maxXSiz = (std::uint16_t)r->m_maxXSiz;
	m_maxYSiz = (std::uint16_t)r->m_maxYSiz;

    #ifdef DEBUG
	weak_assert((std::uint32_t)m_maxXSiz == r->m_maxXSiz);
	weak_assert((std::uint32_t)m_maxYSiz == r->m_maxYSiz);
    #endif

	m_siz = r->m_siz;
	m_inUse = r->m_inUse;
	m_numRecords = r->m_numRecords;
	m_range = (std::uint16_t *)realloc(m_range, sizeof(std::uint16_t) * m_siz);
	m_val = (T *)realloc(m_val, sizeof(T) * m_siz);
	if (m_range && m_val)
		for (std::uint32_t i = 0; i < m_siz; i++) {
			m_range[i] = (std::uint16_t)r->m_range[i];

    #ifdef _DEBUG
		weak_assert((std::uint32_t)m_range[i] == r->m_range[i]);
    #endif

		m_val[i] = (T)r->m_val[i];
	}
	else
		AfxThrowMemoryException();
}

#if (!defined(__INTEL_COMPILER)) && (!defined(__INTEL_LLVM_COMPILER))
#pragma managed(pop)
#endif

#endif

#endif /* __RLLEA_H_ */