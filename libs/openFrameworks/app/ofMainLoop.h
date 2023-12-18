#pragma once

#include "ofEvents.h"
#include <unordered_map>

class ofBaseApp;
class ofAppBaseWindow;
class ofWindowSettings;

class ofMainLoop {
public:
	ofMainLoop();
	virtual ~ofMainLoop();

	std::shared_ptr<ofAppBaseWindow> createWindow(const ofWindowSettings & settings);

	template<typename Window>
	void addWindow(const std::shared_ptr<Window> & window){
		allowMultiWindow = Window::allowsMultiWindow();
		if(Window::doesLoop()){
			windowLoop = Window::loop;
		}
		if(Window::needsPolling()){
			windowPollEvents = Window::pollEvents;
		}
		if(!allowMultiWindow){
			windowsApps.clear();
		}
		windowsApps[window] = std::shared_ptr<ofBaseApp>();
		currentWindow = window;
		ofAddListener(window->events().keyPressed,this,&ofMainLoop::keyPressed);
	}

		// VWCUSTOM  - ADDED FUNCTION BEGIN
	// ++++++++++++ Custom added functions +++++++++++
	// !!!!!!!!!!!! This seems to Work !!!!!!!!!!!!!!
	// Safely delete a ofAppBaseWindow and its associated OfBaseApp
	// that also deletes any derived class created from them.
	template<typename Window>
	bool delete_window(std::shared_ptr<Window> window) {
		auto index = windowsApps.find(window); // Find iterator index of window in running apps

		if (index == windowsApps.end()) return false; // window not found in running apps

		window->close();          // Close window and assiciated running app
		windowsApps.erase(index); // remove references to running app from mapping list 
		// so as no attempt is made to execute a non existing app in the main loop

		return true;
	}
	// VWCUSTOM  - ADDED FUNCTION END
	// ++++++++++++++++++++++++++++++++++++++++++++++

	void run(const std::shared_ptr<ofAppBaseWindow> & window, std::shared_ptr<ofBaseApp> && app);
	void run(std::shared_ptr<ofBaseApp> && app);
	int loop();
	void loopOnce();
	void pollEvents();
	void exit();
	void shouldClose(int status);
	std::shared_ptr<ofAppBaseWindow> getCurrentWindow();
	void setCurrentWindow(const std::shared_ptr<ofAppBaseWindow> & window);
	void setCurrentWindow(ofAppBaseWindow * window);
	std::shared_ptr<ofBaseApp> getCurrentApp();
	void setEscapeQuitsLoop(bool quits);

	ofEvent<void> exitEvent;
	ofEvent<void> loopEvent;
	
	std::thread::id get_thread_id() { return thread_id; };

private:
	std::thread::id thread_id { std::this_thread::get_id() };

	void keyPressed(ofKeyEventArgs & key);
	std::unordered_map<std::shared_ptr<ofAppBaseWindow>, std::shared_ptr<ofBaseApp> > windowsApps;
	bool bShouldClose;
	std::weak_ptr<ofAppBaseWindow> currentWindow;
	int status;
	bool allowMultiWindow;
	std::function<void()> windowLoop;
	std::function<void()> windowPollEvents;
	bool escapeQuits;
};
