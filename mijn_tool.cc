#include <iostream>
#include <string>

std::string translate(std::string line, std::string argument)
{ std::string result = ""; // implementeer dit
  std::string tmp = "";
  unsigned int i = 0;
  unsigned int lineSize = line.size();
  while (i < lineSize) {
    std::string newLine = "";
    for (unsigned int j = i; j < lineSize; j++) {
      if (line[j] == ' ') {
        i = j;
        break;
      }
      else {
        tmp = line[j];
        newLine.append(tmp);
      }
    }
    std::string tmp2 = "";
    tmp2 = newLine[0];
    newLine.erase(newLine.begin());
    newLine.append(tmp2 + argument + " ");
    result.append(newLine);
  }
  return result;
}

int main(int argc, char *argv[])
{ std::string line;

  if(argc != 2)
  { std::cerr << "Deze functie heeft exact 1 argument nodig" << std::endl;
    return -1; }

  while(std::getline(std::cin, line))
  { if(line == "EOF")
    { return 0; }
    std::cout << translate(line, argv[1]) << std::endl; } 

  return 0; }
