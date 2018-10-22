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
��������ʱ��
0.���俪ʼʱ���ȡ�����еĹ�ʽ
1.��ȡ��Ϻ����зִʴ洢
�ƾֽ���ʱ��
1.����ʱ����ݹ�����װ��ʽ
2.��װ�Ĺ�ʽΪ��׺���ʽ
->	(1) һ������ͨ�Ĵ�����Ĺ�ʽ
(2) ����ϸ˵���ļ��㹫ʽ ���磺(��һɫ2+������2)*�׷�0.5+��ϲ3*10+Сϲ4*20
3.(1)ת��׺���ʽ
4.����ֵ�ó����
*/

namespace EXPRESSION
{
	typedef std::vector<std::string> exprVec;

	//-----------------------------------------------------------------------------
	//������
	static const std::string operatorList[] =
	{
		"+", "-", "*", "/", "%", "(", ")", ","
	};
	static const std::size_t operatorListSize = sizeof(operatorList) / sizeof(char);

	//-----------------------------------------------------------------------------
	//���ܺ���
	static const std::string funcList[] =
	{
		//�����С Ŀǰֻ֧��2����
		//���� ����ȡ��
		//����ֵ ƽ��ֵ ������
		"min", "max", "ceil", "floor", "sqrt", "abs", "bsd"
	};
	static const std::size_t funcListSize = sizeof(funcList) / sizeof(std::string);

	//-----------------------------------------------------------------------------
	//һЩ�����⺬���ֵ
	static const std::string specialList[] =
	{
		"limit", "base", "player"
	};
	static const std::size_t specialListSize = sizeof(specialList) / sizeof(std::string);

	//-----------------------------------------------------------------------------
	//���������ȼ�
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
	//�ж���Ч�ַ�
	inline bool isValid(const char c)
	{
		return isspace(c) || iscntrl(c);
	}

	//-----------------------------------------------------------------------------
	//�жϲ�����
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
	//�жϲ�����
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
	//�жϺ���
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
	//�ж����������
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
	//�ж�����
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
	//�ж���ĸ
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
	//�ж�������
	inline bool isLeftBracket(const char c)
	{
		return (c == '(');
	}
	//-----------------------------------------------------------------------------
	//�ж�������
	inline bool isLeftBracket(const std::string& ops)
	{
		return ops.compare("(") == 0;
	}

	//-----------------------------------------------------------------------------
	//�ж�������
	inline bool isRightBracket(const char c)
	{
		return (c == ')');
	}
	//-----------------------------------------------------------------------------
	//�ж�������
	inline bool isRightBracket(const std::string& ops)
	{
		return ops.compare(")") == 0;
	}

	//-----------------------------------------------------------------------------
	//�ж϶���
	inline bool isComma(const std::string& ops)
	{
		return ops.compare(",") == 0;
	}

	//-----------------------------------------------------------------------------
	//�жϺ���������ֵ
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
	//�ַ���תСд
	void caseNormaliseToLower(std::string& str)
	{
		std::transform(str.begin(), str.end(), str.begin(), tolower);
	}

	//-----------------------------------------------------------------------------
	//�ַ���ת��д
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

	//���ú���
	//������
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
	//�����
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
		//�ִ�
		static exprVec exprParticiple(std::string& exprStr)
		{
			caseNormaliseToLower(exprStr);

			exprVec vecExpr;
			std::string strToken;
			/*
			������Ч�ַ�
			�����Ƿ��žʹ洢
			��������������֮ǰ�洢���γ�һ��token
			�洢token,�洢������
			*/
			std::size_t pos = 0;
			for (; pos != exprStr.size(); ++pos)
			{
				const char& c = exprStr[pos];
				//��Ч�ַ�����¼
				if (isValid(c))continue;
				//�ǲ�����ֱ�Ӵ洢
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

	//���ʽ����
	/*
	1.�������ֶ���double����
	2.���ú�׺���ʽ��ʽ
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
		//��ֵ
		T _getResult()
		{
			double res = 0.0f;

			//������
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
				��׺���ʽ��ֵ
				1.��������ȫ����ջ
				2.����������ȡջ�����������м���
				3.ջ��Ψһ�����������ս�
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

					//Ŀǰ���Ƕ�Ԫ������
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

		//���ʽת��׺���ʽ
		static exprVec _expNifixToSuffix(exprVec& vecExpr)
		{
			/*
			��׺���ʽ->��׺���ʽ
			1.����������ֱ�Ӵ洢
			2.��ջΪ��ʱ������������ֱ��ѹջ
			3.������������������ջ
			4.����������ʱ��ִ�г�ջ������ֱ��������Ϊֹ
			5.��������������ʱ�������������ȼ����ڵ�ǰ��������ջ��Ԫ�أ���󽫵�ǰ��������ջ
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
						//��������������ջ
						stOps.push(token);
					}
					else if (isRightBracket(token))
					{
						//������ ��������
						while (!stOps.empty() && !isLeftBracket(stOps.top()))
						{
							exprSuffix.push_back(stOps.top());
							stOps.pop();
						}
						stOps.pop();
					}
					else if (stOps.empty())
					{
						//ջ��Ϊ��ֱ����ջ
						stOps.push(token);
					}
					else
					{
						//���������ȼ��Ƚ�
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
			��׺���ʽ->��׺���ʽ
			1.����������ֱ�Ӵ洢
			2.��ջΪ��ʱ������������ֱ��ѹջ
			3.������������������ջ
			4.����������ʱ��ִ�г�ջ������ֱ��������Ϊֹ
			5.��������������ʱ�������������ȼ����ڵ�ǰ��������ջ��Ԫ�أ���󽫵�ǰ��������ջ
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
						//��������������ջ
						stOps.push(token);
					}
					else if (isRightBracket(token))
					{
						//������ ��������
						while (!stOps.empty() && !isLeftBracket(stOps.top()))
						{
							m_vecExprSuffix.push_back(stOps.top());
							stOps.pop();
						}
						stOps.pop();
					}
					else if (stOps.empty())
					{
						//ջ��Ϊ��ֱ����ջ
						stOps.push(token);
					}
					else
					{
						//���������ȼ��Ƚ�
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
	��ֹ�ʽ����
	1.�������ֶ���double����
	2.���ú�׺���ʽ��ʽ
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
		//���� �������е��ַ��������ͨ����������
		void _analyzer(exprVec& subExpr, std::size_t pos)
		{
			while (pos != subExpr.size())
			{
				const std::string& token = subExpr[pos];

				if (isSpecialWord(token))
				{
					//����ֱֵ���滻
					subExpr.erase(subExpr.begin() + pos);
					std::stringstream ss;
					ss << m_mapSpecialValue[token];
					subExpr.insert(subExpr.begin() + pos, ss.str());
				}
				else if (isFunc(token))
				{
					//������
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
					//����ֱֵ���滻
					subExpr.erase(subExpr.begin() + startPos);
					std::stringstream ss;
					ss << m_mapSpecialValue[token];
					subExpr.insert(subExpr.begin() + startPos, ss.str());
				}
				else if (isFunc(token))
				{
					//������

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

			//��������
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

		//��Ѱ������ֹ�±�
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