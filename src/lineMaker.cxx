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
    //PRINT_DEBUG
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);  /* same as rewind(f); */
    //PRINT_DEBUG

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
                case('`'):
                case('"'):
                {
                    char limiter = __content[i];
                    while(whitespace > 0)
                    {
                        content__[ii++] = ' ';
                        whitespace--;
                    }
                    content__[ii++] = __content[i++];
                    while(__content[i] != limiter && __content[i] != 0x00)
                    {
                        switch(__content[i])
                        {
                            //case('\\'):
                            //    switch(__content[++i])
                            //    {
                            //        case('n'):
                            //            content__[ii++] = c_newline;
                            //            break;
                            //        case('t'):
                            //            content__[ii++] = c_horizontaltab;
                            //            break;
                            //        case('v'):
                            //            content__[ii++] = c_verticaltab;
                            //            break;
                            //        case('r'):
                            //            content__[ii++] = c_cariagereturn;
                            //            break;
                            //        case('a'):
                            //            content__[ii++] = c_alert;
                            //            break;
                            //        case('b'):
                            //            content__[ii++] = c_backspace;
                            //            break;
                            //        case('e'):
                            //            content__[ii++] = c_escape;
                            //            break;
                            //        case('f'):
                            //            content__[ii++] = c_formfeedpagebreak;
                            //            break;
                            //        case('0'):
                            //            content__[ii++] = '0';
                            //            break;
                            //    }
                            //    break;
                            //case('\n'):
                            //    Line++;
                            default:
                                content__[ii++] = __content[i];
                                break;
                        }
                        i++;
                    }
                    content__[ii++] = __content[i];
                    break;
                }
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
                            //case('\\'):
                            //    switch(__content[++i])
                            //    {
                            //        case('n'):
                            //            content__[ii++] = c_newline;
                            //            break;
                            //        case('t'):
                            //            content__[ii++] = c_horizontaltab;
                            //            break;
                            //        case('v'):
                            //            content__[ii++] = c_verticaltab;
                            //            break;
                            //        case('r'):
                            //            content__[ii++] = c_cariagereturn;
                            //            break;
                            //        case('a'):
                            //            content__[ii++] = c_alert;
                            //            break;
                            //        case('b'):
                            //            content__[ii++] = c_backspace;
                            //            break;
                            //        case('e'):
                            //            content__[ii++] = c_escape;
                            //            break;
                            //        case('f'):
                            //            content__[ii++] = c_formfeedpagebreak;
                            //            break;
                            //        case('0'):
                            //            content__[ii++] = '0';
                            //            break;
                            //    }
                            //    break;
                            case('\n'):
                                content__[ii++] = '\n';
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
                        //case('/'):
                        //    i+=2;
                        //    whitespace = 0;
                        //    while(__content[i] != '\n' && __content[i] != 0x00)
                        //        i++;
                        //    break;
                        case('*'):
                            i+=2;
                            whitespace = 0;
                            while(!(__content[i] == '*' && __content[i+1] == '/') && __content[i]!=0x00)
                            {
                                if(__content[i] == '\n')
                                    content__[ii++] = '\n';
                                i++;
                            }
                            if(__content[i] == '\n')
                                content__[ii++] = '\n';
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
                //case('\n'):
                //    Line++;
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

    if(options::ddebug)
    {
        std::cout << "code after comments: " << std::endl;
        std::cout << content << std::endl;
    }

    if(options::ddebug)
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
        uint64_t oldLine = 1;
        loopStart:;
        oldLine = Line;
        i=0;
        terminatorSEMI = ';';
        terminatorLINE = ';';//this is on purpose
        terminatorCOLO = ':';
        terminatorSCOP = '{';
        terminatorSCCL = '}';
        if(startsWith(content,"//"))
            terminatorLINE = '\n';
        if(startsWith(content,"@"))
        {
            terminatorLINE = '\n';
            terminatorSEMI = '\n';
            terminatorCOLO = '\n';
            terminatorSCOP = '\n';
            terminatorSCCL = '\n';
        }
        if(startsWith(content,"#"))
            terminatorLINE = '\n';
        if(startsWith(content,"template"))
            terminatorLINE = '\n';
        if (content.find("class") != std::string::npos) 
        {
            terminatorCOLO = ';';
            terminatorLINE = '\n';
        }
        if (content.find("namespace") != std::string::npos) 
        {
            terminatorCOLO = ';';
            terminatorLINE = '\n';
        }
        std::string lineText;
        while(content[i] == '\n')
        {
            Line++;
            i++;
        }
        while(
            content[i] != terminatorLINE
            && content[i] != terminatorSEMI
            && content[i] != terminatorCOLO
            && content[i] != terminatorSCOP
            && content[i] != terminatorSCCL
            && content[i] != 0x00)
        {
            switch(content[i])
            {
                case('\''):
                case('`'):
                case('"'):
                {
                    char limiter = content[i];
                    lineText.push_back(content[i++]);
                    while(true)
                    {
                        switch(content[i])
                        {
                            case('\\'):
                                lineText.push_back(content[i]);
                                lineText.push_back(content[++i]);
                                break;
                            case(0x00):
                                goto textEnd;
                            default:
                                if(content[i] == limiter)
                                {
                                    lineText.push_back(content[i]);
                                    goto endString;
                                }
                                else
                                {
                                    lineText.push_back(content[i]);
                                }
                        }
                        i++;
                    }
                    endString:;
                    textEnd:;
                    //std::cout << "string: " << t.text << std::endl;
                    break;
                }
                case('\n'):
                    lineText.push_back(content[i]);
                    Line++;
                    break;
                default:
                    lineText.push_back(content[i]);
                    break;
            }
            i++;
        }
        if(content[i] == 0x00)
        {
            loopExit = true;
        }
        else if(content[i] != '\n')
        {
            lineText.push_back(content[i]);
            i++;
        }
        line L;
        L.lineNum = oldLine;
        L.tline = oldLine;
        L.whitespace = 0;
        while(content[i] == '\n')
        {
            Line++;
            i++;
        }
        //strPopFront(content,lineText.length());
        if(i > 0 && content.length() >= i)
            strPopFront(content,i);
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
        for(uint64_t j = 0; j < lineText.length(); j=0)
        {
            if(lineText[j] == ' ')
            {
                lineText = lineText.substr(1,lineText.length());
                leadingSpace++;
                j++;
            }
            else if(lineText[j] == '\n')
            {
                break;
            }
            else
                break;
        }
        //std::cout << "leading space: " << leadingSpace << std::endl;
        bool wasComment = false;
        if(startsWith(lineText,"//"))
        {
            lineText = "#outcom " + lineText.substr(2,lineText.length()-3);
            wasComment = true;
        }
        L.text = lineText;
        L.file = fname;
        L.leadingSpaces = leadingSpace;
        L.whitespace = leadingSpace;
        if(wasComment)
        {
            if(options::keepComments)
                lines.push_back(L);
        }
        else
        {
            if(L.text == "")
            {
                L.text = "#EOL";
                L.leadingSpaces = 0;
            }
            lines.push_back(L);
        }
        if(!loopExit)
            goto loopStart;
    }

    line endl;
    endl.leadingSpaces = 0;
    endl.text = "#EOF";
    lines.push_back(endl);

    if(options::ddebug)
        for(line& i : lines)
            std::cout << "line: " << i.text << std::endl;

    return lines;
}
