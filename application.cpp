

#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <cstring>
using namespace std;

ifstream in;
typedef struct temp{
        char name[30];
        char start[17];
        char end[17];
        char desc[100];
}Temp;

class DateTime
{
  public:
    DateTime(){};
    void setData(std::string);             // parse from a string format
    std::string getFormatted() const;  // return as a formatted string

  private:
    int minute; // 0 to 59
    int hour;   // 0 to 23
    int day;    // 1 to 31
    int month;  // 1 to 12
    int year;
};

void DateTime::setData(std::string datetime)
{
    // DD/MM/YYYY HH:MM
    std::istringstream iss(datetime);
    char dump;
    iss >> day >> dump >> month >> dump >> year >> hour >> dump >> minute;
}

std::string DateTime::getFormatted() const
{
    // DD/MM/YYYY HH:MM
    std::ostringstream oss;
    oss << std::setfill('0');
    oss << std::setw(2) << day << '/';
    oss << std::setw(2) << month << '/';
    oss << year << ' ';
    oss << std::setw(2) << hour << ':';
    oss << std::setw(2) << minute;
    return oss.str();
}

class TimeAllocation{
      public:
             char desc[100];
             DateTime start;
             DateTime end;

             TimeAllocation(){};
             void setData(char *start,char *end, char *desc){
                      this->start.setData(string(start));
                      this->end.setData(string(end));
                      strcpy(this->desc,desc);
             }

             virtual void getData()=0;
};

class Meeting:public TimeAllocation{
      public:
             Meeting(char start[17],char end[17],char desc[100]){
                           this->setData(start,end,desc);
             }

             void getData(){
                  cout<<"   "<<start.getFormatted()<<" "<<end.getFormatted()<<" - "<<desc<<" "<<endl;
             }
};

class WorkDone:public TimeAllocation{
      public:
             WorkDone(char start[17],char end[17],char desc[100]){
                           this->setData(start,end,desc);
             }

             void getData(){
                  cout<<"   "<<start.getFormatted()<<end.getFormatted()<<" - "<<desc<<endl;
             }
};

class BugFix:public TimeAllocation{
      static unsigned int meetID;
      unsigned int ID;
      static void newID();
      public:
             BugFix(char start[17],char end[17],char desc[100]){
                      newID();
                      ID=meetID;
                      this->setData(start,end,desc);
             }

             void getData(){
                  cout<<"   "<<ID<<start.getFormatted()<<end.getFormatted()<<" - "<<"Bug #"<<ID<<" - "<<desc<<endl;
                  }
};

unsigned int BugFix::meetID=0;
void BugFix::newID(){
     meetID++;
}

class Task{
      char name[30];
      char desc[100];
      DateTime start;
      DateTime end;
      vector<Meeting> meetings;
      vector<Meeting>::iterator iterMeet;
      vector<WorkDone> workDone;
      vector<WorkDone>::iterator iterWork;
      vector<BugFix> bugFixes;
      vector<BugFix>::iterator iterBug;

      public:
             Task(char name[30],char start[17],char end[17],char desc[100]){
                       Temp t;
                       char str[255],*token;
                       strcpy(this->name,name);
                       this->start.setData(start);
                       this->end.setData(end);
                       strcpy(this->desc,desc);

                       in.getline(str, 255,'\n');
                       while(str && strcmp(str,"WorkDone")){
                                 in.getline(str, 255,'\n');
                                 if(!strcmp(str,"WorkDone"))break;
                                 token=strtok(str,",");
                                 strcpy(t.start,token);
                                 token=strtok(NULL,",");
                                 strcpy(t.end,token);
                                 token=strtok(NULL,",");
                                 strcpy(t.desc,token);

                                 meetings.push_back(Meeting(t.start,t.end,t.desc));
                       }

                       while(str && strcmp(str,"BugFix")){
                                 in.getline(str, 255,'\n');
                                 if(!strcmp(str,"BugFix"))break;
                                 token=strtok(str,",");
                                 strcpy(t.start,token);
                                 token=strtok(NULL,",");
                                 strcpy(t.end,token);
                                 desc=strtok(NULL,",");
                                 strcpy(t.desc,token);

                                 workDone.push_back(WorkDone(t.start,t.end,t.desc));
                       }

                       while(str && strcmp(str,"Task") && strcmp(str,"Project")){
                                 in.getline(str, 255,'\n');
                                 if(!strcmp(str,"Task") || !strcmp(str,"Project"))break;
                                 token=strtok(str,",");
                                 strcpy(t.start,token);
                                 token=strtok(NULL,",");
                                 strcpy(t.end,token);
                                 token=strtok(NULL,",");
                                 strcpy(t.desc,token);

                                 bugFixes.push_back(BugFix(t.start,t.end,t.desc));
                       }
             }

