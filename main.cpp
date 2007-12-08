#include <iostream>
#include <iomanip>
using namespace std;
#include <windows.h>

#include "Clock.h"
#include "Recorder.h"
#include "ClockTimeSource.h"
#include "FileTimeSource.h"
#include "Timer.h"

void main() 
{
    // Create the clock
    Clock::Instance().Start( new ClockTimeSource );
    // Start the recorder
    Recorder::Instance().setFile( "output3.txt" );
    if( !Recorder::Instance().start() )
        cout << "Could not open output file for writing!" << endl;

    // Create a timer
    Timer *timer1 = Clock::Instance().GetTimer();
    timer1->setDuration(1.0);
    timer1->start();
    while(timer1->isRunning()) {
        Clock::Instance().FrameStep();
        //cout << "Last DeltaTime   : " << Clock::Instance().getLastDeltaTime() << endl;
        //cout << "Average DeltaTime: " << Clock::Instance().getDeltaTime() << endl;
        cout << "Current time     : " << Clock::Instance().getCurrentTime() << endl;
    }

    Recorder::Destroy();
    Clock::Destroy();

    //char m; cin >> m;

    // Read in the data from file time source
    Clock::Instance().Start( new FileTimeSource( "output3.txt" ) );
    //Recorder::Instance().setFile( "output4.txt" );
    //if( !Recorder::Instance().start() )
        //cout << "Could not open output file for writing!" << endl;

    do {
        Clock::Instance().FrameStep();
        //cout << "Last DeltaTime   : " << Clock::Instance().getLastDeltaTime() << endl;
        //cout << "Average DeltaTime: " << Clock::Instance().getDeltaTime() << endl;
        cout << "Current time     : " << Clock::Instance().getCurrentTime() << endl;
    }
    while(Clock::Instance().getCurrentTime()>0.0);

    Clock::Destroy();
    Recorder::Destroy();

    char k; cin >> k;
}