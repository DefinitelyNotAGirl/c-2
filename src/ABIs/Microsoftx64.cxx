/*
 * Created Date: Wednesday September 13th 2023
 * Author: Lilith
 * -----
 * Last Modified: Wednesday May 22nd 2024 11:30:22 am
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

 #include <compiler.h>
 #include <mangling.h>
 
 #define constructor __attribute__ ((constructor))
 
 namespace __ABI__{
 
 static ABI* abi = nullptr;
 
 static void genProlouge(section* code, scope* sc)
 {
 }
 
 static void genEpilouge(section* code, scope* sc)
 {
 }
 
 static void preCall(function* func)
 {
 }
 
 static void postCall(function* func)
 {
 }
 
 static void setArgStorages(function* func,std::vector<variable*>& args)
 {
 }
 
 static void moveArguments(function* func,std::vector<variable*>& args)
 {
 }
 
 static variable* call(function* func,std::vector<variable*>& args)
 {
	 return nullptr;
 }
 
 static void instrCall(function* func)
 {
 }
 
 constructor static void init()
 {
	 abi = new ABI;
	 abi->name = "Microsoftx64";
	 abi->moveArguments = &moveArguments;
	 abi->setArgStorages = &setArgStorages;
	 abi->genProlouge = &genProlouge;
	 abi->genEpilouge = &genEpilouge;
	 abi->call = &__ABI__::call;
	 abi->preCall = &preCall;
	 abi->postCall = &postCall;
	 abi->instrCall = &instrCall;
 
	 ABIs.push_back(abi);//add our ABI to the global ABI list
 }
 
 }//namespace __ABI__