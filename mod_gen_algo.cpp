#include <bits/stdc++.h>

using namespace std;

/***********************************************************************************************\
*    This code is a console version of the genetic algorithm for                                *
*    roster management. It contains the following major functions                               *
*    and class:                                                                                 *
*                                                                                               *
*    1. class Employee - the gene                                                               *
*    2. class Schedule - the chromosome                                                         *
*    3. vector<Schedule> - the domain of genes                                                  *
*    4. int mutated_genes() - a function to select random gene from domain                      *
*    5. vector<int> createGnome() - a function to create random schedule                        *
*            it only returns the array of chromosome, a part of the                             *
*            "Schedule" class not all of it. Total schedule can be                              *
*            created by the use of Schedule::Schedule(argument) function.                       *
*                                                                                               *
*    #Functions to be noted:                                                                    *
*        1. mate(): -a function of the class "Schedule"                                         *
*                -responsible for production of offspring by mating 2 parents                   *
*                -crossover and mutation occurs here                                            *
*        2. calcFitness():  - function of the class "Schedule"                                  *
*                        - decides the acceptance value of a schedule                           *
*                        - responsible for handling the constraints of the roster management    *
*                                                                                               *
\***********************************************************************************************/

const int POPULATION_SIZE=100;
const int WORKING_HOUR=10;

//Employee is a gene; the class contains the info about the gene
class Employee{
private:
	string name;
	int startShift, endShift, workHour;
public:
    int id;
    Employee(string empName, int id, int frstShift, int lstShift);
    void addShift(int frstShift, int lstShift);
    void showDetails();
};

//defining chromosome or individual
class Schedule{
public:
    vector<Employee>chromosome;
    int fitness;
    Schedule(vector<Employee> chromosome);
    Schedule mate(Schedule parent2);
    int calcFitness();
    void displaySchedule();
};

//the domain of genes (employee)
std::vector<Employee> domain;

//returns a random mutated gene i.e. employee
Employee mutated_genes(){
    int len=domain.size();
    int i=rand()%len;
    return domain[i];
}

//creates a random schedule (individual)
vector<Employee> createGnome(int len){
    vector<Employee> gnome;
    for (int i=0; i<len; ++i)
        gnome.push_back(mutated_genes());
    return gnome;
}

//overload the "<" operator for comparison between class
bool operator<(const Schedule &s1, const Schedule &s2){
    return s1.fitness<s2.fitness;
}

//employee class constructor
Employee::Employee(string empName, int idValue, int frstShift, int lstShift){
	name=empName;
	id=idValue;
	startShift=frstShift;
	endShift=lstShift;
	workHour = endShift - startShift;
}

//change the shifts for employees
void Employee::addShift(int frstShift, int lstShift){
	this->startShift=frstShift;
	this->endShift=lstShift;
	this->workHour=endShift - startShift;
}

//show the details regarding employee shifts
void Employee::showDetails(){
    cout<<endl;
	cout<<"Name: "<<name<<endl;
	cout<<"Employee id: "<<id<<endl;
	cout<<"Start time: "<<startShift<<endl;
	cout<<"End time: "<<endShift<<endl;
	cout<<"Total work hour: "<<endShift - startShift<<endl;
}


//constructor for schedule class
Schedule::Schedule(vector<Employee> chromosome){
    this->chromosome=chromosome;
    fitness=calcFitness();
}


//mating function that produces offspring based on 2 parents
Schedule Schedule::mate(Schedule parent2){
    //crossover and mutation goes here
    vector<Employee> child_chromosome;
    //make the size of child same as the parent
    int len=chromosome.size();
    for (int i=0; i<len; ++i){
        int p=rand()%101;
        if (p<45)
            child_chromosome.push_back(chromosome[i]);
        else if (p<90)
            child_chromosome.push_back(parent2.chromosome[i]);
        else
            child_chromosome.push_back(mutated_genes());
    }
    return Schedule(child_chromosome);
}


