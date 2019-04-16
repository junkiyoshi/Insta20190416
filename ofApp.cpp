#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetLineWidth(2);
	ofEnableDepthTest();
	ofSetRectMode(ofRectMode::OF_RECTMODE_CENTER);

	ofTrueTypeFont font;
	font.loadFont("fonts/Kazesawa-Bold.ttf", 250, true, true, true);
	this->span = 10;

	ofFbo fbo;
	fbo.allocate(ofGetWidth(), ofGetHeight());
	fbo.begin();
	ofClear(0);
	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);

	string word = "TILE";
	font.drawString(word, font.stringWidth(word) * -0.5, font.stringHeight(word) * 0.5);

	fbo.end();

	ofPixels pixels;
	fbo.readToPixels(pixels);

	for (int x = 0; x < fbo.getWidth(); x += this->span) {

		for (int y = 0; y < fbo.getHeight(); y += this->span) {

			if (pixels.getColor(x, y) != ofColor(0, 0)) {

				this->locations.push_back(glm::vec2(x - ofGetWidth() * 0.5, ofGetHeight() - y - ofGetHeight() * 0.5));
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	float threshold = 0.8;
	for (auto& location : this->locations) {

		float noise_value = ofNoise(location.x * 0.005, location.y * 0.005, ofGetFrameNum() * 0.005);
		int z = 0;
		float rotate_x = 0.f;
		if (noise_value > threshold) {

			z += ofMap(noise_value, threshold, 1.0, 0, 300);
			rotate_x = ofMap(noise_value, threshold, 1.0, 0, 360 * 5);
		}

		auto draw_location = glm::vec3(location, z);
				
		ofPushMatrix();
		ofTranslate(draw_location);
		ofRotateX(rotate_x);
		ofRotateY(rotate_x);

		ofFill();
		ofSetColor(239);
		ofDrawRectangle(glm::vec3(), this->span - 1, this->span - 1);

		ofNoFill();
		ofSetColor(39);
		ofDrawRectangle(glm::vec3(), this->span, this->span);

		ofPopMatrix();
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}