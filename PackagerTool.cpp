#include <iostream>
#include <fstream>
#include <json/json.h>
#include <windows.h>
#include <string>
#include <iostream>
#include <cstdlib>  // pour system()

// Fonction utilitaire pour convertir std::string en std::wstring
std::wstring stringToWString(const std::string& str) {
    return std::wstring(str.begin(), str.end());
}
void showInfos(const std::string& uprojectPath) {
    std::ifstream file(uprojectPath);
    if (!file.is_open()) {
        std::cerr << "Impossible d'ouvrir le fichier UPROJECT." << std::endl;
        return;
    }

    Json::Value root;
    file >> root;

    // Récupérer le champ "Name" dans "Modules"
    for (const auto& module : root["Modules"]) {
        std::cout << "Module Name: " << module["Name"].asString() << std::endl;
    }
    std::cout << "Version Unreal Engine : " << root["EngineAssociation"].asString() << std::endl;
    if (root["EngineAssociation"].isNull()) {
        std::cout << "Version UE From Source." << std::endl;
    }

    std::cout << "Plugins utilisés : " << std::endl;
    for (const auto& plugin : root["Plugins"]) {
        std::cout << "- " << plugin["Name"].asString() << std::endl;
    }
}

void buildProject(const std::string& uprojectPath) {
    std::ifstream file(uprojectPath);
    if (!file.is_open()) {
        std::cerr << "Impossible d'ouvrir le fichier UPROJECT." << std::endl;
        return;
    }
    Json::Value root;
    file >> root;
    std::string name;
    // Récupérer le champ "Name" dans "Modules"
    for (const auto& module : root["Modules"]) {
        name = module["Name"].asString();
    }
    std::string commande = ".\\Engine\\Build\\BatchFiles\\Build.bat " + name + " win64 Development " + "..\\..\\"+uprojectPath + " -waitmutex";
    system (commande.c_str());
}

void PackageProject(const std::string& uprojectPath) {
    std::ifstream file(uprojectPath);
    if (!file.is_open()) {
        std::cerr << "Impossible d'ouvrir le fichier UPROJECT." << std::endl;
        return;
    }

    Json::Value root;
    file >> root;
    std::string name;
    // Récupérer le champ "Name" dans "Modules"
    for (const auto& module : root["Modules"]) {
        std::string name = module["Name"].asString();
    }
    std::string commande = ".\\Engine\\Build\\BatchFiles\\RunUAT.bat -ScriptsForProject=" + uprojectPath+ "BuildCookRun -project=" + uprojectPath + "-noP4 -clientconfig=Development -serverconfig=Development -nocompile -nocompileeditor -installed -unrealexe=.\\Engine\\Binaries\\Win64\\UnrealEditor-Cmd.exe -utf8output -platform=Win64 -build -cook -CookCultures=fr -unversionedcookedcontent -stage -package -cmdline= -Messaging -addcmdline=-SessionId=41EACCF047A5A712277F48A0331DBD7C -SessionOwner=conja -SessionName=profileTool" ;
    system (commande.c_str());
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage : MyTool [UPROJECT_PATH] [show-infos|build|package [OUTPUT_PATH]]" << std::endl;
        return 1;
    }

    std::string command = argv[2];
    std::string uprojectPath = argv[1];

    if (command == "show-infos") {
        showInfos(uprojectPath);
    } else if (command == "build") {
        buildProject(uprojectPath);
    }else if (command == "package")
    {
        PackageProject(uprojectPath);
    }
    else {
        std::cerr << "Commande invalide." << std::endl;
        return 1;
    }

    return 0;
}

