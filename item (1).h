#ifndef ITEM_H
#define ITEM_H


using namespace std;
class item {
public:
    item(int v, int w, int r);
    int Get_Weight() const;
    int Get_Value() const;
    int Get_Rank() const;
    void Set_IsTaken(bool t);
    bool Is_taken() const;
    double Get_ratio() const; 
private:
    int value;
    int weight;
    int Rank;
    bool taken;
   
};
item::item(int v, int w, int r) {
    value = v;
    weight = w;
    Rank = r;
    taken = false;
}
int item::Get_Weight() const{
    return weight;
} 
int item::Get_Value() const{
    return value;
}
int item::Get_Rank() const{
    return Rank;
}
void item::Set_IsTaken(bool t)
{
    taken = t;
}
bool item::Is_taken() const{
    return taken;
}
double item::Get_ratio() const {
    double r = double (value) / double (weight);
    return r;
}
#endif 