# Compiler #
## 程序主要结构 
 - lex 为词法分析器，可生成Token串.
 - symboltable 为符号表，具体分为符号总表SYNBL，类型表TYPL，数组表AINFL，函数表FINAL，常量表CONSL，结构表STRUL和长度表LENL。
 - parser 为语法分析器、语义分析器及中间代码生成工具集合。
## 程序使用说明：
 1. 同目录下，grammar.txt 存放文法。
 2. 默认读取default.txt 代码
 3. 默认输出到output.txt 
## 命令行下可使用的参数有: 
 1. -i [filename] 输入源码文件
 2. -o [filename] 输出到文件
 3. -st  符号表
 4. -sm  状态机及各表
 5. -a  分析栈工作过程
 6. -t  Token串
