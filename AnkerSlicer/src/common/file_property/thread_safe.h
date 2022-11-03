/****************************************************************************
Revision 0.1  2022/01/18 19:38:03  Aden Hu
****************************************************************************/
#ifndef THREAD_SAFE_H
#define THREAD_SAFE_H
#include <QObject>
#include <QThread>
#include <QString>
#include <QEvent>
#include <QDebug>

class ThreadObject : public QObject
{
	Q_OBJECT
public:
	explicit ThreadObject(QObject *parent = nullptr);
	virtual ~ThreadObject();

	bool eventFilter(QObject* watched, QEvent* event);

private slots:
	virtual void init();

private:
	QThread m_thread;
};


/*class myThread : public ThreadObject
{
public:
	myThread()
	{

	}

private slots:
	void init()
	{
		qDebug() << QThread::currentThreadId();
	}

public slots:
	void _slot()
	{
		qDebug() << QThread::currentThreadId();
	}
};	*/


#endif // !THREAD_SAFE_H
