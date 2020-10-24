# Microsoft Developer Studio Project File - Name="WinEVE" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=WinEVE - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "WinEsim.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "WinEsim.mak" CFG="WinEVE - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "WinEVE - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "WinEVE - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "WinEVE - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "WinEVE - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MTd /W3 /GX /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "WinEVE - Win32 Release"
# Name "WinEVE - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\src\CyclesChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\src\CyclesView.cpp
# End Source File
# Begin Source File

SOURCE=.\src\DataChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\src\DataDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\src\DataView.cpp
# End Source File
# Begin Source File

SOURCE=.\src\FPDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\src\FRegDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\src\InstructView.cpp
# End Source File
# Begin Source File

SOURCE=.\src\IOChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\src\IOView.cpp
# End Source File
# Begin Source File

SOURCE=.\src\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\src\MemDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\src\MultiDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\src\PipeChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\src\pipeline.cpp
# End Source File
# Begin Source File

SOURCE=.\src\PipeView.cpp
# End Source File
# Begin Source File

SOURCE=.\src\RegChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\src\RegDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\src\RegView.cpp
# End Source File
# Begin Source File

SOURCE=.\src\StatChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\src\StatView.cpp
# End Source File
# Begin Source File

SOURCE=.\src\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\src\utils.cpp
# End Source File
# Begin Source File

SOURCE=.\src\WinEVE.cpp
# End Source File
# Begin Source File

SOURCE=.\WinEVE.rc
# End Source File
# Begin Source File

SOURCE=.\src\WinEVEDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\src\WinEVEView.cpp

!IF  "$(CFG)" == "WinEVE - Win32 Release"

# ADD CPP /O1

!ELSEIF  "$(CFG)" == "WinEVE - Win32 Debug"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\src\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\src\CyclesChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\src\CyclesView.h
# End Source File
# Begin Source File

SOURCE=.\src\DataChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\src\DataDialog.h
# End Source File
# Begin Source File

SOURCE=.\src\DataView.h
# End Source File
# Begin Source File

SOURCE=.\src\FontCtrl.h
# End Source File
# Begin Source File

SOURCE=.\src\FPDialog.h
# End Source File
# Begin Source File

SOURCE=.\src\FRegDialog.h
# End Source File
# Begin Source File

SOURCE=.\src\InstructView.h
# End Source File
# Begin Source File

SOURCE=.\src\IOChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\src\IOView.h
# End Source File
# Begin Source File

SOURCE=.\src\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\src\MemDialog.h
# End Source File
# Begin Source File

SOURCE=.\src\MultiDialog.h
# End Source File
# Begin Source File

SOURCE=.\src\mytypes.h
# End Source File
# Begin Source File

SOURCE=.\src\PipeChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\src\PipeView.h
# End Source File
# Begin Source File

SOURCE=.\src\RegChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\src\RegDialog.h
# End Source File
# Begin Source File

SOURCE=.\src\RegView.h
# End Source File
# Begin Source File

SOURCE=.\src\simulator.h
# End Source File
# Begin Source File

SOURCE=.\src\StatChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\src\StatView.h
# End Source File
# Begin Source File

SOURCE=.\src\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\src\TemplDef.h
# End Source File
# Begin Source File

SOURCE=.\src\utils.h
# End Source File
# Begin Source File

SOURCE=.\src\WinEVE.h
# End Source File
# Begin Source File

SOURCE=.\src\WinEVEDoc.h
# End Source File
# Begin Source File

SOURCE=.\src\WinEVEView.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=".\res\Eve sim.ico"
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon3.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon4.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon5.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon6.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon7.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WinEVE.ico
# End Source File
# Begin Source File

SOURCE=.\res\WinEVE.rc2
# End Source File
# Begin Source File

SOURCE=.\res\WinEVEDoc.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
