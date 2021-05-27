// BitmapTrace.cpp: implementation of the BitmapTrace class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CViewPro.h"
#include "BitmapTrace.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void CBitmapTrace::Init()
{
	m_TraceId = 0;
	m_CurrentTraceMap_Index = 0;
	m_CurrentTraceRecord_Index = 0;
	m_TotalTraceRecord_Count = 0;

	m_CurrentUndoOfTraceMap_Index = 0;
	m_CurrentUndoOfTraceRecord_Index = 0;
	m_CanUndo_Count = 0;
	for(int i=0;i<MAX_TRACEMAP_COUNT;i++){
		m_TraceMap[i] = NULL;
	}
}

void CBitmapTrace::Release()
{
	for(int i=0;i<MAX_TRACEMAP_COUNT;i++){
		if(m_TraceMap[i] != NULL){
			TRACEMAP* ptm = m_TraceMap[i];
			if(ptm){
				if(ptm->hTraceBitmap)
					DeleteObject(ptm->hTraceBitmap);
				if(ptm->hAttachingBitmap)
					DeleteObject(ptm->hAttachingBitmap);
				
				if(ptm->pTraceRecords){
					free(ptm->pTraceRecords);
					ptm->pTraceRecords = NULL;
				}
				free(ptm);
				m_TraceMap[i] = NULL;
			}
		}
	}	
}
CBitmapTrace::CBitmapTrace()
{
	Init();
}

CBitmapTrace::~CBitmapTrace()
{
	Release();
}

HBITMAP CBitmapTrace::MakeCopyBitmap(HBITMAP hBitmap){
	HBITMAP hCopyBitmap = NULL;
	CxImage* cxi = new CxImage();
	cxi->CreateFromHBITMAP(hBitmap);
	if(cxi->IsValid()){
		hCopyBitmap = cxi->MakeBitmap();
	}
	cxi->Destroy();
	return hCopyBitmap;
}

