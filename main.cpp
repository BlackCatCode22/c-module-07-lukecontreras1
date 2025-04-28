//
//  main.cpp
//  module7
//
//  Created by Luke Contreras on 4/26/25.
//


#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <iomanip>
#include <cctype>
using namespace std;

// Constant arrival date (for all animals)
const string ARRIVAL_DATE = "2024-03-05"; // YYYY-MM-DD
const int ARRIVAL_YEAR = 2024;


// Given age and season, generate a birthday in ISO 8601 format (YYYY-MM-DD).
// For simplicity, we assume fixed birth day/month based on season:
// spring -> "03-15", summer -> "06-15", fall -> "09-15", winter -> "12-15".
string genBirthDay(int age, const string& season) {
    int birthYear = ARRIVAL_YEAR - age;
    string monthDay;
    if (season == "spring")
        monthDay = "03-15";
    else if (season == "summer")
        monthDay = "06-15";
    else if (season == "fall")
        monthDay = "09-15";
    else if (season == "winter")
        monthDay = "12-15";
    else  // If season not provided, default to March 15
        monthDay = "03-15";

    return to_string(birthYear) + "-" + monthDay;
}

// Given species and a counter, generate a unique ID.
// Unique ID format: first two letters of species (capitalized) + two-digit sequential number.
string genUniqueID(const string& species, int count) {
    string id;
    if (species.length() >= 2) {
        id.push_back(toupper(species[0]));
        id.push_back(toupper(species[1]));
    } else {
        // If species has less than two letters, pad with 'X'
        id.push_back(toupper(species[0]));
        id.push_back('X');
    }
    // Format the count as two digits, with leading zero if needed.
    ostringstream oss;
    oss << setw(2) << setfill('0') << count;
    id += oss.str();
    return id;
}

// Trim whitespace from both ends of a string.
string trim(const string& str) {
    const char* whitespace = " \t\n\r";
    size_t start = str.find_first_not_of(whitespace);
    if (start == string::npos)
        return "";
    size_t end = str.find_last_not_of(whitespace);
    return str.substr(start, end - start + 1);
}



// Base Animal class
class Animal {
protected:
    string uniqueID;
    string name;
    int age;
    string sex;
    string species;
    string color;
    int weight; // in pounds
    string birthDate; // ISO 8601 format
    string origin;
public:
    Animal(int age, const string& sex, const string& species, const string& color, int weight, const string& origin)
        : age(age), sex(sex), species(species), color(color), weight(weight), origin(origin) {
        // birthDate and uniqueID are generated later.
        birthDate = "";
        uniqueID = "";
        name = "";
    }
    virtual ~Animal() {}

    // Setters
    void setBirthDate(const string& bd) { birthDate = bd; }
    void setUniqueID(const string& id) { uniqueID = id; }
    void setName(const string& n) { name = n; }

    // Getters
    string getUniqueID() const { return uniqueID; }
    string getName() const { return name; }
    int getAge() const { return age; }
    string getSex() const { return sex; }
    string getSpecies() const { return species; }
    string getColor() const { return color; }
    int getWeight() const { return weight; }
    string getBirthDate() const { return birthDate; }
    string getOrigin() const { return origin; }

    // Virtual function to get habitat information; subclasses may override if needed.
    virtual string getHabitat() const {
        // For simplicity, assume habitat is based on species.
        return species + " Habitat";
    }
};

// Subclasses for specific species.
// For this project, they inherit everything from Animal and can be extended if needed.

class Hyena : public Animal {
public:
    Hyena(int age, const string& sex, const string& color, int weight, const string& origin)
        : Animal(age, sex, "hyena", color, weight, origin) {}
};

class Lion : public Animal {
public:
    Lion(int age, const string& sex, const string& color, int weight, const string& origin)
        : Animal(age, sex, "lion", color, weight, origin) {}
};

class Tiger : public Animal {
public:
    Tiger(int age, const string& sex, const string& color, int weight, const string& origin)
        : Animal(age, sex, "tiger", color, weight, origin) {}
};

class Bear : public Animal {
public:
    Bear(int age, const string& sex, const string& color, int weight, const string& origin)
        : Animal(age, sex, "bear", color, weight, origin) {}
};


// Main Function

