#include <Python.h>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <cmath>
#include <string>
#include <stdexcept>

using namespace std;

/*
Description:
	To call this function, simply pass the function name in Python that you wish to call.
Example:
	callProcedure("printsomething");
Output:
	Python will print on the screen: Hello from python!
Return:
	None
*/
void CallProcedure(string pName)
{
	char *procname = new char[pName.length() + 1];
	std::strcpy(procname, pName.c_str());

	Py_Initialize();
	PyObject* my_module = PyImport_ImportModule("PythonCode");
	PyErr_Print();	
	PyObject* my_function = PyObject_GetAttrString(my_module, procname);
	PyObject* my_result = PyObject_CallObject(my_function, NULL);
	Py_Finalize();

	delete[] procname;
}

/*
Description:
	To call this function, pass the name of the Python functino you wish to call and the string parameter you want to send
Example:
	int x = callIntFunc("PrintMe","Test");
Output:
	Python will print on the screen: 
		You sent me: Test
Return:
	100 is returned to the C++	
*/
int callIntFunc(string proc, string param)
{
	char *procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	char *paramval = new char[param.length() + 1];
	std::strcpy(paramval, param.c_str());


	PyObject *pName, *pModule, *pDict, *pFunc, *pValue = nullptr, *presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"PythonCode");
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(z)", paramval);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean 
	delete[] procname;
	delete[] paramval;


	return _PyLong_AsInt(presult);
}

/*
Description:
	To call this function, pass the name of the Python functino you wish to call and the string parameter you want to send
Example:
	int x = callIntFunc("doublevalue",5);
Return:
	25 is returned to the C++
*/
int callIntFunc(string proc, int param)
{
	char *procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	PyObject *pName, *pModule, *pDict, *pFunc, *pValue = nullptr, *presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"PythonCode");
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(i)", param);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean 
	delete[] procname;

	return _PyLong_AsInt(presult);
}


void main()
{
	int choice = -1; // Variable used by the main menu.
	const string readFile = "frequency.dat"; // Constant string variable that contains the filename of the data file used for the histogram. 
	ifstream inFS; // Input file stream
	string temp = ""; // Temporary string variable used to pass user input to python functions. 
	int frequency; // Frequency variable used in the histogram.

	while (choice != 4) { // Main loop
		try {
			// Main menu display.
			cout << "MAIN MENU" << endl;
			cout << "1. Display a Frequency Table" << endl;
			cout << "2. Get the Frequency of a Single Item" << endl;
			cout << "3. Display a Histogram" << endl;
			cout << "4. Exit" << endl;

			// User input for main menu.
			cout << "Enter a number: ";
			cin >> choice;
			if (choice < 1 || choice > 4) { // Conditional used to validate user input.
				throw runtime_error("Invalid input...");
			}
			cout << endl;

			if (choice == 1) { // Displays a frequency table.
				CallProcedure("DisplayFrequencyTable"); // Calls the python function DisplayFrequencyTable()
			}

			if (choice == 2) { // Displays the frequency of a single item. 
				cout << "FREQUENCY OF SINGLE ITEM" << endl;
				cout << "Enter produce name: ";
				cin >> temp; // User input of produce name
				cout << endl;
				
				// Used to throw away extraneous information.
				cin.clear();
				cin.ignore(100, '\n');

				/* Sets the variable frequency to the value returned by the FrequencyOfItem() python function.
				Note: If the user input a string not found in the produce record they will recieve a frequency of 0. */
				frequency = callIntFunc("FrequencyOfItem", temp); 

				cout << "The frequency of " << temp << " is " << frequency << endl; // Displays the frequency of the produce.
				
				// This conditional serves as a warning to the user that if they recieved a frequency of 0 their input may have been incorrect. 
				if (frequency == 0) { 
					cout << "If this value seems incorrect double check your spelling and try again!" << endl;
				}
			}

			if (choice == 3) { // Displays a histogram.
				CallProcedure("WriteDataFile"); // Calls the python function WriteDataFile().
				
				inFS.open(readFile); // Opens the data file produced by the python function.
				if (!inFS.is_open()) { // Checks to see if file open correctly. Throws a runtime error if it did not.
					throw runtime_error(readFile + " failed to open...");
				}

				cout << "FREQUENCY HISTOGRAM" << endl;
				while (!inFS.eof()) { // Loop that iterates through the file generated by WriteDataFile() until it reaches the end of file.
					inFS >> temp; // Reads the produce and stores it in temp.
					if (!inFS.fail()) { 
						cout << temp << " ";

						inFS >> temp; // Reads the frequency and stores it in temp
						// Loop prints the frequency using * as the representation.
						for (int i = 0; i < stoi(temp); ++i) { 
							cout << '*';
						}
						
						cout << endl;
					}
				}

				inFS.close(); // Closes read file.
			}

			cout << endl;
		}
		catch (runtime_error& excpt) { // Catches and reports invalid inputs and read errors.
			cout << endl;
			cout << excpt.what() << endl;
			cout << endl;

			cin.clear();
			cin.ignore(100, '\n');
		}
	}
}