/*
  IsNewTraceBitmap:hTraceBitmap是否是新生成的
*/
void CBitmapTrace::AddTrace(BOOL IsNewTraceBitmap,HBITMAP hTraceBitmap,HBITMAP hAttachingBitmap,TRACERECORD* pRecord)
{
	if(hTraceBitmap!=NULL){
		TRACEMAP* ptm;
		while(CanRedo()){
			ptm = m_TraceMap[m_CurrentTraceMap_Index];
			ptm->count--;
			char* p = (char*)ptm->pTraceRecords;
			p += ptm->count*sizeof(TRACERECORD);
			free(p);
			if(ptm->count==0){
				DeleteObject(ptm->hTraceBitmap);
				DeleteObject(ptm->hAttachingBitmap);
				
				free(ptm);
				m_TraceMap[m_CurrentTraceRecord_Index] = NULL;
				
			}

			if(m_TotalTraceRecord_Count>m_CanUndo_Count){
				m_TotalTraceRecord_Count--;
				m_CurrentTraceRecord_Index--;
				if(m_CurrentTraceRecord_Index<0){
					long Index = m_CurrentTraceMap_Index;
					m_CurrentTraceMap_Index--;
					if(m_CurrentTraceMap_Index<0)
						m_CurrentTraceMap_Index = MAX_TRACEMAP_COUNT + m_CurrentTraceMap_Index;
					ptm = m_TraceMap[m_CurrentTraceMap_Index];
					if(ptm!=NULL){
						m_CurrentTraceRecord_Index = ptm->count -1;
					}else{
						m_CurrentTraceMap_Index = Index; 
						m_CurrentTraceRecord_Index = 0;
					}
				}
			}

		}

		m_CurrentUndoOfTraceMap_Index=m_CurrentTraceMap_Index;
		m_CurrentUndoOfTraceRecord_Index=m_CurrentTraceRecord_Index;
/*
		CString sMessage;
		sMessage.Format("%d %d %d:%d %d %d",m_CurrentTraceMap_Index,m_CurrentTraceRecord_Index,m_CurrentUndoOfTraceMap_Index,m_CurrentUndoOfTraceRecord_Index,m_TotalTraceRecord_Count,m_CanUndo_Count);
		AfxMessageBox(sMessage);
*/
		if(pRecord->Type!=TYPE_ZOOM && pRecord->Type!=TYPE_RORATE && pRecord->Type!=TYPE_CROP && pRecord->Type!=TYPE_DESKEW && pRecord->Type!=TYPE_DESTAIN){
			return;
		}

		if(IsNewTraceBitmap){
			m_TraceId++;
		}
		if(m_TraceMap[m_CurrentTraceMap_Index]==NULL){//初次使用
			m_TraceMap[m_CurrentTraceMap_Index] = (TRACEMAP*)malloc(sizeof(TRACEMAP));
			if(m_TraceMap[m_CurrentTraceMap_Index]){
				ptm = m_TraceMap[m_CurrentTraceMap_Index];
				ptm->id = m_TraceId;
				ptm->hTraceBitmap = hTraceBitmap==NULL?NULL:MakeCopyBitmap(hTraceBitmap);
				ptm->hAttachingBitmap = hAttachingBitmap==NULL?NULL:MakeCopyBitmap(hAttachingBitmap);
				ptm->count = 0;
				ptm->pTraceRecords = NULL;
			}else{
				return;
			}
		}
		

		ptm = m_TraceMap[m_CurrentTraceMap_Index];
		if(ptm->id!=m_TraceId){
			m_CurrentTraceMap_Index++;
			m_CurrentTraceMap_Index = m_CurrentTraceMap_Index%MAX_TRACEMAP_COUNT;
			m_CurrentUndoOfTraceMap_Index = m_CurrentTraceMap_Index;

			ptm = m_TraceMap[m_CurrentTraceMap_Index];
			if(ptm==NULL){
				m_TraceMap[m_CurrentTraceMap_Index] = (TRACEMAP*)malloc(sizeof(TRACEMAP));
				if(m_TraceMap[m_CurrentTraceMap_Index]){
					ptm = m_TraceMap[m_CurrentTraceMap_Index];
				}else{
					return;
				}
			}else{
				//释放原来RECORDS所占用的内存
				ptm = m_TraceMap[m_CurrentTraceMap_Index];
				m_TotalTraceRecord_Count = m_TotalTraceRecord_Count - ptm->count;
				m_CanUndo_Count = m_CanUndo_Count - ptm->count;
				free(ptm->pTraceRecords);
			}
			ptm->id = m_TraceId;
			ptm->hTraceBitmap = hTraceBitmap==NULL?NULL:MakeCopyBitmap(hTraceBitmap);
			ptm->hAttachingBitmap = hAttachingBitmap==NULL?NULL:MakeCopyBitmap(hAttachingBitmap);
			ptm->count = 0;
			ptm->pTraceRecords = NULL;
			m_CurrentTraceRecord_Index = 0;
			m_CurrentUndoOfTraceRecord_Index = 0;	
		}


/*
		CString sMessage;
		sMessage.Format("%d:%d %d:%d %d %d",m_CurrentTraceMap_Index,m_CurrentTraceRecord_Index,m_CurrentUndoOfTraceMap_Index,m_CurrentUndoOfTraceRecord_Index,m_TotalTraceRecord_Count,m_CanUndo_Count);
		AfxMessageBox(sMessage);
*/

		ptm = m_TraceMap[m_CurrentTraceMap_Index];
		if(ptm->id==m_TraceId){
			if(pRecord->Type==TYPE_ZOOM || pRecord->Type==TYPE_RORATE || pRecord->Type==TYPE_CROP || pRecord->Type!=TYPE_DESKEW || pRecord->Type!=TYPE_DESTAIN){
				TRACERECORD* pNewTraceRecord = NULL;
				if(ptm->count==0){
					pNewTraceRecord = (TRACERECORD*)malloc(sizeof(TRACERECORD));
					pNewTraceRecord->Type = pRecord->Type;
					memcpy((char*)pNewTraceRecord,(char*)pRecord,sizeof(TRACERECORD));
					ptm->pTraceRecords = pNewTraceRecord;
					ptm->count++;
				}else{
					//CString sMessage;
					//sMessage.Format("OK:%d %d %d",ptm->count,sizeof(TRACERECORD),m_CurrentTraceMap_Index);
					char* p = (char*)ptm->pTraceRecords;
					pNewTraceRecord = (TRACERECORD*)realloc(p,(ptm->count+1)*sizeof(TRACERECORD));
					if(pNewTraceRecord==NULL){
						//AfxMessageBox("Realloc Error!");
						return;
					}
					
					ptm->pTraceRecords = pNewTraceRecord;
					p = (char*)ptm->pTraceRecords;
					p += (ptm->count*sizeof(TRACERECORD));
					pNewTraceRecord = (TRACERECORD*)(p);
					//pNewTraceRecord += (ptm->count);
					memcpy((char*)pNewTraceRecord,(char*)pRecord,sizeof(TRACERECORD));
					ptm->count++;
					m_CurrentTraceRecord_Index++;
					m_CurrentUndoOfTraceRecord_Index++;
				}
				m_TotalTraceRecord_Count++;
				m_CanUndo_Count++;
			}
		}

	}
}

