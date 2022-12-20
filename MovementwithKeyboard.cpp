#include <iostream>
#include <stdlib.h>
#include "rec/robotino/api2/all.h"
#include <iostream>

using namespace rec::robotino::api2;
using namespace std;
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

double x_distance = 0;
double y_distance = 0;
double phi_absolute = 0;
unsigned int sequence = 0;

class MyOdometry: public Odometry{
  void readingsEvent(double x,double y, double phi, float vx, float vy, float omega, unsigned int seq ){
	cout << "X: " << x << " Y: " << y << " Richtung: " << phi << " Sequenz: " << seq << endl;  
	x_distance = x;
	y_distance = y;
	phi_absolute = phi;
	sequence = seq;
	}
};

MyCom com;
MyBumper bumper;
OmniDrive omniDrive;
MyOdometry odo;



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


bool DriveorRotate = true;   //TRUE == FAHRENB
void rotateLeft(){
	int Rotation = 0;
	int turning = 0;
	cout << "Enter Rotation (2.96 Ganze Umdrehung): ";
	cin >> Rotation;
	
	if (0 <= Rotation){
		turning = 1;
	}

	while (1 == turning){
		com.processEvents();
		omniDrive.setVelocity(0, 0, -0.4);
		cout << phi_absolute;
		rec::robotino::api2::msleep(100);
	    // cout << "X: " << x << " Y: " << y << "Richtung: " << phi << "Sequenz: " << s << endl;
		if (Rotation <= phi_absolute){
			turning = 0;
			
		}
	}
	com.processComEvents();
	

}

void rotateRight(){
	int Rotation = 0;
	int turning = 0;
	cout << "Enter Rotation (2.96 Ganze Umdrehung): ";
	cin >> Rotation;
	
	if (0 <= Rotation){
		turning = 1;
	}

	while (1 == turning){
		com.processEvents();
		omniDrive.setVelocity(0, 0, 0.4);
		cout << phi_absolute;
		rec::robotino::api2::msleep(100);
	    // cout << "X: " << x << " Y: " << y << "Richtung: " << phi << "Sequenz: " << s << endl;
		if (Rotation <= phi_absolute){
			turning = 0;
			
		}
	}
	com.processComEvents();
	

}




void drive()
{



	// Straight = 0, Backwards = 1, RightTop = 2, Right = 3, BottomRight = 4, BottomLeft = 5, Left = 6, TopLeft = 7
	
	



    char w;
  	odo.set(0,0,0,-1);
	

	while( com.isConnected() && false == bumper.value() && _run )
	{

	rec::robotino::api2::msleep(100); 
    
    

    
    	//TODO: your code here
		//omniDrive.setVelocity( xspeed, yspeed, rotspeed );



        system("stty raw");
        w = getchar(); 
        // terminate when "." is pressed
        system("stty cooked");
        system("clear");
        std::cout << w << " was pressed."<< std::endl;

        if (w == 'w'){
            omniDrive.setVelocity(0.2, 0, 0);
        }

        if (w == 'a'){
            omniDrive.setVelocity(0, -0.2, 0);
        }

        if (w == 's'){
            omniDrive.setVelocity(-0.2, 0, 0);
            
        }

        if (w == 'd'){
            omniDrive.setVelocity(0, 0.2, 0);
        }


        if (w == 'r'){
            rotateRight();
        }

        if (w == 'q'){
            rotateLeft();
        }

        if(w == '.') {
            system("stty cooked");
            exit(0);
        }  

    






	com.processEvents();
    rec::robotino::api2::msleep(10); 
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

