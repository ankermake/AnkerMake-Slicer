#include "translator.h"
#include <QDir>
Translator *Translator::m_transInstance = nullptr;


Translator::Translator(QObject *parent)
{
     d = new TranslatorP;
     d->mapLanguageTopLevelDomain[QT_TR_NOOP("English"              )] = "en";
    d->mapLanguageTopLevelDomain[QT_TR_NOOP("Deutsch"               )] = "de";
    d->mapLanguageTopLevelDomain[QT_TR_NOOP("Français"               )] = "fr";
    d->mapLanguageTopLevelDomain[QT_TR_NOOP("Italiano"              )] = "it";
    
    d->activatedLanguage = "English";
}

Translator::~Translator()
{
    delete d;
}

Translator *Translator::instance()
{
    if(!m_transInstance) {
        m_transInstance = new Translator();
    }
    return m_transInstance;
}

void Translator::loadLanguage(Language lang)
{
    removeTranslators();
    d->paths.clear();
    QTranslator* translator = new QTranslator;
    if(lang == Language::EN) {
        d->paths.push_back(":/language/tr_en.qm");
    }
    else if(lang == Language::CH) {
        d->paths.push_back(":/language/tr_zh.qm");
    } else if (lang == Language::JA) {
        d->paths.push_back(":/language/tr_ja.qm");
    }
    if (translator->load(d->paths.back())) {
        qApp->installTranslator(translator);
        d->translators.push_back(translator);
    }
    else {
        delete translator;
    }
}

void Translator::removeTranslators()
{
    for (std::list<QTranslator*>::iterator it = d->translators.begin(); it != d->translators.end(); ++it) {
        qApp->removeTranslator(*it);
        delete *it;
    }

    d->translators.clear();
}

