#ifndef EXPORT_MODEL_THREAD_H
#define EXPORT_MODEL_THREAD_H

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

class COMMONLIB_EXPORT ExportModelThread : public QThread
{
    Q_OBJECT

public:
    explicit ExportModelThread(QObject *parent = nullptr);
    ~ExportModelThread();

    void run();
    void initExportParam(const QString& formatName, const QString& fileName, CMeshO* cm, int mask, vcg::CallBackPos* cb);

public slots:

signals:
    void exportData(int pos, const QString &str);
    void errorEncountered(int, const QString &);

private:
    QString m_formatName;
    QString m_fileName;
    CMeshO *m_cm;
    vcg::CallBackPos *m_cb;
    int m_mask;
    bool successful = true;
};

#endif

