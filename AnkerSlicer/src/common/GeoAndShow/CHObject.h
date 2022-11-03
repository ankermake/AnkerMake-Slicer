/*



*/


#ifndef CHObject_H
#define CHObject_H

#include "QString"


class CHObject
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
