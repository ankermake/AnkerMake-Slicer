#ifndef FDMPREFERENCEDIALOGSERVICE_H
#define FDMPREFERENCEDIALOGSERVICE_H
#include <QObject>
#include "profilemanage/ifdmprofile.h"
#include "common/ak_const.h"
#include "../profilemanage/operatelog.h"



class FdmPreferenceDialogService : public QObject
{
      Q_OBJECT

public:
    void init();
    static FdmPreferenceDialogService* instance()
    {
        static FdmPreferenceDialogService manager;
        return &manager;
    }
    void setSource(AkConst::EOpenPreferenceSource source);

public slots:
    
    void onPreferenceWindowOpen(int idx);
    void onPreferencesPageSwitch(int idx);
    
    void onSaveBtnClicked();
    
    void onCloseBtnClicked();

    
    
    void onCurrentMaterialNameChanged(const QString & name);
    void onCurrentMachineNameChanged(const QString & name);
    void onCurrentParameterNameChanged(const QString & name);
    
    void onSourceTreeApiNodeValueChanged(const QString &categoryName, const FdmSettingItem & item);
    void onSourceTreeApiNodeValueChangedByUI(const QString& categoryName, const FdmSettingItem& item);

signals:
    void sendOperLog(OperateLogPtr operLogPtr);
    
    void operateComplete();

private:
    void syncCurrentParameterFromMangerToTree();

    
    //int activeWidgetIdx;

    
    IFdmProfile *activeProfile;
    
    AkConst::EOpenPreferenceSource openSource;
};

#endif // FDMPREFERENCEDIALOGSERVICE_H
