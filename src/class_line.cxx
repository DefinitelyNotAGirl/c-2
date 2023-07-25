/*
 * Created Date: Tuesday July 25th 2023
 * Author: Lilith
 * -----
 * Last Modified: Tuesday July 25th 2023 12:57:38 am
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

#include <class_line.h>
#include <class_token.h>
#include <common.h>
#include <compiler.h>

//token types:
// 0 - none
// 1 - identifier
// 2 - immediate
// 3 - operator
// 4 - label
// 5 - directive
// 6 - string literal (standard)
// 7 - string literal (widechar)
// 8 - keyword
// 9 - type name
// 10 - variable name

token line::nextToken()
{
    token t;
    std::string s;
    
    for(tpos = tpos;tpos < this->text.size();tpos++)
    {
        switch(this->text[tpos])
        {
            case('"'):
                s += this->text[tpos++];
                while(1)
                {
                    switch(this->text[tpos])
                    {
                        case('"'):
                            s += this->text[tpos++];
                            goto endLoop1;
                        case('\\'):
                            tpos++;
                            switch(this->text[tpos])
                            {
                                case('"'):
                                    tpos++;
                                    s += '"';
                                    break;
                            }
                            break;
                        case(0x00):
                            tpos++;
                            goto endLoop1;
                        default:
                            s += this->text[tpos];
                            tpos++;
                    }
                }
                endLoop2:;
                break;
            case(';'):
            case(' '):
                goto endLoop1;
            default:
                s += this->text[tpos];
        }
    }
    endLoop1:;
    tpos++;

    if(s.empty())
        t.type = 0;
    else if(s == "#include")
        t.type = 5;
    else if(s.back() == '"')
    {
        if(s.front() == '"')
        {
            s = s.substr(1, s.size() - 2);
            t.type = 6;
        }
        else if(s.front() == 'W' || s.front() == 'L')
        {
            if(s[1] == '"')
            {
                t.type = 7;
                s = s.substr(2, s.size() - 3);
            }
        }
    }
    else if(s == "class")
    {
        t.type = 8;
    }
    else
    {
        //some identifier
        for(type* i : types)
        {
            if(i->name == s)
            {
                t.type = 9;//typename
                goto identifierFound;
            }
        }

        //new identifier
        t.type = 1;//identifier

        identifierFound:;
    }
        

    t.Line = this;
    t.text = s;

    return t;
}
