/**
* LICENSE PLACEHOLDER
*
* @file simple_math_evaluator.h
* @class SimpleMathEvaluator
* @package openpst/libopenpst
* @brief Parses simple string math expressions. 
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#include "util/simple_math_evaluator.h"
#include <iostream>
#include <sstream>

using namespace OpenPST;

SimpleMathEvaluator::SimpleMathEvaluator()
{

}

SimpleMathEvaluator::~SimpleMathEvaluator()
{

}

std::string SimpleMathEvaluator::evaluate(const std::string& expr)
{   
    // https://en.wikipedia.org/wiki/Shunting-yard_algorithm
    auto tokens = tokenize(expr);

    // start fresh
    outstack.empty();
    opstack.empty();

    for (auto &token : tokens) {
    	if (token.type == kSimpleMathEvaluatorTokenTypeValue) {
    		pushOut(std::stoul(token.value.c_str(), nullptr, 10));
    		continue;
    	} else if (token.type == kSimpleMathEvaluatorTokenTypeOperator) {
    		if (token.value[0] == '(') {
    			pushOp(getOperator(token));
    			continue;
    		} else if (token.value[0] == ')') {
    			// get the current operator   			

    			while(true) {
    				if (!opstack.size()) {
    					throw std::invalid_argument("Fucked up");
    				}

    				auto op = opstack.top();

    				if (op->op == '(') {
    					break;
    				} else if (op->op == ')') {
    					throw std::invalid_argument("Unexpected an operator");
    				}

    				pushOut(op->evaluate(popOut(), popOut()));
    			}
				

    		} else if (opstack.size()) {
    			// check precedence
    			auto curOperator = getOperator(token);
    			auto topOperator = opstack.top();
            	
            	if ((curOperator->associativity == kSimpleMathEvaluatorAssociativityLeft && curOperator->precedence <= topOperator->precedence) ||
            		 (curOperator->associativity == kSimpleMathEvaluatorAssociativityRight && curOperator->precedence < topOperator->precedence)
            	) {
					// evaluate 
					//pushOut(op->evaluate(popOut(), popOut()));
            	} else {
            		pushOp(curOperator);
            	}
    		} else {
    			pushOp(getOperator(token));
    		}
    	}
    }

    // parse the string, token by token
    /*while (cur != end) {
        SimpleMathEvaluatorToken token = readNextToken(cur, end);
        
        if (token.value.size()) {
            if (token.type == kSimpleMathEvaluatorTokenTypeOperator && token.value[0] != '(' && token.value[0] != ')') {
                if (op.size()) {
                    auto top = op.top();
                    if (top.type == kSimpleMathEvaluatorTokenTypeOperator && top.value[0] != '(' && top.value[0] != ')') {
                    	// check precedence 
                    	auto curOperator = getOperator(token);
                    	auto topOperator = getOperator(top);

                    	if (curOperator->associativity == kSimpleMathEvaluatorAssociativityLeft && curOperator->precedence <= topOperator->precedence) {
							outstack.push(top);
                            op.pop();
                    	} else if (curOperator->associativity == kSimpleMathEvaluatorAssociativityRight && curOperator->precedence < topOperator->precedence) {
							outstack.push(top);
                            op.pop();
                    	}
                    }
                }
                op.push(token);
            } else if(token.type == kSimpleMathEvaluatorTokenTypeOperator && token.value[0] == '(') {
                op.push(token);
            } else if(token.type == kSimpleMathEvaluatorTokenTypeOperator && token.value[0] == ')') {
         
                while (true) {
                	auto top = op.top();
                	if(top.type == kSimpleMathEvaluatorTokenTypeOperator && top.value[0] == '(') {                		
                		break;
                	} else if(top.type == kSimpleMathEvaluatorTokenTypeOperator && top.value[0] == ')') {
                		throw std::invalid_argument("Bad Expression");
                	}

                	outstack.push(top);
                	op.pop();

                    if (!op.size()) {
                        throw std::invalid_argument("Bad Expression");
                    }
                }

                op.pop();
            } else {
                outstack.push(token);
            }

        } else {
            throw std::invalid_argument("Empty Token");
        }
    }

    // pop op stack and push into outstack queue
    while(op.size()) {
        outstack.push(op.top());
        op.pop();
    }

    while(outstack.size()) {
    	auto t  = outstack.front();
    	std::coutstack << "Token: " << t.value << " Type: " << t.type << std::endl;
    	outstack.pop();
    }
    std::coutstack << std::endl;
    return std::to_string(0);

    /*
    float total = 0;

    while(outstack.size()) {
        float l;
        float r;

        auto left  = outstack.front();
        
        outstack.pop(); 
        
        if (left.type == kSimpleMathEvaluatorTokenTypeInt) {            
            l = std::stof(left.value.c_str(), nullptr);
        } else if (left.type == kSimpleMathEvaluatorTokenTypeOperator) {
        	std::stringstream ss;
        	ss << "Invalid Left Expression for token " << left.value << std::endl;
            throw std::invalid_argument(ss.str());
        }

        auto right = outstack.front();
        
        outstack.pop(); 

        if (right.type == kSimpleMathEvaluatorTokenTypeInt) {
            r = std::stof(right.value.c_str(), nullptr);
        } else if (right.type == kSimpleMathEvaluatorTokenTypeOperator) {
            r = l;
            l = total;
        }

        SimpleMathEvaluatorToken op;

        if (right.type == kSimpleMathEvaluatorTokenTypeOperator){
            op = right;
        } else {
             
            op = outstack.front();   
            outstack.pop();   
        }
    
        if (op.type != kSimpleMathEvaluatorTokenTypeOperator) {
        	std::stringstream ss;
        	ss << "Invalid Expression for token " << op.value << std::endl;
            throw std::invalid_argument(ss.str());
        }
        
        switch(getOperatorType(op.value[0])) {
            case kSimpleMathEvaluatorOperatorTypeAdd:
                total = l + r;
                break;
            case kSimpleMathEvaluatorOperatorTypeSubtract:
                total = l - r;
                break;
            case kSimpleMathEvaluatorOperatorTypeMultiply:
                total = l * r;
                break;
            case kSimpleMathEvaluatorOperatorTypeDivide:
                total = l / r;
                break;
            case kSimpleMathEvaluatorOperatorTypePow:
            	total = pow(l, r);
            	break;
        }
    }
    
    return std::to_string(total);  */
}


