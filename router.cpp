//Joe Lebedew
//CSC390
//asg4

#include<iostream>

#include<fstream>

//following inlcudes are for file system api
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<netinet/in.h>

using namespace std;

#define A 0
#define B 1
#define C 2
#define D 3
#define E 4
#define F 5

struct DataPacket
{
    //isData implies that this DataPacket contains data to be written to file
    bool isData;
    //isControl implies that this DataPacket contains an update for its routing table
    bool isControl;
};

sockaddr_in address;
int addressSize;

int routingTable [6][4];
//contains the router numbers of all neighbors
int *neighborTable;
//tracks the number of neighbors found in the neighborTable
int numNeighbors = 0;

//compares costs from a neighbor's routing table to the current routing table
//updates the current routing table if necessary
void updateRoutingTable(int **neighborsRoutingTable) {

}
/*
first:  sends the current routing table
second: receives neighbors' current routing tables 
third:  then updates the current routing table to the next iteration
*/
void nextIteration(int s) {
    sockaddr_in neighborAddr;
    //send this router's routing table to all of its neighbors
    for(int i = 0; i < numNeighbors; i++) {
        neighborAddr.sin_port = htons(neighborTable[i]);
        sendto(s, 
            (void*)&routingTable, 
            sizeof(routingTable), 
            0, (struct sockaddr*) &neighborAddr, (socklen_t) sizeof(neighborAddr));
    }
    //recv routing table updates from all neighbors
    int **neighborsRoutingTable;
    for(int i = 0; i < numNeighbors; i++) {
        int bytesReceived = recvfrom(s, 
            &neighborsRoutingTable, 
            sizeof(routingTable), 
            0, (struct sockaddr*) &address, (socklen_t*) &addressSize);
        updateRoutingTable(neighborsRoutingTable);
    }
}

void printRoutingTable()
{
    cout<<"Destiniation\tCost\t\tOutgoing Port\tDestination Port"<<endl;
    for (int i=0; i<6; i++)
    {
        for (int j=0; j<4; j++)
        {
            cout<<routingTable[i][j]<<"\t\t";
        }
        
        cout<<"\n";
    }
    cout << numNeighbors << endl;
    for(int i = 0; i < numNeighbors; i++)
        cout << neighborTable[i] << endl;
}

void initializeBlankRoutingTable(int thisRouter, int thisRoutersPort)
{
    for (int i=0; i<6; i++)
    {
        routingTable[i][0] = i; //set the name of the node in the list
        if (i == thisRouter)
        {
            routingTable[i][1] = 0; //if creating the row for the current router, make it's cost 0
            routingTable[i][2] = routingTable[i][3] = thisRoutersPort; //set the outoging and incoming port to the current router's port
        }
        else
        {
            routingTable[i][1] = 99999; //if not creating the row for the current router, make it's cost "infinity"
            routingTable[i][2] = routingTable[i][3] = 0; //set the outoging and incoming ports to 0
        }
    }
}

void readInitialRoutingTable(string thisRouter, int thisRoutersPort)
{
    string currentPartOfFile;
    ifstream initalRoutingTable ("irt.txt");
    //sets the size of the neighbor table to the number of lines found in the file
    //there is as much space as there are edges in the graph
    neighborTable = new int
    [count(istreambuf_iterator<char>(initalRoutingTable), 
        istreambuf_iterator<char>(), '\n')];
    initalRoutingTable.seekg(0);

    if (initalRoutingTable.is_open())
    {
        while (!initalRoutingTable.eof())
        {
            initalRoutingTable >> currentPartOfFile;
            if (currentPartOfFile == thisRouter)
            {
                initalRoutingTable >> currentPartOfFile; //get the destination router
                if (currentPartOfFile == "A")
                {
                    initalRoutingTable >> currentPartOfFile; //get the destination port
                    routingTable[A][3] = stoi(currentPartOfFile); //record the destination port in the routing table
                    neighborTable[numNeighbors++] = routingTable[A][3]; //record this as a neighbor
                    initalRoutingTable >> currentPartOfFile; //get the cost
                    routingTable[A][2] = thisRoutersPort;
                    routingTable[A][1] = stoi(currentPartOfFile); //record the cost
                }
                else if (currentPartOfFile == "B")
                {
                    initalRoutingTable >> currentPartOfFile; //get the destination port
                    routingTable[B][3] = stoi(currentPartOfFile); //record the destination port in the routing table
                    neighborTable[numNeighbors++] = routingTable[B][3]; //record this as a neighbor
                    initalRoutingTable >> currentPartOfFile; //get the cost
                    routingTable[B][2] = thisRoutersPort;
                    routingTable[B][1] = stoi(currentPartOfFile); //record the cost
                }
                else if (currentPartOfFile == "C")
                {
                    initalRoutingTable >> currentPartOfFile; //get the destination port
                    routingTable[C][3] = stoi(currentPartOfFile); //record the destination port in the routing table
                    neighborTable[numNeighbors++] = routingTable[C][3]; //record this as a neighbor
                    initalRoutingTable >> currentPartOfFile; //get the cost
                    routingTable[C][2] = thisRoutersPort;
                    routingTable[C][1] = stoi(currentPartOfFile); //record the cost
                }
                else if (currentPartOfFile == "D")
                {
                    initalRoutingTable >> currentPartOfFile; //get the destination port
                    routingTable[D][3] = stoi(currentPartOfFile); //record the destination port in the routing table
                    neighborTable[numNeighbors++] = routingTable[D][3]; //record this as a neighbor
                    initalRoutingTable >> currentPartOfFile; //get the cost
                    routingTable[D][2] = thisRoutersPort;
                    routingTable[D][1] = stoi(currentPartOfFile); //record the cost
                }
                else if (currentPartOfFile == "E")
                {
                    initalRoutingTable >> currentPartOfFile; //get the destination port
                    routingTable[E][3] = stoi(currentPartOfFile); //record the destination port in the routing table
                    neighborTable[numNeighbors++] = routingTable[E][3]; //record this as a neighbor
                    initalRoutingTable >> currentPartOfFile; //get the cost
                    routingTable[E][2] = thisRoutersPort;
                    routingTable[E][1] = stoi(currentPartOfFile); //record the cost
                }
                else if (currentPartOfFile == "F")
                {
                    initalRoutingTable >> currentPartOfFile; //get the destination port
                    routingTable[F][3] = stoi(currentPartOfFile); //record the destination port in the routing table
                    neighborTable[numNeighbors++] = routingTable[F][3]; //record this as a neighbor
                    initalRoutingTable >> currentPartOfFile; //get the cost
                    routingTable[F][2] = thisRoutersPort;
                    routingTable[F][1] = stoi(currentPartOfFile); //record the cost
                }
            }
            else
            {
                initalRoutingTable >> currentPartOfFile;
                initalRoutingTable >> currentPartOfFile;
                initalRoutingTable >> currentPartOfFile;
            }
        }
    }
}

