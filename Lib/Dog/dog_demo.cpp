/////////////////////////////////////////////////////////////////////////////
//
// Demo program for SuperDog licensing API
//
//
// Copyright (C) 2012 SafeNet, Inc. All rights reserved.
//
//
// SuperDog DEMOMA key with features 42 required
//
//////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <algorithm>
#include <sstream>
#include <stdio.h>

#include <string.h>

#include "dog_api_cpp.h"
#include "vendor_code.h"
#include "errorprinter.h"

using namespace std;

//Data for the demo of the encryption/decryption functionality
unsigned char data[] = 
{
    0x74, 0x65, 0x73, 0x74, 0x20, 0x73, 0x74, 0x72,
    0x69, 0x6e, 0x67, 0x20, 0x31, 0x32, 0x33, 0x34
};
const unsigned int dataLen = sizeof(data);

//The size of the part of the data file which 
//will be printed during the data file demo
const dog_size_t numBytesToShow = 64;

void displayCloseMessage()
{
	cout<<endl<<"press ENTER to close the sample"<<endl;

    fflush(stdin);
    while (getchar() == EOF);
}

//Displays the content of mdata in a nice format with an ascii
//and a hexadecimal area.
void displayFileData(unsigned char *mdata, unsigned int mdataLen)
{
    //The number of bytes to be shown in each line
    const unsigned int lineLen = 16; 

    //This is printed before the first char in each line
    const char* margin = "     ";

    unsigned int i/*lines*/, ii/*chars*/;

    for(i=0; i<mdataLen; i+=lineLen)
    {
        ostringstream asciiWriter;
        ostringstream hexWriter;

        for(ii=i; ii<lineLen+i; ii++)
        {
            if(ii<mdataLen)
            {
                //Print the active char to the hex view
                unsigned int val = static_cast<unsigned int>(mdata[ii]);
                if(val<16)
                    hexWriter<<'0';
                hexWriter<<uppercase<<hex<<val<<' ';
                
                //... and to the ascii view
                if(mdata[ii]<32 || mdata[ii]>127) //Do not print some characters
                    asciiWriter<<'.';             //because this may create
                else                              //problems on UNIX terminals
                    asciiWriter<<mdata[ii];
            }
            else
            {
                hexWriter<<"   ";
                asciiWriter<<" ";
            }

            //Display a separator after each 4th byte
            if(!((ii-i+1)%4) && (ii-i+1<lineLen))
                hexWriter<<"| ";
        }

        //Write the line
        cout<<margin<<hexWriter.str()<<" ["<<asciiWriter.str()<<"]"<<endl;
    }
}

//Displays a CDogTime in a nice format
void displayTime(const CDogTime& dogTime)
{
    cout<<"     SuperDog time: "<<dogTime.hour();
    cout<<':'<<dogTime.minute()<<':'<<dogTime.second()<<endl;
    cout<<"     SuperDog date: "<<dogTime.day();
    cout<<'/'<<dogTime.month()<<'/'<<dogTime.year()<<" D/M/Y"<<endl;
}

