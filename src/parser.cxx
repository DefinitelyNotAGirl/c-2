/*
 * Created Date: Tuesday July 25th 2023
 * Author: Lilith
 * -----
 * Last Modified: Tuesday July 25th 2023 7:30:57 pm
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
#include <options.h>
#include <common.h>
#include <compiler.h>
#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <rstack.h>
#include <util.h>

std::vector<line> getLines(std::string fname);
void parse(std::vector<line> lines)
{
    uint64_t i = 0;
    while(true)
    {
        line& L = lines[i];
        revstack<token> TTS;
        if(L.text.empty())
            break;
        std::cout << "line: " << L.text << std::endl;
        token t = L.nextToken();
        while(t.type)
        {
            std::cout << "token: " << t.text << std::endl;

            switch(t.type)
            {
                case(6)://default width string literal
                    if(TTS[0].text == "#include")
                    {
                        std::cout << "including file: " << t.text << std::endl;
                        parse(getLines(t.text));
                    }
                    break;
                case(9):
                    if(TTS[0].text == "class")
                    {
                        std::cout << "ERROR: class \"" << t.text << "\" already exists!" << std::endl;
                    }
                    break;
                case(1):
                    if(TTS.size() > 0)
                    {
                        if(TTS[0].text == "class")
                        {
                            std::cout << "new class: " << t.text << std::endl;
                        }
                    }
                    break;
            }

            TTS.push(t);

            //epilogue
            t = L.nextToken();
        }

        if(i++ >= lines.size())
            break;
    }
}