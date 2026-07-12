//-------------------------------------------------------------------------------------------------
// File: parseutil.hpp
// Author: Dennis Lang
//
// Desc: Parsing utility functions.
//
//-------------------------------------------------------------------------------------------------
//
// Author: Dennis Lang - 2026
// https://landenlabs.com
//
//  
//
// ----- License ----
//
// Copyright (c) 2026 Dennis Lang
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
// of the Software, and to permit persons to whom the Software is furnished to do
// so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include "ll_stdhdr.hpp"
#include "colors.hpp"

#include <regex>
#include <set>
#include <iostream>

typedef std::vector<std::regex> PatternList;

//-------------------------------------------------------------------------------------------------
// Note: ignoreCase/unixRegEx/optionErrCnt/patternErrCnt/parseArgSet are plain,
// unsynchronized state - a single ParseUtil instance is only safe to use from
// one thread at a time (e.g. parse CLI options up front, before spawning workers).
class ParseUtil {

public:
    bool ignoreCase = false;
    bool unixRegEx = false;
    unsigned optionErrCnt = 0;
    unsigned patternErrCnt = 0;
    std::set<std::string> parseArgSet;

    void showUnknown(const char* argStr);

    std::regex getRegEx(const char* value, bool isVerbose = false);

    bool validOption(const char* validCmd, const char* possibleCmd, bool reportErr = true);
    bool validPattern(PatternList& outList, lstring& value, const char* validCmd, const char* possibleCmd, bool reportErr = true, bool isVerbose = false);
    void reportError(const char* userCmd, const char* expectMsg);
 
    bool validFile(fstream& stream, int mode, const lstring& value, const char* validCmd, const char* possibleCmd, bool reportErr = true);

    static bool FileMatches(const lstring& inName, const PatternList& patternList, bool emptyResult);
    static lstring& convertSpecialChar(lstring& inOut);
    static std::string& fmtDateTime(string& outTmStr, time_t& now);
    static lstring& getParts(
            lstring& outPart,
            const char* partSelector,
            const char* name,
            const char* ext,
            unsigned num );
};

//-------------------------------------------------------------------------------------------------
#define NOMINMAX
#include <vector>
#include <limits>

#undef max

// Split string into parts.
class Split : public std::vector<lstring> {
public:
    typedef size_t(*Find_of)(const lstring& str, const char* delimList, size_t begIdx);
    const bool keepEmpty = true;

    Split(const lstring& str, const char* delimList, Find_of find_of) {
        size_t lastPos = 0;
        // size_t pos = str.find_first_of(delimList);
        size_t pos = (*find_of)(str, delimList, 0);

        while (pos != lstring::npos) {
            if (keepEmpty || pos != lastPos)
                push_back(str.substr(lastPos, pos - lastPos));
            lastPos = pos + 1;
            // pos = str.find_first_of(delimList, lastPos);
            pos = (*find_of)(str, delimList, lastPos);
        }
        if (keepEmpty || lastPos < str.length())
            push_back(str.substr(lastPos, pos - lastPos));
    }

    // Returns the absolute position (matching find_first_of's convention) of the
    // next delimiter at/after begIdx, or lstring::npos if none remain.
    static size_t Find(const lstring& str, const char* delimList, size_t begIdx) {
        if (begIdx >= str.length())
            return lstring::npos;
        size_t absPos = begIdx + strcspn(str + begIdx, delimList);
        return (absPos < str.length()) ? absPos : lstring::npos;
    }

    Split(const lstring& str, const char* delimList, int maxSplit = std::numeric_limits<int>::max()) {
        size_t lastPos = 0;
        size_t pos = str.find_first_of(delimList);

        while (pos != lstring::npos && --maxSplit > 0) {
            if (keepEmpty || pos != lastPos)
                push_back(str.substr(lastPos, pos - lastPos));
            lastPos = pos + 1;
            pos = str.find_first_of(delimList, lastPos);
        }
        if (keepEmpty || lastPos < str.length())
            push_back(str.substr(lastPos, (maxSplit == 0) ? str.length() : pos - lastPos));
    }
};