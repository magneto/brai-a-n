/*
** Author: Sebastien Maire
** Contribs:
*/

#pragma once

#include "ANode.hpp"
#include "Controllers\OnTheFly\CompilerAttr.hpp"
#include "BrainView\include.h"

public ref class CodeNode : public ANode {
protected:
public:
	String ^code_;
	CompilerResults ^res_;
	Object ^instance_;
	MethodInfo ^entryPoint_;
	bool	rebuild_;
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
	void Build();
	generic<typename T>
	void Process(T value) override;
};