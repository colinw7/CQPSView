#include <CPSViewI.h>

PSViewGStateTokenMgr::
PSViewGStateTokenMgr(CPSView *psview) :
 psview_        (psview),
 current_gstate_(NULL)
{
}

PSViewGStateTokenMgr::
~PSViewGStateTokenMgr()
{
}

void
PSViewGStateTokenMgr::
init()
{
  current_gstate_ = NULL;
}

PSViewGStateToken *
PSViewGStateTokenMgr::
getCurrent()
{
  if (current_gstate_ == NULL)
    current_gstate_ = new PSViewGStateToken(psview_);

  return current_gstate_;
}

void
PSViewGStateTokenMgr::
setCurrent(PSViewGStateToken *gstate_token)
{
  current_gstate_ = gstate_token;
}

PSViewGStateToken *
PSViewGStateTokenMgr::
save()
{
  PSViewGStateToken *gstate_token = getCurrent()->dup();

  psview_->getGraphicsStack()->push(gstate_token);

  return getCurrent();
}

PSViewGStateToken *
PSViewGStateTokenMgr::
restore()
{
  PSViewGStateToken *gstate_token = psview_->getGraphicsStack()->pop();

  *current_gstate_ = *gstate_token;

  delete gstate_token;

  return getCurrent();
}

void
PSViewGStateTokenMgr::
restoreAll()
{
  int num = psview_->getGraphicsStack()->size();

  if (num < 1)
    return;

  for (int i = 0; i < num - 1; i++) {
    PSViewGStateToken *gstate_token = psview_->getGraphicsStack()->pop();

    delete gstate_token;
  }

  restore();

  getCurrent()->initGraphics();
}

//----------------

PSViewGStateToken::
PSViewGStateToken(CPSView *psview) :
 PSViewToken(psview, PSVIEW_TOKEN_TYPE_GSTATE,
             PSVIEW_TOKEN_COMPOSITE,
             PSVIEW_TOKEN_LITERAL,
             PSVIEW_TOKEN_ACCESS_TYPE_READ_WRITE),
 gstate_    ()
{
  gstate_ = new PSViewGState(psview_->getGStateMgr());
}

PSViewGStateToken::
PSViewGStateToken(const PSViewGStateToken &gstate_token) :
 PSViewToken(gstate_token),
 gstate_    ()
{
  gstate_ = new PSViewGState(*gstate_token.gstate_);
}

PSViewGStateToken::
~PSViewGStateToken()
{
}

PSViewGStateToken &
PSViewGStateToken::
operator=(const PSViewGStateToken &gstate_token)
{
  if (&gstate_token == this)
    return *this;

  *gstate_ = *gstate_token.gstate_;

  return *this;
}

PSViewGStateToken *
PSViewGStateToken::
dup() const
{
  return new PSViewGStateToken(*this);
}

int
PSViewGStateToken::
compare(PSViewToken *token)
{
  if (token->isType(type_)) {
    PSViewGStateToken *gstate_token = dynamic_cast<PSViewGStateToken *>(token);

    return gstate_->compare(gstate_token->gstate_);
  }
  else
    return type_ - token->getType();
}

void
PSViewGStateToken::
executeToken()
{
  CStrUtil::eprintf("PSView: Execute Code for GState Missing\n");
}

const PSViewName &
PSViewGStateToken::
getName()
{
  CTHROW("No name for token");

  return psview_->getNameMgr()->getName("");
}

void
PSViewGStateToken::
print()
{
  CStrUtil::printf("-gstate-");
}

string
PSViewGStateToken::
toString()
{
  return "--nostringval--";
}

void
PSViewGStateToken::
initGraphics()
{
  gstate_->initGraphics();
}

void
PSViewGStateToken::
newPath()
{
  gstate_->newPath();
}

void
PSViewGStateToken::
initMatrix()
{
  gstate_->initMatrix();
}

void
PSViewGStateToken::
initClip()
{
  gstate_->initClip();
}

void
PSViewGStateToken::
moveTo(double x, double y)
{
  gstate_->moveTo(x, y);
}

void
PSViewGStateToken::
rmoveTo(double x, double y)
{
  gstate_->rmoveTo(x, y);
}

void
PSViewGStateToken::
lineTo(double x, double y)
{
  gstate_->lineTo(x, y);
}

