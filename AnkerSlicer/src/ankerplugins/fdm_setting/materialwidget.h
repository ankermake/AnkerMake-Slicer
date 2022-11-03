#ifndef MATERIALWIDGET_H
#define MATERIALWIDGET_H

#include <QWidget>
#include <QQmlEngine>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QUrl>
#include <QQuickView>
#include <QVBoxLayout>
#include "common/controlInterface/pagewidget.h"
#include "preferencesparamters.h"
#include "filemanage/fdmqmlsourcetree.h"
#include "filemanage/fdmqmlengine.h"
#include "common/controlInterface/messageDialog.h"
#include "service/fdmmaterialprofileservice.h"
namespace control {
class MaterialWidget : public PageWidget
{
    Q_OBJECT
public:
    explicit MaterialWidget(PageWidget *parent = nullptr);
    ~MaterialWidget();

private:
    QVariantMap findParameters(FdmParamNode *root, QString keyNodeName, QStringList list);
    void getDefaultNewName(const QStringList &list, const QString &name);
protected:
    void changeEvent(QEvent *e);
signals:
    void materialRename( QString oldName, QString newName);

private slots:
    void renameCustomMaterial(const QString &oldName);
    void textValid(int flag);

private:
    FdmMaterialProfileService *m_service;
    QString m_oldName;
    MessageDialog *m_messageDialog;

    QString m_newName;
};

}
#endif // MATERIALWIDGET_H
