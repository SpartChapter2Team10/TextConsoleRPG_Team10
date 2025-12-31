#pragma once
#include "../Singleton.h"
#include <string>
#include <vector>

using namespace std;

class Player;

class DataManager : public Singleton<DataManager>
{
private:
    string _ResourcePath;
    bool _IsInitialized;

private:
    DataManager() = default;
    friend class Singleton<DataManager>;

    DataManager(const DataManager&) = delete;
    DataManager& operator=(const DataManager&) = delete;

    string GetAsciiPath() const { return string(DEFAULT_RESOURCE_PATH) + "/" + ASCII_FOLDER; }
    string GetCSVPath()   const { return string(DEFAULT_RESOURCE_PATH) + "/" + CSV_FOLDER; }
    string GetDataPath()  const { return string(DEFAULT_RESOURCE_PATH) + "/" + PLAYER_DATA_FOLDER; }
    string GetTextPath()  const { return string(DEFAULT_RESOURCE_PATH) + "/" + TEXT_FOLDER; }

public:
    bool Initialize();
    string LoadTextFile(string fileName);
    vector<vector<string>> LoadCSVFile(string fileName);
    bool SaveTextFile(string fileName, string data);
    bool SavePlayerData(Player* p);
    bool LoadPlayerData(Player* p);
    bool FileExists(string fileName);
    bool DeleteFile(string fileName);
};
