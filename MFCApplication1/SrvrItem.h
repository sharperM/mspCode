
// SrvrItem.h : CMFCApplication1SrvrItem ��Ľӿ�
//

#pragma once

class CMFCApplication1SrvrItem : public COleServerItem
{
	DECLARE_DYNAMIC(CMFCApplication1SrvrItem)

// ���캯��
public:
	CMFCApplication1SrvrItem(CMFCApplication1Doc* pContainerDoc);

// ����
	CMFCApplication1Doc* GetDocument() const
		{ return reinterpret_cast<CMFCApplication1Doc*>(COleServerItem::GetDocument()); }

// ��д
	public:
	virtual BOOL OnDraw(CDC* pDC, CSize& rSize);
	virtual BOOL OnGetExtent(DVASPECT dwDrawAspect, CSize& rSize);

// ʵ��
public:
	~CMFCApplication1SrvrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void Serialize(CArchive& ar);   // Ϊ�ĵ� I/O ��д
};

