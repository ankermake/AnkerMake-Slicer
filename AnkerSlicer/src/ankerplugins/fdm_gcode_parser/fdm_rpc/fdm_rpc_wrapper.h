#include "rep_fdm_rpc_source.h"

class fdmRpcWrapper : public fdmRpcSimpleSource
{
    Q_OBJECT
public:
    explicit fdmRpcWrapper(QObject *parent);

    void pubMsgFromFdmGcodePaser(const QString &msg);

    virtual void sendGCodePrintSignal(const QString &msg);

    virtual void msgFromGcodepreview(int id);

    virtual void checkoutPreviewEvent(bool kk);

    virtual void previewOpenFileEvent();

    //virtual void previewCloseSlot(int id);
};
