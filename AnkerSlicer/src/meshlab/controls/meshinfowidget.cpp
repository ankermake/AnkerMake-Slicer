#include "meshinfowidget.h"
#include <QTimer>
namespace  control{

class LineItem;
MeshInfoWidget::MeshInfoWidget(QWidget *parent)
    : BubbleWidget(parent),
      m_listWidget(new QListWidget(this)),
      m_expandButton (new QToolButton(this))
{
    this->setBackgroundColor(QColor(41, 42, 45));
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(7);
    layout->setContentsMargins(0,0,0,12);
    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->setContentsMargins(16,16,16,0);
    m_title = new QLabel(tr("Object List"),this);
    m_title->setFixedHeight(18);
    m_title->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed));
    hbox->addWidget(m_title,0,Qt::AlignLeft);

    layout->addStretch();

    m_expandButton->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
    m_expandButton->setFixedSize(18,18);
    connect(m_expandButton,&QToolButton::clicked,this,&MeshInfoWidget::hideList);
    m_expandButton->setIcon(QIcon(":/images/icon/fdm_pack_icon_n.png"));
    m_expandButton->setIconSize(QSize(16,16));
    m_expandButton->setStyleSheet("border:none");
    m_expandButton->setObjectName("m_expandButton");
    hbox->addWidget(m_expandButton,0,Qt::AlignRight);

    layout->addLayout(hbox);
  //  m_listWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_listWidget->setAttribute(Qt::WA_TranslucentBackground);
    m_listWidget->installEventFilter(this);
    m_listWidget->setFocusPolicy(Qt::NoFocus);
 //   connect(m_listWidget,&QListWidget::clicked,this,&MeshInfoWidget::clickCurrentIndex);
    connect(m_listWidget,&QListWidget::itemSelectionChanged, this, &MeshInfoWidget::selectedChanged);
    //connect(m_listWidget,&QListWidget::currentItemChanged, this, &MeshInfoWidget::currentItemChanged);
    connect(m_listWidget,&QListWidget::currentRowChanged, this, &MeshInfoWidget::currentRowChanged);

    m_listWidget->setFrameShape(QListWidget::NoFrame);
    m_listWidget->verticalScrollBar()->setStyleSheet(AkConst::GlobalStyleSheet::ScrollBarVertical);
    m_listWidget->horizontalScrollBar()->setStyleSheet(AkConst::GlobalStyleSheet::ScrollBarHorizontal);
    m_listWidget->setContentsMargins(0,0,0,0);
    m_listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    layout->addWidget(m_listWidget);
    this->setMouseTracking(true);

    //connect(getGlobalPick().get(),SIGNAL(selectedObjsChanged(int)),this,SLOT(updateItem()));
    connect(getGlobalPick().get(), &CHPickOperationCommand::selectedObjsChanged, this, &MeshInfoWidget::updateItem);
    connect(getDoc().get(),&CHDoc::modelObjsStatusChanged,this,&MeshInfoWidget::updateItem);
    connect(this, SIGNAL(currentChanged()), getGlobalPick().get(), SLOT(doWithFileListPickedChanged()));
    m_listWidget->setFixedHeight(0);
    m_listWidget->setVisible(false);

    this->setFixedWidth(210);
    //this->setMinimumWidth(210);
    this->setMaximumHeight(200);
   // this->setMinimumHeight(27);

}

void MeshInfoWidget::addMeshModel()
{
    m_listWidget->clear();
    init();
}

