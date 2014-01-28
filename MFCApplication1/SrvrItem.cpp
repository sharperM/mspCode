
// SrvrItem.cpp : CMFCApplication1SrvrItem ���ʵ��
//

#include "stdafx.h"
#include "MFCApplication1.h"

#include "MFCApplication1Doc.h"
#include "SrvrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication1SrvrItem ʵ��

IMPLEMENT_DYNAMIC(CMFCApplication1SrvrItem, COleServerItem)

CMFCApplication1SrvrItem::CMFCApplication1SrvrItem(CMFCApplication1Doc* pContainerDoc)
	: COleServerItem(pContainerDoc, TRUE)
{
	// TODO:  �ڴ����һ���Թ������
	//  (�罫���Ӽ������ʽ��ӵ��������Դ��)
}

CMFCApplication1SrvrItem::~CMFCApplication1SrvrItem()
{
	// TODO:  �ڴ˴�����������
}

void CMFCApplication1SrvrItem::Serialize(CArchive& ar)
{
	// ���������Ƶ��������У����ܽ����� CMFCApplication1SrvrItem::Serialize��
	//  �����
	//  ͨ�� OLE �ص� OnGetClipboardData ���Զ�ִ�С�
	//  ����Ƕ���Ĭ�����ֱ��ί�и��ĵ��� Serialize ������
	//  ���֧�����ӣ���ֻ�����л�
	//  �ĵ���һ���֡�

	if (!IsLinkedItem())
	{
		CMFCApplication1Doc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		if (pDoc)
			pDoc->Serialize(ar);
	}
}

BOOL CMFCApplication1SrvrItem::OnGetExtent(DVASPECT dwDrawAspect, CSize& rSize)
{
	// �����������Ƶ�Ӧ�ó���ֻ�Ի����������
	//  ������д���  ���ϣ��֧���������棬��
	//  DVASPECT_THUMBNAIL (ͨ����д OnDrawEx)����Ӧ�޸�
	//  OnGetExtent �Ĵ�ʵ���Դ���
	//  �������档

	if (dwDrawAspect != DVASPECT_CONTENT)
		return COleServerItem::OnGetExtent(dwDrawAspect, rSize);

	// ���� CMFCApplication1SrvrItem::OnGetExtent �Ա��ȡ�������
	//  HIMETRIC ��Ԫ��ʾ�ķ�Χ��  �˴���Ĭ��ʵ��
	//  ֻ���ص�Ԫ��Ӳ�������֡�

	// TODO:  �滻�������С

	rSize = CSize(3000, 3000);   // 3000 x 3000 HIMETRIC ��Ԫ

	return TRUE;
}

BOOL CMFCApplication1SrvrItem::OnDraw(CDC* pDC, CSize& rSize)
{
	if (!pDC)
		return FALSE;

	// ���ʹ�� rSize���򽫴��Ƴ�
	UNREFERENCED_PARAMETER(rSize);

	// TODO:  ����ӳ��ģʽ�ͷ�Χ
	//  (�˷�Χͨ����� OnGetExtent ���صĴ�С��ͬ)
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowOrg(0,0);
	pDC->SetWindowExt(3000, 3000);

	// TODO:  �ڴ˴���ӻ��ƴ��롣  Ҳ������� HIMETRIC ��Χ��
	//  ���л��ƶ�����ͼԪ�ļ��豸������(pDC)�н��С�

	return TRUE;
}


// CMFCApplication1SrvrItem ���

#ifdef _DEBUG
void CMFCApplication1SrvrItem::AssertValid() const
{
	COleServerItem::AssertValid();
}

void CMFCApplication1SrvrItem::Dump(CDumpContext& dc) const
{
	COleServerItem::Dump(dc);
}
#endif

