PRODUCE_RECORD = "ProduceRecord.txt" # Variable that stores the name of the text file.

# Helper function that creates and returns a list of produce stored in the produce record.
def CreateProduceList():
    # Opens the produce record, stores the values, and closes the file.
    f = open(PRODUCE_RECORD)
    produceList = f.readlines()
    f.close()

    # For loop that adjusts the formatting of the produce in the produce list.
    for num in range(len(produceList)):
        produceList[num] = produceList[num].split('\n')[0]

    return produceList

# Helper function that creates and returns a dictionary of produce with their respective frequencies.
# Default parameter is given by the helper function CreateProduceList().
def CreateFrequencyTable(produceList=CreateProduceList()):
    frequencyTable = {}

    # For loop that iterates through the produce list and appends values to the frequency table.
    for produce in produceList:
        # If a produce can be found in the frequency, table increment the frequency.
        # Else append the produce to the dictionary.
        if produce in frequencyTable:
            frequencyTable[produce] += 1
        else:
            frequencyTable[produce] = 1

    return frequencyTable

# Function used to display a frequency table.
# Uses the helper function CreateFrequencyTable() to get a dictionary of values.
def DisplayFrequencyTable():
    frequencyTable = CreateFrequencyTable()

    print("FREQUENCY TABLE")
    # Loop that iterates through the dictionary and print the key/value pairs which proper formatting.
    for produce, num in frequencyTable.items():
        print("{}: {}".format(produce, num))

# Function that accepts a word and checks if it appears in the produce list.
# Returns the frequency of the given produce.
def FrequencyOfItem(word):
    counter = 0
    produceList = CreateProduceList()

    # Loop that iterate through the produce list and checks for the user's produce.
    for produce in produceList:
        if produce.lower() == word.lower(): # Ignores punctuation
            counter += 1

    return counter

# Function that creates a writable data file called frequency.dat.
# Writes the values of the frequency table to the newly created data file for use in C++.
def WriteDataFile():
    frequencyTable = CreateFrequencyTable()
    f = open("frequency.dat", 'w')

    # Loop that iterates through the items in the frequency table and writes them with proper formatting.
    for produce, num in frequencyTable.items():
        f.write("{} {}\n".format(produce, num))

    f.close()