/*
** Author: Sebastien Maire
** COntribs:
*/

#include "Nodes\ANode.hpp"
#include "Nodes\CodeNode.hpp"
int main()
{
	ANode<int> ^in = gcnew CodeNode<int>(gcnew String("int var;"));

	in->Process(42);
	System::Console::ReadKey();
	return 0;
}