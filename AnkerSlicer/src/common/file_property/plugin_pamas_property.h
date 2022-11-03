/****************************************************************************
Revision 0.1  2022/01/07 9:38:03  Aden Hu
****************************************************************************/

#ifndef PLUGIN_PARAMS_PROPERTY_H
#define PLUGIN_PARAMS_PROPERTY_H

#include <list>
#include <string>
class PluginPamasProperty
{
public:
	PluginPamasProperty();
	~PluginPamasProperty();

	inline void clear()
	{
		pluginNames.clear();
	}

	std::list<std::string> pluginNames;

	inline PluginPamasProperty& operator= (const PluginPamasProperty& _pluginParamsProp)
	{
		pluginNames = _pluginParamsProp.pluginNames;
		return *this;
	}
};




#endif // !PLUGIN_PARAMS_PROPERTY_H