int main() {
    vector<Animal*> zooAnimals;
    vector<string> availableNames;
    map<string, int> speciesCounter; // For unique ID generation per species

    
    ifstream namesFile("animalNames.txt");
    if (!namesFile) {
        cerr << "Error: Unable to open animalNames.txt" << endl;
        return 1;
    }
    string nameLine;
    while (getline(namesFile, nameLine)) {
        if (!nameLine.empty())
            availableNames.push_back(trim(nameLine));
    }
    namesFile.close();
    
    
    ifstream arrivingFile("arrivingAnimals.txt");
    if (!arrivingFile) {
        cerr << "Error: Unable to open arrivingAnimals.txt" << endl;
        return 1;
    }
    
    // Each line expected format:
    // "4 year old female hyena, born in spring, tan color, 70 pounds, from Friguia Park, Tunisia"
    while (getline(arrivingFile, nameLine)) {
        if (nameLine.empty())
            continue;
        
        // Split line by commas
        vector<string> tokens;
        istringstream iss(nameLine);
        string token;
        while(getline(iss, token, ',')) {
            tokens.push_back(trim(token));
        }
        if (tokens.size() < 5) {
            cerr << "Warning: Incomplete data in line: " << nameLine << endl;
            continue;
        }
      
        
        // Parse token[0]: extract age, sex, species.
        istringstream iss0(tokens[0]);
        int age;
        string dummy, sex, species;
        iss0 >> age;  // age: 4
        iss0 >> dummy; // "year"
        iss0 >> dummy; // "old"
        iss0 >> sex;   // e.g., "female"
        iss0 >> species; // e.g., "hyena"
        
        // Parse token[1] for season.
        string season;
        {
            istringstream iss1(tokens[1]); // "born in spring"
            iss1 >> dummy; // "born"
            iss1 >> dummy; // "in"
            iss1 >> season; // season e.g., "spring"
        }
        
        // Parse token[2] for color.
        string color;
        {
            istringstream iss2(tokens[2]); // "tan color"
            iss2 >> color; // "tan"
            // ignore "color"
        }
        
        // Parse token[3] for weight.
        int weight;
        {
            istringstream iss3(tokens[3]); // "70 pounds"
            iss3 >> weight;
            // ignore "pounds"
        }
        
        // Parse token[4] for origin.
        string origin;
        {
            // Remove the leading "from "
            if (tokens[4].substr(0, 5) == "from ") {
                origin = tokens[4].substr(5);
            } else {
                origin = tokens[4];
            }
        }
        
        // Generate birthday using age and season.
        string birthDate = genBirthDay(age, season);
        
        // Create an Animal object based on species.
        Animal* animalPtr = nullptr;
        if (species == "hyena") {
            animalPtr = new Hyena(age, sex, color, weight, origin);
        } else if (species == "lion") {
            animalPtr = new Lion(age, sex, color, weight, origin);
        } else if (species == "tiger") {
            animalPtr = new Tiger(age, sex, color, weight, origin);
        } else if (species == "bear") {
            animalPtr = new Bear(age, sex, color, weight, origin);
        } else {
            // If species is unknown, default to base Animal.
            animalPtr = new Animal(age, sex, species, color, weight, origin);
        }
        
        // Set birth date for the animal.
        animalPtr->setBirthDate(birthDate);
        
        // Generate unique ID for the animal.
        // Increment the counter for the species.
        speciesCounter[species]++;
        string uniqueID = genUniqueID(species, speciesCounter[species]);
        animalPtr->setUniqueID(uniqueID);
        
        // Assign a unique name from availableNames.
        // For simplicity, assign names in order; if we run out, start over.
        static int nameIndex = 0;
        if (!availableNames.empty()) {
            animalPtr->setName(availableNames[nameIndex % availableNames.size()]);
            nameIndex++;
        } else {
            animalPtr->setName("Unknown");
        }
        
        // Add the animal to the zoo vector.
        zooAnimals.push_back(animalPtr);
    }
    arrivingFile.close();
    
    // 3. Organize animals by species (habitat) using a map.
    map<string, vector<Animal*>> habitatMap;
    for (Animal* a : zooAnimals) {
        
        habitatMap[a->getSpecies()].push_back(a);
    }
    
    // 4. Write the detailed report to zooPopulation.txt
    ofstream reportFile("zooPopulation.txt");
    if (!reportFile) {
        cerr << "Error: Unable to open zooPopulation.txt for writing." << endl;
        return 1;
    }
    
    // For each habitat (species), write the header and list animals.
    for (const auto& entry : habitatMap) {
        string sp = entry.first;
        reportFile << sp.substr(0,1).append(sp.substr(1)) << " Habitat:" << endl;
        for (Animal* a : entry.second) {
            reportFile << a->getUniqueID() << "; "
                       << a->getName() << "; "
                       << "birth date " << a->getBirthDate() << "; "
                       << a->getColor() << " color; "
                       << a->getSex() << "; "
                       << a->getWeight() << " pounds; "
                       << "from " << a->getOrigin() << "; "
                       << "arrived " << ARRIVAL_DATE << endl;
        }
        reportFile << endl;
    }
    reportFile.close();
    
    cout << "Report successfully generated in zooPopulation.txt" << endl;
    
    // 5. Clean up dynamic memory.
    for (Animal* a : zooAnimals) {
        delete a;
    }
    
    return 0;
}

