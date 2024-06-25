#include <compiler.h>
#include <mangling.h>

#define constructor __attribute__ ((constructor))

static ABI* extension_abi = nullptr;

static void genProlouge(section* code, scope* sc)
{
}

static void genEpilouge(section* code, scope* sc)
{
}

static void preArgTransfer(function* func){}

static void postReturn(function* func){}

static const std::vector<amd64::Register> integerRegisters = {
	amd64::Register::r8,
	amd64::Register::r9,
	amd64::Register::r10,
	amd64::Register::r11,
	amd64::Register::r12,
	amd64::Register::r13,
	amd64::Register::r14,
};

static const std::vector<amd64::Register> floatRegisters = {
	amd64::Register::xmm1,
	amd64::Register::xmm2,
	amd64::Register::xmm3,
	amd64::Register::xmm4,
	amd64::Register::xmm5,
	amd64::Register::xmm6,
	amd64::Register::xmm7
};

static void setFunctionStorages(function* func)
{
	uint64_t ireg = 0;
	uint64_t freg = 0;
	//+
	//+ this
	//+
	{
		if(func->isMember)
			ireg++;
	}
	//+
	//+ return
	//+
	{
		func->returnValue->storageArch = Architecture::AMD64;
		func->returnValue->storage = new amd64::VariableStorage;
		amd64::VariableStorage* storage = func->returnValue->storage;
		if(func->returnValue->dataType->regMode == 1)
		{
			// return in rax
			storage->mode = amd64::StorageMode::DirectRegister;
			storage->reg = amd64::Register::rax;
		}
		else if(func->returnValue->dataType->regMode == 2)
		{
			// return in xmm0
			storage->mode = amd64::StorageMode::DirectRegister;
			storage->reg = amd64::Register::xmm0;
		}
		else
		{
			// pass reference in rdi or rsi, return nothing
			storage->mode = amd64::StorageMode::DirectRegister;
			storage->reg = integerRegisters[ireg];
			ireg++;
			func->returnType = getType(func->returnType->name+"&");
			func->returnValue->dataType = func->returnType;
		}
	}
	//+
	//+ parameters
	//+
	{
		for(variable* arg : func->vparams)
		{
			arg->storageArch = Architecture::AMD64;
			arg->storage = new amd64::VariableStorage;
			amd64::VariableStorage* storage = arg->storage;
			if(arg->dataType->regMode == 1 && (ireg < integerRegisters.size()))
			{
				// pass via register
				storage->mode = amd64::StorageMode::DirectRegister;
				storage->reg = integerRegisters[ireg];
				ireg++;
			}
			else if(arg->dataType->regMode == 2 && (freg < floatRegisters.size()))
			{
				// pass via register
				storage->mode = amd64::StorageMode::DirectRegister;
				storage->reg = floatRegisters[freg];
				freg++;
			}
			else
			{
				// pass via stack
				uint64_t offset = func->stack.push(arg->dataType->size);
				storage->mode = amd64::StorageMode::IndirectRegister;
				storage->displacement = offset;
				storage->reg = amd64::Register::rbp;
			}
		}	
	}
}

constructor static void init()
{
    extension_abi = new ABI;
    extension_abi->name = "cpe2x64";
    extension_abi->setFunctionStorages = &setFunctionStorages;
    extension_abi->genProlouge = &genProlouge;
    extension_abi->genEpilouge = &genEpilouge;
    extension_abi->preArgTransfer = &preArgTransfer;
    extension_abi->postReturn = &postReturn;

    ABIs.push_back(extension_abi);//add our ABI to the global ABI list
}
