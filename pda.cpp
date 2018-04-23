/*
Arturo Burela
*/
#ifndef PDA_HPP
#define PDA_HPP

#include "state.cpp"
#include <fstream>
#include <sstream>
#include <queue>
#include <stack>
#include <algorithm>

class PDA {
private:
  // Stores all Non-terminal symbols
  std::vector<std::string> nts;
  // Pointer to initial non-terminal
  std::string * initial;
  // Initial, loop and accept state
  State start, loop, accept;
  // Rest of states
  std::vector<State> states;
  // The string to test
  std::queue<char> string;
  // Accepted paths
  std::vector<std::vector<link>> paths;

  // Start test
  /*void test() {
    std::bitset<1> valid;
    // Call to explore to get paths
    paths = initialState->explore(string);
    for (std::vector<std::vector<link>>::iterator it = paths.begin(); it != paths.end(); ++it){
      // If path is accepted
      if (it->at(0).input != '\0') {
        valid.set();
        // Push current state to received path
        it->push_back(link('\0', '\0', false, initialState));
        // Remove first item as it is finalState self pointer
        it->erase(it->begin());
      }
    }
    if (valid.test(0)) {
      std::cout << " String accepted by automaton " << '\n';
      // Log all accepted paths
      logPaths(paths);
    } else {
      // If no path was found then just print and exit
      std::cout << " String not accepted by automaton " << '\n';
    }
  }*/

  // Logs all automaton data
  void logStates() {
    start.logData();
    loop.logData();
    accept.logData();
  }

  //Logs all paths
  void logPaths(std::vector<std::vector<link>> paths) {
    for (std::vector<std::vector<link>>::iterator it = paths.begin(); it != paths.end(); ++it){
      std::cout << '\n' << "/* Path found: */" << '\n';
      // Valid paths are stored BACKWARDS!
      for (std::vector<link>::reverse_iterator it2 = it->rbegin(); it2 != it->rend(); ++it2){
        std::cout << it2->input << "->[" << it2->destination->getName() << "]-";
      }
      std::cout << '\n' << "/* ------------- */" << '\n';
    }
  }

  // Loads automaton data assuming file is formatted correctly
  void loadFile(const std::string& filename){
    std::cout << "Loading automaton..." << std::endl;
    // Aux varaibles to store links
    std::string name;
    //char input;
    std::string destination;
    std::ifstream afile(filename);
    // Stores a line
    std::string line;
    // Usefull to split lines
    char param = ',';
    // Line number counter
    int i = 0;
    // Get line data
    std::cout << "Entrando al while" << '\n';
    while(getline(afile,line))
    {
      std::stringstream   linestream(line);
      // Stores split line value
      std::string value;
      if (i == 4) {
        //param = '\n';
      }
      // Call again with param to read by characters
      while(getline(linestream,value,param))
      {
        // Process data according to line number
        switch (i) {
          case 0:
          std::cout << "Loading Non Terminal Symbols" << '\n';
          nts.push_back(value);
          break;
          case 1:
          std::cout << "Loading alphabet" << '\n';
          // Reading alphabet here is not really necessary
          break;
          case 2:
          std::cout << "Loading initial symbol" << '\n';
          initial = &(*std::find(nts.begin(), nts.end(), value));
          break;
          default:
          std::cout << value << '\n';
          std::cout << "Loading transition function" << '\n';
          destination = "";
          /*for ( std::string::iterator it=value.begin(); it!=value.end(); ++it){
            // Save state name, input or final state according to ',' and ':'
            if (*it == ',') {
              name = destination;
              destination = "";
            } else if (*it == ':') {
              input = destination.at(0);
              destination = "";
            } else {
              destination += *it;
            }
          }*/
          // Find both states by name and add link
          //this->findState(name)->addLink(link(input, '\n', false, this->findState(destination)));
          break;
        }
      }
      i++;
      // std::cout << "Line Finished" << std::endl;
    }
    // Close file
    afile.close();
    std::cout << "Automaton loaded" << std::endl;
  }

  PDA(){}
public:
  // This constructor loads automaton and test string
  PDA(const std::string& filename, const std::string& testString){
    std::cout << "String to test:" << testString << '\n';
    // Transform string into char queue
    for (int i = 0; (unsigned) i < testString.length(); i++){
      string.push(testString.at(i));
    }
    // Init start, loop and final states
    start = State("Start");
    loop = State("Loop");
    accept = State("Accept");
    //Load automaton
    loadFile(filename);
    //Test string
    //test();
  }
};

#endif
