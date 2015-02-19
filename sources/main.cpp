/*
** Author: Sebastien Maire
** COntribs:
*/

#include "Nodes\ANode.hpp"
#include "Nodes\CodeNode.hpp"
int main()
{
	//CompilerConfig::Init();
	ANode<int> ^in = gcnew CodeNode<int>(gcnew String("	\
namespace Foo											\
{														\
    public class Bar									\
	{													\
        public void SayHello()							\
        {												\
            System.Console.WriteLine(\"Hello World\");	\
        }												\
    };													\
};														\
	"));

	in->Process(42);
ANode<ANode<int> ^> ^a = gcnew CodeNode<ANode<int> ^>("toto");
//a->Process(in); // specialisation (where T : class must be reference)
	System::Console::ReadKey();
	return 0;
}