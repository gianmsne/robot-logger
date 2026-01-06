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
        std::cout << " >> Equipment Options: " << std::endl;
        std::cout << " 4) Add Equipment" << std::endl;
        std::cout << " 5) Modify Equipment" << std::endl;
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


void printCheckOutMenu(const std::vector<std::string>& equipments, std::string& pickedEquipment) {
    std::cout << std::endl;
    std::cout << " ------------- CHECK-OUT -------------" << std::endl;
    std::string status;

    // Widths for columns
    int numWidth = std::to_string(equipments.size()).length(); // number column
    int nameWidth = 12;  // equipment name column width
    int statusWidth = 35; // status column width
    int noteWidth = 40;  // note column width

    // Print header row
    std::cout 
        << std::right << std::setw(numWidth) << "#" << " "
        << std::left  << std::setw(nameWidth) << "Equipment"
        << "  > " << std::setw(statusWidth) << "Status"
        << " * " << std::setw(noteWidth) << "Most Recent Note"
        << std::endl;

        std::cout << std::endl;

    if(equipments.empty()) {
        std::cout << "  >> There are no available equipments to check out." <<  std::endl;
        pickedEquipment = "";
        return;
    } else {
        for (unsigned int i = 0; i < equipments.size(); i++) {
            std::string status = getEquipmentStatus(equipments[i]);
            std::string recentNote = getMostRecentNote(equipments[i]);

            if (status.empty()) status = " ";
            if (recentNote.empty()) recentNote = " ";

            std::cout 
                << std::right << std::setw(numWidth) << i + 1 << ") "
                << std::left  << std::setw(nameWidth) << equipments[i] 
                << " | " << std::setw(statusWidth) << status 
                << " | " << std::setw(noteWidth) << recentNote
                << std::endl;
        }
    }

    std::cout << std::endl;
    std::cout << " Enter equipment name or number ([0] to cancel): ";
   
    std::string input;
    std::cin >> input;
    input = stringToLower(input);

    while (true) {

        if (input == "0") {
            std::cout << " >> Check-out cancelled.\n";
            pickedEquipment.clear();
            return;
        }

        if (is_number(input)) {
            int index = std::stoi(input);

            if (index >= 1 && index <= (int)equipments.size()) {
                pickedEquipment = equipments[index - 1];
                return;
            }

            std::cout << " Invalid number. Choose 0-" << equipments.size() << "." << std::endl;

        } else if (vector_contains(equipments, input)) {
            pickedEquipment = input;
            return;
        } else {
            std::cout << " Invalid input. Try again." << std::endl;
        }

        std::cout << " Enter equipment name or number ([0] to cancel): ";
        std::cin >> input;
    }
    
}

void printCheckInMenu(const std::vector<std::string>& equipments, std::string& pickedEquipment, std::string &notes, std::string &permStatus) {
    std::cout << std::endl;
    std::cout << " ------------- CHECK-IN -------------" << std::endl;
    std::string user;

    if(equipments.empty()) {
        std::cout << "  >> There are no equipments to be checked-in." <<  std::endl;
        pickedEquipment = "";
        return;
    } else {
        for (unsigned int i = 0; i < equipments.size(); i++) {
            user = getUserFromID(getCheckOutIdFromEquipment(equipments[i]), user);
            std::cout << " " << i + 1 << ") " << equipments[i]
                      <<  " | Checked out by: " << user << std::endl;
        }
    }

    std::cout << std::endl;
    std::cout << " Enter equipment name or number ([0] to cancel): ";
   
    std::string input;
    std::cin >> input;
    input = stringToLower(input);

    while (true) {

        if (input == "0") {
            std::cout << " >> Check-in cancelled.\n";
            pickedEquipment.clear();
            return;
        }

        if (is_number(input)) {
            int index = std::stoi(input);

            if (index >= 1 && index <= (int)equipments.size()) {
                pickedEquipment = equipments[index - 1];
                break;
            }

            std::cout << "Invalid number. Choose 0-" << equipments.size() << "." << std::endl;

        } else if (vector_contains(equipments, input)) {
            pickedEquipment = input;
            break;
        } else {
            std::cout << "Invalid input. Try again." << std::endl;
        }

        std::cout << "Enter equipment number or name: ";
        std::cin >> input;
    }

    std::cout << std::endl;
    std::cout << " Enter any session notes about the equipment's condition (ENTER to skip): ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, notes);

    std::cout << std::endl;
    std::string status = getEquipmentStatus(pickedEquipment);
    if(status.empty()) { status = "None"; }

    std::cout << " >> Current Permanent Status: " << status << std::endl;
    std::cout << " Update permanent status? (ENTER to skip): ";
    std::getline(std::cin, permStatus);
    
} 

