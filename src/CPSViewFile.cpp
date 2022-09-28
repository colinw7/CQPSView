#include <CPSViewI.h>

std::string PSViewFile::ascii85_chars_ =
"!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuz";

std::string PSViewFile::delim_chars_ = "()<>[]{}/%";

PSViewFile::
PSViewFile(CPSView *psview) :
 psview_(psview)
{
}

PSViewFile::
PSViewFile(const PSViewFile &file) :
 psview_(file.psview_)
{
}

PSViewFile::
~PSViewFile()
{
}

PSViewToken *
PSViewFile::
readToken()
{
  skipSpace();

  int c = lookChar();

  while (c == '%') {
    std::string str;

    str += char(readChar());

    c = lookChar();

    while (c != EOF && c != '\n' && c != '\f') {
      str += char(readChar());

      c = lookChar();
    }

    if (c == '\n' || c == '\f')
      readChar();

    if      (str == "%%DebugOn" )
      getPSView()->setDebug(true);
    else if (str == "%%DebugOff")
      getPSView()->setDebug(false);

    skipSpace();

    c = lookChar();
  }

  PSViewToken *token;

  token = readNumber();

  if (token == nullptr)
    token = readString();

  if (token == nullptr)
    token = readName();

  if (token == nullptr)
    token = readProcedure();

  if (token == nullptr) {
    if (isDelimiter()) {
      char delimiter_string[2];

      delimiter_string[0] = char(readChar());
      delimiter_string[1] = '\0';

      token = new PSViewNameToken(getPSView(), delimiter_string);
    }
  }

  if (getPSView()->getDebug()) {
    CStrUtil::printf("Read ");

    if (token != nullptr)
      token->print();
    else
      CStrUtil::printf("<null>");

    CStrUtil::printf("\n");
  }

  return token;
}

PSViewToken *
PSViewFile::
readNumber()
{
  uint save_buffer_pos;

  getPosition(&save_buffer_pos);

  int c = lookChar();

  bool sign1 = false;

  std::string str;

  if (c == '+' || c == '-') {
    str += char(readChar());

    sign1 = true;
  }

  bool digits1 = false;

  while (isDigit()) {
    str += char(readChar());

    digits1 = true;
  }

  c = lookChar();

  PSViewToken *token = nullptr;

  if (c == '.' || c == 'e' || c == 'E') {
    if (c == '.') {
      str += char(readChar());

      bool digits2 = false;

      while (isDigit()) {
        str += char(readChar());

        digits2 = true;
      }

      if (! digits1 && ! digits2)
        goto fail;

      c = lookChar();
    }

    if (c == 'e' || c == 'E') {
      str += char(readChar());

      c = lookChar();

      if (c == '+' || c == '-')
        str += char(readChar());

      if (! isDigit())
        goto fail;

      while (isDigit())
        str += char(readChar());
    }

    if (! isSeparator())
      goto fail;

    double real = CStrUtil::toReal(str);

    token = new PSViewRealToken(getPSView(), real);
  }
  else if (c == '#' && ! sign1 && digits1) {
    int integer;

    if (! CStrUtil::isInteger(str))
      goto fail;

    int base = int(CStrUtil::toInteger(str));

    if (base < 2 || base > 36)
      goto fail;

    c = char(readChar());

    if (! isBaseDigit(base))
      goto fail;

    str = "";

    while (isBaseDigit(base))
      str += char(readChar());

    if (! isSeparator())
      goto fail;

    if (! CStrUtil::isBaseInteger(str, uint(base)))
      goto fail;

    integer = int(CStrUtil::toBaseInteger(str, uint(base)));

    token = new PSViewIntegerToken(getPSView(), integer);
  }
  else if (digits1 && isSeparator()) {
    if (CStrUtil::isInteger(str)) {
      auto integer = CStrUtil::toInteger(str);

      token = new PSViewIntegerToken(getPSView(), integer);
    }
    else {
      double real = CStrUtil::toReal(str);

      token = new PSViewRealToken(getPSView(), real);
    }
  }
  else
    goto fail;

  return token;

 fail:
  setPosition(save_buffer_pos);

  return nullptr;
}

