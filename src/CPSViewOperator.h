#ifndef PSVIEW_OPERATOR_I_H
#define PSVIEW_OPERATOR_I_H

class PSViewOperatorMgr;

class PSViewOperator {
 private:
  PSViewOperatorMgr  *mgr_;
  const PSViewName   &name_;
  PSViewOperatorProc  proc_;

 public:
  PSViewOperator(PSViewOperatorMgr *mgr, const std::string &name, PSViewOperatorProc proc);

  int compare(PSViewOperator *opr);

  void invoke();

  const PSViewName   &getName  () const { return name_; }
  PSViewOperatorProc  getProc  () const { return proc_; }
  const std::string  &getString() const { return name_.getString(); }
};

class PSViewOperatorMgr {
 private:
  CPSView                       *psview_;
  PSVinteger                     random_seed_;
  std::vector<PSViewOperator *>  system_operators_;
  std::vector<PSViewOperator *>  private_operators_;
  uint                           num_private_operators_;

 public:
  PSViewOperatorMgr(CPSView *psview);
 ~PSViewOperatorMgr();

  CPSView *getPSView() const { return psview_; }

  PSVinteger getRandomSeed() const { return random_seed_; }

  void setRandomSeed(PSVinteger seed) { random_seed_ = seed; }

  void init();

  PSViewOperator *lookup(const std::string &name);

  void addSystemOperators(PSViewDictionaryToken *dictionary);

  static void startArrayOp(PSViewOperatorMgr *mgr);

  static void endArrayOp(PSViewOperatorMgr *mgr);
  void        endArrayOp();

  static void startDictionaryOp(PSViewOperatorMgr *mgr);

  static void endDictionaryOp(PSViewOperatorMgr *mgr);
  void        endDictionaryOp();

