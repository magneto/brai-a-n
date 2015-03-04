/*
** Author: Emmanuel Isidore
** Contribs:
*/

#pragma once

#include "ANode.hpp"
#include "Controllers\OnTheFly\CompilerAttr.hpp"

public ref class FANNNode : public ANode{

private:
	int		outnb_;
	bool	generated_;

public:
	FANNNode();
	void build();
	generic<typename T>
	void Process(T value) override;
};