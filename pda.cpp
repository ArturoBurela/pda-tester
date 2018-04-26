/*
Arturo Burela
*/
#ifndef PDA_HPP
#define PDA_HPP

#include "state.cpp"
#include <fstream>
#include <sstream>
#include <queue>
#include <algorithm>
#include <list>

class PDA {
private:
  // Stores all Non-terminal symbols
  std::vector<char> nts;
  // Pointer to initial non-terminal
  char * initial;
  // Initial, loop and accept state
  State start, loop, accept;
  // Rest of states
  std::list<State> states;
  // The string to test
  std::queue<char> string;
  // Stack alphabet
  std::vector<char> alphabet;
  //PDA stack
  std::stack<char> stack;
  // Accepted paths
  std::vector<std::vector<link>> paths;
  // Aux counter to identify generated states
  int stateNum = 0;

  // Start test
  void test() {
    std::bitset<1> valid;
    // Call to explore to get paths
    paths = start.explore(string, stack);
    for (std::vector<std::vector<link>>::iterator it = paths.begin(); it != paths.end(); ++it){
      // If path is accepted
      if (it->at(0).input != '\0') {
        valid.set();
        // Push current state to received path
        it->push_back(link('\0', '\0', false, &start));
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
    //logPaths(paths);
  }

  // Logs all automaton data
  void logStates() {
    start.logData();
    loop.logData();
    accept.logData();
    for (std::list<State>::iterator it = states.begin(); it != states.end(); ++it){
      it->logData();
    }
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
    std::ifstream afile(filename);
    // Stores a line
    std::string line;
    // Usefull to split lines
    char param = ',';
    // Line number counter
    int i = 0;
    // Get line data
    while(getline(afile,line))
    {
      std::stringstream   linestream(line);
      // Stores split line value
      std::string value;
      // Call again with param to read by characters
      while(getline(linestream,value,param))
      {
        // Process data according to line number
        switch (i) {
          case 0:
          // std::cout << "Loading Non Terminal Symbols" << '\n';
          nts.push_back(value.at(0));
          break;
          case 1:
          // std::cout << "Loading alphabet" << '\n';
          for (size_t i = 0; i < value.size(); i++) {
            //Add to alphabet
            alphabet.push_back(value.at(i));
            // Create links to pop terminals in stack
            loop.addLink(link(value.at(i),value.at(i),'&',&loop));
          }
          break;
          case 2:
          // std::cout << "Loading initial symbol" << '\n';
          initial = &(*std::find(nts.begin(), nts.end(), value.at(0)));
          break;
          default:
          //std::cout << "Loading transition function" << '\n';
          // stores nts
          char nts = *value.begin();
          // array to store each rule
          std::string v;
          // Aux pointer to states
          State *fs, *ns;
          fs = &loop;
          for ( std::string::iterator it=(value.begin()+3); it!=value.end(); ++it){
            v.insert(v.end(),*it);
            // If | found or end of string make link and continue
            if (*it=='|' || it==value.end()-1) {
              if (*it=='|') {
                v.pop_back();
              }
              //Reverse string to use rightmost
              std::reverse(v.begin(), v.end());
              // std::cout << v << '\n';
              // Create new links and states
              for ( std::string::iterator itv=v.begin(); itv!=v.end(); ++itv){
                //std::cout << *itv << '\n';
                // If only one
                if (itv==v.begin() && itv==v.end()-1) {
                  loop.addLink(link('&',nts,*itv,&loop));
                  // If first
                } else if (itv==v.begin()) {
                  // Add new state and push to vector
                  states.push_back(State(std::to_string(stateNum)));
                  stateNum++;
                  ns = &states.back();
                  fs->addLink(link('&',nts,*itv,ns));
                  // Set pointer
                  fs = ns;
                  // If not last
                } else if (itv!=v.end()-1) {
                  // Add new state and push to vector
                  states.push_back(State(std::to_string(stateNum)));
                  stateNum++;
                  ns = &states.back();
                  fs->addLink(link('&','&',*itv,ns));
                  // Set pointer
                  fs = ns;
                } else {
                  // If last
                  fs->addLink(link('&','&',*itv,&loop));
                  fs = &loop;
                }
              }
              //Reset v string
              v.clear();
            }
          }
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
    accept.setFinal();
    //Load automaton
    loadFile(filename);
    //Push $ symbol to stack
    stack.push('$');
    //Link form start to loop
    start.addLink(link('&','&',*initial,&loop));
    //Link from loop to end
    loop.addLink(link('&','$','&',&accept));
    //Logs pda data
    //logStates();
    //Test string
    test();
  }
};

#endif
