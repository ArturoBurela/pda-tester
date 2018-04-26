/*
Arturo Burela
*/
#ifndef STATE_HPP
#define STATE_HPP

#include <string>
#include <bitset>
#include <vector>
#include <iostream>
#include <queue>
#include <stack>

class State;

// Struct to represents links
struct link {
  char input;
  char stackCondition;
  char stackInput;
  State* destination;
  link() = default;
  link(char i, char sc, char si, State* d){
    input = i;
    stackCondition = sc;
    stackInput = si;
    destination = d;
  }
};

class State {
private:
  // State name
  std::string name;
  // If state is final
  std::bitset<1> end;
  // Vector of links to states
  std::vector<link> destinations;
public:
  State(){}
  State(std::string n){
    name = n;
  }

  // Add link to state
  void addLink(link l) {
    destinations.push_back(l);
  }

  // Returns state name
  std::string getName() {
    return name;
  }

  // Mark state as final
  void setFinal() {
    end.set();
  }

  void logString(std::queue<char> string) {
    while (!string.empty()) {
      std::cout << "--" << string.front() << '\n';
      string.pop();
    }
  }

  void logStack(std::stack<char> stack) {
    while (!stack.empty()) {
      std::cout << ":" << stack.top() << '\n';
      stack.pop();
    }
  }

  // Recursive function returns valid paths by testing string
  std::vector<std::vector<link>> explore(std::queue<char> string, std::stack<char> stack) {
    std::cout << '\n';
    std::cout << "CALLED IN STATE: " << name << '\n';
    logString(string);
    logStack(stack);
    // New paths to return
    std::vector<std::vector<link>> paths;
    std::vector<link> x;
    // Aux to tell if valid paths were found
    std::bitset<1> valid;
    // Get next input to test
    char input = string.front();
    std::stack<char> tmp = stack;
    std::queue<char> tmp2 = string;
    // First call on all epsilon links
    for (std::vector<link>::iterator it = destinations.begin(); it != destinations.end(); ++it){
      // If the link input is epsilon
      if (it->input == '&') {
        // If no condition for stack
        if (it->stackCondition == '&') {
          if (it->stackInput != '&') {
            tmp.push(it->stackInput);
          }
          // Get new vector of paths from next call
          std::vector<std::vector<link>> p = it->destination->explore(tmp2, tmp);
          // Iterate over paths and append current link
          for (std::vector<std::vector<link>>::iterator it2 = p.begin(); it2 != p.end(); ++it2){
            // If we receive valid paths append to current state
            if (it2->at(0).input != '\0') {
              valid.set();
              // Push current link to received path
              it2->push_back(*it);
              // Push path to paths
              paths.push_back(*it2);
            }
          }
        }
        // If stack condition is in the stack
        if (stack.top()==it->stackCondition) {
          tmp.pop();
          if (it->stackInput != '&') {
            tmp.push(it->stackInput);
          }
          // Get new vector of paths from next call
          std::vector<std::vector<link>> p = it->destination->explore(tmp2, tmp);
          // Iterate over paths and append current link
          for (std::vector<std::vector<link>>::iterator it2 = p.begin(); it2 != p.end(); ++it2){
            // If we receive valid paths append to current state
            if (it2->at(0).input != '\0') {
              valid.set();
              // Push current link to received path
              it2->push_back(*it);
              // Push path to paths
              paths.push_back(*it2);
            }
          }
        }
      } else if (it->input == input) { //Si el link concuerda con input
        tmp2.pop();
        // If no condition for stack
        if (it->stackCondition == '&') {
          if (it->stackInput != '&') {
            tmp.push(it->stackInput);
          }
          // Get new vector of paths from next call
          std::vector<std::vector<link>> p = it->destination->explore(tmp2, tmp);
          // Iterate over paths and append current link
          for (std::vector<std::vector<link>>::iterator it2 = p.begin(); it2 != p.end(); ++it2){
            // If we receive valid paths append to current state
            if (it2->at(0).input != '\0') {
              valid.set();
              // Push current link to received path
              it2->push_back(*it);
              // Push path to paths
              paths.push_back(*it2);
            }
          }
        }
        // If stack condition is in the stack
        if (stack.top()==it->stackCondition) {
          tmp.pop();
          if (it->stackInput != '&') {
            tmp.push(it->stackInput);
          }
          // Get new vector of paths from next call
          std::vector<std::vector<link>> p = it->destination->explore(tmp2, tmp);
          // Iterate over paths and append current link
          for (std::vector<std::vector<link>>::iterator it2 = p.begin(); it2 != p.end(); ++it2){
            // If we receive valid paths append to current state
            if (it2->at(0).input != '\0') {
              valid.set();
              // Push current link to received path
              it2->push_back(*it);
              // Push path to paths
              paths.push_back(*it2);
            }
          }
        }
      }
      tmp = stack;
      tmp2 = string;
    }
    //Stop condition when string is empty
    if (string.empty()) {
      // Check if state is final
      if (end.test(0)) {
        std::cout << "AQUÍ FUE ACEPTADO!..............................................................." << '\n';
        // Return a link with '\n' if accepted
        x.push_back(link('\n', '\n', false, this));
      } else {
        // Else return '\0'
        x.push_back(link('\0', '\0', false, this));
      }
      paths.push_back(x);
      return paths;
    }
    // If no valid paths were found append no valid path
    if (!valid.test(0)) {
      x.push_back(link('\0', '\0', false, NULL));
      paths.push_back(x);
    }
    return paths;
  }

  //Logs all state data
  void logData() {
    std::cout << "State name: " << name << '\n';
    std::cout << "Final state: " << end[0] << '\n';
    std::cout << "Destinantions: " << '\n';
    for (std::vector<link>::iterator it = destinations.begin(); it != destinations.end(); ++it){
      std::cout << "  Input: " << it->input << '\n';
      std::cout << "  stackCondition: " << it->stackCondition << '\n';
      std::cout << "  stackInput: " << it->stackInput << '\n';
      std::cout << "  Destination: " << it->destination->getName() << '\n';
      std::cout << "/* ------------------- */" << '\n';
    }
  }
};

#endif