PSViewToken *
PSViewFile::
readString()
{
  uint save_buffer_pos;

  getPosition(&save_buffer_pos);

  int c = lookChar();

  if (c == '(') {
    readChar();

    PSVinteger num_brackets = 1;

    c = lookChar();

    std::string str;

    while (c != EOF && (c != ')' || num_brackets > 1)) {
      if      (c == '\\') {
        readChar();

        c = lookChar();

        if      (c == '\r' || c == '\n') {
          readChar();

          if (c == '\r') {
            c = lookChar();

            if (c == '\n')
              readChar();
          }
        }
        else if (c == 'n') {
          str += '\n';

          readChar();
        }
        else if (c == 'r') {
          str += '\r';

          readChar();
        }
        else if (c == 't') {
          str += '\t';

          readChar();
        }
        else if (c == 'b') {
          str += '\b';

          readChar();
        }
        else if (c == 'f') {
          str += '\f';

          readChar();
        }
        else if (c == '\\') {
          str += '\\';

          readChar();
        }
        else if (c == '(') {
          str += '(';

          readChar();
        }
        else if (c == ')') {
          str += ')';

          readChar();
        }
        else if (CStrUtil::isBaseChar(c, 8)) {
          int   octal_value;
          char  octal_string[4];

          octal_string[0] = char(readChar());

          c = lookChar();

          if (CStrUtil::isBaseChar(c, 8)) {
            octal_string[1] = char(readChar());

            c = lookChar();

            if (CStrUtil::isBaseChar(c, 8)) {
              octal_string[2] = char(readChar());

              octal_string[3] = '\0';
            }
            else
              octal_string[2] = '\0';
          }
          else
            octal_string[1] = '\0';

          octal_value = int(CStrUtil::toBaseInteger(octal_string, 8));

          str += char(octal_value);
        }
        else
          str += char(readChar());
      }
      else if (c == '(') {
        num_brackets++;

        str += char(readChar());
      }
      else if (c == ')') {
        num_brackets--;

        str += char(readChar());
      }
      else
        str += char(readChar());

      c = lookChar();
    }

    if (c == EOF)
      CStrUtil::eprintf("PSView: Unterminated String\n");

    readChar();

    auto *token = new PSViewStringToken(getPSView(), str);

    return token;
  }
  else if (c == '<') {
    readChar();

    c = lookChar();

    if (c == '~') {
      CStrUtil::eprintf("PSView: ASCII 85 Encoding not implemented\n");

      readChar();

      c = lookChar();

      while (c != EOF && isASCII85Char(c)) {
        readChar();

        c = lookChar();
      }

      if (c != '~') {
        getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_SYNTAX_ERROR);

        return nullptr;
      }

      readChar();

      c = lookChar();

      if (c != '>') {
        getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_SYNTAX_ERROR);

        return nullptr;
      }

      readChar();

      auto *token = new PSViewStringToken(getPSView(), 0L);

      return token;
    }
    else {
      char hex_string[3];

      c = lookChar();

      if (c == EOF || (! isxdigit(c) && ! isspace(c)))
        goto fail;

      int count = 0;

      std::string str;

      while (c != EOF && c != '>') {
        if (isspace(c)) {
          readChar();

          c = lookChar();

          continue;
        }

        if (! isxdigit(c)) {
          CStrUtil::eprintf("PSView: Invalid Hex String\n");

          goto fail;
        }

        hex_string[count++] = char(readChar());

        if (count == 2) {
          hex_string[count] = '\0';

          int hex_value = int(CStrUtil::toBaseInteger(hex_string, 16));

          str += char(hex_value);

          count = 0;
        }

        c = lookChar();
      }

      if (count == 1) {
        hex_string[count++] = '0';

        hex_string[count] = '\0';

        int hex_value = int(CStrUtil::toBaseInteger(hex_string, 16));

        str += char(hex_value);
      }

      if (c == EOF)
        CStrUtil::eprintf("PSView: Unterminated Hex String\n");

      readChar();

      auto *token = new PSViewStringToken(getPSView(), str);

      return token;
    }
  }

 fail:
  setPosition(save_buffer_pos);

  return nullptr;
}

