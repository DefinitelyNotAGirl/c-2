import util.fs
import util.dependencies
import compile.debug
import os
import options

class target:
	type: str
	path: str
	src: str
	def __init__(self, _type: str, _path: str, _src: str):
		self.type = _type
		self.path = _path
		self.src = _src

def getObjectPath(src: str):
	return src.replace("./src","./build").replace(".cxx",".o")

def buildTarget(trg: target, tcurrent: int, tmax: int):
	rebuild: bool = False
	depfile = trg.path+".deps.json"
	if util.fs.exists(depfile):
		dependencies = util.dependencies.load(depfile)
		lastBuild = dependencies["last build"]
		for file in dependencies["files"]:
			if util.fs.getModifiedTimestamp(file) > lastBuild:
				rebuild = True
				break
	else:
		rebuild = True
	if not rebuild:
		print(" \033[33m\U000025CE\033[0m",trg.path,"(",tcurrent,"/",tmax,")")
		return 0
	if trg.type == "c++ object":
		if compile.debug.cpp(trg.src,trg.path,tcurrent,tmax) != 0:
			return 2
	return 0

def getTargetList() -> list[target]:
	targets: list[target] = list()
	for i in util.fs.getAllFilesOfExtension("./src","cxx"):
		targets.append(target("c++ object",getObjectPath(i),i))
	for i in util.fs.getAllFilesOfExtension("./src","cpp"):
		targets.append(target("c++ object",getObjectPath(i),i))
	for i in util.fs.getAllFilesOfExtension("./src","c"):
		targets.append(target("c object",getObjectPath(i),i))
	return targets

def getCleanList() -> list[str]:
	cleanFiles: list[str] = list()
	for target in getTargetList():
		cleanFiles.append(target.path)
		cleanFiles.append(target.path+".deps.json")
		cleanFiles.append(target.path+".d")
	return cleanFiles

def archive(objects: list[str],out: str) -> int:
	ExitCode: int = 0
	cmd = options.linker
	for object in objects:
		cmd += " "+object
	cmd += " lib/libcargparse.a"
	cmd += " "+options.args_linker
	cmd += " -shared -o "+out+" &> ./build/.stdout"
	#+
	#+ link
	#+
	ExitCode = os.system(cmd)
	if ExitCode != 0:
		print(" \033[31m\u25CB\033[0m",out)
		print(util.fs.readAll("./build/.stdout"),end='')
		return ExitCode
	print(" \033[32m\u25CF\033[0m",out)
	print(util.fs.readAll("./build/.stdout"),end='')
	return ExitCode

def run() -> int:
	print(" \033[32m\U0001F6E0\033[0m running debug build...")
	targets: list[target] = getTargetList()
	objects: list[str] = list()
	i: int = 1
	for trg in targets:
		objects.append(trg.path)
		if buildTarget(trg,i,targets.__len__()) != 0:
			return 2
		i+=1
	if compile.debug.link(objects,"./cp2-noext") != 0:
		return 2
	if archive(objects,"./cp2-archive") != 0:
		return 2
	print(" \033[32m\U0001F6E0\033[0m debug build complete.")
	return 0
