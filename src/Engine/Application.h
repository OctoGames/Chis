#ifndef __APPLICATION_H__
#define __APPLICATION_H__

class Application
{
public:
	Application();
	virtual ~Application();

	virtual void init();
	virtual void run();
	virtual void close();
};

#endif // !__APPLICATION_H__