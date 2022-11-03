#ifndef FDMSETTINGMANAGER_H
#define FDMSETTINGMANAGER_H

#include "fdmparamdata.h"
#include "filemanage/fdmsettingvisibilityconfig.h"

/* TODO :

*/

class FdmSettingManagerHandle;
class FdmQml_Engine;
using namespace fdmsettings;
class FdmSettingManager : public QObject
{
    Q_OBJECT

    /// Construct and destructor
public:
    explicit FdmSettingManager(QObject *parent=nullptr);
    ~FdmSettingManager();


    /// getter
    FdmParamNode * runningParamsRoot(){return (FdmParamNode *)m_runningParams; }
    QHash<QString,QStringList>  runningVisual(){return m_runningVisual; }

    //for FdmParamSettingsWidget
    QList<FdmParamNode *> loadParamList(const QString &file);

    //get setting for slice
    QString getCustomSetting();
    /// slots and signals
public slots:
    /** TODO:


     */
    void saveCustomJsonFile(const QString & saveFile){Q_ASSERT(0);}

    /** TODO:



     */
    void saveCustomIni(const QString & saveFile);

    /** TODO:


     * @param iniFile
     */
    void resetCustomIni(const QString & iniFile);

//    /** TODO:



//     */
//    void saveMachineIni(const QString & saveFile);
//    /** TODO:


//     * @param iniFile
//     */
//    void resetMachineIni(const QString & iniFile);


    /**


     *

     */
    void resetPrinterJsonFile(const QString & jsonFile);

    /** TODO:

     * @param cfgFile
     */
    void resetVisualCfgFile(const QString & cfgFile);


    /** TODO:

     * @param language
     */
    void resetLocaleLanguage(const QString & language);

signals:
    /** TODO:


     */
    void printerSettingFileChange(FdmParamRoot *runningParams);

    /** TODO:

     * @param iniMap
     */
    void visualCfgFileChange( QHash<QString,QStringList> iniMap);

    /// member function
private:
    /** TODO:
     * @brief jsonPathByMachineName jsonPathByCustomCfg iniPathByVisualLevel

     * @param
     * @return
     */
    QString jsonPathByMachineName(const QString & machineName)  { return machineName; }
    QString jsonPathByCustomConfig(const QString & customConfig){ return customConfig;}
    QString cfgPathByVisualOption(const QString & visualOption) { return visualOption; }

    /** TODO:

     * @param jsonPath

     */
    FdmParamRoot *parsePrinterJsonFile(const QString &jsonPath);

    /** TODO:

     * @param cfgPath
     * @return
     */
    QHash<QString,QStringList> parseVisualCfgFile(const QString & cfgPath);


    ///  member variable
private:
    FdmQml_Engine *m_qmlEngine{nullptr};

    QHash<QString, FdmParamRoot *> m_jsonFileMap;
    QHash<QString, QMap<QString,QStringList> > m_visualCfgFileMap;

    QHash<QString,QStringList> m_runningVisual;
    FdmParamRoot * m_runningParams{nullptr};

    QString m_language;
    //Qunorderedmap

    void init();

    //for expert.cfg expert.cfg advanced.cft
    FdmSettingVisibilityConfig *m_config;
};

#endif // FDMSETTINGMANAGER_H
