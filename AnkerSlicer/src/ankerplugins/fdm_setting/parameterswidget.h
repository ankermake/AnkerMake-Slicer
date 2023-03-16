#ifndef PARAMETERSWIDGET_H
#define PARAMETERSWIDGET_H
#include "common/controlInterface/controlInterface.h"
#include <QWidget>
#include <QQmlEngine>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QUrl>
#include <QQuickView>
#include <QVBoxLayout>
#include <QApplication>
#include <QFileDialog>
#include "common/ak_const.h"
#include "common/controlInterface/pagewidget.h"
#include "common/controlInterface/messageDialog.h"
#include "preferencesparamters.h"
#include "filemanage/fdmqmlengine.h"
#include "service/fdmparameterprofileservice.h"
#include "paramlistmodel.h"

namespace control {
class ParametersWidget : public PageWidget
{
    Q_OBJECT
public:
    explicit ParametersWidget(PageWidget *parent = nullptr);
    ~ParametersWidget();
    void setControlmanager(ControlInterface* controlmanager);
private:
    void getDefaultNewName(const QStringList &list, const QString &name);
protected:
    void changeEvent(QEvent *e);
signals:
    void parameterRename(QString oldName, QString newName);
    void importParameter(QString filepath);
    void exportParameter(QString filePath);

private slots:
    void renameCustomParameter(const QString &oldName);
    void textValid(int flag);

    void importButtonClicked();
    void exportButtonClicked(QString name);


private:
    FdmParameterProfileService *m_service;
    QString m_oldName;
    MessageDialog *m_messageDialog;
    QQuickView *m_quickView = nullptr;
    QString m_newName;
    ControlInterface* m_controlmanager = nullptr;
};
}
#endif // PARAMETERSWIDGET_H
