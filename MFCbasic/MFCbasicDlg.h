
// MFCbasicDlg.h : header file
//

#pragma once


// Bọc include Lua trong extern "C" để đảm bảo liên kết C++ với thư viện C
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}


// CMFCbasicDlg dialog
class CMFCbasicDlg : public CDialogEx
{
// Construction
public:
	CMFCbasicDlg(CWnd* pParent = nullptr);	// standard constructor

	~CMFCbasicDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCBASIC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// THÀNH VIÊN DỮ LIỆU VÀ HÀM LUA
protected:
	HICON m_hIcon;


	// KHAI BÁO THÀNH VIÊN LUA STATE
	lua_State* m_L = nullptr; // Khởi tạo bằng nullptr 


// Hàm C++ được đăng ký và gọi từ Lua 
	static int Lua_ShowMessageBox(lua_State* L);
	

public:
	double m_num1;
	double m_num2;
	double m_result;
	int m_operation;
	afx_msg void OnClickedButtonCalc();
	
// Implementation
protected:

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

};
