#include <iostream>
#include <fstream>
#include <windows.h>
#include "json.hpp"
using json = nlohmann::json;


void showInfos(const std::string& uproject_path) {
    // Get the .uproject file
    std::ifstream file(uproject_path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << uproject_path << std::endl;
        return;
    }
    json data = json::parse(file);

    if (data.find("Modules") == data.end()) {
        std::cerr << "No 'Modules' key found in the .uproject file, can't get the name of the project" << std::endl;
        return;
    }

    std::string projectName = data["Modules"][0]["Name"];
    std::string projectVersion = data["EngineAssociation"];

    if (projectVersion.length() > 3) {
        projectVersion = "Build from source";
    }

     std::cout << "Project Name: " << projectName << std::endl;
     std::cout << "Project Version: " << projectVersion << std::endl;

    if (data.find("Plugins") == data.end()) {
        std::cout << "No 'Plugins' key found in the .uproject file, there are no plugins" << std::endl;
        return;
    }

    std::cout << "Plugins installed: " << std::endl;

    json plugins = data["Plugins"];
    for (json::iterator it = plugins.begin(); it != plugins.end(); ++it) {
        std::string pluginName = it.value()["Name"];
        std::cout << pluginName << std::endl;
    }
}

void build(const std::string& uproject_path) {
    // Get the projet name
    std::ifstream file(uproject_path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << uproject_path << std::endl;
        return;
    }

    json data = json::parse(file);

    if (data.find("Modules") == data.end()) {
        std::cerr << "No 'Modules' key found in the .uproject file, can't get the name of the project" << std::endl;
        return;
    }

    std::string projectName = data["Modules"][0]["Name"];

    std::string command = "cd Engine\\Build\\BatchFiles && Build.bat " + projectName + "Editor Win64 Development " + uproject_path + " -waitmutex"; // TODO : changer ca c'est dégeulasse
    std::cout << command << std::endl;
    system (command.c_str());

    std::cout << "Done ! "<< std::endl;

}

void package(const std::string & uproject_path, const std::string & package_location) {
    std::string command = "cd Engine\\Build\\BatchFiles && RunUAT.bat  -ScriptsForProject=" + uproject_path + " BuildCookRun -project=" + uproject_path + " -noP4 -clientconfig=Development -serverconfig=Development -nocompileeditor -unrealexe=./Engine/Binaries/Win64/UnrealEditor-Cmd.exe -utf8output -platform=Win64 -build -cook -CookCultures=fr -unversionedcookedcontent -stage -package -cmdline=\" -Messaging\" -addcmdline=\"-SessionId=59A0E16E4712A7ABD8B72AA191F94592 -SessionOwner='douau' -SessionName='Test_Tom' \" -archivedirectory=" + package_location;
    system(command.c_str());
    std::cout << "Done ! "<< std::endl;
};


int main(int argc, char* argv[]) {

    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <uproject_path> <command_type>" << std::endl;
        return 1;
    }

    std::string uproject_path = argv[1];
    std::string command_type = argv[2];

    std::string package_location = "";

    if (argc > 3) {
        std::string package_location = argv[4];
    }

    std::cout << "UProject Path: " << uproject_path << std::endl;
    std::cout << "Command Type: " << command_type << std::endl;


    if (command_type == "show-infos") {
        showInfos(uproject_path);
    } else if (command_type == "build") {
        build(uproject_path);
    } else if (command_type == "package") {
        package(uproject_path, package_location);
    } else {
        std::cerr << "Unknown command type: " << command_type << std::endl;
        return 1;
    }

    return 0;
}

