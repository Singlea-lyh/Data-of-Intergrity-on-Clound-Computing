
// IFServerUIView.cpp : CIFServerUIView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "IFServerUI.h"
#endif

#include "IFServerUIDoc.h"
#include "IFServerUIView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIFServerUIView

IMPLEMENT_DYNCREATE(CIFServerUIView, CView)

BEGIN_MESSAGE_MAP(CIFServerUIView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CIFServerUIView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CIFServerUIView ����/����

CIFServerUIView::CIFServerUIView()
{
	// TODO: �ڴ˴���ӹ������

}

CIFServerUIView::~CIFServerUIView()
{
}

BOOL CIFServerUIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CIFServerUIView ����

void CIFServerUIView::OnDraw(CDC* /*pDC*/)
{
	CIFServerUIDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CIFServerUIView ��ӡ


void CIFServerUIView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CIFServerUIView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CIFServerUIView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CIFServerUIView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CIFServerUIView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CIFServerUIView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CIFServerUIView ���

#ifdef _DEBUG
void CIFServerUIView::AssertValid() const
{
	CView::AssertValid();
}

void CIFServerUIView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIFServerUIDoc* CIFServerUIView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIFServerUIDoc)));
	return (CIFServerUIDoc*)m_pDocument;
}
#endif //_DEBUG


// CIFServerUIView ��Ϣ�������
