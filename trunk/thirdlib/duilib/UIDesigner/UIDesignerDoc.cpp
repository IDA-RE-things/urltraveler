
// UIDesignerDoc.cpp : CUIDesignerDoc ���ʵ��
//

#include "stdafx.h"
#include "UIDesigner.h"

#include "UIDesignerDoc.h"
#include "DialogSkinFileNew.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUIDesignerDoc

IMPLEMENT_DYNCREATE(CUIDesignerDoc, CDocument)

BEGIN_MESSAGE_MAP(CUIDesignerDoc, CDocument)
	ON_COMMAND(ID_TEMPLATE_SAVE, &CUIDesignerDoc::OnTemplateSave)
END_MESSAGE_MAP()


// CUIDesignerDoc ����/����

CUIDesignerDoc::CUIDesignerDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CUIDesignerDoc::~CUIDesignerDoc()
{
}

BOOL CUIDesignerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)
	CDialogSkinFileNew dlg;
	dlg.m_strFileName = this->GetTitle();
	if(dlg.DoModal() == IDOK)
	{
		CString strFilePath = dlg.GetStyleFilePath();
		if(!strFilePath.IsEmpty())
			this->SetPathName(dlg.GetStyleFilePath());
		if(!dlg.m_strFileName.IsEmpty())
			this->SetTitle(dlg.m_strFileName);
	}
	else
		return FALSE;

	return TRUE;
}




// CUIDesignerDoc ���л�

void CUIDesignerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CUIDesignerDoc ���

#ifdef _DEBUG
void CUIDesignerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CUIDesignerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CUIDesignerDoc ����

void CUIDesignerDoc::OnTemplateSave()
{
	// TODO: �ڴ���������������
}

BOOL CUIDesignerDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: �ڴ����ר�ô����/����û���
	POSITION pos = this->GetFirstViewPosition();
	while(pos)
	{
		CView* pView = this->GetNextView(pos);
		CUIDesignerView* pUIView = DYNAMIC_DOWNCAST(CUIDesignerView, pView);
		ASSERT(pUIView);
		pUIView->OnSaveSkinFile(lpszPathName);
	}

	return TRUE/*CDocument::OnSaveDocument(lpszPathName)*/;
}
