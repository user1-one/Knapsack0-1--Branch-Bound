#include <iostream>
#include <vector>
#include <fstream>
#include "item.h"
#include <algorithm>
#include <ctime>
using namespace std;
// finds upper bound by moving left as much as possible
// places a partial item 
double bound(vector<item*>& ITEMS, const int& capacity, double& cur_value, double& cur_weight, double& new_value, double& new_weight, int& Level, int& Boundlevel, vector<int>& cur_soln) {
    bool found = false;
    double boundVal = -1;
    int n = ITEMS.size();
    new_value = cur_value;
    new_weight = cur_weight;
    Boundlevel = Level + 1;
    while (Boundlevel < n && !found){
        if (new_weight + ITEMS[Boundlevel]->Get_Weight() <= capacity){
            new_weight += ITEMS[Boundlevel]->Get_Weight();
            new_value += ITEMS[Boundlevel]->Get_Value();
            cur_soln[Boundlevel] = 1;
        }
        else{
            boundVal = new_value + (capacity - new_weight) * ITEMS[Boundlevel]->Get_Value() / ITEMS[Boundlevel]->Get_Weight();
            found = true;
        }
        Boundlevel++;
    }
    if (found){
        // If we have a fractional solution for last item in knapsack
        Boundlevel--;
        return boundVal;
    }
    else{
        return new_value;
    }
}
void SolveKnapsack(vector<item*>& ITEMS, const int& capacity, vector<int>& Best_soln, double& Soln_value, double& cur_weight, double& cur_value) {
    
    time_t start, finish;
    time(&start);
    vector<int> cur_soln;
    int n = ITEMS.size();
    cur_soln.resize(ITEMS.size());
    double new_weight = 0;
    double new_value = 0;
    int Level = 0;
    int Boundlevel = 0;
    // we want first run to happen to initialize our variables in bound()
    do{
        while (bound(ITEMS, capacity, cur_value, cur_weight, new_value, new_weight, Level, Boundlevel, cur_soln) <= Soln_value){
            while (Level != 0 && cur_soln[Level] != 1){
        
                Level--;
            }
            if (Level == 0)
                return;
            cur_soln[Level] = 0;
            cur_weight -= ITEMS[Level]->Get_Weight();
            // Reduce solution weight
            cur_value -= ITEMS[Level]->Get_Value();
        }
        // Go back 
        cur_weight = new_weight;
        cur_value = new_value;
        Level = Boundlevel;
        if (Level == n){
            Soln_value = cur_value;
            // Update 
            Best_soln = cur_soln;
            Level = n - 1;
        }
        else
            cur_soln[Level] = 0;
        time(&finish);
    } while (difftime(finish, start) < 900);// runs for maximum of 15 minutes(except for test cases 82_0 and 106_0 in which runs for 2 hours!)

}

const long long int Get_total_Profit(const vector<item*>& ITEMS, const vector<int>& Best_soln) {
    long long int total_Profit = 0;
    long long int total_Weight = 0;
   // item(0,0,0) for initial setup not included in calculation
    for (int i = 1; i < Best_soln.size(); i++) {
        if (Best_soln[i] == 1) {
            ITEMS[i]->Set_IsTaken(true);
            total_Profit += ITEMS[i]->Get_Value();
            total_Weight += ITEMS[i]->Get_Weight();
        }
    }
    cout << "TOTAL WEIGHT USED: " << total_Weight << endl;
    return total_Profit;

}
// used for sort
class compareRatio {
public:
    bool operator() (const item* x, const item* y) const {

        double ratio = double(x->Get_Value()) / double(x->Get_Weight());
        double otherRatio = double(y->Get_Value()) / double(y->Get_Weight());
        return (ratio > otherRatio);
    }
};
//used for sort
class compareRank {
public:
    bool operator() (const item* x, const item* y) const {
        return (x->Get_Rank() < y->Get_Rank());
    }
};

int main() {
    string filename;
    ifstream InputFile;
    ofstream OutputFile;
    vector<item*> ITEMS;
    cout << "File name: ";
    cin >> filename;
    InputFile.open(filename.data());

    while (!InputFile.is_open()) {
        cout << "Wrong filename! Please try again: ";
        InputFile.clear();
        cout << "File name: ";
        cin >> filename;
        InputFile.open(filename.data());
    }

    int temp;
    InputFile >> temp;
    int size = temp;
    ITEMS.resize(size + 1);
    InputFile >> temp;
    long int capacity = temp;
    long int counter = 1;
    while (counter < ITEMS.size()) {
        InputFile >> temp;
        int w = temp;
        InputFile >> temp;
        int v = temp;
        ITEMS[counter] = new item(w, v, counter);
        counter = counter + 1;
    }
    InputFile.close();
    ITEMS[0] = new item(0, 0, 0);
    sort(ITEMS.begin() + 1, ITEMS.end(), compareRatio());
    vector<int> Best_soln;
    Best_soln.resize(ITEMS.size());
    double Soln_value = -1;
    double cur_weight = 0;
    double cur_value = 0;
    SolveKnapsack(ITEMS, capacity, Best_soln, Soln_value, cur_weight,cur_value);
    long long int total_Profit = Get_total_Profit(ITEMS, Best_soln);
    sort(ITEMS.begin() + 1, ITEMS.end(), compareRank());
    OutputFile.open("output-" + filename);
    OutputFile << total_Profit << endl;
    for (int i = 1; i < ITEMS.size(); i++) {
        OutputFile << ITEMS[i]->Is_taken() << " ";
    }

}
