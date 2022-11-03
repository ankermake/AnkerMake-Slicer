#pragma once
#include "fdmparameterprofile.h"


class FdmRealTimeProfile: public FdmParameterProfile
{
public:
	FdmRealTimeProfile(const QString& dateSource);
	FdmRealTimeProfile(const QString& dateSource, const QList<FdmProfileCategory>& categories);
	
	void reload(FdmRealTimeProfile* rtProfile);
	
	QString getProfileName();
	void setProfileName(QString name);

	void setRealTimeMetaDate(const QString& key, const QVariant& value);
	QVariant getRealTimeMetaDate(const QString& key);

    void updateProfile(FdmParameterProfile* profile);
	
	
	//void saveInnerProfile();
	//void saveAsInnerProfile(QString filePath);
	
	void getInnerProfileCategories(QList<FdmProfileCategory>& categories);
};

