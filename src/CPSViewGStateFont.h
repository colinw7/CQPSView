#ifndef CPSViewGStateFont_H
#define CPSViewGStateFont_H

enum PSViewFontBuildType {
  PSVIEW_FONT_BUILD_TYPE_NONE,
  PSVIEW_FONT_BUILD_TYPE_GLYPH,
  PSVIEW_FONT_BUILD_TYPE_CHAR
};

class PSViewGStateFont0Data {
 public:
  PSViewGStateFont0Data() { }

  PSViewGStateFont0Data(const PSViewGStateFont0Data &font_data) :
   encoding_size(font_data.encoding_size),
   encoding     (font_data.encoding),
   map_type     (font_data.map_type),
   escape_char  (font_data.escape_char),
   num_fonts    (font_data.num_fonts),
   font_array   (),
   current_font (font_data.current_font) {
    font_array.resize(uint(num_fonts));

    for (int i = 0; i < num_fonts; ++i)
      font_array[uint(i)] = font_data.font_array[uint(i)]->dup();
  }

 private:
  PSViewGStateFont0Data &operator=(const PSViewGStateFont0Data &font_data);

 public:
  using Encoding  = std::vector<int>;
  using FontArray = std::vector<PSViewDictionaryToken *>;

  int       encoding_size { 0 };
  Encoding  encoding;
  int       map_type      { 0 };
  char      escape_char   { '\0' };
  int       num_fonts     { 0 };
  FontArray font_array;
  bool      current_font  { false };
};

//---

class PSViewGStateFont3Data {
 public:
  PSViewGStateFont3Data() { }

  PSViewGStateFont3Data(const PSViewGStateFont3Data &font_data) :
   matrix          (font_data.matrix),
   encoding        (nullptr),
   build           (nullptr),
   build_type      (font_data.build_type),
   cache_device_wx (font_data.cache_device_wx),
   cache_device_wy (font_data.cache_device_wy),
   cache_device_llx(font_data.cache_device_llx),
   cache_device_lly(font_data.cache_device_lly),
   cache_device_urx(font_data.cache_device_urx),
   cache_device_ury(font_data.cache_device_ury) {
    if (font_data.encoding != nullptr)
      encoding = font_data.encoding->dup();
    else
      encoding = nullptr;

    if (font_data.build != nullptr)
      build = font_data.build->dup();
    else
      build = nullptr;
  }

  void execute() {
    if (build != nullptr)
      build->execute();
  }

 private:
  PSViewGStateFont3Data &operator=(const PSViewGStateFont3Data &font_data);

 public:
  CMatrix2D           matrix;
  PSViewToken*        encoding         { nullptr };
  PSViewToken*        build            { nullptr };
  PSViewFontBuildType build_type       { PSVIEW_FONT_BUILD_TYPE_NONE };
  double              cache_device_wx  { 0.0 };
  double              cache_device_wy  { 0.0 };
  double              cache_device_llx { 0.0 };
  double              cache_device_lly { 0.0 };
  double              cache_device_urx { 0.0 };
  double              cache_device_ury { 0.0 };
};

//---

class PSViewGStateFontData {
 public:
  PSViewGStateFontData() {
    font0 = new PSViewGStateFont0Data;
    font3 = new PSViewGStateFont3Data;
  }

  PSViewGStateFontData(const PSViewGStateFontData &font_data) {
    if (font_data.font0)
      font0 = new PSViewGStateFont0Data(*font_data.font0);

    if (font_data.font3)
      font3 = new PSViewGStateFont3Data(*font_data.font3);
  }

  PSViewGStateFontData &operator=(const PSViewGStateFontData &font_data) {
    if (this == &font_data)
      return *this;

    if (font_data.font0)
      font0 = new PSViewGStateFont0Data(*font_data.font0);
    else
      font0 = nullptr;

    if (font_data.font3)
      font3 = new PSViewGStateFont3Data(*font_data.font3);
    else
      font3 = nullptr;

    return *this;
  }

 public:
  PSViewGStateFont0Data *font0 { nullptr };
  PSViewGStateFont3Data *font3 { nullptr };
};

//---

enum PSViewFontType {
  PSVIEW_FONT_TYPE_DEFAULT = -1,
  PSVIEW_FONT_TYPE_0       = 0,
  PSVIEW_FONT_TYPE_1       = 1,
  PSVIEW_FONT_TYPE_3       = 3
};

class PSViewGStateFont {
 public:
  PSViewGStateFont() :
   dictionary_(nullptr),
   type_      (PSVIEW_FONT_TYPE_DEFAULT),
   data_      () {
  }

  PSViewGStateFont(const PSViewGStateFont &font) :
   dictionary_(nullptr),
   type_      (font.type_),
   data_      (font.data_)  {
    if (font.dictionary_ != nullptr)
      dictionary_ = new PSViewDictionaryToken(*font.dictionary_);
    else
      dictionary_ = nullptr;
  }

 ~PSViewGStateFont() { }

  PSViewDictionaryToken *getDictionary() const { return dictionary_; }

  PSViewFontType getType() const { return type_; }

  PSViewGStateFont0Data *getFont0Data() const { return data_.font0; }
  PSViewGStateFont3Data *getFont3Data() const { return data_.font3; }

  void setDefaultFont(PSViewDictionaryToken *dictionary, CMatrix2D *matrix) {
    dictionary_ = dictionary;
    type_       = PSVIEW_FONT_TYPE_DEFAULT;

    //------

    PSViewGStateFont3Data *font_data = data_.font3;

    font_data->encoding   = nullptr;
    font_data->build      = nullptr;
    font_data->build_type = PSVIEW_FONT_BUILD_TYPE_NONE;
    font_data->matrix     = *matrix;
  }

  void setType1Font(PSViewDictionaryToken *dictionary, int encoding_size,
                    std::vector<int> &encoding, int map_type, char escape_char, int num_fonts,
                    std::vector<PSViewDictionaryToken *> &font_array) {
    dictionary_ = dictionary;
    type_       = PSVIEW_FONT_TYPE_0;

    PSViewGStateFont0Data *font_data = data_.font0;

    font_data->encoding_size = encoding_size;
    font_data->encoding      = encoding;
    font_data->map_type      = map_type;
    font_data->escape_char   = escape_char;
    font_data->num_fonts     = num_fonts;
    font_data->font_array    = font_array;
  }

  void setType3Font(PSViewDictionaryToken *dictionary, CMatrix2D *matrix, PSViewToken *encoding,
                    PSViewToken *build, PSViewFontBuildType build_type) {
    dictionary_ = dictionary;
    type_       = PSVIEW_FONT_TYPE_3;

    //------

    PSViewGStateFont3Data *font_data = data_.font3;

    font_data->encoding   = encoding;
    font_data->build      = build;
    font_data->build_type = build_type;
    font_data->matrix     = *matrix;
  }

 private:
  PSViewGStateFont &operator=(const PSViewGStateFont &font);

 private:
  PSViewDictionaryToken *dictionary_;
  PSViewFontType         type_;
  PSViewGStateFontData   data_;
};

#endif
