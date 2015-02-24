#include "nesca_3.h"
#include <QApplication>
#if defined(Q_OS_WIN32)
#include <windows.h>
#include <qfontdatabase.h>
#endif

//--Fixing _ITERATOR_DEBUG_LEVEL 0!=2
//#if defined(Q_OS_WIN32)
//int WINAPI WinMain(HINSTANCE hInstance,
//  HINSTANCE hPrevInstance, LPSTR lpcmdline, int ncmdshow)
//{
//	QApplication a(ncmdshow, (char **)lpcmdline);
//#else
//int main(int argc, char *argv[])
//{
//	QApplication a(argc, argv);
//#endif
//#if defined(Q_OS_WIN32)
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QStringList list;
	list << "Eurostile.ttf";
	int fontID(-1);
	bool fontWarningShown(false);
	for (QStringList::const_iterator constIterator = list.constBegin(); constIterator != list.constEnd(); ++constIterator) {
		QFile res(":/nesca_3/" + *constIterator);
		if (res.open(QIODevice::ReadOnly) == false) {
			if (fontWarningShown == false) {
				fontWarningShown = true;
			}
		} else {
			fontID = QFontDatabase::addApplicationFontFromData(res.readAll());
			if (fontID == -1 && fontWarningShown == false) {
				fontWarningShown = true;
			}
		}
	}
	
//#endif
nesca_3 *gui = new nesca_3();
	
	gui->showNormal();
	return a.exec();
}
	