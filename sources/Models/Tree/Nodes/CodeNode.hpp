/*
** Author: Sebastien Maire
** Contribs:
*/

#pragma once

#include "ANode.hpp"
#include "Controllers\OnTheFly\CompilerAttr.hpp"

public ref class CodeNode : public ANode {
protected:
	String ^code_;
	CompilerResults ^res_;
	Object ^instance_;
	MethodInfo ^entryPoint_;
	bool	rebuild_;
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
	CodeNode(String ^code);
	void Build();
	generic<typename T>
	void Process(T value) override;
};