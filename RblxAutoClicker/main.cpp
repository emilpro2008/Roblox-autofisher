#include <iostream>
#include <Windows.h>
#include <thread>
#include <vector>
#include <conio.h>

// Handle to the window, in this case, Roblox.
HWND hWND;
// Boolean variable to allow one keypress at a time to prevent starting and stopping multiple times
bool keydown = false;

std::string versionNumber = "1.4.0";

// Struct for storing analytical data about fishing
struct DataFish {
	float minFishTime = 0;
	float maxFishTime = 0;
	float avgFishTime = 0;
};

// Returns last error as string instead of returning a numeric error code
static std::string GetLastErrorAsString() {
	// Stores the last error code as a dword variable
	DWORD errorMessageID = ::GetLastError();
	// If the error code is zero, return an empty string
	if (errorMessageID == 0) {
		// Return empty string
		return std::string();
	}
	// Long Pointer to a STRing, basically a char* called messageBuffer initialized as a nullptr
	LPSTR messageBuffer = nullptr;
	// Specifies the size of the string
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
	// Creates a string from the char*
	std::string message(messageBuffer, size);
	// Frees the messageBuffer object
	LocalFree(messageBuffer);
	// Returns the message
	return message;
}

// 
static void ConsoleGotoCoord(int x, int y) {
	// Create a structure holding the x and y coordinates
	COORD coord;
	coord.X = x;
	coord.Y = y;

	// Stores a handle to the standard output device, the console
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// If it wasn't able to set the cursor position at the specified location then enter the statement
	if (!SetConsoleCursorPosition(hConsole, coord))
	{
		// Print the error message as a string instead of integer using the the GetLastErrorAsString() function
		std::cout << GetLastErrorAsString() << std::endl;
	}
}

// Console class 
class Console {
public:
	// Constructor with argument defining which line to start to print at
	Console(int offset) {
		// Move the data in the offset variable into the member offset variable
		m_offset = offset;
		// Initialize the m_sameprintcount to 1
		m_sameprintcount = 1;
	}

	// Prints a string where it prints the last message to the top 
	// Multiple of the same message in a row results in a string x 2,3,4,5...
	void Print(std::string str) {
		// Creating a struct object
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		// Here we exctract info about the console and pass it to the csbi object
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		// Calculates the amount of rows and columns in the console window and stores the numbers in the m_rows and m_columns variables respectively
		m_columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		m_rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

		// if the string is the same as the last string then 
		if (str == m_lastprint) {
			// Set the m_sameprintcount to m_sameprintcount + 1
			// This holds the amount of times the same string has been printed in a row
			m_sameprintcount++;
			// Go to the coords 0, m_offset where m_offset is the offset specified when the class object is created
			ConsoleGotoCoord(0, m_offset);
			// First, clears the line about to be printed at, as there's a chance that there's already text on the console
			// Second, concatenates the string, char 158 which is a ASCII muliplication sign and the m_sameprintcount
			// Third, the concatenated string is then printed
			std::cout << "\33[2K" << str + (char)158 + std::to_string(m_sameprintcount) << std::endl;
			// Store the concatenated string in the consoleOutput vector at the index consoleOutput's size - 1
			consoleOutput.at(consoleOutput.size()-1) = (str + (char)158 + std::to_string(m_sameprintcount));
		}
		// If string isnt the last string printed, then
		else {
			// Sets m_sameprintcount to 1, indicating that a new string, that isn't the same as the last one, is being printed
			m_sameprintcount = 1;
			// Set the console cursor to the coordinates x: 0, y: m_offset
			ConsoleGotoCoord(0, m_offset);

			// If theres more than one message printed then
			if (consoleOutput.size() >= 1) {
				// Push the new string message into the consoleOutput vector
				consoleOutput.push_back(str);

				// Initialize i to zero, while i is less than or equal to consoleOutput's size - 1 loop and then post increment i
				// This basically loops consoleOutput.size-1 times. Minus one as the size function returns the amount of elements in
				// the vector ie between one and inf while the index is between zero and inf-1
				for (int i = 0; i <= consoleOutput.size()-1; i++) {
					// Print the contents of consoleOutput at index consoleOutput.size - 1 - i and clears the 
					std::cout << "\33[2K" << consoleOutput.at(consoleOutput.size() - 1 - i) << std::endl;
				}
			}
			// if there isn't a message in the console yet, then
			else {
				// Clear the line and print the string specified in the function at the line we went to earlier
				std::cout << "\33[2K" << str << std::endl;
				// Push the string into the consoleOutput vector
				consoleOutput.push_back(str);
			}
		}
		// After all this, set the m_lastprint to the string that's been printed
		m_lastprint = str;
	}

