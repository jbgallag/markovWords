#include "ofApp.h"
 bool mySort (int i,int j) { return (i<j); }
//--------------------------------------------------------------
void ofApp::setup() {
    if(arguments.size() != 7) {
	printf("Usage: ./markovWords <file> <seedWord> <waitTime> <lineLength> <nLevels> <waitTime>\n");
	std::exit(0);
     }
      
    firstWord = arguments.at(2);
    changeTime = atof(arguments.at(3).c_str());
    resetLength = atoi(arguments.at(4).c_str());
    nlevels = atoi(arguments.at(5).c_str());
    doNothingTime = atof(arguments.at(6).c_str());
    stime = 0;
    etime = 0; 
    sdTime = 0;
    firstRun = true;
    strWidth = 25;
    ofBackgroundHex(0x000000);
    ofSetFrameRate(30);

    lineCount      = 1;
    lineSize      = 100;
    deleteKey = 0.0;
    brokeLoopOne = false;
    brokeLoopTwo = false;
    int clevel; 
    nWordsToLine = (rand() % nlevels) + (nlevels/2);
    wordCount = 0;
    //seed random number
    srand((unsigned)time(0));
    // this is our buffer to stroe the text data
    ofBuffer buffer = ofBufferFromFile(arguments.at(1));
    
    if(buffer.size()) { 
	for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
	     string line = *it;
             if(line.empty() == false) {
		auto wordsInLine = ofSplitString(line," ");
		for(int i=0; i<nlevels; i++) {
                 clevel = nlevels - i;
		 for(int j=0; j<wordsInLine.size()-clevel; j++) {
                  string activeString;
                  for(int k=0; k<clevel; k++) {
		   if(k > 0)
                     activeString.append(" ");
                   activeString.append(wordsInLine[j+k]);
	          }
		   itMrk = myMarkovChain.find(activeString);
		   if(itMrk == myMarkovChain.end()) {
			wordMap *aWordMap = new wordMap;
			aWordMap->insert(std::pair<string,float>(wordsInLine[j+clevel],1.0));
			myMarkovChain.insert(std::pair<string,wordMap>(activeString,*aWordMap));
			delete aWordMap;
		   } else {
			wordMap::iterator itMM = itMrk->second.find(wordsInLine[j+clevel]);
			if(itMM == itMrk->second.end()) {
			  wordMap *aWordMap = &itMrk->second;
			  aWordMap->insert(std::pair<string,float>(wordsInLine[j+clevel],1.0));
			} else {
			  itMM->second = itMM->second + 1;
			}
		   }
		}
	     }
         }
     }	
  }
     for(markovChain::iterator itM = myMarkovChain.begin(); itM != myMarkovChain.end(); itM++) {
	float sum = 0.0;
	for(wordMap::iterator itMM = itM->second.begin(); itMM != itM->second.end(); itMM++) {
	   sum += itMM->second;
	}
	for(wordMap::iterator itMM = itM->second.begin(); itMM != itM->second.end(); itMM++) {
           itMM->second = itMM->second/sum;
	}
     }
    
     for(markovChain::iterator itM = myMarkovChain.begin(); itM != myMarkovChain.end(); itM++) {
	//go over word map pushing back values in tmpVector
	vector<float> tmpVector;
	for(wordMap::iterator itMM = itM->second.begin(); itMM != itM->second.end(); itMM++) {
	     tmpVector.push_back(itMM->second);
	}
	//sort the vector
	std::sort (tmpVector.begin(), tmpVector.end(), mySort);
	map<float,int> uniqProbs;
	for(vector<float>::iterator itV = tmpVector.begin(); itV != tmpVector.end(); itV++) {
	  map<float,int>::iterator itF = uniqProbs.find(*itV);
	  if(itF == uniqProbs.end()) {
	    uniqProbs[*itV] = 0;
	  } else {
	    uniqProbs[*itV] = uniqProbs[*itV] + 1;
	  }
	}
        float lastProb = 0.0;
	rWordMap *aRWordMap = new rWordMap;
	for(map<float,int>::iterator itV = uniqProbs.begin(); itV != uniqProbs.end(); itV++) {
	   for(wordMap::iterator itMM = itM->second.begin(); itMM != itM->second.end(); itMM++) {
		if(itMM->second == itV->first) {
		   aRWordMap->insert(std::pair<float,string>((lastProb+itV->first),itMM->first));
		   lastProb = (itV->first+lastProb);
		}
	    }
	}
        myProbChain.insert(std::pair<string,rWordMap>(itM->first,*aRWordMap));
	delete aRWordMap;
	tmpVector.clear();
	uniqProbs.clear();
     }
	
	   
      for(markovRChain::iterator itM = myProbChain.begin(); itM != myProbChain.end(); itM++) {
        for(rWordMap::iterator itRM = itM->second.begin(); itRM != itM->second.end(); itRM++) {
	   printf("%s %s %f\n",itM->first.c_str(),itRM->second.c_str(),itRM->first);
	}
      }

	   
printf("Finished building markov chain: %lu\n",myMarkovChain.size());
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw() {
    
    itRMrk = myProbChain.find(firstWord); 
    if(itRMrk == myProbChain.end()) {	
      auto splitWords = ofSplitString(firstWord," ");
      int idx = (int)splitWords.size()-1;
      itRMrk = myProbChain.find(splitWords[idx]);
      firstWord.clear();
      firstWord = splitWords[idx];
      activeWord.append("\n");

      printf("New level one word: %s\n",splitWords[idx].c_str());
    }


    if(stime == 0)
        stime = ofGetElapsedTimeMillis();
    if(sdTime == 0) {
        if(etime > changeTime)
	   AddWord();
	   wordCount++;
      } else {
         if(ofGetElapsedTimeMillis() - sdTime > doNothingTime) {
	       sdTime = 0;
               activeWord.clear();
         }
     }
	
    // draw the line
    float x = 50;
    float y = 50; 
    ofSetColor(0);
    ofDrawRectangle(x, y, strWidth, 15);
    ofSetColor(255);
    ofDrawBitmapString(activeWord, x+4, y+11);
    if(activeWord.length() > (lineSize*lineCount)) {
       lineCount++;
       activeWord.append("\n");
    }
    if(activeWord.length() > resetLength) {
	lineCount = 1;
	if(sdTime == 0) 
          sdTime = ofGetElapsedTimeMillis(); 
    }
    // this is our timer for grabbing the next letter
    if(stime != 0)
       etime = ofGetElapsedTimeMillis() - stime; 
}

void ofApp::AddWord()
{
 float rnum = ((double) rand())/RAND_MAX;
 if(firstRun) {
   activeWord.append(firstWord);
   firstRun = false;
 }


 float prevProb = 0.0;
 for(rWordMap::iterator itMM = itRMrk->second.begin(); itMM != itRMrk->second.end(); itMM++) {
   if(rnum > prevProb && rnum < itMM->first) {
        printf("Found Word: %f %f %f %s\n",prevProb,itMM->first,rnum,itMM->second.c_str());
      	firstWord.append(" ");
       	firstWord.append(itMM->second);
	} 
   prevProb = itMM->first;
  }


 auto splitWords = ofSplitString(firstWord," ");
 int idx = (int)splitWords.size()-1;
 activeWord.append(" ");
 activeWord.append(splitWords[idx]);

 etime = 0;
 stime = 0;

}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}
