/*
This file was created by Paul Barvinko (pbarvinko@yahoo.com).
This file is distributed "as is", e.g. there are no warranties 
and obligations and you could use it in your applications on your
own risk. 
Your comments and questions are welcome.
If using in your applications, please mention author in credits for your app.
*/
//#include "stdafx.h"
#ifdef _MSC_VER
#include "intel_check.h"
class CRenderTarget;
#include <afx.h>
#endif
#include "intel_check.h"
#if __has_include(<mathimf.h>)
#include <mathimf.h>
#else
#include <cmath>
#endif

#include "cpoints.h"
#include <float.h>


CPointsCollection::CPointsCollection()
{
	max_x = min_x = max_y = min_y = 0;
}


CPointsCollection::CPointsCollection(CPointsCollection* pcol)
{
	if (!pcol)
	{
		max_x = min_x = max_y = min_y = 0;
		return;
	}
	max_x = pcol->max_x;
	min_x = pcol->min_x;
	max_y = pcol->max_y;
	min_y = pcol->min_y;
	XYC_Point ssp;
	points.reserve(pcol->GetSize());
	for (int i=0; i<pcol->GetSize(); i++)
	{
		if (pcol->GetPoint(i, &ssp) != -1)
		{
			points.push_back(ssp);
		}
	}
}


void CPointsCollection::PadRoom(int size)
{
	if (points.capacity() < (points.size() + size))
		points.reserve(points.size() + size + 200);
}


int CPointsCollection::AddPoint(XYC_Point *csp, BOOL bReScan)
{
	return AddPoint2(csp->x, csp->y, csp->m_color, csp->m_pointColor, csp->m_modeFlag, bReScan);
}


int CPointsCollection::AddPoint(double _x, double _y, COLORREF color, XYC_MODE mode, BOOL bReScan) {
	return AddPoint2(_x, _y, color, color, mode, bReScan);
}


int CPointsCollection::AddPoint2(double _x, double _y, COLORREF color, COLORREF pointColor, XYC_MODE mode, BOOL bReScan)
{
	int index;
	if ((bReScan) && (!(mode & XYC_MODE_BREAK)))
    {
		if (GetSize() == 0)
        {
			max_x = min_x = _x;
			max_y = min_y = _y;
		}
        else
        {
			if (max_x < _x) max_x = _x;
			if (min_x > _x) min_x = _x;
			if (max_y < _y) max_y = _y;
			if (min_y > _y) min_y = _y;
		}
	}
	XYC_Point ssp(_x, _y, color, pointColor, mode);
	XYC_Point spoint;
	long array_size = GetSize();
	if (points.size() == points.capacity())
		points.reserve(points.size() + 200);

	points.push_back(ssp);
	index = array_size;
	return index;
}


void CPointsCollection::RemovePoint(int index, BOOL bReScan)
{
	if (index>=0 && index<GetSize()) {
		points.erase(points.begin() + index);
		if (bReScan) RescanPoints();	
	}
}


int CPointsCollection::GetPoint(int index, double* x, double *y, COLORREF *color, XYC_MODE *mode) const
{
	XYC_Point ssp;
	if (GetPoint(index, &ssp) != -1)
    {
		*x = ssp.x;
		*y = ssp.y;
		*color = ssp.m_color;
		*mode = ssp.m_modeFlag;
		return index;
	}
	return -1;
}


void CPointsCollection::RescanPoints()
{
	int size = GetSize();
	if (size == 0)
	{
		max_x = min_x = max_y = min_y = 0;
		return;
	}
	XYC_Point ssp;
	GetPoint(0, &ssp);
	min_x = max_x = ssp.x;
	min_y = max_y = ssp.y;
	for (int i=1; i<size; i++)
    {
		GetPoint(i, &ssp);
		if (!(ssp.m_modeFlag & XYC_MODE_BREAK))
        {
			if (ssp.x < min_x)
            {
				min_x = ssp.x;
			}
            else
            {
				if (ssp.x>max_x)
                {
					max_x = ssp.x;
				}
			}
		}
		if (!(ssp.m_modeFlag & XYC_MODE_BREAK))
        {
			if (ssp.y < min_y)
            {
				min_y = ssp.y;
			}
            else
            {
				if (ssp.y > max_y)
                {
					max_y = ssp.y;
				}
			}
		}
	}
}


int CPointsCollection::EditPoint(int index, double x, double y, COLORREF color, XYC_MODE mode, BOOL bRescan)
{
	if (index<0 || index>=GetSize()) return -1;
	int ret;
	XYC_Point ssp(x, y, color, mode);
	//if bSortX not specified - just replace given point
	RemovePoint(index, false);
    InsertPoint(index, ssp.x, ssp.y, color, mode, bRescan);
    ret = index;
	return ret;
}


int CPointsCollection::InsertPoint(int index, double x, double y, COLORREF color, XYC_MODE mode, BOOL bRescan)
{
	return InsertPoint2(index, x, y, color, color, mode, bRescan);
}


int CPointsCollection::InsertPoint2(int index, double x, double y, COLORREF color, COLORREF pointColor, XYC_MODE mode, BOOL bRescan)
{
	if (index<0 || index>GetSize())
    {
		weak_assert(false);
		return -1;
	}
	XYC_Point ssp(x, y, color, pointColor, mode);
	int ret;
    //this is just append point
	if (index == GetSize())
	{
		ret = AddPoint(&ssp, bRescan/*, &res*/);
	}
    else
	{
		if (points.size() == points.capacity())
			points.reserve(points.size() + 200);
		points.insert(points.begin() + index, ssp);
		ret = index;
    }
	return ret;
}


void CPointsCollection::RemoveAll()
{
	points.clear();
	max_x = min_x = max_y = min_y = 0;
}


int CPointsCollection::AddPointDist(double _x, double _y, double Dist, COLORREF color, COLORREF pointColor, XYC_MODE mode, BOOL bReScan)
{
	int index=AddPoint2(_x,_y, color, pointColor, mode, bReScan);
	points[index].m_dblDistance=Dist;

    #ifdef DEBUG
	XYC_Point pt = points[index];
    #endif

	return index;
}


int CPointsCollection::InsertPointDist(int index, double x, double y, double distance, COLORREF color, COLORREF pointColor, XYC_MODE mode, BOOL bReScan)
{
	int ind=InsertPoint2(index, x, y, color, pointColor, mode, bReScan);
	points[ind].m_dblDistance=distance;
	return ind;
}


template void __cdecl copy_call<XYC_Point>(APTR from, APTR to);
template void* __cdecl init_call<XYC_Point>(APTR init);
template void __cdecl deinit_call<XYC_Point>(APTR deinit);
