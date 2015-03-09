/*
** Author: Emmanuel Isidore
** Contribs:
*/

#pragma once

#include "ANode.hpp"
#include "Controllers\OnTheFly\CompilerAttr.hpp"
//#include "fan/toussa²/blablabla.h"//ajouter les fichiers fann au projet

public ref class FANNNode : public ANode {

private:
	int		outnb_;
	bool	generated_;
	//fann_type input_father_;
	//fann_type 

public:
	FANNNode();
	void build();
	generic<typename T>
	void Process(T value) override;
};