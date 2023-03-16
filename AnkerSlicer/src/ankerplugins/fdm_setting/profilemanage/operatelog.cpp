#include "operatelog.h"

void OperateLog::AddRenameMap(EProfileType profileType, QMap<QString, QString> renameMap)
{
	if (renameMap.size() == 0)
	{
		return;
	}
	if (!operMap.contains(profileType))
	{
		operMap[profileType] = QMap<QString, QMap<EFileOperType, AkOperatePtr>>();
	}
	QMap<QString, QString>::iterator iter = renameMap.begin();
	while (iter != renameMap.end())
	{
		QString originName = iter.key();
		if (!operMap[profileType].contains(originName))
		{
			operMap[profileType][originName] = QMap<EFileOperType, AkOperatePtr>();
		}
		AkOperateRenamePtr renamePtr = AkOperateRenamePtr(new AkOperateRename);
		renamePtr->profileName = originName;
		renamePtr->currentName = iter.value();
		operMap[profileType][originName][EFileOperType::Rename] = renamePtr;
		iter++;
	}
}

void OperateLog::AddRemoveSet(EProfileType profileType, QSet<QString> removedSet)
{
	if (removedSet.size() == 0)
	{
		return;
	}
	if (!operMap.contains(profileType))
	{
		operMap[profileType] = QMap<QString, QMap<EFileOperType, AkOperatePtr>>();
	}
	QSet<QString>::iterator iter = removedSet.begin();
	foreach(const QString& originName, removedSet)
	{
		if (!operMap[profileType].contains(originName))
		{
			operMap[profileType][originName] = QMap<EFileOperType, AkOperatePtr>();
		}
		operMap[profileType][originName][EFileOperType::Remove] = AkOperateRemovePtr(new AkOperateRemove);
	}
}

void OperateLog::AddValueChangedSet(EProfileType profileType, QSet<QString> valueChangeSet)
{
	if (valueChangeSet.size() == 0)
	{
		return;
	}
	if (!operMap.contains(profileType))
	{
		operMap[profileType] = QMap<QString, QMap<EFileOperType, AkOperatePtr>>();
	}
	QSet<QString>::iterator iter = valueChangeSet.begin();
	foreach(const QString & originName, valueChangeSet)
	{
		if (!operMap[profileType].contains(originName))
		{
			operMap[profileType][originName] = QMap<EFileOperType, AkOperatePtr>();
		}
		operMap[profileType][originName][EFileOperType::ValueChange] = AkOperateValueChangePtr(new AkOperateValueChange);
	}
}


void OperateLog::AddManuallyValueChangedSet(EProfileType profileType, QSet<QString> valueChangeSet)
{
	if (valueChangeSet.size() == 0)
	{
		return;
	}
	if (!operMap.contains(profileType))
	{
		operMap[profileType] = QMap<QString, QMap<EFileOperType, AkOperatePtr>>();
	}
	QSet<QString>::iterator iter = valueChangeSet.begin();
	foreach(const QString & originName, valueChangeSet)
	{
		if (!operMap[profileType].contains(originName))
		{
			operMap[profileType][originName] = QMap<EFileOperType, AkOperatePtr>();
		}
		operMap[profileType][originName][EFileOperType::ValueChangeManually] = AkOperateValueChangePtr(new AkOperateValueChange);
	}
}

bool OperateLog::constainsOper(EProfileType profileType, QString profileName)
{
	if (!operMap.contains(profileType))
	{
		return false;
	}
	if (!operMap[profileType].contains(profileName))
	{
		return false;
	}
	return true;
}

bool OperateLog::constainsRemoveOper(EProfileType profileType, QString profileName)
{
	if (!constainsOper(profileType, profileName))
	{
		return false;
	}
	if (!operMap[profileType][profileName].contains(EFileOperType::Remove))
	{
		return false;
	}
	return true;
}

bool OperateLog::constainsRenameOper(EProfileType profileType, QString profileName)
{
	if (!constainsOper(profileType, profileName))
	{
		return false;
	}
	if (!operMap[profileType][profileName].contains(EFileOperType::Rename))
	{
		return false;
	}
	return true;
}

bool OperateLog::constainsValueChangeOper(EProfileType profileType, QString profileName)
{
	if (!constainsOper(profileType, profileName))
	{
		return false;
	}
	if (!operMap[profileType][profileName].contains(EFileOperType::ValueChange))
	{
		return false;
	}
	return true;
}

bool OperateLog::constainsManuallyValueChangeOper(EProfileType profileType, QString profileName)
{
	if (!constainsOper(profileType, profileName))
	{
		return false;
	}
	if (!operMap[profileType][profileName].contains(EFileOperType::ValueChangeManually))
	{
		return false;
	}
	return true;
}

QString OperateLog::getCurrentName(EProfileType profileType, QString profileName)
{
	if (!constainsRenameOper(profileType, profileName))
	{
		return profileName;
	}
	auto operPtr = operMap[profileType][profileName][EFileOperType::Rename];
	auto renameOperPtr = std::dynamic_pointer_cast<AkOperateRename>(operPtr);
	return renameOperPtr->currentName;
}


AkOperateValueChangeDetailPtr OperateLog::getValueChangeDetail(EProfileType profileType, QString profileName)
{
	return AkOperateValueChangeDetailPtr();
}


QString OperateLog::toString()
{
	QStringList result;
	auto iter = operMap.begin();
	while (iter != operMap.end())
	{
		auto profileType = iter.key();
		result << QString("profileType %1").arg(profileType);
		auto profileMap = iter.value();
		auto profileMapIter = profileMap.begin();
		while (profileMapIter != profileMap.end())
		{
			auto profileName = profileMapIter.key();
			result << QString("    profileName %1").arg(profileName);
			auto profileOperMap = profileMapIter.value();
			auto profileOperMapIter = profileOperMap.begin();
			while (profileOperMapIter != profileOperMap.end())
			{
				auto operType = profileOperMapIter.key();
				result << QString("        operType %1 %2").arg(operType).arg(profileOperMapIter.value()->toString());
				profileOperMapIter++;
			}
			profileMapIter++;
		}
		iter++;
	}

	return result.join("\r\n");
}
