#ifndef IRC_NMBLINKER_H
#define IRC_NMBLINKER_H

#include "nesca_3.h"

class IRC_NMBlinker : public QThread
{
	Q_OBJECT

public:
	static void doEmitBlinkMessage();

public: signals: void sBlinkMessage();

protected:
	void run();
};
extern IRC_NMBlinker *irc_nmb;

#endif // IRC_NMBLINKER_H
