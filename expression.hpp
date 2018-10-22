#ifndef _MAHJONG_ALGORITHM_EXPRESSION_H_
#define _MAHJONG_ALGORITHM_EXPRESSION_H_

#include <string>
#include <algorithm>
#include <stack>
#include <vector>
#include <map>
#include <cctype>
#include <sstream>


/*
创建房间时：
0.房间开始时会读取配置中的公式
1.读取完毕后会进行分词存储
牌局结束时：
1.结束时会根据规则组装公式
2.组装的公式为中缀表达式
->	(1) 一个是普通的纯计算的公式
(2) 带详细说明的计算公式 例如：(清一色2+碰碰胡2)*底分0.5+大喜3*10+小喜4*20
3.(1)转后缀表达式
4.计算值得出结果
*/

namespace EXPRESSION
{
	typedef std::vector<std::string> exprVec;

	//-----------------------------------------------------------------------------
	//操作符
	static const std::string operatorList[] =
	{
		"+", "-", "*", "/", "%", "(", ")", ","
	};
	static const std::size_t operatorListSize = sizeof(operatorList) / sizeof(char);

	//-----------------------------------------------------------------------------
	//功能函数
	static const std::string funcList[] =
	{
		//最大最小 目前只支持2参数
		//向上 向下取整
		//绝对值 平均值 包三搭
		"min", "max", "ceil", "floor", "sqrt", "abs", "bsd"
	};
	static const std::size_t funcListSize = sizeof(funcList) / sizeof(std::string);

	//-----------------------------------------------------------------------------
	//一些有特殊含义的值
	static const std::string specialList[] =
	{
		"limit", "base", "player"
	};
	static const std::size_t specialListSize = sizeof(specialList) / sizeof(std::string);

	//-----------------------------------------------------------------------------
	//操作符优先级
	int getOpsLevel(const char c)
	{
		switch (c)
		{
		case '+':
		case '-':
			return 1;
		case '*':
		case '/':
		case '%':
			return 2;
		case  '^':
			return 3;
		case '(':
			return 0;
		default:
			break;
		}
		return -1;
	}
	int getOpsLevel(const std::string& ops)
	{
		if (ops.compare("+") == 0 || ops.compare("-") == 0)
		{
			return 1;
		}
		else if (ops.compare("*") == 0 || ops.compare("/") == 0 || ops.compare("%") == 0)
		{
			return 2;
		}
		else if (ops.compare("^") == 0)
		{
			return 3;
		}
		else if (ops.compare("(") == 0)
		{
			return 4;
		}
		return -1;
	}


	//-----------------------------------------------------------------------------
	//判断无效字符
	inline bool isValid(const char c)
	{
		return isspace(c) || iscntrl(c);
	}

	//-----------------------------------------------------------------------------
	//判断操作符
	inline bool isOperator(const char c)
	{
		std::string ops;
		ops += c;
		for (std::size_t i = 0; i < operatorListSize; ++i)
		{
			if (operatorList[i] == ops)
			{
				return true;
			}
		}
		return false;
	}
	//-----------------------------------------------------------------------------
	//判断操作符
	inline bool isOperator(const std::string& ops)
	{
		for (std::size_t i = 0; i < operatorListSize; ++i)
		{
			if (operatorList[i] == ops)
			{
				return true;
			}
		}
		return false;
	}

	//-----------------------------------------------------------------------------
	//判断函数
	inline bool isFunc(const std::string& func)
	{
		for (std::size_t i = 0; i < funcListSize; ++i)
		{
			if (funcList[i] == func)
			{
				return true;
			}
		}
		return false;
	}

	//-----------------------------------------------------------------------------
	//判断内置特殊词
	inline bool isSpecialWord(const std::string& word)
	{
		for (std::size_t i = 0; i < specialListSize; ++i)
		{
			if (word.compare(specialList[i]) == 0)
			{
				return true;
			}
		}
		return false;
	}

	//-----------------------------------------------------------------------------
	//判断数字
	inline bool isDigit(const char c)
	{
		return isdigit(c);
	}
	inline bool isDigit(const std::string& s)
	{
		for (std::size_t i = 0; i < s.size(); ++i)
		{
			if (!isDigit(s[i]))
			{
				return false;
			}
		}
		return true;
	}

	//-----------------------------------------------------------------------------
	//判断字母
	inline bool isLetter(const char c)
	{
		return isalpha(c);
	}
	inline bool isLetter(const std::string& s)
	{
		for (std::size_t i = 0; i < s.size(); ++i)
		{
			if (!isLetter(s[i]))
			{
				return false;
			}
		}
		return true;
	}

