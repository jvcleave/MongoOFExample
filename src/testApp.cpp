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