#ifndef _APPLICATION_H_
#define _APPLICATION_H_

class Application
{
public:
	Application();
	~Application();

	void run();

	inline bool isRunning() const { return running_; }
	inline void setRunning(bool running) { running_ = running; }

private:
	bool running_;

	void initSystems();
	void closeSystems();
	void initDemo();

	void handleInput();
	void update();
	void render();
};

#endif // !_APPLICATION_H_
