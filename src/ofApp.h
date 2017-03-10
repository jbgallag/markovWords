#pragma once
#include "ofMain.h"
#include <cstdio>
#include <cstdlib>
#include <locale>



class ofApp : public ofBaseApp {
    
public:
    
    void setup();
    void update();
    void draw();
    void AddWord();
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    float           nextLetterTime;
    int             lineCount;
    int             lineSize;
    int             letterCount;
    vector <string> seussLines;
    vector<string> arguments;
    map<string,int> uniqueWordMap;

    typedef map<string,float> wordMap;
    typedef map<float,string> rWordMap;
    typedef map<string,wordMap> markovChain;
    typedef map<string,rWordMap> markovRChain;

    markovChain myMarkovChain;
    markovRChain myProbChain;
    markovChain myMarkovChainActive;
    string firstWord;
    string displayWord;
    string activeWord;
    float changeTime;
    float stime;
    float etime;
    int resetLength;
    int strWidth;
    int nlevels;
    float sdTime;
    float doNothingTime;
    bool firstRun;
    bool brokeLoopOne;
    bool brokeLoopTwo;
    float deleteKey;
    markovChain::iterator itMrk;
    markovRChain::iterator itRMrk;
    markovChain::iterator itMrkActive;
    int wordCount;
    int nWordsToLine;
    
};
