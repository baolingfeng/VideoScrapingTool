#include "GUICompenent.h"


GUIComponent::GUIComponent(void)
{
	this->tobescraped = false;
}


GUIComponent::~GUIComponent(void)
{
	for(int i=0; i<this->components.size(); i++)
	{
		delete components[i];
	}
}
