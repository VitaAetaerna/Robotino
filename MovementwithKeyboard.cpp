
#include <iostream>
#include <stdlib.h>

#include "rec/robotino/api2/all.h"

using namespace rec::robotino::api2;

bool _run = true;

class MyCom : public Com
{
public:
	MyCom()
		: Com( "example_wallfollow" )
	{
	}

	void errorEvent( const char* errorString )
	{
		std::cerr << "Error: " << errorString << std::endl;
	}

	void connectedEvent()
	{
		std::cout << "Connected." << std::endl;
	}

	void connectionClosedEvent()
	{
		std::cout << "Connection closed." << std::endl;
	}

	void logEvent( const char* message, int level )
	{
		std::cout << message << std::endl;
	}
};

class MyBumper : public Bumper
{
public:
	MyBumper()
		: bumped( false )
	{
	}

	void bumperEvent( bool hasContact )
	{
		bumped |= hasContact;
    if (true == bumped){
      std::cout << "Bumper has " << ( hasContact ? "contact" : "no contact") << std::endl;
    }
	}

	bool bumped;
};


MyCom com;
Bumper bumper;
OmniDrive omniDrive;

void init( const std::string& hostname )
{

	// Connect
	std::cout << "Connecting...";
	com.setAddress( hostname.c_str() );

	com.connectToServer( true );

	if( false == com.isConnected() )
	{
		std::cout << std::endl << "Could not connect to " << com.address() << std::endl;
		rec::robotino::api2::shutdown();
		exit( 1 );
	}
	else
	{
		std::cout << "success" << std::endl;
	}
}

void drive()
{
	while( com.isConnected() && false == bumper.value() && _run )
	{
    com.processEvents();
		//TODO: your code here
		//omniDrive.setVelocity( xspeed, yspeed, rotspeed );
		// void setVelocity(float 1.0, float 0, float 0)
		//com.processEvents();
		//rec::robotino::api2::msleep( 100 );
    // void setVelocity(float -1.0, float 0, float 0)
    // com.processEvents();
		// rec::robotino::api2::msleep( 100 );
    //void setVelocity(float 0, float 1.0, float 0)
    // com.processEvents();
		// rec::robotino::api2::msleep( 100 );
    // void setVelocity(float 0, float -1.0, float 0)
		// com.processEvents();
		// rec::robotino::api2::msleep( 100 );


    // FAHREN MIT TASTEN!!
	
    void InputDrive(){
	  char InputButton = ' ';

	  com.processEvents();


      if (InputButton == 'W'){
		void setVelocity(float 1.0, float 0, float 0)
        com.processEvents();
        rec::robotino::api2::msleep( 100 );
        InputDrive();
    }

      if (InputButton == 'S'){
		void setVelocity(float -1.0, float 0, float 0)
        com.processEvents();
        rec::robotino::api2::msleep( 100 );
        InputDrive();
    }


      if (InputButton == 'A'){
		void setVelocity(float 0, float -1.0, float 0)
        com.processEvents();
        rec::robotino::api2::msleep( 100 );
        InputDrive();
    }

      if (InputButton == 'D'){
		void setVelocity(float 0, float 1.0, float 0)
        com.processEvents();
        rec::robotino::api2::msleep( 100 );
        InputDrive();
    }

  



		com.processEvents();
		rec::robotino::api2::msleep( 100 );



    }
		InputDrive();

	}
}

void destroy()
{
	com.disconnectFromServer();
}

int main( int argc, char **argv )
{
	std::string hostname = "172.27.1.1";
	if( argc > 1 )
	{
		hostname = argv[1];
	}

	struct sigaction act;
	memset( &act, 0, sizeof( act ) );
	act.sa_handler = sigint_handler;
	sigaction( SIGINT, &act, NULL );

	//main program routine starts in here
	try
	{
		//connect to your robotino (or try at least)
		init( hostname );
		//drive (this contains a while loop)
		drive();
		//exit
		destroy();
	}
	catch( const rec::robotino::api2::RobotinoException& e )
	{
		std::cerr << "Com Error: " << e.what() << std::endl;
	}
	catch( const std::exception& e )
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
	catch( ... )
	{
		std::cerr << "Unknow Error" << std::endl;
	}

	rec::robotino::api2::shutdown();

#ifdef WIN32
	std::cout << "Press any key to exit..." << std::endl;
	rec::robotino::api2::waitForKey();
#endif
}
