import os

def execute(cmd: str):
	if os.system(cmd) != 0:
		exit (1)

def CreateDirectory(path: str):
	os.system("mkdir "+path)

def DeleteFile(path: str):
	os.system("rm "+path)

def CMDtoHTML(cmd: str,dst: str):
	execute(cmd+" &> "+dst)
	execute("ansiToHtml "+dst)
	DeleteFile(dst)

def dumpELF(file: str,dir: str):
	CreateDirectory(dir)
	CMDtoHTML("eu-elflint "+file,dir+"/eu-elflint")
	CMDtoHTML("eu-readelf -a "+file,dir+"/eu-readelf")
	CMDtoHTML("readelf -a "+file,dir+"/gnu-readelf")
	CMDtoHTML("objdump -M intel -M x86-64 -M amd64 --disassembler-color=extended --visualize-jumps=extended-color --wide --show-all-symbols -d "+file,dir+"/disassembly")
	return

execute("python ./build-system/main.py debug")
os.system("make -C ../cpe2Example C2ARGS=\"-Wno-unimplemented -Wno-deprecated\"")
dumpELF("../cpe2Example/build/main.o","debug/main.o")
dumpELF("../cpe2Example/test.exe","debug/test.exe")
