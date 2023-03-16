/*



*/


#ifndef CHObject_H
#define CHObject_H

#include "QString"
#if defined(QT_SHARED)
#ifdef COMMONLIB
#define COMMONLIB_EXPORT Q_DECL_EXPORT
#else
#define COMMONLIB_EXPORT Q_DECL_IMPORT
#endif
#else
#define COMMONLIB_EXPORT
#endif

class COMMONLIB_EXPORT CHObject
{
public:
	CHObject();
	~CHObject();


public:

	qint64 getId();
	void setId(qint64 id);
	void setObjectName(const QString& name);
	QString getObjectName();


protected:
	qint64 m_id;
	QString m_name;

};


#endif
