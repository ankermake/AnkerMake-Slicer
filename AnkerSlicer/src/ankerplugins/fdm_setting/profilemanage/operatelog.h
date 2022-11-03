#pragma once
#include <QMap>
#include <QSet>
#include <QVariant>
#include "ifdmprofile.h"
#include "common/GeoAndShow/CHBase.h"

DEF_PTR(AkOperate)
class AkOperate
{
public:
	
	QString profileName;
	virtual QString toString() { return ""; }
};
DEF_PTR(AkOperateAdd)
class AkOperateAdd:public AkOperate
{
public:
	QString toString() override { return QString("create %1").arg(profileName); }
};
DEF_PTR(AkOperateRemove)
class AkOperateRemove :public AkOperate
{
public:
	QString toString() override { return QString("remove %1").arg(profileName); }
};
DEF_PTR(AkOperateValueChange)
class AkOperateValueChange :public AkOperate
{
public:
	QString toString() override { return QString("modify %1").arg(profileName); }
};
DEF_PTR(AkOperateRename)
class AkOperateRename :public AkOperate
{
public:
	
	QString currentName;
	QString toString() override { return QString("rename %1->%2").arg(profileName).arg(currentName); }
};



DEF_PTR(AkOperateValueChangeItem)
class AkOperateValueChangeItem
{
public:
	QString  categoryName;
	QString  key;
	QVariant value;
};
DEF_PTR(AkOperateVcAddItem)
class AkOperateVcAddItem: public AkOperateValueChangeItem
{
};
DEF_PTR(AkOperateVcRemoveItem)
class AkOperateVcRemoveItem : public AkOperateValueChangeItem
{
};
DEF_PTR(AkOperateVcModifyItem)
class AkOperateVcModifyItem : public AkOperateValueChangeItem
{
public:
	QVariant currentValue;
};
DEF_PTR(AkOperateValueChangeDetail)
class AkOperateValueChangeDetail
{
	QList<AkOperateValueChangeItemPtr> currentValue;
};

DEF_PTR(OperateLog)
class OperateLog
{
public:
	enum EFileOperType
	{
		Add,
		Remove,
		Rename,
		ValueChange
	};
	//enum EContentOperType
	//{
	//	Add,
	//	Remove,
	//	Rename,
	//	ValueChange
	//};

	
	
	void AddRenameMap(EProfileType profileType, QMap<QString, QString> renameMap);
	
	void AddRemoveSet(EProfileType profileType, QSet<QString> removedSet);
	
	void AddValueChangedSet(EProfileType profileType, QSet<QString> valueChangeSet);

	
	bool constainsOper(EProfileType profileType, QString profileName);
	bool constainsRemoveOper(EProfileType profileType, QString profileName);
	bool constainsRenameOper(EProfileType profileType, QString profileName);
	bool constainsValueChangeOper(EProfileType profileType, QString profileName);

	
	
	QString getCurrentName(EProfileType profileType, QString profileName);
	
	AkOperateValueChangeDetailPtr getValueChangeDetail(EProfileType profileType, QString profileName);

	
	QString toString();

private:
	
	
	//QMap<EProfileType, QMap<QString, QMap<EFileOperType, QList<AkOperatePtr>>>> operMap;
	
	QMap<EProfileType, QMap<QString, QMap<EFileOperType, AkOperatePtr>>> operMap;

	
	QMap<EProfileType, QMap<QString, AkOperateValueChangeDetailPtr>> contentModifyDetailMap;
};

