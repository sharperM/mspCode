
// SrvrItem.h : CMFCApplication1SrvrItem 类的接口
//

#pragma once

class CMFCApplication1SrvrItem : public COleServerItem
{
	DECLARE_DYNAMIC(CMFCApplication1SrvrItem)

// 构造函数
public:
	CMFCApplication1SrvrItem(CMFCApplication1Doc* pContainerDoc);

// 特性
	CMFCApplication1Doc* GetDocument() const
		{ return reinterpret_cast<CMFCApplication1Doc*>(COleServerItem::GetDocument()); }

// 重写
	public:
	virtual BOOL OnDraw(CDC* pDC, CSize& rSize);
	virtual BOOL OnGetExtent(DVASPECT dwDrawAspect, CSize& rSize);

// 实现
public:
	~CMFCApplication1SrvrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void Serialize(CArchive& ar);   // 为文档 I/O 重写
};