void printEquipmentType() {
    std::cout << " >> Choose Equipment Type: " << std::endl;
    std::cout << "     1) Booster" << std::endl;
    std::cout << "     2) Nao" << std::endl;
    std::cout << " >> Select: ";
}

std::string getEquipmentType(int typeChoice) {
    switch(typeChoice){
        case 1:
            return "booster";
        case 2:
            return "nao";
        default:
            return "unknown";
    }
}

void printEquipmentCondition() {
    std::cout << " >> Choose Equipment Condition: " << std::endl;
    std::cout << "     1) Functional" << std::endl;
    std::cout << "     2) Minor Issue" << std::endl;
    std::cout << "     3) Needs Checking" << std::endl;
    std::cout << "     4) Non-functional" << std::endl;
    std::cout << "     5) Retired" << std::endl;
    std::cout << " >> Select: ";
}

std::string getEquipmentCondition(int conditionChoice) {
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

void printModifyEquipmentMenu() {
    std::cout << " >> Choose equipment detail to modify: " << std::endl;
    std::cout << "     1) Type" << std::endl;
    std::cout << "     2) Condition" << std::endl;
    std::cout << "     3) ID" << std::endl;
    std::cout << "     4) Location" << std::endl;
    std::cout << "     5) Availability" << std::endl;
    std::cout << "     6) Delete Equipment" << std::endl;
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
    std::cout << "                             ------------- Equipment Logger -------------\n" << std::endl;
}


void printNotesMenu(const std::vector<std::string>& equipments, std::string& pickedEquipment) {
    std::cout << std::endl;
    std::cout << " ------------- ADD/VIEW NOTES -------------" << std::endl;
    std::string status;

    if(equipments.empty()) {
        std::cout << "  >> There are no equipments in the database." <<  std::endl;
        return;
    } else {
        for (unsigned int i = 0; i < equipments.size(); i++) {
            std::cout << " " << i + 1 << ") " << equipments[i] << std::endl;
        }
    }

    std::cout << std::endl;
    std::cout << " Enter equipment name or number ([0] to cancel): ";
   
    std::string input;
    std::cin >> input;
    input = stringToLower(input);

    while (true) {

        if (input == "0") {
            std::cout << " >> Returning to menu.\n";
            pickedEquipment.clear();
            return;
        }

        if (is_number(input)) {
            int index = std::stoi(input);

            if (index >= 1 && index <= (int)equipments.size()) {
                pickedEquipment = equipments[index - 1];
                return;
            }

            std::cout << " Invalid number. Choose 0-" << equipments.size() << "." << std::endl;

        } else if (vector_contains(equipments, input)) {
            pickedEquipment = input;
            return;
        } else {
            std::cout << " Invalid input. Try again." << std::endl;
        }

        std::cout << " Enter equipment name or number ([0] to cancel): ";
        std::cin >> input;
    }
    
}

void printNotes(const std::string &pickedEquipment, const std::string &currUserID) {
    std::cout << std::endl;
    std::cout << " ------------- " << pickedEquipment << " NOTES -------------" << std::endl;
    std::string status;
    std::vector<std::string> notes = getEquipmentNotes(pickedEquipment);

    if(notes.empty()) {
        std::cout << "  >> There are no notes for this equipment." <<  std::endl;
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
                addNote(pickedEquipment, newNote, currUserID);
                std::cout << " >> Note added successfully.\n";
            } else {
                std::cout << " >> No note entered.\n";
            }

            return;
        }

    }
}