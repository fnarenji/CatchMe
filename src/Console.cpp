/**
 * @file   Console.cpp
 * 
 * @author F. Narenji, O. Richit, H. Morales, V. Pelegrin
 *
 * @date   12/01/2014
 *
 * @brief  Console manipulation (colors, timed-out noncanonical input, VT100)
 *
 **/
#include <sys/ioctl.h>
#include <cstdio>
#include <termios.h>
#include <unistd.h>
#include <poll.h>

#include "Console.h"

using namespace std;

std::ostream& nsConsole::operator<< (std::ostream& os, const Colors& Color)
{
    return os << "\033[" << static_cast<unsigned> (Color) << "m";
} // operator<< Colors

std::ostream& nsConsole::operator<< (std::ostream& os, const BackgroundColors& Color)
{
    return os << "\033[" << static_cast<unsigned> (Color) << "m";
} // operator<< BackgroundColors

void nsConsole::ClearScreen ()
{
    cout << "\033[H\033[2J";
} // ClearScreen

// http://www.cplusplus.com/forum/general/5304/#msg23940
bool nsConsole::WaitForKeyPress (const unsigned TimeOut)
{
    pollfd pls[1];
    
    pls[0].fd     = STDIN_FILENO;
    pls[0].events = POLLIN | POLLPRI;
    
    return poll (pls, 1, TimeOut ) > 0;
} // WaitForKeyPress

// http://stackoverflow.com/a/1022961
void nsConsole::GetScreenSize (unsigned& x, unsigned& y)
{
    winsize w;

    ioctl (STDOUT_FILENO, TIOCGWINSZ, &w);

    x = w.ws_col;
    y = w.ws_row;
} // GetScreenSize

// Restore previous mode
void EnableCanonicalInputMode ()
{
    termios term;

    tcgetattr (STDIN_FILENO, &term);
    term.c_lflag |= ICANON | ECHO;
    tcsetattr (0, TCSANOW, &term);
} // EnableCanonicalInputMode

// Manipulating the terminal requires sacrificing C++ :/
void nsConsole::DisableCanonicalInputMode ()
{
    termios term;

    tcgetattr (STDIN_FILENO, &term); // Get terminal attributes
    term.c_lflag &= ~(ICANON | ECHO); // Remove canonical mode, echo
    tcsetattr (STDIN_FILENO, TCSANOW, &term); // Set modified terminal attributes

    cin.sync_with_stdio(); // This is necessary for non-blocking timed I/O using poll.

    atexit (EnableCanonicalInputMode); // Once the program exits, put it back the way it was before
} // DisableCanonicalInputMode

