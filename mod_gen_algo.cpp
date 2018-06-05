#include <bits/stdc++.h>

using namespace std;

class Employee{
private:
	int startShift, endShift, workHour;
public:
    string name;
    Employee();
    Employee(string empName, int frstShift, int lstShift);
    void addShift(int frstShift, int lstShift);
    void showDetails();
    void operator=(Employee emp);
};

Employee::Employee(){
    name="";
    startShift=0;
    endShift=0;
    workHour=0;
}

Employee::Employee(string empName, int frstShift, int lstShift){
	name=empName;
	startShift=frstShift;
	endShift=lstShift;
	workHour=endShift - startShift;
}

void Employee::addShift(int frstShift, int lstShift){
	this->startShift=frstShift;
	this->endShift=lstShift;
}

void Employee::showDetails(){
    cout<<endl;
	cout<<"Name: "<<name<<endl;
	cout<<"Start time: "<<startShift<<endl;
	cout<<"End time: "<<endShift<<endl;
	cout<<"Total work hour: "<<endShift - startShift<<endl;
}

int main(){
	std::vector<Employee> domain;
	string inp;
	string nm;
	int fst,lst;
	bool quit=false;
	while (!quit){
        cout<<endl;
        cout<<"Enter command:\n";
        cin>>inp;
        if(inp=="add"){
            cout<<"About to add employee:\n";
            cout<<"Enter name, start hour and end hour: \t";
            cin>>nm>>fst>>lst;
            Employee *e=new Employee(nm,fst,lst);
            domain.push_back(*e);
        }
        else if(inp=="show"){
            for(int i=0; i<domain.size(); ++i){
                domain[i].showDetails();
            }
        }
        else if (inp=="quit")
            quit=true;
	}
	return 0;
}
