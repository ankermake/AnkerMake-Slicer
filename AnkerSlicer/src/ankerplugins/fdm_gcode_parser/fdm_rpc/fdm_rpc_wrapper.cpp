#include "fdm_rpc_wrapper.h"

fdmRpcWrapper::fdmRpcWrapper(QObject *parent):fdmRpcSimpleSource(parent)
{

}

void fdmRpcWrapper::pubMsgFromFdmGcodePaser(const QString &msg)
{
    emit msgFromFdmGcodePaser(msg);
}

void fdmRpcWrapper::sendGCodePrintSignal(const QString &msg)
{
    qDebug() << "gcode file: " << msg;
    emit sendGCodePrint(msg);
}


void fdmRpcWrapper::msgFromGcodepreview(int id)
{
    qDebug() << "progress: " << id;
    emit progress(id);
}

void fdmRpcWrapper::checkoutPreviewEvent(bool kk)
{
    qDebug() << "checkoutPreviewEvent: " << kk;
    emit cheeckoutPreview(kk);
}


//void fdmRpcWrapper::previewCloseSlot(int id)
//{
//    qDebug() << "close this id: " << id;
//}
