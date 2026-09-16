#include "lexer.hpp"

#include <string>
#include <vector>

void push_token(Token &token, string &current_value, vector<Token> &output)
{
    token.value = current_value;
    current_value.clear();
    output.push_back(token);

    token.type = TokenType::WORD;
    token.value = "";
}

using namespace std;
vector<Token> tokenize(const string &cmd)
{
    vector<Token> out;
    Token curr_token;
    curr_token.type = TokenType::WORD;
    string cur = "";
    State state = State::Plain;

    for (int i = 0; i < cmd.length(); i++)
    {
        char ch = cmd[i];
        char next_ch = (i + 1 < cmd.length()) ? cmd[i + 1] : '\0';
        switch (state)
        {
        case State::Plain:
            if (isspace(ch))
            {
                if (!cur.empty())
                {
                    push_token(curr_token, cur, out);
                }
            }
            else if (ch == '\'')
            {
                state = State::InSingleQuote;
            }
            else if (ch == '\"')
            {
                state = State::InDoubleQuote;
            }
            else if (ch == '\\')
            {
                state = State::EscapedOut;
            }
            else if (ch == '>' || (ch == '2' && next_ch == '>'))
            {
                state = State::Redirect;
                i--;
            }
            else
            {
                cur.push_back(ch);
            }

            break;

        case State::InSingleQuote:
            if (ch == '\'')
                state = State::Plain;
            else
                cur.push_back(ch);
            break;

        case State::InDoubleQuote:
            if (ch == '\"')
            {
                state = State::Plain;
            }
            else if (ch == '\\')
            {
                state = State::EscapedInDoubleQuote;
            }
            else
            {
                cur.push_back(ch);
            }
            break;

        case State::EscapedOut:
            cur.push_back(ch);
            state = State::Plain;
            break;

        case State::EscapedInDoubleQuote:
            if (ch == '\"' || ch == '\\')
                cur.push_back(ch);
            else
                cur.append({'\\', ch});
            state = State::InDoubleQuote;
            break;

        case State::Redirect:

            if (ch == '2' && next_ch == '>')
            {
                if (i + 2 < cmd.length() && cmd[i + 2] == '>')
                {
                    curr_token.type = TokenType::STD_ERR_APPEND;
                    i += 2;
                }
                else
                {

                    curr_token.type = TokenType::STD_ERR;
                    i++;
                }
            }

            else if (ch == '>' && next_ch == '>')
            {
                curr_token.type = TokenType::STD_OUT_APPEND;
                i++;
            }
            else if (ch == '>')
            {
                curr_token.type = TokenType::STD_OUT;
            }

            state = State::Collect;

            break;

        case State::Collect:

            if (isspace(ch))
            {
                if (!cur.empty())
                {
                    push_token(curr_token, cur, out);
                }

                state = State::Plain;
            }
            else
            {
                cur.push_back(ch);
            }

            break;

        default:
            break;
        }
    }
    if (!cur.empty())
    {
        push_token(curr_token, cur, out);
    }
    curr_token.type = TokenType::EOL;
    push_token(curr_token, cur, out);

    return out;
}
