#ifndef CPSViewFileToken_H
#define CPSViewFileToken_H

class PSViewFileToken : public PSViewToken {
 protected:
  PSViewFileToken(CPSView *psview);

  PSViewFileToken(const PSViewFileToken &file_token);

  PSViewFileToken &operator=(const PSViewFileToken &file_token);

 public:
  //----

  // virtual functions

  virtual ~PSViewFileToken();

  virtual PSViewFileToken *dup() const = 0;

  virtual int compare(PSViewToken *token);

  virtual void executeToken() = 0;

  virtual const PSViewName &getName() = 0;

  virtual void print() = 0;

  virtual std::string toString() = 0;

  //----

  PSVboolean initPostScript();

  virtual void          flush() = 0;
  virtual PSViewToken  *readToken() = 0;
  virtual bool          isValid() const = 0;
  virtual uint          bytesAvailable() = 0;
  virtual PSVboolean    setPosition(uint pos) = 0;
  virtual PSVboolean    getPosition(uint *pos) = 0;
  virtual std::string   getFileName() = 0;
  virtual int           readChar() = 0;
  virtual PSVboolean    writeChar(PSVchar c) = 0;
  virtual void          reset() = 0;
  virtual void          close() = 0;

  PSViewToken *readString();

  PSViewFile *getFile() { return file_; }

 protected:
  PSViewFile *file_ { nullptr };
};

#endif
