#ifndef GENERALWIDGET_H
#define GENERALWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include "common/controlInterface/pagewidget.h"
#include "settingmanager.h"
#include <QDebug>
#include <QTranslator>
#include <QApplication>
#include <QListView>
#include "translator.h"
#include "message/messageprocessing.h"
namespace  control{

enum LANGUAGE{
    English = 0,
    Chinese,

};

enum UPDATETYPE {
    Install = 0,
    Update,
};
class GeneralWidget : public PageWidget
{
    Q_OBJECT
public:
    explicit GeneralWidget(MessageProcessing * messageProcessing, PageWidget *parent = nullptr);
    void init();
    QString displayName() const {return m_displayName;};
    virtual void closeWidget();;
    virtual void save();;
protected:
    void changeEvent(QEvent *e);
signals:
   void networkLanguageChangedSignal(qint64);
public slots:
    void receiveMsgFromNetwork(PluginMessageData metadata);

private slots:
    void checkButtonClicked();
private:
     settings::SettingManager m_settings;
    // QTranslator *m_translator;
     QStringList m_languageList ;

     void update_app(const QString&);
     QPushButton *m_checkButton;
     QString m_filePath;
     QComboBox *m_languageCombox;
     QCheckBox *m_aiCheckBox;
     QCheckBox *m_updateCheckBox;
	 MessageProcessing *m_messageProcessing;
     QLabel *m_languageLabel = nullptr;
     QLabel *m_aiLabel = nullptr;
     bool m_updateFlag = false;
     QLabel *m_versionLabel = nullptr;
};



}
#endif // GENERALWIDGET_H
