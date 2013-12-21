#include "GameUI.h"

#include <iostream>

using namespace std;

void Game::UI::ShowMatrix (const CMatrix & Matrix)
{
    Console::ClearScreen ();

    // Upper border
    for (unsigned i = 0; i < KMatrixSize.second + 2; ++i)
        cout << BackgroundColors::KGreen << ' ';

    cout << BackgroundColors::KDefault << endl;
    
    for (CLine Line : Matrix)
    {
        // Left border
        cout << BackgroundColors::KGreen << ' ' << BackgroundColors::KDefault;

        for (char C : Line)
        {
            try
            {
                cout << KTokenColors.at(C).second << KTokenColors.at(C).first; // We use at() for const-correctness
                //cout << unsigned(KTokenColors.at(C).first) << " LOL " << unsigned(KTokenColors.at(C).second);
            }
            catch (out_of_range e)
            {
                cerr << "Color information for token '" << C << "' is missing. Assuming default colorset." << endl;
                cout << Colors::KDefault << BackgroundColors::KDefault;
            }

            cout << C;
        }

        // Right border
        cout << BackgroundColors::KGreen << ' ' << BackgroundColors::KDefault << endl;
    }

    // Bottom border
    cout << BackgroundColors::KGreen;
    for (unsigned i = 0; i < KMatrixSize.second + 2; ++i)
        cout << ' ';

    cout << BackgroundColors::KDefault << endl;
}

void Game::UI::ShowControls(const unsigned int CurrentPlayer)
{
    cout << endl << "Current player: " << KTokens.at(CurrentPlayer)
        << endl << "Controls: " << endl
        << "UpLeft(" << KControlsByToken.at(KTokens.at(CurrentPlayer))[0] << ") "
        << "Up(" << KControlsByToken.at(KTokens.at(CurrentPlayer))[1] << ") "
        << "UpRight(" << KControlsByToken.at(KTokens.at(CurrentPlayer))[2] << ") " << endl
        << "Left(" << KControlsByToken.at(KTokens.at(CurrentPlayer))[3] << ") "
        << "Stay(" << KControlsByToken.at(KTokens.at(CurrentPlayer))[4] << ") "
        << "Right(" << KControlsByToken.at(KTokens.at(CurrentPlayer))[5] << ") " << endl
        << "DownLeft(" << KControlsByToken.at(KTokens.at(CurrentPlayer))[6] << ") "
        << "Down(" << KControlsByToken.at(KTokens.at(CurrentPlayer))[7] << ") "
        << "DownRight(" << KControlsByToken.at(KTokens.at(CurrentPlayer))[8] << ") " << endl;
}