void
PSViewGStateToken::
rlineTo(double x, double y)
{
  gstate_->rlineTo(x, y);
}

void
PSViewGStateToken::
arc(double x, double y, double r, double angle1, double angle2)
{
  gstate_->arc(x, y, r, angle1, angle2);
}

void
PSViewGStateToken::
arcN(double x, double y, double r, double angle1, double angle2)
{
  gstate_->arcN(x, y, r, angle1, angle2);
}

void
PSViewGStateToken::
arcTo(double x1, double y1, double x2, double y2, double r,
      double *xt1, double *yt1, double *xt2, double *yt2)
{
  gstate_->arcTo(x1, y1, x2, y2, r, xt1, yt1, xt2, yt2);
}

void
PSViewGStateToken::
curveTo(double x1, double y1, double x2, double y2, double x3, double y3)
{
  gstate_->curveTo(x1, y1, x2, y2, x3, y3);
}

void
PSViewGStateToken::
rcurveTo(double x1, double y1, double x2, double y2, double x3, double y3)
{
  gstate_->rcurveTo(x1, y1, x2, y2, x3, y3);
}

bool
PSViewGStateToken::
getCurrentPoint(double *x, double *y)
{
  return gstate_->getCurrentPoint(x, y);
}

void
PSViewGStateToken::
show(const string &str)
{
  gstate_->show(str);
}

void
PSViewGStateToken::
ashow(double ax, double ay, const string &str)
{
  gstate_->ashow(ax, ay, str);
}

void
PSViewGStateToken::
widthShow(double cx, double cy, int c, const string &str)
{
  gstate_->widthShow(cx, cy, c, str);
}

void
PSViewGStateToken::
awidthShow(double cx, double cy, int c, double ax, double ay,
           const string &str)
{
  gstate_->awidthShow(cx, cy, c, ax, ay, str);
}

void
PSViewGStateToken::
glyphShow(const PSViewName &name)
{
  gstate_->glyphShow(name);
}

void
PSViewGStateToken::
kshow(PSViewToken *proc, const string &str)
{
  gstate_->kshow(proc, str);
}

void
PSViewGStateToken::
stringWidth(const string &str, double *wx, double *wy)
{
  gstate_->stringWidth(str, wx, wy);
}

void
PSViewGStateToken::
charPath(const string &str, bool flag)
{
  gstate_->charPath(str, flag);
}

void
PSViewGStateToken::
image(char *image_data, int width, int height, int bits_per_sample,
      CMatrix2D *matrix, double *decode_array, int decode_size)
{
  gstate_->image(image_data, width, height, bits_per_sample,
                 matrix, decode_array, decode_size);
}

void
PSViewGStateToken::
imageMask(char *image_data, int width, int height,
          int polarity, CMatrix2D *matrix)
{
  gstate_->imageMask(image_data, width, height, polarity, matrix);
}

void
PSViewGStateToken::
closePath()
{
  gstate_->closePath();
}

void
PSViewGStateToken::
rectStroke(double x, double y, double width, double height, CMatrix2D *matrix)
{
  gstate_->rectStroke(x, y, width, height, matrix);
}

void
PSViewGStateToken::
rectFill(double x, double y, double width, double height)
{
  gstate_->rectFill(x, y, width, height);
}

void
PSViewGStateToken::
rectClip(double *x, double *y, double *width, double *height, int num_rects)
{
  gstate_->rectClip(x, y, width, height, num_rects);
}

void
PSViewGStateToken::
stroke()
{
  gstate_->stroke();
}

void
PSViewGStateToken::
strokePath()
{
  gstate_->strokePath();
}

void
PSViewGStateToken::
fill()
{
  gstate_->fill();
}

void
PSViewGStateToken::
eofill()
{
  gstate_->eofill();
}

void
PSViewGStateToken::
clip()
{
  gstate_->clip();
}

void
PSViewGStateToken::
eoclip()
{
  gstate_->eoclip();
}

void
PSViewGStateToken::
clipPath()
{
  gstate_->clipPath();
}

void
PSViewGStateToken::
flattenPath()
{
  gstate_->flattenPath();
}

void
PSViewGStateToken::
reversePath()
{
  gstate_->reversePath();
}

void
PSViewGStateToken::
pathBBox(double *llx, double *lly, double *urx, double *ury)
{
  gstate_->pathBBox(llx, lly, urx, ury);
}