//main function that determines whether or not to accept the answer
int Schedule::calcFitness(){
    //fitness calculation function goes here
    map<int,int> mem;
    int i=0;
    for (int j=0; j<chromosome.size(); ++j)
        mem[chromosome[j].id]=0;

    //initialize fitness value
    int fit=0;

    //code for no pair integer
    while (i<chromosome.size()){
        if (i==0){
            ++mem[chromosome[i].id];
            if (chromosome[i+1].id!=chromosome[i].id)
                ++fit;
            ++i;
        }
        else{
            if (chromosome[i].id==chromosome[i-1].id){
                ++i;
            }
            else{
                if (i<chromosome.size()-1 and chromosome[i].id!=chromosome[i+1].id)
                    ++fit;
                else if (i==chromosome.size()-1)
                    ++fit;
                mem[chromosome[i].id]++;
                ++i;
            }
        }
    }

    //code for repetitive integer
    for (int i=0; i<chromosome.size(); ++i){
        if(mem[chromosome[i].id]!=0){
            fit+=(mem[chromosome[i].id]-1);
            mem[chromosome[i].id]=0;
        }
    }
    return fit;
}

//display the schedule created
void Schedule::displaySchedule(){
    for (int i=0; i<chromosome.size(); ++i)
        cout<<chromosome[i].id<<" ";
}

int main(){
    srand((unsigned)(time(0)));
	string inp; //used to store console input
	string nm;  //name of employee
	int generation=0;   //keep count of generation
	vector<Schedule> population;    //main population that evolves with time
	int idVal,fst,lst;  //variables to hold employee details
	bool found=false, quit=false;   //to keep the loop running
	while(!quit){
        /*The following commented code was used for console i/o*/
        cout<<endl;
        cout<<"Enter command:\n";
        cin>>inp;
        if(inp=="add"){
            cout<<"About to add employee:\n";
            cout<<"Enter name, id, start hour and end hour: \t";
            cin>>nm>>idVal>>fst>>lst;
            Employee *e=new Employee(nm,idVal,fst,lst);
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
	for(int i=0; i<POPULATION_SIZE; ++i){
        vector<Employee> gnome=createGnome(WORKING_HOUR);    //creates a random schedule of length WORKING_HOUR
        population.push_back(Schedule(gnome));      //add the random schedule to population
	}

	while (!found){
        sort(population.begin(), population.end()); //sort according to fitness, lower fitness value -> best answer
        if (population[0].fitness<=0){      //if fitness=0, we've found the answer
        /* Here "<=" is used instead of "=="
        to make sure that our loop terminates once answer is found*/
            found=true;
            break;
        }
        vector<Schedule> new_generation;    //new generation of population

        //Send the fittest 10% population to the next generation directly
        int s=(10*POPULATION_SIZE)/100;
        for (int i=0; i<s; ++i)
            new_generation.push_back(population[i]);

        //mate the 2 random parents for rest 90% of the new_generation
        s=(90*POPULATION_SIZE)/100;
        for (int i=0; i<s; ++i){
            int len=population.size();
            int r=rand()%100;           //select random 1st parent
            Schedule parent2=population[r];
            r=rand()%100;               //select random 2nd parent
            Schedule parent1=population[r];
            Schedule offspring=parent1.mate(parent2);
            new_generation.push_back(offspring);
        }
        population=new_generation;      //new_generation becomes the current population
        cout<<"Generation: "<<generation<<"\t";

        cout<<"Schedule: ";
        population[0].displaySchedule();
        cout<<"\t";

        cout<<"Fitness: "<<(population[0]).fitness<<"\n";
        ++generation;           //next generation
	}
	cout<<"SOLUTION:\n";

    cout<<"Generation: "<<generation<<"\t";

    cout<<"Schedule: ";
    population[0].displaySchedule();
    cout<<"\t";

    cout<<"Fitness: "<<(population[0]).fitness<<"\n";

	return 0;
}
