/*
 * Created Date: Wednesday July 19th 2023
 * Author: Lilith
 * -----
 * Last Modified: Wednesday July 19th 2023 6:13:25 pm
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

#include <errors.h>

namespace error
{
    void variableNameLatinChar(uint64_t tPOS, std::vector<token_t*>& tokens)
    {
        std::cerr << "\033[0m";
        std::cerr << "\033[31mERROR:\033[0m Variable names must start with a latin letter or underscore!" << std::endl;
        std::cerr << "In file: " << tokens[tPOS]->file << " at line: " << tokens[tPOS]->line << ": " << tokens[tPOS-1]->text << " \033[31m" << tokens[tPOS]->text <<"\033[0m"<< std::endl;
        uint64_t arrowPOS = strlen(tokens[tPOS]->file)+10+11+strlen(std::to_string(tokens[tPOS]->line).c_str())+strlen(tokens[tPOS-1]->text)+1;
        for(uint64_t i = 0; i < arrowPOS; i++)
            std::cout << ' ';
        std::cout << "\033[31m^\033[0m" << std::endl;
    }
}
