#pragma once
#include <string>
namespace Error {
	namespace pairError {
		std::string  noteError_1 = "注释出错，没有找到 */，程序结束";
	}
	namespace formError {
		std::string numberError_1 = "数字格式有错误";
		std::string identiferError_1 = "标识符格式错误";
		std::string unkownerror_1 = "未找到匹配的保留字或者标识符格式错误";
	}
	namespace actionError {
		std::string scanner_finished_error = "扫描已完成";
	}
}