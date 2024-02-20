// Michael Fleagle
// CS 470

// Honor Code:
// "I pledge that I have neither given nor recieved help from anyone other than the instructor/TA for all program components included here!"
// - Michael Fleagle

#include<stdio.h>
#include<iostream>
#include<vector>
#include<cstring>
#include<sys/stat.h>
#include<sys/statfs.h>


// STD prefix for ease
using namespace std;


// method to split combined switches
vector<string> splitSwitches(vector<string> &args)
{
	// for loop to iterate through all strings in vector
	for(int i = 0; i < args.size(); i++)
	{
		// check if any of the switches contain more than one element together (example -ie -> -i -e)
		if(args.at(i).find('-') != string::npos)
		{
			// '-' found
			// switch cannot be the first argument in the command
			if(i > 0)
			{
				// check whether a switch is combined (size > 2)
				if(args.at(i).length() > 2 && args.at(i) != "--help")
				{
					// there is a combined switch that is not the help command
					string temp;
					
					// create string to hold the orignial string value
					string nonParsed = args.at(i);

					// split the string into individual arguments
					char splitSwitch[args.at(i).length()];
					
					strcpy(splitSwitch, args.at(i).c_str()); // not working
					
					// take the first two charcters which should be "-" + "CHAR"
					temp = splitSwitch[0];
					temp.push_back(splitSwitch[1]);
					
					// change the current .at(i) to be the temp string
					args.at(i) = temp;
					
					// update the rest of the switches
					for(int j = 2; j < (nonParsed.length()); j++)
					{
						// clear the previous temp
						temp.erase(); 
						temp = "-";
						temp.push_back(splitSwitch[j]);
						
						// insert the temp into the vector of commands
						args.insert(args.begin() + (i + (j-1)), temp);
					}
				}
				else
				{
					// there is not a combined switch or it is the help command
					continue;
				}
			}
			else
			{
				continue;
			}
		}
		else
		{
			// '-' not found
			continue;
		}
	}
	return args;
}

// method to help remove the extra vector spaces
vector<string> removeExtra(vector<string> &args)
{
	vector<string> tempVec;
	for(int i = 0; i < args.size(); i++)
	{
		if(args.at(i) != "")
		{
			tempVec.push_back(args.at(i));
		}
	}
	
	return tempVec;
}

// method to parse the commands into sepearte strings for ease of reading
vector<string> parseCommands(string commands)
{
	// add a space to the end of the input to make parsing easier
	commands.append(" ");
	
	// create vector of strings to hold commands and command arguments
	vector<string> parsedCommands;
	vector<string> updatedParseCommands;
	
	while(commands.length() > 0)
	{
		parsedCommands.push_back(commands.substr(0, (commands.find(" "))));
		commands.erase(0, (commands.find(" ") + 1));
	}
	
	// pass the commands to the split switch function to split any combined switches Ex: [-iee] -> [-i][-e][-e]
	parsedCommands = splitSwitches(parsedCommands);
	
	updatedParseCommands = removeExtra(parsedCommands);
	
	// return the parsed commands
	return updatedParseCommands;
}

// method that sends the commands to the system() function
int sysCommands(string args)
{
	// int variable to hold returned value from system()
	int sysReturn;
	
	// convert the string to a char array
	char commandChar[args.length()  + 1];
	
	// put string into char array to pass to system()
	strcpy(commandChar, args.c_str());
	
	sysReturn = system(commandChar);
	
	return sysReturn;
	
}

// method to handle the local exit command
int exitCommand(vector<string> commands, int returnVal)
{
	int exitVal;
	
	// check if there are switches
	if(commands.size() > 2)
	{
		// print error command for too many arguments
		cout << "Error: Too many arguments. Type exit --help for more details" << endl;
	}
	else if(commands.size() == 2)
	{
		if(commands.at(1) != "--help")
		{
			// make sure that the value input is an integer
			try {
				exitVal = stoi(commands.at(1));
			}
			catch (std::invalid_argument excpt1)
			{
				// print error if not a valid arugment in stoi
				cout << "Error: Argument is not an integer value. Type exit --help for more details" << endl;
				return -1;
			}
			return exitVal;
		}
		else
		{
			// print the help command for the exit command
			cout << "exit [n] -- terminates the shell causing a return from the shell's main(). If an argument (n) is given, it will be the shell's exit value" << endl;
			return -1;
		}
	}
	// if the  command size is less than 2
	return returnVal;
}

// mehtod to handle the local prompt command
string promptCommand(vector<string> args, string curPrompt)
{
	// string to hold prompt
	string updatePrompt;
	
	// check if there is a new prompt of if it is a reset
	if(args.size() == 1)
	{
		// set the prompt to the default and return
		updatePrompt = "cwushell";
		return updatePrompt;
	}
	else if(args.size() == 2)
	{// fix this to handle errors
		if(args.at(1) == "--help")
		{
			// print the help command for the prompt command
			cout << "prompt [new_prompt] -- will change the cuurent shell prompt to the new_prmpt. Typing prompt restores the shell's default prompt" << endl;
		}
		else
		{
			// return the new shell
			return args.at(1);
		}
	}
	else
	{
		cout << "Error: Too many arguments. Type prompt --help for more details" << endl;
	}
	return curPrompt;
}