void MeshInfoWidget::addItem(QString name,bool visble,bool selected,int index)
{
    if(name.isNull()) {
        return ;
    }

    QWidget *widget = new QWidget(m_listWidget);

    widget->setStyleSheet("background:transparent");
    QHBoxLayout *hbox = new QHBoxLayout(widget);
    hbox->setSpacing(4);
    hbox->setContentsMargins(16,0,12,0);

    QLabel *meshId = new QLabel(widget);
    meshId->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed));
    meshId->setMaximumWidth(90);
    QFont font = this->font();
    if(font.pointSize() >=2 ) {
        font.setPointSize(font.pointSize() - 2);
        meshId->setFont(font);
    }
    QFontMetrics metrics(meshId->font());
    meshId->setText(metrics.elidedText(name,Qt::ElideMiddle,meshId->width()));
    hbox->addWidget(meshId);
    QToolButton *showButton = new QToolButton(widget);
    showButton->setProperty("index",index);
    connect(showButton,&QToolButton::clicked,this,&MeshInfoWidget::showMesh);
    showButton->setIcon(QIcon(visble ? ":/images/icon/fdm_eye_icon_n.png" : ":/images/icon/fdm_hidden_icon_n.png"));
    showButton->setIconSize(QSize(20,20));
    hbox->addWidget(showButton,1,Qt::AlignRight);

    QToolButton *deleteButton = new QToolButton(widget);
    //deleteButton->setFlat(true);
    //deleteButton->setStyleSheet("background:transparent");
    deleteButton->setProperty("index",index);
    connect(deleteButton,&QToolButton::clicked,this,&MeshInfoWidget::deleteMesh);
    deleteButton->setIcon(QIcon(":/images/icon/fdm_delete_icon_n.png"));
    deleteButton->setIconSize(QSize(20,20));
    hbox->addWidget(deleteButton,0,Qt::AlignRight);

    QListWidgetItem *item = new QListWidgetItem(m_listWidget);
  //  widget->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_listWidget->setItemWidget(item,widget);

    if(selected) {
        m_listWidget->setCurrentItem(item);
    }
    m_listWidget->verticalScrollBar()->setValue(verticalScrollBarValue);
}

void MeshInfoWidget::init()
{
    //qDebug() << "init().";
    CHDocPtr docPtr = getDoc();
    auto  docs = docPtr->m_printObjs;
    for(auto i = 0 ; i <docs.size(); ++i) {
        //qDebug() << "name: " << docs[i]->getObjectName() << ", status: " << docs[i]->getStatus();
        addItem(docs[i]->getObjectName(), docs[i]->getVisuable(), docs[i]->getStatus() == selected ? true : false, i);
    }
    int height = 0;
    if(docs.size() > 5 ) {
        height = 30 * 5;
    } else {
        height = 30 * docs.size();
    }
    m_listWidget->setVisible(height == 0 ? false : true);

    m_listWidget->setFixedHeight(height);
    //  this->setFixedHeight(27 + 50 * docs.size());
}

bool MeshInfoWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched != m_listWidget) {
        return QWidget::eventFilter(watched, event);
    }
    if(event->type() == QEvent::KeyPress ) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if(keyEvent->modifiers() == Qt::ControlModifier && keyEvent->key() == Qt::Key_A) {

            QTimer::singleShot(0, [&](){
                if(docPtrStatusChanged(getDoc())) {
                    emit  currentChanged();
                }
            });
        }
    }
   // qDebug() << "event==== " << event->type();
    return QWidget::eventFilter(watched, event);
}

void MeshInfoWidget::showMesh()
{
    QToolButton *button = qobject_cast<QToolButton *>(sender());
    QListWidgetItem *item = m_listWidget->itemAt(button->parentWidget()->pos());
   // item->setSelected(true);
    QVariant var = button->property("index");
    CHDocPtr docPtr = getDoc();
    std::vector<CH3DPrintModelPtr> ptrList;
    CH3DPrintModelPtr  modelPtr = findCurrentModel(docPtr,var.toInt());    
    if(modelPtr == 0) {
        return;
    }
    bool visuable = modelPtr->getVisuable();
    ObjStatus status = modelPtr->getStatus();
    std::vector<CHMeshShowObjPtr>  meshes;
    //qDebug() <<" status ===" << status  << " name =" << modelPtr->getObjectName();
    if(status != ObjStatus::selected) {
        // std::vector<CHMeshShowObjPtr>  meshes;

        meshes.push_back(std::dynamic_pointer_cast<CHMeshShowObj>(modelPtr));
    }
    else {

        CHPickOperationCommandPtr pickPtr =  getGlobalPick();
        std::set<CHMeshShowObjPtr> ptr = pickPtr->m_selectedObjs;
        if(ptr.empty()) {

            for(int i = 0; i < m_listWidget->count(); i++)
            {
                if(docPtr->m_printObjs[i] != nullptr && m_listWidget->item(i)->isSelected())
                {
                    meshes.push_back(docPtr->m_printObjs[i]);
                    docPtr->m_printObjs[i]->setStatus(general);
                }
            }
            getDoc()->setObjsVisuable(meshes, !visuable);
            return;
        }

        for(auto iter = ptr.begin() ; iter != ptr.end() ; ++iter) {
            meshes.push_back(std::dynamic_pointer_cast<CHMeshShowObj>(*iter));
        }
    }
    getDoc()->setObjsVisuable(meshes, !visuable);
}