int main()
{
    cout<<endl;
    cout<<"A simple demo program for the SuperDog licensing functions."<<endl;
    cout<<"Copyright (C) SafeNet, Inc. All rights reserved."<<endl<<endl;

    //Prints the error messages for the return values of the functions
    ErrorPrinter errorPrinter;

    //Used to hold the return value of the called functions
    dogStatus status;


    //Demonstrates the login to the default feature of a key
    //Searches both locally and remotely for it
    cout<<"login to default feature         : ";

    CDog dog1(CDogFeature::defaultFeature());
    status = dog1.login(vendorCode);
    errorPrinter.printError(status);

    if (!DOG_SUCCEEDED(status))
    {
        displayCloseMessage();
        return 1;
    }

    //Demonstrates how getSessionInfo can be used to create a scope
    //which can be later used for a login

    //This info template is used to create a scope which limits
    //the login to the current key
    std::string infoTemplate ="<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"
                              "<dogformat root=\"dogscope\">"
                              "<dog>"
                              "<attribute name=\"id\" />"
                              "</dog>"
                              "</dogformat>";

    //The output of the getInfo call
    std::string resultingScope;

    cout<<"get a login scope                : ";
    status = dog1.getSessionInfo(infoTemplate, resultingScope);
    errorPrinter.printError(status);

    if (!DOG_SUCCEEDED(status))
    {
        displayCloseMessage();
        return 1;
    }

    if (DOG_SUCCEEDED(status))
        dog1.logout();

    cout<<endl;

    //Demonstrates the login to a specific feature of a key
    //Searches only on the local pc for it
    cout<<"login to feature 42              : ";

    CDogFeature feature42 = CDogFeature::fromFeature(42);

    const char* defaultScope = "<dogscope />";

    CDog dog2(feature42);
    status = dog2.login(vendorCode, defaultScope);
    errorPrinter.printError(status);

    if (DOG_SUCCEEDED(status))
        dog2.logout();    

    //Demonstrates the login to a specific feature of a key
    //Searches using the previously got scope
    cout<<"login to feature number 42       : ";

    CDogFeature feature3 = CDogFeature::fromFeature(42);

    CDog dog42(feature3);
    status = dog42.login(vendorCode, defaultScope);
    errorPrinter.printError(status);

    if (!DOG_SUCCEEDED(status))
    {
    	displayCloseMessage();
        return 1;
    }

    cout<<endl;

    //Retrieves the attributes and additional information
    //of the connected SuperDog
    cout<<"get session info                 : ";

    std::string info;
    status = dog42.getSessionInfo(CDog::keyInfo(), info);
    errorPrinter.printError(status);
    cout<<endl;

    if (DOG_SUCCEEDED(status))
    {
        cout<<"==============="<<endl;
        cout<<info;
        cout<<"==============="<<endl;
    }

    cout<<endl;

    //Retrieves the size of the default data file
    cout<<"retrieving the size of default data file : ";
    
    dog_size_t size = 0;
    CDogFile defaultFile = dog42.getFile(CDogFile::fileReadWrite);

    status = defaultFile.getFileSize(size);
    
    if (DOG_SUCCEEDED(status))
        cout<<"The size is "<<size<<" bytes"<<endl;
    else
        errorPrinter.printError(status);

    //If SuperDog holds default data file
    if(size)
    {
        //Reads a part of the data file into a buffer
        unsigned int fsize = (size < numBytesToShow) ? size : numBytesToShow;
        cout<<"reading "<<fsize<<" bytes from data file  : ";

        unsigned char *filedata = new unsigned char[fsize];
        
        status = defaultFile.read(filedata, fsize);
        errorPrinter.printError(status);

        if (DOG_SUCCEEDED(status))
            displayFileData(filedata, fsize);
        else
        {
            delete[] filedata;
            displayCloseMessage();
            return 1;
        }
        cout<<endl;

        cout<<"incrementing every byte in the buffer"<<endl<<endl;
        
        for (unsigned int i=0; i<fsize; i++) 
            (filedata[i])++;

        //Writes the changed buffer back to SuperDog

        cout<<"writing "<<fsize<<" bytes to data file    : ";
        status = defaultFile.write(filedata, fsize);
        errorPrinter.printError(status);

        if (!DOG_SUCCEEDED(status))
        {
            delete[] filedata;
            displayCloseMessage();
            return 1;
        }

        cout<<endl;
        memset(filedata, 0, fsize);

        //Reads the data file again in order to check the write function
        cout<<"reading "<<fsize<<" bytes from data file  : ";
        status = defaultFile.read(filedata, fsize);
        errorPrinter.printError(status);

        if (DOG_SUCCEEDED(status))
            displayFileData(filedata, fsize);
        else
        {
            delete[] filedata;
            displayCloseMessage();
            return 1;
        }
        cout<<endl;
        
        delete[] filedata;
    }

    //Encryption demo
    cout<<"buffer before encryption         : "<<endl;
    displayFileData(data, dataLen);

    cout<<"encrypting the buffer            : ";
    status = dog42.encrypt(data, dataLen);
    errorPrinter.printError(status);
    if (DOG_SUCCEEDED(status))
        displayFileData(data, dataLen);

    cout<<"decrypting the buffer            : ";
    status = dog42.decrypt(data, dataLen);
    errorPrinter.printError(status);
    if (DOG_SUCCEEDED(status))
        displayFileData(data, dataLen);
    
    cout<<endl;

    //Time access demo
    cout<<"reading current time and date    : ";
    CDogTime dogTime;
    status = dog42.getTime(dogTime);
    errorPrinter.printError(status);
    if (DOG_SUCCEEDED(status) || DOG_TIME_ERR == status)
        displayTime(dogTime);

    //Logout
    cout<<"logout from feature number 42    : ";
    status = dog42.logout();
    errorPrinter.printError(status);
    cout<<endl;

	displayCloseMessage();
    
    return 0;
}
