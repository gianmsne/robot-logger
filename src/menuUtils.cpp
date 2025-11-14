#include "menuUtils.h"
#include "inputValidation.h"
#include "dbUtils.h"

void printStartText() {
    std::cout << "                           >> Scan or enter your student ID: ";
   
}

void printStartTextScan() {
    std::cout << "                                    >> Scan Your User ID Card" << std::endl;
}



void printMainMenu(const std::string& id, bool isAdmin) {
    std::cout << std::endl;
    std::cout << " ------------- User: " << id << " -------------" << std::endl;
    std::cout << " 1) Check Out" << std::endl;
    std::cout << " 2) Check In" << std::endl;
    std::cout << " 3) Add/view notes" << std::endl;
    std::cout << std::endl;
    if(isAdmin){
        std::cout << " >> Robot Options: " << std::endl;
        std::cout << " 4) Add Robot" << std::endl;
        std::cout << " 5) Modify Robot" << std::endl;
        std::cout << "\n >> User Options: "<< std::endl;
        std::cout << " 6) Add User" << std::endl;
        std::cout << " 7) Modify User" << std::endl;
        std::cout << std::endl;
        std::cout << " 0) Logout" << std::endl;
    } else {
        std::cout << " 0) Logout" << std::endl;
    }
    std::cout << std::endl;
    std::cout << " Enter Menu item to continue: ";
}


void printCheckOutMenu(const std::vector<std::string>& robots, std::string& pickedRobot) {
    std::cout << std::endl;
    std::cout << " ------------- CHECK-OUT -------------" << std::endl;
    std::string status;

    // Widths for columns
    int numWidth = std::to_string(robots.size()).length(); // number column
    int nameWidth = 12;  // robot name column width
    int statusWidth = 35; // status column width
    int noteWidth = 40;  // note column width

    // Print header row
    std::cout 
        << std::right << std::setw(numWidth) << "#" << " "
        << std::left  << std::setw(nameWidth) << "Robot"
        << "  > " << std::setw(statusWidth) << "Status"
        << " * " << std::setw(noteWidth) << "Most Recent Note"
        << std::endl;

        std::cout << std::endl;

    if(robots.empty()) {
        std::cout << "  >> There are no available robots to check out." <<  std::endl;
        pickedRobot = "";
        return;
    } else {
        for (unsigned int i = 0; i < robots.size(); i++) {
            std::string status = getRobotStatus(robots[i]);
            std::string recentNote = getMostRecentNote(robots[i]);

            if (status.empty()) status = " ";
            if (recentNote.empty()) recentNote = " ";

            std::cout 
                << std::right << std::setw(numWidth) << i + 1 << ") "
                << std::left  << std::setw(nameWidth) << robots[i] 
                << " | " << std::setw(statusWidth) << status 
                << " | " << std::setw(noteWidth) << recentNote
                << std::endl;
        }
    }

    std::cout << std::endl;
    std::cout << " Enter robot name or number ([0] to cancel): ";
   
    std::string input;
    std::cin >> input;
    input = stringToLower(input);

    while (true) {

        if (input == "0") {
            std::cout << " >> Check-out cancelled.\n";
            pickedRobot.clear();
            return;
        }

        if (is_number(input)) {
            int index = std::stoi(input);

            if (index >= 1 && index <= (int)robots.size()) {
                pickedRobot = robots[index - 1];
                return;
            }

            std::cout << " Invalid number. Choose 0-" << robots.size() << "." << std::endl;

        } else if (vector_contains(robots, input)) {
            pickedRobot = input;
            return;
        } else {
            std::cout << " Invalid input. Try again." << std::endl;
        }

        std::cout << " Enter robot name or number ([0] to cancel): ";
        std::cin >> input;
    }
    
}

