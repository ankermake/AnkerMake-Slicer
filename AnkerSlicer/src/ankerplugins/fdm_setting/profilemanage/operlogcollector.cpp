#include "operlogcollector.h"
#include "fdmmachineprofilemanager.h"
#include "fdmmaterialprofilemanager.h"
#include "fdmparameterprofilemanager.h"


OperateLogPtr OperLogCollector::getLog()
{
	OperateLogPtr operLogPtr = OperateLogPtr(new OperateLog);
	
	{
		auto removeSet = FdmMachineProfileManager::Instance().getRemovedSet();
		auto renameMap = FdmMachineProfileManager::Instance().getRenamedMap();
		auto valueChangeSet = FdmMachineProfileManager::Instance().getValueChangeSet();
		operLogPtr->AddRemoveSet(EProfileType::Machine, removeSet);
		operLogPtr->AddRenameMap(EProfileType::Machine, renameMap);
		operLogPtr->AddValueChangedSet(EProfileType::Machine, valueChangeSet);
	}

	
	{
		auto removeSet = FdmMaterialProfileManager::Instance().getRemovedSet();
		auto renameMap = FdmMaterialProfileManager::Instance().getRenamedMap();
		auto valueChangeSet = FdmMaterialProfileManager::Instance().getValueChangeSet();
		operLogPtr->AddRemoveSet(EProfileType::Material, removeSet);
		operLogPtr->AddRenameMap(EProfileType::Material, renameMap);
		operLogPtr->AddValueChangedSet(EProfileType::Material, valueChangeSet);
	}

	
	{
		auto removeSet = FdmParameterProfileManager::Instance().getRemovedSet();
		auto renameMap = FdmParameterProfileManager::Instance().getRenamedMap();
		auto valueChangeSet = FdmParameterProfileManager::Instance().getValueChangeSet();
		operLogPtr->AddRemoveSet(EProfileType::Parameter, removeSet);
		operLogPtr->AddRenameMap(EProfileType::Parameter, renameMap);
		operLogPtr->AddValueChangedSet(EProfileType::Parameter, valueChangeSet);
	}

	return operLogPtr;
}
