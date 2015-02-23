#pragma once

using namespace System;

[AttributeUsage(AttributeTargets::Class |
	AttributeTargets::Method)]
public ref class MainEntry : Attribute
{
public:
};

