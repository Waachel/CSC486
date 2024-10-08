// Capture The Flag
// AB 4/21

#include <iostream>
#include "CaptureTheFlag.h"
#include "NetworkManager.h"

void AdjustWindow()
{
	HWND hwndFound = GetConsoleWindow();
	RECT rc;
	GetWindowRect(GetDesktopWindow(), &rc);  // screen dimension in right and bottom
	
	// left (client) postion
	MoveWindow(hwndFound, 0, 0, rc.right / 2, rc.bottom / 2, true);

	// Right (server) position
	//MoveWindow(hwndFound, rc.right / 2, 0, rc.right / 2, rc.bottom / 2, true);
}

int main()
{
	// Uncomment to adjust window
	AdjustWindow(); // Positioning console window for convenience

	NetworkManager networkMan;
	networkMan.SetDuplicationProb(70);

	sockaddr_in TargetAddr;
	TargetAddr.sin_family = AF_INET;
	TargetAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // This computer see localhost https://en.wikipedia.org/wiki/Localhost
	TargetAddr.sin_port = htons(8888);

	srand((unsigned int) GetTickCount64());

	CaptureTheFlag game;
	CaptureTheFlag::MoveData DataP1;
	CaptureTheFlag::MoveData DataP2;
	char winner = CTFField::NoOneMarker;

	while (winner == CTFField::NoOneMarker) // this is the "game loop"
	{
		// Update phase
		// Update part 1: process user inputs for P2
			//Player 1
			DataP1.player = CaptureTheFlag::Player::Player1;
			DataP1.dir = (CaptureTheFlag::Direction)(rand() % 2);
			DataP1.val = (2 * (rand() % 2)) - 1; // either -1 or 1

			//Player 2
			//DataP2.player = CaptureTheFlag::Player::Player2;
			//DataP2.dir = CaptureTheFlag::Direction::Hor;
			//DataP2.val = 0;

		// Network
		networkMan.SendDataWithACK(TargetAddr, DataP1); // send P1 data
		networkMan.RcvDataWithACK(TargetAddr, DataP2); // receive P2 data
		
		// Update part 2: we can now update the game state
			game.Action(DataP1); //Move P1
			game.Action(DataP2); //Move P2
			game.UpdateField();       // Adjust markers on field 
			winner = game.TestForWinner();

		// Draw phase
		system("cls");
		game.Display();

		Sleep(100);			
		//system("pause");	// For development/lecture demo/Manual operation
	}
	printf("\tPlayer %c wins!!!\n\n", winner);

	networkMan.CleanUp();

	//system("pause");

	return 0;
}