void MeshInfoWidget::deleteMesh()
{
    QToolButton *button =qobject_cast<QToolButton *>(sender());
    QVariant var = button->property("index");

    CHDocPtr docPtr = getDoc();
    std::vector<CH3DPrintModelPtr> ptrList;
    CH3DPrintModelPtr  modelPtr = findCurrentModel(docPtr,var.toInt());
    if(modelPtr == 0) {
        return;
    }

    std::vector<CH3DPrintModelPtr> meshes;
     ObjStatus status = modelPtr.get()->getStatus();
    if(status != ObjStatus::selected) {
        // std::vector<CHMeshShowObjPtr>  meshes;
        meshes.push_back(std::dynamic_pointer_cast<CH3DPrintModel>(modelPtr));
    }
    else {

        CHPickOperationCommandPtr pickPtr =  getGlobalPick();
        std::set<CHMeshShowObjPtr> ptr = pickPtr->m_selectedObjs;
        if(ptr.empty()) {
            meshes.push_back(std::dynamic_pointer_cast<CH3DPrintModel>(modelPtr));
        }
        else
        {
            for(auto iter = ptr.begin() ; iter != ptr.end() ; ++iter) {
                meshes.push_back(std::dynamic_pointer_cast<CH3DPrintModel>(*iter));
            }
        }
    }
   // meshes.push_back(std::dynamic_pointer_cast<CH3DPrintModel>(modelPtr));

    getDoc()->deleteObjs(meshes);

//        CHPickOperationCommandPtr ptr = getGlobalPick();
//        ptr->m_selectedObjs;
//        ptr->deleteSelectedObjs();

}


void MeshInfoWidget::hideList()
{
    if(m_listWidget->height() <= 0 ) {
        return;
    }
    m_listWidget->setVisible(m_listWidget->isVisible() ? false : true);
    m_expandButton->setIcon(QIcon(m_listWidget->isVisible() ? ":/images/icon/fdm_pack_icon_n.png" :":/images/icon/fdm_an_icon_n.png"  ));
}

//void MeshInfoWidget::clickCurrentIndex(const QModelIndex &index)
//{
//    //qDebug() << "clickCurrentIndex ...............";
//    if(!index.isValid()) {
//        return;
//    }

////    if(docPtrStatusChanged(getDoc())) {
////        emit  currentChanged();
////    }
//}

void MeshInfoWidget::selectedChanged()
{
    if(flag) {
        flag = false;
        return;
    }
      qDebug() << "selectedChanged ...............";
    
    if(docPtrStatusChanged(getDoc())) {
        emit  currentChanged();
    }
}

void MeshInfoWidget::updateItem()
{
    //qDebug() << " update ..." << "flag ===" << flag  << "listWidget count = " << m_listWidget->count()  << " sender == " << (sender());
    if(m_listWidget->count() > 0) {
        flag = true;
    }else {
        flag = false;
    }
    verticalScrollBarValue = m_listWidget->verticalScrollBar()->value();
    m_listWidget->clear();

    init();
}

void MeshInfoWidget::currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    qDebug() << "currentItemChanged ." << current;
}

void MeshInfoWidget::currentRowChanged(int currentRow)
{
  //  qDebug() << "verticalScrollBar current index ==" << m_listWidget->verticalScrollBar()->value();

    if(currentRow < 0)
    {
        return;
    }

    if(flag) {
        flag = false;
        return;
    }

    
    if(docPtrStatusChanged(getDoc())) {
        emit  currentChanged();
    }
}


CH3DPrintModelPtr MeshInfoWidget::findCurrentModel(CHDocPtr docPtr,int index)
{
    auto  docs = docPtr->m_printObjs;
    if(docs.size() > index) {
        return docs[index];
    }
    return 0;
}

bool MeshInfoWidget::docPtrStatusChanged(CHDocPtr docPtr)
{
    bool changedFlag = false;
    int count = m_listWidget->count();
    auto printObjs =  docPtr->m_printObjs;
    if(printObjs.empty()) {
        return changedFlag;
    }

    for(auto i = 0; i <count; ++i) {
        QListWidgetItem *item =  m_listWidget->item(i);
        if(!item) {
            continue;
        }
        ObjStatus status = printObjs.at(i)->getStatus();
        bool st = (status == ObjStatus::selected  ? true : false );
        if(st != item->isSelected()) {
            changedFlag = true;
            printObjs.at(i)->setStatus(item->isSelected() ? ObjStatus::selected : ObjStatus::general);
            printObjs.at(i)->updateToScene();
        }
    }
    return changedFlag;
}

void MeshInfoWidget::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange) {
        if (m_title) {
            m_title->setText(tr("Object List"));
        }
    }
}

}
