# Wire-Cam
This is code for a wire camera system like those in sports stadiums.

The code is for a personal, just-for-fun project.  It is written in C++, html, css, js, and jquery.

The frontend is a desktop application written with the Electron framework, allowing it to be written in html, css, and js.
The main loop of the program is run on a computer in C++, which communicates over a serial connection to an Arduino that tells the winces what speed to set.  The Arduino also sends data about encoders back to the computer.  The computer then communicates to the frontend via a socket.
