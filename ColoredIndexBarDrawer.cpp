#include "ColoredIndexBarDrawer.h"

int cIndex = 0;

void ColoredIndexBarDrawer::doEmitionDrawPointerPB2(int pointer)
{
	emit pbTh2->drawPointerPB2(pointer);
};

void ColoredIndexBarDrawer::run()
{
	globalScanFlag = true;
	int oldPointer = 0;
	msleep(1000);
	while (globalScanFlag)
	{
		msleep(200);
		int sz = stt->getColoredIndexes().size();
		if (sz > 0) {
			int pointer = 100 * cIndex / sz;

			if (pointer < oldPointer) {
				oldPointer = 0;
			}
			if (pointer > oldPointer) {
				doEmitionDrawPointerPB2(pointer);
				oldPointer = pointer;
			}
		}
		msleep(1000);
	}
};