/*
 * Copyright (c) 2012-2013 Agnes Chavez and Alessandro Saccoia
 * Written by Alessandro Saccoia, <alessandro.saccoia@gmail.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Sotware.
 */


ofxControlPanel::setBackgroundColor(simpleColor(0, 0, 0, 255));
ofxControlPanel::setTextColor(simpleColor(255, 255, 255, 255));
controlPanel.defaultConfigurationDirectory = "configurations/";

int panelNr = 0;

controlPanel.addPanel("General settings", 4, false);
controlPanel.setWhichPanel(panelNr++);

/*
controlPanel.setWhichColumn(0);

controlPanel.addLabel("Configuration file");
m_configurationLister.listDir("configurations/");


ofFile m_initFile(INIT_FILENAME);
m_currentConfigurationFile = m_initFile.readToBuffer().getText(); // why the hell there's a newline after??
m_currentConfigurationFile = m_currentConfigurationFile.substr(0, m_currentConfigurationFile.size() - 1);
m_initFile.close();

int configIndex = 0;
for (; configIndex < m_configurationLister.entries.size(); ++configIndex) {
  if (m_configurationLister.getName(configIndex) == m_currentConfigurationFile)
    break;
}

//assert(m_configurationLister.entries.size() && configIndex < m_configurationLister.entries.size());
 
guiBaseObject* fileLister = controlPanel.addFileLister("Configuration", &m_configurationLister, 200, 200);
//m_configurationLister.setSelectedFile(configIndex);
//fileLister->setValue(configIndex, 0);
*/
controlPanel.setWhichColumn(0);
controlPanel.addLabel("Background");
controlPanel.addToggle("Use background image", "useBackgroundImage", true);
controlPanel.addSlider("Background trans. time", "backgroundTransitionTime", 10, 1, 120, false);
controlPanel.addLabel("Layers control");
controlPanel.addToggle("Activate flowers", "showFlowers", false);
controlPanel.addToggle("Activate leaves", "showLeaves", false);
controlPanel.addSlider("Trees opacity", "treesOpacity", 255, 0, 255, false);
controlPanel.addSlider("Twitter opacity", "twitterOpacity", 255, 0, 255, false);
controlPanel.addSlider("Splash opacity", "splashOpacity", 255, 0, 255, false);
controlPanel.addSlider("Flowers opacity", "flowersOpacity", 255, 0, 255, false);
controlPanel.addSlider("Ghost img time", "treesFadeTime", 30, 1, 60, false);
controlPanel.addSlider("Splash frequency", "splashFrequency", 60, 30, 600, false);
controlPanel.addSlider("Splash duration", "splashDuration", 15, 5, 60, false);
controlPanel.addSlider("Splash opacity", "splashOpacity", 100, 1, 255, true);



controlPanel.setWhichColumn(1);
//controlPanel.addLabel("Audio settings");
//m_soundtracksLister.listDir("soundtracks/");
//controlPanel.addFileLister("Soundtrack", &m_soundtracksLister, 200, 200);
//controlPanel.addToggle("Use interactive audio", "useInteractiveAudio", true);

controlPanel.addLabel("Loop settings");
//controlPanel.addSlider("Seed time", "waitSeedTime", 5, 1, 60, false);
controlPanel.addSlider("First iterations", "firstIterations", 3, 0, 10, true);

controlPanel.addSlider("Wait in seed mode", "waitSeedTime", 5, 0, 60, false);
controlPanel.addSlider("Wait for lines time", "waitLinesTime", 5, 0, 60, false);
controlPanel.addSlider("Detach leaves after time", "startDetachingLeavesTime", 5, 0, 60, false);
controlPanel.addSlider("Wait after regenerate time", "waitRegenerateTime", 5, 0, 60, false);

controlPanel.setWhichColumn(2);
controlPanel.addLabel("Twitter settings");
/*
controlPanel.addToggle("Timed show", "timedExhibit", true);
controlPanel.addSlider("Show duration (min)", "showDuration", 10, 1, 60, true);
controlPanel.addSlider("Nr. past tweets", "pastTweets", 5, 0, 100, true);
*/
controlPanel.addToggle("Show twitter thumbnails", "showThumbs", 1);
controlPanel.addToggle("Joints are circles", "jointsAreCircles", true);

controlPanel.addLabel("General tree structure");
controlPanel.addSlider("Fertility", "fertility", 0, 0, 5, true);
controlPanel.addSlider("Max levels", "maxLevel", 10, 3, 20, true);
controlPanel.addSlider("Min colonization to regenerate", "minColonization", 30, 1, 100, true);
controlPanel.addSlider("Min colonization to create leaves", "minColonizationForLeaves", 20, 1, 100, true);