void printCheckInMenu(const std::vector<std::string>& robots, std::string& pickedRobot, std::string &notes, std::string &permStatus) {
    std::cout << std::endl;
    std::cout << " ------------- CHECK-IN -------------" << std::endl;
    std::string user;

    if(robots.empty()) {
        std::cout << "  >> There are no robots to be checked-in." <<  std::endl;
        pickedRobot = "";
        return;
    } else {
        for (unsigned int i = 0; i < robots.size(); i++) {
            user = getUserFromID(getCheckOutIdFromRobot(robots[i]), user);
            std::cout << " " << i + 1 << ") " << robots[i]
                      <<  " | Checked out by: " << user << std::endl;
        }
    }

    std::cout << std::endl;
    std::cout << " Enter robot name or number ([0] to cancel): ";
   
    std::string input;
    std::cin >> input;
    input = stringToLower(input);

    while (true) {

        if (input == "0") {
            std::cout << " >> Check-in cancelled.\n";
            pickedRobot.clear();
            return;
        }

        if (is_number(input)) {
            int index = std::stoi(input);

            if (index >= 1 && index <= (int)robots.size()) {
                pickedRobot = robots[index - 1];
                break;
            }

            std::cout << "Invalid number. Choose 0-" << robots.size() << "." << std::endl;

        } else if (vector_contains(robots, input)) {
            pickedRobot = input;
            break;
        } else {
            std::cout << "Invalid input. Try again." << std::endl;
        }

        std::cout << "Enter robot number or name: ";
        std::cin >> input;
    }

    std::cout << std::endl;
    std::cout << " Enter any session notes about the robot's condition (ENTER to skip): ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, notes);

    std::cout << std::endl;
    std::string status = getRobotStatus(pickedRobot);
    if(status.empty()) { status = "None"; }

    std::cout << " >> Current Permanent Status: " << status << std::endl;
    std::cout << " Update permanent status? (ENTER to skip): ";
    std::getline(std::cin, permStatus);
    
} 

void printRobotType() {
    std::cout << " >> Choose Robot Type: " << std::endl;
    std::cout << "     1) Booster" << std::endl;
    std::cout << "     2) Nao" << std::endl;
    std::cout << " >> Select: ";
}

std::string getRobotType(int typeChoice) {
    switch(typeChoice){
        case 1:
            return "booster";
        case 2:
            return "nao";
        default:
            return "unknown";
    }
}

void printRobotCondition() {
    std::cout << " >> Choose Robot Condition: " << std::endl;
    std::cout << "     1) Functional" << std::endl;
    std::cout << "     2) Minor Issue" << std::endl;
    std::cout << "     3) Needs Checking" << std::endl;
    std::cout << "     4) Non-functional" << std::endl;
    std::cout << "     5) Retired" << std::endl;
    std::cout << " >> Select: ";
}

std::string getRobotCondition(int conditionChoice) {
    switch(conditionChoice){
        case 1:
            return "Functional";
        case 2:
            return "Minor Issue";
        case 3:
            return "Needs Checking";
        case 4:
            return "Non-functional";
        case 5:
            return "Retired";
        default:
            return "Unknown";
    }
}

void printModifyUserMenu() {
    std::cout << " >> Choose user detail to modify: " << std::endl;
    std::cout << "     1) Given Name" << std::endl;
    std::cout << "     2) Family Name" << std::endl;
    std::cout << "     3) Admin Status" << std::endl;
    std::cout << "     4) Induction Status" << std::endl;
    std::cout << "     5) Delete User" << std::endl;
    std::cout << "     6) Done" << std::endl;
    std::cout << " >> Select: ";
}

void printModifyRobotMenu() {
    std::cout << " >> Choose robot detail to modify: " << std::endl;
    std::cout << "     1) Type" << std::endl;
    std::cout << "     2) Condition" << std::endl;
    std::cout << "     3) ID" << std::endl;
    std::cout << "     4) Location" << std::endl;
    std::cout << "     5) Availability" << std::endl;
    std::cout << "     6) Delete Robot" << std::endl;
    std::cout << "     7) Done" << std::endl;
    std::cout << " >> Select: ";
}

