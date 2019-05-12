
#if !defined(MASK_H) 
    #define MASK_H 
    
    class Mask
    {
        protected:
            int _minValue;
            int _maxValue;
        
        public:
            Mask();
            Mask(int min, int max);
            ~Mask();

            int min();
            int max();
    }; 


    class MaskHSV
    {
        protected:
            Mask _H;
            Mask _S;
            Mask _V;
        
        public:
            MaskHSV();
            MaskHSV(Mask H, Mask S, Mask V);
            ~MaskHSV();

            Mask maskH();
            Mask maskS();
            Mask maskV();
    };

#endif