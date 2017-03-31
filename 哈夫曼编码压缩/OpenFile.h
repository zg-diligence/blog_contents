/*
 * Copyright(c) 2017 Gang Zhang
 * All rights reserved.
 * Author:Gang Zhang
 * Date:2017.3.26
 */

/*
 * Abstract:
 *   1. conversion between string and wstring.
 *   2. connect to Explorer in windows.
 */

#ifndef OPENFILE
#define OPENFILE

#include <string>
#include <ShlObj.h>
#include <tchar.h>
#include <cstdlib>
using namespace std;

/* convert wchar_t characters into string. */
string w2c(const TCHAR *wstr)
{
    size_t len = wcslen(wstr) + 1, converted = 0;
    char *str = (char*)malloc(len * sizeof(char));
    wcstombs_s(&converted, str, len, wstr, _TRUNCATE);
    string tmp = str; delete str;
    return tmp;
}

/* convert char characters into wstring. */
wstring c2w(const char *str)
{
    size_t len = strlen(str) + 1, converted = 0;
    TCHAR *wstr = (wchar_t*)malloc(len * sizeof(wchar_t));
    mbstowcs_s(&converted, wstr, len, str, _TRUNCATE);
    wstring tmp = wstr; delete wstr;
    return tmp;
}

/* open Explorer and choose file, return path of the file. */
string openFile(const TCHAR *widePath)
{
    TCHAR wpath[MAX_PATH] = { 0 };
    OPENFILENAME OpenFile = { 0 };
    OpenFile.lStructSize = sizeof(OpenFile);
    OpenFile.lpstrInitialDir = widePath;
    OpenFile.lpstrFile = wpath;
    OpenFile.nMaxFile = sizeof(wpath) / sizeof(TCHAR);
    GetOpenFileName(&OpenFile);
    return w2c(wpath);
}

#endif