void
PSViewGStateToken::
pathForAll(PSViewToken *move_to_token, PSViewToken *line_to_token,
           PSViewToken *curve_to_token, PSViewToken *close_path_token)
{
  gstate_->pathForAll(move_to_token, line_to_token,
                      curve_to_token, close_path_token);
}

void
PSViewGStateToken::
showPage()
{
  gstate_->showPage();
}

void
PSViewGStateToken::
erasePage()
{
  gstate_->erasePage();
}

void
PSViewGStateToken::
defineFont(PSViewToken *key, PSViewDictionaryToken *dictionary)
{
  gstate_->defineFont(key, dictionary);
}

void
PSViewGStateToken::
undefineFont(PSViewToken *key)
{
  gstate_->undefineFont(key);
}

void
PSViewGStateToken::
setCacheDevice(double wx, double wy, double llx, double lly,
               double urx, double ury)
{
  gstate_->setCacheDevice(wx, wy, llx, lly, urx, ury);
}

void
PSViewGStateToken::
setCharWidth(double wx, double wy)
{
  gstate_->setCharWidth(wx, wy);
}

void
PSViewGStateToken::
getCharWidth(double *wx, double *wy)
{
  gstate_->getCharWidth(wx, wy);
}

PSViewDictionaryToken *
PSViewGStateToken::
findFont(PSViewToken *key)
{
  return gstate_->findFont(key);
}

PSViewDictionaryToken *
PSViewGStateToken::
readFont(PSViewToken *key)
{
  return gstate_->readFont(key);
}

void
PSViewGStateToken::
setFont(PSViewDictionaryToken *font)
{
  gstate_->setFont(font);
}

PSViewToken *
PSViewGStateToken::
getCurrentFont()
{
  return gstate_->getCurrentFont();
}

PSViewToken *
PSViewGStateToken::
getRootFont()
{
  return gstate_->getRootFont();
}

void
PSViewGStateToken::
selectScaleFont(PSViewToken *key, PSVreal scale)
{
  gstate_->selectScaleFont(key, scale);
}

void
PSViewGStateToken::
selectMakeFont(PSViewToken *key, CMatrix2D *matrix)
{
  gstate_->selectMakeFont(key, matrix);
}

PSViewDictionaryToken *
PSViewGStateToken::
scaleFont(PSViewDictionaryToken *font_dictionary, PSVreal scale)
{
  return gstate_->scaleFont(font_dictionary, scale);
}

PSViewDictionaryToken *
PSViewGStateToken::
makeFont(PSViewDictionaryToken *font_dictionary, CMatrix2D *matrix)
{
  return gstate_->makeFont(font_dictionary, matrix);
}

PSViewDictionaryToken *
PSViewGStateToken::
makePattern(PSViewDictionaryToken *pattern_dictionary, CMatrix2D *matrix)
{
  return gstate_->makePattern(pattern_dictionary, matrix);
}

void
PSViewGStateToken::
setPattern(PSViewDictionaryToken *pattern_dictionary)
{
  gstate_->setPattern(pattern_dictionary);
}

void
PSViewGStateToken::
execForm(PSViewDictionaryToken *form_dictionary)
{
  gstate_->execForm(form_dictionary);
}

void
PSViewGStateToken::
setColorSpace(const PSViewName &color_space)
{
  gstate_->setColorSpace(color_space);
}

void
PSViewGStateToken::
setPatternColor(bool pattern)
{
  gstate_->setPatternColor(pattern);
}

const PSViewName &
PSViewGStateToken::
getColorSpace()
{
  return gstate_->getColorSpace();
}

PSViewNameToken *
PSViewGStateToken::
getColorSpaceToken()
{
  const PSViewName &color_space = gstate_->getColorSpace();

  PSViewNameToken *token = new PSViewNameToken(psview_, color_space);

  token->setLiteral();

  return token;
}

bool
PSViewGStateToken::
getPatternColor()
{
  return gstate_->getPatternColor();
}

void
PSViewGStateToken::
setCMYKColor(const CCMYK &cmyk)
{
  gstate_->setCMYKColor(cmyk);
}

void
PSViewGStateToken::
getCMYKColor(CCMYK &cmyk)
{
  gstate_->getCMYKColor(cmyk);
}

void
PSViewGStateToken::
setHSBColor(const CHSB &hsb)
{
  gstate_->setHSBColor(hsb);
}

