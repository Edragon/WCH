# Microsoft Developer Studio Project File - Name="MyUsbLowerFilterDll" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=MyUsbLowerFilterDll - Win32 Checked
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MyUsbLowerFilterDll.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MyUsbLowerFilterDll.mak" CFG="MyUsbLowerFilterDll - Win32 Checked"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MyUsbLowerFilterDll - Win32 Checked" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "MyUsbLowerFilterDll - Win32 Free" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MyUsbLowerFilterDll - Win32 Checked"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "objchk\i386"
# PROP BASE Intermediate_Dir "objchk\i386"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "objchk\i386"
# PROP Intermediate_Dir "objchk\i386"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /Gz /MTd /W3 /Gm /GX /Zi /Od /I "$(TARGET_INC_PATH)" /I "$(CRT_INC_PATH)" /I "$(ATL_INC_PATH)\atl30" /D "_UNICODE" /D "UNICODE" /D _X86_=1 /D i386=1 /D "STD_CALL" /D CONDITION_HANDLING=1 /D NT_INST=0 /D WIN32=100 /D _NT1X_=100 /D WINNT=1 /D WIN32_LEAN_AND_MEAN=1 /D DEVL=1 /D FPO=0 /D "NDEBUG" /D DBG=1 /D "DEPRECATE_DDK_FUNCTIONS" /D _WIN32_WINNT=$(_WIN32_WINNT) /D WINVER=$(WINVER) /D _WIN32_IE=$(_WIN32_IE) /D NTDDI_VERSION=$(NTDDI_VERSION) /FR /YX /Fd"objchk\i386/MyUsbLowerFilterDll.pdb" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /o "NUL" /win32
# ADD MTL /nologo /I "$(DDK_INC_PATH)" /D "_DEBUG" /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /machine:IX86
# ADD LINK32 setupapi.lib kernel32.lib user32.lib libcmtd.lib advapi32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib ntdll.lib /nologo /dll /machine:I386 /nodefaultlib /def:"MyUsbLowerFilterDll.def" /libpath:"$(TARGET_LIB_PATH)" /libpath:"$(SDK_LIB_PATH)" /ignore:4010,4037,4039,4065,4070,4078,4087,4089,4221,4210

!ELSEIF  "$(CFG)" == "MyUsbLowerFilterDll - Win32 Free"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "objfre\i386"
# PROP BASE Intermediate_Dir "objfre\i386"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "objfre\i386"
# PROP Intermediate_Dir "objfre\i386"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /Gz /MT /W3 /GX /O2 /I "$(TARGET_INC_PATH)" /I "$(CRT_INC_PATH)" /I "$(ATL_INC_PATH)\atl30" /D "_UNICODE" /D "UNICODE" /D _X86_=1 /D i386=1 /D "STD_CALL" /D CONDITION_HANDLING=1 /D NT_INST=0 /D WIN32=100 /D _NT1X_=100 /D WINNT=1 /D WIN32_LEAN_AND_MEAN=1 /D DEVL=1 /D FPO=0 /D "NDEBUG" /D _WIN32_WINNT=$(_WIN32_WINNT) /D WINVER=$(WINVER) /D _WIN32_IE=$(_WIN32_IE) /D NTDDI_VERSION=$(NTDDI_VERSION) /YX /Fd"objfre/i386/MyUsbLowerFilterDll.pdb" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /o "NUL" /win32
# ADD MTL /nologo /I "$(DDK_INC_PATH)" /D "NDEBUG" /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /machine:IX86
# ADD LINK32 setupapi.lib kernel32.lib user32.lib libcmt.lib advapi32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib ntdll.lib /nologo /dll /machine:I386 /nodefaultlib /def:"MyUsbLowerFilterDll.def" /libpath:"$(TARGET_LIB_PATH)" /libpath:"$(SDK_LIB_PATH)" /ignore:4010,4037,4039,4065,4070,4078,4087,4089,4221,4210

!ENDIF 

# Begin Target

# Name "MyUsbLowerFilterDll - Win32 Checked"
# Name "MyUsbLowerFilterDll - Win32 Free"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\MyUsbLowerFilterDll.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\MyUsbLowerFilterDll.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\MyUsbLowerFilterDll.def
# End Source File
# Begin Source File

SOURCE=.\MyUsbLowerFilterDll.rc
# End Source File
# End Group
# Begin Source File

SOURCE=.\makefile
# End Source File
# Begin Source File

SOURCE=.\sources
# End Source File
# End Target
# End Project
