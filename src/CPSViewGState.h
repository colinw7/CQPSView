#ifndef CPSViewGState_H
#define CPSViewGState_H

#include <CPen.h>
#include <CBrush.h>

class PSViewGStateFont;
class PSViewGStatePattern;

class PSViewGStateMgr {
 public:
  PSViewGStateMgr(CPSView *psview);
 ~PSViewGStateMgr();

  void init();

  CPSView *getPSView() const { return psview_; }

  const PSViewName &getCMYKColorSpace() const {
    return *CMYKColorSpaceName_;
  }

  const PSViewName &getRGBColorSpace() const {
    return *RGBColorSpaceName_;
  }

  const PSViewName &getGrayColorSpace() const {
    return *GrayColorSpaceName_;
  }

  const PSViewName &getPatternColorSpace() const {
    return *PatternColorSpaceName_;
  }

  const std::string &getFontDir() const { return font_dir_; }

  int getNextFontId() { return ++current_font_id_; }

  CMatrix2D *getDefaultMatrix();

  PSViewDictionaryToken *getDefaultFont() const { return default_font_; }

 private:
  void createDefaultFont();

 private:
  PSViewGStateMgr(const PSViewGStateMgr &rhs);
  PSViewGStateMgr &operator=(const PSViewGStateMgr &rhs);

 private:
  CPSView               *psview_                { nullptr };
  const PSViewName      *CMYKColorSpaceName_    { nullptr };
  const PSViewName      *RGBColorSpaceName_     { nullptr };
  const PSViewName      *GrayColorSpaceName_    { nullptr };
  const PSViewName      *PatternColorSpaceName_ { nullptr };
  std::string            font_dir_;
  CMatrix2D              default_ctm_matrix_;
  int                    current_font_id_       { 0 };
  PSViewDictionaryToken *default_font_          { nullptr };
};

//---

class PSViewGState {
 public:
  PSViewGState(PSViewGStateMgr *mgr);
  PSViewGState(const PSViewGState &gstate);
 ~PSViewGState();

  PSViewGState &operator=(const PSViewGState &gstate);

  PSViewGStateMgr *getGStateMgr() const { return mgr_; }

  CPSView *getPSView() const { return mgr_->getPSView(); }

  PSViewGStateFont *getFont() const { return font_; }

  PSViewGStatePattern *getPattern() const { return pattern_; }

  int compare(PSViewGState *gstate);

  void initGraphics();
  void setGraphics();

  void initMatrix();

  void newPath();

  void initClip();

  void moveTo(double x, double y);
  void rmoveTo(double x, double y);
  void lineTo(double x, double y);
  void rlineTo(double x, double y);

  void arc(double, double, double, double, double);
  void arcN(double, double, double, double, double);
  void arcTo(double, double, double, double, double,
             double *, double *, double *, double *);

  void curveTo(double, double, double, double, double, double);
  void rcurveTo(double, double, double, double, double, double);

  bool getCurrentPoint(double *, double *);

  void show(const std::string &str);
  void ashow(double, double, const std::string &str);
  void widthShow(double, double, int, const std::string &str);
  void awidthShow(double, double, int, double, double, const std::string &str);
  void glyphShow(const PSViewName &name);
  void kshow(PSViewToken *, const std::string &str);

  void stringWidth(const std::string &str, double *, double *);

  void charPath(const std::string &str, int flag);

  void image(char *image_data, int width, int height, int bits_per_sample,
             CMatrix2D *matrix, double *decode_array, int decode_size);

  void imageMask(char *image_data, int width, int height, int polarity, CMatrix2D *matrix);

  void closePath();

  void rectStroke(double, double, double, double, CMatrix2D *);
  void rectFill(double, double, double, double);
  void rectClip(double *, double *, double *, double *, int);

  void stroke();
  void strokePath();

  void fill();
  void patternFill();
  void eofill();

  void clip();
  void eoclip();

  void clipPath();
  void flattenPath();
  void reversePath();

  void pathBBox(double *, double *, double *, double *);
  void pathForAll(PSViewToken *, PSViewToken *, PSViewToken *, PSViewToken *);

  void adjustPoint(double *x, double *y);

  void showPage();
  void erasePage();

  void setCacheDevice(double, double, double, double, double, double);

  void setCharWidth(double, double);
  void getCharWidth(double *, double *);

  void defineFont(PSViewToken *key, PSViewDictionaryToken *dictionary);
  void undefineFont(PSViewToken *key);

  PSViewDictionaryToken *findFont(PSViewToken *key);

  void setFont(PSViewDictionaryToken *font);

  PSViewDictionaryToken *getCurrentFont();
  PSViewDictionaryToken *getRootFont();

