#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QWidget>
#include <QPushButton>
#include <QToolButton>
#include <QAction>
#include <QHBoxLayout>
#include <QFrame>
#include <QMenu>
#include <QStyleOption>
#include <QPainter>
#include <QToolTip>
#include <QLabel>
#include <QEvent>
#include "common/controlInterface/controlInterface.h"
#include "common/controlInterface/bubbleWidget.h"
#include "common/controlInterface/line.h"

namespace  control{
class ToolBar : public BubbleWidget
{
    Q_OBJECT
public:
    explicit ToolBar(QWidget *parent = nullptr);

    void addActionToToolBar(QAction *action);
    void addAction(QAction *action,Qt::Alignment aligment = Qt::AlignLeft);
    void addSeparatorToToolBar();
    //Create menu according to id, Returns itself if it exists
    QMenu *addMenu(ControlId  id);

    bool addActionToMenu(QAction *action,ControlId  id);
    bool addMenuToMenu(QMenu *menu,ControlId  id);
    void addSeparatorToMenu(ControlId  id);

    void setAlignment(Qt::Alignment alignment);
    QMenu *mainMenu() {return m_mainmenu;};

    void addStruct();

    void addMenuToToolBar(QMenu *menu, Qt::Alignment aligment = Qt::AlignLeft);

private:
    void init();
    void createMenuButton();
    void createButton();
    void changeEvent(QEvent *e);
private slots:
    void deleteButton();

private:
    QHBoxLayout *m_mainLayout;
    QToolButton *m_toolMenu;
    QMenu *m_mainmenu;
    QMap<QString, QMenu*> m_menuMap;

    QMap<QString, QAction*> m_actionMap;
    QMap <QAction *,QToolButton *> m_toolMap;
};

}
#endif // TOOLBAR_H
