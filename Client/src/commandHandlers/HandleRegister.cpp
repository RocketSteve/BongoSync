#include "../../include/commandHandlers/HandleRegister.h"

//TODO implement message interpretation


void HandleRegister::initiateRegistration() {

    if (Utility::configExists()) {
        std::cout << "This computer is already registered, please start the application with:\n bongo start\n";
        return;
    }

    std::string email = Utility::promptForEmail();
    std::string password;
    std::string hostname = Utility::getHostname();

    do {
        password = Utility::promptForPassword();
    } while (!Utility::confirmPassword(password));

    std::string serverIP;
    int serverPort;

    std::cout << "Enter server IP: ";
    while (true) {
        std::cin >> serverIP;
        if (Utility::isValidIPAddress(serverIP)) {
            break;
        } else {
            std::cout << "Invalid IP address. Please enter a valid IPv4 address: ";
        }
    }

    std::cout << "Enter server port: ";
    while (true) {
        if (std::cin >> serverPort && Utility::isValidPort(serverPort)) {
            break;
        } else {
            std::cout << "Invalid port. Please enter a valid port number (1-65535): ";
            std::cin.clear(); // Clear the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard the input
        }
    }

    if (!checkWithServer(email, password, hostname, serverIP, serverPort)) {
        std::cout << "Registration failed\n";
        initiateRegistration();
    } else {
        createConfigFile(email, hostname, serverIP, serverPort);

        MerkleTree merkleTree;
        merkleTree.buildTree(Utility::getPathFromConfig());
        std::string workspaceHash = merkleTree.getTreeHash();

        std::cout << "Workspace hash: " << workspaceHash << std::endl;

        Utility::updateWorkspaceHash(workspaceHash);
        std::cout << "Read workspaceHash" << Utility::readWorkspaceHash() << std::endl;

        try {
            std::filesystem::path currentDir = std::filesystem::current_path();
            std::filesystem::path newDir = currentDir / hostname;

            if (!std::filesystem::exists(newDir)) {
                std::filesystem::create_directory(newDir);
                std::cout << "Directory created: " << newDir << std::endl;
            } else {
                std::cout << "Directory already exists: " << newDir << std::endl;
            }
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Error creating directory: " << e.what() << std::endl;
        }

        std::cout << "User registered successfully.\n";
    }
}

bool HandleRegister::checkWithServer(const std::string& email, const std::string& password, const std::string& hostname, const std::string& serverIP, int serverPort) {
    std::cout << "Checking with server ...\n" << email << "\n" << password << "\n" << hostname << "\n";

    auto& communicator = ServerCommunicator::getInstance();
    if (!communicator.isConnectedToServer()) {
        if (!communicator.connectToServer(serverIP, serverPort)) {
            std::cerr << "Failed to connect to server\n";
            return false;
        }
        std::cout << "Connected to server\n";
    }

    std::string hashedPassword = Utility::hashPassword(password);
    std::cout << "Hashed password: " << hashedPassword << std::endl;

    std::filesystem::path currentPath = std::filesystem::current_path();
    std::filesystem::path directoryPath = currentPath / hostname;

    if (!std::filesystem::exists(directoryPath)) {
        std::filesystem::create_directories(directoryPath);
        std::cout << "Directory created: " << directoryPath << std::endl;
    }

    MerkleTree merkleTree;
    merkleTree.buildTree(directoryPath);
    std::cout << "Tree built" << std::endl;
    std::string workspaceHash = merkleTree.getTreeHash();

    std::cout << "Workspace hash: " << workspaceHash << std::endl;

    MessageBuilder messageCreator;
    std::string regMessage = MessageCreator::create()
            .setEmail(email)
            .setHostname(hostname)
            .setPassword(hashedPassword)
            .setTreeHash(workspaceHash)
            .buildRegistrationMessage();

    if (communicator.sendMessage(regMessage)) {
        std::cout << "Message sent" << std::endl;
        std::string response = communicator.receiveMessage();

        nlohmann::json jsonResponse = nlohmann::json::parse(response);
        std::string responseType = jsonResponse["response"];

        if (responseType == "user_exists") {
            std::cout << "User already exists" << std::endl;
            return false;
        } else if (responseType == "user_added") {
            std::cout << "User does not exist. Registration successful" << std::endl;
            return true;
        } else {
            std::cerr << "Unexpected server response" << std::endl;
            return false;
        }
    } else {
        std::cerr << "Failed to send registration message" << std::endl;
        return false;
    }
}


void HandleRegister::createConfigFile(const std::string& email, const std::string& hostname, const std::string& serverIP, int serverPort) {
    std::cout << "Creating config file ...\n";
    const std::string filesDir = std::filesystem::current_path() / hostname;
    const std::string configDir = std::filesystem::path(getenv("HOME")) / ".bongo";
    const std::string configFile = configDir + "/config.json";

    // Create the hidden directory if it does not exist
    std::filesystem::create_directories(configDir);

    // Construct the JSON object
    nlohmann::json configJson = {
            {"Email", email},
            {"Hostname", hostname},
            {"Path", filesDir},
            {"ServerIP", serverIP},
            {"ServerPort", serverPort},
            {"WorkspaceHash", ""},
            {"modified_at", ""}
    };

    // Serialize the JSON object to a string
    std::string serializedJson = configJson.dump();

    // Write the serialized JSON string to the file
    std::ofstream outFile(configFile);
    if (outFile) {
        outFile << serializedJson;
        std::cout << "Configuration file created at: " << configFile << std::endl;
    } else {
        std::cerr << "Failed to create configuration file at: " << configFile << std::endl;
    }
}
