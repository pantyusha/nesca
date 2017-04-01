#include "nesca_3.h"
#include <QApplication>
#include <qfontdatabase.h>
#include <QMessageBox>
#include <qpixmap.h>
#include "Utils.h"
#include "externData.h"

int main(int argc, char *argv[])
{
	bool isWM = false;
	//DWORD Type;
	//char value[512] = { 0 };
	//char resultString[512] = { 0 };
	//HKEY hkey;
	//if (RegOpenKey(HKEY_LOCAL_MACHINE,
	//	TEXT("HARDWARE\\DESCRIPTION\\System"), &hkey) == ERROR_SUCCESS)
	//{
	//	DWORD value_length = 512;
	//	RegQueryValueEx(hkey, L"VideoBiosVersion", 0, &Type, (BYTE*)&value, &value_length);
	//	RegCloseKey(hkey);

	//	for (int i = 0, j = 0; i < 256; ++i, j += 2) {
	//		char ch = value[j];
	//		if (ch != '\0') {
	//			resultString[i] = ch;
	//		}
	//		else {
	//			resultString[i] = '\n';
	//		}
	//	}
	//	resultString[256] = '\0';
	//}
	//if (strstr(resultString, "VirtualBox") ||
	//	strstr(resultString, "virtualbox")) {
	//	isWM = true;
	//};

	QApplication a(argc, argv);

	if (isWM) {
		//QMessageBox msgBox(
		//	QMessageBox::Information,
		//	"Nope",
		//	"");
		//QPalette palette;
		//palette.setBrush(QPalette::Background, Qt::cyan);
		//msgBox.setIconPixmap(QPixmap(":/nesca_3/xc.jpg"));
		//msgBox.setPalette(palette);
		//msgBox.setVisible(true);
		//msgBox.exec();
	} else {
		QStringList list;
		list << "small_font.ttf";
		int fontID(-1);
		bool fontWarningShown(false);
		for (QStringList::const_iterator constIterator = list.constBegin(); constIterator != list.constEnd(); ++constIterator) {
			QFile res(":/nesca_3/" + *constIterator);
			if (res.open(QIODevice::ReadOnly) == false) {
				if (fontWarningShown == false) {
					fontWarningShown = true;
				}
			}
			else {
				fontID = QFontDatabase::addApplicationFontFromData(res.readAll());
				if (fontID == -1 && fontWarningShown == false) {
					fontWarningShown = true;
				}
			}
		}

		nesca_3 *gui = new nesca_3(isWM, 0);
		//if (isWM) {
		//	Utils::emitScaryError();
		//}
		gui->showNormal();
	}
	return a.exec();
}
	
