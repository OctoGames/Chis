#ifndef __CHIS_APP_H__
#define __CHIS_APP_H__

#include <Engine.h>

class ChisApp : public Application
{
public:
	ChisApp();
	~ChisApp();

	virtual void init();
	virtual void run();
	virtual void close();

	void registerFactories();
	void reset();

	static bool reset_;
};

#endif // !__CHIS_APP_H__