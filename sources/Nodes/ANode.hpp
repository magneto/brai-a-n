/*
** Author: Sebastien Maire
** Contribs:
*/

#pragma once

using namespace System;

generic<typename T>
public ref class ANode abstract {
public:
	virtual void Process(T %val) abstract;
	virtual void Process(T val) abstract;
};