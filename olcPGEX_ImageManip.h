#ifndef OLC_PGEX_IMAGE_MANIP
#define OLC_PGEX_IMAGE_MANIP

namespace olc {

struct ImageManip : public olc::PGEX {
   static inline Sprite *Crop(Sprite *sprite, int left, int top, int width, int height);

   //static inline Sprite *Clone(Sprite *sprite);

   //static inline Sprite *FlipV(Sprite *sprite);
   //static inline Sprite *FlipH(Sprite *sprite);

   //template<typename Func> // Func = void(Pixel &pixel)
   //static inline Sprite *Filter(Func f, bool inplace = true);

   //template<typename Func> // Func = void(Pixel &pixel, int x, int y, Pixel get(int x, int y))
   //static inline Sprite *FilterComplex(Func f, bool inplace = true);
};

Sprite *ImageManip::Crop(Sprite *sprite, int left, int top, int width, int height){
    Sprite *result = new Sprite(width, height);

    for(int i = 0; i < height; i++)
    for(int j = 0; j < width;  j++){
        result->SetPixel(i, j, sprite->GetPixel(left + j, top + i));
    }

    return result;
}

}

#endif
