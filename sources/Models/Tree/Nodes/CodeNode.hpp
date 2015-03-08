/*
** Author: Sebastien Maire
** Contribs:
*/

#pragma once

#include "ANode.hpp"
#include "Controllers\OnTheFly\CompilerAttr.hpp"
#include "BrainView\include.h"

[SerializableAttribute]
public ref class CodeNode : public ANode {
protected:
public:
	String ^code_;
	CompilerResults ^res_;
	Object ^instance_;
	MethodInfo ^entryPoint_;
	bool	rebuild_;
	[NonSerializedAttribute]
	TextBlock^ console_;
public:
	enum class LanguageSel {
		CSHARP = 0,
		VB = 1,
		JSCRIPT = 2,
	};
	LanguageSel language_;
	property LanguageSel Language {
		LanguageSel get() {
			return language_;
		}
		void set(LanguageSel l) {
			language_ = l;
		}
	}
	CodeNode(String ^code, TextBlock^ console);
	CodeNode(TextBlock ^console);
	String	^generateTemplateCode(LanguageSel lang);
	virtual void Build() override;
	generic<typename T>
	void Process(T value) override;
};