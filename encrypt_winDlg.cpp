// encrypt_winDlg.cpp : implementation file
//

/*
openssl 编译
perl Configure VC-WIN32 no-asm  -DOPENSSL_USE_IPV6=0 --prefix=c:\openssl\ 
ms\do_ms 
nmake -f ms\nt.mak 
nmake -f ms\nt.mak install 

工具 -> 选项 -> 目录    下拉 Libray files  添加 C:\openssl\lib
工具 -> 选项 -> 目录    下拉 Include files 添加 C:\openssl\include
*/

#include "stdafx.h"
#include "encrypt_win.h"
#include "encrypt_winDlg.h"

#include <direct.h>

#include <openssl/aes.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <openssl/applink.c>  
#include <openssl/evp.h>

#pragma comment(lib, "libeay323.lib")
#pragma comment(lib, "ssleay323.lib")

#define IV_LEN 16
#define KEY_LEN 32 //256 bit
#define DATA_LEN 64 //data length must be : length % 16 == 0 and less than 4G Bytes

#define PASSWORD_PADDING 2048

#define EN_DIR "._已加密"
#define DE_DIR "._已解密"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEncrypt_winDlg dialog

CEncrypt_winDlg::CEncrypt_winDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEncrypt_winDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEncrypt_winDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEncrypt_winDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEncrypt_winDlg)
	DDX_Control(pDX, IDC_FILE_LIST, m_file_list);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEncrypt_winDlg, CDialog)
	//{{AFX_MSG_MAP(CEncrypt_winDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ENC_BTN, OnEncBtn)
	ON_BN_CLICKED(IDC_FILE_BTN, OnFileBtn)
	ON_BN_CLICKED(IDC_DEC_BTN, OnDecBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEncrypt_winDlg message handlers

BOOL CEncrypt_winDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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
	
	// TODO: Add extra initialization here
	DWORD dwStyle = GetWindowLong(m_file_list.m_hWnd, GWL_STYLE);
	SetWindowLong( m_file_list.m_hWnd, GWL_STYLE, dwStyle | LVS_REPORT);
	
	
	DWORD styles = m_file_list.GetExtendedStyle();
	m_file_list.SetExtendedStyle(styles|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	
	m_file_list.InsertColumn(0,"文件序号",LVCFMT_LEFT,60); 
	m_file_list.InsertColumn(1,"文件名",LVCFMT_LEFT,150); 
	m_file_list.InsertColumn(2,"操作结果",LVCFMT_CENTER,60);
	m_file_list.InsertColumn(3,"校验结果",LVCFMT_CENTER,60);
	m_file_list.InsertColumn(4,"信息",LVCFMT_LEFT,120);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CEncrypt_winDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CEncrypt_winDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CEncrypt_winDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

unsigned char* CEncrypt_winDlg::hex_dump(unsigned char *src, int src_len ,unsigned char *result){

	static unsigned char  hex[] = "0123456789abcdef";
	int len = src_len;
    while (len--) {
        *result++ = hex[*src >> 4];
        *result++ = hex[*src++ & 0xf];
    }
	*result = '\0';
	return result;
}

void CEncrypt_winDlg::OnEncBtn() 
{
	pThread = AfxBeginThread(encryptAdp,this);
}

UINT CEncrypt_winDlg::encryptAdp(LPVOID pParam) 
{
	CEncrypt_winDlg*   pDlg   =   (CEncrypt_winDlg*)pParam; 
	pDlg ->encrypt();
	return 0;
}


//openssl enc -aes-256-cbc -in  ReadMe.txt -out ReadMe.txt.aes256  -iv 670b14728ad9902aecba32e22fa4f6bd -K 91b4d142823f7d20c5f08df69122de43f35f057a988d9619f6d3138485c9a203  -nosalt  -p

void CEncrypt_winDlg::encrypt()
{
	FILE *fpr,*fpw;
	int i,n,length,path_length;
	char file_name[1024],file_aes[1024];
	char extend[24];//substr(smd5,8)+substr(dmd5,8).aes256
	CString temp;
	MD5_CTX md5_ctx,out_md5_ctx;
	unsigned char md5[MD5_DIGEST_LENGTH], out_md5[MD5_DIGEST_LENGTH], orignal_md5[MD5_DIGEST_LENGTH];
	unsigned char md5_string[MD5_DIGEST_LENGTH*2+4], sha256_string[32*2+4];
	unsigned char buffer[4096];
	unsigned char out_buffer[4096];
	//已放在读文件里处理
	//unsigned char out_buffer[4096 + EVP_MAX_BLOCK_LENGTH ]; //为防止末尾pkcs#7填充16Byte 导致溢出 当然也可以是 AES_BLOCK_SIZE，但通用env中一般用 EVP_MAX_BLOCK_LENGTH

	SHA256_CTX sha256_ctx;
	
	unsigned char iv[IV_LEN];
	unsigned char key[KEY_LEN];

	GetDlgItemText(IDC_PASSWORD,temp);
	
	if(temp.GetLength()==0){
		AfxMessageBox("密码不能为空");
		return;
	}
	
	

	length = temp.GetLength();
	for(i=0;i<PASSWORD_PADDING;i++){
			buffer[i] = temp.GetAt(i%length);	
	}
	length = PASSWORD_PADDING;

	MD5_Init(&md5_ctx);
	MD5_Update(&md5_ctx, buffer, length/2);
	MD5_Final(iv, &md5_ctx);
	//hex_dump(iv,IV_LEN,md5_string);
	//AfxMessageBox((char *)md5_string);

	SHA256_Init(&sha256_ctx);
	SHA256_Update(&sha256_ctx, buffer, length);
	SHA256_Final(key, &sha256_ctx);
	//hex_dump(key,KEY_LEN,sha256_string);
	//AfxMessageBox((char *)sha256_string);

	//创建输出文件夹
	if(m_AryFilename.GetSize()>0){
		temp = m_AryFilename.GetAt(0);
		length = temp.GetLength();
		for(i = length -1; i>0;i--)
		{
			if( '\\' == temp.GetAt(i))
			{
				//判断当前字符是否是'\'
				temp = temp.Left( i ) + "\\"+EN_DIR+"\\";
				temp.Replace("\\","\\\\");
				path_length = sprintf(file_aes,"%s",temp.GetBuffer(0));
				mkdir(temp.GetBuffer(0));
				
				break;//跳出循环
			}
		}
	}else{
		AfxMessageBox("未选择文件");
		return;
	}

	//加密开始
	GetDlgItem(IDC_ENC_BTN)->EnableWindow(FALSE);
	GetDlgItem(IDC_DEC_BTN)->EnableWindow(FALSE);

	
	EVP_CIPHER_CTX *env_ctx;
	//EVP_CipherInit_ex(env_ctx, EVP_aes_256_cbc(), NULL, key, iv, AES_ENCRYPT);
	for(i=0;i<m_AryFilename.GetSize();i++)
	{	
		m_file_list.SetItemText(i,4,"加密中...");
		temp = m_AryFilename.GetAt(i);
		temp.Replace("\\","\\\\");

		_snprintf(file_name,sizeof(file_name),"%s",temp.GetBuffer(0));
		fpr = fopen(file_name,"rb");
		if(fpr == NULL){
			m_file_list.SetItemText(i,4,"原文件打开失败");
			continue;
		}
		temp = m_AryFiletitle.GetAt(i);
		memcpy(file_aes+path_length,temp.GetBuffer(0),temp.GetLength()+1);
		fpw = fopen(file_aes,"wb");
		if(fpw == NULL){
			m_file_list.SetItemText(i,4,"创建加密文件失败");
			fclose(fpr);
			continue;
		}
		MD5_Init(&md5_ctx);
		MD5_Init(&out_md5_ctx);

		env_ctx = EVP_CIPHER_CTX_new();
		EVP_CipherInit_ex(env_ctx, EVP_aes_256_cbc(), NULL, key, iv, AES_ENCRYPT);

		while (!feof(fpr)) {
			length = fread(buffer,1,4096 - EVP_MAX_BLOCK_LENGTH,fpr);
			MD5_Update(&md5_ctx, buffer, length);
			
			//EVP_MAX_BLOCK_LENGTH
			//EVP_CIPHER_CTX_set_padding 默认是pkcs#7所以不用设置了
			EVP_CipherUpdate(env_ctx, out_buffer, &n, buffer, length);
				
			MD5_Update(&out_md5_ctx, out_buffer, n);
			fwrite(out_buffer,1,n,fpw);
		}

		EVP_CipherFinal_ex(env_ctx, out_buffer, &n);
		EVP_CIPHER_CTX_free(env_ctx);

		fwrite(out_buffer,1,n,fpw);

		MD5_Update(&out_md5_ctx, out_buffer, n);

		MD5_Final(orignal_md5, &md5_ctx);
		hex_dump(orignal_md5,MD5_DIGEST_LENGTH,md5_string);
		//temp.Format("%s",md5_string);
		//m_file_list.SetItemText(i,4,temp); //openssl dgst -md5  ReadMe.txt

		//取源文件的md5前8个
		memset(extend,0,sizeof(extend));
		memcpy(extend,md5_string,8);

		//取输出文件md5的前8个
		MD5_Final(out_md5, &out_md5_ctx);
		hex_dump(out_md5,MD5_DIGEST_LENGTH,md5_string);
		
		memcpy(extend+8,md5_string,8);
		memcpy(extend+16,".aes256",7);
		
		fclose(fpr);
		fclose(fpw);
		m_file_list.SetItemText(i,2,"加密完成");
		m_file_list.SetItemText(i,4,"校验中...");
		//check 
		fpr = fopen(file_aes,"rb");
		MD5_Init(&md5_ctx);
		env_ctx = EVP_CIPHER_CTX_new();
		EVP_CipherInit_ex(env_ctx, EVP_aes_256_cbc(), NULL, key, iv, AES_DECRYPT);
		while (!feof(fpr)) {
			length = fread(buffer,1,4096 - EVP_MAX_BLOCK_LENGTH,fpr);

			EVP_CipherUpdate(env_ctx, out_buffer, &n, buffer, length);
			
			MD5_Update(&md5_ctx, out_buffer, n);
		}
		fclose(fpr);

		EVP_CipherFinal_ex(env_ctx, out_buffer, &n);
		EVP_CIPHER_CTX_free(env_ctx);

		MD5_Update(&md5_ctx, out_buffer, n);
		MD5_Final(md5, &md5_ctx);
		if(0==memcmp(md5,orignal_md5,MD5_DIGEST_LENGTH)){
			m_file_list.SetItemText(i,3,"成功");
			sprintf((char *)buffer,"%s.%s",file_aes,extend);
			unlink((char *)buffer);
			rename(file_aes,(char *)buffer);
			
		}else{
			hex_dump(md5,MD5_DIGEST_LENGTH,md5_string);
			temp.Format("失败%s",md5_string);
			m_file_list.SetItemText(i,3,temp);
		}
		m_file_list.SetItemText(i,4,"处理完毕");
	}
	m_AryFilename.RemoveAll();
	m_AryFiletitle.RemoveAll();
	GetDlgItem(IDC_ENC_BTN)->EnableWindow(TRUE);
	GetDlgItem(IDC_DEC_BTN)->EnableWindow(TRUE);
}

void CEncrypt_winDlg::OnFileBtn() 
{
	// TODO: Add your control notification handler code here
	CString  pathName,fileName;
	CString  temp;
	int      i,j;
	CFileDialog fdlg(true, "*", "*.*", OFN_ALLOWMULTISELECT | OFN_ENABLESIZING | OFN_HIDEREADONLY,
		"所有文件(*.*)|*.*||", NULL);	
	fdlg.m_ofn.lpstrTitle="请选择需要加密/解密的文件";
	
	//最多可以打开1000个文件
	fdlg.m_ofn.nMaxFile = 1000 * MAX_PATH;
	fdlg.m_ofn.lpstrFile = new TCHAR[fdlg.m_ofn.nMaxFile];
	ZeroMemory(fdlg.m_ofn.lpstrFile, sizeof(TCHAR) * fdlg.m_ofn.nMaxFile); 
	
	if(IDOK == fdlg.DoModal())
	{
		m_AryFilename.RemoveAll();
		m_AryFiletitle.RemoveAll();
		
		POSITION pos_file;  
		pos_file = fdlg.GetStartPosition();  
		i = 0;
		while ( m_file_list.DeleteItem(0));
		while(pos_file != NULL)
		{
			pathName = fdlg.GetNextPathName(pos_file);
			
			int length = pathName.GetLength();
			for(j = length -1; j>0;j--)
			{
				if( '\\' == pathName.GetAt(j))
				{
					//判断当前字符是否是'\'
					fileName = pathName.Right(length - j -1);
					break;//跳出循环
				}
			}
			//AfxMessageBox(pathName);
			temp.Format("%d",++i);
			m_file_list.InsertItem(0,temp,0);
			m_file_list.SetItemText(0,1,pathName); 
			m_file_list.SetItemText(0,1,fileName); 
			m_file_list.SetItemText(0,2,""); 
			m_file_list.SetItemText(0,3,"");
			m_AryFilename.Add(pathName);
			m_AryFiletitle.Add(fileName);
		}
		if(i>1000){
			AfxMessageBox("只能处理1000个文件");
		}
	}
}

void CEncrypt_winDlg::OnDecBtn() 
{
	
	pThread = AfxBeginThread(decryptAdp,this);
}

UINT CEncrypt_winDlg::decryptAdp(LPVOID pParam) 
{
	CEncrypt_winDlg*   pDlg   =   (CEncrypt_winDlg*)pParam; 
	pDlg->decrypt();
	return 0;
}

void CEncrypt_winDlg::decrypt()
{

	
	FILE *fpr,*fpw;
	int i,n,length,path_length;
	char file_name[1024],file_aes[1024];
	CString temp;
	MD5_CTX md5_ctx,out_md5_ctx;
	unsigned char md5[MD5_DIGEST_LENGTH], aes_md5[MD5_DIGEST_LENGTH], orignal_md5[MD5_DIGEST_LENGTH];
	unsigned char md5_string[MD5_DIGEST_LENGTH*2+4], sha256_string[32*2+4];
	unsigned char buffer[4096];
	unsigned char out_buffer[4096];

	SHA256_CTX sha256_ctx;
	
	unsigned char iv[IV_LEN];
	unsigned char key[KEY_LEN];

	

	//创建密码

	GetDlgItemText(IDC_PASSWORD,temp);
	
	if(temp.GetLength()==0){
		AfxMessageBox("密码不能为空");
		return;
	}
	length = temp.GetLength();
	for(i=0;i<PASSWORD_PADDING;i++){
		buffer[i] = temp.GetAt(i%length);	
	}
	length = PASSWORD_PADDING;

	MD5_Init(&md5_ctx);
	MD5_Update(&md5_ctx, buffer, length/2);
	MD5_Final(iv, &md5_ctx);
	//hex_dump(iv,IV_LEN,md5_string);
	//AfxMessageBox((char *)md5_string);

	SHA256_Init(&sha256_ctx);
	SHA256_Update(&sha256_ctx, buffer, length);
	SHA256_Final(key, &sha256_ctx);
	//hex_dump(key,KEY_LEN,sha256_string);
	//AfxMessageBox((char *)sha256_string);


	//创建输出文件夹
	if(m_AryFilename.GetSize()>0){
		temp = m_AryFilename.GetAt(0);
		length = temp.GetLength();
		for(i = length -1; i>0;i--)
		{
			if( '\\' == temp.GetAt(i))
			{
				//判断当前字符是否是'\'
				temp = temp.Left( i ) + "\\"+DE_DIR+"\\";
				temp.Replace("\\","\\\\");
				path_length = sprintf(file_name,"%s",temp.GetBuffer(0));
				mkdir(temp.GetBuffer(0));
				
				break;//跳出循环
			}
		}
	}else{
		AfxMessageBox("未选择文件");
		return;
	}

	//解密开始
	GetDlgItem(IDC_ENC_BTN)->EnableWindow(FALSE);
	GetDlgItem(IDC_DEC_BTN)->EnableWindow(FALSE);
	
	EVP_CIPHER_CTX *env_ctx;
	for(i=0;i<m_AryFilename.GetSize();i++)
	{	
		m_file_list.SetItemText(i,4,"解密中...");
		temp = m_AryFilename.GetAt(i);
		temp.Replace("\\","\\\\");
		
		_snprintf(file_aes,sizeof(file_aes),"%s",temp.GetBuffer(0));
		fpr = fopen(file_aes,"rb");
		if(fpr == NULL){
			m_file_list.SetItemText(i,4,"原文件打开失败");
			continue;
		}
		
		length = m_AryFiletitle.GetAt(i).GetLength();
		temp =  m_AryFiletitle.GetAt(i).Left(length-24);
		memcpy(file_name+path_length,temp.GetBuffer(0),length-24+1);
		fpw = fopen(file_name,"wb");
		if(fpw == NULL){
			m_file_list.SetItemText(i,4,"创建解密文件失败");
			fclose(fpr);
			continue;
		}
		MD5_Init(&md5_ctx);
		MD5_Init(&out_md5_ctx);

		env_ctx = EVP_CIPHER_CTX_new();
		EVP_CipherInit_ex(env_ctx, EVP_aes_256_cbc(), NULL, key, iv, AES_DECRYPT);
		
		while (!feof(fpr)) {
			length = fread(buffer,1,4096 - EVP_MAX_BLOCK_LENGTH,fpr);
			MD5_Update(&md5_ctx, buffer, length);
			
			EVP_CipherUpdate(env_ctx, out_buffer, &n, buffer, length);

				
			MD5_Update(&out_md5_ctx, out_buffer, n);
			fwrite(out_buffer,1,n,fpw);
		}

		EVP_CipherFinal_ex(env_ctx, out_buffer, &n);
		EVP_CIPHER_CTX_free(env_ctx);

	
		fwrite(out_buffer,1,n,fpw);
		fclose(fpr);
		fclose(fpw);

		MD5_Update(&out_md5_ctx, out_buffer, n);
		MD5_Final(orignal_md5, &out_md5_ctx);
		hex_dump(orignal_md5,MD5_DIGEST_LENGTH,buffer);
		

		MD5_Final(aes_md5, &md5_ctx);
		hex_dump(aes_md5,MD5_DIGEST_LENGTH,buffer+8);
		
		temp = m_AryFiletitle.GetAt(i).Right(23);
		memcpy(buffer+16,temp.GetBuffer(0),16);
		
		
		m_file_list.SetItemText(i,2,"解密完成");
		
		//check 
		if(0==memcmp(buffer,buffer+16,16)){
			m_file_list.SetItemText(i,3,"成功");
		}else if(0 != memcmp(buffer+8,buffer+16+8,8)){
			m_file_list.SetItemText(i,3,"文件损坏");
		}else if(0!=memcmp(buffer,buffer+16,8)){
			m_file_list.SetItemText(i,3,"密码错误");
		}else{
			buffer[16] = '\0'; 
			temp.Format("失败%s",buffer);
			m_file_list.SetItemText(i,3,temp);
		}
		m_file_list.SetItemText(i,4,"处理完毕");
	}

	m_AryFilename.RemoveAll();
	m_AryFiletitle.RemoveAll();

	GetDlgItem(IDC_ENC_BTN)->EnableWindow(TRUE);
	GetDlgItem(IDC_DEC_BTN)->EnableWindow(TRUE);

}