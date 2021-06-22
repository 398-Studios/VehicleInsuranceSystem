#include "VehicleInsuranceSystem.h"
//Everything Is Fine Dont Worry About This
#pragma warning( disable : E0266)
ifstream inFile;

//Simple Little Function Which Carries The Whole Project
User SearchForUser(string SearchForEmail) {
    string UserFileString = "Files/" + SearchForEmail + ".json";
    ifstream UserFile(UserFileString);

    if (UserFile.fail()) {
        User InvaldUser = User("", "InvalidUser", "", false, false, true, "");
        return InvaldUser;
    }

    json CurrentUser;
    
    UserFile >> CurrentUser;

    string Email = SearchForEmail;
    string Username = CurrentUser["Username"];
    string Password = CurrentUser["Password"];
    bool IsAdmin = CurrentUser["IsAdmin"];
    bool RootUser = CurrentUser["RootUser"];
    bool Banned = CurrentUser["Banned"];
    json Policys = CurrentUser["Policys"];

    User ValidUser = User(Email, Username, Password, IsAdmin, RootUser, Banned, Policys);
    return ValidUser;
}
string DisplayBool(bool display) {
    if (display == true) {
        return "Y";
    }
    else {
        return "N";
    }
}
int main()
{
    //This Has To Be The Shortest Bloody main() That I Have Ever Done. Refer To The Header File For More Reasons Why I Fucking Hate This Shit
    VehicleInsuranceSystem::Login();
}
void VehicleInsuranceSystem::ConfirmPolicy(User Admin, bool AdminEdit, User CurrentUser, int PolicyNumber, string Plate, string Type, string Model, float Value, bool Modifications, float ValueOfMods, bool Financed, bool Business, int Kms, string Address, string FirstName, string LastName, string DOB, string Gender, int Incidents, int Drivers, int Length, float Cost, bool Status)
{
    string fileLocation = "Files/" + CurrentUser.Email + ".json";
    ofstream file;
    file.open(fileLocation, ofstream::out | ofstream::trunc);
    
    json policy;

    policy["User"] = CurrentUser.Email;
    policy["Registration"] = Plate;
    policy["Type"] = Type;
    policy["Model"] = Model;
    policy["Value"] = Value;
    policy["Modifications"] = Modifications;
    policy["ValueOfMods"] = ValueOfMods;
    policy["Financed"] = Financed;
    policy["Business"] = Business;
    policy["Kms"] = Kms;
    policy["Address"] = Address;
    policy["FirstName"] = FirstName;
    policy["LastName"] = LastName;
    policy["DOB"] = DOB;
    policy["Gender"] = Gender;
    policy["Incidents"] = Incidents;
    policy["Drivers"] = Drivers;
    policy["Length"] = Length;
    policy["Status"] = Status;

    for (int i = 0; i < CurrentUser.Policies.size(); i++) {
        if (CurrentUser.Policies[i]["Registration"] == Plate) {
            CurrentUser.Policies[i] = policy;
        }
    }

    if (PolicyNumber > CurrentUser.Policies.size()) {
        CurrentUser.Policies.push_back(policy);
    }
    else {
        for (int i = 0; i < CurrentUser.Policies.size(); i++) {
            if (i == PolicyNumber) {
                CurrentUser.Policies[i] = policy;
            }
            else {
                CurrentUser.Policies[i] = CurrentUser.Policies[i];
            }
        }
    }
    
    json User;
    User["Email"] = CurrentUser.Email;
    User["Username"] = CurrentUser.Username;
    User["Password"] = CurrentUser.Password;
    User["IsAdmin"] = CurrentUser.IsAdmin;
    User["RootUser"] = CurrentUser.RootUser;
    User["Banned"] = CurrentUser.Banned;
    User["Policys"] = CurrentUser.Policies;

    file << User;
    

    file.close();
   
    if (AdminEdit) {
        ViewAllPolicys(Admin);
    }
    else {
        ViewPolicys(CurrentUser);
    }
}
// It Just Works
void VehicleInsuranceSystem::PolicyDetails(User Admin, bool AdminEdit, bool AdminView, User CurrentUser, bool IsCanceled, bool IsNewQuote, int PolicyNumber, string Plate, string Type, string Model, float Value, bool Modifications, float ValueOfMods, bool Financed, bool Business, int Kms, string Address, string FirstName, string LastName, string DOB, string Gender, int Incidents, int Drivers, int Length, bool PolicyStatus)
{
    //Stupid Maths For A Stupid Thing
    float percentage = 15;
    float TotalValue = Value + ValueOfMods;
    if (Gender == "Male") {
        percentage = percentage + 5;
    }
    if (Incidents > 0) {
        for (int i = 0; i < Incidents; i++) {
            percentage = percentage + 5;
        }
    }
    int Cost = TotalValue * percentage / 100;
    Cost = Cost * 7 / 100 * Length;

    system("CLS");
    // Probably A Better Way To Do This But I Cant Be Assed
    string QuoteType;
    if (IsNewQuote) {
        QuoteType = "Estimated Quote";
    }
    else {
        QuoteType = "Quote";
    }
    cout << "======================================================================================================" << endl;
    cout << "                 " << QuoteType << " For " << FirstName << " " << LastName << " For " << Plate << endl;
    cout << "======================================================================================================\n" << endl;

    cout << "Estimated Quote For " << Length << " Months Of Insurance On Vehicle " << Plate << " Is $" << Cost << endl;
    if (PolicyStatus) {
        cout << "This Quote Is Currently Active" << endl;
    }
    else {
        cout << "This Quote Is Currently Canceled " << endl;
    }
    cout << "\n=================[ Quote Details ]=================" << endl;
    cout << "------------[ Vehicle Details ]------------" << endl;
    cout << "Vehicle Regerstration Number: " << Plate << endl;
    cout << "Vehicle Type: " << Type << endl;
    cout << "Value Of Vehicle: $" << Value << endl;
    cout << "Are There Any Modifications On The Vehicle: " << DisplayBool(Modifications) << endl;
    if (Modifications == true) {
        cout << "Value Of The Modifications On The Vehicle: $" << ValueOfMods << endl;
    }
    cout << "Is The Vehicle Financed: " << DisplayBool(Financed) << endl;
    cout << "Is This A Business Vehicle: " << DisplayBool(Business) << endl;
    cout << "How Many Kms Per Year Average Will The Vehicle Do: " << Kms << endl;
    cout << "Address That The Vehicle Will Be Primarily Stored At: " << Address << endl;
    cout << "\n------------[ Driver Details ]------------" << endl;
    cout << "First Name Of The Named Driver: " << FirstName << endl;
    cout << "Last Name Of The Named Drvier: " << LastName << endl;
    cout << "Named Drivers DOB: " << DOB << endl;
    cout << "Named Drivers Gender: " << Gender << endl;
    cout << "Named Drivers Amount Of Incidents Withing Last 3 Years: " << Incidents << endl;
    cout << "\n------------[ Quote Details ]------------" << endl;
    cout << "Amount Of Other Drivers That Will Drive The Vehicle: " << Drivers << endl;
    cout << "Amount Of Months That This Policy Will Cover The Vehicle: " << Length << "\n\n\n" << endl;

    string command;
    // To Cancel A Policy
    if (IsCanceled && PolicyStatus) {
        string StringValue;
        cout << "Would You Like To Cancel This Policy (Y | N): ";
        cancelPolicy:
        getline(cin, StringValue);
        if (!StringValue.empty()) {
            transform(StringValue.begin(), StringValue.end(), StringValue.begin(), ::tolower);
            if (StringValue == "y") {
                ConfirmPolicy(Admin, AdminEdit, CurrentUser, PolicyNumber, Plate, Type, Model, Value, Modifications, ValueOfMods, Financed, Business, Kms, Address, FirstName, LastName, DOB, Gender, Incidents, Drivers, Length, Cost, false);
            }
            if (StringValue == "n") {
                if (AdminEdit) {
                    ViewAllPolicys(Admin);
                }
                else {
                    ViewPolicys(CurrentUser);
                }
            }
            else {
                cout << "Please Enter A Valid Input (Y | N): ";
                goto cancelPolicy;
            }
        }
    }
    // To Renew A Canceled Policy Admin Only
    if (IsCanceled && !PolicyStatus && AdminEdit) {
        string StringValue;
        cout << "Would You Like To Renew This Policy (Y | N): ";
    renewPolicy:
        getline(cin, StringValue);
        if (!StringValue.empty()) {
            transform(StringValue.begin(), StringValue.end(), StringValue.begin(), ::tolower);
            if (StringValue == "y") {
                ConfirmPolicy(Admin, AdminEdit, CurrentUser, PolicyNumber, Plate, Type, Model, Value, Modifications, ValueOfMods, Financed, Business, Kms, Address, FirstName, LastName, DOB, Gender, Incidents, Drivers, Length, Cost, true);
            }
            if (StringValue == "n") {
                ViewAllPolicys(Admin);
            }
            else {
                cout << "Please Enter A Valid Input (Y | N): ";
                goto renewPolicy;
            }
        }
    }
    // To Renew A Canceled Policy
    if (IsCanceled && !PolicyStatus) {
        string StringValue;
        cout << "Would You Like To Renew This Policy (Y | N): ";
    renewPolicy2:
        getline(cin, StringValue);
        if (!StringValue.empty()) {
            transform(StringValue.begin(), StringValue.end(), StringValue.begin(), ::tolower);
            if (StringValue == "y") {
                ConfirmPolicy(Admin, AdminEdit, CurrentUser, PolicyNumber, Plate, Type, Model, Value, Modifications, ValueOfMods, Financed, Business, Kms, Address, FirstName, LastName, DOB, Gender, Incidents, Drivers, Length, Cost, true);
            }
            if (StringValue == "n") {
                if (AdminEdit) {
                    ViewAllPolicys(Admin);
                }
                else {
                    ViewPolicys(CurrentUser);
                }
            }
            else {
                cout << "Please Enter A Valid Input (Y | N): ";
                goto renewPolicy2;
            }
        }
    }
    if (AdminView) {
        cout << "Would You Like To Edit This Policy: Edit (0) | Back (1) | Quit (2)" << endl;
    }
    else if (AdminEdit) {
        cout << "Would You Like To Confirm This Policy Or Would You Like To Edit Some Details: Confirm (0) | Edit (1) | Back (2) | Quit (3)" << endl;
    }
    else {
        cout << "Would You Like To Confirm This Policy Or Would You Like To Edit Some Details: Confirm (0) | Edit (1) | Menu (2) | Quit (3)" << endl;
    }
policyconfirm:
    cout << ">";
    getline(cin, command);
    // Love Rewriting This Part
    if (AdminView) {
        transform(command.begin(), command.end(), command.begin(), ::tolower);
        if (command == "edit" || command == "back" || command == "quit" || command == "0" || command == "1" || command == "2") {            
            if (command == "edit" || command == "0") {
                if (AdminView) {
                    PolicyNumber = PolicyNumber;
                }
                else {
                    PolicyNumber = CurrentUser.Policies.size() + 1;
                }
                EditPolicy(Admin, AdminEdit, CurrentUser, IsNewQuote, PolicyNumber, Plate, Type, Model, Value, Modifications, ValueOfMods, Financed, Business, Kms, Address, FirstName, LastName, DOB, Gender, Incidents, Drivers, Length);
            }
            if (command == "back" || command == "1") {
                ViewAllPolicys(Admin);
            }
            if (command == "quit" || command == "2") {
                exit(0);
            }
        }
        else {
            cout << "Please Enter A Valid Command. Current Avalible Commands Are: Confirm | Edit | Menu | Quit" << endl;
            goto policyconfirm;
        }
    }
    // Same Thing Just Different
    else if (AdminEdit) {
        transform(command.begin(), command.end(), command.begin(), ::tolower);
        if (command == "confirm" || command == "edit" || command == "back" || command == "quit" || command == "0" || command == "1" || command == "2" || command == "3") {
            if (command == "confirm" || command == "0") {
                if (AdminEdit) {
                    PolicyNumber = PolicyNumber;
                }
                else {
                    PolicyNumber = CurrentUser.Policies.size() + 1;
                }
                ConfirmPolicy(Admin, AdminEdit, CurrentUser, PolicyNumber, Plate, Type, Model, Value, Modifications, ValueOfMods, Financed, Business, Kms, Address, FirstName, LastName, DOB, Gender, Incidents, Drivers, Length, Cost, true);
            }
            if (command == "edit" || command == "1") {
                if (AdminEdit) {
                    PolicyNumber = PolicyNumber;
                }
                else {
                    PolicyNumber = CurrentUser.Policies.size() + 1;
                }
                EditPolicy(Admin, AdminEdit, CurrentUser, IsNewQuote, PolicyNumber, Plate, Type, Model, Value, Modifications, ValueOfMods, Financed, Business, Kms, Address, FirstName, LastName, DOB, Gender, Incidents, Drivers, Length);
            }
            if (command == "back" || command == "2") {
                ViewAllPolicys(Admin);
            }
            if (command == "quit" || command == "3") {
                exit(0);
            }
        }
    }
    // I Just Fucking Hate This Shit Now
    else
    {
        transform(command.begin(), command.end(), command.begin(), ::tolower);
        if (command == "confirm" || command == "edit" || command == "menu" || command == "quit" || command == "0" || command == "1" || command == "2") {
            if (command == "confirm" || command == "0") {
                if (AdminEdit) {
                    PolicyNumber = PolicyNumber;
                }
                else {
                    PolicyNumber = CurrentUser.Policies.size() + 1;
                }
                ConfirmPolicy(Admin, AdminEdit, CurrentUser, PolicyNumber, Plate, Type, Model, Value, Modifications, ValueOfMods, Financed, Business, Kms, Address, FirstName, LastName, DOB, Gender, Incidents, Drivers, Length, Cost, true);
            }
            if (command == "edit" || command == "1") {
                if (AdminEdit) {
                    PolicyNumber = PolicyNumber;
                }
                else {
                    PolicyNumber = CurrentUser.Policies.size() + 1;
                }
                EditPolicy(Admin, AdminEdit, CurrentUser, IsNewQuote, PolicyNumber, Plate, Type, Model, Value, Modifications, ValueOfMods, Financed, Business, Kms, Address, FirstName, LastName, DOB, Gender, Incidents, Drivers, Length);
            }
            if (command == "menu" || command == "2") {
                if (AdminEdit) {
                    Menu(Admin, Admin.RootUser);
                }
                else {
                    Menu(CurrentUser, CurrentUser.RootUser);
                }
            }
            if (command == "quit" || command == "3") {
                exit(0);
            }
        }
        else {
            cout << "Please Enter A Valid Command. Current Avalible Commands Are: Confirm | Edit | Menu | Quit" << endl;
            goto policyconfirm;
        }
    }
}
// Oh Why Did I Put Myself Through This Hell
void VehicleInsuranceSystem::EditPolicy(User Admin, bool AdminEdit, User CurrentUser,bool IsNewQuote, int PolicyNumber, string Plate, string Type, string Model, float Value, bool Modifications, float ValueOfMods, bool Financed, bool Business, int Kms, string Address, string FirstName, string LastName, string DOB, string Gender, int Incidents, int Drivers, int Length)
{
    // Policy Inputs Round 2. I Just Want To Fucking Die Now
    system("CLS");
    SetConsoleTitle(TEXT("398 Studios Insurance | Edit Policy"));
    cout << "======================================================================================================" << endl;
    cout << "                  Editing Quote For " << FirstName << " " << LastName << " For " << Plate << ". Leave Blank To Skip Input" << endl;
    cout << "======================================================================================================\n" << endl;

    string StringValue; // I Just Love How This Value Carries This Whole Piece Of Code
    cout << "Edit The Vehicle Regerstration Number (" << Plate << "): ";
    getline(cin, StringValue);
    if (!StringValue.empty()) {
        Plate = StringValue;
    }

    cout << "Edit The Vehicle Type (" << Type << "): ";
    getline(cin, StringValue);
    if (!StringValue.empty()) {
        Type = StringValue;
    }

    cout << "Edit The Vehicle Model (" << Model << "): ";
    getline(cin, StringValue);
    if (!StringValue.empty()) {
        Model = StringValue;
    }

    float TempValue;
aa:
    cout << "Edit The Value Of The Vehicle ($" << Value << "): ";
    getline(cin, StringValue);
    if (!StringValue.empty()) {
        TempValue = stof(StringValue);
        if (TempValue <= 100) {
            cout << "The Value Needs To Be Greater Than $100";
            goto aa;
        }
        Value = TempValue;
    }

    // There Is Probably A Better Way To Do This Code But Im Lazy And This Works So I Aint Gonna Fuck With It
    float TempValueOfMods = 0;
    string ModsBool;
    cout << "Does Your Vehicle Have Any Modifications (" << DisplayBool(Modifications) << ") (Y | N): ";
bb:
    getline(cin, ModsBool);
    if (!ModsBool.empty()) {
        transform(ModsBool.begin(), ModsBool.end(), ModsBool.begin(), ::tolower);
        if (ModsBool == "y") {
            Modifications = true;
            cout << "Edit The Value Of The Modifications On The Vehicle ($" << ValueOfMods << ") (MAX 6 Chars): $";
        cc:
            getline(cin, StringValue);
            if (!StringValue.empty()) {
                if (StringValue.length() <= 6) {
                    for (int i = 0; i < StringValue.length(); i++) {
                        if (!isdigit(StringValue[i])) {
                            cout << "Please Enter A Valid Number: $";
                            goto cc;
                        }
                    }
                    ValueOfMods = stof(StringValue);
                }
                if (StringValue.length() >= 7) {
                    cout << "You Cannot Input More Than 6 Chars : ";
                    goto cc;
                }
            }
        }
        else if (ModsBool == "n") {
            Modifications = false;
        }
        else {
            cout << "Please Enter A Valid Input (Y | N): ";
            goto bb;
        }
    }
    else {
        if (Modifications == true) {
        dd:
            cout << "Edit The Value Of The Modifications On The Vehicle ($" << ValueOfMods << ") (MAX 6 Chars): $";
            getline(cin, StringValue);
            if (!StringValue.empty()) {
                if (StringValue.length() <= 6) {
                    for (int i = 0; i < StringValue.length(); i++) {
                        if (!isdigit(StringValue[i])) {
                            cout << "Please Enter A Valid Number: $";
                            goto dd;
                        }
                    }
                    ValueOfMods = stof(StringValue);
                }
                if (StringValue.length() >= 7) {
                    cout << "You Cannot Input More Than 6 Chars : ";
                    goto dd;
                }
            }
        }
    }

    cout << "Is The Car Financed (" << DisplayBool(Financed) << ") (Y | N): ";
ee:
    getline(cin, StringValue);
    if (!StringValue.empty()) {
        transform(StringValue.begin(), StringValue.end(), StringValue.begin(), ::tolower);
        if (StringValue == "y") {
            Financed = true;
        }
        if (StringValue == "n") {
            Financed = false;
        }
        else {
            cout << "Please Enter A Valid Input (Y | N): ";
            goto ee;
        }
    }

    cout << "Is This A Business Vehicle (" << DisplayBool(Business) << ") (Y | N): ";
ff:
    getline(cin, StringValue);
    if (!StringValue.empty()) {
        transform(StringValue.begin(), StringValue.end(), StringValue.begin(), ::tolower);
        if (StringValue == "y") {
            Business = true;
        }
        else if (StringValue == "n") {
            Business = false;
        }
        else {
            cout << "Please Enter A Valid Input (Y | N): ";
            goto ff;
        }
    }

    cout << "What Is The Average Amount Of Kilometers You Will Travel In The Vehicle (" << Kms << "): ";
hh:
    getline(cin, StringValue);
    if (!StringValue.empty()) {
        if (StringValue.length() <= 6) {
            for (int i = 0; i < StringValue.length(); i++) {
                if (!isdigit(StringValue[i])) {
                    cout << "Please Enter A Valid Number: ";
                    goto hh;
                }
            }
            Kms = stof(StringValue);
        }
        if (StringValue.length() >= 7) {
            cout << "You Cannot Input More Than 6 Chars : ";
            goto hh;
        }
    }

    cout << "What Address Will The Vehicle Primarly Be Stored At (" << Address << "): ";
    getline(cin, StringValue);
    if (!StringValue.empty()) {
        Address = StringValue;
    }

    cout << "\n------------[ Driver Details ]------------" << endl;

    cout << "Please Enter The First Name Of The Main Driver (" << FirstName << "): ";
    getline(cin, StringValue);
    if (!StringValue.empty()) {
        FirstName = StringValue;
    }

    cout << "Please Enter The Last Name Of The Main Driver (" << LastName << "): ";
    getline(cin, StringValue);
    if (!StringValue.empty()) {
        LastName = StringValue;
    }

    cout << "Please Enter The Date Of Birth Of The Main Driver (" << DOB << ") (Format: DD/MM/YYYY): ";
    getline(cin, StringValue);
    if (!StringValue.empty()) {
        DOB = StringValue;
    }

    cout << "Please Enter The Gender Of The Main Driver (" << Gender << ")  (Male | Female): ";
ii:
    getline(cin, StringValue);
    if (!StringValue.empty()) {
        transform(StringValue.begin(), StringValue.end(), StringValue.begin(), ::tolower);
        if (StringValue == "male") {
            Gender = StringValue;
        }
        else if (StringValue == "female") {

        }
        else {
            cout << "Please Enter A Vaild Value (Male | Female): ";
            goto ii;
        }
    }

    cout << "How Many Incidents Has The Main Driver Had During The Last 3 Years (" << Incidents << "): ";
jj:
    getline(cin, StringValue);
    if (!StringValue.empty()) {
        if (StringValue.length() <= 6) {
            for (int i = 0; i < StringValue.length(); i++) {
                if (!isdigit(StringValue[i])) {
                    cout << "Please Enter A Valid Number: $";
                    goto jj;
                }
            }
            Incidents = stof(StringValue);
        }
        if (StringValue.length() >= 7) {
            cout << "You Cannot Input More Than 6 Chars : ";
            goto jj;
        }
    }

    cout << "How Many Other Drivers Will Drive This Vehicle (" << Drivers << "): ";
kk:
    getline(cin, StringValue);
    if (!StringValue.empty()) {
        if (StringValue.length() <= 6) {
            for (int i = 0; i < StringValue.length(); i++) {
                if (!isdigit(StringValue[i])) {
                    cout << "Please Enter A Valid Number: $";
                    goto kk;
                }
            }
            Drivers = stof(StringValue);
        }
        if (StringValue.length() >= 7) {
            cout << "You Cannot Input More Than 6 Chars : ";
            goto kk;
        }
    }

    cout << "How Many Months Would You Like To Have The Vehicle Insured For (" << Length << "): ";
ll:
    getline(cin, StringValue);
    if (!StringValue.empty()) {
        if (StringValue.length() <= 6) {
            for (int i = 0; i < StringValue.length(); i++) {
                if (!isdigit(StringValue[i])) {
                    cout << "Please Enter A Valid Number: $";
                    goto ll;
                }
            }
            Length = stof(StringValue);
        }
        if (StringValue.length() >= 7) {
            cout << "You Cannot Input More Than 6 Chars : ";
            goto ll;
        }
    }

    // Now To Return To Square One

    PolicyDetails(Admin, AdminEdit, false, CurrentUser, false, IsNewQuote, PolicyNumber, Plate, Type, Model, Value, Modifications, ValueOfMods, Financed, Business, Kms, Address, FirstName, LastName, DOB, Gender, Incidents, Drivers, Length, true);
}
// What Do You Think This Does Because I Dont Know
void VehicleInsuranceSystem::NewPolicy(User Admin, bool AdminEdit, User CurrentUser, int PolicyNumber)
{
    // Policy Inputs Round 1. Oh How I Under estimated this part.
    SetConsoleTitle(TEXT("398 Studios Insurance | New Policy"));
    system("CLS");
    
    cout << "======================================================================================================" << endl;
    cout << "                  New Policy For " << CurrentUser.Username << endl;
    cout << "======================================================================================================\n" << endl;
    cout << "------------[ Vehicle Details ]------------" << endl;
    
    string StringValue;
    
    string Plate;
        
    cout << "Please Enter The Vehicle Regerstration Number: ";
a:
    getline(cin, Plate);
    if (Plate.empty()) {
        cout << "Please Enter A Value: ";
        goto a;
    }
    // I Dont Know Why I Did This It Probably Nukes Preformance But I Dont Give 2 Shits
    string CheckIfExists = string("Files/Policies/") + Plate + string(".txt");
    if (std::filesystem::exists(CheckIfExists)) {
        cout << "Sorry A Policy Already Exists With This Regerstration Number. Enter Another Regerstration Number: ";
        goto a;
    }
    
    string Type;    
    cout << "Please Enter Vehicle Type: ";
b:
    getline(cin, Type);
    if (Type.empty()) {
        cout << "Please Enter A Value: ";
        goto b;
    }
    
    string Model;
    cout << "Please Enter The Vehicle Model: ";
c:
    getline(cin, Model);
    if (Model.empty()) {
        cout << "Please Enter A Value: ";
        goto c;
    }
    
    // Jesus Christ I Spent Way Too Much Fucking Time On This Instead Of Just Doing cin >> Value;
    float Value;
    cout << "Please Enter The Value Of The Vehicle (MAX 6 Chars): $";
d:
    getline(cin, StringValue);
    if (!StringValue.empty()) {
        if (StringValue.length() <= 6) {
            for (int i = 0; i < StringValue.length(); i++) {
                if (!isdigit(StringValue[i])){
                    cout << "Please Enter A Valid Number: $";
                    goto d;
                }
            }
    
            Value = stoi(StringValue);
            if (Value <= 100) {
                cout << "The Value Needs To Be Greater Than $100: " << endl;
                goto d;
            }
        } 
        if (StringValue.length() >= 7) {
            cout << "You Cannot Input More Than 6 Chars : ";
            goto d;
        }
    }
    else {
        cout << "This Input Can Not Be Empty: ";
        goto d;
    }
    
    // I Decided To Make This System Way OverComplicated For Shits And Giggles But Im Starting To Regret It
    bool Modifications;
    float ValueOfMods = 0;
    string ModsBool;
    cout << "Does Your Vehicle Have Any Modifications (Y | N): ";
e:
    getline(cin, ModsBool);
    if (!ModsBool.empty()) {
        transform(ModsBool.begin(), ModsBool.end(), ModsBool.begin(), ::tolower);
        if (ModsBool == "y") {
            Modifications = true;
            cout << "What Is The Value Of The Modifications On The Vehicle (MAX 6 Chars): $";
        f:
            getline(cin, StringValue);
            if (!StringValue.empty()) {
                if (StringValue.length() <= 6) {
                    for (int i = 0; i < StringValue.length(); i++) {
                        if (!isdigit(StringValue[i])) {
                            cout << "Please Enter A Valid Number: $";
                            goto f;
                        }
                    }
                    ValueOfMods = stof(StringValue);
                }
                if (StringValue.length() >= 7) {
                    cout << "You Cannot Input More Than 6 Chars : ";
                    goto f;
                }
            }
            else {
                cout << "This Input Cant Be Empty: ";
                goto f;
            }
        }
        else if (ModsBool == "n") {
            Modifications = false;
        }
        else {
            cout << "Please Enter A Valid Input (Y | N): ";
            goto e;
        }
    } else {
        cout << "This Input Cant Be Empty: ";
        goto e;
    }
    
    bool Financed = false;
    g:
    cout << "Is The Car Financed (Y | N): ";
    getline(cin, StringValue);
    if (!StringValue.empty()) {
        transform(StringValue.begin(), StringValue.end(), StringValue.begin(), ::tolower);
        if (StringValue == "y") {
            Financed = true;
        }
        if (StringValue == "n") {
            Financed = false;
        }
        else {
            cout << "Please Enter A Valid Input (Y | N): ";
            goto g;
        }
    }
    else {
        cout << "This Input Cant Be Empty: ";
        goto g;
    }
    
    bool Business = false;    
    cout << "Is This A Business Vehicle (Y | N): ";
h:
    getline(cin, StringValue);
    if (!StringValue.empty()) {
        transform(StringValue.begin(), StringValue.end(), StringValue.begin(), ::tolower);
        if (StringValue == "y") {
            Business = true;
        }
        else if (StringValue == "n") {
            Business = false;
        }
        else {
            cout << "Please Enter A Valid Input (Y | N): ";
            goto h;
        }
    }
    else {
        cout << "This Input Cant Be Empty: ";
        goto h;
    }
    
    int Kms;
    cout << "What Is The Average Amount Of Kilometers You Will Travel In The Vehicle (MAX 6 Chars): ";
i:
    getline(cin, StringValue);
    if (!StringValue.empty()) {
        if (StringValue.length() <= 6) {
            for (int i = 0; i < StringValue.length(); i++) {
                if (!isdigit(StringValue[i])) {
                    cout << "Please Enter A Valid Number: ";
                    goto i;
                }
            }
            Kms = stof(StringValue);
        }
        if (StringValue.length() >= 7) {
            cout << "You Cannot Input More Than 6 Chars : ";
            goto i;
        }
    }
    else {
        cout << "This Input Cant Be Empty: ";
        goto i;
    }
    
    string Address;
    cout << "What Address Will The Vehicle Primarly Be Stored At: ";
    j:
    getline(cin, StringValue);
    if (!StringValue.empty()) {
        Address = StringValue;
    }
    else {
        cout << "This Input Cant Be Empty: ";
        goto j;
    }
    
    cout << "\n------------[ Driver Details ]------------" << endl;
    string FirstName;
    cout << "Please Enter The First Name Of The Main Driver: ";
    k:
    getline(cin, StringValue);
    if (!StringValue.empty()) {
        FirstName = StringValue;
    }
    else {
        cout << "This Input Cant Be Empty: ";
        goto k;
    }
    
    
    string LastName;
    cout << "Please Enter The Last Name Of The Main Driver: ";
    l:
    getline(cin, StringValue);
    if (!StringValue.empty()) {
        LastName = StringValue;
    }
    else {
        cout << "This Input Cant Be Empty: ";
        goto l;
    }
    
    
    string DOB;
    cout << "Please Enter The Date Of Birth Of The Main Driver (Format: DD/MM/YYYY): ";
    m:
    getline(cin, StringValue);
    if (!StringValue.empty()) {
        DOB = StringValue;
    }
    else {
        cout << "This Input Cant Be Empty: ";
        goto m;
    }
    
    
    string Gender;
    cout << "Please Enter The Gender Of The Main Driver (Male | Female): ";
    n:
    getline(cin, StringValue);
    if (!StringValue.empty()) {
        transform(StringValue.begin(), StringValue.end(), StringValue.begin(), ::tolower);
        if (StringValue == "male") {
            Gender = "Male";
        }
        else if (StringValue == "female") {
            Gender = "Female";
        }
        else {
            cout << "Please Enter A Vaild Value (Male | Female): ";
            goto n;
        }
    }
    else {
        cout << "This Input Cant Be Empty: ";
        goto n;
    }
    
    int Incidents;
    cout << "How Many Incidents Has The Main Driver Had During The Last 3 Years: ";
    p:
    getline(cin, StringValue);
    if (!StringValue.empty()) {
        if (StringValue.length() <= 6) {
            for (int i = 0; i < StringValue.length(); i++) {
                if (!isdigit(StringValue[i])) {
                    cout << "Please Enter A Valid Number: ";
                    goto p;
                }
            }
            Incidents = stof(StringValue);
        }
        if (StringValue.length() >= 7) {
            cout << "You Cannot Input More Than 6 Chars : ";
            goto p;
        }
    }
    else {
        cout << "This Input Cant Be Empty: ";
        goto p;
    }
    
    int Drivers;
    cout << "How Many Other Drivers Will Drive This Vehicle: ";
    q:
    getline(cin, StringValue);
    if (!StringValue.empty()) {
        if (StringValue.length() <= 6) {
            for (int i = 0; i < StringValue.length(); i++) {
                if (!isdigit(StringValue[i])) {
                    cout << "Please Enter A Valid Number: ";
                    goto q;
                }
            }
            Drivers = stof(StringValue);
        }
        if (StringValue.length() >= 7) {
            cout << "You Cannot Input More Than 6 Chars : ";
            goto q;
        }
    }
    else {
        cout << "This Input Cant Be Empty: ";
        goto q;
    }
    
    int Length;
    cout << "How Many Months Would You Like To Have The Vehicle Insured For: ";
    r:
    getline(cin, StringValue);
    if (!StringValue.empty()) {
        if (StringValue.length() <= 6) {
            for (int i = 0; i < StringValue.length(); i++) {
                if (!isdigit(StringValue[i])) {
                    cout << "Please Enter A Valid Number: $";
                    goto r;
                }
            }
            Length = stof(StringValue);
        }
        if (StringValue.length() >= 7) {
            cout << "You Cannot Input More Than 6 Chars : ";
            goto r;
        }
    }
    else {
        cout << "This Input Cant Be Empty: ";
        goto r;
    }
        
    PolicyDetails(Admin, AdminEdit, false, CurrentUser, false, true, PolicyNumber, Plate, Type, Model, Value, Modifications, ValueOfMods, Financed, Business, Kms, Address, FirstName, LastName, DOB, Gender, Incidents, Drivers, Length, true);
}
// Time For Speedrun Of Killing Myself Round 2
void VehicleInsuranceSystem::ViewPolicys(User CurrentUser)
{
    User Admin = User("", "", "", false, false, true, "");
    system("CLS");
        // For Coding This At 1am I Think Its Pretty Good
        json PolicysList;
        json CanceledPolicysList;
        int Policys = 0;
        if (CurrentUser.Policies.size() > 0) {
            for (int n = 0; n < CurrentUser.Policies.size(); n++) {
                if (CurrentUser.Policies[n]["Status"] != false) {
                    Policys++;
                }
            }
            if (Policys != 0) {
                cout << "Listing Current Policys For " << CurrentUser.Email << endl;
                for (int k = 0; k < CurrentUser.Policies.size(); k++) {
                    if (CurrentUser.Policies[k]["Status"] != false) {
                        json currentPushBack = {
                            { "User", CurrentUser.Email},
                            { "Policy", CurrentUser.Policies[k] },
                            { "PolicyNumber", k }
                        };
                        PolicysList.push_back(currentPushBack);

                        cout << "Policy (" << PolicysList.size() << ") " << CurrentUser.Policies[k]["Registration"] << " Model: " << CurrentUser.Policies[k]["Model"] << endl;
                    }
                }
            }
        }

        cout << "\nCanceled Policys\n\n";
        int CanceledPolicys = 0;
        if (CurrentUser.Policies.size() > 0) {
            for (int n = 0; n < CurrentUser.Policies.size(); n++) {
                if (CurrentUser.Policies[n]["Status"] == false) {
                    CanceledPolicys++;
                }
            }
            if (CanceledPolicys != 0) {
                cout << "Listing Canceled Policys For " << CurrentUser.Email << endl;
                for (int k = 0; k < CurrentUser.Policies.size(); k++) {
                    if (CurrentUser.Policies[k]["Status"] == false) {
                        json currentPushBack = {
                            { "User", CurrentUser.Email},
                            { "Policy", CurrentUser.Policies[k] },
                            { "PolicyNumber", k }
                        };
                        CanceledPolicysList.push_back(currentPushBack);

                        cout << "Policy (" << CanceledPolicysList.size() << ") " << CurrentUser.Policies[k]["Registration"] << " Model: " << CurrentUser.Policies[k]["Model"] << endl;
                    }
                }
            }
        }
        if (CanceledPolicysList.size() == 0) {
            cout << "There Are No Canceled Policys" << endl;
        }
        if (CanceledPolicysList.size() == 0 && PolicysList.size() == 0) {
            Menu(CurrentUser, CurrentUser.RootUser);
        }

        string command;
        if (PolicysList.size() > 0 && CanceledPolicysList.size() == 0) {
            cout << "\nYour Current Options Are: View Policy (0) | Edit Policy (1) | Cancel Policy (2) | Back (3) | Quit (4)" << endl;
        viewAllCommandPolicysOnly1:
            cout << ">";
            getline(cin, command);
            if (command == "View Policy" || command == "0") {
                string StringValue;
                int PolicyChoice;
            policysViewChoose7:
                cout << "Please Enter The Number Of The Policy You Would Like To Edit: ";
                getline(cin, StringValue);
                if (!StringValue.empty()) {
                    if (StringValue.length() <= 6) {
                        for (int i = 0; i < StringValue.length(); i++) {
                            if (!isdigit(StringValue[i])) {
                                cout << "Please Enter A Valid Number: ";
                                goto policysViewChoose7;
                            }
                        }
                        PolicyChoice = stoi(StringValue);

                        if (PolicyChoice - 1 > PolicysList.size()) {
                            cout << "Policy Number Cant Be More Than The Amount Of Policys: ";
                            goto policysViewChoose7;
                        }
                        json Policy = PolicysList[PolicyChoice - 1]["Policy"];
                        //Fucking Hell This Is A Fucking Damn Long Ass Line And I Hate It Because I Made It Even Longer
                        PolicyDetails(Admin, false, false, CurrentUser, false, false, PolicysList[PolicyChoice - 1]["PolicyNumber"], Policy["Registration"], Policy["Type"], Policy["Model"], Policy["Value"], Policy["Modifications"], Policy["ValueOfMods"], Policy["Financed"], Policy["Business"], Policy["Kms"], Policy["Address"], Policy["FirstName"], Policy["LastName"], Policy["DOB"], Policy["Gender"], Policy["Incidents"], Policy["Drivers"], Policy["Length"], true);
                    }
                }
                else {
                    cout << "This Input Cant Be Empty: ";
                    goto policysViewChoose7;
                }
            }
            if (command == "Edit Policy" || command == "1") {
                string StringValue;
                int PolicyChoice;
            policysEditChoose5:
                cout << "Please Enter The Number Of The Policy You Would Like To Edit: ";
                getline(cin, StringValue);
                if (!StringValue.empty()) {
                    if (StringValue.length() <= 6) {
                        for (int i = 0; i < StringValue.length(); i++) {
                            if (!isdigit(StringValue[i])) {
                                cout << "Please Enter A Valid Number: ";
                                goto policysEditChoose5;
                            }
                        }
                        PolicyChoice = stoi(StringValue);

                        if (PolicyChoice - 1 > PolicysList.size()) {
                            cout << "Policy Number Cant Be More Than The Amount Of Policys: ";
                            goto policysEditChoose5;
                        }
                        json Policy = PolicysList[PolicyChoice - 1]["Policy"];
                        //Fucking Hell This Is A Fucking Damn Long Ass Line
                        EditPolicy(Admin, false, CurrentUser, false, PolicysList[PolicyChoice - 1]["PolicyNumber"], Policy["Registration"], Policy["Type"], Policy["Model"], Policy["Value"], Policy["Modifications"], Policy["ValueOfMods"], Policy["Financed"], Policy["Business"], Policy["Kms"], Policy["Address"], Policy["FirstName"], Policy["LastName"], Policy["DOB"], Policy["Gender"], Policy["Incidents"], Policy["Drivers"], Policy["Length"]);
                    }
                }
                else {
                    cout << "This Input Cant Be Empty: ";
                    goto policysEditChoose5;
                }
            }
            if (command == "Cancel Policy" || command == "2") {
                string StringValue;
                int PolicyChoice;
            policysCancelChoose2:
                cout << "Please Enter The Number Of A Current Active Policy You Would Like To Cancel: ";
                getline(cin, StringValue);
                if (!StringValue.empty()) {
                    if (StringValue.length() <= 6) {
                        for (int i = 0; i < StringValue.length(); i++) {
                            if (!isdigit(StringValue[i])) {
                                cout << "Please Enter A Valid Number: ";
                                goto policysCancelChoose2;
                            }
                        }
                        PolicyChoice = stoi(StringValue);

                        if (PolicyChoice - 1 > PolicysList.size()) {
                            cout << "Policy Number Cant Be More Than The Amount Of Active Policys: ";
                            goto policysCancelChoose2;
                        }
                        json Policy = PolicysList[PolicyChoice - 1]["Policy"];
                        //Fucking Hell This Is A Fucking Damn Long Ass Line And I Hate It Because I Made It Even Longer
                        PolicyDetails(Admin, false, false, CurrentUser, true, false, PolicysList[PolicyChoice - 1]["PolicyNumber"], Policy["Registration"], Policy["Type"], Policy["Model"], Policy["Value"], Policy["Modifications"], Policy["ValueOfMods"], Policy["Financed"], Policy["Business"], Policy["Kms"], Policy["Address"], Policy["FirstName"], Policy["LastName"], Policy["DOB"], Policy["Gender"], Policy["Incidents"], Policy["Drivers"], Policy["Length"], true);
                    }
                }
                else {
                    cout << "This Input Cant Be Empty: ";
                    goto policysCancelChoose2;
                }
            }
            if (command == "Back" || command == "3") {
                Menu(CurrentUser, CurrentUser.RootUser);
            }
            if (command == "Quit" || command == "4") {
                exit(0);
            }
            else {
                cout << "Unknown Command. Valid Commands Are: View Policy (0) | Edit Policy (1) | Cancel Policy (2) | Back (3) | Quit (4)" << endl;
                goto viewAllCommandPolicysOnly1;
            }
        }
        if (CanceledPolicysList.size() > 0 && PolicysList.size() == 0) {
            cout << "\nYour Current Options Are: View Policy (0) | Renew Policy (1) | Back (2) | Quit (3)" << endl;
        viewAllCanceledPolicysCommand2:
            cout << ">";
            getline(cin, command);
            if (command == "View Policy" || command == "0") {
                string StringValue;
                int PolicyChoice;
            policysViewChoose6:
                cout << "Please Enter The Number Of The Policy You Would Like To Edit: ";
                getline(cin, StringValue);
                if (!StringValue.empty()) {
                    if (StringValue.length() <= 6) {
                        for (int i = 0; i < StringValue.length(); i++) {
                            if (!isdigit(StringValue[i])) {
                                cout << "Please Enter A Valid Number: ";
                                goto policysViewChoose6;
                            }
                        }
                        PolicyChoice = stoi(StringValue);

                        if (PolicyChoice - 1 > CanceledPolicysList.size()) {
                            cout << "Policy Number Cant Be More Than The Amount Of Policys: ";
                            goto policysViewChoose6;
                        }
                        json Policy = CanceledPolicysList[PolicyChoice - 1]["Policy"];
                        //Fucking Hell This Is A Fucking Damn Long Ass Line And I Hate It Because I Made It Even Longer
                        PolicyDetails(Admin, false, false, CurrentUser, true, false, CanceledPolicysList[PolicyChoice - 1]["PolicyNumber"], Policy["Registration"], Policy["Type"], Policy["Model"], Policy["Value"], Policy["Modifications"], Policy["ValueOfMods"], Policy["Financed"], Policy["Business"], Policy["Kms"], Policy["Address"], Policy["FirstName"], Policy["LastName"], Policy["DOB"], Policy["Gender"], Policy["Incidents"], Policy["Drivers"], Policy["Length"], false);
                    }
                }
                else {
                    cout << "This Input Cant Be Empty: ";
                    goto policysViewChoose6;
                }
            }
            if (command == "Renew Policy" || command == "1") {
                string StringValue;
                int PolicyChoice;
            policysRenewChoose6:
                cout << "Please Enter The Number Of A Current Canceled Policy You Would Like To Renew: ";
                getline(cin, StringValue);
                if (!StringValue.empty()) {
                    if (StringValue.length() <= 6) {
                        for (int i = 0; i < StringValue.length(); i++) {
                            if (!isdigit(StringValue[i])) {
                                cout << "Please Enter A Valid Number: ";
                                goto policysRenewChoose6;
                            }
                        }
                        PolicyChoice = stoi(StringValue);

                        if (PolicyChoice - 1 > CanceledPolicysList.size()) {
                            cout << "Policy Number Cant Be More Than The Amount Of Canceled Policys: ";
                            goto policysRenewChoose6;
                        }

                        json Policy = CanceledPolicysList[PolicyChoice - 1]["Policy"];
                        PolicyDetails(Admin, false, false, CurrentUser, true, false, CanceledPolicysList[PolicyChoice - 1]["PolicyNumber"], Policy["Registration"], Policy["Type"], Policy["Model"], Policy["Value"], Policy["Modifications"], Policy["ValueOfMods"], Policy["Financed"], Policy["Business"], Policy["Kms"], Policy["Address"], Policy["FirstName"], Policy["LastName"], Policy["DOB"], Policy["Gender"], Policy["Incidents"], Policy["Drivers"], Policy["Length"], false);
                    }
                }
                else {
                    cout << "This Input Cant Be Empty: ";
                    goto policysRenewChoose6;
                }
            }
            if (command == "Back" || command == "2") {
                Menu(CurrentUser, CurrentUser.RootUser);
            }
            if (command == "Quit" || command == "3") {
                exit(0);
            }
            else {
                cout << "Your Current Options Are: View Policy (0) | Renew Policy (1) | Back (2) | Quit (3)" << endl;
                goto viewAllCanceledPolicysCommand2;
            }
        }
        if (CanceledPolicysList.size() > 0 && PolicysList.size() > 0) {
            cout << "\nYour Current Options Are: View Policy (0) | View Canceled Policy (1) | Edit Policy (2)| Cancel Policy (3) | Renew Policy (4) | Back (5) | Quit (6)" << endl;
        viewallcommand2:
            cout << ">";
            getline(cin, command);
            if (command == "View Policy" || command == "0") {
                string StringValue;
                int PolicyChoice;
            policysViewChoose4:
                cout << "Please Enter The Number Of The Policy You Would Like To Edit: ";
                getline(cin, StringValue);
                if (!StringValue.empty()) {
                    if (StringValue.length() <= 6) {
                        for (int i = 0; i < StringValue.length(); i++) {
                            if (!isdigit(StringValue[i])) {
                                cout << "Please Enter A Valid Number: ";
                                goto policysViewChoose4;
                            }
                        }
                        PolicyChoice = stoi(StringValue);

                        if (PolicyChoice - 1 > CanceledPolicysList.size()) {
                            cout << "Policy Number Cant Be More Than The Amount Of Policys: ";
                            goto policysViewChoose4;
                        }
                        json Policy = CanceledPolicysList[PolicyChoice - 1]["Policy"];
                        PolicyDetails(Admin, false, false, CurrentUser, false, true, CanceledPolicysList[PolicyChoice - 1]["PolicyNumber"], Policy["Registration"], Policy["Type"], Policy["Model"], Policy["Value"], Policy["Modifications"], Policy["ValueOfMods"], Policy["Financed"], Policy["Business"], Policy["Kms"], Policy["Address"], Policy["FirstName"], Policy["LastName"], Policy["DOB"], Policy["Gender"], Policy["Incidents"], Policy["Drivers"], Policy["Length"], true);
                    }
                }
                else {
                    cout << "This Input Cant Be Empty: ";
                    goto policysViewChoose4;
                }
            }
            if (command == "View Canceled Policy" || command == "1") {
                string StringValue;
                int PolicyChoice;
            policysViewChoose5:
                cout << "Please Enter The Number Of The Canceled Policy You Would Like To View: ";
                getline(cin, StringValue);
                if (!StringValue.empty()) {
                    if (StringValue.length() <= 6) {
                        for (int i = 0; i < StringValue.length(); i++) {
                            if (!isdigit(StringValue[i])) {
                                cout << "Please Enter A Valid Number: ";
                                goto policysViewChoose5;
                            }
                        }
                        PolicyChoice = stoi(StringValue);

                        if (PolicyChoice - 1 > CanceledPolicysList.size()) {
                            cout << "Policy Number Cant Be More Than The Amount Of Policys: ";
                            goto policysViewChoose5;
                        }
                        json Policy = CanceledPolicysList[PolicyChoice - 1]["Policy"];
                        PolicyDetails(Admin, false, false, CurrentUser, true, false, CanceledPolicysList[PolicyChoice - 1]["PolicyNumber"], Policy["Registration"], Policy["Type"], Policy["Model"], Policy["Value"], Policy["Modifications"], Policy["ValueOfMods"], Policy["Financed"], Policy["Business"], Policy["Kms"], Policy["Address"], Policy["FirstName"], Policy["LastName"], Policy["DOB"], Policy["Gender"], Policy["Incidents"], Policy["Drivers"], Policy["Length"], false);
                    }
                }
                else {
                    cout << "This Input Cant Be Empty: ";
                    goto policysViewChoose5;
                }
            }
            if (command == "Edit Policy" || command == "2") {
                string StringValue;
                int PolicyChoice;
            policysEditChoose4:
                cout << "Please Enter The Number Of The Policy You Would Like To Edit: ";
                getline(cin, StringValue);
                if (!StringValue.empty()) {
                    if (StringValue.length() <= 6) {
                        for (int i = 0; i < StringValue.length(); i++) {
                            if (!isdigit(StringValue[i])) {
                                cout << "Please Enter A Valid Number: ";
                                goto policysEditChoose4;
                            }
                        }
                        PolicyChoice = stoi(StringValue);

                        if (PolicyChoice - 1 > PolicysList.size()) {
                            cout << "Policy Number Cant Be More Than The Amount Of Policys: ";
                            goto policysEditChoose4;
                        }
                        json Policy = PolicysList[PolicyChoice - 1]["Policy"];
                        EditPolicy(Admin, false, CurrentUser, false, PolicysList[PolicyChoice - 1]["PolicyNumber"], Policy["Registration"], Policy["Type"], Policy["Model"], Policy["Value"], Policy["Modifications"], Policy["ValueOfMods"], Policy["Financed"], Policy["Business"], Policy["Kms"], Policy["Address"], Policy["FirstName"], Policy["LastName"], Policy["DOB"], Policy["Gender"], Policy["Incidents"], Policy["Drivers"], Policy["Length"]);
                    }
                }
                else {
                    cout << "This Input Cant Be Empty: ";
                    goto policysEditChoose4;
                }
            }
            if (command == "Cancel Policy" || command == "3") {
                string StringValue;
                int PolicyChoice;
            policysCancelChoose4:
                cout << "Please Enter The Number Of A Current Active Policy You Would Like To Cancel: ";
                getline(cin, StringValue);
                if (!StringValue.empty()) {
                    if (StringValue.length() <= 6) {
                        for (int i = 0; i < StringValue.length(); i++) {
                            if (!isdigit(StringValue[i])) {
                                cout << "Please Enter A Valid Number: ";
                                goto policysCancelChoose4;
                            }
                        }
                        PolicyChoice = stoi(StringValue);

                        if (PolicyChoice - 1 > PolicysList.size()) {
                            cout << "Policy Number Cant Be More Than The Amount Of Active Policys: ";
                            goto policysCancelChoose4;
                        }
                        json Policy = PolicysList[PolicyChoice - 1]["Policy"];
                        //Fucking Hell This Is A Fucking Damn Long Ass Line And I Hate It Because I Made It Even Longer
                        PolicyDetails(Admin, false, false, CurrentUser, true, false, PolicysList[PolicyChoice - 1]["PolicyNumber"], Policy["Registration"], Policy["Type"], Policy["Model"], Policy["Value"], Policy["Modifications"], Policy["ValueOfMods"], Policy["Financed"], Policy["Business"], Policy["Kms"], Policy["Address"], Policy["FirstName"], Policy["LastName"], Policy["DOB"], Policy["Gender"], Policy["Incidents"], Policy["Drivers"], Policy["Length"], true);
                    }
                }
                else {
                    cout << "This Input Cant Be Empty: ";
                    goto policysCancelChoose4;
                }
            }
            if (command == "Renew Policy" || command == "4") {
                string StringValue;
                int PolicyChoice;
            policysRenewChoose4:
                cout << "Please Enter The Number Of A Current Canceled Policy You Would Like To Renew: ";
                getline(cin, StringValue);
                if (!StringValue.empty()) {
                    if (StringValue.length() <= 6) {
                        for (int i = 0; i < StringValue.length(); i++) {
                            if (!isdigit(StringValue[i])) {
                                cout << "Please Enter A Valid Number: ";
                                goto policysRenewChoose4;
                            }
                        }
                        PolicyChoice = stoi(StringValue);

                        if (PolicyChoice - 1 > CanceledPolicysList.size()) {
                            cout << "Policy Number Cant Be More Than The Amount Of Canceled Policys: ";
                            goto policysRenewChoose4;
                        }
                        json Policy = CanceledPolicysList[PolicyChoice - 1]["Policy"];
                        PolicyDetails(Admin, false, false, CurrentUser, true, false, CanceledPolicysList[PolicyChoice - 1]["PolicyNumber"], Policy["Registration"], Policy["Type"], Policy["Model"], Policy["Value"], Policy["Modifications"], Policy["ValueOfMods"], Policy["Financed"], Policy["Business"], Policy["Kms"], Policy["Address"], Policy["FirstName"], Policy["LastName"], Policy["DOB"], Policy["Gender"], Policy["Incidents"], Policy["Drivers"], Policy["Length"], false);
                    }
                }
                else {
                    cout << "This Input Cant Be Empty: ";
                    goto policysRenewChoose4;
                }
            }
            if (command == "Back" || command == "5") {
                Menu(CurrentUser, CurrentUser.RootUser);
            }
            if (command == "Quit" || command == "6") {
                exit(0);
            }
            else {
                cout << "Unknown Command. Valid Commands Are: View Policy (0) | Edit Policy (1) | Delete Policy (2) | Back (3) | Quit (4)" << endl;
                goto viewallcommand2;
            }
        }
}
// I NEVER EVER WANT TO TOUCH THIS FUCKING PIECE OF SHIT AGAIN. Oh Shit I forgot About The Non Admin Side
void VehicleInsuranceSystem::ViewAllPolicys(User Admin)
{
    system("CLS");
    string UsersFile = "Files/AllUsers.json";
    ifstream file(UsersFile);

    if (!file.fail()) {
        json j;
        
        file >> j;

        // For Coding This At 1am I Think Its Pretty Good
        json PolicysList;
        json CanceledPolicysList;
        for (int i = 0; i < j.size(); i++)
        {
            User CurrentUser = SearchForUser(j[i]["User"]);
            int Policys = 0;
            if (CurrentUser.Policies.size() > 0) {
                for (int n = 0; n < CurrentUser.Policies.size(); n++) {
                    if (CurrentUser.Policies[n]["Status"] != false) {
                        Policys++;
                    }
                }
                if (Policys != 0) {
                    cout << "Listing Current Policys For " << CurrentUser.Email << endl;
                    for (int k = 0; k < CurrentUser.Policies.size(); k++) {
                        if (CurrentUser.Policies[k]["Status"] != false) {
                            json currentPushBack = {
                                { "User", CurrentUser.Email},
                                { "Policy", CurrentUser.Policies[k] },
                                { "PolicyNumber", k }
                            };
                            PolicysList.push_back(currentPushBack);

                            cout << "Policy (" << PolicysList.size() << ") " << CurrentUser.Policies[k]["Registration"] << " Model: " << CurrentUser.Policies[k]["Model"] << endl;
                        }
                    }
                }
            }
        }        

        cout << "\nCanceled Policys\n\n";
        for (int i = 0; i < j.size(); i++) {
            User CurrentUser = SearchForUser(j[i]["User"]);   
            int CanceledPolicys = 0;
            if (CurrentUser.Policies.size() > 0) {
                for (int n = 0; n < CurrentUser.Policies.size(); n++) {
                    if (CurrentUser.Policies[n]["Status"] == false) {
                        CanceledPolicys++;
                    }
                }
                if (CanceledPolicys != 0) {
                    cout << "Listing Canceled Policys For " << CurrentUser.Email << endl;
                    for (int k = 0; k < CurrentUser.Policies.size(); k++) {
                        if (CurrentUser.Policies[k]["Status"] == false) {
                            json currentPushBack = {
                                { "User", CurrentUser.Email},
                                { "Policy", CurrentUser.Policies[k] },
                                { "PolicyNumber", k }
                            };
                            CanceledPolicysList.push_back(currentPushBack);

                            cout << "Policy (" << CanceledPolicysList.size() << ") " << CurrentUser.Policies[k]["Registration"] << " Model: " << CurrentUser.Policies[k]["Model"] << endl;
                        }
                    }
                }
            }
        }
        if (CanceledPolicysList.size() == 0) {
            cout << "There Are No Canceled Policys" << endl;
        }
        file.close();
        if (CanceledPolicysList.size() == 0 && PolicysList.size() == 0) {
            Menu(Admin, Admin.RootUser);
        }

        string command;
        if (PolicysList.size() > 0 && CanceledPolicysList.size() == 0) {
            cout << "\nYour Current Options Are: View Policy (0) | Edit Policy (1) | Delete Policy (2) | Cancel Policy (3) | Back (4) | Quit (5)" << endl;
        viewAllCommandPolicysOnly:
            cout << ">";
            getline(cin, command);
            transform(command.begin(), command.end(), command.begin(), ::tolower);
            if (command == "view policy" || command == "0") {
                string StringValue;
                int PolicyChoice;
            policysViewChoose:
                cout << "Please Enter The Number Of The Policy You Would Like To View: ";
                getline(cin, StringValue);
                if (!StringValue.empty()) {
                    if (StringValue.length() <= 6) {
                        for (int i = 0; i < StringValue.length(); i++) {
                            if (!isdigit(StringValue[i])) {
                                cout << "Please Enter A Valid Number: ";
                                goto policysViewChoose;
                            }
                        }
                        PolicyChoice = stoi(StringValue);

                        if (PolicyChoice - 1 > PolicysList.size()) {
                            cout << "Policy Number Cant Be More Than The Amount Of Policys: ";
                            goto policysViewChoose;
                        }
                        User PolicyUser = SearchForUser(PolicysList[PolicyChoice - 1]["User"]);
                        json Policy = PolicysList[PolicyChoice - 1]["Policy"];
                        //Fucking Hell This Is A Fucking Damn Long Ass Line And I Hate It Because I Made It Even Longer
                        PolicyDetails(Admin, true, true, PolicyUser, false, false, PolicysList[PolicyChoice - 1]["PolicyNumber"], Policy["Registration"], Policy["Type"], Policy["Model"], Policy["Value"], Policy["Modifications"], Policy["ValueOfMods"], Policy["Financed"], Policy["Business"], Policy["Kms"], Policy["Address"], Policy["FirstName"], Policy["LastName"], Policy["DOB"], Policy["Gender"], Policy["Incidents"], Policy["Drivers"], Policy["Length"], true);
                    }
                }
                else {
                    cout << "This Input Cant Be Empty: ";
                    goto policysViewChoose;
                }
            }
            if (command == "edit policy" || command == "1") {
                string StringValue;
                int PolicyChoice;
            policysEditChoose:
                cout << "Please Enter The Number Of The Policy You Would Like To Edit: ";
                getline(cin, StringValue);
                if (!StringValue.empty()) {
                    if (StringValue.length() <= 6) {
                        for (int i = 0; i < StringValue.length(); i++) {
                            if (!isdigit(StringValue[i])) {
                                cout << "Please Enter A Valid Number: ";
                                goto policysEditChoose;
                            }
                        }
                        PolicyChoice = stoi(StringValue);

                        if (PolicyChoice - 1 > PolicysList.size()) {
                            cout << "Policy Number Cant Be More Than The Amount Of Policys: ";
                            goto policysEditChoose;
                        }
                        User PolicyUser = SearchForUser(PolicysList[PolicyChoice - 1]["User"]);
                        json Policy = PolicysList[PolicyChoice - 1]["Policy"];
                        //Fucking Hell This Is A Fucking Damn Long Ass Line
                        EditPolicy(Admin, true, PolicyUser, false, PolicysList[PolicyChoice - 1]["PolicyNumber"], Policy["Registration"], Policy["Type"], Policy["Model"], Policy["Value"], Policy["Modifications"], Policy["ValueOfMods"], Policy["Financed"], Policy["Business"], Policy["Kms"], Policy["Address"], Policy["FirstName"], Policy["LastName"], Policy["DOB"], Policy["Gender"], Policy["Incidents"], Policy["Drivers"], Policy["Length"]);
                    }
                }
                else {
                    cout << "This Input Cant Be Empty: ";
                    goto policysEditChoose;
                }
            }
            // So Much Hassle To Just Say !fuckoff To A Piece Of Shit
            if (command == "delete policy" || command == "2") {
                string StringValue;
                int PolicyChoice;
            policysDeleteChoose:
                cout << "Please Enter The Number Of The Policy You Would Like To Edit: ";
                getline(cin, StringValue);
                if (!StringValue.empty()) {
                    if (StringValue.length() <= 6) {
                        for (int i = 0; i < StringValue.length(); i++) {
                            if (!isdigit(StringValue[i])) {
                                cout << "Please Enter A Valid Number: ";
                                goto policysDeleteChoose;
                            }
                        }
                        PolicyChoice = stoi(StringValue);

                        if (PolicyChoice - 1 > PolicysList.size()) {
                            cout << "Policy Number Cant Be More Than The Amount Of Policys: ";
                            goto policysDeleteChoose;
                        }
                        User PolicyUser = SearchForUser(PolicysList[PolicyChoice - 1]["User"]);
                        json Policy = PolicysList[PolicyChoice - 1]["Policy"];
                        //Fucking Oath This Is Probably The Stupidest Way To Do This But Anyways For Doing This At 1am And Wanting To Sleep I Cant Give 2 Shits
                        json tempPolicys;
                        for (int i = 0; i < PolicyUser.Policies.size(); i++) {
                            if (i != PolicysList[PolicyChoice - 1]["PolicyNumber"]) {
                                tempPolicys.push_back(PolicyUser.Policies[i]);
                            }
                            else {

                            }
                        }
                        string fileLocation = "Files/" + PolicyUser.Email + ".json";
                        ofstream file;
                        file.open(fileLocation, ofstream::out | ofstream::trunc);

                        json User;
                        User["Email"] = PolicyUser.Email;
                        User["Username"] = PolicyUser.Username;
                        User["Password"] = PolicyUser.Password;
                        User["IsAdmin"] = PolicyUser.IsAdmin;
                        User["RootUser"] = PolicyUser.RootUser;
                        User["Banned"] = PolicyUser.Banned;
                        User["Policys"] = tempPolicys;

                        file << User;

                        file.close();

                        ViewAllPolicys(Admin);
                    }

                }
                else {
                    cout << "This Input Cant Be Empty: ";
                    goto policysDeleteChoose;
                }
            }
            if (command == "cancel policy" || command == "3") {
                string StringValue;
                int PolicyChoice;
            policysCancelChoose:
                cout << "Please Enter The Number Of A Current Active Policy You Would Like To Cancel: ";
                getline(cin, StringValue);
                if (!StringValue.empty()) {
                    if (StringValue.length() <= 6) {
                        for (int i = 0; i < StringValue.length(); i++) {
                            if (!isdigit(StringValue[i])) {
                                cout << "Please Enter A Valid Number: ";
                                goto policysCancelChoose;
                            }
                        }
                        PolicyChoice = stoi(StringValue);

                        if (PolicyChoice - 1 > PolicysList.size()) {
                            cout << "Policy Number Cant Be More Than The Amount Of Active Policys: ";
                            goto policysCancelChoose;
                        }
                        User PolicyUser = SearchForUser(PolicysList[PolicyChoice - 1]["User"]);
                        json Policy = PolicysList[PolicyChoice - 1]["Policy"];
                        //Fucking Hell This Is A Fucking Damn Long Ass Line And I Hate It Because I Made It Even Longer
                        PolicyDetails(Admin, true, false, PolicyUser, true, false, PolicysList[PolicyChoice - 1]["PolicyNumber"], Policy["Registration"], Policy["Type"], Policy["Model"], Policy["Value"], Policy["Modifications"], Policy["ValueOfMods"], Policy["Financed"], Policy["Business"], Policy["Kms"], Policy["Address"], Policy["FirstName"], Policy["LastName"], Policy["DOB"], Policy["Gender"], Policy["Incidents"], Policy["Drivers"], Policy["Length"], true);
                    }
                }
                else {
                    cout << "This Input Cant Be Empty: ";
                    goto policysCancelChoose;
                }
            }
            if (command == "back" || command == "4") {
                Menu(Admin, Admin.RootUser);
            }
            if (command == "quit" || command == "5") {
                exit(0);
            }
            else {
                cout << "Unknown Command. Valid Commands Are: View Policy (0) | Edit Policy (1) | Delete Policy (2) | Cancel Policy (3) | Back (4) | Quit (5)" << endl;
                goto viewAllCommandPolicysOnly;
            }
        }
        if (CanceledPolicysList.size() > 0 && PolicysList.size() == 0) {
            cout << "\nYour Current Options Are: View Policy (0) | Renew Policy (1) | Back (2) | Quit (3)" << endl;
        viewAllCanceledPolicysCommand:
            cout << ">";
            getline(cin, command);
            transform(command.begin(), command.end(), command.begin(), ::tolower);
            if (command == "view policy" || command == "0") {
                string StringValue;
                int PolicyChoice;
            policysViewChoose1:
                cout << "Please Enter The Number Of The Policy You Would Like To Edit: ";
                getline(cin, StringValue);
                if (!StringValue.empty()) {
                    if (StringValue.length() <= 6) {
                        for (int i = 0; i < StringValue.length(); i++) {
                            if (!isdigit(StringValue[i])) {
                                cout << "Please Enter A Valid Number: ";
                                goto policysViewChoose1;
                            }
                        }
                        PolicyChoice = stoi(StringValue);

                        if (PolicyChoice - 1 > CanceledPolicysList.size()) {
                            cout << "Policy Number Cant Be More Than The Amount Of Policys: ";
                            goto policysViewChoose1;
                        }
                        User PolicyUser = SearchForUser(CanceledPolicysList[PolicyChoice - 1]["User"]);
                        json Policy = CanceledPolicysList[PolicyChoice - 1]["Policy"];
                        //Fucking Hell This Is A Fucking Damn Long Ass Line And I Hate It Because I Made It Even Longer
                        PolicyDetails(Admin, true, true, PolicyUser, true, false, CanceledPolicysList[PolicyChoice - 1]["PolicyNumber"], Policy["Registration"], Policy["Type"], Policy["Model"], Policy["Value"], Policy["Modifications"], Policy["ValueOfMods"], Policy["Financed"], Policy["Business"], Policy["Kms"], Policy["Address"], Policy["FirstName"], Policy["LastName"], Policy["DOB"], Policy["Gender"], Policy["Incidents"], Policy["Drivers"], Policy["Length"], false);
                    }
                }
                else {
                    cout << "This Input Cant Be Empty: ";
                    goto policysViewChoose1;
                }
            }
            if (command == "renew policy" || command == "1") {
                string StringValue;
                int PolicyChoice;
            policysRenewChoose:
                cout << "Please Enter The Number Of A Current Canceled Policy You Would Like To Renew: ";
                getline(cin, StringValue);
                if (!StringValue.empty()) {
                    if (StringValue.length() <= 6) {
                        for (int i = 0; i < StringValue.length(); i++) {
                            if (!isdigit(StringValue[i])) {
                                cout << "Please Enter A Valid Number: ";
                                goto policysRenewChoose;
                            }
                        }
                        PolicyChoice = stoi(StringValue);

                        if (PolicyChoice - 1 > CanceledPolicysList.size()) {
                            cout << "Policy Number Cant Be More Than The Amount Of Canceled Policys: ";
                            goto policysRenewChoose;
                        }
                        User PolicyUser = SearchForUser(CanceledPolicysList[PolicyChoice - 1]["User"]);
                        json Policy = CanceledPolicysList[PolicyChoice - 1]["Policy"];
                        PolicyDetails(Admin, true, false, PolicyUser, true, false, CanceledPolicysList[PolicyChoice - 1]["PolicyNumber"], Policy["Registration"], Policy["Type"], Policy["Model"], Policy["Value"], Policy["Modifications"], Policy["ValueOfMods"], Policy["Financed"], Policy["Business"], Policy["Kms"], Policy["Address"], Policy["FirstName"], Policy["LastName"], Policy["DOB"], Policy["Gender"], Policy["Incidents"], Policy["Drivers"], Policy["Length"], false);
                    }
                }
                else {
                    cout << "This Input Cant Be Empty: ";
                    goto policysRenewChoose;
                }
            }
            if (command == "back" || command == "2") {
                Menu(Admin, Admin.RootUser);
            }
            if (command == "quit" || command == "3") {
                exit(0);
            }
            else {
                cout << "Your Current Options Are: View Policy (0) | Renew Policy (1) | Back (2) | Quit (3)" << endl;
                goto viewAllCanceledPolicysCommand;
            }
        }
        if (CanceledPolicysList.size() > 0 && PolicysList.size() > 0) {
            cout << "\nYour Current Options Are: View Policy (0) | View Canceled Policy (1) | Edit Policy (2) | Delete Policy (3) | Cancel Policy (4) | Renew Policy (5) | Back (6) | Quit (7)" << endl;
        viewallcommand:
            cout << ">";
            getline(cin, command);
            transform(command.begin(), command.end(), command.begin(), ::tolower);
            if (command == "view policy" || command == "0") {
                string StringValue;
                int PolicyChoice;
            policysViewChoose3:
                cout << "Please Enter The Number Of The Policy You Would Like To Edit: ";
                getline(cin, StringValue);
                if (!StringValue.empty()) {
                    if (StringValue.length() <= 6) {
                        for (int i = 0; i < StringValue.length(); i++) {
                            if (!isdigit(StringValue[i])) {
                                cout << "Please Enter A Valid Number: ";
                                goto policysViewChoose3;
                            }
                        }
                        PolicyChoice = stoi(StringValue);

                        if (PolicyChoice - 1 > CanceledPolicysList.size()) {
                            cout << "Policy Number Cant Be More Than The Amount Of Policys: ";
                            goto policysViewChoose3;
                        }
                        User PolicyUser = SearchForUser(CanceledPolicysList[PolicyChoice - 1]["User"]);
                        json Policy = CanceledPolicysList[PolicyChoice - 1]["Policy"];
                        //Fucking Hell This Is A Fucking Damn Long Ass Line And I Hate It Because I Made It Even Longer
                        PolicyDetails(Admin, true, true, PolicyUser, false, true, CanceledPolicysList[PolicyChoice - 1]["PolicyNumber"], Policy["Registration"], Policy["Type"], Policy["Model"], Policy["Value"], Policy["Modifications"], Policy["ValueOfMods"], Policy["Financed"], Policy["Business"], Policy["Kms"], Policy["Address"], Policy["FirstName"], Policy["LastName"], Policy["DOB"], Policy["Gender"], Policy["Incidents"], Policy["Drivers"], Policy["Length"], true);
                    }
                }
                else {
                    cout << "This Input Cant Be Empty: ";
                    goto policysViewChoose3;
                }
            }
            if (command == "view canceled policy" || command == "1") {
                string StringValue;
                int PolicyChoice;
            policysViewChoose4:
                cout << "Please Enter The Number Of The Canceled Policy You Would Like To View: ";
                getline(cin, StringValue);
                if (!StringValue.empty()) {
                    if (StringValue.length() <= 6) {
                        for (int i = 0; i < StringValue.length(); i++) {
                            if (!isdigit(StringValue[i])) {
                                cout << "Please Enter A Valid Number: ";
                                goto policysViewChoose4;
                            }
                        }
                        PolicyChoice = stoi(StringValue);

                        if (PolicyChoice - 1 > CanceledPolicysList.size()) {
                            cout << "Policy Number Cant Be More Than The Amount Of Policys: ";
                            goto policysViewChoose4;
                        }
                        User PolicyUser = SearchForUser(CanceledPolicysList[PolicyChoice - 1]["User"]);
                        json Policy = CanceledPolicysList[PolicyChoice - 1]["Policy"];
                        //Fucking Hell This Is A Fucking Damn Long Ass Line And I Hate It Because I Made It Even Longer
                        PolicyDetails(Admin, true, true, PolicyUser, true, false, CanceledPolicysList[PolicyChoice - 1]["PolicyNumber"], Policy["Registration"], Policy["Type"], Policy["Model"], Policy["Value"], Policy["Modifications"], Policy["ValueOfMods"], Policy["Financed"], Policy["Business"], Policy["Kms"], Policy["Address"], Policy["FirstName"], Policy["LastName"], Policy["DOB"], Policy["Gender"], Policy["Incidents"], Policy["Drivers"], Policy["Length"], false);
                    }
                }
                else {
                    cout << "This Input Cant Be Empty: ";
                    goto policysViewChoose4;
                }
            }
            if (command == "edit policy" || command == "2") {
                string StringValue;
                int PolicyChoice;
            policysEditChoose3:
                cout << "Please Enter The Number Of The Policy You Would Like To Edit: ";
                getline(cin, StringValue);
                if (!StringValue.empty()) {
                    if (StringValue.length() <= 6) {
                        for (int i = 0; i < StringValue.length(); i++) {
                            if (!isdigit(StringValue[i])) {
                                cout << "Please Enter A Valid Number: ";
                                goto policysEditChoose3;
                            }
                        }
                        PolicyChoice = stoi(StringValue);

                        if (PolicyChoice - 1 > PolicysList.size()) {
                            cout << "Policy Number Cant Be More Than The Amount Of Policys: ";
                            goto policysEditChoose3;
                        }
                        User PolicyUser = SearchForUser(PolicysList[PolicyChoice - 1]["User"]);
                        json Policy = PolicysList[PolicyChoice - 1]["Policy"];
                        //Fucking Hell This Is A Fucking Damn Long Ass Line
                        EditPolicy(Admin, true, PolicyUser, false, PolicysList[PolicyChoice - 1]["PolicyNumber"], Policy["Registration"], Policy["Type"], Policy["Model"], Policy["Value"], Policy["Modifications"], Policy["ValueOfMods"], Policy["Financed"], Policy["Business"], Policy["Kms"], Policy["Address"], Policy["FirstName"], Policy["LastName"], Policy["DOB"], Policy["Gender"], Policy["Incidents"], Policy["Drivers"], Policy["Length"]);
                    }
                }
                else {
                    cout << "This Input Cant Be Empty: ";
                    goto policysEditChoose3;
                }
            }
            if (command == "delete policy" || command == "3") {
                string StringValue;
                int PolicyChoice;
            policysDeleteChoose3:
                cout << "Please Enter The Number Of The Policy You Would Like To Edit: ";
                getline(cin, StringValue);
                if (!StringValue.empty()) {
                    if (StringValue.length() <= 6) {
                        for (int i = 0; i < StringValue.length(); i++) {
                            if (!isdigit(StringValue[i])) {
                                cout << "Please Enter A Valid Number: ";
                                goto policysDeleteChoose3;
                            }
                        }
                        PolicyChoice = stoi(StringValue);

                        if (PolicyChoice - 1 > PolicysList.size()) {
                            cout << "Policy Number Cant Be More Than The Amount Of Policys: ";
                            goto policysDeleteChoose3;
                        }
                        User PolicyUser = SearchForUser(PolicysList[PolicyChoice - 1]["User"]);
                        json Policy = PolicysList[PolicyChoice - 1]["Policy"];
                        //Fucking Oath This Is Probably The Stupidest Way To Do This But Anyways For Doing This At 1am And Wanting To Sleep I Cant Give 2 Shits
                        json tempPolicys;
                        for (int i = 0; i < PolicyUser.Policies.size(); i++) {
                            if (i != PolicyChoice - 1) {
                                tempPolicys.push_back(PolicyUser.Policies[i]);
                            }
                            else {

                            }
                        }
                        string fileLocation = "Files/" + PolicyUser.Email + ".json";
                        ofstream file;
                        file.open(fileLocation, ofstream::out | ofstream::trunc);

                        json User;
                        User["Email"] = PolicyUser.Email;
                        User["Username"] = PolicyUser.Username;
                        User["Password"] = PolicyUser.Password;
                        User["IsAdmin"] = PolicyUser.IsAdmin;
                        User["RootUser"] = PolicyUser.RootUser;
                        User["Banned"] = PolicyUser.Banned;
                        User["Policys"] = tempPolicys;

                        file << User;

                        file.close();

                        ViewAllPolicys(Admin);
                    }

                }
                else {
                    cout << "This Input Cant Be Empty: ";
                    goto policysDeleteChoose3;
                }
            }
            if (command == "cancel policy" || command == "4") {
                string StringValue;
                int PolicyChoice;
            policysCancelChoose3:
                cout << "Please Enter The Number Of A Current Active Policy You Would Like To Cancel: ";
                getline(cin, StringValue);
                if (!StringValue.empty()) {
                    if (StringValue.length() <= 6) {
                        for (int i = 0; i < StringValue.length(); i++) {
                            if (!isdigit(StringValue[i])) {
                                cout << "Please Enter A Valid Number: ";
                                goto policysCancelChoose3;
                            }
                        }
                        PolicyChoice = stoi(StringValue);

                        if (PolicyChoice - 1 > PolicysList.size()) {
                            cout << "Policy Number Cant Be More Than The Amount Of Active Policys: ";
                            goto policysCancelChoose3;
                        }
                        User PolicyUser = SearchForUser(PolicysList[PolicyChoice - 1]["User"]);
                        json Policy = PolicysList[PolicyChoice - 1]["Policy"];
                        //Fucking Hell This Is A Fucking Damn Long Ass Line And I Hate It Because I Made It Even Longer
                        PolicyDetails(Admin, true, false, PolicyUser, true, false, PolicysList[PolicyChoice - 1]["PolicyNumber"], Policy["Registration"], Policy["Type"], Policy["Model"], Policy["Value"], Policy["Modifications"], Policy["ValueOfMods"], Policy["Financed"], Policy["Business"], Policy["Kms"], Policy["Address"], Policy["FirstName"], Policy["LastName"], Policy["DOB"], Policy["Gender"], Policy["Incidents"], Policy["Drivers"], Policy["Length"], true);
                    }
                }
                else {
                    cout << "This Input Cant Be Empty: ";
                    goto policysCancelChoose3;
                }
            }
            if (command == "renew policy" || command == "5") {
                string StringValue;
                int PolicyChoice;
            policysRenewChoose3:
                cout << "Please Enter The Number Of A Current Canceled Policy You Would Like To Renew: ";
                getline(cin, StringValue);
                if (!StringValue.empty()) {
                    if (StringValue.length() <= 6) {
                        for (int i = 0; i < StringValue.length(); i++) {
                            if (!isdigit(StringValue[i])) {
                                cout << "Please Enter A Valid Number: ";
                                goto policysRenewChoose3;
                            }
                        }
                        PolicyChoice = stoi(StringValue);

                        if (PolicyChoice - 1 > CanceledPolicysList.size()) {
                            cout << "Policy Number Cant Be More Than The Amount Of Canceled Policys: ";
                            goto policysRenewChoose3;
                        }
                        User PolicyUser = SearchForUser(CanceledPolicysList[PolicyChoice - 1]["User"]);
                        json Policy = CanceledPolicysList[PolicyChoice - 1]["Policy"];
                        PolicyDetails(Admin, true, false, PolicyUser, true, false, CanceledPolicysList[PolicyChoice - 1]["PolicyNumber"], Policy["Registration"], Policy["Type"], Policy["Model"], Policy["Value"], Policy["Modifications"], Policy["ValueOfMods"], Policy["Financed"], Policy["Business"], Policy["Kms"], Policy["Address"], Policy["FirstName"], Policy["LastName"], Policy["DOB"], Policy["Gender"], Policy["Incidents"], Policy["Drivers"], Policy["Length"], false);
                    }
                }
                else {
                    cout << "This Input Cant Be Empty: ";
                    goto policysRenewChoose3;
                }
            }
            if (command == "back" || command == "6") {
                Menu(Admin, Admin.RootUser);
            }
            if (command == "quit" || command == "7") {
                exit(0);
            }
            else {
                cout << "Unknown Command. Valid Commands Are: View Policy (0) | Edit Policy (1) | Delete Policy (2) | Back (3) | Quit (4)" << endl;
                goto viewallcommand;
            }
        }
    }
}
// round 5 of saying my
void VehicleInsuranceSystem::ViewUser(User Admin, bool AdminEdit, User CurrentUser)
{
    system("CLS");
    cout << "======================================================================================================" << endl;
    cout << "                       Editing Current User: " << CurrentUser.Email << endl;
    cout << "======================================================================================================\n" << endl;    

    if (AdminEdit && Admin.RootUser) {
        cout << "\tEmail: " << CurrentUser.Email << endl;
        cout << "\tUsername: " << CurrentUser.Username << endl;
        cout << boolalpha;
        cout << "\tAdmin: " << CurrentUser.IsAdmin << endl;
        cout << "\tIs User Banned: " << CurrentUser.Banned << endl;
        
        if (!CurrentUser.Banned && CurrentUser.IsAdmin) {
            cout << "\nYour Current Commands Are: Demote Admin (0) | Ban Admin (1) | Back (2) | Quit (3)" << endl;
        }
        if (!CurrentUser.Banned && !CurrentUser.IsAdmin) {
            cout << "\nYour Current Commands Are: Promote User (0) | Ban User (1) | Back (2) | Quit (3)" << endl;
        }
        if (CurrentUser.Banned && CurrentUser.IsAdmin) {
            cout << "\nYour Current Commands Are: Demote Admin (0) | Unban Admin (1) | Back (2) | Quit (3)" << endl;
        }
        if (CurrentUser.Banned && !CurrentUser.IsAdmin) {
            cout << "\nYour Current Commands Are: Promote User (0) | Unban User (1) | Back (2) | Quit (3)" << endl;
        }
        
        string str;
    rootedit:
        cout << ">";
        getline(cin, str);
        if (!str.empty())
        {
            transform(str.begin(), str.end(), str.begin(), ::tolower);
            if (CurrentUser.IsAdmin) {
                if (str == "demote admin" || str == "1") {
                    cout << "Do You Want To Demote This Admin (Y | N): ";
                    string strValue;
                rootdemote:
                    getline(cin, strValue);
                    if (!strValue.empty()) {
                        transform(strValue.begin(), strValue.end(), strValue.begin(), ::tolower);
                        if (strValue == "n") {
                            ViewUser(Admin, true, CurrentUser);
                        }
                        if (strValue == "y") {

                            json temp = {
                                {"Email", CurrentUser.Email},
                                {"Username", CurrentUser.Username},
                                {"Password", CurrentUser.Password},
                                {"RootUser", CurrentUser.RootUser},
                                {"IsAdmin", false},
                                {"Banned", CurrentUser.Banned},
                                {"Policys", CurrentUser.Policies}
                            };

                            string filename = "Files/" + CurrentUser.Email + ".json";
                            ofstream UserFile(filename, ofstream::out | ofstream::trunc);

                            if (UserFile.is_open()) {
                                UserFile << temp;
                            }
                            UserFile.close();

                            CurrentUser = SearchForUser(temp["Email"]);
                            ViewUser(Admin, true, CurrentUser);
                        }
                        else {
                            cout << "Please Input A Valid Value (Y | N)" << endl;
                            goto rootdemote;
                        }
                    }
                    else {
                        cout << "The Value Can Not Be Empty" << endl;
                        goto rootdemote;
                    }
                }
                if (!CurrentUser.Banned) {
                    if (str == "ban admin" || str == "2") {
                        cout << "Do You Want To Ban This Admin (Y | N): ";
                        string strValue;
                    rootban:
                        getline(cin, strValue);
                        if (!strValue.empty()) {
                            transform(strValue.begin(), strValue.end(), strValue.begin(), ::tolower);
                            if (strValue == "n") {
                                ViewUser(Admin, true, CurrentUser);
                            }
                            if (strValue == "y") {

                                json temp = {
                                    {"Email", CurrentUser.Email},
                                    {"Username", CurrentUser.Username},
                                    {"Password", CurrentUser.Password},
                                    {"RootUser", CurrentUser.RootUser},
                                    {"IsAdmin", CurrentUser.IsAdmin},
                                    {"Banned", true},
                                    {"Policys", CurrentUser.Policies}
                                };

                                string filename = "Files/" + CurrentUser.Email + ".json";
                                ofstream UserFile(filename, ofstream::out | ofstream::trunc);

                                if (UserFile.is_open()) {
                                    UserFile << temp;
                                }
                                UserFile.close();

                                CurrentUser = SearchForUser(temp["Email"]);
                                ViewUser(Admin, true, CurrentUser);
                            }
                            else {
                                cout << "Please Input A Valid Value (Y | N)" << endl;
                                goto rootban;
                            }
                        }
                        else {
                            cout << "The Value Can Not Be Empty" << endl;
                            goto rootban;
                        }
                    }
                }
                else {
                    if (str == "unban admin" || str == "2") {
                        cout << "Do You Want To Ban This Admin (Y | N): ";
                        string strValue;
                    rootUnban:
                        getline(cin, strValue);
                        if (!strValue.empty()) {
                            transform(strValue.begin(), strValue.end(), strValue.begin(), ::tolower);
                            if (strValue == "n") {
                                ViewUser(Admin, true, CurrentUser);
                            }
                            if (strValue == "y") {

                                json temp = {
                                    {"Email", CurrentUser.Email},
                                    {"Username", CurrentUser.Username},
                                    {"Password", CurrentUser.Password},
                                    {"RootUser", CurrentUser.RootUser},
                                    {"IsAdmin", CurrentUser.IsAdmin},
                                    {"Banned", false},
                                    {"Policys", CurrentUser.Policies}
                                };

                                string filename = "Files/" + CurrentUser.Email + ".json";
                                ofstream UserFile(filename, ofstream::out | ofstream::trunc);

                                if (UserFile.is_open()) {
                                    UserFile << temp;
                                }
                                UserFile.close();

                                CurrentUser = SearchForUser(temp["Email"]);
                                ViewUser(Admin, true, CurrentUser);
                            }
                            else {
                                cout << "Please Input A Valid Value (Y | N)" << endl;
                                goto rootUnban;
                            }
                        }
                        else {
                            cout << "The Value Can Not Be Empty" << endl;
                            goto rootUnban;
                        }
                    }
                }
            }
            else {
                if (str == "promote admin" || str == "1") {
                    cout << "Do You Want To Promote This User (Y | N): ";
                    string strValue;
                adminpromote:
                    getline(cin, strValue);
                    if (!strValue.empty()) {
                        transform(strValue.begin(), strValue.end(), strValue.begin(), ::tolower);
                        if (strValue == "n") {
                            ViewUser(Admin, true, CurrentUser);
                        }
                        if (strValue == "y") {

                            json temp = {
                                {"Email", CurrentUser.Email},
                                {"Username", CurrentUser.Username},
                                {"Password", CurrentUser.Password},
                                {"RootUser", CurrentUser.RootUser},
                                {"IsAdmin", true},
                                {"Banned", CurrentUser.Banned},
                                {"Policys", CurrentUser.Policies}
                            };

                            string filename = "Files/" + CurrentUser.Email + ".json";
                            ofstream UserFile(filename, ofstream::out | ofstream::trunc);

                            if (UserFile.is_open()) {
                                UserFile << temp;
                            }
                            UserFile.close();

                            CurrentUser = SearchForUser(temp["Email"]);
                            ViewUser(Admin, true, CurrentUser);
                        }
                        else {
                            cout << "Please Input A Valid Value (Y | N)" << endl;
                            goto adminpromote;
                        }
                    }
                    else {
                        cout << "The Value Can Not Be Empty" << endl;
                        goto adminpromote;
                    }
                }
                if (!CurrentUser.Banned) {
                    if (str == "ban user" || str == "2") {
                        cout << "Do You Want To Ban This User (Y | N): ";
                        string strValue;
                    userban:
                        getline(cin, strValue);
                        if (!strValue.empty()) {
                            transform(strValue.begin(), strValue.end(), strValue.begin(), ::tolower);
                            if (strValue == "n") {
                                ViewUser(Admin, true, CurrentUser);
                            }
                            if (strValue == "y") {

                                json temp = {
                                    {"Email", CurrentUser.Email},
                                    {"Username", CurrentUser.Username},
                                    {"Password", CurrentUser.Password},
                                    {"RootUser", CurrentUser.RootUser},
                                    {"IsAdmin", CurrentUser.IsAdmin},
                                    {"Banned", true},
                                    {"Policys", CurrentUser.Policies}
                                };

                                string filename = "Files/" + CurrentUser.Email + ".json";
                                ofstream UserFile(filename, ofstream::out | ofstream::trunc);

                                if (UserFile.is_open()) {
                                    UserFile << temp;
                                }
                                UserFile.close();

                                CurrentUser = SearchForUser(temp["Email"]);
                                ViewUser(Admin, true, CurrentUser);
                            }
                            else {
                                cout << "Please Input A Valid Value (Y | N)" << endl;
                                goto userban;
                            }
                        }
                        else {
                            cout << "The Value Can Not Be Empty" << endl;
                            goto userban;
                        }
                    }
                }
                else {
                    if (str == "unban user" || str == "2") {
                        cout << "Do You Want To Ban This User (Y | N): ";
                        string strValue;
                    userUnban:
                        getline(cin, strValue);
                        if (!strValue.empty()) {
                            transform(strValue.begin(), strValue.end(), strValue.begin(), ::tolower);
                            if (strValue == "n") {
                                ViewUser(Admin, true, CurrentUser);
                            }
                            if (strValue == "y") {

                                json temp = {
                                    {"Email", CurrentUser.Email},
                                    {"Username", CurrentUser.Username},
                                    {"Password", CurrentUser.Password},
                                    {"RootUser", CurrentUser.RootUser},
                                    {"IsAdmin", CurrentUser.IsAdmin},
                                    {"Banned", false},
                                    {"Policys", CurrentUser.Policies}
                                };

                                string filename = "Files/" + CurrentUser.Email + ".json";
                                ofstream UserFile(filename, ofstream::out | ofstream::trunc);

                                if (UserFile.is_open()) {
                                    UserFile << temp;
                                }
                                UserFile.close();

                                CurrentUser = SearchForUser(temp["Email"]);
                                ViewUser(Admin, true, CurrentUser);
                            }
                            else {
                                cout << "Please Input A Valid Value (Y | N)" << endl;
                                goto userUnban;
                            }
                        }
                        else {
                            cout << "The Value Can Not Be Empty" << endl;
                            goto userUnban;
                        }
                    }
                }
            }
            if (str == "back" || str == "3") {
                ViewAllUsers(Admin);
            }
            if (str == "quit" || str == "4") {
                exit(0);
            }
        }
        else {
            cout << "This Input Cant Be Empty: ";
            goto rootedit;
        }
    }
    if (AdminEdit && !Admin.RootUser) {
        cout << "\tEmail: " << CurrentUser.Email << endl;
        cout << "\tUsername: " << CurrentUser.Username << endl;
        cout << boolalpha;
        cout << "\tIs User Banned: " << CurrentUser.Banned << endl;

        if (!CurrentUser.Banned) {
            cout << "\nYour Current Commands Are: Ban User (0) | Back (1) | Quit (2)" << endl;
        }
        if (CurrentUser.Banned) {
            cout << "\nYour Current Commands Are: Unban User (0) | Back (1) | Quit (2)" << endl;
        }

        string str;
    adminedit:
        cout << ">";
        getline(cin, str);
        transform(str.begin(), str.end(), str.begin(), ::tolower);
        if (!str.empty())
        {
            if (!CurrentUser.Banned) {
                if (str == "ban user" || str == "0") {
                    cout << "Do You Want To Ban This User (Y | N): ";
                    string strValue;
                adminUserban:
                    getline(cin, strValue);
                    if (!strValue.empty()) {
                        transform(strValue.begin(), strValue.end(), strValue.begin(), ::tolower);
                        if (strValue == "n") {
                            ViewUser(Admin, true, CurrentUser);
                        }
                        if (strValue == "y") {

                            json temp = {
                                {"Email", CurrentUser.Email},
                                {"Username", CurrentUser.Username},
                                {"Password", CurrentUser.Password},
                                {"RootUser", CurrentUser.RootUser},
                                {"IsAdmin", CurrentUser.IsAdmin},
                                {"Banned", true},
                                {"Policys", CurrentUser.Policies}
                            };

                            string filename = "Files/" + CurrentUser.Email + ".json";
                            ofstream UserFile(filename, ofstream::out | ofstream::trunc);

                            if (UserFile.is_open()) {
                                UserFile << temp;
                            }
                            UserFile.close();

                            CurrentUser = SearchForUser(temp["Email"]);
                            ViewUser(Admin, true, CurrentUser);
                        }
                        else {
                            cout << "Please Input A Valid Value (Y | N)" << endl;
                            goto adminUserban;
                        }
                    }
                    else {
                        cout << "The Value Can Not Be Empty" << endl;
                        goto adminUserban;
                    }
                }
            }
            else {
                if (str == "unban user" || str == "0") {
                    cout << "Do You Want To Ban This User (Y | N): ";
                    string strValue;
                adminUnban:
                    getline(cin, strValue);
                    if (!strValue.empty()) {
                        transform(strValue.begin(), strValue.end(), strValue.begin(), ::tolower);
                        if (strValue == "n") {
                            ViewUser(Admin, true, CurrentUser);
                        }
                        if (strValue == "y") {

                            json temp = {
                                {"Email", CurrentUser.Email},
                                {"Username", CurrentUser.Username},
                                {"Password", CurrentUser.Password},
                                {"RootUser", CurrentUser.RootUser},
                                {"IsAdmin", CurrentUser.IsAdmin},
                                {"Banned", false},
                                {"Policys", CurrentUser.Policies}
                            };

                            string filename = "Files/" + CurrentUser.Email + ".json";
                            ofstream UserFile(filename, ofstream::out | ofstream::trunc);

                            if (UserFile.is_open()) {
                                UserFile << temp;
                            }
                            UserFile.close();

                            CurrentUser = SearchForUser(temp["Email"]);
                            ViewUser(Admin, true, CurrentUser);
                        }
                        else {
                            cout << "Please Input A Valid Value (Y | N)" << endl;
                            goto adminUnban;
                        }
                    }
                    else {
                        cout << "The Value Can Not Be Empty" << endl;
                        goto adminUnban;
                    }
                }
            }
            if (str == "back" || str == "1") {
                ViewAllUsers(Admin);
            }
            if (str == "quit" || str == "2") {
                exit(0);
            }
            else {
                cout << "Unknown Command: ";
                goto adminedit;
            }
        }
        else {
            cout << "This Input Cant Be Empty: ";
            goto adminedit;
        }
    }
}
// round 4 Of speedrunning life
void VehicleInsuranceSystem::ViewAllUsers(User Admin)
{
    system("CLS");
    if (Admin.RootUser) {

        string UsersFile = "Files/AllUsers.json";
        ifstream file(UsersFile);

        if (!file.fail()) {
            json j;

            file >> j;

            json AdminsList;
            json UserList;
            cout << "\t\t\t\t====[ Admins ]====" << endl;
            for (int i = 0; i < j.size(); i++)
            {
                User CurrentUser = SearchForUser(j[i]["User"]);
                if (!CurrentUser.RootUser) {
                    if (CurrentUser.IsAdmin) {
                        json k = {
                            { "Email", CurrentUser.Email}
                        };
                        cout << boolalpha;
                        cout << "   (" << (AdminsList.size() + 1) << ") Email: " << CurrentUser.Email << " Username: " << CurrentUser.Username + " Is User Banned: " << CurrentUser.Banned << " Current Status: Admin" << endl;
                        AdminsList.push_back(k);
                    }
                    else {

                    }
                }
            }
            if (AdminsList.size() == 0) {
                cout << "\nThere Are No Admins" << endl;
            }
            cout << "\n\t\t\t\t====[ Users ]====" << endl;
            for (int i = 0; i < j.size(); i++) {
                User CurrentUser = SearchForUser(j[i]["User"]);
                if (!CurrentUser.RootUser) {
                    if (!CurrentUser.IsAdmin) {
                        json k = {
                            { "Email", CurrentUser.Email}
                        };
                        cout << boolalpha;
                        cout << "   (" << (UserList.size() + 1) << ") Email: " << CurrentUser.Email << " Username: " << CurrentUser.Username + " Is User Banned: " << CurrentUser.Banned << " Current Status: User" << endl;
                        UserList.push_back(k);
                    }
                    else {

                    }
                }
            }
            if (UserList.size() == 0) {
                cout << "\n\nThere Are No Users" << endl;
            }

            if (AdminsList.size() == 0 && UserList.size() == 0) {
                Menu(Admin, Admin.RootUser);
            }
            if (AdminsList.size() > 0 && UserList.size() > 0) {
                cout << "Your Current Options Are: View Admin (0) | View User (1) | Back (2) | Quit (3)" << endl;
                string command;
            viewAllUsers:
                cout << ">";
                getline(cin, command);
                transform(command.begin(), command.end(), command.begin(), ::tolower);
                if (!command.empty()) {
                    if (command == "view admin" || command == "0") {
                        string str;
                        int AdminChoice;
                        cout << "\nPlease Pick A Admin That You Would Like To View: ";
                    adminsViewChoose:
                        getline(cin, str);
                        if (!str.empty()) {
                            if (str.length() <= 6) {
                                for (int i = 0; i < str.length(); i++) {
                                    if (!isdigit(str[i])) {
                                        cout << "Please Enter A Valid Number: ";
                                        goto adminsViewChoose;
                                    }
                                }
                                AdminChoice = stoi(str);

                                if (AdminChoice - 1 > AdminsList.size()) {
                                    cout << "Number Cant Be More Than The Amount Of Admins: ";
                                    goto adminsViewChoose;
                                }
                                User CurrentUser = SearchForUser(AdminsList[AdminChoice - 1]["Email"]);
                                ViewUser(Admin, true, CurrentUser);
                            }
                        }
                    }
                    if (command == "view user" || command == "1") {
                        string str;
                        int UserChoice;
                        cout << "\nPlease Pick A Admin That You Would Like To View: ";
                    usersViewChoose:
                        getline(cin, str);
                        if (!str.empty()) {
                            if (str.length() <= 6) {
                                for (int i = 0; i < str.length(); i++) {
                                    if (!isdigit(str[i])) {
                                        cout << "Please Enter A Valid Number: ";
                                        goto usersViewChoose;
                                    }
                                }
                                UserChoice = stoi(str);

                                if (UserChoice - 1 > AdminsList.size()) {
                                    cout << "Number Cant Be More Than The Amount Of Admins: ";
                                    goto usersViewChoose;
                                }
                                User CurrentUser = SearchForUser(UserList[UserChoice - 1]["Email"]);
                                ViewUser(Admin, true, CurrentUser);
                            }
                        }
                    }
                    if (command == "back" || command == "2") {
                        Menu(Admin, Admin.RootUser);
                    }
                    if (command == "quit" || command == "3") {
                        exit(0);
                    }
                    else {
                        cout << "Unknown Command: ";
                        goto viewAllUsers;
                    }
                }
                else {
                    cout << "This Input Cant Be Empty: ";
                    goto viewAllUsers;
                }
            }
            if (AdminsList.size() > 0 && UserList.size() == 0) {
                cout << "Your Current Options Are: View Admin (0) | Back (1) | Quit (2)" << endl;
                string command;
            viewAllAdmins:
                cout << ">";
                getline(cin, command);
                transform(command.begin(), command.end(), command.begin(), ::tolower);
                if (!command.empty()) {
                    if (command == "view admin" || command == "0") {
                        string str;
                        int AdminChoice;
                        cout << "\nPlease Pick A Admin That You Would Like To View: ";
                    adminsViewChoose2:
                        getline(cin, str);
                        if (!str.empty()) {
                            if (str.length() <= 6) {
                                for (int i = 0; i < str.length(); i++) {
                                    if (!isdigit(str[i])) {
                                        cout << "Please Enter A Valid Number: ";
                                        goto adminsViewChoose2;
                                    }
                                }
                                AdminChoice = stoi(str);

                                if (AdminChoice - 1 > AdminsList.size()) {
                                    cout << "Number Cant Be More Than The Amount Of Admins: ";
                                    goto adminsViewChoose2;
                                }
                                User CurrentUser = SearchForUser(AdminsList[AdminChoice - 1]["Email"]);
                                ViewUser(Admin, true, CurrentUser);
                            }
                        }
                    }
                    if (command == "back" || command == "1") {
                        Menu(Admin, Admin.RootUser);
                    }
                    if (command == "quit" || command == "2") {
                        exit(0);
                    }
                    else {
                        cout << "Unknown Command: ";
                        goto viewAllAdmins;
                    }
                }
                if (AdminsList.size() == 0 && UserList.size() > 0) {
                    cout << "\nYour Current Options Are: View User (0) | Back (1) | Quit (2)" << endl;
                    string command;
                adminViewAllUsers2:
                    cout << ">";
                    getline(cin, command);
                    transform(command.begin(), command.end(), command.begin(), ::tolower);
                    if (!command.empty()) {
                        if (command == "view user" || command == "0") {
                            string str;
                            int UserChoice;
                            cout << "\nPlease Pick A User That You Would Like To View: ";
                        usersViewChoose3:
                            getline(cin, str);
                            if (!str.empty()) {
                                if (str.length() <= 6) {
                                    for (int i = 0; i < str.length(); i++) {
                                        if (!isdigit(str[i])) {
                                            cout << "Please Enter A Valid Number: ";
                                            goto usersViewChoose3;
                                        }
                                    }
                                    UserChoice = stoi(str);

                                    if (UserChoice - 1 > UserList.size()) {
                                        cout << "Number Cant Be More Than The Amount Of Users: ";
                                        goto usersViewChoose3;
                                    }
                                    User CurrentUser = SearchForUser(UserList[UserChoice - 1]["Email"]);
                                    ViewUser(Admin, true, CurrentUser);
                                }
                            }
                        }
                        if (command == "back" || command == "1") {
                            Menu(Admin, Admin.RootUser);
                        }
                        if (command == "quit" || command == "2") {
                            exit(0);
                        }
                        else {
                            cout << "Unknown Command: ";
                            goto adminViewAllUsers2;
                        }
                    }
                    else {
                        cout << "This Input Cant Be Empty: ";
                        goto adminViewAllUsers2;
                    }
                }
            }
        }
        

    }
    if (Admin.IsAdmin && !Admin.RootUser) {

        string UsersFile = "Files/AllUsers.json";
        ifstream file(UsersFile);

        if (!file.fail()) {
            json j;

            file >> j;

            json UserList;
            cout << "\t\t\t\t====[ Users ]====" << endl;
            for (int i = 0; i < j.size(); i++) {
                User CurrentUser = SearchForUser(j[i]["User"]);
                if (!CurrentUser.RootUser) {
                    if (!CurrentUser.IsAdmin) {
                        json k = {
                            { "Email", CurrentUser.Email}
                        };
                        cout << boolalpha;
                        cout << "   (" << (UserList.size() + 1) << ") Email: " << CurrentUser.Email << " Username: " << CurrentUser.Username + " Is User Banned: " << CurrentUser.Banned << " Current Status: User" << endl;
                        UserList.push_back(k);
                    }
                    else {

                    }
                }
            }
            if (UserList.size() == 0) {
                Menu(Admin, Admin.RootUser);
            }

            cout << "\nYour Current Options Are: View User (0) | Back (1) | Quit (2)" << endl;
            string command;
        adminViewAllUsers:
            cout << ">";
            getline(cin, command);
            transform(command.begin(), command.end(), command.begin(), ::tolower);
            if (!command.empty()) {
                if (command == "view user" || command == "0") {
                    string str;
                    int UserChoice;
                    cout << "\nPlease Pick A User That You Would Like To View: ";
                usersViewChoose2:
                    getline(cin, str);
                    if (!str.empty()) {
                        if (str.length() <= 6) {
                            for (int i = 0; i < str.length(); i++) {
                                if (!isdigit(str[i])) {
                                    cout << "Please Enter A Valid Number: ";
                                    goto usersViewChoose2;
                                }
                            }
                            UserChoice = stoi(str);

                            if (UserChoice - 1 > UserList.size()) {
                                cout << "Number Cant Be More Than The Amount Of Users: ";
                                goto usersViewChoose2;
                            }
                            User CurrentUser = SearchForUser(UserList[UserChoice - 1]["Email"]);
                            ViewUser(Admin, true, CurrentUser);
                        }
                    }
                }
                if (command == "back" || command == "1") {
                    Menu(Admin, Admin.RootUser);
                }
                if (command == "quit" || command == "2") {
                    exit(0);
                }
                else {
                    cout << "Unknown Command: ";
                    goto adminViewAllUsers;
                }
            }
            else
            {
                cout << "This Input Cant Be Empty: ";
                goto adminViewAllUsers;
            }
        }
    }
}
void VehicleInsuranceSystem::Menu(User CurrentUser, bool RootUser) {
    SetConsoleTitle(TEXT("398 Studios Insurance | Menu"));
    system("CLS");
    bool menu = true;
    cout << "======================================================================================================" << endl;
    cout << "                  Welcome Back " << CurrentUser.Username << " To 398 Studios Insurance" << endl;
    cout << "======================================================================================================\n" << endl;

    User FakeUser = User("", "", "", false, false, true, "");

    if (!CurrentUser.IsAdmin) {
        cout << "Welcome. Your Current Commands Are: Manage Personal Policys (0) | New Policy (1) | Logout (3) | Quit (4)" << endl;
        while (menu) {
            string command;
            cout << ">";
            getline(cin, command);
            transform(command.begin(), command.end(), command.begin(), ::tolower);
            if (command == "manage personal policys" || command == "0")
            {
                ViewPolicys(CurrentUser);
            }
            if (command == "new policy" || command == "1")
            {
                NewPolicy(FakeUser, false, CurrentUser, CurrentUser.Policies.size() + 1);
            }
            if (command == "logout" || command == "3") {
                Login();
            }
            else if (command == "quit" || command == "4") {
                exit(0);
            }
            else
            {
                cout << "Unknown Command:\n";
            }
        }
    }
    else if (CurrentUser.RootUser) {
        cout << "Welcome. Your Current Commands Are: Manage All Policies (0) | Manage Personal Policys (1) |  New Policy (2) | Manage All Users (3) | Logout (4) | Quit (5)" << endl;
        while (menu) {
            string command;
            cout << ">";
            getline(cin, command);
            transform(command.begin(), command.end(), command.begin(), ::tolower);
            if (command == "manage all policies" || command == "0")
            {
                ViewAllPolicys(CurrentUser);
            }
            if (command == "manage personal policies" || command == "1")
            {
                ViewPolicys(CurrentUser);
            }
            if (command == "new policy" || command == "2")
            {
                NewPolicy(FakeUser, false, CurrentUser, CurrentUser.Policies.size() + 1);
            }
            if (command == "manage all users" || command == "3")
            {
                ViewAllUsers(CurrentUser);
            }
            if (command == "logout" || command == "4") {
                Login();
            }
            else if (command == "quit" || command == "5") {
                exit(0);
            }
            else
            {
                cout << "Unknown Command:\n";
            }
        }
    }
    else {        
        cout << "Welcome. Your Current Commands Are: Manage All Policies (0) | Manage Personal Policys (1) |  New Policy (2) | Manage All Users (3) | Logout (4) | Quit (5)" << endl;
        while (menu) {
            string command;
            cout << ">";
            getline(cin, command);
            transform(command.begin(), command.end(), command.begin(), ::tolower);
            if (command == "manage all policies" || command == "0")
            {
                ViewAllPolicys(CurrentUser);
            }
            if (command == "manage personal policies" || command == "1")
            {
                ViewPolicys(CurrentUser);
            }
            if (command == "new policy" || command == "2")
            {
                NewPolicy(FakeUser, false, CurrentUser, CurrentUser.Policies.size() + 1);
            }
            if (command == "manage all users" || command == "3")
            {
                ViewAllUsers(CurrentUser);
            }
            if (command == "logout" || command == "4") {
                Login();
            }
            else if (command == "quit" || command == "5") {
                exit(0);
            }
            else
            {
                cout << "Unknown Command:\n";
            }
        }
    }
}
void VehicleInsuranceSystem::Login()
{
    system("CLS");
    cout << "======================================================================================================" << endl;
    cout << "                                   Welcome To 398 Studios Insurance" << endl;
    cout << "======================================================================================================\n" << endl;
    bool startup = true;

    while (startup)
    {
        User CurrentUser = User("", "", "", false, false, true, "");

        SetConsoleTitle(TEXT("398 Studios Insurance | Launch Menu"));
        string command;
        cout << "Welcome. Your Current Commands Are: Login (0) | Signup (1) | Quit (2)" << endl;
        cout << ">";
        getline(cin, command);
        transform(command.begin(), command.end(), command.begin(), ::tolower);
        //===========================================================================================================================================================
        //                                                                  Login System
        //===========================================================================================================================================================
        if (command == "login" || command == "0")
        {
            SetConsoleTitle(TEXT("398 Studios | Login"));
            system("CLS");

        login:
            string Email;
            cout << "Please The Email Address For Your Account: ";
            getline(cin, Email);
            // ----------------------------- Root User Account
            if (Email == "system" || Email == "System" || Email == "xboxcreepersplays@gmail.com" || Email == "Rage") {
                system("CLS");
                string AuthCode;
                cout << "Sorry I Cannot Log You In To The System Account Without Other Authencation.";
                cout << "\nPlease Enter The Authencation Code: ";
                getline(cin, AuthCode);

                if (AuthCode == "398Studios2021") {
                    system("CLS");
                    CurrentUser = SearchForUser("xboxcreepersplays@gmail.com");
                    Menu(CurrentUser, CurrentUser.RootUser);
                }
                else {
                    system("CLS");
                    goto login;
                }
            }
            if (Email.length() < 1) {
                cout << "Please Enter A Valid Email Longer Than 1 Characters" << endl;
                goto login;
            }
            // ----------------------------- Password Check
            else {
                string Password;
                cout << "\nPlease Enter Your Password: ";
                getline(cin, Password);

                CurrentUser = SearchForUser(Email);

                if (CurrentUser.Email == Email && CurrentUser.Password == Password) {
                    system("CLS");
                    if (!CurrentUser.Banned) {
                        Menu(CurrentUser, CurrentUser.RootUser);
                    }
                    else {
                        cout << "=================================================================================================================================" << endl;
                        cout << "You Account Has Been Banned From The 398 Studios Vehicle Insurance System. Please Contact An Admin To Get This Fixed" << endl;
                        cout << "=================================================================================================================================\n\n\n" << endl;
                        exit(0);
                    }
                }
                else if (CurrentUser.Username == "InvalidUser") {
                    system("CLS");
                    cout << "Sorry An Account With This Email Does Not Exist\n";
                    goto login;
                }
                else if (CurrentUser.Password != Password) {
                    int attempts = 1;
                incorrectpass:
                    cout << "Incorrect Password Please Try Again: ";
                    getline(cin, Password);
                    if (CurrentUser.Password != Password) {
                        attempts++;
                        if (attempts == 4) {
                            system("CLS");
                            cout << "You Have Had Too Many Incorrect Attempts At Logging In. Please Restart The Program\n\n\n\n";
                            startup = false;
                        }
                        else {
                            goto incorrectpass;
                        }
                    }
                    else {
                        if (!CurrentUser.Banned) {
                            Menu(CurrentUser, CurrentUser.RootUser);
                        }
                        else {
                            cout << "You Account Has Been Banned From The 398 Studios Vehicle Insurance System. Please Contact An Admin To Get This Fixed\n\n\n";
                            startup = false;
                        }
                    }
                }
            }
        }

        if (command == "rootlogin" || command == "398") {
            system("CLS");
            CurrentUser = SearchForUser("xboxcreepersplays@gmail.com");
            Menu(CurrentUser, true);
        }
        //===========================================================================================================================================================
        //                                                                  Login System
        //===========================================================================================================================================================
        if (command == "signup" || command == "1")
        {
            NewUser();
        }
        if (command == "quit" || command == "2")
        {
            exit(0);
        }
    }
}
void VehicleInsuranceSystem::NewUser()
{
    User CurrentUser = User("", "", "", false, false, true, "");
    
    system("CLS");
    SetConsoleTitle(TEXT("398 Studios Insurance | Registration"));
    cout << "======================================================================================================" << endl;
    cout << "                             Welcome To 398 Studios Insurance" << endl;
    cout << "======================================================================================================\n" << endl;
    cout << "Please Enter The Email For This Account (N To Leave): ";
    string Email;
regEmail:
    getline(cin, Email);
    if (Email == "N" || Email == "n") {
        Login();
    }
    while (Email.length() < 3) {
        cout << "Please Enter A Email Longer Than 3 Chars: ";
        getline(cin, Email);
    }
    regEmailCheck:
    string UserFileString = "Files/" + Email + ".json";
    ifstream UserFile(UserFileString);
    if (!UserFile.fail()) {
        string StringValue;
        cout << "An Account With This Email Already Exists. Please Enter A Valid Account :";
        getline(cin, Email);
        goto regEmailCheck;
    }

    cout << "Please Enter The Username For This Account: ";
    string Username;
    getline(cin, Username);
    while (Username.length() < 3) {
        cout << "Please Enter A Username Longer Than 3 Chars: ";
        getline(cin, Username);
    }

    cout << "Please Enter A Password (MIN 6 Chars): ";
    string Password;
    getline(cin, Password);
    while (Password.length() < 6) {
        cout << "Please Enter A Password Longer Than 6 Chars: ";
        getline(cin, Password);
    }

    cout << "Would You Like To Signup For The Account With The Email: " + Email + " (Y | N): ";
    string StringValue;
regConfirm:
    getline(cin, StringValue);
    if (!StringValue.empty()) {
        transform(StringValue.begin(), StringValue.end(), StringValue.begin(), ::tolower);
        if (StringValue == "y") {
            string UserFileString = "Files/" + Email + ".json";
            ofstream UserFile(UserFileString);

            if (UserFile.is_open())
            {
                json User;
                User["Email"] = Email;
                User["Username"] = Username;
                User["Password"] = Password;
                User["IsAdmin"] = false;
                User["RootUser"] = false;
                User["Banned"] = false;
                User["Policys"] = {};

                UserFile << User;                
            }
            UserFile.close();
            ifstream GetUsers("Files/AllUsers.json");
            
            if (!GetUsers.fail()) {
                json j;
                GetUsers >> j;
                json j2 = {
                    {"User", Email}
                };
                j.push_back(j2);

                ofstream ToFile("Files/AllUsers.json", ofstream::out | ofstream::trunc);
                if (!ToFile.fail()) {
                    ToFile << j;
                }                
            }
            else {
                ofstream NewFile("Files/AllUsers.json");
                if (!UserFile.fail())
                {
                    json k;
                    json n = { {"User", Email} };
                    k.push_back(n);
                    NewFile << k;
                }
            }
            CurrentUser = SearchForUser(Email);
            Menu(CurrentUser, CurrentUser.RootUser);
        }
        if (StringValue == "n") {
            system("CLS");
            exit(0);
        }
        else {
            cout << "Please Enter A Valid Input (Y | N): ";
            goto regConfirm;
        }
    }
    else {
        cout << "This Input Cant Be Empty: ";
        goto regConfirm;
    }
}
// 3000 Lines Baby