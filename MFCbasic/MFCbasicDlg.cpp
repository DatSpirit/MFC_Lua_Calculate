
// MFCbasicDlg.cpp : implementation file
//
#include "pch.h"
#include "framework.h"
#include "MFCbasic.h"
#include "MFCbasicDlg.h"
#include "afxdialogex.h"
#include "resource.h"


extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}




//#include <sstream> // std::stringstream
//#include <iomanip> // định dạng số thập phân
//#include <locale>
//#include <string> 
//#include <stdexcept>  // Cần cho try/catch
//#include <atlbase.h>  // Cần cho ATL/OLE2 conversion macros
//#include <atlconv.h>  // Cần cho ATL/OLE2 conversion macros

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCbasicDlg dialog



CMFCbasicDlg::CMFCbasicDlg(CWnd* pParent )
	: CDialogEx(IDD_MFCBASIC_DIALOG, pParent)
	, m_result(0.0)
	, m_operation(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCbasicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NUM1, m_num1);
	DDX_Text(pDX, IDC_EDIT_NUM2, m_num2);
	DDX_Text(pDX, IDC_EDIT_RESULT, m_result);
	DDX_Radio(pDX, IDC_RADIO_SUM, m_operation);
}

BEGIN_MESSAGE_MAP(CMFCbasicDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CALC, &CMFCbasicDlg::OnClickedButtonCalc)
END_MESSAGE_MAP()


CMFCbasicDlg::~CMFCbasicDlg()
{
	if (m_L) {
		lua_close(m_L);
	}
}

// Định nghĩa hàm static đã khai báo ở MFCbasicDlg.h
int CMFCbasicDlg::Lua_ShowMessageBox(lua_State* L)
{
	// Lấy đối tượng CMFCbasicDlg hiện tại (cần để gọi MessageBox của MFC)
	CMFCbasicDlg* pDlg = (CMFCbasicDlg*)AfxGetMainWnd();

	// Kiểm tra và lấy tham số đầu tiên (chuỗi)
	if (lua_isstring(L, 1))
	{
		// Lấy chuỗi từ stack Lua
		const char* message = lua_tostring(L, 1);

		// Thực hiện hành động C++ (Hiển thị MFC MessageBox)
		if (pDlg)
		{
			pDlg->MessageBox(CString(message), _T("Notification from Lua"));
		}
	}

	// Trả về số lượng giá trị trả về cho Lua (0 giá trị)
	return 0;
}
// CMFCbasicDlg message handlers

BOOL CMFCbasicDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


	// Khởi tạo trạng thái Lua và tải các thư viện chuẩn
	m_L = luaL_newstate();
	if (m_L)
	{
		luaL_openlibs(m_L);

		// Đăng ký hàm C++ để Lua có thể gọi 
		lua_register(m_L, "ShowMessageBox", Lua_ShowMessageBox);

		// Tải file Lua script 
		if (luaL_dofile(m_L, "script.lua") != LUA_OK)
		{
			const char* error = lua_tostring(m_L, -1);
			CString strError;
			strError.Format(_T("Error dowload Lua script: %s"), CString(error));
			MessageBox(strError, _T("Lua Error"), MB_ICONERROR);
			lua_pop(m_L, 1);
		}
	}
	else
	{
		MessageBox(_T("Error: Không thể khởi tạo Lua State."), _T("Lua Error"), MB_ICONERROR);
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCbasicDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCbasicDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCbasicDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMFCbasicDlg::OnClickedButtonCalc()
{
	// KIỂM TRA LỖI Ô RỖNG (EMPTY INPUT)

	CString strNum1, strNum2;
	GetDlgItemText(IDC_EDIT_NUM1, strNum1);
	GetDlgItemText(IDC_EDIT_NUM2, strNum2);


	// bỏ khoảng trắng thừa ở 2 đầu
	strNum1.Trim();
	strNum2.Trim();

	if (strNum1.IsEmpty() || strNum2.IsEmpty())
	{
		MessageBox(_T("Vui lòng nhập giá trị vào cả hai ô số."), _T("Error!"), MB_ICONERROR);
		// Xóa kết quả cũ
		m_result = 0.0;
		UpdateData(FALSE);
		return;
	}

	// LẤY DỮ LIỆU TỪ UI (DDX)
	// BÁO LỖI nếu gặp ký tự và return FALSE
	if (!UpdateData(TRUE))
	{
		
		MessageBox(_T("Vui lòng nhập số hợp lệ."), _T("Error!"), MB_ICONERROR);
		m_result = 0.0;
		UpdateData(FALSE); // Xóa kết quả cũ
		return;
	}

	//  THỰC HIỆN TÍNH TOÁN
	double dResult = 0.0;


	const char* luaFunctionName = nullptr;// đảm bảo nó không trỏ đến một địa chỉ không xác định ban đầu.

	switch (m_operation)// chọn tên hàm Lua dựa trên lựa chọn Radio Button (m_operation)
	{// các phép tính 
	case 0: luaFunctionName = "LuaAdd"; break;  // cộng 
	case 1: luaFunctionName = "LuaMultiply"; break; // nhân
	case 2: luaFunctionName = "LuaSubtract"; break; // trừ
	case 3: luaFunctionName = "LuaDivide"; break;  // chia

	}

	// 1. Tìm hàm trong Lua
	lua_getglobal(m_L, luaFunctionName);

	if (lua_isfunction(m_L, -1))
	{
		lua_pushnumber(m_L, m_num1);
		lua_pushnumber(m_L, m_num2);

		// 2. Gọi hàm Lua: (2 arguments, 1 result, 0 error handler)
		if (lua_pcall(m_L, 2, 1, 0) == LUA_OK)
		{
			// 3. Lấy kết quả
			if (lua_isnumber(m_L, -1))
			{
				dResult = lua_tonumber(m_L, -1);
			}
			lua_pop(m_L, 1); // Dọn dẹp kết quả
		}
		else
		{
			// Xử lý lỗi Lua Runtime 
			const char* error = lua_tostring(m_L, -1);
			MessageBox(CString(error), _T("Lỗi Phép Toán từ Lua"), MB_ICONERROR);
			lua_pop(m_L, 1);
			m_result = 0.0;
			UpdateData(FALSE);
			return;
		}
	}
	else
	{
		// Lỗi nếu tên hàm trong Lua script sai
		MessageBox(_T("Lỗi: Không tìm thấy hàm Lua cần thiết."), _T("Lua Error"), MB_ICONWARNING);
		lua_pop(m_L, 1);
		m_result = 0.0;
		UpdateData(FALSE);
		return;
	}


	// BƯỚC 3: ĐẨY KẾT QUẢ RA UI
	m_result = dResult;
	UpdateData(FALSE); // Đẩy giá trị m_result ra IDC_EDIT_RESULT
}