	//-----------------------------------------------------------------------------
	//判断左括号
	inline bool isLeftBracket(const char c)
	{
		return (c == '(');
	}
	//-----------------------------------------------------------------------------
	//判断左括号
	inline bool isLeftBracket(const std::string& ops)
	{
		return ops.compare("(") == 0;
	}

	//-----------------------------------------------------------------------------
	//判断右括号
	inline bool isRightBracket(const char c)
	{
		return (c == ')');
	}
	//-----------------------------------------------------------------------------
	//判断右括号
	inline bool isRightBracket(const std::string& ops)
	{
		return ops.compare(")") == 0;
	}

	//-----------------------------------------------------------------------------
	//判断逗号
	inline bool isComma(const std::string& ops)
	{
		return ops.compare(",") == 0;
	}

	//-----------------------------------------------------------------------------
	//判断函数和特殊值
	inline bool hasFuncAndSpecial(const exprVec& expr)
	{
		for (std::size_t pos = 0; pos < expr.size(); ++pos)
		{
			if (isSpecialWord(expr[pos])
				|| isFunc(expr[pos]))
			{
				return true;
			}
		}
		return false;
	}

	//-----------------------------------------------------------------------------
	//字符串转小写
	void caseNormaliseToLower(std::string& str)
	{
		std::transform(str.begin(), str.end(), str.begin(), tolower);
	}

	//-----------------------------------------------------------------------------
	//字符串转大写
	void caseNormaliseToUpper(std::string& str)
	{
		std::transform(str.begin(), str.end(), str.begin(), toupper);
	}

	template<typename T = double>
	T handlerOperator(const std::string& ops, T n1, T n2)
	{
		if (ops.compare("+") == 0)
		{
			return n2 + n1;
		}
		else if (ops.compare("-") == 0)
		{
			return n2 - n1;
		}
		else if (ops.compare("*") == 0)
		{
			return n2*n1;
		}
		else if (ops.compare("/") == 0)
		{
			return n2 / n1;
		}
		else if (ops.compare("%") == 0)
		{
			return int(n2) % int(n1);
		}
		else if (ops.compare("^") == 0)
		{
			std::pow(n2, n1);
		}
		return T(0);
	}

	//内置函数
	//单参数
	template<typename T>
	inline T ceilImpl(const T v)
	{
		return std::ceil(v);
	}
	template<typename T>
	inline T floorImpl(const T v)
	{
		return std::floor(v);
	}
	template<typename T>
	inline T absImpl(const T v)
	{
		return (v < T(0) ? -v : v);
	}
	template<typename T>
	inline T sqrtImpl(const T v)
	{
		return std::sqrt(v);
	}
	//多参数
	template<typename T>
	inline T minImpl(const T v1, const T v2)
	{
		return std::min<T>(v1, v2);
	}
	template<typename T>
	inline T maxImpl(const T v1, const T v2)
	{
		return std::max<T>(v1, v2);
	}

	template<typename T>
	inline bool isZero(const T v)
	{
		return static_cast<int>(v) == 0;
	}

	class lexer
	{
	public:
		//分词
		static exprVec exprParticiple(std::string& exprStr)
		{
			caseNormaliseToLower(exprStr);

			exprVec vecExpr;
			std::string strToken;
			/*
			忽略无效字符
			遇到非符号就存储
			遇到操作符代表之前存储的形成一个token
			存储token,存储操作符
			*/
			std::size_t pos = 0;
			for (; pos != exprStr.size(); ++pos)
			{
				const char& c = exprStr[pos];
				//无效字符不记录
				if (isValid(c))continue;
				//是操作符直接存储
				if (isOperator(c))
				{
					if (!strToken.empty())
					{
						vecExpr.push_back(strToken);
						strToken.clear();
					}

					std::string piece;
					piece.push_back(c);
					vecExpr.push_back(piece);
					continue;
				}

				strToken.push_back(c);
				if ((pos + 1) == exprStr.size())
				{
					vecExpr.push_back(strToken);
				}
			}

			return vecExpr;
		}
	};

