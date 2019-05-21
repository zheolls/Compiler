#pragma once
#include <string>
namespace Error {
	namespace  pairError {
		const std::string  noteError_1 = "注释出错，没有找到 */，程序结束";
	};
	namespace formError {
		const std::string numberError_1 = "数字格式有错误";
		const std::string identiferError_1 = "标识符格式错误";
		const std::string unkownerror_1 = "未找到匹配的保留字或者标识符格式错误";
	};
	namespace actionError {
		const std::string scanner_finished_error = "扫描已完成";
	};
	namespace stateError {
		const std::string statement_error = "声明错误";
		const std::string unique_identifer = "标识符重复定义";
		const std::string type_errot = "类型未定义";
		const std::string identifer_not_defined = "标识符未定义";
		const std::string derivation_error = "语义动作过多";
	};
	namespace typeError {
		const std::string havenotype = "没有对应的类型";
	};
	namespace SyntexError {
		const std::string syntexerror = "语法错误";
		const std::string syntexerror_1 = "超出文法表示范围";
	};
};