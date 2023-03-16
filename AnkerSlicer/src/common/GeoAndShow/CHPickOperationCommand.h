/*



*/



#ifndef CHPickOperationCommand_H
#define CHPickOperationCommand_H


#include "qevent.h"
#include "CHMeshShowObj.h"
#include "CH3DPrintModel.h"
#include <QMenu>

#if defined(QT_SHARED)
#ifdef COMMONLIB
#define COMMONLIB_EXPORT Q_DECL_EXPORT
#else
#define COMMONLIB_EXPORT Q_DECL_IMPORT
#endif
#else
#define COMMONLIB_EXPORT
#endif

class SceneRightMenu : public QMenu{
    Q_OBJECT
public:
    using ThisType = SceneRightMenu;
    explicit SceneRightMenu(QObject * scenePick);
    using QMenu::popup;

    inline void setEnabledDelete(bool enable) {if(actDelete) actDelete->setEnabled(enable);}
    inline void setEnabledCopy  (bool enable) {if(actCopy  ) actCopy  ->setEnabled(enable);}
    inline void setEnabledPaste (bool enable) {if(actPaste ) actPaste ->setEnabled(enable);}
    inline void setEnabledHide  (bool enable) {if(actHide  ) actHide  ->setEnabled(enable);}
    inline void setEnabledShow  (bool enable) {if(actShow  ) actShow  ->setEnabled(enable);}
    inline void setEnabledReset (bool enable) {if(actReset ) actReset ->setEnabled(enable);}

protected:
    void changeEvent(QEvent *e);
protected:
    QAction* actDelete  {nullptr};
    QAction* actCopy    {nullptr};
    QAction* actPaste   {nullptr};
    QAction* actHide    {nullptr};
    QAction* actShow    {nullptr};
    QAction* actReset   {nullptr};
};


DEF_PTR(CHPickOperationCommand)
class COMMONLIB_EXPORT CHPickOperationCommand : public QObject
{
    Q_OBJECT

        //class MeshModel;

public:
    CHPickOperationCommand();
    virtual ~CHPickOperationCommand();

public:

    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void keyPressEvent(QKeyEvent* event);

    
    CHAABB3D getPickedObjsAABB();
    void currentSelectedPrints(std::vector<CH3DPrintModelPtr> &);

    
    bool havePickedObjs();
    bool havePickedPrintObjs();
    bool haveEditPluginsActive();
    bool haveCanPickedObj();

public Q_SLOTS:
    void doWithDocMeshModelsChanged();

    
    void doWithFileListPickedChanged();

    void selectAllobjs();

    void copySelectedObjs();
    void deleteSelectedObjs();
    void copySelectedAndPasteObjs();


Q_SIGNALS:
    void selectedObjsChanged(QVariant);
    void resetSeletedObjsSig();

    public
Q_SLOTS:

    void pasteObjs();
    void hideSelectedObjs();
    void showSelectedObjs();
    void resetSelectedObjs();


public:
    std::set<CHMeshShowObjPtr> m_selectedObjs;
    bool rightBtnPressed = false;
    bool mouseRightMoved = false;

private:
    std::vector<CHMeshShowObjPtr> m_copiedObjs;

    CHMeshShowObjPtr m_tmesh;

    bool m_keyDown;

    SceneRightMenu * rightMenu{nullptr};
};


#endif
