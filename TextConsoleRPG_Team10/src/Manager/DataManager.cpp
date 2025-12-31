#include "../../include/Manager/DataManager.h"
#include "../../include/Config.h"

using namespace std;

bool DataManager::Initialize()
{
    // Implementation needed
    // TODO: Save & Load 경로를 저장함
    bool IsInitialized = false;

    string BasePath = DEFAULT_RESOURCE_PATH;



    return IsInitialized;
}

string DataManager::LoadTextFile(string FileName)
{
    // Implementation needed
    return "";
}

vector<vector<string>> DataManager::LoadCSVFile(string FileName)
{
    // Implementation needed
    return vector<vector<string>>();
}

bool DataManager::SaveTextFile(string FileName, string Data)
{
    // Implementation needed
    return false;
}

bool DataManager::SavePlayerData(Player* p)
{
    // Implementation needed
    return false;
}

bool DataManager::LoadPlayerData(Player* p)
{
    // Implementation needed
    return false;
}

bool DataManager::FileExists(string FileName)
{
    // Implementation needed
    return false;
}

bool DataManager::DeleteFile(string FileName)
{
    // Implementation needed
    return false;
}

bool DataManager::DirectoryExists(const string& DirPath)
{
    return false;
}