#include "snake_common.h"
#include "saveData.h"
#include "embSnakeDevice.h"

extern u8 buff[128];

SaveData_Struct gSetting = {0};

void SD_loadSetting(void)
{
    LOG("LOAD DATA Start\r\n");
    devLoadSetting(&gSetting);
    LOG("LOAD DATA done.\r\n");
}

void SD_saveSetting(void)
{
    devSaveSetting(&gSetting);
}