	// Function meant to draw an horizontal line across the console.
	// 'y' is the y coordinate to draw on and 'c' is a char which is a character you can choose to use to create a line with
	void LineH(int y, char c) {
		// Creating a struct object
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		// Here we exctract info about the console and pass it to the csbi object
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		// Calculates the amount of rows and columns in the console window and stores the numbers in the m_rows and m_columns variables respectively
		m_columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		m_rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

		// While i is less than m_columns then
		for (int i = 0; i < m_columns; i++) {
			// Go to the coordinates x: 'i', y: 'y'. 'i'.
			ConsoleGotoCoord(i, y);
			// Print the character choosen as a argument
			std::cout << c << std::endl;
		}
		
	}
// Private members
private: 
	// m_offset, used to remember what line the console starts.
	int m_offset;
	// Stores the last printed string in a variable called m_lastprint
	std::string m_lastprint;
	// m_sameprintcout keeps track of the amount of times the same string has been printed in a row
	int m_sameprintcount;
	// Stores all the printed messages in this string vector called consoleOutput
	std::vector<std::string> consoleOutput;

	// Variables for storing the amount of rows and columns
	int m_rows = 0;
	int m_columns = 0;
};


// Extracts the Pixel color of a specified pixel as a int vector called pos
static std::vector<int> GetPixelColor(std::vector<int>pos) {
	// Retrieves a handle to a device context (DC) for the entire screen and saves it in a variable
	HDC dng = GetDC(NULL);
	// Variable holding the color of the pixel at 'pos' which is done using the GetPixel function 
	COLORREF col = GetPixel(dng, pos.at(0), pos.at(1));
	// Frees the DC 
	ReleaseDC(hWND, dng);
	// Returns the color values at the position specified as a int type vector
	return {GetRValue(col), GetGValue(col), GetBValue(col) };
}

// Returns a bool and requires 3 arguments, a int vector called pos, a int vector called rgb and an int called avgrange
static bool IsPxInRange(std::vector<int>pos, std::vector<int>rgb, int avgrange) {
	// Creates a int vector called pxcol holding the color of the pixel at the coordinates specified
	// in the 'pos' argument using the GetPixelColor function created
	std::vector<int> pxcol = GetPixelColor(pos);
	
	// This basically does vector subtraction and then adds all the values in the new vector together and then divides the value by 3
	// resulting in the average difference between the two color vectors.
	// After the subtraction we take the absolute values of the 3 results so that there isnt a negative value present
	float rgbdiff = ((unsigned int)(pxcol.at(0) - rgb.at(0)) + (unsigned int)(pxcol.at(1) - rgb.at(1)) + (unsigned int)(pxcol.at(2) - rgb.at(2))) / 3;

	// Returns 1 if the rgbdiff variables number is less than the avgrange requires ie the max difference between the pixel color
	// and the color required.
	// TL:DW this returns 1 if the rgb value wanted to be matched is close enough to the pixel color and returns zero if those
	// requirements aren't met.
	return (rgbdiff < avgrange) ? 1 : 0;
}

// left clicks at specifed coords on the screen
static void LeftClick(int x, int y) {
	// Creates a _INPUT Variable
    INPUT _INPUT = { 0 };
	// Specifies the type of input, in this case mouse input
	_INPUT.type = INPUT_MOUSE;
	// Sets the flags about movement and position and a flag that tells the system that leftclick is down
	_INPUT.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN;
	// Specifies the x and y coordinates to click
	_INPUT.mi.dx = x;
	_INPUT.mi.dy = y;
	// Click on the screen with the data specified before
	SendInput(1, &_INPUT, sizeof(_INPUT));

	std::this_thread::sleep_for(std::chrono::milliseconds(1));

	// Clears the memory for the _INPUT variable
	ZeroMemory(&_INPUT, sizeof(_INPUT));
	// Again, specifies the type of input, in this case mouse input
	_INPUT.type = INPUT_MOUSE;
	// Sets the flag that tells the system that leftclick is up
	_INPUT.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTUP;
	// Specifies the x and y coordinates to click
	_INPUT.mi.dx = x;
	_INPUT.mi.dy = y;
	// Send the data from the _INPUT to the SentInput function to tell the system that we released leftclick
	SendInput(1, &_INPUT, sizeof(_INPUT));
}

