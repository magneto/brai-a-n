/*
** Author: Emmanuel Isidore
** Contribs:
*/

#pragma once

#include "ANode.hpp"
#include "Controllers\OnTheFly\CompilerAttr.hpp"
#include "C:\FANN\include\fann.h"

public ref class FANNNode : public ANode{

private:
	int		outnb_;
	bool	generated_;
	struct fann *ann_;
	fann_type *input_father_;
	fann_type *output_;

public:
	FANNNode();
	~FANNNode();
	void build();
	generic<typename T>
	void Process(T value) override;
};