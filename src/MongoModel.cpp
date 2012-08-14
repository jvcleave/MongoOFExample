/*
 *  MongoModel.cpp
 *  mongoTest
 *
 *  Created by jason van cleave on 8/14/12.
 *  Copyright 2012 jasonvancleave.com. All rights reserved.
 *
 */

#include "MongoModel.h"

string MongoModel::logName = "MongoModel";
using namespace mongo;

void printIfAge(DBClientConnection& c, string dbName, int age) 
{
	auto_ptr<DBClientCursor> cursor = c.query(dbName, QUERY( "age" << LT << age ).sort("age") );
	int resultCounter = 0;
	while( cursor->more() ) 
	{
		resultCounter++;
		BSONObj p = cursor->next();
		ofLogVerbose(MongoModel::logName) << "resultCounter: " << resultCounter << " name: " << p["name"] << " age: " <<  p["age"];
	}
}

MongoModel::MongoModel()
{
	mongodBinPath = "/usr/local/bin/mongod";
	databaseName = "tutorial.persons";
	mongoBindIP = "127.0.0.1";
	connection = NULL;
}

void MongoModel::setup()
{
	//lets get a real absolute path to the data directory
	string currentPath = ofToDataPath("", true); //current path to bin/data, will have ../../blah - can't use it
	
	vector<string> parts = ofSplitString(currentPath, "bin");
	realAbsPath = parts[0]+"bin/data/";
	
	mongoDataRootDirectory = ofDirectory(realAbsPath+"mongo");
	
	checkMongoDirectories();
	
	shutdownMongo();
	bool autoReconnect = true;
	connection = new DBClientConnection(autoReconnect, NULL, NULL);
	connectToDB();
	
}
void MongoModel::checkMongoDirectories()
{
	
	if (!mongoDataRootDirectory.exists())
	{
		ofLogVerbose(logName) << "trying to create " << mongoDataRootDirectory.path();
		if(mongoDataRootDirectory.create(true))
		{
			ofLogVerbose(logName) << "created " << mongoDataRootDirectory.path();
		}
	}
	
	mongoDBpath = realAbsPath+"mongo/db";
	mongoDBDirectory = ofDirectory(mongoDBpath); 
	if (!mongoDBDirectory.exists()) 
	{
		ofLogVerbose(logName) << "trying to create " << mongoDBDirectory.path();
		if(mongoDBDirectory.create(true))
		{
			ofLogVerbose(logName) << "created " << mongoDBDirectory.path();
		}
	}
	
	mongoLogDirectory = ofDirectory(realAbsPath+"mongo/log");
	if (!mongoLogDirectory.exists()) 
	{
		ofLogVerbose(logName) << "trying to create " << mongoLogDirectory.path();
		if(mongoLogDirectory.create(true))
		{
			ofLogVerbose(logName) << "created " << mongoLogDirectory.path();
		}
	}
	
	mongodLogFilePath = realAbsPath+"mongo/log/mongod.log";
	ofFile mongodLogFile (mongodLogFilePath);
	if (!mongodLogFile.exists()) 
	{
		mongodLogFile.create();
	}
	
	
	mongoConfPath = realAbsPath+"mongo/mongo.conf";
	ofFile confFile(mongoConfPath);
	if (confFile.exists())
	{
		confFile.copyTo(mongoConfPath+".bak", true);
		confFile.remove();//delete existing file
	}
	
	//write conf file
	stringstream confContents;
	
	confContents << "dbpath = "  << mongoDBpath       << endl;
	confContents << "logpath = " << mongodLogFilePath << endl;
	confContents << "bind_ip = " << mongoBindIP       << endl;
	
	ofBuffer confOutput(confContents);
	
	ofBufferToFile(mongoConfPath, confOutput, false);
	
}
void MongoModel::connectToDB()
{

	string startCommand = mongodBinPath + " run --config=" + mongoConfPath + " > /dev/null 2>&1 &";
	ofLogVerbose(logName) << "sending command" << startCommand.c_str();
	int status = -1;
	status = system(startCommand.c_str());
	if(status != 0)//strange but true
	{
		ofLogError() << "mongodb startup failed" << endl;
	}
	
	try 
	{
		connection->connect("localhost");
		ofLogVerbose(logName) << "CONNECTED TO MONGO";
		
	} catch( mongo::DBException &e ) 
	{
		ofLogVerbose(logName) << "COULD NOT CONNECT " << e.what() << endl;
		connectToDB();
	}
	
}
void MongoModel::shutdownMongo()
{
	
	string shutdownMongoCommand = "killall mongod &";
	ofLogVerbose(logName) << "exiting with shutdownMongoCommand: " << shutdownMongoCommand;
	
	int status = system(shutdownMongoCommand.c_str());
	if (status != 0) 
	{
		ofLogVerbose(logName) << "SHUTDOWN FAILED WITH STATUS " << status;
	}
	ofFile mongoLockFile(mongoLockFilePath);
	if (mongoLockFile.exists()) 
	{
		ofLogVerbose(logName) << "removing mongoLockFile: " << mongoLockFile.path();
		mongoLockFile.remove();
	}
}
void MongoModel::exit()
{
	shutdownMongo();
}

void MongoModel::doTest()
{
	BSONObj person = BSON( "name" << "Joe" << "age" << (int) ofRandom(66) );
	
	connection->insert(databaseName, person);
	connection->ensureIndex(databaseName, BSON( "age" << 1 ));
	
	ofLogVerbose(logName) << "count:" << connection->count(databaseName);
	
	auto_ptr<DBClientCursor> cursor = connection->query(databaseName, BSONObj());
	while( cursor->more() )
	{
		ofLogVerbose(logName) << cursor->next().toString();
	}
	
}
