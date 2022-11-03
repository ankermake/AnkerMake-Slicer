#ifndef EXPORT_PROJECT_THREAD_H
#define EXPORT_PROJECT_THREAD_H

#include <QObject>
#include <QThread>
#include "mesh_model_import_export.h"
#include "../common/GeoAndShow/CH3DPrintModel.h"

class ExportProjectThread : public QThread
{
    Q_OBJECT
public:
    explicit ExportProjectThread(QObject* parent = nullptr);
    void run();
    void setPrintModels(const std::vector<CH3DPrintModelPtr>& printModels);
    void initExportProjectParams(const std::vector<CH3DPrintModelPtr>& printModels, const QString& proDir, int mask, vcg::CallBackPos* cb);
    bool exportModel(const QString& dirPath, const QString& modelName, const CMeshO& _cm);
signals:
    void exportModelSignal(int num, int totalsNum, const QString& str);

private:
    QString m_proDir;
    int m_mask;
    vcg::CallBackPos* m_cb;

    std::vector<CH3DPrintModelPtr> m_printModels;
};

class ExportProjectWorker : public QObject
{
    Q_OBJECT

public:
    explicit ExportProjectWorker(QObject* parent = nullptr);
    void initExportProjectParams(const std::vector<CH3DPrintModelPtr>& printModels, const QString& proDir, int mask, vcg::CallBackPos* cb);
    bool exportModel(const QString& dirPath, const QString& modelName, CMeshO& _cm);

public slots:
    void doWork();

signals:
    void exportModelSignal(int num, int totalsNum, const QString& str);
    void exitThreadSignal();
    void errorEncountered(int, const QString &);

private:
    QString m_proDir;
    int m_mask;
    vcg::CallBackPos* m_cb;

    std::vector<CH3DPrintModelPtr> m_printModels;
    bool successful = true;
};

#endif
