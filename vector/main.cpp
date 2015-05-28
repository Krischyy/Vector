#include "Vector.h"
#include <iostream>
#include <string.h>
#include <string> 
using namespace std;
class Food
{
private:
	string m_name;
	int m_calories;
public:
	Food(): m_name("banana"),m_calories(48){} 
	Food(string name, int calories) : m_name (name), m_calories(calories){}
};
int main ()
{
Food f1("watermelon",46);
Food f2("potato",115);
Food f3("chocolate",210);
Food f4("chicken",142);
Food f5("milk",9);

Vector <Food> food;
	food.push_back(f1);
	food.push_back(f2);
	food.push_back(f3);
	food.push_back(f4);
	food.push_back(f5);

Vector<int> v1;
v1.push_back(0);
v1.push_back(1);
v1.push_back(1);
v1.push_back(2);
v1.push_back(18);
v1.push_back(3);
v1.push_back(400);
v1.push_back(5);
v1.push_back(8);
v1.erase(4);
v1.erase(5);
cout <<"The 7th Fibonacci number is " << v1.back() << endl; //8
v1.pop_back();
cout <<"The 6th Fibonacci number is " << v1.back() << endl; //5
cout <<"The 5th Fibonacci number is " << v1[4] << endl; //3
//Извежда 0 1 1 2 3 5 
cout << "The first 6 Fibonacci numbers are: ";
for (int i = 0; i < v1.size(); i++)
{
cout << v1[i] << " ";
}
cout <<endl;
cout << "-------------------------------------------------";
cout << endl;

Vector<string> SHINee;
SHINee.push_back("Onew");
SHINee.push_back("Key");
SHINee.push_back("Minho");
SHINee.push_back("Jonghyun");
SHINee.push_back("Taemin");
cout << "The members of SHINee are: "<<endl;
for (int i=0; i<SHINee.size();i++)
{
	cout << SHINee[i] << endl;
}
cout << "-------------------------------------------------";
cout << endl;

Vector<string> TVXQ;
TVXQ.push_back("Uknow Yunho");
TVXQ.push_back("Max Changmin");

TVXQ.insert(2, "Hero Jaejoong");
TVXQ.insert(3, "Micky Yoochun");
TVXQ.insert(4, "Xiah Junsu");

cout << "The members of TVXQ are: " << endl;
for (int i=0; i<TVXQ.size();i++)
{
	cout << TVXQ[i] << endl;
}

return 0;
}
