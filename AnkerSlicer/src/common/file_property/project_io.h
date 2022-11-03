/****************************************************************************
Revision 0.1  2022/01/13 15:04:30  Aden Hu
****************************************************************************/

#ifndef PROJECT_IO_H
#define PROJECT_IO_H
#include "file_base.h"
#include "../ml_document/mesh_document.h"
#include "QtGui/5.15.2/QtGui/private/qzipreader_p.h"
#include "QtGui/5.15.2/QtGui/private/qzipwriter_p.h"
#include "file_property.h"
#include "xml_stream.h"
#include <QObject>
#include <QString>

class ProjectIO : public FileBase , public QObject
{
	Q_OBJECT
public:
	ProjectIO();
	virtual ~ProjectIO();

public:
	bool loadZip(QString filePath/*????*/, const QString& proName/*???*/, const QString& outPath/*????????*/);
	bool saveZip(const QString& filePath/*???????????*/, const QString& folderName/*???????*/, const QString& proName/*???*/, QString& layer/*??*/, const QString& outPath/*????????*/);
	bool QZipWriterEx(QZipWriter* writer, QString dirPath, QString layer);

	bool readAllXml(const QString& proPath/*????????????*/);
	bool readXml(const QString& dirPath/*??????*/, const QString& xmlName/*xml??*/, const XmlStream::XmlStreamType& xmlType);
	bool writeAllXml(const QString& proPath/*??????????*/);
	bool writeXml(const QString& dirPath/*??????*/, const QString& xmlName/*xml??*/, const XmlStream::XmlStreamType& xmlType);

	bool deleteDir(const QString& proDir/*??????*/);
	bool saveModelFolder(const QString& dirPath/*?????????*/, const QString& folderName);
	bool loadModelFolder(const QString& dirPath/*?????????*/, const QString& folderName);
	bool importModel(const QString& dirPath/*?????????*/, const QString& modelName, MeshDocument& doc);
	bool exportModel(const QString& dirPath/*?????????*/, const QString& modelName, MeshDocument& doc);
	bool exportAllModel(const QString& proDir/*??????*/, MeshDocument& doc);

	void setDocumentPropertyFromMeshDocument(const MeshDocument& doc);
	void setAppPropertyFromMain(const AppParamsProperty& params);  //???????????????
	void setViewPropertyFromViewCK(const ViewProperty& view);//        ?chuck???view????????
};




#endif // !PROJECT_IO_H

