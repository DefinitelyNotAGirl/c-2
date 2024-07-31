import os
import platform

def execute(cmd: str):
	if os.system(cmd) != 0:
		exit (1)

def CreateDirectory(path: str):
	if not os.path.isdir(path):
		os.system("mkdir "+path)

def DeleteFile(path: str):
	os.system("rm "+path)

def CMDtoHTML(cmd: str,dst: str):
	execute(cmd+" &> "+dst)
	execute("ansiToHtml "+dst)
	DeleteFile(dst)

def dumpELF(file: str,dir: str):
	CreateDirectory(dir)
	if(platform.system() == "Linux"):
		CMDtoHTML("eu-elflint "+file,dir+"/eu-elflint")
		CMDtoHTML("eu-readelf -a "+file,dir+"/eu-readelf")
	if(platform.system() == "Linux"):
		CMDtoHTML("readelf -a "+file,dir+"/readelf")
	elif(platform.system() == "Darwin"):
		CMDtoHTML("/Volumes/programming/cross-compilers/amd64/bin/x86_64-elf-readelf -a "+file,dir+"/readelf")
	if(platform.system() == "Linux"):
		CMDtoHTML("objdump -M intel -M x86-64 -M amd64 --disassembler-color=extended --visualize-jumps=extended-color --wide --show-all-symbols -d "+file,dir+"/disassembly.objdump")
	elif(platform.system() == "Darwin"):
		CMDtoHTML("/Volumes/programming/cross-compilers/amd64/bin/x86_64-elf-objdump -M intel -M x86-64 -M amd64 --disassembler-color=extended --visualize-jumps=extended-color --wide --show-all-symbols -d "+file,dir+"/disassembly.objdump")
	execute("HyperTextDisas "+file+" "+dir+"/disassembly.html")
	return

execute("python ./build-system/main.py debug")
os.system(
    "make -C ../cpe2Example C2ARGS=\"-Wno-unimplemented -Wno-deprecated\""
    +" AS=/Volumes/programming/cross-compilers/amd64/bin/x86_64-elf-as"
    +" LD=/Volumes/programming/cross-compilers/amd64/bin/x86_64-elf-ld"
    +" c2=../c-2/cp2"
)
dumpELF("../cpe2Example/build/c2resources.o","debug/resources")
dumpELF("../cpe2Example/build/main.o","debug/main.o")
#dumpELF("../stdcpe2/libcpe2.a","debug/libcpe2")
dumpELF("../cpe2Example/test.exe","debug/test.exe")
