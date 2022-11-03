#ifndef PARAMETERSWIDGET_H
#define PARAMETERSWIDGET_H

#include <QWidget>
#include <QQmlEngine>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QUrl>
#include <QQuickView>
#include <QVBoxLayout>
#include <QApplication>
#include <QFileDialog>
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

    QString m_newName;
};
}
#endif // PARAMETERSWIDGET_H
