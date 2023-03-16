#ifndef SEARCHER_H
#define SEARCHER_H

#include<QString>
#include<QMap>
#include<QList>
#include<QAction>
#include<QRegExp>
#include<QVector>
#include<QSet>
#if defined(QT_SHARED)
#ifdef COMMONLIB
#define COMMONLIB_EXPORT Q_DECL_EXPORT
#else
#define COMMONLIB_EXPORT Q_DECL_IMPORT
#endif
#else
#define COMMONLIB_EXPORT
#endif

class COMMONLIB_EXPORT WordActionsMap
{
public:
	WordActionsMap();
	void addWordsPerAction(QAction& act,const QStringList& words);
	void removeActionReferences(QAction& act);
	bool getActionsPerWord( const QString& word,QList<QAction*>& res ) const;
	void clear();
private:
    QMap<QString,QList<QAction*> > wordacts;
};

class RankedMatches;

class COMMONLIB_EXPORT WordActionsMapAccessor
{
public:
	WordActionsMapAccessor();
	void addWordsPerAction(QAction& act,const QString& st);
	inline void removeActionReferences(QAction& act) {map.removeActionReferences(act);}
	inline void setSeparator(const QRegExp& sep) {sepexp = sep;}
	inline void setIgnoredWords(const QRegExp& ign) {ignexp = ign;}
	int rankedMatchesPerInputString(const QString& input,RankedMatches& rm) const;
	inline QRegExp separtor() const {return sepexp;}
	inline QRegExp ignored() const {return ignexp;}
	void clear() {map.clear(); sepexp = QRegExp(); ignexp = QRegExp();}

private:
	void purifiedSplit(const QString& input,QStringList& res) const;
	void addSubStrings(QStringList& res) const;
	WordActionsMap map;
	QRegExp sepexp;
	QRegExp ignexp;
};

class COMMONLIB_EXPORT RankedMatches
{
public:
	RankedMatches();
	void getActionsWithNMatches(const int n,QList<QAction*>& res); 
private:
	friend int WordActionsMapAccessor::rankedMatchesPerInputString(const QString& input,RankedMatches& rm) const;
	int computeRankedMatches(const QStringList& inputst,const WordActionsMap& map,bool matchesontitlearemoreimportant = true);
    QVector<QList<QAction*> > ranking;
};

#endif
