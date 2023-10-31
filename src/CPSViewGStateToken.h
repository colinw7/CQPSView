#ifndef CPSViewGStateToken_H
#define CPSViewGStateToken_H

class PSViewGStateTokenMgr {
 public:
  PSViewGStateTokenMgr(CPSView *psview);
 ~PSViewGStateTokenMgr();

  CPSView *getPSView() const { return psview_; }

  void init();

  PSViewGStateToken *getCurrent();

  void setCurrent(PSViewGStateToken *gstate_token);

  PSViewGStateToken *save();
  PSViewGStateToken *restore();

  void restoreAll();

 private:
  PSViewGStateTokenMgr(const PSViewGStateTokenMgr &rhs);
  PSViewGStateTokenMgr &operator=(const PSViewGStateTokenMgr &rhs);

 private:
  CPSView           *psview_;
  PSViewGStateToken *current_gstate_;
};

//---

class PSViewGStateToken : public PSViewToken {
 public:
  PSViewGStateToken(CPSView *psview);
  PSViewGStateToken(const PSViewGStateToken &gstate_token);

  PSViewGStateToken &operator=(const PSViewGStateToken &gstate_token);

  //----

  PSViewGState *getGState() const { return gstate_; }

  PSViewGStatePattern *getPattern() const { return gstate_->getPattern(); }

  //----

  // virtual functions

  ~PSViewGStateToken();

  PSViewGStateToken *dup() const override;

  int compare(PSViewToken *token) override;

  void executeToken() override;

  const PSViewName &getName() override;

  void print() override;

  std::string toString() override;

  //----

  void initGraphics();

  void newPath();

  void initMatrix();
  void initClip();

  void moveTo(double x, double y);
  void rmoveTo(double x, double y);
  void lineTo(double x, double y);
  void rlineTo(double x, double y);
  void arc(double x, double y, double r, double angle1, double angle2);
  void arcN(double x, double y, double r, double angle1, double angle2);
  void arcTo(double x1, double y1, double x2, double y2, double r,
             double *xt1, double *yt1, double *xt2, double *yt2);
  void curveTo(double x1, double y1, double x2, double y2, double x3, double y3);
  void rcurveTo(double x1, double y1, double x2, double y2, double x3, double y3);
  bool getCurrentPoint(double *x, double *y);
  void show(const std::string &str);
  void ashow(double ax, double ay, const std::string &str);
  void widthShow(double cx, double cy, int c, const std::string &str);
  void awidthShow(double cx, double cy, int c, double ax, double ay, const std::string &str);
  void glyphShow(const PSViewName &name);
  void kshow(PSViewToken *proc, const std::string &str);
  void stringWidth(const std::string &str, double *wx, double *wy);
  void charPath(const std::string &str, bool flag);
  void image(char *image_data, int width, int height, int bits_per_sample, CMatrix2D *matrix,
             double *decode_array, int decode_size);
  void imageMask(char *image_data, int width, int height, int polarity, CMatrix2D *matrix);
  void closePath();
  void rectStroke(double x, double y, double width, double height, CMatrix2D *matrix);
  void rectFill(double x, double y, double width, double height);
  void rectClip(double *x, double *y, double *width, double *height, int num_rects);
  void stroke();
  void strokePath();
  void fill();
  void eofill();
  void clip();
  void eoclip();
  void clipPath();
  void flattenPath();
  void reversePath();
  void pathBBox(double *llx, double *lly, double *urx, double *ury);
  void pathForAll(PSViewToken *move_to_token, PSViewToken *line_to_token,
                  PSViewToken *curve_to_token, PSViewToken *close_path_token);
  void showPage();
  void erasePage();
  void defineFont(PSViewToken *key, PSViewDictionaryToken *dictionary);
  void undefineFont(PSViewToken *key);
  void setCacheDevice(double wx, double wy, double llx, double lly, double urx, double ury);
  void setCharWidth(double wx, double wy);
  void getCharWidth(double *wx, double *wy);

  PSViewDictionaryToken *findFont(PSViewToken *key);
  PSViewDictionaryToken *readFont(PSViewToken *key);
  void                   setFont(PSViewDictionaryToken *font);

  PSViewToken *getCurrentFont();
  PSViewToken *getRootFont();

  void selectScaleFont(PSViewToken *key, PSVreal scale);
  void selectMakeFont(PSViewToken *key, CMatrix2D *matrix);

  PSViewDictionaryToken *scaleFont(PSViewDictionaryToken *font, PSVreal scale);
  PSViewDictionaryToken *makeFont(PSViewDictionaryToken *font, CMatrix2D *matrix);
  PSViewDictionaryToken *makePattern(PSViewDictionaryToken *pattern, CMatrix2D *matrix);

  void setPattern(PSViewDictionaryToken *pattern_dictionary);
  void execForm(PSViewDictionaryToken *form_dictionary);
  void setColorSpace(const PSViewName &color_space);
  void setPatternColor(bool pattern);

  const PSViewName &getColorSpace();

  PSViewNameToken *getColorSpaceToken();

  bool getPatternColor();
  void setCMYKColor(const CCMYK &cmyk);
  void getCMYKColor(CCMYK &cmyk);
  void setHSBColor(const CHSB &hsb);
  void getHSBColor(CHSB &hsb);
  void setRGBColor(const CRGBA &rgba);
  void getRGBColor(CRGBA &rgba);
  void setGray(double gray);
  void getGray(double *gray);

  void   setFlat(double flat);
  double getFlat();

  void   setLineWidth(double line_width);
  double getLineWidth();

  void         setLineCap(CLineCapType line_cap);
  CLineCapType getLineCap();

  void          setLineJoin(CLineJoinType line_join);
  CLineJoinType getLineJoin();

  void   setMitreLimit(double mitre_limit);
  double getMitreLimit();

  void setStrokeAdjust(bool flag);
  bool getStrokeAdjust();

  void setDashPattern(double *dash_array, int num_dashes, double dash_offset);
  void getDashPattern(const double **dash_array, int *num_dashes, double *dash_offset);

  void         setBlackGeneration(PSViewToken *token);
  PSViewToken *getBlackGeneration();

  void         setUnderColorRemoval(PSViewToken *token);
  PSViewToken *getUnderColorRemoval();

  CMatrix2D *getDefaultMatrix();
  void       setCTMMatrix(CMatrix2D *matrix);
  CMatrix2D *getCTMMatrix();
  CMatrix2D *getInverseCTMMatrix();

  void preMultiplyCTMMatrix(CMatrix2D *matrix);
  void postMultiplyCTMMatrix(CMatrix2D *matrix);

  void multiplyByCTMMatrix(double x1, double y1, double *x2, double *y2);
  void multiplyByInverseCTMMatrix(double x1, double y1, double *x2, double *y2);

 private:
  CAutoPtr<PSViewGState> gstate_;
};

#endif
