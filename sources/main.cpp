/*
** Author: Sebastien Maire
** COntribs:
*/

#include "Nodes\ANode.hpp"
#include "Nodes\CodeNode.hpp"

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
			[MainEntry]\
        public void SayHello(Dictionary<String, ANode>  childs, int v)							\
        {												\
            System.Console.WriteLine(\"In, v\");		\
			/*childs[\"Bar\"].Process(21);*/				\
		}												\
		    };											\
								}						\
												"));/*
	CodeNode ^two = gcnew CodeNode(gcnew String("		\
   	//[MainEntry]											\
    public class Bar									\
	{													\
		//[MainEntry]										\
	    public void SayHello(Dictionary<String ^, ANode ^> ^childs, int i)						\
	    {												\
	        System.Console.WriteLine(\"Child, i\");		\
		}												\
	};													\
	"));*/
	
	in->Process(42);
	}
	catch (Exception ^e) {
		Console::WriteLine(e->ToString());
	}
	System::Console::ReadKey();
	return 0;
}