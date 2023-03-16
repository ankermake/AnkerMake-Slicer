#ifndef IMPORT_MODEL_THREAD_H
#define IMPORT_MODEL_THREAD_H

#include <QObject>
#include <QThread>
#include "mesh_model_import_export.h"
#if defined(QT_SHARED)
#ifdef COMMONLIB
#define COMMONLIB_EXPORT Q_DECL_EXPORT
#else
#define COMMONLIB_EXPORT Q_DECL_IMPORT
#endif
#else
#define COMMONLIB_EXPORT
#endif
class COMMONLIB_EXPORT ImportModelThread : public QThread
{
    Q_OBJECT
public:
    explicit ImportModelThread(QObject *parent = nullptr);
    void run();

    //static void open(const QString& formatName, const QString& fileName, CMeshO& cm, vcg::CallBackPos* cb);
    //MeshModelImport_Export::open(extension, fileName, *tmpPrintModelPtr->m_oldMesh, QCallBack_ak);
    void initOpenParam(const QString& formatName, const QString& fileName, CMeshO* cm, vcg::CallBackPos* cb);
    inline bool openFileSuccessful() { return successful;};
    inline bool getExit() const {   return m_exit; }
public slots:
    void exitThreadSLot();
    void usbChanged(int);

signals:
    void resultReady(const QString &s);
    void openData(int pos, const QString &str);
    void exitThreadSignal();
    void errorEncountered(int type, QString errorString);

private:
    QString m_formatName;
    QString m_fileName;
    CMeshO *m_cm;
    vcg::CallBackPos *m_cb;

    bool m_exit = false;
    bool successful = true;
};

class COMMONLIB_EXPORT ImportModelWoker : public QObject
{
    Q_OBJECT

public:
    explicit ImportModelWoker(QObject *parent = nullptr);
    void initOpenParam(const QString& formatName, const QString& fileName, CMeshO* cm, vcg::CallBackPos* cb);

public slots:
    void doWork();

signals:
    void resultReady(const QString &result);
    void openData(int pos, const QString &str);
    void loadingSignal(int pos);
    void exitThreadSignal();

private:
    QString m_formatName;
    QString m_fileName;
    CMeshO *m_cm;
    vcg::CallBackPos *m_cb;
};

class COMMONLIB_EXPORT ImportModelController : public QObject
{
    Q_OBJECT
    QThread workerThread;

public:
    ImportModelController();
    ~ImportModelController();

public slots:
    void handleResults(const QString &);

signals:
    void operate();
};


#endif