// method to handle the local fileinfo command
int fileInfoCommand(vector<string> args)
{
	if(args.size() < 2)
	{
		cout << "Error: Too few arguments. Type fileinfo --help for more details" << endl;
		
		// return 0 for failed to work
		return 0;
	}
	else if(args.size() > 5)
	{
		cout << "Error: Too many arguments. Type fileinfo --help for more details" << endl;
		// return 0 for failed to work
		return 0;
	}
	else
	{
		// create array to hold the filename
		char fileName[args.at(args.size()-1).length()];
		// copy the filename from the array to the character array
		strcpy(fileName, args.at(args.size() - 1).c_str());
		// create struct stat to get file information
		struct stat buf;
		
		// if statement for when only 2 arguments are passed
		if(args.size() == 2)
		{
			if(args.at(1) == "--help")
			{
				// print the help command for the fileinfo command
				cout << "fileinfo -[switch] filename -- will print on the screen different file related information based on the switch." << endl;
				cout << "\tfileinfo switches:" << endl;
				cout << "\t\t-i -- will print the inode numberof the file" << endl;
				cout << "\t\t-t -- will print the type of the file pointed by filename (ex. regular file, directory, symlink)" << endl;
				cout << "\t\t-m -- will print the last modification date of th efile pointed by filename" << endl;
				cout << "\tIf no switch is specified, all three switches will be output." << endl << endl;
				
				return 1;
			}
			else
			{
				args.insert((args.begin() + 1), "-m");
				args.insert((args.begin() + 1), "-t");
				args.insert((args.begin() + 1), "-i");
			}
		}
		
		// pass the file to stat to see if the file even exists
		int val;
		val = stat(fileName, &buf);
		if(val < 0)
		{
			cout << "Error: " << fileName << " not found." << endl;
			return 0;
		}
		else
		{
			// print the filename before any information prints
			cout << "Filename: " << fileName << endl;
			
			// loop to go through all given switches/arguments
			for(int i = 1; i < args.size(); i++)
			{
				if(args.at(i).find('-') != string::npos)
				{

					if(args.at(i) == "-i")
					{
						// print the inode value of the given file
						cout << "Inode value: " << buf.st_ino << endl;
					}
					else if(args.at(i) == "-t")
					{
						string fileType;
						
						// switch to determine the time of file
						switch(buf.st_mode & S_IFMT)
						{
							case S_IFBLK:
								fileType = "block device";
								break;
							case S_IFCHR:
								fileType = "character device";
								break;
							case S_IFDIR:
								fileType = "directory";
								break;
							case S_IFIFO:
								fileType = "FIFO/pipe";
								break;
							case S_IFLNK:
								fileType = "symlink";
							case S_IFREG:
								fileType = "regular file";
								break;
							case S_IFSOCK:
								fileType = "socket";
							default:
								fileType = "unknown";
								break;
						}
						
						// print the type of the file pointed at by filename
						cout << "File type: " << fileType << endl;
					}
					else if(args.at(i) == "-m")
					{
						string time;
						const time_t timeLong = buf.st_mtime;
						
						// convert the time long into a readable string format
						time = ctime(&timeLong);

						// print the last modification date and time
						cout << "Last modification date and time: " << time;
					} 
					else
					{
						// argument not found
						cout << "Error: Switch not recognized. Type fileinfo --help for more details" << endl;
						return 0;
					}
				}
			}
		}
	}
	return 0;
}

