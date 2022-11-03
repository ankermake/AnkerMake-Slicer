#ifndef ANKER_PLUGIN_DEMO_H
#define ANKER_PLUGIN_DEMO_H

#include <common/plugins/interfaces/anker_plugin.h>
#include <QTimer>
#include <QThread>
#include <QDebug>


class AnkerPluginDemo : public QObject, public AnkerPlugin
{
    Q_OBJECT
    MESHLAB_PLUGIN_IID_EXPORTER(Anker_PLUGIN_IID)
    Q_INTERFACES(AnkerPlugin)

    /**

     * @return
     */
    QString pluginName() const final{return QString("AnkerPluginDemo");}

public:
    /**

     */
    AnkerPluginDemo(){}
    ~AnkerPluginDemo(){}

    /**



     */
    void initialize(ControlInterface *controlmanager, RichParameterList * globalParameterList) override final{}
    //void initialize(ControlInterface *controlmanager){}


    /**

     * @param msgBody
     */
    void demo1MessageProcessing(PluginMessageData msgBody){
        qDebug()<< __LINE__ << __FUNCTION__;
        //  do something
    }
    void demo2CommandProcessing(PluginMessageData msgBody){
        qDebug()<< __LINE__ << __FUNCTION__;
        //  do something

        PluginMessageData replyMessage;
        replyMessage.from = msgBody.dest; // = this->pluginName();
        replyMessage.dest = msgBody.from;
        replyMessage.msg = QString("Reply to something");
        

        if(true){   
            emit sendMsg2Manager(replyMessage);
        }
        else{       
            QTimer::singleShot(0, this, [this, replyMessage](){
                emit sendMsg2Manager(replyMessage);
            });
        }
    }
private:
    QThread * thread{nullptr};
    QObject * asyncThreadContext{nullptr};
    void demo3AsyncInit(){
        thread = new QThread(this);
        thread->start();
        asyncThreadContext = new QObject();
        asyncThreadContext->moveToThread(thread);
        QObject::connect(this, &QObject::destroyed, [&](){
            thread->quit();
            thread->wait();
            thread->deleteLater();
            asyncThreadContext->deleteLater();
            thread = nullptr;
            asyncThreadContext = nullptr;
        });
    }
public:
    void demo3AsyncProcessing(PluginMessageData msgBody){
        if(!asyncThreadContext){
            demo3AsyncInit();
        }

        qDebug()<< __LINE__ << __FUNCTION__ << QThread::currentThreadId();
        QTimer::singleShot(0, asyncThreadContext, [](){


            qDebug()<< __LINE__ << __FUNCTION__ << QThread::currentThreadId();
            //  do something in Asynchronous thread
        });
    }

public slots:
    /**


     */
    void recMsgfromManager(PluginMessageData msgBody)override final{
        if (msgBody.msg == QString("DEMO1: The message")){
            demo1MessageProcessing(msgBody);
            return;
        }
        if (msgBody.msg == QString("DEMO2: The command")){
            demo2CommandProcessing(msgBody);
            return;
        }
        if (msgBody.msg == QString("DEMO3: The Asynchronous")){
            demo3AsyncProcessing(msgBody);
            return;
        }
        if(true){ // test
            demo3AsyncProcessing(msgBody);
            return;
        }
    }
signals:
    /**


     */
    void sendMsg2Manager(PluginMessageData msgBody)override final;
};



#endif // ANKER_PLUGIN_DEMO_H
