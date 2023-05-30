/**
 * config.h
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

/*
This file is loosely drived from code created by Paul Barvinko (pbarvinko@yahoo.com).
This file is distributed "as is", e.g. there are no warranties 
and obligations and you could use it in your applications on your
own risk. 
Your comments and questions are welcome.
If using in your applications, please mention author in credits for your app.
*/

#ifndef __HSS_POINTS_H_
#define __HSS_POINTS_H_

#include "types.h"

HSS_PRAGMA_DISABLE_MSVC_WARNING(4100)

#include <vector>
#include "vvector.h"


typedef LONG	XYC_MODE;

#ifdef HSS_SHOULD_PRAGMA_PACK
#pragma pack(push, 8)
#endif

class XYC_Point {
public:
					// old modes - -1 = same color, -2 = break, -3 = area/circle?
	COLORREF m_color, m_pointColor;	// to describe the color for drawing with other points
	XYC_MODE m_modeFlag;	// how to interpret the color, if it's a break, etc.
	ULONG	m_pad;
	double x, y;

#define XYC_MODE_CONTINUECOLOR	((XYC_MODE)(0x00000001))
#define XYC_MODE_BREAK		((XYC_MODE)(0x00000002))
#define XYC_MODE_CIRCLE		((XYC_MODE)(0x00000004))
#define XYC_MODE_SQUARE		((XYC_MODE)(0x00000010))
#define XYC_MODE_AREA		((XYC_MODE)(0x00000008))

	double m_dblDistance;

	XYC_Point() = default;
	XYC_Point(double X, double Y, COLORREF c, XYC_MODE mode = 0)		{ x = X; y = Y; m_color = c; m_pointColor = c; m_modeFlag = mode; m_dblDistance = 0.0;}
	XYC_Point(double X, double Y, COLORREF c, COLORREF pc, XYC_MODE mode = 0){x = X; y = Y; m_color = c; m_pointColor = pc; m_modeFlag = mode; m_dblDistance = 0.0; }
};


class CPointsCollection {
public:
	CPointsCollection();
	CPointsCollection(CPointsCollection* pcol);

	int AddPoint(XYC_Point *csp, BOOL bReScan);
	int AddPoint(double _x, double _y, COLORREF color, XYC_MODE mode, BOOL bReScan);
	int AddPoint2(double _x, double _y, COLORREF color, COLORREF pointColor, XYC_MODE mode, BOOL bReScan);
	int AddPointDist(double _x, double _y, double Dist, COLORREF color, COLORREF pointColor, XYC_MODE mode, BOOL bReScan);

	int AddBreak(BOOL bReScan)					{ return AddPoint(0.0, 0.0, (COLORREF)0, XYC_MODE_BREAK, bReScan); }
	int InsertBreak(int index, BOOL bReScan)			{ return InsertPoint(index, 0.0, 0.0, (COLORREF)0, XYC_MODE_BREAK, bReScan); }

	void RemovePoint(int index, BOOL bReScan);
	void RemoveAll();

	void PadRoom(int size);

	int EditPoint(int index, double x, double y, COLORREF color, XYC_MODE mode, BOOL bRescan);
	int InsertPoint(int index, double x, double y, COLORREF color, XYC_MODE mode, BOOL bRescan);
	int InsertPoint2(int index, double x, double y, COLORREF color, COLORREF pointColor, XYC_MODE mode, BOOL bRescan);
	int InsertPointDist(int index, double x, double y, double distance, COLORREF color, COLORREF pointColor, XYC_MODE mode, BOOL bReScan);

	int GetPoint(int index, double* x, double *y, COLORREF *color, XYC_MODE *mode) const;
	int GetPoint(int index, XYC_Point *result) const		{ *result = points.at(index); return index; };

	int GetSize() const						{ return (int)points.size(); };
	void RescanPoints();

	double max_x, max_y, min_x, min_y;

protected:
	vvector<XYC_Point> points;
};

#ifdef HSS_SHOULD_PRAGMA_PACK
#pragma pack(pop)			// go back to whatever the project settings were
#endif

#endif