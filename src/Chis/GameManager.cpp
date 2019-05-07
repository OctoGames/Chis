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
	return;
}

Component * GameManager::clone()
{
	GameManager* clonedComponent = new GameManager();

	clonedComponent->enabled_ = this->enabled_;

	return clonedComponent;
}

void GameManager::init()
{
	setEnabled(enabled_);
}