void printLogin(){

    for(int i = 0; i < 30; i++) {
        std::cout << std::endl;
    }

    std::cout << R"(
                                                                                                    
                                                                                                    
                                            @@@@@@@@@@@@                                            
                                       @@@@%%%%%%%%%%%%%@@@@@@                                      
                                  @@@@@==##%%###########%%%#++@@@                                   
                              @@@@%%%%##*+===++********++====*#%%@@@@                               
                           @@@%%%################*****#############%%@@@                            
                       @@@@%%%%#######################################%%@@@@                        
                      @@%%%%%###############*************##############%%%%@@@                      
                   @@@%%%%%%#############*******************############%%%%%%@@                    
            @@@@@@@- :%%%%%%############********************#############%%%%%%@@@                  
         @@%         -%%%%%%#############*******************#############%%%%%%.  +@@@@             
        @.            @%%%%%%##############***************##############%%%%%%%        :@@@         
       @@             :@%%%%%%####################*###################%%%%%%%%=           @@@       
       @%              .@%%%%%%%%###################################%%%%%%%%%#              @@      
       @                 #%%%%%%%%%%#############@%@#############%%%%%%%%%%%@               @@      
       @                  .@%%%%%%%%%%%%%%######@%@%@######%%%%%%%%%%%%%%%%*                *@      
      @@                     %@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@*                  +@      
      @@                        :@@@%%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%                      -@      
      @@                                                                                    .@      
      @@    .......    ....... ..........     ... ..      .. .       .. .       .  .       . @      
      @@.....................======+=............................:-:..................... ...@      
      @*...................==========+:.......................-+=======.....................:@      
      @-..................=====@@@@====......................============...................=@      
      @...................===%@@@@@@*=+.....................:===@@@@@@===:..................#@      
      @...................===#@@@@@@+=+.....................===@@@@@@@*==-..................@@      
      @...................:+===@@@@==+=.....................-===@@@@@@===:..................@@      
      @-...................:=+=====+=:.......................============...................@@      
      @*......................:---:...........................:========:....................@@      
      @@:::::::::::::::::::::::::::::::::::::::::::::.::......:::::::.......................@       
      @@:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::-@       
      @@:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#@       
       @:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::@@       
       @@----------------------------------------@%-----::::----:::::::::::::::::::::::::::@@       
       @@--------------------------------------%%%@@+--------------------------------------@@       
        @*-------------------------------------=+++%---------------------------------------@        
        @@+=============================--------------------------------------------------@@        
          @@@@@@@@=======================================================================@@         
                  @@@@@@=========================================================@@@@@@@@           
                        @@@@#+++++++++++++++++++++++++++++++++++++++++++++*@@@@@                    
                            @@@++++++++++++++++++++++++++++++++++++++*@@@@                          
                               @@@******+++++*++++++++++++++++++++@@@@                              
                                  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                                                                  
                                                                                                    
                                                                                                    
)" << std::endl;

    // Print heading
    std::cout << "                             ------------- Robot Logger -------------\n" << std::endl;
}


void printNotesMenu(const std::vector<std::string>& robots, std::string& pickedRobot) {
    std::cout << std::endl;
    std::cout << " ------------- ADD/VIEW NOTES -------------" << std::endl;
    std::string status;

    if(robots.empty()) {
        std::cout << "  >> There are no robots in the database." <<  std::endl;
        return;
    } else {
        for (unsigned int i = 0; i < robots.size(); i++) {
            std::cout << " " << i + 1 << ") " << robots[i] << std::endl;
        }
    }

    std::cout << std::endl;
    std::cout << " Enter robot name or number ([0] to cancel): ";
   
    std::string input;
    std::cin >> input;
    input = stringToLower(input);

    while (true) {

        if (input == "0") {
            std::cout << " >> Returning to menu.\n";
            pickedRobot.clear();
            return;
        }

        if (is_number(input)) {
            int index = std::stoi(input);

            if (index >= 1 && index <= (int)robots.size()) {
                pickedRobot = robots[index - 1];
                return;
            }

            std::cout << " Invalid number. Choose 0-" << robots.size() << "." << std::endl;

        } else if (vector_contains(robots, input)) {
            pickedRobot = input;
            return;
        } else {
            std::cout << " Invalid input. Try again." << std::endl;
        }

        std::cout << " Enter robot name or number ([0] to cancel): ";
        std::cin >> input;
    }
    
}

void printNotes(const std::string &pickedRobot, const std::string &currUserID) {
    std::cout << std::endl;
    std::cout << " ------------- " << pickedRobot << " NOTES -------------" << std::endl;
    std::string status;
    std::vector<std::string> notes = getRobotNotes(pickedRobot);

    if(notes.empty()) {
        std::cout << "  >> There are no notes for this robot." <<  std::endl;
    } else {
        for (unsigned int i = 0; i < notes.size(); i++) {
        std::cout << " " << i + 1 << ") " << notes[i] << std::endl;
        }
    }

    std::cout << std::endl;
    std::cout << " >> Would you like to add a new note?";
   
    char input;
    input = getYesNo();

    while (true) {

        if (input == '\0' || input == 'N') {
            std::cout << " >> Returning to menu.\n";
            return;
        }

        if (input == 'Y') {
            std::string newNote;
            
            // Clear leftover newline from previous input, but only if needed
            if (std::cin.peek() == '\n') std::cin.get();

            std::cout << " Enter new note: ";
            std::getline(std::cin, newNote);

            if (!newNote.empty()) {
                addNote(pickedRobot, newNote, currUserID);
                std::cout << " >> Note added successfully.\n";
            } else {
                std::cout << " >> No note entered.\n";
            }

            return;
        }

    }
}