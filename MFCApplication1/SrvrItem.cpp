
// SrvrItem.cpp : CMFCApplication1SrvrItem 类的实现
//

#include "stdafx.h"
#include "MFCApplication1.h"

#include "MFCApplication1Doc.h"
#include "SrvrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication1SrvrItem 实现

IMPLEMENT_DYNAMIC(CMFCApplication1SrvrItem, COleServerItem)

CMFCApplication1SrvrItem::CMFCApplication1SrvrItem(CMFCApplication1Doc* pContainerDoc)
	: COleServerItem(pContainerDoc, TRUE)
{
	// TODO:  在此添加一次性构造代码
	//  (如将附加剪贴板格式添加到项的数据源中)
}

CMFCApplication1SrvrItem::~CMFCApplication1SrvrItem()
{
	// TODO:  在此处添加清理代码
}

void CMFCApplication1SrvrItem::Serialize(CArchive& ar)
{
	// 如果将该项复制到剪贴板中，则框架将调用 CMFCApplication1SrvrItem::Serialize。
	//  这可以
	//  通过 OLE 回调 OnGetClipboardData 来自动执行。
	//  对于嵌入项，默认最好直接委托给文档的 Serialize 函数。
	//  如果支持链接，则只需序列化
	//  文档的一部分。

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
	// 大多数与此类似的应用程序只对绘制项的内容
	//  方面进行处理。  如果希望支持其他方面，如
	//  DVASPECT_THUMBNAIL (通过重写 OnDrawEx)，则应修改
	//  OnGetExtent 的此实现以处理
	//  其他方面。

	if (dwDrawAspect != DVASPECT_CONTENT)
		return COleServerItem::OnGetExtent(dwDrawAspect, rSize);

	// 调用 CMFCApplication1SrvrItem::OnGetExtent 以便获取整个项的
	//  HIMETRIC 单元表示的范围。  此处的默认实现
	//  只返回单元的硬编码数字。

	// TODO:  替换此任意大小

	rSize = CSize(3000, 3000);   // 3000 x 3000 HIMETRIC 单元

	return TRUE;
}

BOOL CMFCApplication1SrvrItem::OnDraw(CDC* pDC, CSize& rSize)
{
	if (!pDC)
		return FALSE;

	// 如果使用 rSize，则将此移除
	UNREFERENCED_PARAMETER(rSize);

	// TODO:  设置映射模式和范围
	//  (此范围通常与从 OnGetExtent 返回的大小相同)
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowOrg(0,0);
	pDC->SetWindowExt(3000, 3000);

	// TODO:  在此处添加绘制代码。  也可以填充 HIMETRIC 范围。
	//  所有绘制都将在图元文件设备上下文(pDC)中进行。

	return TRUE;
}


// CMFCApplication1SrvrItem 诊断

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

