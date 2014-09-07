#include "irc_nmblinker.h"
#include "nesca_3.h"

void IRC_NMBlinker::doEmitBlinkMessage()
{
	emit irc_nmb->sBlinkMessage();
};

void IRC_NMBlinker::run()
{
	while(widgetIsHidden == false && disableBlink == false && tray->isVisible() == false)
	{
		irc_nmb->doEmitBlinkMessage();
		msleep(500);
	};
};