SimpleMathEvaluatorToken SimpleMathEvaluator::readNextToken(std::string::iterator& cur, std::string::iterator& end)
{
    SimpleMathEvaluatorToken ret;

    while(cur != end) {
        char v = *cur;
        if (isOperator(v)) {
            if (ret.value.size()) {
                break;
            }
            ret.type   = kSimpleMathEvaluatorTokenTypeOperator;
            ret.value.append(&v, sizeof(v));
            cur++;
            break;        
        } else {
            ret.type = kSimpleMathEvaluatorTokenTypeValue;
        }

        if (!std::isspace(v)) {
            ret.value.append(&v, sizeof(v));
        } 
        cur++;
    }    

    return ret;
}

std::vector<SimpleMathEvaluatorToken> SimpleMathEvaluator::tokenize(const std::string& expr)
{
	std::string e = expr;
	std::vector<SimpleMathEvaluatorToken>  ret;

	std::string::iterator cur = e.begin();
    std::string::iterator end = e.end();

    // parse the string, token by token
    while (cur != end) {
    	auto token = readNextToken(cur, end);
        
        if (!token.value.size()) {
    		throw std::invalid_argument("Empty Token");
    	}

    	ret.push_back(token);
    }

    return ret;
}

bool SimpleMathEvaluator::isOperator(char v)
{   
    for (int i = 0; i < sizeof(operators); i++) {
    	if (v == operators[i].op) {
    		return true;
    	}
    }
    return false;
}

SimpleMathEvaluatorOp* SimpleMathEvaluator::getOperator(const SimpleMathEvaluatorToken& token)
{
	if (!token.value.size() || !isOperator(token.value[0])) {
		throw std::invalid_argument("Token is not an operator");
	}

    for (int i = 0; i < sizeof(operators); i++) {
    	if (token.value[0] == operators[i].op) {
    		return &operators[i];
    	}
    }

    throw std::invalid_argument("Operator not found");
}