void
PSViewGStateToken::
getHSBColor(CHSB &hsb)
{
  gstate_->getHSBColor(hsb);
}

void
PSViewGStateToken::
setRGBColor(const CRGBA &rgba)
{
  gstate_->setRGBColor(rgba);
}

void
PSViewGStateToken::
getRGBColor(CRGBA &rgba)
{
  gstate_->getRGBColor(rgba);
}

void
PSViewGStateToken::
setGray(double gray)
{
  gstate_->setGray(gray);
}

void
PSViewGStateToken::
getGray(double *gray)
{
  *gray = gstate_->getGray();
}

void
PSViewGStateToken::
setFlat(double flat)
{
  gstate_->setFlat(flat);
}

double
PSViewGStateToken::
getFlat()
{
  return gstate_->getFlat();
}

void
PSViewGStateToken::
setLineWidth(double line_width)
{
  gstate_->setLineWidth(line_width);
}

double
PSViewGStateToken::
getLineWidth()
{
  return gstate_->getLineWidth();
}

void
PSViewGStateToken::
setLineCap(CLineCapType line_cap)
{
  gstate_->setLineCap(line_cap);
}

CLineCapType
PSViewGStateToken::
getLineCap()
{
  return gstate_->getLineCap();
}

void
PSViewGStateToken::
setLineJoin(CLineJoinType line_join)
{
  gstate_->setLineJoin(line_join);
}

CLineJoinType
PSViewGStateToken::
getLineJoin()
{
  return gstate_->getLineJoin();
}

void
PSViewGStateToken::
setMitreLimit(double mitre_limit)
{
  gstate_->setMitreLimit(mitre_limit);
}

double
PSViewGStateToken::
getMitreLimit()
{
  return gstate_->getMitreLimit();
}

void
PSViewGStateToken::
setStrokeAdjust(bool flag)
{
  gstate_->setStrokeAdjust(flag);
}

bool
PSViewGStateToken::
getStrokeAdjust()
{
  return gstate_->getStrokeAdjust();
}

void
PSViewGStateToken::
setDashPattern(double *dash_array, int num_dashes, double dash_offset)
{
  gstate_->setDashPattern(dash_array, num_dashes, dash_offset);
}

void
PSViewGStateToken::
getDashPattern(const double **dash_array, int *num_dashes, double *dash_offset)
{
  gstate_->getDashPattern(dash_array, num_dashes, dash_offset);
}

void
PSViewGStateToken::
setBlackGeneration(PSViewToken *token)
{
  gstate_->setBlackGeneration(token);
}

PSViewToken *
PSViewGStateToken::
getBlackGeneration()
{
  return gstate_->getBlackGeneration();
}

void
PSViewGStateToken::
setUnderColorRemoval(PSViewToken *token)
{
  gstate_->setUnderColorRemoval(token);
}

PSViewToken *
PSViewGStateToken::
getUnderColorRemoval()
{
  return gstate_->getUnderColorRemoval();
}

CMatrix2D *
PSViewGStateToken::
getDefaultMatrix()
{
  return gstate_->getGStateMgr()->getDefaultMatrix();
}

void
PSViewGStateToken::
setCTMMatrix(CMatrix2D *matrix)
{
  gstate_->setCTMMatrix(matrix);
}

CMatrix2D *
PSViewGStateToken::
getCTMMatrix()
{
  return gstate_->getCTMMatrix();
}

CMatrix2D *
PSViewGStateToken::
getInverseCTMMatrix()
{
  return gstate_->getInverseCTMMatrix();
}

void
PSViewGStateToken::
preMultiplyCTMMatrix(CMatrix2D *matrix)
{
  gstate_->preMultiplyCTMMatrix(matrix);
}

void
PSViewGStateToken::
postMultiplyCTMMatrix(CMatrix2D *matrix)
{
  gstate_->postMultiplyCTMMatrix(matrix);
}

void
PSViewGStateToken::
multiplyByCTMMatrix(double x1, double y1, double *x2, double *y2)
{
  gstate_->multiplyByCTMMatrix(x1, y1, x2, y2);
}

void
PSViewGStateToken::
multiplyByInverseCTMMatrix(double x1, double y1, double *x2, double *y2)
{
  gstate_->multiplyByInverseCTMMatrix(x1, y1, x2, y2);
}
