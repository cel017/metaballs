#include "graph.h"


std::unordered_map<std::string, int> const OPS_PRIORITY(
{
	{"^", 4},
	{"*", 3},
	{"/", 2},
	{"+", 1},
	{"-", 0}
});

float calculate(float a, float b, std::string op)
{
	if (op == "^")
		return pow(a, b);
	else if (op =="*")
		return a*b;
	else if (op == "/")
		return a/b;
	else if (op == "+")
		return a+b;
	else if (op == "-")
		return a-b;
	else
		return 0;
}

// operator wrapper function //
float add(float a, float b)
{
	return a+b;
}

float sub(float a, float b)
{
	return a-b;
}

float mul(float a, float b)
{
	return a*b;
}

float div(float a, float b)
{
	return a/b;
}

Equation::Equation(std::string equation)
{
    bool is_digit = false;
    bool found_token = false;
    std::string token = "";
    std::stack<std::string> ops;

    // get implicit eqn
    for (int i = 0; i<(int)equation.length(); i++)
    {
    	if (equation[i] == '=')
    	{
    		imp_exp = equation.substr(0, i)+"-("+equation.substr(i+1, equation.length()-i-1)+")";
    	}
    }

    for (int i = 0; i<(int)imp_exp.length(); i++)
    {
    	// get token
    	if (token=="")
    	{
    		if (imp_exp[i] == 'x' || imp_exp[i] == 'y')
    		{
    			token += imp_exp[i];
    			found_token = true;
    			is_digit = true;
    		}
    		else if (isdigit(imp_exp[i]))
    		{
    			is_digit = true;
    			token += imp_exp[i];
    		}
    		else
    		{
    			token = imp_exp[i];
    			found_token = true;
    		}
    	}
    	else if (token!="" && is_digit)
    	{
    		if (isdigit(imp_exp[i]))
    		{
    			token += imp_exp[i];
    			if (i == (int)imp_exp.length()-1)
    			{	// no more chars left to parse
    				found_token = true;
    			}
    		}
    		else  // num found
    		{
    			// set index back so the next token
    			// (operator) does not get ignored
    			i--;
    			found_token = true;
    		}
    	}

		// put token in stack/queue
    	if (found_token)
    	{
    		if (is_digit)
    		{
    			rev_pol.push(token);
    			// reset is_digit after token ahs been processed
		    	is_digit = false;
		    }
    		else if (token == "(")
    			ops.push(token);
    		else if (token == ")")
    		{
    			while (ops.top() != "(")
    			{
    				// push operator to reverse polish queue
    				rev_pol.push(ops.top());
    				// pop operator from stack
    				ops.pop();
    			}
    			ops.pop();  // pop "("
    		}
    		else	// all other operators
    		{
    			if (ops.empty() || 
    				ops.top() == "(" || 
    				OPS_PRIORITY.at(token) > OPS_PRIORITY.at(ops.top()))
    			{
    				// higher priority gets pushed automatically to the top
    				ops.push(token);
    			}
    			else
    			{
    				while (OPS_PRIORITY.at(token) < OPS_PRIORITY.at(ops.top()))
    				{
    					// push operator to queue
    					rev_pol.push(ops.top());
    					ops.pop();
    				}
    				ops.push(token);
    			}

    		}

    		// reset token
   			token = "";
   			found_token = false;
    	}
    }

    // parse stack
    while (!ops.empty())
    {
    	rev_pol.push(ops.top());
    	ops.pop();
    }

    this->equation = equation;
}
