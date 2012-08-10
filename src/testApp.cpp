#include "testApp.h"

using namespace mongo;

void printIfAge(DBClientConnection& c, string dbName, int age) 
{
	auto_ptr<DBClientCursor> cursor = c.query(dbName, QUERY( "age" << LT << age ).sort("age") );
	int resultCounter = 0;
	while( cursor->more() ) 
	{
		resultCounter++;
		BSONObj p = cursor->next();
		ofLogVerbose() << "resultCounter: " << resultCounter << " name: " << p["name"] << " age: " <<  p["age"];
	}
}
//--------------------------------------------------------------
void testApp::setup()
{
	
	
	
	//lets get a real absolute path to the data directory
	
	string currentPath = ofToDataPath("", true);
	vector<string> parts = ofSplitString(currentPath, "bin");
	string binDataDir = parts[0]+"bin/data/";
	string confPath = binDataDir+"mongo/mongo.conf";
	cout << confPath << endl;
	ofFile confFile(confPath, ofFile::ReadWrite, false);
	confFile.remove(false);
	confFile.create();
	ofBuffer input;
	
	input << "hi";
	return;
	//confFile.save();
	/*
	 confFile
	return;
	string startCommand = "	/usr/local/bin/mongod run --config=/Volumes/WORK_IN_PROGRESS/OPENFRAMEWORKS/0071_RELEASE/apps/myApps/mongoTest/bin/data/mongo/mongo.conf > /dev/null 2>&1 &";
	cout << startCommand.c_str() << endl;
	return;
	int procID = system(startCommand.c_str());
	cout << "procID: " << procID << endl;
	dynamicDBName = ofGetTimestampString()+".persons";
	ofSetLogLevel(OF_LOG_VERBOSE);
	try 
	{
		connection.connect("localhost");
		ofLogVerbose() << "CONNECTED TO MONGO";
		
	} catch( mongo::DBException &e ) 
	{
		ofLogVerbose() << "COULD NOT CONNECT " << e.what() << endl;
	}
	
	
		
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key == ' ')
	{
		BSONObj person = BSON( "name" << "Joe" << "age" << (int) ofRandom(66) );
		
		connection.insert(dynamicDBName, person);
		connection.ensureIndex(dynamicDBName, BSON( "age" << 1 ));
		
		ofLogVerbose() << "count:" << connection.count(dynamicDBName);
		
		auto_ptr<DBClientCursor> cursor = connection.query(dynamicDBName, BSONObj());
		while( cursor->more() )
		{
			ofLogVerbose() << cursor->next().toString();
		}
		printIfAge(connection, dynamicDBName, 33);
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}