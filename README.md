# COMP3004 - Team 9: CES Device Simulator
**Project Overview:** A CES device simulator created for the team project in COMP 3004.

**Team Number:** 9

**Assigned TA:** Fazlay

**Member Names & Responsibility Breakdown:** 

Alex Chan - Design Documentation, Battery, Connection Test

Mariami Lomtadze - Design Documentation, Database, Save Session

Natalie Jeong - Design Documentation, User Interface, Power On/Off

Saki Futamura - Design Documentation, Session, Therapy

**GitHub Repository Link:** https://github.com/natjeo/3004-project

## File Organization:

The project directory is broken down as follows

- **README** current file outlining submission overview
- **Design Documentation** containing jpg files of use cases/ diagram, sequence diagrams, UML and traceability matrix
- **CESDevSim** source files (.h and .c) and supporting materials required to run a working version of our Qt c++ application
- **Team9_finalreport.pdf** file containing all submission requirements 

## Running Instructions:

1. Build program in Qt c++
2. Power on the device using the power button
3. Select intensity to begin a session using the up/ down button (duration and session type has been selected by default)
4. Select ok to begin session (hold for 2 seconds to save).
5. Upon completion, device will turn off.

## Tested Scenarios:

Functionality involving ear clips were not included as a part of our program.

- Connection Test passes by default
- Ear clips are assumed to be connected at all times

User Designed Sessions were not implemented due to *DAVID Session Editor* being required (sold separately).

All other requirements have been included.