controlPanel.addLabel("Leaves");
controlPanel.addSlider("Leaves rows", "leavesRows", 8, 1, 20, true);
controlPanel.addSlider("Leaves columns", "leavesColumns", 6, 1, 20, true);
controlPanel.addSlider("Leaves life", "leavesLife", 100, 1, 200, true);



//controlPanel.addLabel("Twilio settings");
//controlPanel.addToggle("Use twilio", "useTwilio", true);

controlPanel.setWhichColumn(3);
controlPanel.addLabel("Timing");
controlPanel.addSlider("Branch min (ms)", "growthMin", 1500, 1000, 10000, true);
controlPanel.addSlider("Branch max (ms)", "growthMax", 3000, 1000, 10000, true);
controlPanel.addSlider("Twitter min (ms)", "minFrequency", 3000, 100, 20000, true);
controlPanel.addSlider("Twitter max (ms)", "maxFrequency", 7000, 100, 20000, true);
controlPanel.addSlider("Lines min (ms)", "LinesMin", 50, 10, 10000, false);
controlPanel.addSlider("Lines max (ms)", "LinesMax", 300, 10, 10000, false);
controlPanel.addSlider("Flowers min (ms)", "flowersMin", 50, 10, 10000, false);
controlPanel.addSlider("Flowers max (ms)", "flowersMax", 300, 10, 10000, false);
controlPanel.addSlider("Leaves min (ms)", "leavesMinFreq", 5000, 100, 100000, false);
controlPanel.addSlider("Leaves max (ms)", "leavesMaxFreq", 15000, 100, 100000, false);

controlPanel.addPanel("Messages", 4, false);
controlPanel.setWhichPanel(panelNr++);
controlPanel.setWhichColumn(0);
controlPanel.addSlider("Message opacity", "msgOpacity", 150, 0, 255, false);
controlPanel.addSlider("Line R", "tLineR", 0, 0, 255, true);
controlPanel.addSlider("Line G", "tLineG", 0, 0, 255, true);
controlPanel.addSlider("Line B", "tLineB", 0, 0, 255, true);
controlPanel.addSlider("Line width", "tLineWidth", 1, 1, 10, true);
controlPanel.addSlider("Floating speed", "floatingSpeed", 20, 1, 100, true);
controlPanel.addSlider("Min life (ms)", "tweetMin", 5000, 1000, 20000, false);
controlPanel.addSlider("Max life (ms)", "tweetMax", 10000, 1000, 20000, false);


controlPanel.setWhichColumn(1);
controlPanel.addLabel("Twitter messages");
controlPanel.addSlider("Message size (px)", "tweetMsgSize", 10, 6, 20, true);
controlPanel.addSlider("Message R", "tweetMsgR", 255, 0, 255, true);
controlPanel.addSlider("Message G", "tweetMsgG", 255, 0, 255, true);
controlPanel.addSlider("Message B", "tweetMsgB", 255, 0, 255, true);
controlPanel.addSlider("User R", "tweetUsR", 255, 0, 255, true);
controlPanel.addSlider("User G", "tweetUsG", 255, 0, 255, true);
controlPanel.addSlider("User B", "tweetUsB", 255, 0, 255, true);

controlPanel.setWhichColumn(2);
controlPanel.addLabel("Twilio messages");
controlPanel.addSlider("Message size (px)", "twilioMsgSize", 10, 6, 20, true);
controlPanel.addSlider("Message R", "twilioMsgR", 255, 0, 255, true);
controlPanel.addSlider("Message G", "twilioMsgG", 255, 0, 255, true);
controlPanel.addSlider("Message B", "twilioMsgB", 255, 0, 255, true);
controlPanel.addSlider("User R", "twilioUsR", 255, 0, 255, true);
controlPanel.addSlider("User G", "twilioUsG", 255, 0, 255, true);
controlPanel.addSlider("User B", "twilioUsB", 255, 0, 255, true);

controlPanel.setWhichColumn(3);
controlPanel.addLabel("Database messages");
controlPanel.addSlider("Message size (px)", "databaseMsgSize", 10, 6, 20, true);
controlPanel.addSlider("Message R", "databaseMsgR", 255, 0, 255, true);
controlPanel.addSlider("Message G", "databaseMsgG", 255, 0, 255, true);
controlPanel.addSlider("Message B", "databaseMsgB", 255, 0, 255, true);
controlPanel.addSlider("User R", "databaseUsR", 255, 0, 255, true);
controlPanel.addSlider("User G", "databaseUsG", 255, 0, 255, true);
controlPanel.addSlider("User B", "databaseUsB", 255, 0, 255, true);