  static void print1Op(PSViewOperatorMgr *mgr);
  static void print2Op(PSViewOperatorMgr *mgr);
  static void absOp(PSViewOperatorMgr *mgr);
  static void addOp(PSViewOperatorMgr *mgr);
  static void aloadOp(PSViewOperatorMgr *mgr);
  static void anchorSearchOp(PSViewOperatorMgr *mgr);
  static void andOp(PSViewOperatorMgr *mgr);
  static void arcOp(PSViewOperatorMgr *mgr);
  static void arcNOp(PSViewOperatorMgr *mgr);
  static void arcTOp(PSViewOperatorMgr *mgr);
  static void arcToOp(PSViewOperatorMgr *mgr);
  static void arrayOp(PSViewOperatorMgr *mgr);
  static void ashowOp(PSViewOperatorMgr *mgr);
  static void astoreOp(PSViewOperatorMgr *mgr);
  static void atanOp(PSViewOperatorMgr *mgr);
  static void awidthShowOp(PSViewOperatorMgr *mgr);
  static void beginOp(PSViewOperatorMgr *mgr);
  static void bindOp(PSViewOperatorMgr *mgr);
  static void bitShiftOp(PSViewOperatorMgr *mgr);
  static void bytesAvailableOp(PSViewOperatorMgr *mgr);
  static void ceilingOp(PSViewOperatorMgr *mgr);
  static void charPathOp(PSViewOperatorMgr *mgr);
  static void clearOp(PSViewOperatorMgr *mgr);
  static void clearDictStackOp(PSViewOperatorMgr *mgr);
  static void clearToMarkOp(PSViewOperatorMgr *mgr);
  static void clipOp(PSViewOperatorMgr *mgr);
  static void clipPathOp(PSViewOperatorMgr *mgr);
  static void closeFileOp(PSViewOperatorMgr *mgr);
  static void closePathOp(PSViewOperatorMgr *mgr);
  static void concatOp(PSViewOperatorMgr *mgr);
  static void concatMatrixOp(PSViewOperatorMgr *mgr);
  static void copyOp(PSViewOperatorMgr *mgr);
  static void cosOp(PSViewOperatorMgr *mgr);
  static void countOp(PSViewOperatorMgr *mgr);
  static void countDictStackOp(PSViewOperatorMgr *mgr);
  static void countExecStackOp(PSViewOperatorMgr *mgr);
  static void countToMarkOp(PSViewOperatorMgr *mgr);
  static void currentBlackGenerationOp(PSViewOperatorMgr *mgr);
  static void currentCMYKColorOp(PSViewOperatorMgr *mgr);
  static void currentColorOp(PSViewOperatorMgr *mgr);
  static void currentColorSpaceOp(PSViewOperatorMgr *mgr);
  static void currentDashOp(PSViewOperatorMgr *mgr);
  static void currentDictOp(PSViewOperatorMgr *mgr);
  static void currentFileOp(PSViewOperatorMgr *mgr);
  static void currentFlatOp(PSViewOperatorMgr *mgr);
  static void currentFontOp(PSViewOperatorMgr *mgr);
  static void currentGlobalOp(PSViewOperatorMgr *mgr);
  static void currentGrayOp(PSViewOperatorMgr *mgr);
  static void currentGStateOp(PSViewOperatorMgr *mgr);
  static void currentHSBColorOp(PSViewOperatorMgr *mgr);
  static void currentLineCapOp(PSViewOperatorMgr *mgr);
  static void currentLineJoinOp(PSViewOperatorMgr *mgr);
  static void currentLineWidthOp(PSViewOperatorMgr *mgr);
  static void currentMatrixOp(PSViewOperatorMgr *mgr);
  static void currentMitreLimitOp(PSViewOperatorMgr *mgr);
  static void currentPackingOp(PSViewOperatorMgr *mgr);
  static void currentPointOp(PSViewOperatorMgr *mgr);
  static void currentRGBColorOp(PSViewOperatorMgr *mgr);
  static void currentStrokeAdjustOp(PSViewOperatorMgr *mgr);
  static void currentUnderColorRemovalOp(PSViewOperatorMgr *mgr);
  static void curveToOp(PSViewOperatorMgr *mgr);
  static void cviOp(PSViewOperatorMgr *mgr);
  static void cvlitOp(PSViewOperatorMgr *mgr);
  static void cvnOp(PSViewOperatorMgr *mgr);
  static void cvrOp(PSViewOperatorMgr *mgr);
  static void cvrsOp(PSViewOperatorMgr *mgr);
  static void cvsOp(PSViewOperatorMgr *mgr);
  static void cvxOp(PSViewOperatorMgr *mgr);
  static void defOp(PSViewOperatorMgr *mgr);
  static void defaultMatrixOp(PSViewOperatorMgr *mgr);
  static void defineFontOp(PSViewOperatorMgr *mgr);
  static void defineResourceOp(PSViewOperatorMgr *mgr);
  static void defineUserObjectOp(PSViewOperatorMgr *mgr);
  static void deleteFileOp(PSViewOperatorMgr *mgr);
  static void dictOp(PSViewOperatorMgr *mgr);
  static void dictStackOp(PSViewOperatorMgr *mgr);
  static void divOp(PSViewOperatorMgr *mgr);
  static void dtransformOp(PSViewOperatorMgr *mgr);
  static void dupOp(PSViewOperatorMgr *mgr);
  static void echoOp(PSViewOperatorMgr *mgr);
  static void eexecOp(PSViewOperatorMgr *mgr);
  static void endOp(PSViewOperatorMgr *mgr);
  static void eoclipOp(PSViewOperatorMgr *mgr);
  static void eofillOp(PSViewOperatorMgr *mgr);
  static void eqOp(PSViewOperatorMgr *mgr);
  static void erasePageOp(PSViewOperatorMgr *mgr);
  static void exchOp(PSViewOperatorMgr *mgr);
  static void execOp(PSViewOperatorMgr *mgr);
  static void execFormOp(PSViewOperatorMgr *mgr);
  static void execStackOp(PSViewOperatorMgr *mgr);
  static void execUserObjectOp(PSViewOperatorMgr *mgr);
  static void executeOnlyOp(PSViewOperatorMgr *mgr);
  static void executiveOp(PSViewOperatorMgr *mgr);
  static void exitOp(PSViewOperatorMgr *mgr);
  static void expOp(PSViewOperatorMgr *mgr);
  static void falseOp(PSViewOperatorMgr *mgr);
  static void fileOp(PSViewOperatorMgr *mgr);
  static void filenameForAllOp(PSViewOperatorMgr *mgr);
  static void filePositionOp(PSViewOperatorMgr *mgr);
  static void fillOp(PSViewOperatorMgr *mgr);
  static void filterOp(PSViewOperatorMgr *mgr);
  static void findFontOp(PSViewOperatorMgr *mgr);
  static void findResourceOp(PSViewOperatorMgr *mgr);
  static void flattenPathOp(PSViewOperatorMgr *mgr);
  static void floorOp(PSViewOperatorMgr *mgr);
  static void flushOp(PSViewOperatorMgr *mgr);
  static void flushFileOp(PSViewOperatorMgr *mgr);
  static void forOp(PSViewOperatorMgr *mgr);
  static void forAllOp(PSViewOperatorMgr *mgr);
  static void gcheckOp(PSViewOperatorMgr *mgr);
  static void geOp(PSViewOperatorMgr *mgr);
  static void getOp(PSViewOperatorMgr *mgr);
  static void getIntervalOp(PSViewOperatorMgr *mgr);
  static void globalDictOp(PSViewOperatorMgr *mgr);
  static void glyphShowOp(PSViewOperatorMgr *mgr);
  static void grestoreOp(PSViewOperatorMgr *mgr);
  static void grestoreAllOp(PSViewOperatorMgr *mgr);
  static void gsaveOp(PSViewOperatorMgr *mgr);
  static void gstateOp(PSViewOperatorMgr *mgr);
  static void gtOp(PSViewOperatorMgr *mgr);
  static void handleErrorOp(PSViewOperatorMgr *mgr);
  static void identMatrixOp(PSViewOperatorMgr *mgr);
  static void idivOp(PSViewOperatorMgr *mgr);
  static void idtransformOp(PSViewOperatorMgr *mgr);
  static void ifOp(PSViewOperatorMgr *mgr);
  static void ifelseOp(PSViewOperatorMgr *mgr);
  static void imageOp(PSViewOperatorMgr *mgr);
  static void imageMaskOp(PSViewOperatorMgr *mgr);
  static void indexOp(PSViewOperatorMgr *mgr);
  static void initClipOp(PSViewOperatorMgr *mgr);
  static void initGraphicsOp(PSViewOperatorMgr *mgr);
  static void initMatrixOp(PSViewOperatorMgr *mgr);
  static void internalDictOp(PSViewOperatorMgr *mgr);
  static void invertMatrixOp(PSViewOperatorMgr *mgr);
  static void itransformOp(PSViewOperatorMgr *mgr);
  static void knownOp(PSViewOperatorMgr *mgr);
  static void kshowOp(PSViewOperatorMgr *mgr);
  static void languageLevelOp(PSViewOperatorMgr *mgr);
  static void leOp(PSViewOperatorMgr *mgr);
  static void lengthOp(PSViewOperatorMgr *mgr);
  static void lineToOp(PSViewOperatorMgr *mgr);
  static void lnOp(PSViewOperatorMgr *mgr);
  static void loadOp(PSViewOperatorMgr *mgr);
  static void logOp(PSViewOperatorMgr *mgr);
  static void loopOp(PSViewOperatorMgr *mgr);
  static void ltOp(PSViewOperatorMgr *mgr);
  static void makeFontOp(PSViewOperatorMgr *mgr);
  static void makePatternOp(PSViewOperatorMgr *mgr);
  static void markOp(PSViewOperatorMgr *mgr);
  static void matrixOp(PSViewOperatorMgr *mgr);
  static void maxLengthOp(PSViewOperatorMgr *mgr);
  static void modOp(PSViewOperatorMgr *mgr);
  static void moveToOp(PSViewOperatorMgr *mgr);
  static void mulOp(PSViewOperatorMgr *mgr);
  static void neOp(PSViewOperatorMgr *mgr);
  static void negOp(PSViewOperatorMgr *mgr);
  static void newPathOp(PSViewOperatorMgr *mgr);
  static void notOp(PSViewOperatorMgr *mgr);
  static void noAccessOp(PSViewOperatorMgr *mgr);
  static void nullOp(PSViewOperatorMgr *mgr);
  static void orOp(PSViewOperatorMgr *mgr);
  static void packedArrayOp(PSViewOperatorMgr *mgr);
  static void pathBBoxOp(PSViewOperatorMgr *mgr);
  static void pathForAllOp(PSViewOperatorMgr *mgr);
  static void popOp(PSViewOperatorMgr *mgr);
  static void printOp(PSViewOperatorMgr *mgr);
  static void productOp(PSViewOperatorMgr *mgr);
  static void promptOp(PSViewOperatorMgr *mgr);
  static void pstackOp(PSViewOperatorMgr *mgr);
  static void putOp(PSViewOperatorMgr *mgr);
  static void putIntervalOp(PSViewOperatorMgr *mgr);
  static void quitOp(PSViewOperatorMgr *mgr);
  static void randOp(PSViewOperatorMgr *mgr);
  static void rcheckOp(PSViewOperatorMgr *mgr);
  static void rcurveToOp(PSViewOperatorMgr *mgr);
  static void readOp(PSViewOperatorMgr *mgr);
  static void readHexStringOp(PSViewOperatorMgr *mgr);
  static void readLineOp(PSViewOperatorMgr *mgr);
  static void readOnlyOp(PSViewOperatorMgr *mgr);
  static void readStringOp(PSViewOperatorMgr *mgr);
  static void realTimeOp(PSViewOperatorMgr *mgr);
  static void rectClipOp(PSViewOperatorMgr *mgr);
  static void rectFillOp(PSViewOperatorMgr *mgr);
  static void rectStrokeOp(PSViewOperatorMgr *mgr);
  static void renameFileOp(PSViewOperatorMgr *mgr);
  static void repeatOp(PSViewOperatorMgr *mgr);
  static void resetFileOp(PSViewOperatorMgr *mgr);
  static void resourceForAllOp(PSViewOperatorMgr *mgr);
  static void restoreOp(PSViewOperatorMgr *mgr);
  static void reversePathOp(PSViewOperatorMgr *mgr);
  static void revisionOp(PSViewOperatorMgr *mgr);
  static void rlineToOp(PSViewOperatorMgr *mgr);
  static void rmoveToOp(PSViewOperatorMgr *mgr);
  static void rollOp(PSViewOperatorMgr *mgr);
  static void rootFontOp(PSViewOperatorMgr *mgr);
  static void rotateOp(PSViewOperatorMgr *mgr);
  static void roundOp(PSViewOperatorMgr *mgr);
  static void rrandOp(PSViewOperatorMgr *mgr);
  static void runOp(PSViewOperatorMgr *mgr);
  static void saveOp(PSViewOperatorMgr *mgr);
  static void scaleOp(PSViewOperatorMgr *mgr);
  static void scaleFontOp(PSViewOperatorMgr *mgr);
  static void searchOp(PSViewOperatorMgr *mgr);
  static void selectFontOp(PSViewOperatorMgr *mgr);
  static void serialNumberOp(PSViewOperatorMgr *mgr);
  static void setBlackGenerationOp(PSViewOperatorMgr *mgr);
  static void setCacheDeviceOp(PSViewOperatorMgr *mgr);
  static void setCharWidthOp(PSViewOperatorMgr *mgr);
  static void setCMYKColorOp(PSViewOperatorMgr *mgr);
  static void setColorOp(PSViewOperatorMgr *mgr);
  static void setColorSpaceOp(PSViewOperatorMgr *mgr);
  static void setDashOp(PSViewOperatorMgr *mgr);
  static void setFilePositionOp(PSViewOperatorMgr *mgr);
  static void setFlatOp(PSViewOperatorMgr *mgr);
  static void setFontOp(PSViewOperatorMgr *mgr);
  static void setGlobalOp(PSViewOperatorMgr *mgr);
  static void setGrayOp(PSViewOperatorMgr *mgr);
  static void setGStateOp(PSViewOperatorMgr *mgr);
  static void setHSBColorOp(PSViewOperatorMgr *mgr);
  static void setLineCapOp(PSViewOperatorMgr *mgr);
  static void setLineJoinOp(PSViewOperatorMgr *mgr);
  static void setLineWidthOp(PSViewOperatorMgr *mgr);
  static void setMatrixOp(PSViewOperatorMgr *mgr);
  static void setMitreLimitOp(PSViewOperatorMgr *mgr);
  static void setPackingOp(PSViewOperatorMgr *mgr);
  static void setPatternOp(PSViewOperatorMgr *mgr);
  static void setRGBColorOp(PSViewOperatorMgr *mgr);
  static void setStrokeAdjustOp(PSViewOperatorMgr *mgr);
  static void setUnderColorRemovalOp(PSViewOperatorMgr *mgr);
  static void showOp(PSViewOperatorMgr *mgr);
  static void showPageOp(PSViewOperatorMgr *mgr);
  static void sinOp(PSViewOperatorMgr *mgr);
  static void sqrtOp(PSViewOperatorMgr *mgr);
  static void srandOp(PSViewOperatorMgr *mgr);
  static void stackOp(PSViewOperatorMgr *mgr);
  static void statusOp(PSViewOperatorMgr *mgr);
  static void stopOp(PSViewOperatorMgr *mgr);
  static void stoppedOp(PSViewOperatorMgr *mgr);
  static void storeOp(PSViewOperatorMgr *mgr);
  static void stringOp(PSViewOperatorMgr *mgr);
  static void stringWidthOp(PSViewOperatorMgr *mgr);
  static void strokeOp(PSViewOperatorMgr *mgr);
  static void strokePathOp(PSViewOperatorMgr *mgr);
  static void subOp(PSViewOperatorMgr *mgr);
  static void systemDictOp(PSViewOperatorMgr *mgr);
  static void tokenOp(PSViewOperatorMgr *mgr);
  static void transformOp(PSViewOperatorMgr *mgr);
  static void translateOp(PSViewOperatorMgr *mgr);
  static void trueOp(PSViewOperatorMgr *mgr);
  static void truncateOp(PSViewOperatorMgr *mgr);
  static void typeOp(PSViewOperatorMgr *mgr);
  static void undefOp(PSViewOperatorMgr *mgr);
  static void undefineFontOp(PSViewOperatorMgr *mgr);
  static void undefineUserObjectOp(PSViewOperatorMgr *mgr);
  static void userDictOp(PSViewOperatorMgr *mgr);
  static void userTimeOp(PSViewOperatorMgr *mgr);
  static void versionOp(PSViewOperatorMgr *mgr);
  static void vmStatusOp(PSViewOperatorMgr *mgr);
  static void wcheckOp(PSViewOperatorMgr *mgr);
  static void whereOp(PSViewOperatorMgr *mgr);
  static void widthShowOp(PSViewOperatorMgr *mgr);
  static void writeOp(PSViewOperatorMgr *mgr);
  static void writeHexStringOp(PSViewOperatorMgr *mgr);
  static void writeStringOp(PSViewOperatorMgr *mgr);
  static void xcheckOp(PSViewOperatorMgr *mgr);
  static void xorOp(PSViewOperatorMgr *mgr);
  static void xshowOp(PSViewOperatorMgr *mgr);
  static void xyshowOp(PSViewOperatorMgr *mgr);
  static void yshowOp(PSViewOperatorMgr *mgr);
  static void unimplementedOp(PSViewOperatorMgr *mgr);
  static void charPathFillOp(PSViewOperatorMgr *mgr);
  static void charPathStrokeTrueOp(PSViewOperatorMgr *mgr);
  static void charPathStrokeFalseOp(PSViewOperatorMgr *mgr);

 private:
  void bind1(PSViewToken *token);

  PSViewToken *readStringFileToken(const std::string &str, PSVinteger *pos);

 private:
  PSViewOperatorMgr(const PSViewOperatorMgr &rhs);
  PSViewOperatorMgr &operator=(const PSViewOperatorMgr &rhs);
};

#endif