	//表达式解析
	/*
	1.所有数字都用double计算
	2.采用后缀表达式形式
	*/
	template<class T>
	class ExpressionParser
	{
	public:
		ExpressionParser() {}
		~ExpressionParser() {}
		ExpressionParser(std::string& exp)
			:m_vecExpr(lexer::exprParticiple(exp))
		{

		}
		ExpressionParser(exprVec& vecExpr)
			:m_vecExpr(vecExpr)
		{
			_expNifixToSuffix();
		}
	public:
		T getResult()
		{
			return _getResult();
		}
	private:
		//求值
		T _getResult()
		{
			double res = 0.0f;

			//单数字
			if (m_vecExprSuffix.size() == 1 && isDigit(m_vecExprSuffix[0]))
			{
				std::stringstream ss;
				ss << m_vecExprSuffix[0];
				ss >> res;

				return res;
			}

			std::stack<std::string> stNum;

			std::size_t pos = 0;
			while (pos != m_vecExprSuffix.size())
			{
				/*
				后缀表达式求值
				1.遇到数字全部入栈
				2.遇到操作符取栈顶两个数进行计算
				3.栈中唯一的数字是最终解
				*/
				const std::string& token = m_vecExprSuffix[pos];

				if (isOperator(token))
				{
					double n1 = 0.0f;
					double n2 = 0.0f;
					std::stringstream ss;
					ss << stNum.top();
					ss >> n1;
					stNum.pop();

					ss.clear();

					ss << stNum.top();
					ss >> n2;
					stNum.pop();

					ss.clear();
					ss.str("");

					//目前都是二元操作符
					ss << handlerOperator(token, n1, n2);
					stNum.push(ss.str());
				}
				else
				{
					stNum.push(token);
				}
				pos++;
			}
			if (stNum.size() == 1)
			{
				std::stringstream ss;
				ss << stNum.top();
				ss >> res;
			}
			return res;
		}

		//表达式转后缀表达式
		static exprVec _expNifixToSuffix(exprVec& vecExpr)
		{
			/*
			中缀表达式->后缀表达式
			1.遇到操作数直接存储
			2.当栈为空时，遇到操作符直接压栈
			3.遇到左括号无条件入栈
			4.遇到右括号时，执行出栈操作，直到左括号为止
			5.遇到其他操作符时，弹出所有优先级大于当前操作符的栈中元素，最后将当前操作符入栈
			*/
			std::stack<std::string> stOps;
			exprVec exprSuffix;

			std::size_t pos = 0;
			while (pos != vecExpr.size())
			{
				const std::string& token = vecExpr[pos];
				if (isOperator(token))
				{
					if (isLeftBracket(token))
					{
						//左括号无条件入栈
						stOps.push(token);
					}
					else if (isRightBracket(token))
					{
						//右括号 弹出操作
						while (!stOps.empty() && !isLeftBracket(stOps.top()))
						{
							exprSuffix.push_back(stOps.top());
							stOps.pop();
						}
						stOps.pop();
					}
					else if (stOps.empty())
					{
						//栈中为空直接入栈
						stOps.push(token);
					}
					else
					{
						//操作符优先级比较
						while (!stOps.empty() && getOpsLevel(stOps.top()) >= getOpsLevel(token))
						{
							exprSuffix.push_back(stOps.top());
							stOps.pop();
						}
						stOps.push(token);
					}
				}
				else
				{
					exprSuffix.push_back(token);
				}

				pos++;
			}
			while (!stOps.empty())
			{
				exprSuffix.push_back(stOps.top());
				stOps.pop();
			}

			return exprSuffix;
		}
		void _expNifixToSuffix()
		{
			/*
			中缀表达式->后缀表达式
			1.遇到操作数直接存储
			2.当栈为空时，遇到操作符直接压栈
			3.遇到左括号无条件入栈
			4.遇到右括号时，执行出栈操作，直到左括号为止
			5.遇到其他操作符时，弹出所有优先级大于当前操作符的栈中元素，最后将当前操作符入栈
			*/
			std::stack<std::string> stOps;
			exprVec exprSuffix;

			std::size_t pos = 0;
			while (pos != m_vecExpr.size())
			{
				const std::string& token = m_vecExpr[pos];
				if (isOperator(token))
				{
					if (isLeftBracket(token))
					{
						//左括号无条件入栈
						stOps.push(token);
					}
					else if (isRightBracket(token))
					{
						//右括号 弹出操作
						while (!stOps.empty() && !isLeftBracket(stOps.top()))
						{
							m_vecExprSuffix.push_back(stOps.top());
							stOps.pop();
						}
						stOps.pop();
					}
					else if (stOps.empty())
					{
						//栈中为空直接入栈
						stOps.push(token);
					}
					else
					{
						//操作符优先级比较
						while (!stOps.empty() && getOpsLevel(stOps.top()) >= getOpsLevel(token))
						{
							m_vecExprSuffix.push_back(stOps.top());
							stOps.pop();
						}
						stOps.push(token);
					}
				}
				else
				{
					m_vecExprSuffix.push_back(token);
				}

				pos++;
			}
			while (!stOps.empty())
			{
				m_vecExprSuffix.push_back(stOps.top());
				stOps.pop();
			}
		}