controlPanel.addPanel("Other", 4, false);
controlPanel.setWhichPanel(panelNr++);
controlPanel.setWhichColumn(0);
controlPanel.addLabel("Background color");
controlPanel.addSlider("Background R", "backgroundR", 0, 0, 255, true);
controlPanel.addSlider("Background G", "backgroundG", 0, 0, 255, true);
controlPanel.addSlider("Background B", "backgroundB", 0, 0, 255, true);
controlPanel.addLabel("Splash screen");
controlPanel.addSlider("Splash Screen R", "splashR", 255, 0, 255, true);
controlPanel.addSlider("Splash Screen G", "splashG", 255, 0, 255, true);
controlPanel.addSlider("Splash Screen B", "splashB", 255, 0, 255, true);

controlPanel.setWhichColumn(1);
controlPanel.addLabel("Branches");
controlPanel.addSlider("Branch R", "treeR", 255, 0, 255, true);
controlPanel.addSlider("Branch G", "treeG", 255, 0, 255, true);
controlPanel.addSlider("Branch B", "treeB", 255, 0, 255, true);
controlPanel.addSlider("Branch A", "treeA", 255, 0, 255, true);
controlPanel.addSlider("Branch width (px)", "branchWidth", 1, 1, 30, true);
controlPanel.addSlider("Branch min width (px)", "branchMinWidth", 1, 1, 30, true);
controlPanel.addSlider("Width decrease (%)", "widthDecrease", 1, 1, 100, true);
controlPanel.addSlider("Branch length (px)", "branchLength", 100, 50, 500, true);
controlPanel.addSlider("Length decrease (%)", "lengthDecrease", 1, 1, 100, true);
controlPanel.addSlider("Min angle (deg.)", "minAngle", 1, 1, 90, false);
controlPanel.addSlider("Max angle (deg)", "maxAngle", 90, 1, 90, false);

controlPanel.setWhichColumn(2);
controlPanel.addLabel("Branch joints");
controlPanel.addSlider("Joint R", "jointR", 255, 0, 255, true);
controlPanel.addSlider("Joint G", "jointG", 0, 0, 255, true);
controlPanel.addSlider("Joint B", "jointB", 0, 0, 255, true);
controlPanel.addSlider("Joint thickness", "jointThickness", 1, 1, 10, true);
controlPanel.addSlider("Joint length", "jointLength", 20, 0, 80, true);
controlPanel.addLabel("Flowers");
controlPanel.addSlider("Line Width", "crazyLineWidth", 2, 1, 10, true);
controlPanel.addSlider("Circle Width", "crazyCircleWidth", 15, 2, 50, true);
controlPanel.addSlider("Flowers R", "tFlowersR", 255, 0, 255, true);
controlPanel.addSlider("Flowers G", "tFlowersG", 255, 0, 255, true);
controlPanel.addSlider("Flowers B", "tFlowersB", 255, 0, 255, true);
controlPanel.addSlider("Flowers lines R", "tFlowersLinesR", 255, 0, 255, true);
controlPanel.addSlider("Flowers lines G", "tFlowersLinesG", 255, 0, 255, true);
controlPanel.addSlider("Flowers lines B", "tFlowersLinesB", 255, 0, 255, true);

controlPanel.setWhichColumn(3);
controlPanel.addLabel("Leaves");
controlPanel.addSlider("Leaf R", "tLeavesR", 0, 0, 255, true);
controlPanel.addSlider("Leaf G", "tLeavesG", 0, 0, 255, true);
controlPanel.addSlider("Leaf B", "tLeavesB", 0, 0, 255, true);
controlPanel.addSlider("Leaf Opacity", "tLeavesA", 255, 0, 255, true);
controlPanel.addSlider("Leaf fertility", "leafFertility", 1, 1, 10, true);
controlPanel.addSlider("Leaf min width", "leafMinWidth", 100, 5, 300, true);
controlPanel.addSlider("Leaf max width", "leafMaxWidth", 140, 5, 300, true);

controlPanel.addPanel("Audio Setup", 4, false);
controlPanel.setWhichPanel(panelNr++);
controlPanel.setWhichColumn(1);
controlPanel.addSlider("Global Volume", "soundVolume", 30, 0, 100, false);
controlPanel.addLabel("Parametric Equalizer");
controlPanel.addSlider("eq100", "eq100", 30, 0, 100, true);
controlPanel.addSlider("eq250", "eq250", 44, 0, 100, true);
controlPanel.addSlider("eq700", "eq700", 50, 0, 100, true);
controlPanel.addSlider("eq1500", "eq1500", 50, 0, 100, true);
controlPanel.addSlider("eq3000", "eq3000", 50, 0, 100, true);
controlPanel.addSlider("eq7000", "eq7000", 50, 0, 100, true);
controlPanel.addSlider("eq15000", "eq15000", 50, 0, 100, true);
controlPanel.addSlider("eqGain", "eqGain", 50, 0, 100, true);