// Checks if different keys are pressed
static void CheckKeyPress(bool& Running, HWND hWND, int& xConverted, int& yConverted) {
	// Variable holding x, y data
	POINT p;
	// If the F6 key is pressed then
	if (GetAsyncKeyState(VK_F6)) { // Start stop
		// If key is down then
		if (!keydown) {
			// Invert the running bool
			Running = !Running;

			// Go to the coordinates specified
			ConsoleGotoCoord(0, 8);
			// Saves the state of the program in a string where it says Running if the 'Running' bool is true and otherwise says "Stopped"
			// It does this in one line using the ternary operators
			std::string state = (Running) ? "Running" : "Stopped";
			// Now it prints the current state of the program to the console at the specified coordiantes
			std::cout << "State: " << state << std::endl;

			// We set keydown to true to let the program know that the key being pressed is down
			keydown = true;
		}
	}
	// Else if F7 is pressed then
	else if (GetAsyncKeyState(VK_F7)) { // Set Cursor Position
		// If key is down then
		if (!keydown) {
			// We get the cursor position at it's put into the p variable
			GetCursorPos(&p);
			// Converts screen coords to window area coords
			ScreenToClient(hWND, &p);

			// Move typing cursor to line 9 and print the coords of where the cursor is located on the screen
			ConsoleGotoCoord(0, 9);
			std::cout << "x: " << p.x << " | y: " << p.y << std::endl;

			// Get total screen coordinates
			int screen_x = GetSystemMetrics(SM_CXSCREEN);
			int screen_y = GetSystemMetrics(SM_CYSCREEN);

			// Holds info about the window
			WINDOWPLACEMENT wp{};
			GetWindowPlacement(hWND, &wp);

			// xConverted and yconverted are variable which are able to set the position to click on the screen
			xConverted = (65535 * (p.x + wp.rcNormalPosition.left)) / screen_x;
			yConverted = (65535 * (p.y + wp.rcNormalPosition.top)) / screen_y;
			
			// We set keydown to true to let the program know that the key being pressed is down
			keydown = true;
		}

	}
	// Else if the F8 key is pressed then
	else if (GetAsyncKeyState(VK_F8)) { // Exit
		// If key is down then
		if (!keydown) {
			// We set keydown to true to let the program know that the key being pressed is down
			keydown = true;
			// Exits the program
			exit(0);
		}
	}
	else {
		// Set keydown to false if no key was pressed
		keydown = false;
	}
}

static void cui() {
	// Print info onto the console starting at the first line
	ConsoleGotoCoord(0, 0);
	// Copyright
	std::cout << "Copyright 2024 by Emil Nygaard Poulsen. All rights reserved." << std::endl;
	// Version
	std::cout << "v1.3.3 - Stable\n\n" << std::endl;

	// How to use the program
	std::cout << "Start / Stop: F6\n";
	std::cout << "Set Position: F7\n";
	std::cout << "Close:        F8\n" << std::endl;

	// The current state of the autofisher
	std::cout << "State: Stopped" << std::endl;
}

