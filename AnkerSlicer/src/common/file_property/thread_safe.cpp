#include "thread_safe.h"

ThreadObject::ThreadObject(QObject* parent) : QObject(parent)
{
	installEventFilter(this);
	moveToThread(&m_thread);
}

ThreadObject::~ThreadObject()
{
	m_thread.quit();
	m_thread.wait();
}

bool ThreadObject::eventFilter(QObject* watched, QEvent* event)
{
	Q_UNUSED(watched);

	switch (event->type())
	{
	case QEvent::ThreadChange:
		connect(&m_thread, &QThread::started, this, &ThreadObject::init);
		m_thread.start();
		return true;
	default:
		return false;
	}
}

void ThreadObject::init()
{

}
