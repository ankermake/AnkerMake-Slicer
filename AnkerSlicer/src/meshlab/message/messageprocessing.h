#ifndef MESSAGEPROCESSING_H
#define MESSAGEPROCESSING_H

#include <common/plugins/interfaces/meshlab_plugin.h>

class MessageProcessing : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    void sendMsg2NetworkAKeyPrint(const QString &filePath); 
    void sendMsg2NetworkLogin();

public slots:
    void recMsgfromManager(PluginMessageData metadata);  // fromPlugin fromManager

    
	void receiveModelObjsStatusChangedFromDoc();
	void receiveModelTransformChangedFromDoc();
    void receiveModelOutOfRangeChangedFromDoc(bool allInPrintBox);
    void receiveVisibleModelCountChangedFromDoc(int modelCount);
    void receiveModelSupportNumberChangedFromDoc(int sptCount);
    
    void sendMsg2Preview();

signals:
    void sendMsg2Manager(PluginMessageData metadata);
    void sendMsg2GetpreviewWid(QObject *object);
    void checkoutPreview(bool isC);

    void networkSendWid2AnkerMake(QObject *object);
    void swtichNetworkWidgetSignal();
    void networkDeadSig();
    void mainwindowModalSig(int wid);

    //void sendMainwindowPtr2Network(QMainWindow *);

    //update
    void sendMsg2Update(PluginMessageData metadata);
private:
    bool processingWriteAllMeshModelsToStlFile(PluginMessageData& metadata);
};

#endif // MESSAGEPROCESSING_H