// method to handle the local osinfo command
int osinfoCommand(vector<string> args)
{
	if(args.size() < 2)
	{
		cout << "Error: Too few arguments. Type osinfo --help for more details" << endl;
		
		// return 0 for failed to work
		return 0;
	}
	else if(args.size() > 5)
	{
		cout << "Error: Too many arguments. Type osinfo --help for more details" << endl;
		// return 0 for failed to work
		return 0;
	}
	else
	{
		// create statvfs buffer
		struct statfs blocks;
		// struct statfs availBlocks;
		struct statfs nameLen;
		struct statfs fs;
		
		// Add loop here to go through all given switches/arguments //
		for(int i = 1; i < args.size(); i++)
		{
			if(args.at(i) == "-T")
			{
				// string variable to hold what type of file system
				string fileSystem;
				
				statfs("/", &fs);
				
				// switch statement to get file system type
				switch(fs.f_type)
				{
					case 2035054128:
						fileSystem = "OverlayFS";
						break;
					case 1481003842:
						fileSystem = "XFS";
						break;
					case 1397118030:
						fileSystem = "NTFS";
						break;
					case 16964:
						fileSystem = "NFS";
						break;
					case 61267:
						fileSystem = "EXT2/EXT3/EXT4";
						break;
					default:
						fileSystem = "Other";
						break;
				}
				
				// print the type of the file system
				cout << "The file system type is: " << fileSystem << endl;
			}
			else if(args.at(i) == "-b")
			{	
				statfs("/", &blocks);
				
				// print the total number of blocks in the file system
				cout << "There are " << blocks.f_bavail << " blocks available out of a total of " << blocks.f_blocks << endl; 
			}
			else if(args.at(i) == "-s")
			{
				statfs("/", &nameLen);
				
				// print the maximum number of characters that can be used in a filename
				cout << "The maximum number of characters that can be used in a filename is: " << nameLen.f_namelen << " characters" << endl;
			}
			else if(args.at(i) == "--help")
			{
				// print the help command for the osinfo command
				cout << "osinfo -switch -- will print on the screen different system related information based on a required switch" << endl;
				cout << "\tosifo switches:" << endl;
				cout << "\t\t-T -- will print the type of the file system (ex. DOS, FAT, FAT32, EXT2, EXT3, EXT4, NTFS, etc.)" << endl;
				cout << "\t\t-b -- will print the total number of blocks in the files system" << endl;
				cout << "\t\t-s -- will print the maximum number of characters that can be used in a filename" << endl;
			}
			else
			{
				// argument not found
				cout << "Error: Switch(s) not recognized. Type osinfo --help for more details" << endl;
				return 0;
			}
		}
		return 1;
	}
	return 0;
}

// method to print the help file to the screen
int displayManual()
{	
	// print cwushell manual
	cout << "-----cwushell help manual-----" << endl << endl;
	
	// exit command man
	cout << "exit [n] -- terminates the shell causing a return from the shell's main(). If an argument (n) is given, it will be the shell's exit value" << endl <<endl;
	
	// prompt command man
	cout << "prompt [new_prompt] -- will change the cuurent shell prompt to the new_prmpt. Typing prompt restores the shell's default prompt" << endl << endl;
	
	// fileinfo command man
	cout << "fileinfo -[switch] filename -- will print on the screen different file related information based on the switch." << endl;
	cout << "\tfileinfo switches:" << endl;
	cout << "\t\t-i -- will print the inode numberof the file" << endl;
	cout << "\t\t-t -- will print the type of the file pointed by filename (ex. regular file, directory, symlink)" << endl;
	cout << "\t\t-m -- will print the last modification date of th efile pointed by filename" << endl;
	cout << "\tIf no switch is specified, all three switches will be output." << endl << endl;
	
	// osinfo command man
	cout << "osinfo -switch -- will print on the screen different system related information based on a required switch" << endl;
	cout << "\tosifo switches:" << endl;
	cout << "\t\t-T -- will print the type of the file system (ex. DOS, FAT, FAT32, EXT2, EXT3, EXT4, NTFS, etc.)" << endl;
	cout << "\t\t-b -- will print the total number of blocks in the files system" << endl;
	cout << "\t\t-s -- will print the maximum number of characters that can be used in a filename" << endl;
	
	return 1;
}

// main driver method
int main()
{
	string command;
	string arguments;
	int returnVal = 0;
	vector<string> commandsParsed;
	string defaultPrompt = "cwushell";
	string currentPrompt = defaultPrompt;
	
	while (1)
	{
		// print the shell line
		cout << currentPrompt << ">> ";
		
		// get the input line from the user
		getline(cin, command);
	
		// asign the parsed commands to a vector in the main
		commandsParsed = parseCommands(command);
		
		// if statements to determine if the input command is a local command
		if(commandsParsed.at(0) == "exit")
		{
			int exitReturn = exitCommand(commandsParsed, returnVal);
			// call exit method
			if(exitReturn != -1)
			{
				return exitReturn;
			}
			else
			{
				returnVal = 0;
			}
		}
		else if (commandsParsed.at(0) == "prompt")
		{	
			// call prompt method
			currentPrompt = promptCommand(commandsParsed, currentPrompt);
		}
		else if (commandsParsed.at(0) == "fileinfo")
		{
			// call fileinfo method
			returnVal = fileInfoCommand(commandsParsed);
		}
		else if (commandsParsed.at(0) == "osinfo")
		{
			// call osinfo method
			returnVal = osinfoCommand(commandsParsed);
		}
		else if (commandsParsed.at(0) == "manual")
		{
			// call display manual method
			returnVal = displayManual();
		}
		// if the command is not a local command, return 0 for a failed execution
		else
		{
			cout << commandsParsed.at(0) << endl;
			// check if the command is a system command
			returnVal = sysCommands(command);
		}

	}
	
	// something horrible has somehow happened, return the returnval
	return returnVal;
}
