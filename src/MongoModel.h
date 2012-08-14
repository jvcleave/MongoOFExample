/*
 *  MongoModel.h
 *  mongoTest
 *
 *  Created by jason van cleave on 8/14/12.
 *  Copyright 2012 jasonvancleave.com. All rights reserved.
 *
 */
#pragma once

#include "ofMain.h"
#undef check

#include "mongo/client/dbclient.h"

class MongoModel
{
public:
	MongoModel();
	void setup();
	void connectToDB();
	void shutdownMongo();
	void checkMongoDirectories();
	void exit();
	void doTest();
	mongo::DBClientConnection* connection;
	string databaseName;
	
	string realAbsPath;
	string mongodBinPath;
	string mongodLogFilePath;
	
	string mongoConfPath;
	string mongoLockFilePath;
	string mongoDBpath;
	
	ofDirectory mongoDataRootDirectory;
	ofDirectory mongoDBDirectory;
	ofDirectory mongoLogDirectory;
	string mongoBindIP;
	static string logName;
};