	private:
		exprVec m_vecExpr;
		exprVec m_vecExprSuffix;
		//std::string m_strToken;
	};

	/*
	算分公式解析
	1.所有数字都用double计算
	2.采用后缀表达式形式
	*/
	template<class T>
	class Formula
	{
	public:
		Formula() {}
		~Formula() {}
	public:
		//
		T eval(exprVec& expr)
		{
			try
			{
				if (hasFuncAndSpecial(expr))
				{
					_analyzer(expr, 0);
				}

				return ExpressionParser(expr).getResult();
			}
			catch (...)
			{
				return std::numeric_limits<T>::quiet_NaN();
			}
		}
		T eval(std::string& strExpr)
		{
			try
			{
				exprVec vecExpr(lexer::exprParticiple(strExpr));
				return eval(vecExpr);
			}
			catch (...)
			{
				return std::numeric_limits<T>::quiet_NaN();
			}
		}


	private:
		//分析 把容器中的字符串变成普通的四则运算
		void _analyzer(exprVec& subExpr, std::size_t pos)
		{
			while (pos != subExpr.size())
			{
				const std::string& token = subExpr[pos];

				if (isSpecialWord(token))
				{
					//特殊值直接替换
					subExpr.erase(subExpr.begin() + pos);
					std::stringstream ss;
					ss << m_mapSpecialValue[token];
					subExpr.insert(subExpr.begin() + pos, ss.str());
				}
				else if (isFunc(token))
				{
					//处理函数
					std::size_t endPos = 0;

					std::string tmpRes(_funcHandler(subExpr, pos, endPos));
					subExpr.erase(subExpr.begin() + pos, subExpr.begin() + endPos + 1);
					subExpr.insert(subExpr.begin() + pos, tmpRes);
				}

				pos++;
			}
		}

		std::string _analyzer(exprVec& subExpr, std::size_t startPos, std::size_t endPos)
		{
			while (startPos != endPos)
			{
				const std::string& token = subExpr[startPos];

				if (isSpecialWord(token))
				{
					//特殊值直接替换
					subExpr.erase(subExpr.begin() + startPos);
					std::stringstream ss;
					ss << m_mapSpecialValue[token];
					subExpr.insert(subExpr.begin() + startPos, ss.str());
				}
				else if (isFunc(token))
				{
					//处理函数

					std::string tmpRes(_funcHandler(subExpr, startPos, endPos));
					subExpr.erase(subExpr.begin() + startPos, subExpr.begin() + endPos + 1);
					subExpr.insert(subExpr.begin() + startPos, tmpRes);

					break;
				}

				startPos++;
			}

			return subExpr[startPos];
		}

#ifndef expr_two_param_func_handler_declaration
#define expr_two_param_func_handler_declaration(funcName)															\
	std::string _##funcName##Handler(exprVec& subExpr, std::size_t startPos, std::size_t& endPos){					\
		startPos = startPos + 1;																					\
																													\
		std::size_t leftBracketCount = 0;																			\
		std::size_t rightBracketCount = 0;                                                                          \
																													\
		T n1 = 0.0f;																								\
		T n2 = 0.0f;																								\
																													\
		exprVec tmpSubExpr;                                                                                         \
																													\
		while (startPos != subExpr.size())                                                                          \
		{                                                                                                           \
		const std::string& token = subExpr[startPos];																\
		if (isFunc(token))																							\
		{																											\
		tmpSubExpr.push_back(_analyzer(subExpr, startPos, _searchExprEndPos(subExpr, startPos + 1)));				\
		if (isZero(n1))																								\
		{																											\
		n1 = eval(tmpSubExpr);																						\
		}																											\
				else                                                                                                \
		{																											\
		n2 = eval(tmpSubExpr);																						\
		}																											\
		tmpSubExpr.clear();																							\
		}																											\
			else if (isLeftBracket(token))                                                                          \
		{																											\
		leftBracketCount++;																							\
		}																											\
			else if (isRightBracket(token))                                                                         \
		{																											\
		rightBracketCount++;																						\
		}																											\
			else if (isComma(token))                                                                                \
		{																											\
																													\
		n1 = (isZero(n1) ? eval(tmpSubExpr) : n1);																	\
																													\
		tmpSubExpr.clear();                                                                                 \
		}                                                                                                       \
			else                                                                                                    \
		{                                                                                                       \
		tmpSubExpr.push_back(token);                                                                        \
		}                                                                                                       \
		\
		if (leftBracketCount                                                                                    \
		&& rightBracketCount                                                                                \
		&& leftBracketCount == rightBracketCount)                                                           \
		{                                                                                                       \
		endPos = startPos;                                                                                  \
		n2 = (isZero(n2) ? eval(tmpSubExpr) : n2);															\
		break;                                                                                              \
		}                                                                                                       \
		\
		startPos++;                                                                                             \
		}                                                                                                           \
		\
		std::stringstream ss;                                                                                       \
		ss << funcName##Impl(n1, n2);                                                                               \
		\
		return ss.str();                                                                                            \
	}


