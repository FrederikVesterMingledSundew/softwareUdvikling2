#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#define FILENAME "ToDoList.conf"

class Task
{
public:

    Task(std::string description){
        _description = description;
        completed = false;
        date = false;
    }

    Task(std::string description, bool complete, std::string duedate) : _description(description), completed(complete), date(duedate){
        ; //
    }

    bool isCompleted() {
        return completed;
    }

    std::string getDescription() {
        return _description;
    }

    void markComplete() {
        completed = true;
    }
    void setDate(std::string date2){
        date = date2;
    }
    std::string getDate(){
        return date;
    }

private:
    std::string _description; //Description
    bool completed = false; //Is the Task completed
    std::string date = "";
};

class TodoList
{

public:
    TodoList(){}
    void add(const Task &toDoTask) {
        this->todolist.push_back(toDoTask);
    }

    std::vector<Task> getList() {
        return todolist;
    }

    int getSize() {
        return todolist.size();
    }

    void remove(int ID) {
        todolist.erase(todolist.begin() + ID);
    }

    void markDone(int ID) {
        todolist[ID].markComplete();
    }
    void setDueDate(int ID, std::string date){
        todolist[ID].setDate(date);
    }

private:
    std::vector<Task> todolist;

};

class config
{
public:
    static bool ReadFromConfig(std::vector<Task> &output) { //Read from config file
        std::ifstream File(FILENAME); // Start stream
        if(File.is_open()) {
            std::string nextLine;
            int taskID = 0;
            while(getline(File, nextLine)) {
                if(nextLine.at(0) == '#') { //Hvis vi ønsker at lave en kommentar i To do listen
                    ; //Do nothing
                }
                else {
                    int splitter = nextLine.find('='); //Vi skal finde et ligmed tegn ellers stop projektet
                    if(splitter != 0) {

                        std::string completeString = nextLine.substr(splitter+1,nextLine.length()-(splitter+1));
                        bool completed = false;

                        if(completeString.compare("1")) {
                            completed = true;
                        }
                        Task middleman = Task(nextLine.substr(0,splitter), completed, "");
                        output.push_back(middleman);
                        ++taskID;
                    }
                    else {
                        File.close();
                        return false;
                    }
                }
            }
            File.close();
            return true;
        }
        File.close();
        return false;

    }

    static bool SaveToConfig(const std::vector<Task> &input) { //Laver en array ud af det for at opdele hver persons ID
        std::ofstream File(FILENAME);// Start stream
        if(File.is_open()) {
            for(Task linje:input) {
                File << linje.getDescription() << "=" << (linje.isCompleted()?"1":"0") << std::endl;
            }
            File.close();
            return true;
        }
        File.close();
        return false;
    }

};

int main() {

    TodoList todo = TodoList();

    std::vector<Task> Reader{};
    if (config::ReadFromConfig(Reader)) {

        for (Task task : Reader) {
            todo.add(task);
        }

    }

    std::string Err{};
    do {
        system("cls");
        std::cout << "\033[2J\033[1;1H";

        std::cout << "------------------[ To do list ]------------------" << std::endl;

        int taskId = 0;
        for(Task task : todo.getList()) {
            std::cout << std::right << std::setw(10) << "["<<taskId<<"]: ";
            std::cout << task.getDescription() << (task.isCompleted() ? " [Completed]" : " [Incomplete]") <<
                         task.getDate() << std::endl;
            ++taskId;
        }
        if(!taskId) {
            std::cout << "Nothing on your list..." << std::endl;
        }
        std::cout << "--------------------------------------------------" << std::endl;
        std::cout << "\n" << std::endl;
        if(Err == "") {

        }
        else {
            std::cout << Err << "\n" << std::endl;
        }
        std::cout << "Do you wish to make changes to your list?"<< std::endl;
        std::cout << "/add [task description] - To add a task"<< std::endl;
        std::cout << "/rm [task ID] - To remove a task"<< std::endl;
        std::cout << "/done [task ID] - Mark complete"<< std::endl;
        std::cout << "/save - To save list"<< std::endl;
        std::cout << "/exit - To exit program"<< std::endl;
        std::cout << "/due [task ID] [dd/mm/yyyy] - to set a due date for your task"<< std::endl; // dato er en string så due date kunne også bare være f.eks "monday"


        std::string msgIn;
        std::getline(std::cin, msgIn);

        if(msgIn.at(0) == '/') { //check om det er en kommando
            int splitter = msgIn.find((char)32); //check om der er et space
            if(splitter == -1) {
                if(!msgIn.compare("/save")) {
                    config::SaveToConfig(todo.getList());
                    continue;
                }
                if(!msgIn.compare("/exit")) {
                    return 0;
                }
                Err = "No arguments...";
                continue;
            }
            else {
                if(!msgIn.substr(0,splitter).compare("/add")) {
                    todo.add(Task(msgIn.substr(splitter+1,msgIn.length()-(splitter+1))));
                    continue;
                }
                if(!msgIn.substr(0,splitter).compare("/rm")) {
                    int num = atoi(msgIn.substr(splitter+1,msgIn.length()-(splitter+1)).c_str());
                    if(num > todo.getSize() || num < 0) { //Not valid ID
                        Err = "ID was not valid";
                        continue;
                    }
                    todo.remove(num);
                    Err = "";
                    continue;
                }
                if(!msgIn.substr(0,splitter).compare("/done")) {
                    int num = atoi(msgIn.substr(splitter+1,msgIn.length()-(splitter+1)).c_str());
                    if(num > todo.getSize() || num < 0) { //Not valid ID
                        Err = "ID was not valid";
                        continue;
                    }
                    todo.markDone(num);
                    Err = "";
                    continue;
                }
                if(!msgIn.substr(0,splitter).compare("/due")) {
                    int num = atoi(msgIn.substr(splitter+1,msgIn.length()-(splitter+1)).c_str());
                    if(num > todo.getSize() || num < 0) { //Not valid ID
                        Err = "ID was not valid";
                        continue;
                    }
                    std::string date = (msgIn.substr(splitter+2,msgIn.length()-(splitter+1)).c_str());
                    todo.setDueDate(num, date);
                    Err = "";
                    continue;
                }
            }
        }
    }
    while(1);
    return 0;
}
