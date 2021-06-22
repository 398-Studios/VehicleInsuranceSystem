#include <iostream>
#include <filesystem>
#include <string>
#include <stdlib.h>
#include <locale>
#include <Windows.h>
#include <vector>
#include <numeric> 
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <filesystem>
#include <time.h> 
#include <stdio.h>
#include <algorithm>
#include "nlohmann/json.hpp"

using namespace std;

using json = nlohmann::json;

class User {
public:
    User(string, string, string, bool, bool, bool, json);
    string Email;
    string Username;
    string Password;
    bool IsAdmin;
    bool RootUser;
    bool Banned;
    json Policies;
};

User::User(string Email1, string Username1, string Password1, bool Admin, bool Root, bool IsBanned, json Policies1) {
    Email = Email1;
    Username = Username1;
    Password = Password1;
    IsAdmin = Admin;
    RootUser = Root;
    Banned = IsBanned;
    Policies = Policies1;
}

class VehicleInsuranceSystem {
public:
    // Love Re-Coding This Section Like 5 Times
	static void ConfirmPolicy(User Admin, bool AdminEdit, User CurrentUser, int PolicyNumber, string Plate, string Type, string Model, float Value, bool Modifications, float ValueOfMods, bool Financed, bool Business, int Kms, string Address, string FirstName, string LastName, string DOB, string Gender, int Incidents, int Drivers, int Length, float Cost, bool Status);

    // Nice Simple Piece Of Code Atlease
    static void PolicyDetails(User Admin, bool AdminEdit, bool AdminView, User CurrentUser, bool IsCanceled, bool IsNewQuote, int PolicyNumber, string Plate, string Type, string Model, float Value, bool Modifications, float ValueOfMods, bool Financed, bool Business, int Kms, string Address, string FirstName, string LastName, string DOB, string Gender, int Incidents, int Drivers, int Length, bool PolicyStatus);

    // I Love Basically Coping A Whole Piece Of Code Then Having To Plow More Fucking Hours Into It And Then Deciding To Remake The Whole Fucking System.
    static void EditPolicy(User Admin, bool AdminEdit, User CurrentUser, bool IsNewQuote, int PolicyNumber, string Plate, string Type, string Model, float Value, bool Modifications, float ValueOfMods, bool Financed, bool Business, int Kms, string Address, string FirstName, string LastName, string DOB, string Gender, int Incidents, int Drivers, int Length);

    // This Stupid Piece Of Shit Code Was Only Ment To Be Sub 100 Lines But Its Now 300+ Lines And Took Way Too Fucking Long
    static void NewPolicy(User Admin, bool AdminEdit, User CurrentUser, int PolicyNumber);
    
    static void ViewPolicys(User CurrentUser);

    //Time For Round 2
    static void ViewAllPolicys(User Admin);

    //Why Oh Why Do I Do This Stuff
    static void ViewUser(User Admin, bool AdminEdit, User CurrentUser);
    
    static void ViewAllUsers(User Admin);

    //Stupid Thing
    static void Menu(User CurrentUser, bool RootUser);
    
    // Simple Little Piece Of Code But Still Retarded
    static void Login();

    // This Crashed Me Out Like 10 Times And I Fucking Hate It
    static void NewUser();
};