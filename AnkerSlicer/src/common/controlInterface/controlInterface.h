#ifndef CONTROLINTERFACE_H
#define CONTROLINTERFACE_H
#include <QObject>
#include <QMenu>
#include <QToolBar>
#include <QWidget>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include "../ml_document/mesh_document.h"
#include "../common/ak_const.h"
#include "pagewidget.h"
namespace  control {

	namespace constants {

		//const char MENBAR[]  =  "MenBar";
		const char M_FILE[] = "Menu.File";
		const char M_EDIT[] = "Menu.Edit";
		const char M_CONNECT[] = "Menu.Connect";
		const char M_SETTINGS[] = "Menu.Settings";
		const char M_HELP[] = "Menu.Help";
		const char M_SCENE[] = "Menu.Scene";
	}

	class ControlId {
	public:
		ControlId(const char key[]) { m_id = QString(key); }
		ControlId(const QString& key) { m_id = key; }
		QString name() { return m_id.split(".").last(); }
		QString id() { return m_id; }
	private:
		QString m_id;
	};

	class ControlInterface :public QObject
	{
		Q_OBJECT
	public:
		virtual void addSeparatorToToolBar() = 0;
		virtual void addActionToToolBar(QAction* action) = 0;
		virtual QMenu* addMenu(ControlId  id) = 0;
		virtual void addActionToMenu(QAction* action, ControlId  id) = 0;
		virtual void addMenuToMenu(QMenu* menu, ControlId  id) = 0;

		virtual void addSeparatorToMenu(ControlId id) = 0;

		virtual void addActionToModelTranform(QAction* action) = 0;

		virtual void addWidgetToModelTransForm(QWidget* widget, int actionIndex) = 0;
        virtual void addSeparatorToModelTranform() {};

//        virtual void addFdmSettings(QWidget* widget) = 0;

//        virtual void addSliceWidget(QWidget* widget) = 0;

//		virtual void repaceWidget(int index, QWidget* widget) = 0;
        virtual void addWidgetToRightSettings(AkConst::EFdmRightWidgetIndex index,  QWidget* widget) = 0;

        //virtual void addWidgetToView(QWidget* widget) = 0;
		virtual void addActionToViewTransform(QAction* action) = 0;

		virtual void addMeshInfoWidget() = 0;

		virtual void addProgressBar(QWidget* widget) = 0;

		virtual void addPageToPreferences(PageWidget* widget, int index) = 0;
        virtual void openPreferences(int index) = 0;
		// void openPreferences(QWidget *widget){};

	signals:
		void openPreferencesDialog(int index);
        void switchPreferencesPage(int index);

	};

}
#endif // CONTROLINTERFACE_H