PSViewToken *
PSViewFile::
readName()
{
  int c1 = lookChar();

  if (c1 == '<' || c1 == '>') {
    char name_string[3];

    readChar();

    int c2 = lookChar();

    if ((c1 == '<' && c2 == '<') || (c1 == '>' && c2 == '>')) {
      readChar();

      name_string[0] = char(c1);
      name_string[1] = char(c2);
      name_string[2] = '\0';
    }
    else {
      name_string[0] = char(c1);
      name_string[2] = '\0';
    }

    auto *token = new PSViewNameToken(getPSView(), name_string);

    return token;
  }

  PSVboolean literal  = false;
  PSVboolean evaluate = false;

  if (c1 == '/') {
    readChar();

    int c2 = lookChar();

    if (c2 == '/') {
      readChar();

      evaluate = true;
    }
    else
      literal = true;
  }

  std::string str;

  while (lookChar() != EOF && ! isSeparator())
    str += char(readChar());

  if (str.size() == 0)
    return nullptr;

  auto *token = new PSViewNameToken(getPSView(), str);

  if (evaluate) {
    PSViewToken *value = getPSView()->getDictionaryMgr()->lookup(token);

    if (value == nullptr) {
      getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_UNDEFINED);

      return nullptr;
    }

    return value;
  }
  else {
    if (literal)
      token->setLiteral();

    return token;
  }
}

PSViewToken *
PSViewFile::
readProcedure()
{
  PSViewArrayToken *procedure_token = nullptr;

  uint save_buffer_pos;

  getPosition(&save_buffer_pos);

  int c = lookChar();

  if (c != '{') {
    setPosition(save_buffer_pos);

    return nullptr;
  }

  readChar();

  getPSView()->getExecutionStack()->push(getPSView()->getMarkToken());

  PSViewToken *token = readToken();

  while (token != nullptr) {
    if (token->isName()) {
      auto *name_token = static_cast<PSViewNameToken *>(token);

      const PSViewName &name = name_token->getValue();

      if (name.compare(getPSView()->getTokenMgr()->getRBraceName()) == 0) {
        int num_tokens = getPSView()->getExecutionStack()->countToMark();

        procedure_token = new PSViewArrayToken(getPSView(), uint(num_tokens));

        for (int i = num_tokens; i >= 1; i--) {
          PSViewToken *sub_token = getPSView()->getExecutionStack()->pop();

          procedure_token->setValue(uint(i), sub_token);
        }

        procedure_token->setExecutable();

        getPSView()->getExecutionStack()->pop();

        break;
      }
      else
        getPSView()->getExecutionStack()->push(token);
    }
    else
      getPSView()->getExecutionStack()->push(token);

    token = readToken();
  }

  return procedure_token;
}

void
PSViewFile::
skipComment()
{
  int c = lookChar();

  while (c != EOF && c != '\n' && c != '\f') {
    readChar();

    c = lookChar();
  }

  if (c == '\n' || c == '\f')
    readChar();
}

void
PSViewFile::
skipSpace()
{
  int c = lookChar();

  while (c != EOF && isspace(c)) {
    readChar();

    c = lookChar();
  }
}

PSVboolean
PSViewFile::
isBaseDigit(PSVinteger base)
{
  int c = lookChar();

  return CStrUtil::isBaseChar(char(c), uint(base));
}

PSVboolean
PSViewFile::
isDigit()
{
  int c = lookChar();

  return isdigit(c);
}

PSVboolean
PSViewFile::
isSeparator()
{
  int c = lookChar();

  if (c == EOF || c == '%' || isspace(c))
    return true;

  return isDelimiter();
}

PSVboolean
PSViewFile::
isDelimiter()
{
  int c = lookChar();

  return (delim_chars_.find(char(c)) != std::string::npos);
}

// TODO: Move to CStrUtil
std::string
PSViewFile::
charsToASCII85(uint chars)
{
  if (chars == 0)
    return "z";

  int c[5];

  for (uint i = 0; i < 5; i++) {
    int temp = chars/85;

    c[i] = int(chars) - temp*85;

    chars = uint(temp);
  }

  std::string str;

  for (uint i = 0; i < 5; i++)
    str += char(ascii85_chars_[uint(c[4 - i])]);

  return str;
}

PSVboolean
PSViewFile::
isASCII85Char(int c)
{
  return (ascii85_chars_.find(char(c)) != std::string::npos);
}