BOOL CBitmapTrace::CanUndo()
{
	if(m_TotalTraceRecord_Count>0){
		if(m_CanUndo_Count>0){
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CBitmapTrace::CanRedo()
{
	if(m_TotalTraceRecord_Count>0){
		if(m_TotalTraceRecord_Count>m_CanUndo_Count){
			return TRUE;
		}
	}
	return FALSE;
}

void CBitmapTrace::Undo()
{
	if(CanUndo()){
		m_CanUndo_Count--;
		if(m_CanUndo_Count>=0 && m_CanUndo_Count<m_TotalTraceRecord_Count-1){
			m_CurrentUndoOfTraceRecord_Index--;
			if(m_CurrentUndoOfTraceRecord_Index<0){
				m_CurrentUndoOfTraceMap_Index--;
				if(m_CurrentUndoOfTraceMap_Index<0){
					m_CurrentUndoOfTraceMap_Index = MAX_TRACEMAP_COUNT + m_CurrentUndoOfTraceMap_Index;
				}
				TRACEMAP* ptm = m_TraceMap[m_CurrentUndoOfTraceMap_Index];
				m_CurrentUndoOfTraceRecord_Index = ptm->count -1;
			}
		}
/*
		CString sMessage;
		sMessage.Format("%d:%d %d %d",m_CurrentUndoOfTraceMap_Index,m_CurrentUndoOfTraceRecord_Index,m_TotalTraceRecord_Count,m_CanUndo_Count);
		AfxMessageBox(sMessage);
*/
	}
}

void CBitmapTrace::Redo()
{
	if(CanRedo()){
		m_CanUndo_Count++;
		if(m_CanUndo_Count>0 && m_CanUndo_Count<m_TotalTraceRecord_Count){
			m_CurrentUndoOfTraceRecord_Index++;
			TRACEMAP* ptm = m_TraceMap[m_CurrentUndoOfTraceMap_Index];
			if(m_CurrentUndoOfTraceRecord_Index>=ptm->count){
				m_CurrentUndoOfTraceRecord_Index = 0;
				m_CurrentUndoOfTraceMap_Index++;
				m_CurrentUndoOfTraceMap_Index = m_CurrentUndoOfTraceMap_Index % MAX_TRACEMAP_COUNT;
			}
		}
/*		
		CString sMessage;
		sMessage.Format("%d:%d %d %d",m_CurrentUndoOfTraceMap_Index,m_CurrentUndoOfTraceRecord_Index,m_TotalTraceRecord_Count,m_CanUndo_Count);
		AfxMessageBox(sMessage);
*/
	}
}

HBITMAP	CBitmapTrace::GetTraceBitmap(int index){

	TRACEMAP* ptm = m_TraceMap[m_CurrentUndoOfTraceMap_Index];
	if(ptm!=NULL){
		if(index==0)
			return MakeCopyBitmap(ptm->hTraceBitmap);
		else
			return MakeCopyBitmap(ptm->hAttachingBitmap);
	}
	return NULL;
}


void CBitmapTrace::GetTrace(TRACERECORD* pRecord)
{
	TRACEMAP* ptm = m_TraceMap[m_CurrentUndoOfTraceMap_Index];
	if(ptm!=NULL){
		TRACERECORD* pTraceRecord = ptm->pTraceRecords;
		if(pTraceRecord){
			char* p = (char*)pTraceRecord;
			p += (m_CurrentUndoOfTraceRecord_Index*sizeof(TRACERECORD));
			pTraceRecord = (TRACERECORD*)p;
			memcpy((char*)pRecord,(char*)pTraceRecord,sizeof(TRACERECORD));
		}
	}
}


void CBitmapTrace::Clear()
{
	
	Release();
	Init();

}