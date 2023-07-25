/*
 * Created Date: Tuesday July 25th 2023
 * Author: Lilith
 * -----
 * Last Modified: Tuesday July 25th 2023 1:24:33 am
 * Modified By: Lilith (definitelynotagirl115169@gmail.com)
 * -----
 * Copyright (c) 2023-2023 DefinitelyNotAGirl@github
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <common.h>
#include <compiler.h>

static void strReplace(std::string& str, std::string& from, std::string& to)
{
    size_t start_pos = str.find(from);
    while (start_pos!= std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos = str.find(from, start_pos + to.length());
    }
}

static bool startsWith(std::string str, std::string prefix)
{
    uint64_t i = 0;
    while(1)
    {
        switch(str[i])
        {
            case(' '):
            case('\t'):
            case('\n'):
                break;
            default:
                goto whiteSpaceCleared;
        }
        i++;
    }
    whiteSpaceCleared:;
    return str.substr(i,prefix.length()) == prefix;
}

static void strPopFront(std::string& str, uint64_t count)
{
    if(count == 0)
        return;
    str = str.substr(count,str.length());
}

std::vector<line> getLines(std::string fname)
{
    FILE* f = fopen(fname.c_str(), "rb");
    if(f == NULL)
    {
        std::cerr << "ERROR: could not open input file " << fname << std::endl;
        exit(-1);
    }
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

    char* content__ = malloc(fsize + 1);
    fread(content__, fsize, 1, f);
    fclose(f);
    content__[fsize] = 0x00;//terminate content__ string
    std::string __content = content__;
    memset(content__,0x00,fsize);

    uint64_t Line = 1;

    {
        //cut out comments
        uint64_t ii = 0;
        uint64_t whitespace = 0;
        for(uint64_t i = 0; i < __content.length(); i++)
        {
            switch(__content[i])
            {
                case('"'):
                    while(whitespace > 0)
                    {
                        content__[ii++] = ' ';
                        whitespace--;
                    }
                    content__[ii++] = __content[i++];
                    while(__content[i] != '"' && __content[i] != 0x00)
                    {
                        switch(__content[i])
                        {
                            case('\\'):
                                switch(__content[++i])
                                {
                                    case('n'):
                                        content__[ii++] = c_newline;
                                        break;
                                    case('t'):
                                        content__[ii++] = c_horizontaltab;
                                        break;
                                    case('v'):
                                        content__[ii++] = c_verticaltab;
                                        break;
                                    case('r'):
                                        content__[ii++] = c_cariagereturn;
                                        break;
                                    case('a'):
                                        content__[ii++] = c_alert;
                                        break;
                                    case('b'):
                                        content__[ii++] = c_backspace;
                                        break;
                                    case('e'):
                                        content__[ii++] = c_escape;
                                        break;
                                    case('f'):
                                        content__[ii++] = c_formfeedpagebreak;
                                        break;
                                    case('0'):
                                        content__[ii++] = '0';
                                        break;
                                }
                                break;
                            case('\n'):
                                Line++;
                            default:
                                content__[ii++] = __content[i];
                                break;
                        }
                        i++;
                    }
                    content__[ii++] = __content[i];
                    break;
                case('\''):
                    while(whitespace > 0)
                    {
                        content__[ii++] = ' ';
                        whitespace--;
                    }
                    content__[ii++] = __content[i++];
                    while(__content[i] != '\'' && __content[i] != 0x00)
                    {
                        switch(__content[i])
                        {
                            case('\\'):
                                switch(__content[++i])
                                {
                                    case('n'):
                                        content__[ii++] = c_newline;
                                        break;
                                    case('t'):
                                        content__[ii++] = c_horizontaltab;
                                        break;
                                    case('v'):
                                        content__[ii++] = c_verticaltab;
                                        break;
                                    case('r'):
                                        content__[ii++] = c_cariagereturn;
                                        break;
                                    case('a'):
                                        content__[ii++] = c_alert;
                                        break;
                                    case('b'):
                                        content__[ii++] = c_backspace;
                                        break;
                                    case('e'):
                                        content__[ii++] = c_escape;
                                        break;
                                    case('f'):
                                        content__[ii++] = c_formfeedpagebreak;
                                        break;
                                    case('0'):
                                        content__[ii++] = '0';
                                        break;
                                }
                                break;
                            case('\n'):
                                Line++;
                            default:
                                content__[ii++] = __content[i];
                                break;
                        }
                        i++;
                    }
                    content__[ii++] = __content[i];
                    break;
                case('/'):
                    switch(__content[i+1])
                    {
                        case('/'):
                            i+=2;
                            whitespace = 0;
                            while(__content[i] != '\n' && __content[i] != 0x00)
                                i++;
                            break;
                        case('*'):
                            i+=2;
                            whitespace = 0;
                            while(!(__content[i] == '*' && __content[i+1] == '/') && __content[i]!=0x00)
                                i++;
                            i++;
                            break;
                        default:
                            goto defaultChar;
                    }
                    break;
                case(' '):
                    whitespace++;
                    break;
                case('\t'):
                    whitespace+=tabLength;
                    break;
                case('\n'):
                    Line++;
                default:
                    defaultChar:;
                    while(whitespace > 0)
                    {
                        content__[ii++] = ' ';
                        whitespace--;
                    }
                    content__[ii++] = __content[i];
            }
        }
    }

    std::string content = content__;

    std::cout << "code after comments: " << std::endl;
    std::cout << content << std::endl;

    if(options::dprintTokens)
        std::cout << "lines: " << std::endl;
    std::vector<line> lines;

    {
        char terminatorSEMI;
        char terminatorLINE;
        char terminatorCOLO;
        char terminatorSCOP;
        char terminatorSCCL;
        bool loopExit = false;
        uint64_t i = 0;
        loopStart:;
        i=0;
        terminatorSEMI = ';';
        terminatorLINE = ';';//this is on purpose
        terminatorCOLO = ':';
        terminatorSCOP = '{';
        terminatorSCCL = '}';
        if(startsWith(content,"#"))
            terminatorLINE = '\n';
        if(startsWith(content,"class"))
            terminatorCOLO = ';';

        std::string lineText;
        if(content[0] == '\n')
        {
            strPopFront(content,1);
        }
        else
        {
            while(
                content[i] != terminatorLINE
                && content[i] != terminatorSEMI
                && content[i] != terminatorCOLO
                && content[i] != terminatorSCOP
                && content[i] != terminatorSCCL
                && content[i] != 0x00)
            {
                lineText.push_back(content[i]);
                i++;
            }
            if(content[i] == 0x00)
                loopExit = true;
            else
            {
                lineText.push_back(content[i]);
                i++;
            }
            strPopFront(content,lineText.length());
            //for(uint64_t j = 0; j < lineText.length(); j++)
            //{
            //    if(lineText[j] == '\n')
            //        lineText[j] = ' ';
            //    else if(lineText[j] == '\t')
            //        lineText[j] = ' ';
            //    else if(lineText[j] == '\v')
            //        lineText[j] = ' ';
            //}
            uint64_t leadingSpace = 0;
            if(options::dprintTokens)
                std::cout <<"line: "<< lineText << std::endl;
            for(uint64_t j = 0; j < lineText.length(); j=0)
            {
                if(lineText[j] == ' ')
                {
                    lineText = lineText.substr(1,lineText.length());
                    leadingSpace++;
                    j++;
                }
                else
                    break;
            }
            line L;
            L.lineNum = Line;
            //std::cout << "leading space: " << leadingSpace << std::endl;
            L.text = lineText;
            L.file = fname;
            L.leadingSpaces = leadingSpace;
            lines.push_back(L);
        }

        if(!loopExit)
            goto loopStart;
    }

    return lines;
}