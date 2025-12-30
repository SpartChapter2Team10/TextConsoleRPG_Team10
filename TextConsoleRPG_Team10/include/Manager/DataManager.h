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
    string _SaveDataPath;
    bool _IsInitialized;

public:
    void Initialize();
    string LoadTextFile(string fileName);
    vector<vector<string>> LoadCSVFile(string fileName);
    bool SaveTextFile(string fileName, string data);
    bool SavePlayerData(Player* p);
    bool LoadPlayerData(Player* p);
    bool FileExists(string fileName);
    bool DeleteFile(string fileName);
};
