# # README.TXT

This project is a generic base for an ANSI-C project for the MC68HC908GP32.
This project is prepared to be designed with Processor Expert.

The 40-pin, 42-pin and 44-pin CPU MC68HC908GP32 is inserted into the Processor Expert project panel, 
in 6 configurations (Debug_40pin, Debug_42pin, Debug_44pin, Release_40pin, Release_42pin and Release_32pin). 
Change of the configuration is possible by the mouse double-click on it. 
All the installed Embedded Beans are accessible in the Bean Selector window, grouped into folders 
according to their function. The mouse double-click on selected Embedded Bean in the Bean Selector window
adds the Bean to the project. The mouse double-click on the Bean icon in the Project panel opens the Bean Inspector 
window, which is used to set the Bean properties. Source code is generated after 
selecting the Code Design 'Project_name.mcp' menu command from the CodeWarrior main window 
(Processor Expert | Code design 'Project_name.mcp'). 
Use the bean methods and events to write your code in the main module 'Project_name'.c
and the event module Events.c.

Switching among several PE projects already opened in IDE can be performed only this way:
Processor Expert | Suspend Processor Expert, 
Project | Set Default project | 'desired PE project', 
Processor Expert | Open Processor Expert for 'desired PE project'.


For more help please read Processor Expert help (Processor Expert | Help | 'Topic').


The following folders are used in CodeWarrior project window:
- User modules: contains your sources. The main module 'Project_name'.c and event module Events.c are located 
  here after the Processor Expert code generation.
- Debugger Cmd Files: Here you can place your own specific debugger command
  files.
- Prm: Linker parameter file used for linking. Note that the file used for the linker
  is specified in the Linker Preference Panel. To open the Preference Panel, please press  (<ALT-F7> or 
  open the Edit | 'Current Build Target Name' settings menu item in the CodeWarrior main window menu,
  while the project window is opened). After Processor Expert code generation 'Project_name'.prm file
  will be placed here.
- Libs: contains the ANSI library to be linked with.
- Debugger Project File: This *.ini file is passed to the debugger/simulator
  as configuration file. It contains various target interface settings, and path
  information as well. This file can be edited from the simulator/debugger e.g. using
  the "File | Configuration" menu item in the simulator/debugger main window menu.
- Generated code: this folder appears after the Processor Expert code generation and contains generated 
  code from Processor Expert.
- Doc: other files generated from the Processor Expert (documentation)

Available CodeWarrior Targets:

- P&E PEDebug FCS-ICS-ICD
  This target is set up to generate an application to be debugged 
  with the P&E PEDebug target interface in FCS (Full Chip Simulation),
  ICS (In-Circuit Simulation) or ICD (In-Circuit Debug/Programming). 

- MMDS-MMEVS
  This target is set up to generate an application to be debugged 
  with the Motosil target interface on MMDS/MMEVS. 

- MON08 (RAM)
  This target is set up to generate an application to be debugged 
  with the MON08 target interface (RAM debbuging). 

- MON08 (FLASH)
  This target is set up to generate an application to be debugged 
  with the MON08 target interface FLASH debbuging). 

- CodeWarrior Simulator
  This target is set up to generate an application to be debugged 
  with the CodeWarrior simulator. 
  
This project includes a burner command file (.bbl) in order to generate a 
SRecord file (.s19). Remove this file from prm folder if no SRecord is needed.

UNIS & Metrowerks
