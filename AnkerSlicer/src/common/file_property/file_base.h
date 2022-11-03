/****************************************************************************
Revision 0.1  2022/01/13 15:04:30  Aden Hu
****************************************************************************/

#ifndef FILE_BASE_H
#define FILE_BASE_H

#include <QString>
#include <iostream>
#include "file_property.h"
class FileBase
{
public:
	FileBase();
	virtual  ~FileBase();

	virtual bool needSaveProject() { return false; }//???????????
	virtual bool saveProject(const QString& projectPath);//?????????????????????????????
	virtual bool loadProject(const QString& projectPath);

protected:
	std::shared_ptr<FileProperty> fileProperty_ptr;
};




#endif // !FILE_BASE_H
