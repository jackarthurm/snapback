#ifndef UNICODE
#define UNICODE
#endif 

#include "windows.h"
#include <memory>
#include <iostream>
#include <string>

#include "rect/rect.hh"


const float CORNER_DETECT_BORDER_FRACTION = 0.02;
const float EDGE_DETECT_BORDER_FRACTION = 0.01;


void moveWindowToRect(HWND hwnd, DiscreteRect targetRect) {
  
  SetWindowPos(
    hwnd, 
    HWND_TOP, 
    int(targetRect.left), 
    int(targetRect.top), 
    int(targetRect.width()), 
    int(targetRect.height()),
    SWP_SHOWWINDOW | SWP_DRAWFRAME
  );
}

void maximiseWindow(HWND hwnd) {

  ShowWindow(
    hwnd, 
    SW_MAXIMIZE
  );
}


namespace Proc {

  std::unordered_map<HMONITOR, ScreenArea> monitors;

  BOOL CALLBACK monitorEnum(HMONITOR handle, HDC deviceContext, LPRECT rect, LPARAM extra) {

    if (NULL != deviceContext) {
      return FALSE;
    }

    // Get the monitor workspace rect
    MONITORINFO monitorInfo;
    monitorInfo.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(handle, &monitorInfo);

    monitors.insert(
      {
        handle, 
        ScreenArea(
          monitorInfo.rcWork.left,
          monitorInfo.rcWork.right,
          monitorInfo.rcWork.top,
          monitorInfo.rcWork.bottom
        )
      }
    );
  }

  void CALLBACK windowMoveResize(
    HWINEVENTHOOK hWinEventHook,
    DWORD event,
    HWND hwnd,
    LONG idObject,
    LONG idChild,
    DWORD idEventThread,
    DWORD dwmsEventTime
  ) {

    POINT lpPoint;
    if (!GetCursorPos(&lpPoint)) {
      return;
    }

    const HMONITOR currentMonitorHandle = MonitorFromPoint(lpPoint, MONITOR_DEFAULTTONULL);

    if (NULL == currentMonitorHandle) {
      return;
    }

    const ScreenArea monitor = monitors.at(currentMonitorHandle);

    if (
      monitor.cornerSquareTopLeft(CORNER_DETECT_BORDER_FRACTION).containsPoint(lpPoint.x, lpPoint.y)
    ) {

      const DiscreteRect targetRect = monitor.quadrantTopLeft();

      moveWindowToRect(hwnd, targetRect);
    }

    else if (
      monitor.cornerSquareTopRight(CORNER_DETECT_BORDER_FRACTION).containsPoint(lpPoint.x, lpPoint.y)
    ) {

      const DiscreteRect targetRect = monitor.quadrantTopRight();

      moveWindowToRect(hwnd, targetRect);
    }

    else if (
      monitor.cornerSquareBottomLeft(CORNER_DETECT_BORDER_FRACTION).containsPoint(lpPoint.x, lpPoint.y)
    ) {

      const DiscreteRect targetRect = monitor.quadrantBottomLeft();

      moveWindowToRect(hwnd, targetRect);
    }

    else if (
      monitor.cornerSquareBottomRight(CORNER_DETECT_BORDER_FRACTION).containsPoint(lpPoint.x, lpPoint.y)
    ) {

      const DiscreteRect targetRect = monitor.quadrantBottomRight();

      moveWindowToRect(hwnd, targetRect);
    }

    else if (
      monitor.edgeRectLeft(EDGE_DETECT_BORDER_FRACTION).containsPoint(lpPoint.x, lpPoint.y)
    ) {

      const DiscreteRect targetRect = monitor.halfLeft();

      moveWindowToRect(hwnd, targetRect);
    }

    else if (
      monitor.edgeRectRight(EDGE_DETECT_BORDER_FRACTION).containsPoint(lpPoint.x, lpPoint.y)
    ) {

      const DiscreteRect targetRect = monitor.halfRight();

      moveWindowToRect(hwnd, targetRect);
    }

    else if (
      monitor.edgeRectTop(EDGE_DETECT_BORDER_FRACTION).containsPoint(lpPoint.x, lpPoint.y)
    ) {

      maximiseWindow(hwnd);
    }

    else if (
      monitor.edgeRectBottom(EDGE_DETECT_BORDER_FRACTION).containsPoint(lpPoint.x, lpPoint.y)
    ) {

      const DiscreteRect targetRect = monitor.halfBottom();

      moveWindowToRect(hwnd, targetRect);
    }
  }
}

void getMonitors() {

  Proc::monitors.clear();

  EnumDisplayMonitors(NULL, NULL, Proc::monitorEnum, 0);
}

HWINEVENTHOOK setMoveResizeHook() {

  return SetWinEventHook(
    EVENT_SYSTEM_MOVESIZEEND, 
    EVENT_SYSTEM_MOVESIZEEND, 
    NULL,
    Proc::windowMoveResize,
    0,
    0,
    WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS
  );
}


int main() {

  getMonitors();
  const HWINEVENTHOOK moveResizeHook = setMoveResizeHook();

  MSG msg = {};
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return UnhookWinEvent(moveResizeHook);
}
