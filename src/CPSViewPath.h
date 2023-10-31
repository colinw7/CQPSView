#ifndef CPSViewPath_H
#define CPSViewPath_H

enum PSViewPathPartId {
  PSVIEW_PATH_NONE,
  PSVIEW_PATH_MOVE_TO,
  PSVIEW_PATH_LINE_TO,
  PSVIEW_PATH_BEZIER2_TO,
  PSVIEW_PATH_BEZIER3_TO,
  PSVIEW_PATH_ARC_TO,
  PSVIEW_PATH_CLOSE
};

class PSViewPath;

class PSViewPathVisitor {
 public:
  PSViewPathVisitor() : path_(NULL) { }

  virtual ~PSViewPathVisitor() { }

  void setPath(PSViewPath *path) { path_ = path; }

  virtual void init() { }
  virtual void term() { }

  virtual void moveTo(const CPoint2D &p) = 0;

  virtual void lineTo(const CPoint2D &p) = 0;

  virtual void bezier2To(const CPoint2D &p1, const CPoint2D &p2) = 0;

  virtual void bezier3To(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3) = 0;

  virtual void arcTo(const CPoint2D &c, double xr, double yr, double theta, double delta) = 0;

  virtual void close() = 0;

 private:
  PSViewPathVisitor(const PSViewPathVisitor &rhs);
  PSViewPathVisitor &operator=(const PSViewPathVisitor &rhs);

 protected:
  PSViewPath *path_;
};

//---

class PSViewPathPart {
 public:
  PSViewPathPart(PSViewPathPartId id) :
   id_(id) {
  }

  virtual ~PSViewPathPart() { }

  virtual PSViewPathPart *dup() const = 0;

  PSViewPathPartId getId() const { return id_; }

  virtual void print(std::ostream &os) const = 0;

  virtual void visit(PSViewPathVisitor &visitor) = 0;

 private:
  PSViewPathPartId id_;
};

//---

class PSViewPathMoveTo : public PSViewPathPart {
 public:
  PSViewPathMoveTo(const CPoint2D &p) :
   PSViewPathPart(PSVIEW_PATH_MOVE_TO), p_(p) {
  }

  PSViewPathMoveTo *dup() const override {
    return new PSViewPathMoveTo(*this);
  }

  void print(std::ostream &os) const override {
    os << "MoveTo: " << p_ << "\n";
  }

  void visit(PSViewPathVisitor &visitor) override {
    visitor.moveTo(p_);
  }

 private:
  CPoint2D p_;
};

//---

class PSViewPathLineTo : public PSViewPathPart {
 public:
  PSViewPathLineTo(const CPoint2D &p) :
   PSViewPathPart(PSVIEW_PATH_LINE_TO), p_(p) {
  }

  PSViewPathLineTo *dup() const override {
    return new PSViewPathLineTo(*this);
  }

  void print(std::ostream &os) const override {
    os << "LineTo: " << p_ << "\n";
  }

  void visit(PSViewPathVisitor &visitor) override {
    visitor.lineTo(p_);
  }

 private:
  CPoint2D p_;
};

//---

class PSViewPathBezier2To : public PSViewPathPart {
 public:
  PSViewPathBezier2To(const CPoint2D &p1, const CPoint2D &p2) :
   PSViewPathPart(PSVIEW_PATH_BEZIER2_TO), p1_(p1), p2_(p2) {
  }

  PSViewPathBezier2To *dup() const override {
    return new PSViewPathBezier2To(*this);
  }

  void print(std::ostream &os) const override {
    os << "Bezier2To: " << p1_ << ", " << p2_ << "\n";
  }

  void visit(PSViewPathVisitor &visitor) override {
    visitor.bezier2To(p1_, p2_);
  }

 private:
  CPoint2D p1_;
  CPoint2D p2_;
};

//---

class PSViewPathBezier3To : public PSViewPathPart {
 public:
  PSViewPathBezier3To(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3) :
   PSViewPathPart(PSVIEW_PATH_BEZIER3_TO), p1_(p1), p2_(p2), p3_(p3) {
  }

  PSViewPathBezier3To *dup() const override {
    return new PSViewPathBezier3To(*this);
  }

  void print(std::ostream &os) const override {
    os << "Bezier3To: " << p1_ << ", " << p2_ << ", " << p3_ << "\n";
  }

  void visit(PSViewPathVisitor &visitor) override {
    visitor.bezier3To(p1_, p2_, p3_);
  }

 private:
  CPoint2D p1_;
  CPoint2D p2_;
  CPoint2D p3_;
};

//---

class PSViewPathArcTo : public PSViewPathPart {
 public:
  PSViewPathArcTo(const CPoint2D &c, double xr, double yr, double theta, double delta) :
   PSViewPathPart(PSVIEW_PATH_ARC_TO), c_(c), xr_(xr), yr_(yr), theta_(theta), delta_(delta) {
  }

  PSViewPathArcTo *dup() const override {
    return new PSViewPathArcTo(*this);
  }

  void print(std::ostream &os) const override {
    os << "ArcTo: " << c_ << ", " << xr_ << ", " << yr_ << ", " <<
          theta_ << ", " << delta_ << "\n";
  }

  void visit(PSViewPathVisitor &visitor) override {
    visitor.arcTo(c_, xr_, yr_, theta_, delta_);
  }

 private:
  CPoint2D c_;
  double   xr_, yr_;
  double   theta_, delta_;
};

//---

class PSViewPathClose : public PSViewPathPart {
 public:
  PSViewPathClose() :
   PSViewPathPart(PSVIEW_PATH_CLOSE) {
  }

  PSViewPathClose *dup() const override {
    return new PSViewPathClose(*this);
  }

  void print(std::ostream &os) const override {
    os << "Close: \n";
  }

  void visit(PSViewPathVisitor &visitor) override {
    visitor.close();
  }
};

//---

class PSViewPath {
 public:
  PSViewPath();

  PSViewPath(const PSViewPath &path);

 ~PSViewPath();

  PSViewPath &operator=(const PSViewPath &path);

  PSViewPath *dup() const;

  void init();

  bool moveTo    (const CPoint2D &p);
  bool rmoveTo   (const CPoint2D &p);
  bool lineTo    (const CPoint2D &p);
  bool rlineTo   (const CPoint2D &p);
  bool bezier2To (const CPoint2D &p1, const CPoint2D &p2);
  bool rbezier2To(const CPoint2D &p1, const CPoint2D &p2);
  bool bezier3To (const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3);
  bool rbezier3To(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3);
  bool arc       (const CPoint2D &c, double xr, double yr, double a1, double a2);
  bool arcN      (const CPoint2D &c, double xr, double yr, double a1, double a2);
  bool arcTo     (const CPoint2D &p1, const CPoint2D &p2, double r1, double r2);

  bool addChar(int c, const CMatrix2D &m, double *x, double *y);

  void close();

  bool getCurrentPoint(CPoint2D &p);

  void flatten();
  void reverse();

  void setStrokeAdjust(bool);

  void getBounds(double *xmin, double *ymin, double *xmax, double *ymax);

  void process(PSViewPathVisitor &visitor);

  void print(std::ostream &os=std::cout) const;

 private:
  void reset();

 private:
  typedef std::vector<PSViewPathPart *> PartList;

  PartList parts_;
  bool     closed_;
  CPoint2D start_;
  CPoint2D current_;
  bool     current_set_;
};

#endif
