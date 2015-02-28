/*
** Author: Sebastien Maire
** COntribs:
*/

#include "Models\Tree\Nodes\CodeNode.hpp"

int main()
{
	try {
		CodeNode ^in = gcnew CodeNode(gcnew String("		\
												      using System.Collections.Generic;\
													     using System;\
	namespace toto {									\
		[MainEntry]										\
    public class Babar									\
	{													\
			[MainEntry]									\
        public void SayHello(Dictionary<String, ANode>  childs, int v)							\
        {												\
				System.Console.WriteLine(\"aaff\"+v);			\
			childs[\"Bar\"].Process(v);				\
		}												\
		    };											\
								}						\
												"));
	CodeNode ^two = gcnew CodeNode(gcnew String("		\
												using System; \
												using System.Collections.Generic;\
   	[MainEntry]											\
    public class Bar									\
	{													\
		[MainEntry]										\
	    public void SayHello(Dictionary<String , ANode > childs, int i)						\
	    {												\
		System.Console.WriteLine(i);		\
		}												\
	};													\
	"));
	in->AddChild("Bar", two);
	int i = 0;
	bool trigger = true;
	while (1) {
		if (trigger) {
			i++;
			if (i > 50)
				trigger = false;
		}
		else {
			i--;
			if (i <= 0)
				trigger = true;
		}
		in->Process(i);
		System::Threading::Thread::Sleep(250);
	}
	}
	catch (Exception ^e) {
		Console::WriteLine(e->ToString());
	}
	System::Console::ReadKey();
	return 0;
}