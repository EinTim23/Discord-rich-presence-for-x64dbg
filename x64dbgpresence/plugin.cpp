#include "plugin.h"
#include <iostream>
#include <chrono>
#include "DiscordSDK/include/discord_rpc.h"
#include <fstream>
#include "icon.h"
std::ifstream inFile;
std::ofstream outfile;
std::ofstream outfile2;
std::ofstream outfile3;
std::string richpresence;
std::string file;
void Initialize() {
    DiscordEventHandlers Handler;
    memset(&Handler, 0, sizeof(&Handler));
    Discord_Initialize("758373090253209600", &Handler, 1, NULL);
}
void Update() {
    int64_t dctime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    std::string file1 = file.substr(file.find_last_of("\\"), file.find_last_of("\\"));
    std::string fileend = file1.replace(0, 1, "");
    std::string status = "Debugging " + fileend;
    DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));
    discordPresence.state = status.c_str();
    discordPresence.startTimestamp = dctime;
    discordPresence.largeImageText = "Made by EinTim#0777";
    discordPresence.largeImageKey = "x64dbg-lanczos3";
    Discord_UpdatePresence(&discordPresence);
}
void UpdateIdle() {
    DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));
    discordPresence.state = "No File, Idling";
    discordPresence.largeImageKey = "x64dbg-lanczos3";
    discordPresence.largeImageText = "Made by EinTim#0777";
    Discord_UpdatePresence(&discordPresence);
}
enum
{
    MENU_CREDITS,
    MENU_ENABLE,
    MENU_DISABLE
};

static duint exprZero(int argc, duint* argv, void* userdata)
{
    return 0;
}

PLUG_EXPORT void CBINITDEBUG(CBTYPE cbType, PLUG_CB_INITDEBUG* info)
{
    file = info->szFileName;
    if (richpresence == "true") {
        Update();
    }
    
}

PLUG_EXPORT void CBSTOPDEBUG(CBTYPE cbType, PLUG_CB_STOPDEBUG* info)
{
    if (richpresence == "true") {
        UpdateIdle();
    }
   
}

PLUG_EXPORT void CBEXCEPTION(CBTYPE cbType, PLUG_CB_EXCEPTION* info)
{
    
}

PLUG_EXPORT void CBDEBUGEVENT(CBTYPE cbType, PLUG_CB_DEBUGEVENT* info)
{
    if(info->DebugEvent->dwDebugEventCode == EXCEPTION_DEBUG_EVENT)
    {
        dprintf("DebugEvent->EXCEPTION_DEBUG_EVENT->%.8X\n", info->DebugEvent->u.Exception.ExceptionRecord.ExceptionCode);
    }
}

PLUG_EXPORT void CBMENUENTRY(CBTYPE cbType, PLUG_CB_MENUENTRY* info)
{
    switch (info->hEntry)
    {
    case MENU_CREDITS:
        MessageBoxA(hwndDlg, "Discord Rich presence. Made by EinTim#0777", PLUGIN_NAME, MB_ICONINFORMATION);
        break;
    case MENU_ENABLE:
        MessageBoxA(hwndDlg, "Enabled discord rich presence", PLUGIN_NAME, MB_ICONINFORMATION);
        remove("plugins\\rpc.ini");
        richpresence = "true";
        outfile2.open("plugins\\rpc.ini");
        outfile2 << "true";
        outfile2.close();
        UpdateIdle();
        break;
    case MENU_DISABLE:
        MessageBoxA(hwndDlg, "Disabled discord rich presence", PLUGIN_NAME, MB_ICONINFORMATION);
        remove("plugins\\rpc.ini");
        richpresence = "false";
        outfile3.open("plugins\\rpc.ini");
        outfile3 << "false";
        outfile3.close();
        Discord_ClearPresence();
        break;
    }
}


bool pluginInit(PLUG_INITSTRUCT* initStruct)
{
    inFile.open("rpc.ini");
    if (inFile.fail()) {
        richpresence = "true";
        outfile.open("rpc.ini");
        outfile << "true";
        outfile.close();
    }
    inFile >> richpresence;
    inFile.close();
    Initialize();
    if (richpresence == "true") {
        UpdateIdle();
    }
    _plugin_registerexprfunction(pluginHandle, PLUGIN_NAME ".zero", 0, exprZero, nullptr);
    return true;
}

void pluginStop()
{
    if (richpresence == "true") {
        Discord_ClearPresence();
        Discord_Shutdown();
    }
    
}


void pluginSetup()
{
    ICONDATA menu_icon;
    menu_icon.data = icon;
    menu_icon.size = sizeof(icon);
    _plugin_menuseticon(hMenu, &menu_icon);
    _plugin_menuaddentry(hMenu, MENU_ENABLE, "&enable discord rich presence");
    _plugin_menuaddentry(hMenu, MENU_DISABLE, "&disable discord rich presence");
    _plugin_menuaddentry(hMenu, MENU_CREDITS, "&credits");
}
