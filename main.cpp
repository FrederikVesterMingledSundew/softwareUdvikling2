#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <string>
#include <fstream>

#define FILENAME "ToDoList.conf"

class Task
{
	public:
	
	Task(std::string description){
		_description = description;
		completed = false;
	}

	Task(std::string description, bool complete) : _description(description), completed(complete){
		; //
	}
	
	bool isCompleted() {
		return completed;
	}
	std::string getDescription() {
		return _description;
	}
	private:
	std::string _description; //Description
	bool completed = false; //Is the Task completed
};

class TodoList
{
	
	public:
	std::vector<Task> todolist;
	TodoList(){}

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
						Task middleman = Task(nextLine.substr(0,splitter), completed);
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
	
	static bool SaveToConfig(const std::map<std::string, std::string> input[], std::size_t sizeOfArray) { //Laver en array ud af det for at opdele hver persons ID
		std::ofstream File(FILENAME);// Start stream
		if(File.is_open()) {
			for(std::size_t i = 0; i < sizeOfArray; ++i) { //Laver en array ud af det for at opdele hver persons ID
				for(auto &linje:input[i]) {
					File<<"["<<i<<"]"<<linje.first<<"="<<linje.second<<std::endl;
				}
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
	config::ReadFromConfig(Reader);
	
    if (config::ReadFromConfig(Reader)) {
        std::cout << "------------------[ To do list ]------------------" << std::endl;
        for (auto& task : Reader) {
            std::cout << task.getDescription() << (task.isCompleted() ? " [Completed]" : " [Incomplete]") << std::endl;
        }
        std::cout << "--------------------------------------------------" << std::endl;
    } else {
        std::cerr << "Failed to read from config file." << std::endl;
    }

    return 0;
}
