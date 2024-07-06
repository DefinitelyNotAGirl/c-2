import os

def execute(cmd: str):
	if os.system(cmd) != 0:
		exit (1)

execute("python ./build-system/main.py debug")
if os.system("make -C ../cpe2Example C2ARGS=\"-Wno-unimplemented -Wno-deprecated\"") != 0:
	os.system("eu-elflint ../cpe2Example/build/main.o &> debug/elflint.ans")
	os.system("ansiToHtml debug/elflint.ans")
	os.system("eu-readelf -a ../cpe2Example/build/main.o &> debug/readelf.ans")
	os.system("ansiToHtml debug/readelf.ans")
	exit (1)
execute("eu-elflint ../cpe2Example/build/main.o &> debug/elflint.ans")
execute("ansiToHtml debug/elflint.ans")
execute("eu-readelf -a ../cpe2Example/build/main.o &> debug/readelf.ans")
execute("ansiToHtml debug/readelf.ans")
execute("objdump -M intel -M x86-64 -M amd64 --disassembler-color=extended --visualize-jumps=extended-color --wide --show-all-symbols -d ../cpe2Example/build/main.o &> debug/main.o.disas")
execute("ansiToHtml debug/main.o.disas")
execute("objdump -M intel -M x86-64 -M amd64 --disassembler-color=extended --visualize-jumps=extended-color --wide --show-all-symbols -d ../cpe2Example/test.exe &> debug/test.exe.disas")
execute("ansiToHtml debug/test.exe.disas")
