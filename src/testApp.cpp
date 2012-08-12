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
	
	
	ofSetLogLevel(OF_LOG_VERBOSE);

	//lets get a real absolute path to the data directory
	
	string currentPath = ofToDataPath("", true); //current path to bin/data, will have ../../etc - can't use it
	
	vector<string> parts = ofSplitString(currentPath, "bin");
	string realAbsPath = parts[0]+"bin/data/";
	string mongoConfPath = realAbsPath+"mongo/mongo.conf";
	ofFile confFile(mongoConfPath, ofFile::ReadWrite, false);
	confFile.copyTo(mongoConfPath+".bak", true);
	confFile.remove();//delete existing file
	string mongoDBpath = realAbsPath+"mongo/db";
	
	ofFile mongoDBPathCheck(mongoDBpath);
	if (!mongoDBPathCheck.exists()) 
	{
		ofDirectory mongoDBDir(mongoDBPathCheck);
		mongoDBDir.create();
	}
	stringstream ss;
	ss << "dbpath = " << mongoDBpath << endl;
	ss << "logpath =" << realAbsPath+"mongo/log/mongod.log" << endl;
	ss << "bind_ip = 127.0.0.1" << endl;
	ofBuffer output(ss);
	ofBufferToFile(mongoConfPath, output, false);
	string startCommand = "	/usr/local/bin/mongod run --config="+mongoConfPath+" > /dev/null 2>&1 &";
	ofLogVerbose() << "sending command" << startCommand.c_str() << endl;
	int status = -1;
	status = system(startCommand.c_str());
	if(status != 0)//strange but true
	{
		ofLogError() << "mongodb startup failed" << endl;
	}
	dynamicDBName = "tutorial.persons";
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