			expr_two_param_func_handler_declaration(min)

			expr_two_param_func_handler_declaration(max)

#endif

#ifndef expr_single_param_func_handler_declaration
#define expr_single_param_func_handler_declaration(funcName)															\
	std::string _##funcName##Handler(exprVec& subExpr, std::size_t startPos, std::size_t& endPos){						\
			startPos = startPos + 1;																					\
			\
			std::size_t leftBracketCount = 0;                                                                           \
			std::size_t rightBracketCount = 0;                                                                          \
			\
			T n = 0.0f;                                                                                                 \
			\
			exprVec tmpSubExpr;                                                                                         \
			\
		while (startPos != subExpr.size())                                                                          \
		{                                                                                                           \
		const std::string& token = subExpr[startPos];                                                           \
		if (isFunc(token))                                                                                      \
		{                                                                                                       \
		tmpSubExpr.push_back(_analyzer(subExpr, startPos, _searchExprEndPos(subExpr, startPos + 1)));       \
		}                                                                                                       \
			else if (isLeftBracket(token))                                                                          \
		{                                                                                                       \
		leftBracketCount++;                                                                                 \
		}                                                                                                       \
			else if (isRightBracket(token))                                                                         \
		{                                                                                                       \
		rightBracketCount++;                                                                                \
		}                                                                                                       \
			else                                                                                                    \
		{                                                                                                       \
		tmpSubExpr.push_back(token);                                                                        \
		}                                                                                                       \
		\
		if (leftBracketCount                                                                                    \
		&& rightBracketCount                                                                                \
		&& leftBracketCount == rightBracketCount)                                                           \
		{                                                                                                       \
		endPos = startPos;                                                                                  \
		n = (isZero(n) ? eval(tmpSubExpr) : n);																\
		break;                                                                                              \
		}                                                                                                       \
		\
		startPos++;                                                                                             \
		}                                                                                                           \
		\
		std::stringstream ss;                                                                                       \
		ss << funcName##Impl(n);                                                                                    \
		\
		return ss.str();                                                                                            \
	}

			expr_single_param_func_handler_declaration(ceil)

			expr_single_param_func_handler_declaration(floor)

			expr_single_param_func_handler_declaration(abs)

			expr_single_param_func_handler_declaration(sqrt)

#endif

#ifndef expr_single_param_func_caller
#define expr_single_param_func_caller(funcName)	\
	return _##funcName##Handler(subExpr, startPos, endPos)
#endif

#ifndef expr_two_param_func_caller
#define expr_two_param_func_caller(funcName)	\
	return _##funcName##Handler(subExpr, startPos, endPos)
#endif

			//函数处理
			std::string _funcHandler(exprVec& subExpr, const std::size_t& startPos, std::size_t& endPos)
			{
				const std::string& funcName = subExpr[startPos];

				if (funcName.compare("min") == 0)
				{
					expr_two_param_func_caller(min);
				}
				else if (funcName.compare("max") == 0)
				{
					expr_two_param_func_caller(max);
				}
				else if (funcName.compare("ceil") == 0)
				{
					expr_single_param_func_caller(ceil);
				}
				else if (funcName.compare("floor") == 0)
				{
					expr_single_param_func_caller(floor);
				}
				else if (funcName.compare("sqrt") == 0)
				{
					expr_single_param_func_caller(sqrt);
				}
				else if (funcName.compare("abs") == 0)
				{
					expr_single_param_func_caller(abs);
				}

				return "";
			}

		//搜寻函数终止下标
		std::size_t _searchExprEndPos(const exprVec& expr, std::size_t pos)
		{
			std::size_t leftBracketCount = 0;
			std::size_t rightBracketCount = 0;

			while (pos != expr.size())
			{
				const std::string& token = expr[pos];
				if (isLeftBracket(token))
				{
					leftBracketCount++;
				}
				else if (isRightBracket(token))
				{
					rightBracketCount++;
				}

				if (leftBracketCount
					&&rightBracketCount
					&&leftBracketCount == rightBracketCount)
				{
					break;
				}

				pos++;
			}

			return pos;
		}
	};
}

#endif