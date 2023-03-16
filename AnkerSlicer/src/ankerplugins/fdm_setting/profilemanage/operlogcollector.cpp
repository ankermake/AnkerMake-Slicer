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
		auto manuallyValueChangeSet = FdmMachineProfileManager::Instance().getManuallyValueChangeSet();
		operLogPtr->AddRemoveSet(EProfileType::Machine, removeSet);
		operLogPtr->AddRenameMap(EProfileType::Machine, renameMap);
		operLogPtr->AddValueChangedSet(EProfileType::Machine, valueChangeSet);
		operLogPtr->AddManuallyValueChangedSet(EProfileType::Machine, manuallyValueChangeSet);
	}

	
	{
		auto removeSet = FdmMaterialProfileManager::Instance().getRemovedSet();
		auto renameMap = FdmMaterialProfileManager::Instance().getRenamedMap();
		auto valueChangeSet = FdmMaterialProfileManager::Instance().getValueChangeSet();
		auto manuallyValueChangeSet = FdmMaterialProfileManager::Instance().getManuallyValueChangeSet();
		operLogPtr->AddRemoveSet(EProfileType::Material, removeSet);
		operLogPtr->AddRenameMap(EProfileType::Material, renameMap);
		operLogPtr->AddValueChangedSet(EProfileType::Material, valueChangeSet);
		operLogPtr->AddManuallyValueChangedSet(EProfileType::Material, manuallyValueChangeSet);
	}

	
	{
		auto removeSet = FdmParameterProfileManager::Instance().getRemovedSet();
		auto renameMap = FdmParameterProfileManager::Instance().getRenamedMap();
		auto valueChangeSet = FdmParameterProfileManager::Instance().getValueChangeSet();
		auto manuallyValueChangeSet = FdmParameterProfileManager::Instance().getManuallyValueChangeSet();
		operLogPtr->AddRemoveSet(EProfileType::Parameter, removeSet);
		operLogPtr->AddRenameMap(EProfileType::Parameter, renameMap);
		operLogPtr->AddValueChangedSet(EProfileType::Parameter, valueChangeSet);
		operLogPtr->AddManuallyValueChangedSet(EProfileType::Parameter, manuallyValueChangeSet);
	}

	return operLogPtr;
}
