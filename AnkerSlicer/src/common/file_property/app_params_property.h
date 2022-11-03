/****************************************************************************
Revision 0.1  2022/01/07 9:38:03  Aden Hu
****************************************************************************/

#ifndef APP_PARAMS_PROPERTY_H
#define APP_PARAMS_PROPERTY_H

#include <list>
#include <string>


class AppParamsProperty
{
public:
	AppParamsProperty();
	~AppParamsProperty();

	inline void clear()
	{
		m_recentFileList.clear();
		m_language = Language_Unknown;
		m_version = "";
	}

	enum LanguageType
	{
		Language_Unknown = -1,
		Language_Chinese_0 = 1,
		Language_English_US = 2,
	};

	std::list<std::string> m_recentFileList;
	LanguageType m_language;

	//Default Register
	std::string m_version;	//????

	inline AppParamsProperty& operator= (const AppParamsProperty& _appParamsProp)
	{
		m_recentFileList = _appParamsProp.m_recentFileList;
		m_language = _appParamsProp.m_language;
		m_version = _appParamsProp.m_version;

		return *this;
	}
};






#endif // !APP_PARAMS_PROPERTY_H