             void getData(){

                  cout<<"  "<<name<<endl;
                  cout<<" -"<<desc<<endl<<" -Started: "<<start.getFormatted()<<endl<<" -Deadline"<<end.getFormatted()<<endl;
                  cout<<"  Time Allocations"<<endl;
                  for(iterMeet = meetings.begin(); iterMeet != meetings.end(); iterMeet++)
                    {
                               (*iterMeet).getData();
                    }

                    for(iterWork = workDone.begin(); iterWork != workDone.end(); iterWork++)
                    {
                               (*iterWork).getData();
                    }

                    for(iterBug = bugFixes.begin(); iterBug != bugFixes.end(); iterBug++)
                    {
                               (*iterBug).getData();
                    }
             }
};

class Project{
      char name[30];
      char desc[100];
      DateTime start;
      DateTime deadline;
      vector<Task> tasks;
      vector<Task>::iterator iterTask;

      public:
             Project(){};
             void setData(char name[30],char start[17],char end[17],char desc[100]){
                       Temp t;
                       char str[256],*token;
                       strcpy(this->name,name);
                       this->start.setData(start);
                       this->deadline.setData(end);
                       strcpy(this->desc,desc);

                       in.getline(str, 255,'\n');
                       while(str && !strcmp(str,"Task")){
                                 in.getline(str, 255,'\n');
                                 token=strtok(str,",");
                                 strcpy(t.name,token);
                                 token=strtok(NULL,",");
                                 strcpy(t.start,token);
                                 token=strtok(NULL,",");
                                 strcpy(t.end,token);
                                 token=strtok(NULL,",");
                                 strcpy(t.desc,token);

                                 tasks.push_back(Task(t.name,t.start,t.end,t.desc));
                       }
             }

             void getData(){
                  cout<<"=================================="<<endl<<name<<endl<<"----------------------------------"<<endl;
                  cout<<" -"<<desc<<endl<<" -Started: "<<start.getFormatted()<<endl<<" -Deadline"<<deadline.getFormatted()<<endl<<"--------------------"<<endl;
                  cout<<"  Code Classes"<<endl<<"  ------------"<<endl;
                  for(iterTask = tasks.begin(); iterTask != tasks.end(); iterTask++)
                    {
                               (*iterTask).getData();
                    }
             }
};

int main(){
    Project project;
    char name[30],start[16],end[16],desc[16];
    Temp t;
    in.open("Bootcamp.txt",ios::in);
    char str[256],*token;
    in.getline(str, 255,'\n');
    while(in && !strcmp(str,"Project"))
    if(!strcmp(str,"Project")){
       in.getline(str, 255,'\n');
       token=strtok(str,",");
       strcpy(t.name,token);
       token=strtok(NULL,",");
       strcpy(t.start,token);
       token=strtok(NULL,",");
       strcpy(t.end,token);
       token=strtok(NULL,",");
       strcpy(t.desc,token);
       project.setData(t.name,t.start,t.end,t.desc);
    }

    project.getData();

    system("pause");
    return 0;
}
