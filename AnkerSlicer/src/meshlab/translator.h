#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QObject>
#include <QTranslator>
#include <QApplication>
enum class Language
{
    EN,
    CH,
    JA
};

typedef std::list<std::string> TStringList;
typedef std::map<std::string, std::string> TStringMap;
class TranslatorP
{
public:
    std::string activatedLanguage; /**< Active language */
    std::map<std::string, std::string> mapLanguageTopLevelDomain;
    TStringMap mapSupportedLocales;
    std::list<QTranslator*> translators; /**< A list of all created translators */
    QStringList paths;
};

class Translator : public QObject
{
    Q_OBJECT
public:
    static Translator* instance();
    void removeTranslators();
    QStringList directories() const;
    TranslatorP *GetTranslatorP() { return d;};
signals:
    void languagechanged(QString lanaguage);
//    explicit Translator(QObject *parent = nullptr);

public slots:
    void loadLanguage(Language lang);

private:
    static Translator *m_transInstance;
    Translator(QObject *parent = nullptr);
    ~Translator();
    TranslatorP* d = nullptr;
};

#endif // TRANSLATOR_H
