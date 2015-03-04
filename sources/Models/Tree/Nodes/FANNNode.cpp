/*
** Author: Emmanuel Isidore
** Contribs:
*/

#include "C:\FANN\include\fann.h"
#include "FANNNode.hpp"

FANNNode::FANNNode():
outnb_(0), generated_(false)
{}

void FANNNode::build()
{}

generic<typename T>
void FANNNode::Process(T value)
{
	if ((generated_ == false) || (outnb_ != children_->Count))
		build();

}