#pragma once

#ifndef HOOKS_HPP
#define HOOKS_HPP

class DetouredFunction_Container;

namespace Hooks
{
	void Initialise();
	void Destroy();

	extern DetouredFunction_Container* g_phkRunString;
	extern DetouredFunction_Container* g_phkCreateMove;
}

#endif