  void selectScaleFont(PSViewToken *, PSVreal);
  void selectMakeFont(PSViewToken *, CMatrix2D *);

  PSViewDictionaryToken *scaleFont(PSViewDictionaryToken *font_dictionary,
                                   PSVreal scale);
  PSViewDictionaryToken *makeFont(PSViewDictionaryToken *font_dictionary,
                                  CMatrix2D *matrix);
  PSViewDictionaryToken *readFont(PSViewToken *key);

  PSViewDictionaryToken *makePattern(PSViewDictionaryToken *pattern_dictionary,
                                     CMatrix2D *matrix);

  void setPattern(PSViewDictionaryToken *pattern_dictionary);

  void execForm(PSViewDictionaryToken *form_dictionary);

  void setColorSpace(const PSViewName &name);

  const PSViewName &getColorSpace();

  void setPatternColor(bool flag);
  bool getPatternColor();

  void setCMYKColor(const CCMYK &cmyk);
  void getCMYKColor(CCMYK &cmyk);

  void setHSBColor(const CHSB &hsb);
  void getHSBColor(CHSB &hsb);

  void setRGBColor(const CRGBA &rgba);
  void getRGBColor(CRGBA &rgba);

  void   setGray(double);
  double getGray();

  void   setFlat(double);
  double getFlat();

  void   setLineWidth(double);
  double getLineWidth();

  void         setLineCap(CLineCapType line_cap);
  CLineCapType getLineCap();

  void          setLineJoin(CLineJoinType line_join);
  CLineJoinType getLineJoin();

  void   setMitreLimit(double);
  double getMitreLimit();

  void setStrokeAdjust(bool flag);
  bool getStrokeAdjust();

  void setDashPattern(double *, int, double);
  void getDashPattern(const double **, int *, double *);

  void         setBlackGeneration(PSViewToken *token);
  PSViewToken *getBlackGeneration();

  void         setUnderColorRemoval(PSViewToken *token);
  PSViewToken *getUnderColorRemoval();

  void       setCTMMatrix(CMatrix2D *matrix);
  CMatrix2D *getCTMMatrix();
  CMatrix2D *getInverseCTMMatrix();

  void preMultiplyCTMMatrix(CMatrix2D *matrix);
  void postMultiplyCTMMatrix(CMatrix2D *matrix);

  void multiplyDistByCTMMatrix(double x1, double y1, double *x2, double *y2);

  void multiplyByCTMMatrix(double x1, double y1, double *x2, double *y2);
  void multiplyByInverseCTMMatrix(double x1, double y1,
                                  double *x2, double *y2);

 private:
  void getCharWidth1(double *wx, double *wy);

  void showDefaultChar(int, double *, double *);

  void defaultCharPath(int, double *, double *);

  int getType0Char(const std::string &str, uint *pos, PSViewDictionaryToken **font);

  void showType3Char(int, double *, double *);

  void addType3CharPath(int, double *, double *);

  void defineType0Font(PSViewToken *key, PSViewDictionaryToken *dictionary);
  void defineType3Font(PSViewToken *key, PSViewDictionaryToken *dictionary);

  void setFont1(PSViewDictionaryToken *font_dictionary);

  CCMYK rgbToCMYK(const CRGBA &rgba);

  void setRGBColor1(const CRGBA &rgba);

  double rgbToGray(double r, double g, double b) {
    return (0.3*r + 0.59*g + 0.11*b);
  }

  double cmykToGray(double c, double m, double y, double k) {
    return (1.0 - std::min(1.0, 0.3*c + 0.59*m + 0.11*y + k));
  }

  void debugPrintImage(char *image_data, int width, int height, int bits_per_sample,
                       CMatrix2D *matrix, double *decode_array, int decode_size);

  void debugPrintImageMask(char *image_data, int width, int height,
                           int polarity, CMatrix2D *matrix);

 private:
  PSViewGStateMgr               *mgr_;
  CMatrix2D                      ctm_matrix_;
  const PSViewName              *color_space_;
  bool                           pattern_color_;
  CPen                           pen_;
  CBrush                         brush_;
  CCMYK                          cmyk_;
  bool                           cmyk_valid_;
  CHSB                           hsb_;
  bool                           hsb_valid_;
  bool                           stroke_adjust_;
  CAutoPtr<PSViewToken>          black_generation_;
  CAutoPtr<PSViewToken>          undercolor_removal_;
  CAutoPtr<PSViewGStateFont>     font_;
  CPSViewRenderer               *graphics_;
  CAutoPtr<PSViewGStatePattern>  pattern_;
  CAutoPtr<PSViewPath>           path_;
  CAutoPtr<PSViewPath>           clippath_;
};

#endif
