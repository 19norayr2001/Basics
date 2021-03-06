#include "Prefix.h"
#include <iostream>
#include <stack>
Prefix::Prefix()
{}

Prefix::Prefix(const std::string& s)
	:Expression(s)
{
	SwapBrackets();
	std::reverse(m_infix.begin(), m_infix.end());
	ToPostfix(m_prefix, false);
	std::reverse(m_infix.begin(), m_infix.end());
	SwapBrackets();
	std::reverse(m_prefix.begin(), m_prefix.end());
}

Prefix::Prefix(const Prefix& obj)
{
	Allocate(obj);
}

Prefix& Prefix::operator=(const Prefix& obj)
{
	if (this != &obj)
	{
		Deallocate();
		Allocate(obj);
	}
	return *this;
}

Prefix::~Prefix()
{
	Deallocate();
}

int Prefix::Calculate(std::unordered_map<std::string, int>& mp) const
{
	int sz = m_prefix.size();
	std::stack<int> st;
	for (int i = sz - 1; i >= 0; --i)
	{
		if (dynamic_cast<Number*>(m_prefix[i]) != nullptr)
		{
			st.push(StringToInteger(m_prefix[i]->info));
		}
		else if (dynamic_cast<Identificator*>(m_prefix[i]) != nullptr)
		{
			st.push(mp[m_prefix[i]->info]);
		}
		else
		{
			int num1 = st.top(); st.pop();
			int num2 = st.top(); st.pop();
			st.push(CalculateOption(num1, num2, m_prefix[i]->info[0]));
		}
	}
	return st.top();
}

void Prefix::Allocate(const Prefix& obj)
{
	AllocateVector(m_infix, obj.m_infix);
	AllocateVector(m_prefix, obj.m_prefix);
}

void Prefix::Deallocate()
{
	DeallocateVector(m_infix);
	m_prefix.clear();
}

void Prefix::SwapBrackets()
{
	int sz = m_infix.size();
	for (int i = 0; i < sz; ++i)
	{
		if (dynamic_cast<OpenBracket*>(m_infix[i]) != nullptr)
		{
			delete m_infix[i];
			m_infix[i] = new CloseBracket(")");
		}
		else if (dynamic_cast<CloseBracket*>(m_infix[i]) != nullptr)
		{
			delete m_infix[i];
			m_infix[i] = new OpenBracket("(");
		}
	}
}

std::ostream& operator<<(std::ostream& out, const Prefix& obj)
{
	for (auto smb : obj.m_prefix)
	{
		out << smb->info << " ";
	}
	out << std::endl;
	return out;
}