#include "Utils.h"
#include <sstream>
#include "STh.h"

std::string Utils::startDate;
std::string Utils::startTime;
std::string Utils::currentTarget;

//void Utils::emitScaryError() {
//	__asm{
//		push   edx
//			push   ecx
//			push   ebx
//
//			mov    eax, 'VMXh'
//			mov    ebx, 0
//			mov    ecx, 10
//			mov    edx, 'VX'
//
//			in     eax, dx
//			cmp    ebx, 'VMXh'
//
//			pop    ebx
//			pop    ecx
//			pop    edx
//	};
//}

std::string Utils::getHeaderValue(std::string *buff, const std::string headerValue, const std::string outputName) {
	if (buff->size() > 0) {
		int headerSize = headerValue.size();
		int pos = buff->find(headerValue);
		if (-1 != pos) {
			int diff = pos + headerSize;
			std::string fieldChunk = buff->substr(diff, buff->find("\r\n", pos) - diff);
			std::string fieldHeader = outputName + fieldChunk.substr(0, fieldChunk.find(";"));
			return fieldHeader;
		}
		else {
			return "";
		}
	}
	else {
		return "";
	}
}
void Utils::saveStartDate() {
	QDate date = QDate::currentDate();
	startDate = date.toString("dd.MM.yyyy").toUtf8().constData();
}

void Utils::saveStartTime() {
	QTime time = QTime::currentTime();
	startTime = time.toString("HH_mm").toUtf8().constData();
}
std::string Utils::getStartDate() {
	return startDate;
}

int Utils::addBARow(QString str1, QString str2, QString str3, int rowIndex) {
	if (BALogSwitched) {
		if (rowIndex == -1) {
			rowIndex = nesca_3::addBARow(str1, str2, str3);
		}
		else {
			stt->doEmitionChangeBARow(rowIndex, str2, str3);
		}

		return rowIndex;
	}
	
	return -1;
}

std::string Utils::getStartTime() {
	return startTime;
}

void Utils::setCurrentTarget(const std::string target) {
	currentTarget = target;
}

std::string Utils::getCurrentTarget() {
	return currentTarget;
}

int Utils::isDigest(const std::string *buffer) {
	if (Utils::ustrstr(buffer, "401 authorization") != -1
		|| Utils::ustrstr(buffer, "401 unauthorized") != -1
		|| (Utils::ustrstr(buffer, "www-authenticate") != -1
		&& Utils::ustrstr(buffer, "401 ") != -1
		)
		|| Utils::ustrstr(buffer, "401 unauthorized access denied") != -1
		|| Utils::ustrstr(buffer, "401 unauthorised") != -1
		|| (Utils::ustrstr(buffer, "www-authenticate") != -1
		&& Utils::ustrstr(buffer, " 401\r\n") != -1
		)
		) {
		if (Utils::ustrstr(buffer, "digest realm") != -1
			&& Utils::ustrstr(buffer, "basic realm") == -1) {
			return 1;
		}
		else return 0;
	};
	return -1;
}

std::vector<std::string> Utils::splitToStrVector(const std::string &s, char delim) {
	std::vector<std::string> elems;
	std::stringstream ss(s);
	std::string item;

	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}

	return elems;
}
std::vector<int> Utils::splitToIntVector(const std::string &s, char delim) {
	std::vector<int> elems;
	std::stringstream ss(s);
	std::string item;

	while (std::getline(ss, item, delim)) {
		elems.push_back(std::stoi(item));
	}

	return elems;
}

std::string Utils::getStrValue(const std::string &data, const std::string &delim1, const std::string &delim2) {
	int pos1 = data.find(delim1);
	int pos2;
	int offset;

	if (pos1 != std::string::npos) {
		offset = delim1.length();
		pos2 = data.find(delim2, pos1 + offset);
		if (pos2 != std::string::npos) {
			return data.substr(pos1 + offset, pos2 - pos1 - offset);
		}
	}
	return "";
}

char *getSystemProxy() {
	return "";
}

int Utils::getProxyPort() {
	return 0;
}

char * Utils::getProxy() {
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#else
    getSystemProxy();
#endif
	return "";
}