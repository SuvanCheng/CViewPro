// BitmapTrace.h: interface for the BitmapTrace class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BITMAPTRACE_H__3A9C2DD3_A890_45A9_8F59_B562CC9CE749__INCLUDED_)
#define AFX_BITMAPTRACE_H__3A9C2DD3_A890_45A9_8F59_B562CC9CE749__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "include\CxImage\ximage.h"
#ifdef    _DEBUG   
	#pragma comment(lib,"lib\\CxImage\\Debug\\cximage.lib")
	#pragma comment(lib,"lib\\CxImage\\Debug\\jasper.lib")
	#pragma comment(lib,"lib\\CxImage\\Debug\\jbig.lib")
	#pragma comment(lib,"lib\\CxImage\\Debug\\Jpeg.lib")
	#pragma comment(lib,"lib\\CxImage\\Debug\\libdcr.lib")
	#pragma comment(lib,"lib\\CxImage\\Debug\\mng.lib")
	#pragma comment(lib,"lib\\CxImage\\Debug\\png.lib")
	#pragma comment(lib,"lib\\CxImage\\Debug\\Tiff.lib")
	#pragma comment(lib,"lib\\CxImage\\Debug\\zlib.lib")
#else   
	#pragma comment(lib,"lib\\CxImage\\Release\\cximage.lib")
	#pragma comment(lib,"lib\\CxImage\\Release\\jasper.lib")
	#pragma comment(lib,"lib\\CxImage\\Release\\jbig.lib")
	#pragma comment(lib,"lib\\CxImage\\Release\\Jpeg.lib")
	#pragma comment(lib,"lib\\CxImage\\Release\\libdcr.lib")
	#pragma comment(lib,"lib\\CxImage\\Release\\mng.lib")
	#pragma comment(lib,"lib\\CxImage\\Release\\png.lib")
	#pragma comment(lib,"lib\\CxImage\\Release\\Tiff.lib")
	#pragma comment(lib,"lib\\CxImage\\Release\\zlib.lib")
#endif


#define MAX_TRACEMAP_COUNT	2

class CBitmapTrace  
{
public:
	enum TraceType {
		TYPE_UNKNOW = 0,
		TYPE_ZOOM = 1,
		TYPE_RORATE = 2,
		TYPE_CROP = 3,
		TYPE_DESKEW = 4,
		TYPE_DESTAIN = 5
	};
	struct TRACERECORD{
		TraceType Type;
		double lRotateAngle;
		double cRotateAngle;
		double lZoomScale;
		double cZoomScale;
    };
	struct TRACEMAP{
		int id;
		HBITMAP hTraceBitmap;
		HBITMAP hAttachingBitmap;
		long count;
		TRACERECORD* pTraceRecords;
    };


public:
	CBitmapTrace();
	virtual ~CBitmapTrace();


private:
	void Init();
	void Release();
	HBITMAP MakeCopyBitmap(HBITMAP hBitmap);
public:
	void AddTrace(BOOL IsNewTraceBitmap,HBITMAP hBitmap,HBITMAP hAttachingBitmap,TRACERECORD* pRecord);
	BOOL CanUndo();
	BOOL CanRedo();
	void Undo();
	void Redo();
	void Clear();
	HBITMAP		GetTraceBitmap(int index=0);
	void		GetTrace(TRACERECORD* pRecord);


private:
	TRACEMAP* m_TraceMap[MAX_TRACEMAP_COUNT];
	int m_TraceId;
	int m_CurrentTraceMap_Index;
	long m_CurrentTraceRecord_Index;
	long m_TotalTraceRecord_Count;
	long m_CanUndo_Count;
	long m_CurrentUndoOfTraceMap_Index;
	long m_CurrentUndoOfTraceRecord_Index;

};

#endif // !defined(AFX_BITMAPTRACE_H__3A9C2DD3_A890_45A9_8F59_B562CC9CE749__INCLUDED_)
