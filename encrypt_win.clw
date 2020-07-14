; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CEncrypt_winDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "encrypt_win.h"

ClassCount=3
Class1=CEncrypt_winApp
Class2=CEncrypt_winDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_ENCRYPT_WIN_DIALOG

[CLS:CEncrypt_winApp]
Type=0
HeaderFile=encrypt_win.h
ImplementationFile=encrypt_win.cpp
Filter=N

[CLS:CEncrypt_winDlg]
Type=0
HeaderFile=encrypt_winDlg.h
ImplementationFile=encrypt_winDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_FILE_LIST

[CLS:CAboutDlg]
Type=0
HeaderFile=encrypt_winDlg.h
ImplementationFile=encrypt_winDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_ENCRYPT_WIN_DIALOG]
Type=1
Class=CEncrypt_winDlg
ControlCount=6
Control1=IDC_STATIC,static,1342308352
Control2=IDC_PASSWORD,edit,1350631552
Control3=IDC_FILE_BTN,button,1342242816
Control4=IDC_ENC_BTN,button,1342242816
Control5=IDC_DEC_BTN,button,1342242816
Control6=IDC_FILE_LIST,SysListView32,1350631424

