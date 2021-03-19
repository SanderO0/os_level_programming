#include "shell.hh"
#include <iostream>
#include <ostream>
#include <string>
#include <sys/syscall.h>
#include <unistd.h>

int main()
{ std::string input;

  // ToDo: Vervang onderstaande regel: Laad prompt uit bestand
  std::string prompt = "";
{ int fd = syscall(SYS_open, "prompt.txt", O_RDONLY, 0755); // Gebruik de SYS_open call om een bestand te openen.
  char byte[1];                                             // 0755 zorgt dat het bestand de juiste rechten krijgt (leesbaar is).
  while(syscall(SYS_read, fd, byte, 1))                     // Blijf SYS_read herhalen tot het bestand geheel gelezen is,
    prompt.append(byte);}                                    //   zet de gelezen byte in "byte" zodat deze geschreven kan worden.
  while(true)
  { std::cout << prompt;                   // Print het prompt
    std::getline(std::cin, input);         // Lees een regel
    if (input == "new_file") new_file();   // Kies de functie
    else if (input == "ls") list();        //   op basis van
    else if (input == "src") src();        //   de invoer
    else if (input == "find") find();
    else if (input == "seek") seek();
    else if (input == "exit") return 0;
    else if (input == "quit") return 0;
    else if (input == "error") return 1;

    if (std::cin.eof()) return 0; } }      // EOF is een exit

void new_file() // ToDo: Implementeer volgens specificatie.
{ std::string bestandsnaam = "";
  std::string textInput = "";
  std::string line = "";
  std::cout << "Voer de bestandsnaam in: ";
  std::getline(std::cin, bestandsnaam);
  std::cout << "Voer hieronder de text in: " << std::endl;
  while (line != "<EOF>")
  { std::getline(std::cin, line);
    if (line != "<EOF>")
    { textInput.append(line + "\n"); }}
  const char* text = textInput.c_str();
  int fd = syscall(SYS_creat, bestandsnaam.c_str(), 0755);
  syscall(SYS_write, fd, text, textInput.size());
}

void list() // ToDo: Implementeer volgens specificatie.
{ int procid = syscall(SYS_fork);
if(procid == 0)
{ const char* argumenten[] = {"/bin/ls", "-a", "-l", NULL};
  syscall(SYS_execve, argumenten[0], argumenten, NULL); }
else
{ syscall(SYS_wait4, procid, NULL, NULL, NULL); }
}

void find() // ToDo: Implementeer volgens specificatie.
{ int fileDesc[2];
  syscall(SYS_pipe, &fileDesc);
  std::string zoek = "";
  std::cout << "Voer hier in waar u naar zoekt: " << std::endl;
  std::getline(std::cin, zoek);
  int procid1 = syscall(SYS_fork);
  if(procid1 == 0)
  { syscall(SYS_close,fileDesc[0]);
    syscall(SYS_dup2, fileDesc[1], 1);
    const char* argumenten1[] = {"/usr/bin/find", ".", NULL};
    syscall(SYS_execve, argumenten1[0], argumenten1, NULL); }
  else
  { int procid2 = syscall(SYS_fork); 
    if(procid2 == 0)
    { syscall(SYS_close, fileDesc[1]);  
      syscall(SYS_dup2, fileDesc[0], 0);
      const char* argumenten2[] = {"/bin/grep", zoek.c_str(), NULL}; 
      syscall(SYS_execve, argumenten2[0], argumenten2, NULL); }
    else
    { syscall(SYS_close, fileDesc[0]);
      syscall(SYS_close, fileDesc[1]);
      syscall(SYS_wait4, procid1, NULL, NULL, NULL);
      syscall(SYS_wait4, procid2, NULL, NULL, NULL); }
  }
}

void seek() // ToDo: Implementeer volgens specificatie.
{ int zoek = syscall(SYS_creat, "seek", 0644);
  char x[1] = {'x'};
  syscall(SYS_write, zoek, x, 1);
  syscall(SYS_lseek, zoek, 5000000, 1);
  syscall(SYS_write, zoek, x, 1);
  char nul[1] = {'\0'};
  int loop = syscall(SYS_creat, "loop", 0644);
  syscall(SYS_write, loop, x, 1);
  for (unsigned int i = 0; i < 5000000; i++)
  { syscall(SYS_write, loop, x, 1); }  
  syscall(SYS_write, loop, x, 1);
} // De files verschillen niet van grootte, maar seek was sneller

void src() // Voorbeeld: Gebruikt SYS_open en SYS_read om de source van de shell (shell.cc) te printen.
{ int fd = syscall(SYS_open, "mijn_shell.cc", O_RDONLY, 0755); // Gebruik de SYS_open call om een bestand te openen.
  char byte[1];                                           // 0755 zorgt dat het bestand de juiste rechten krijgt (leesbaar is).
  while(syscall(SYS_read, fd, byte, 1))                   // Blijf SYS_read herhalen tot het bestand geheel gelezen is,
    std::cout << byte; }                                  //   zet de gelezen byte in "byte" zodat deze geschreven kan worden.
