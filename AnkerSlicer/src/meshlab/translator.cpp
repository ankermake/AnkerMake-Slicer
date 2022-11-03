#include "translator.h"
#include <QDir>
Translator *Translator::m_transInstance = nullptr;


Translator::Translator(QObject *parent)
{
     d = new TranslatorP;
     d->mapLanguageTopLevelDomain[QT_TR_NOOP("English"              )] = "en";
    d->mapLanguageTopLevelDomain[QT_TR_NOOP("German"               )] = "de";
    d->mapLanguageTopLevelDomain[QT_TR_NOOP("Spanish"              )] = "es-ES";
    d->mapLanguageTopLevelDomain[QT_TR_NOOP("French"               )] = "fr";
    d->mapLanguageTopLevelDomain[QT_TR_NOOP("Italian"              )] = "it";
    d->mapLanguageTopLevelDomain[QT_TR_NOOP("Japanese"             )] = "ja";
    d->mapLanguageTopLevelDomain[QT_TR_NOOP("Chinese Simplified"   )] = "zh-CN";
    d->mapLanguageTopLevelDomain[QT_TR_NOOP("Chinese Traditional"  )] = "zh-TW";
    d->mapLanguageTopLevelDomain[QT_TR_NOOP("Korean"               )] = "ko";
    d->mapLanguageTopLevelDomain[QT_TR_NOOP("Russian"              )] = "ru";
    d->mapLanguageTopLevelDomain[QT_TR_NOOP("Swedish"              )] = "sv-SE";
    d->mapLanguageTopLevelDomain[QT_TR_NOOP("Afrikaans"            )] = "af";
    d->mapLanguageTopLevelDomain[QT_TR_NOOP("Norwegian"            )] = "no";
    d->mapLanguageTopLevelDomain[QT_TR_NOOP("Portuguese, Brazilian")] = "pt-BR";
    d->mapLanguageTopLevelDomain[QT_TR_NOOP("Portuguese"           )] = "pt-PT";
    d->mapLanguageTopLevelDomain[QT_TR_NOOP("Dutch"                )] = "nl";
    d->mapLanguageTopLevelDomain[QT_TR_NOOP("Ukrainian"            )] = "uk";
    d->mapLanguageTopLevelDomain[QT_TR_NOOP("Finnish"              )] = "fi";
    d->mapLanguageTopLevelDomain[QT_TR_NOOP("Croatian"             )] = "hr";
    d->mapLanguageTopLevelDomain[QT_TR_NOOP("Polish"               )] = "pl";
    d->mapLanguageTopLevelDomain[QT_TR_NOOP("Czech"                )] = "cs";
    d->mapLanguageTopLevelDomain[QT_TR_NOOP("Hungarian"            )] = "hu";
    d->mapLanguageTopLevelDomain[QT_TR_NOOP("Romanian"             )] = "ro";
    d->mapLanguageTopLevelDomain[QT_TR_NOOP("Slovak"               )] = "sk";
    d->mapLanguageTopLevelDomain[QT_TR_NOOP("Turkish"              )] = "tr";
    d->mapLanguageTopLevelDomain[QT_TR_NOOP("Slovenian"            )] = "sl";
    d->mapLanguageTopLevelDomain[QT_TR_NOOP("Basque"               )] = "eu";
    d->mapLanguageTopLevelDomain[QT_TR_NOOP("Catalan"              )] = "ca";
    d->mapLanguageTopLevelDomain[QT_TR_NOOP("Galician"             )] = "gl";
    d->mapLanguageTopLevelDomain[QT_TR_NOOP("Kabyle"               )] = "kab";
    d->mapLanguageTopLevelDomain[QT_TR_NOOP("Korean"               )] = "ko";
    d->mapLanguageTopLevelDomain[QT_TR_NOOP("Filipino"             )] = "fil";
    d->mapLanguageTopLevelDomain[QT_TR_NOOP("Indonesian"           )] = "id";
    d->mapLanguageTopLevelDomain[QT_TR_NOOP("Lithuanian"           )] = "lt";
    d->mapLanguageTopLevelDomain[QT_TR_NOOP("Valencian"            )] = "val-ES";
    d->mapLanguageTopLevelDomain[QT_TR_NOOP("Arabic"               )] = "ar";
    d->mapLanguageTopLevelDomain[QT_TR_NOOP("Vietnamese"           )] = "vi";

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

