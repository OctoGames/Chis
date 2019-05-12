#include "GameManager.h"


std::string GameManager::name_ = "GameManager";


GameManager::GameManager()
{
}

GameManager::~GameManager()
{
}

void GameManager::load(const std::map<std::string, ValueType>& params)
{
	auto it = params.begin();
	it = params.find("enabled_gm"); if (it != params.end()) enabled_ = params.at("enabled_gm").b;
}

Component * GameManager::clone()
{
	GameManager* clonedComponent = new GameManager();

	clonedComponent->enabled_ = this->enabled_;

	return clonedComponent;
}

void GameManager::init()
{
	gameObject()->setDestroyable(false);
	setEnabled(enabled_);
}