int main() {
	// Dont remember why this is here but its used to allocate a new console
	AllocConsole();
	// Sets the console title
	SetConsoleTitleA("Roblox Auto Fisher - Intended for Pet Simulator 99");

	// Creates a hWND with a handle to the Roblox window
	HWND hWND = FindWindow(nullptr, L"Roblox");
	// If the hWND is NULL, indicating that it couldn't find the roblox process, then
	while (hWND == NULL) {
		// Try to create hWND with handle to the roblox window repeatedly until it finds it
		hWND = FindWindow(nullptr, L"Roblox");
		// Prints "Start Roblox!"
		std::cout << "Start Roblox!" << std::endl;
		// Sleeps for 1 second until it loops again
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	system("cls"); // Not very safe   Replace in future

	// Creates objects about Window placement, fishing data and console object
	WINDOWPLACEMENT wp{};
	DataFish datafish{0};
	Console console{17};

	// Prints text to the console
	cui();
	console.LineH(2, (char)196);
	ConsoleGotoCoord(0, 11);
	std::cout << "Time: " << "N/A" << std::endl;
	std::cout << "Min: " << "N/A" << std::endl;
	std::cout << "Max: " << "N/A" << std::endl;
	std::cout << "Avg: " << "N/A" << std::endl;
	console.LineH(16, (char)196);

	// Puts the data about window placement into the wp variable
	GetWindowPlacement(hWND, &wp);
	// Calculates the standard xConverted and yConverted values if no click position is specified
	int xConverted = (65535 * (1920 / 2 + wp.rcNormalPosition.left)) / GetSystemMetrics(SM_CXSCREEN);
	int yConverted = (65535 * (1080 / 2 + wp.rcNormalPosition.top)) / GetSystemMetrics(SM_CYSCREEN);
	// Print x and y coordinates in the console at the coords 0,9
	ConsoleGotoCoord(0, 9);
	std::cout << "x: " << 1920 / 2 << " | y: " << 1080 / 2 << std::endl;

	// Initialize the values
	datafish.minFishTime = FLT_MAX;
	datafish.maxFishTime = FLT_MIN;
	datafish.avgFishTime = 0;

	// Initialize a variable called CastFails to 0. This variable holds track of the amount of times the program fails to cast the rod.
	int CastFails = 0;
	// Running variable is used to keep track of the programs state, if its running or not
	bool Running = false;
	// Looping indefinitely until exited by the CheckKeyPress function
	while (true) {
		// If it isn't running then
		if (!Running) {
			// Check for keypresses and sets the Running variable to 1 if the key to start is pressed
			CheckKeyPress(Running, hWND, xConverted, yConverted);
			// Sleep for 5 ms to avoid over utilization of the cpu
			std::this_thread::sleep_for(std::chrono::milliseconds(30));
		}
		// else if it's running
		else {
			// Wait 800 ms which is the time needed to be able to case the fishing rod again
			console.Print("Before");
			std::this_thread::sleep_for(std::chrono::milliseconds(800));
			console.Print("After");
			// Cast the fishing rod by emulating a click on the screen using the LeftClick function 
			LeftClick(xConverted, yConverted);
			// We print to the console using the console.Print telling the user that the fishing rod has been casted.
			// This is a smart move as it's able to detect when the same message has been sent multiple times in a row, resulting in
			// a less cluttered console window
			console.Print("Casted Fishing Rod");

			// Sleep for 4.5 seconds as thats the time i've found the most reliable to catch all fish and not miss a single one
			std::this_thread::sleep_for(std::chrono::milliseconds(4500));

			// Creates a variable storing data about the current time
			const clock_t begint = clock();
			// Then it loops while the color of the pixel at the position specified isn't close enough to the color required
			// Here we have a 1 stating we at most want the pixels off by one each to return 1 that means that one pixel could be off
			// by 3 and the others off by zero and it would still return 1, more than that and it would return zero.
			// So while the pixel color isnt a bright green at the position specified, we loop
			while (!IsPxInRange({ 1487, 838 }, { {109, 255, 37} }, 1)) {
				// Repeatedly leftclick at the coordinates specified
				LeftClick(xConverted, yConverted);
				// Sleep for 5 ms to avoid over utilization of the cpu
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				// Check for keypresses and do something if F6, F7 or F8 is pressed
				CheckKeyPress(Running, hWND, xConverted, yConverted);
				// If you've been in this loop for more than 6 six seconds indicating that something went wrong
				// and it's stuck casting, then
				if ((static_cast<float>(clock() - begint) / CLOCKS_PER_SEC) > 6.5) {
					// Increments with every time it fails to cast the fishing rod
					CastFails++;
					// Sleep for 2 seconds
					std::this_thread::sleep_for(std::chrono::milliseconds(3500));
					// Break out of loop
					break;
				}
			}
			// Calculate the time the loop lasted which should be the time it took to reel in
			float time = static_cast<float>(clock() - begint) / CLOCKS_PER_SEC;
			// If time is more than the maxFishTime, then
			if (time > datafish.maxFishTime)
				// Set maxFishTime to time 
				datafish.maxFishTime = time;
			// If time is less than minFishTime then
			if (time < datafish.minFishTime)
				// Set minFishTime to time
				datafish.minFishTime = time;
			// If avgFishTime is zero set it to time else set it to (avgFishTime + time) / 2
			datafish.avgFishTime = (datafish.avgFishTime == 0) ? time : (datafish.avgFishTime + time) / 2;

			// Go to the coords 0, 11
			ConsoleGotoCoord(0, 11);
			// Prints the Time taken to reel in and clears the current line
			std::cout << "\33[2K" << "Time: " << time << "s" << std::endl;
			// Prints the current minimum time it took to reel in and clears the current line
			std::cout << "\33[2K" << "Min: " << datafish.minFishTime << "s" << std::endl;
			// Prints the current maximum time it took to reel in and clears the current line
			std::cout << "\33[2K" << "Max: " << datafish.maxFishTime << "s" << std::endl;
			// Prints the current avg time it took to reel in and clears the current line
			std::cout << "\33[2K" << "Avg: " << datafish.avgFishTime << "s" << std::endl;
			// Prints the amount of failed casts.
			std::cout << "\33[2K" << "Failed casts: " << CastFails << std::endl;
		}
	}
	// Returns zero indicating that the program stopped with no errors
	return 0;
}