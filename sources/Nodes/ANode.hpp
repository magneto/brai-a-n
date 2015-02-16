/*
** Author: Sebastien Maire
** Contribs:
*/

#pragma once

using namespace System;

generic<typename T>
public ref class ANode abstract {
public:
	virtual void Process(const T% val) abstract;
};