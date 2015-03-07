/*
** Author: Sebastien Maire
** Contribs:
*/

#include "CodeNode.hpp"
#include "Controllers\OnTheFly\CompilerAttr.hpp"
#include "Models\OnTheFly\MainEntryAttr.hpp"

using namespace System;
using namespace OnTheFly;

String	^CodeNode::generateTemplateCode(LanguageSel lang) {
	StringBuilder	^builder = gcnew StringBuilder();

	builder->Append("using System;\n");
	builder->Append("using System.Collections.Generic;\n\n");
	builder->Append("[MainEntry]\n");
	builder->Append(String::Format("class {0}\n", name_ + "Class"));
	builder->Append("{\n");
	builder->Append("[MainEntry]\n");
	builder->Append(String::Format("public void {0}(Dictionary<String, ANode> children, Object o)\n", name_ + "Func"));
	builder->Append("{\n");
	builder->Append("// Code Goes here, see documentation to know how it works\n");
	builder->Append("}\n");
	builder->Append("};\n");
	return builder->ToString();
}

CodeNode::CodeNode(String ^code, TextBlock^ console) :
	ANode(),
	code_(code),
	res_(nullptr),
	language_(LanguageSel::CSHARP),
	console_(console) {
}

CodeNode::CodeNode(TextBlock ^console) :
	ANode(),
	code_(""),
	res_(nullptr),
	language_(LanguageSel::CSHARP),
	console_(console) {
	code_ = generateTemplateCode(LanguageSel::CSHARP);
}

void CodeNode::Build() {
	CompilerResults ^newRes = nullptr;

	newRes = CompilerAttr::provider[(int)language_]->CompileAssemblyFromSource(CompilerAttr::compilerParams, code_);
	if (newRes->Errors->Count != 0) {
		console_->Text += String::Format("ERROR: [0x04]: Cannot build due to source build fail on node \"{0}\".\n", name_);
		for each (CompilerError ^ce in newRes->Errors) {
			console_->Text += String::Format("\t" + ce->ToString() + "\n");
		}
		return ;
	}
	array<Type ^>	^types = newRes->CompiledAssembly->GetTypes();
	Type ^entryClass = nullptr;
	for each (Type ^t in types) {
		if (entryClass && t->GetCustomAttributes(MainEntry::typeid, false)->Length > 0) {
			console_->Text += String::Format("ERROR: [0x02]: Cannot build because there are too many EntryClass.\n");
			return ;
		}
		if (t->GetCustomAttributes(MainEntry::typeid, false)->Length > 0) {
			entryClass = t;
		}
	}
	if (!entryClass) {
		console_->Text += String::Format("ERROR: [0x00]: Cannot build because there’s no EntryClass. (Must set [MainEntry] on a class.)\n");
		return ;
	}
	entryPoint_ = nullptr;
	for each (MethodInfo ^i in entryClass->GetMethods()) {
		if (entryPoint_ && i->GetCustomAttributes(MainEntry::typeid, false)->Length > 0) {
			console_->Text += String::Format("ERROR: [0x03]: Cannot build because there are too many EntryMethod in node \"{0}\".\n", name_);
			return ;
		}
		Console::WriteLine(i->Name);
		if (i->GetCustomAttributes(MainEntry::typeid, false)->Length > 0) {
			entryPoint_ = i;
		}
	}
	if (!entryPoint_) {
		console_->Text += String::Format("ERROR: [0x01]: Cannot build because there’s no EntryMethod. (Must set [MainEntry] on a method of the class tagged [MainEntry]\"{0}\".)\n", name_);
		return ;
	}
	instance_ = newRes->CompiledAssembly->CreateInstance(entryClass->FullName);
	res_ = newRes;
}

generic<typename T>
void CodeNode::Process(T value) {
	if (rebuild_) {
		this->Build();
		rebuild_ = false;
	}
	if (res_) {
		entryPoint_->Invoke(instance_, gcnew array < Object ^ > {children_, value});
	}
}