int main(int argc, char* argv[])
{
    //following code prepares setting up for a socket connection
    int s = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    
    if (s <= 0) {
        cerr << "server: Socket creation failed." << endl;
        exit(-1);
    }

    //sockaddr_in address;
    //int addressSize = sizeof(address);
    addressSize = sizeof(address);
    bzero(&address,addressSize);
    address.sin_family=AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    
    if (argc != 2)
    {
        cerr<<"ERROR! -- Incorrect parameters passed. Please pass in:\n\t 1. 'A'-'F' to start a normal router.\n\t 2. 'H' to send a data packet from 'A' to 'D'.\n\t 3. 'X' to cleanly turn off all processes."<<endl;
        exit(-1);
    }
    
    if (*argv[1] == 'A')
    {
        address.sin_port  = htons(10000); //create port
        initializeBlankRoutingTable(A, 10000); //create an initial routing table for A
        readInitialRoutingTable("A", 10000); //read in the inital routing table from file and fill in direct neighbors of router A
    }
    else if (*argv[1] == 'B')
    {
        address.sin_port  = htons(10001); //create port
        initializeBlankRoutingTable(B, 10001); //create an initial routing table for B
        readInitialRoutingTable("B", 10001); //read in the inital routing table from file and fill in direct neighbors of router B
    }
    else if (*argv[1] == 'C')
    {
        address.sin_port  = htons(10002); //create port
        initializeBlankRoutingTable(C, 10002); //create an initial routing table for C
        readInitialRoutingTable("C", 10002); //read in the inital routing table from file and fill in direct neighbors of router C
    }
    else if (*argv[1] == 'D')
    {
        address.sin_port  = htons(10003); //create port
        initializeBlankRoutingTable(D, 10003); //create an initial routing table for D
        readInitialRoutingTable("D", 10003); //read in the inital routing table from file and fill in direct neighbors of router D
    }
    else if (*argv[1] == 'E')
    {
        address.sin_port  = htons(10004); //create port
        initializeBlankRoutingTable(E, 10004); //create an initial routing table for E
        readInitialRoutingTable("E", 10004); //read in the inital routing table from file and fill in direct neighbors of router E
    }
    else if (*argv[1] == 'F')
    {
        address.sin_port  = htons(10005); //create port
        initializeBlankRoutingTable(F, 10005); //create an initial routing table for F
        readInitialRoutingTable("F", 10005); //read in the inital routing table from file and fill in direct neighbors of router F
    }
    else if (*argv[1] == 'H')
    {
        //send data packet from A to D
        exit(0); //will not want to process any packet information if not a real "router" process
    }
    else if (*argv[1] == 'X')
    {
        //cleanly shutdown all processes
        exit(0); //will not want to process any packet information if not a real "router" process
    }
    else
    {
        cerr<<"ERROR! -- Incorrect parameters passed. Please pass in:\n\t 1. 'A'-'F' to start a normal router.\n\t 2. 'H' to send a data packet from 'A' to 'D'.\n\t 3. 'X' to cleanly turn off all processes."<<endl;
        exit(-1);
    }
    
    printRoutingTable();
    
    int status=0;
    //status = bind(s,(struct sockaddr*) &address, sizeof(address));
    bind(s,(struct sockaddr*) &address, sizeof(address));
    
    if (status != 0) {
        cerr << "server: Bind failed, unable to create port." << endl;
        exit(-1);
    }
    
    close(s);
    delete [] neighborTable;
    return 0;
}
