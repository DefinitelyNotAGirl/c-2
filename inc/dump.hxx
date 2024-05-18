/*
 * Created Date: Saturday May 11th 2024
 * Author: Lilith
 * -----
 * Last Modified: Saturday May 11th 2024 5:03:26 am
 * Modified By: Lilith (definitelynotagirl115169@gmail.com)
 * -----
 * Copyright (c) 2023-2024 DefinitelyNotAGirl@github
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

#include <compiler.h>

#define sdump(var) dump(std::string(#var),var,"");

void dump(std::string name, __register__* obj, std::string indent);
void dump(std::string name, std::string* obj, std::string indent);
void dump(std::string name, uint64_t* obj, std::string indent);
void dump(std::string name, scope* obj, std::string indent);
void dump(std::string name, variable* obj, std::string indent, bool shallow = false);
void dump(std::string name, function* obj, std::string indent);
void dump(std::string name, type* obj, std::string indent);
void dump(std::string name, castFunction* obj, std::string indent);
void dump(std::string name, arch* obj, std::string indent);
void dump(std::string name, ABI* obj, std::string indent);
void dump(std::string name, functionStorage* obj, std::string indent);
void dump(std::string name, line* obj, std::string indent);
void dump(std::string name, token* obj, std::string indent);
template<typename T>
void listdump(std::string name, std::vector<T>* obj, std::string indent);
template<typename T>
void vlistdump(std::string name, std::vector<T>* obj, std::string indent);
