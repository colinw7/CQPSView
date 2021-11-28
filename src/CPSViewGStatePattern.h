#ifndef CPSViewGStatePattern_H
#define CPSViewGStatePattern_H

class PSViewGStatePattern {
 public:
  PSViewGStatePattern() :
   dictionary_  (NULL),
   pattern_type_(0),
   paint_type_  (0),
   tiling_type_ (0),
   x_step_      (0.0),
   y_step_      (0.0),
   paint_proc_  (NULL),
   gstate_      (NULL) {
    bbox_[0] = 0.0;
    bbox_[1] = 0.0;
    bbox_[2] = 0.0;
    bbox_[3] = 0.0;
  }

  PSViewGStatePattern(PSViewToken *dictionary, int pattern_type, int paint_type, int tiling_type,
                      double bbox[4], double x_step, double y_step,
                      PSViewToken *paint_proc, PSViewGState *gstate) :
   dictionary_  (dictionary),
   pattern_type_(pattern_type),
   paint_type_  (paint_type),
   tiling_type_ (tiling_type),
   x_step_      (x_step),
   y_step_      (y_step),
   paint_proc_  (paint_proc),
   gstate_      (gstate) {
    memcpy(bbox_, bbox, sizeof(bbox_));
  }

  PSViewGStatePattern(const PSViewGStatePattern &pattern) :
   dictionary_  (pattern.dictionary_),
   pattern_type_(pattern.pattern_type_),
   paint_type_  (pattern.paint_type_),
   tiling_type_ (pattern.tiling_type_),
   x_step_      (pattern.x_step_),
   y_step_      (pattern.y_step_),
   paint_proc_  (pattern.paint_proc_),
   gstate_      (pattern.gstate_) {
    memcpy(bbox_, pattern.bbox_, sizeof(bbox_));
  }

 ~PSViewGStatePattern() { }

  PSViewToken *getDictionary() const { return dictionary_; }

  const double *getBBox() const { return bbox_; }

  double getXStep() const { return x_step_; }
  double getYStep() const { return y_step_; }

  PSViewToken *getPaintProc() const { return paint_proc_; }

  PSViewGState *getGState() const { return gstate_; }

  void execute() {
    paint_proc_->execute();
  }

 private:
  PSViewGStatePattern &operator=(const PSViewGStatePattern &pattern);

 private:
  PSViewToken  *dictionary_ { nullptr };
  int           pattern_type_;
  int           paint_type_;
  int           tiling_type_;
  double        bbox_[4];
  double        x_step_;
  double        y_step_;
  PSViewToken  *paint_proc_ { nullptr };
  PSViewGState *gstate_     { nullptr };
};

#endif
