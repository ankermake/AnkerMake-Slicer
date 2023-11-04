#ifndef IMPORT_PROJECT_THREAD_H
#define IMPORT_PROJECT_THREAD_H

#include "import_model_thread.h"
#include <QObject>
#include <QThread>

//class ImportProjectThread : public QThread
//{
//    Q_OBJECT

//public:
//    explicit ImportProjectThread(QObject *parent = nullptr);
//    void run();
//    void setPrintModelNumber(int num);
//    void initOpenModelParams();

//signals:
//    void openModelSignal(int);

//private:
//    int m_num; 
//};
class ImportProjectWorker : public QObject
{
    Q_OBJECT

public:
    explicit ImportProjectWorker(QObject *parent = nullptr);
    void initImportProjectParams(const QString &proPath, std::vector<CH3DPrintModelPtr> *printModels, vcg::CallBackPos *cb);
public slots:
    void doWork();

signals:
    void importModelSignal(int num, int totalsNum, const QString& str);
    void exitThreadSignal();
    void errorEncountered(int , const QString &);

private:
    QString m_proPath;
    std::vector<CH3DPrintModelPtr> *m_printModels;
    vcg::CallBackPos *m_cb;
    bool successful = true;
};


#endif
