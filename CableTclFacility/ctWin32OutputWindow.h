/*=========================================================================

  Program:   CABLE - CABLE Automates Bindings for Language Extension
  Module:    ctWin32OutputWindow.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
  See Copyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _ctWin32OutputWindow_h
#define _ctWin32OutputWindow_h

#include "ctUtils.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace _cable_tcl_
{

class _cable_tcl_EXPORT Win32OutputWindow
{
public:
  typedef Win32OutputWindow Self;
  
  static Win32OutputWindow* GetInstance();
  
  // New lines are converted to carriage return new lines.
  void DisplayText(const char*);
  //BTX
  static LRESULT APIENTRY WndProc(HWND hWnd, UINT message, 
                                  WPARAM wParam, LPARAM lParam);
  //ETX
private: 
  Win32OutputWindow() {}
  ~Win32OutputWindow() {}
  Win32OutputWindow(const Self&);
  void operator=(const Self&);

  static void AddText(const char*);
  static bool Initialize();
  static HWND outputWindow;
};

} // namespace _cable_tcl_

#endif
