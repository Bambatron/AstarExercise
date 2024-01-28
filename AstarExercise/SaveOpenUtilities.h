#pragma once

#include <iostream>
#include <fstream>
#include <tchar.h>

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <locale>
#include <codecvt>

#include "json.hpp"

std::string OpenGrid() {
    wchar_t filename[MAX_PATH];

    OPENFILENAME ofn;
    ZeroMemory(&filename, sizeof(filename));
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
    ofn.lpstrFilter = _T("Json Files\0*.json\0");
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle = _T("Select a File, yo!");
    ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn)) {
        std::cout << "You chose the file \"" << filename << "\"\n";
        std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
        return converter.to_bytes(filename);
    }
    else {
        // All this stuff below is to tell you exactly how you messed up above. 
        // Once you've got that fixed, you can often (not always!) reduce it to a 'user cancelled' assumption.
        switch (CommDlgExtendedError())
        {
        case CDERR_DIALOGFAILURE: std::cout << "CDERR_DIALOGFAILURE\n";   break;
        case CDERR_FINDRESFAILURE: std::cout << "CDERR_FINDRESFAILURE\n";  break;
        case CDERR_INITIALIZATION: std::cout << "CDERR_INITIALIZATION\n";  break;
        case CDERR_LOADRESFAILURE: std::cout << "CDERR_LOADRESFAILURE\n";  break;
        case CDERR_LOADSTRFAILURE: std::cout << "CDERR_LOADSTRFAILURE\n";  break;
        case CDERR_LOCKRESFAILURE: std::cout << "CDERR_LOCKRESFAILURE\n";  break;
        case CDERR_MEMALLOCFAILURE: std::cout << "CDERR_MEMALLOCFAILURE\n"; break;
        case CDERR_MEMLOCKFAILURE: std::cout << "CDERR_MEMLOCKFAILURE\n";  break;
        case CDERR_NOHINSTANCE: std::cout << "CDERR_NOHINSTANCE\n";     break;
        case CDERR_NOHOOK: std::cout << "CDERR_NOHOOK\n";          break;
        case CDERR_NOTEMPLATE: std::cout << "CDERR_NOTEMPLATE\n";      break;
        case CDERR_STRUCTSIZE: std::cout << "CDERR_STRUCTSIZE\n";      break;
        case FNERR_BUFFERTOOSMALL: std::cout << "FNERR_BUFFERTOOSMALL\n";  break;
        case FNERR_INVALIDFILENAME: std::cout << "FNERR_INVALIDFILENAME\n"; break;
        case FNERR_SUBCLASSFAILURE: std::cout << "FNERR_SUBCLASSFAILURE\n"; break;
        default: std::cout << "You cancelled.\n";
        }

        return "";
    }
}


void WriteNum(std::ofstream& outputFile, std::string& paramName, int value, bool comma = true) {
    outputFile << "\"" << paramName << "\": " << value;
    if (comma)
        outputFile << ",";
    outputFile << "\n";
}

template<typename Grid>
void SaveGrid(const Grid& grid) {
    // Initialize COM library
    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

    // Initialize the OPENFILENAME structure
    OPENFILENAME ofn;
    TCHAR szFileName[MAX_PATH] = { 0 };

    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = sizeof(szFileName);
    ofn.lpstrFilter = _T("Json Files (*.json)\0*.json\0");
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_OVERWRITEPROMPT;
    ofn.lpstrDefExt = _T("json");  // Set the default file extension

    // Show the Save As dialog
    if (GetSaveFileName(&ofn) == TRUE) {
        // User selected a file, you can use szFileName to get the selected file path
        std::ofstream outputFile(szFileName);

        if (outputFile.is_open()) {
            // Write data to the file
            nlohmann::json resultJson = grid.ToJson();

            // Write JSON to the file
            outputFile << std::setw(4) << resultJson << std::endl;
            
            // Close the file
            outputFile.close();

            MessageBox(NULL, _T("File saved successfully."), _T("Success"), MB_OK);
        }
        else {
            MessageBox(NULL, _T("Unable to open the file for writing."), _T("Error"), MB_OK | MB_ICONERROR);
        }
    }
    else {
        // User canceled the dialog
        MessageBox(NULL, _T("Operation canceled by user."), _T("Canceled"), MB_OK | MB_ICONEXCLAMATION);
    }

    // Release COM library
    CoUninitialize();
}